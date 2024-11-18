#pragma once
#include "text.h"
#include "../Abstracts/scale.h"
#include "../Abstracts/UIElement.h"

#define STANDART_BAR_WALL_WIDTH 5

////////////////////////////////////////////////////////////
// Class
////////////////////////////////////////////////////////////

template <typename T>
class Bar : public UIElement {
public:
    mutable sf::RectangleShape background, foreground, wall;
    mutable PlacedText ValueText;
    Scale<T>* value = nullptr;
    bool ShowWall = true, ShowBackground = true, ShowForeground = true, ShowText = true;
    float wallWidth = STANDART_BAR_WALL_WIDTH;

    Bar();
    Bar(std::string name);
    Bar(std::string name, UI::Anchor anchor, UI::Anchor anchoringPoint, sf::Vector2f = { 0, 0 });
    Bar(std::string name, sf::FloatRect rect);

    void setValue(Scale<T>& v) { value = &v; ValueText.setCenter(getCenter()); }
    void setWallWidth(float w);
    void setSize(float w, float h);
    void setPosition(float x, float y);
    void setPosition(sf::Vector2f v) { setPosition(v.x, v.y); }
    void setColors(sf::Color wallColor, sf::Color foregroundColor, sf::Color backgroundColor);
    void setString(sf::String s)     { ValueText.setString(s); }
    void setFontString(FontString s) { ValueText.setFontString(s); }
    void draw(sf::RenderTarget&, sf::RenderStates = sf::RenderStates::Default) const;
};

////////////////////////////////////////////////////////////
// Realization
////////////////////////////////////////////////////////////

template <class T>
Bar<T>::Bar() {
    setScale(scwScale, schScale);
    foreground.setPosition(wallWidth, wallWidth);
    background.setPosition(wallWidth, wallWidth);
}

template <class T>
Bar<T>::Bar(std::string name) : Bar() {
    setName(name);
}

template <class T>
Bar<T>::Bar(std::string name, UI::Anchor anchor, UI::Anchor anchoringPoint, sf::Vector2f size) : Bar(name) {
    setAnchors(anchor, anchoringPoint);
    setRect(0, 0, size.x, size.y);
    setSize(size.x, size.y);
}

template <class T>
Bar<T>::Bar(std::string name, sf::FloatRect rect) : Bar(name, UI::none, UI::none, { rect.width, rect.height }) {
    setPosition(rect.left, rect.top);
}

template <typename T>
void Bar<T>::setWallWidth(float w) {
    wallWidth = w;
    setSize(wall.getLocalBounds().width, wall.getLocalBounds().height);
    foreground.setPosition(wallWidth, wallWidth);
    background.setPosition(wallWidth, wallWidth);
}

template <typename T>
void Bar<T>::setSize(float w, float h) {
    UIElement::setSize(w, h);
    wall.setSize({w, h});
    background.setSize({w - wallWidth * 2.f, h - wallWidth * 2.f});
    foreground.setSize({w - wallWidth * 2.f, h - wallWidth * 2.f});
}

template <typename T>
void Bar<T>::setPosition(float x, float y) {
    sf::Vector2f prevPos = getPosition();
    UIElement::setPosition(x, y);
    ValueText.setCenter(getCenter());
}

template <typename T>
void Bar<T>::setColors(sf::Color wallColor, sf::Color foregroundColor, sf::Color backgroundColor) {
    wall.setFillColor(wallColor);
    background.setFillColor(backgroundColor);
    foreground.setFillColor(foregroundColor);
}

template <typename T>
void Bar<T>::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    if (value) {
        foreground.setScale(value->filling(), 1);
        ValueText.setString(std::to_string((int)value->cur) + " / " + std::to_string((int)value->top));
        ValueText.setCenter(getCenter());
    }
    if (ShowWall) target.draw(wall, states.transform * getTransform());
    if (ShowBackground) target.draw(background, states.transform * getTransform());
    if (ShowForeground) target.draw(foreground, states.transform * getTransform());
    if (ShowText) target.draw(ValueText, states);
}
