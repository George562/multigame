#pragma once
#include "../../SFML-2.5.1/include/SFML/Graphics.hpp"
#include "interactionRect.h"
#include "mouseTools.h"

class TextBox : public InteractionRect
{
protected:
    bool readInput = false;
    float initialWidth;

    std::string fullString;

    sf::Color inactiveColor = sf::Color(200, 200, 200);
    sf::Color activeColor = sf::Color::White;
    sf::Text text;

public:
    TextBox() {}
    TextBox(float, float, float, float, sf::Font&);

    virtual std::string getText() { return fullString; }
    virtual void setText(std::string newText) { text.setString(newText); fullString = newText; }

    bool isActivated(sf::Event&) override;
    void draw(sf::RenderTarget&, sf::RenderStates) const override;
};

TextBox::TextBox(float _posX, float _posY, float _width, float _height, sf::Font& font) : InteractionRect(_posX, _posY, _width, _height)
{
    setOutlineColor(sf::Color::Black); setOutlineThickness(2);
    setFillColor(inactiveColor);

    text = sf::Text("", font, std::min(width * 9 / 10, height * 9 / 10));
    text.move(5, 0);
    text.setFillColor(sf::Color::Black);

    initialWidth = width;
}

bool TextBox::isActivated(sf::Event& event)
{
    if(event.type == sf::Event::KeyPressed && readInput)
    {
        if(event.key.code == sf::Keyboard::V && event.key.control)
        {
            if(text.getString().getSize() != 0)
                text.setString(text.getString().substring(0, text.getString().getSize() - 1));

            text.setString(sf::Clipboard::getString());
        }
        else if(event.key.code == sf::Keyboard::C && event.key.control)
            sf::Clipboard::setString(text.getString());

        else if(event.key.code == sf::Keyboard::Backspace && text.getString().getSize() != 0)
            text.setString(text.getString().substring(0, text.getString().getSize() - 1));

        else if(event.key.code >= sf::Keyboard::A && event.key.code <= sf::Keyboard::Z)
            text.setString(text.getString() + (char)((event.key.shift ? 'A' : 'a') + event.key.code));

        else if(event.key.code >= sf::Keyboard::Num0 && event.key.code <= sf::Keyboard::Num9)
            text.setString(text.getString() + (char)('0' + event.key.code - sf::Keyboard::Num0));

        else if(event.key.code >= sf::Keyboard::Numpad0 && event.key.code <= sf::Keyboard::Numpad9)
            text.setString(text.getString() + (char)('0' + event.key.code - sf::Keyboard::Numpad0));

        else if(event.key.code == sf::Keyboard::Hyphen)
            text.setString(text.getString() + (char)(event.key.shift ? '_' : '-'));

        else if(event.key.code == sf::Keyboard::Semicolon)
            text.setString(text.getString() + (char)(';' - event.key.shift));

        else if(event.key.code == sf::Keyboard::Backslash && !event.key.shift)
            text.setString(text.getString() + '\\');

        else if(event.key.code == sf::Keyboard::Slash && !event.key.shift)
            text.setString(text.getString() + '/');

        fullString = (std::string)text.getString();
        setSize(std::max((int)initialWidth, (int)(text.getGlobalBounds().width + text.getCharacterSize() / 3)), height);
        return true;
    }

    if(event.type == sf::Event::MouseButtonPressed)
    {
        readInput = in(*this, event.mouseButton);
        if(!readInput)
        {
            setSize(initialWidth, height);
            text.setString(text.getString().substring(0, initialWidth / (text.getCharacterSize() * 2 / 3)));
        }
        else
        {
            text.setString(fullString);
            setSize(std::max((int)initialWidth, (int)(text.getGlobalBounds().width + text.getCharacterSize() / 3)), height);
        }
        setFillColor(readInput ? activeColor : inactiveColor);
        return true;
    }

    return false;
}

void TextBox::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    states.transform *= getTransform();
    target.draw(drawRect, states);
    target.draw(text, states);
}