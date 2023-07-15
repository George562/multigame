#pragma once
#include "../../SFML-2.5.1/include/SFML/Graphics.hpp"

class Button : sf::Drawable
{
private:
    float posX, posY;
    int width, height;
    bool pressed = false;
    sf::Color pressColor = sf::Color(150, 150, 150);
    sf::Color inactiveColor = sf::Color::White;
    sf::RectangleShape buttonRect;
    sf::Font font;
    sf::Text text;

public:
    Button(float, float, int, int);
    float getX() { return posX; }
    float getY() { return posY; }
    int getWidth() { return width; }
    int getHeight() { return height; }
    bool isActivated(sf::Event&);
    void draw(sf::RenderTarget&, sf::RenderStates) const;
    void setText(std::string str) { text.setString(str); }
};

Button::Button(float _posX, float _posY, int _width, int _height)
{
    posX = _posX; posY = _posY;
    width = _width; height = _height;

    buttonRect = sf::RectangleShape(sf::Vector2f(width, height));
    buttonRect.setPosition(posX, posY); buttonRect.setSize(sf::Vector2f(width, height));
    buttonRect.setOutlineColor(sf::Color::Black); buttonRect.setOutlineThickness(3);
    buttonRect.setFillColor(inactiveColor);

    font.loadFromFile("Resources/Fonts/arial.ttf");

    text = sf::Text("", font, 30);
    text.setPosition(buttonRect.getPosition() + sf::Vector2f(5, 0));
    text.setFillColor(sf::Color::Black);
}

bool Button::isActivated(sf::Event& event)
{
    if(event.mouseButton.x >= posX && event.mouseButton.x <= posX + width && event.mouseButton.y >= posY && event.mouseButton.y <= posY + height)
    {
        if(event.type == sf::Event::MouseButtonPressed)
        {
            pressed = true;
            buttonRect.setFillColor(pressColor);
            return false;
        }
        if(event.type == sf::Event::MouseButtonReleased)
        {
            pressed = false;
            buttonRect.setFillColor(inactiveColor);
            return true;
        }
    }
    return false;
}

void Button::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    target.draw(buttonRect, states);
    target.draw(text, states);
}