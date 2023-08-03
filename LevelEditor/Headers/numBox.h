#pragma once
#include "../../SFML-2.5.1/include/SFML/Graphics.hpp"
#include "textBox.h"

class NumBox : public TextBox
{
private:
    int numLength = 1;

public:
    NumBox() {}
    NumBox(float, float, float, float, sf::Font&, int);

    std::string getText() override { return text.getString(); }

    bool isActivated(sf::Event&) override;
};

NumBox::NumBox(float _posX, float _posY, float _width, float _height, sf::Font& font, int length) : TextBox(_posX, _posY, _width, _height, font)
{
    numLength = length;
}

bool NumBox::isActivated(sf::Event& event)
{
    if(event.type == sf::Event::KeyPressed && readInput)
    {
        if(event.key.code == sf::Keyboard::Backspace && text.getString().getSize() != 0)
            text.setString(text.getString().substring(0, text.getString().getSize() - 1));

        if(event.key.code >= sf::Keyboard::Num0 && event.key.code <= sf::Keyboard::Num9 && text.getString().getSize() <= numLength - 1)
            text.setString(text.getString() + (char)('0' + event.key.code - 26));

        return true;
    }

    if(event.type == sf::Event::MouseButtonPressed)
    {
        readInput = in(posX, posY, width, height, event.mouseButton);
        drawRect.setFillColor(readInput ? activeColor : inactiveColor);
        return true;
    }
    
    return false;
}