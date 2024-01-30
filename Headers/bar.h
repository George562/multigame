#pragma once
#include "text.h"
#include "scale.h"

#define STANDART_BAR_WALL_WIDTH 5

////////////////////////////////////////////////////////////
// Class
////////////////////////////////////////////////////////////

template <typename T>
class Bar : public sf::Drawable, public sf::Transformable {
public:
    mutable sf::RectangleShape background, foreground, wall;
    mutable PlacedText ValueText;
    Scale<T>* value = nullptr;
    bool ShowWall = true, ShowBackground = true, ShowForeground = true, ShowText = true;
    float wallWidth = STANDART_BAR_WALL_WIDTH;

    Bar() { foreground.setPosition(wallWidth, wallWidth); background.setPosition(wallWidth, wallWidth); }
    void setValue(Scale<T>& v) { value = &v; }
    sf::Vector2f getSize() const { return wall.getSize(); }
    void setWallWidth(float w);
    void setSize(float w, float h);
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
    foreground.setPosition(wallWidth, wallWidth);
    background.setPosition(wallWidth, wallWidth);
}

template <typename T>
void Bar<T>::setSize(float w, float h) {
    wall.setSize({w, h});
    background.setSize({w - wallWidth * 2.f, h - wallWidth * 2.f});
    foreground.setSize({w - wallWidth * 2.f, h - wallWidth * 2.f});
}

template <typename T>
void Bar<T>::setColors(sf::Color wallColor, sf::Color foregroundColor, sf::Color backgroundColor) {
    wall.setFillColor(wallColor);
    background.setFillColor(backgroundColor);
    foreground.setFillColor(foregroundColor);
}

template <typename T>
void Bar<T>::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    states.transform *= getTransform();
    if (ShowWall) target.draw(wall, states);
    if (ShowBackground) target.draw(background, states);
    if (ShowForeground) target.draw(foreground, states);

    if (value && ShowText) {
        foreground.setScale(value->filling(), 1);
        ValueText.setString(std::to_string((int)value->cur) + " / " + std::to_string((int)value->top));
        ValueText.setCenter(getSize() / 2.f);
        target.draw(ValueText, states);
    }
}
