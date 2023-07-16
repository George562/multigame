#pragma once
#include "../../SFML-2.5.1/include/SFML/Graphics.hpp"

class TextBox : public sf::Drawable
{
private:
    float posX, posY;
    int width, height;
    bool readInput = false;
    sf::Color inactiveColor = sf::Color(200, 200, 200);
    sf::Color activeColor = sf::Color::White;
    sf::RectangleShape boxRect;
    sf::Text text;

public:
    TextBox() {}
    TextBox(float, float, int, int, sf::Font&);
    float getX() { return posX; }
    float getY() { return posY; }
    int getWidth() { return width; }
    int getHeight() { return height; }
    bool isActivated(sf::Event&);
    std::string getText();
    void draw(sf::RenderTarget&, sf::RenderStates) const;
};

TextBox::TextBox(float _posX, float _posY, int _width, int _height, sf::Font& font)
{
    posX = _posX; posY = _posY;
    width = _width; height = _height;

    boxRect = sf::RectangleShape(sf::Vector2f(width, height));
    boxRect.setPosition(posX, posY); boxRect.setSize(sf::Vector2f(width, height));
    boxRect.setOutlineColor(sf::Color::Black); boxRect.setOutlineThickness(3);
    boxRect.setFillColor(inactiveColor);

    text = sf::Text("", font, 40);
    text.setPosition(boxRect.getPosition() + sf::Vector2f(5, 0));
    text.setFillColor(sf::Color::Black);
}

bool TextBox::isActivated(sf::Event& event)
{
    if(event.type == sf::Event::KeyPressed && readInput)
    {
        if(event.key.code == sf::Keyboard::Backspace && text.getString().getSize() != 0)
        {
            text.setString(text.getString().substring(0, text.getString().getSize() - 1));
            return true;
        }

        if(event.key.code >= sf::Keyboard::A && event.key.code <= sf::Keyboard::Z)
        {
            text.setString(text.getString() + (char)('a' + event.key.code));
            return true;
        }

        if(event.key.code >= sf::Keyboard::Num0 && event.key.code <= sf::Keyboard::Num9)
        {
            text.setString(text.getString() + (char)('0' + event.key.code - sf::Keyboard::Num0));
            return true;
        }
    }

    if(event.type == sf::Event::MouseButtonPressed)
    {
        readInput = event.mouseButton.x >= posX && event.mouseButton.x <= posX + width && event.mouseButton.y >= posY && event.mouseButton.y <= posY + height;
        boxRect.setFillColor(readInput ? activeColor : inactiveColor);
        return true;
    }

    return false;
}

std::string TextBox::getText()
{
    return text.getString();
}

void TextBox::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    target.draw(boxRect);
    target.draw(text);
}