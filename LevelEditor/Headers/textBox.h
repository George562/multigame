#pragma once
#include "../../SFML-2.5.1/include/SFML/Graphics.hpp"
#include "interactionRect.h"
#include "mouseTools.h"

class TextBox : public InteractionRect
{
protected:
    bool readInput = false, shiftPressed = false, ctrlPressed = false;
    float initialWidth;

    std::string fullString;

    sf::Color inactiveColor = sf::Color(200, 200, 200);
    sf::Color activeColor = sf::Color::White;
    sf::Text text;

public:
    TextBox() {}
    TextBox(float, float, float, float, sf::Font&);

    virtual std::string getText() { return fullString; }

    void move(float x, float y) override
    { 
        posX += x; posY += y; 
        drawRect.setPosition(posX, posY);
        text.setPosition(text.getPosition().x + x, text.getPosition().y + y);
    }

    bool isActivated(sf::Event&) override;
    void draw(sf::RenderTarget&, sf::RenderStates) const override;
};

TextBox::TextBox(float _posX, float _posY, float _width, float _height, sf::Font& font) : InteractionRect(_posX, _posY, _width, _height)
{
    drawRect.setOutlineColor(sf::Color::Black); drawRect.setOutlineThickness(2);
    drawRect.setFillColor(inactiveColor);

    text = sf::Text("", font, std::min(width * 9 / 10, height * 9 / 10));
    text.setPosition(drawRect.getPosition() + sf::Vector2f(5, 0));
    text.setFillColor(sf::Color::Black);

    initialWidth = width;
}

bool TextBox::isActivated(sf::Event& event)
{
    if(event.type == sf::Event::KeyPressed && (event.key.code == sf::Keyboard::LControl || event.key.code == sf::Keyboard::RControl))
    {
        ctrlPressed = true;
        return false;
    }

    if(event.type == sf::Event::KeyPressed && (event.key.code == sf::Keyboard::LShift || event.key.code == sf::Keyboard::RShift))
    {
        shiftPressed = true;
        return false;
    }

    if(event.type == sf::Event::KeyReleased && (event.key.code == sf::Keyboard::LControl || event.key.code == sf::Keyboard::RControl))
    {
        ctrlPressed = false;
        return false;
    }
    
    if(event.type == sf::Event::KeyReleased && (event.key.code == sf::Keyboard::LShift || event.key.code == sf::Keyboard::RShift))
    {
        shiftPressed = false;
        return false;
    }

    if(event.type == sf::Event::KeyPressed && readInput)
    {
        if(event.key.code == sf::Keyboard::Backspace && text.getString().getSize() != 0)
            text.setString(text.getString().substring(0, text.getString().getSize() - 1));

        if(event.key.code >= sf::Keyboard::A && event.key.code <= sf::Keyboard::Z)
            text.setString(text.getString() + (char)((shiftPressed ? 'A' : 'a') + event.key.code));

        if(event.key.code >= sf::Keyboard::Num0 && event.key.code <= sf::Keyboard::Num9)
            text.setString(text.getString() + (char)('0' + event.key.code - sf::Keyboard::Num0));

        if(event.key.code == sf::Keyboard::Hyphen)
            text.setString(text.getString() + (char)(shiftPressed ? '_' : '-'));

        if(event.key.code == sf::Keyboard::Semicolon)
            text.setString(text.getString() + (char)(';' - shiftPressed));
        
        if(event.key.code == sf::Keyboard::Backslash && !shiftPressed)
            text.setString(text.getString() + '\\');

        if(event.key.code == sf::Keyboard::Slash && !shiftPressed)
            text.setString(text.getString() + '/');

        if(event.key.code == sf::Keyboard::V && ctrlPressed)
        {
            if(text.getString().getSize() != 0)
                text.setString(text.getString().substring(0, text.getString().getSize() - 1));

            text.setString(sf::Clipboard::getString());
        }

        if(event.key.code == sf::Keyboard::C && ctrlPressed)
            sf::Clipboard::setString(text.getString());
        
        fullString = (std::string)text.getString();
        width = std::max((int)initialWidth, (int)(text.getGlobalBounds().width + text.getCharacterSize() / 3));
        drawRect.setSize(sf::Vector2f(width, height));
        return true;
    }

    if(event.type == sf::Event::MouseButtonPressed)
    {
        readInput = in(posX, posY, width, height, event.mouseButton);
        if(!readInput)
        {
            width = initialWidth;
            drawRect.setSize(sf::Vector2f(width, height));
            text.setString(text.getString().substring(0, initialWidth / (text.getCharacterSize() * 2 / 3)));
        }
        else
        {
            text.setString(fullString);
            width = std::max((int)initialWidth, (int)(text.getGlobalBounds().width + text.getCharacterSize() / 3));
            drawRect.setSize(sf::Vector2f(width, height));
        }
        drawRect.setFillColor(readInput ? activeColor : inactiveColor);
        return true;
    }

    return false;
}

void TextBox::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    target.draw(drawRect);
    target.draw(text);
}