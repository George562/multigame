#pragma once
#include "../../SFML-2.5.1/include/SFML/Graphics.hpp"

class PushTile : sf::Drawable
{
private:
    float posX, posY;
    int width, height, state = 0;
    bool readPosition = false;
    int lastButton;
    sf::Color activeColor = sf::Color(0, 175, 100);
    sf::Color inactiveColor = sf::Color::White;
    sf::RectangleShape tileRect;
    sf::Font font;
    sf::Text text;

public:
    PushTile() {}
    PushTile(float, float, int, int);
    float getX() { return posX; }
    float getY() { return posY; }
    int getWidth() { return width; }
    int getHeight() { return height; }
    int getState() { return state; }
    bool isActivated(sf::Event&);
    void setState(int val);
    bool in(sf::Event::MouseButtonEvent&);
    bool in(sf::Event::MouseMoveEvent&);
    void centerText();
    void draw(sf::RenderTarget&, sf::RenderStates) const;
};

PushTile::PushTile(float _posX, float _posY, int _width, int _height)
{
    posX = _posX; posY = _posY;
    width = _width; height = _height;

    tileRect = sf::RectangleShape(sf::Vector2f(width, height));
    tileRect.setPosition(posX, posY); tileRect.setSize(sf::Vector2f(width, height));
    tileRect.setOutlineColor(sf::Color::Black); tileRect.setOutlineThickness(2);
    tileRect.setFillColor(inactiveColor);

    font.loadFromFile("Resources/Fonts/arial.ttf");

    text = sf::Text((char)('0' + state), font, std::min(width / 2, height / 2));
    text.setPosition(tileRect.getPosition());
    text.setFillColor(sf::Color::Black);
    centerText();
}

bool PushTile::isActivated(sf::Event& event)
{
    if(event.type == sf::Event::MouseButtonPressed)
    {
        readPosition = true;
        lastButton = event.mouseButton.button;
        return in(event.mouseButton);
    }
    if(readPosition && event.type == sf::Event::MouseButtonReleased)
    {
        readPosition = false;
        if(in(event.mouseButton))
        {
            if(lastButton == sf::Mouse::Left)
            {
                setState(1);
                text.setString(std::to_string(1));
                tileRect.setFillColor(activeColor);
            }
            else if(lastButton == sf::Mouse::Right)
            {
                setState(0);
                text.setString(std::to_string(0));
                tileRect.setFillColor(inactiveColor);
            }
            return in(event.mouseButton);
        }
    }

    if(readPosition && in(event.mouseMove))
    {
        readPosition = false;
        if(lastButton == sf::Mouse::Left)
        {
            setState(1);
            text.setString(std::to_string(1));
            tileRect.setFillColor(activeColor);
        }
        else if(lastButton == sf::Mouse::Right)
        {
            setState(0);
            text.setString(std::to_string(0));
            tileRect.setFillColor(inactiveColor);
        }
        return true;
    }

    return false;
}

void PushTile::setState(int val)
{
    state = val;
}

bool PushTile::in(sf::Event::MouseButtonEvent& event)
{
    return event.x >= posX && event.x <= posX + width && event.y >= posY && event.y <= posY + height;
}

bool PushTile::in(sf::Event::MouseMoveEvent& event)
{
    return event.x >= posX && event.x <= posX + width && event.y >= posY && event.y <= posY + height;
}

void PushTile::centerText()
{
    text.setPosition(tileRect.getPosition().x + tileRect.getGlobalBounds().width / 2 - text.getGlobalBounds().width / 2,
                     tileRect.getPosition().y + tileRect.getGlobalBounds().height / 2 - text.getGlobalBounds().height * 2 / 3);
}

void PushTile::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    target.draw(tileRect);
    target.draw(text);
}