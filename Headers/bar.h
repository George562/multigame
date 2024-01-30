#pragma once
#include "text.h"
#include "scale.h"

#define STANDART_BAR_HEIGHT 50

////////////////////////////////////////////////////////////
// Class
////////////////////////////////////////////////////////////

template <typename T>
class Bar : public sf::Drawable {
public:
    mutable sf::RectangleShape background, foreground, wall;
    mutable PlacedText ValueText;
    Scale<T>* value = nullptr;
    bool ShowWall = true, ShowBackground = true, ShowForeground = true, ShowText = true;
    float wallWidth = 10;

    Bar() {}
    void setValue(Scale<T>& v) { value = &v; }
    sf::Vector2f getSize() const { return wall.getSize(); }
    void setWallWidth(float w);
    void setWidth(float w) { setSize(w, STANDART_BAR_HEIGHT); }
    void setSize(float w, float h);
    sf::Vector2f getPosition() const { return wall.getPosition(); }
    void setPosition(float x, float y);
    void setPosition(sf::Vector2f v) { setPosition(v.x, v.y); }
    void setColors(sf::Color wallColor, sf::Color foregroundColor, sf::Color backgroundColor);
    void draw(sf::RenderTarget&, sf::RenderStates = sf::RenderStates::Default) const;
};

////////////////////////////////////////////////////////////
// Realization
////////////////////////////////////////////////////////////

template <typename T>
void Bar<T>::setWallWidth(float w) {
    wallWidth = w;
    setSize(wall.getLocalBounds().width, wall.getLocalBounds().height);
}

template <typename T>
void Bar<T>::setSize(float w, float h) {
    wall.setSize({w, h});
    background.setSize({w - wallWidth, h - wallWidth});
    foreground.setSize({w - wallWidth, h - wallWidth});
}

template <typename T>
void Bar<T>::setPosition(float x, float y) {
    wall.setPosition(x, y);
    background.setPosition(x + wallWidth / 2.f, y + wallWidth / 2.f);
    foreground.setPosition(x + wallWidth / 2.f, y + wallWidth / 2.f);
    ValueText.setCenter(getPosition() + getSize() / 2.f);
}

template <typename T>
void Bar<T>::setColors(sf::Color wallColor, sf::Color foregroundColor, sf::Color backgroundColor) {
    wall.setFillColor(wallColor);
    background.setFillColor(backgroundColor);
    foreground.setFillColor(foregroundColor);
}

template <typename T>
void Bar<T>::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    if (value != nullptr) {
        foreground.setScale(value->filling(), 1);
        ValueText.setString(std::to_string((int)value->cur) + " / " + std::to_string((int)value->top));
        ValueText.setCenter(getPosition() + getSize() / 2.f);
    }
    if (ShowWall) target.draw(wall, states);
    if (ShowBackground) target.draw(background, states);
    if (ShowForeground) target.draw(foreground, states);
    if (ShowText) target.draw(ValueText, states);
}
