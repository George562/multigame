#pragma once
#include "text.h"
#include "scale.h"

#define STANDART_BAR_HEIGHT 50
#define STANDART_BAR_WALL_WIDTH 10

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

    Bar() {}
    void setValue(Scale<T>& v) { value = &v; }
    sf::Vector2f getSize() const { return wall.getSize(); }
    void setWidth(float);
    void setSize(float, float);
    sf::Vector2f getPosition() const { return wall.getPosition(); }
    void setPosition(float, float);
    void setColors(sf::Color wallColor, sf::Color foregroundColor, sf::Color backgroundColor);
    void draw(sf::RenderTarget&, sf::RenderStates = sf::RenderStates::Default) const;
};

////////////////////////////////////////////////////////////
// Realization
////////////////////////////////////////////////////////////

template <typename T>
void Bar<T>::setWidth(float w) {
    wall.setSize({w, STANDART_BAR_HEIGHT});
    background.setSize({w - STANDART_BAR_WALL_WIDTH, STANDART_BAR_HEIGHT - STANDART_BAR_WALL_WIDTH});
    foreground.setSize({w - STANDART_BAR_WALL_WIDTH, STANDART_BAR_HEIGHT - STANDART_BAR_WALL_WIDTH});
}
template <typename T>
void Bar<T>::setSize(float w, float h) {
    wall.setSize({w, h});
    background.setSize({w - STANDART_BAR_WALL_WIDTH, h - STANDART_BAR_WALL_WIDTH});
    foreground.setSize({w - STANDART_BAR_WALL_WIDTH, h - STANDART_BAR_WALL_WIDTH});
}

template <typename T>
void Bar<T>::setPosition(float x, float y) {
    wall.setPosition(x, y);
    background.setPosition(x + 5, y + 5);
    foreground.setPosition(x + 5, y + 5);
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
