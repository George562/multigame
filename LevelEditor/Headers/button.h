#pragma once
#include "../../SFML-2.5.1/include/SFML/Graphics.hpp"
#include "interactionRect.h"
#include "stringTools.h"
#include "mouseTools.h"

class Button : public InteractionRect
{
private:
    bool pressed = false;

    sf::Color pressColor = sf::Color(150, 150, 150);
    sf::Color inactiveColor = sf::Color::White;

    sf::Text text;

public:
    Button() {}
    Button(float, float, int, int, sf::Font&);

    int getCharacterSize() { return text.getCharacterSize(); }

    void setText(std::string str) { text.setString(str); fitText(); centerText(); }
    void centerText();
    void fitText();
    void setPressColor(sf::Color color) { pressColor = color; drawRect.setFillColor(pressed ? pressColor : inactiveColor); }
    void setInactiveColor(sf::Color color) { inactiveColor = color; drawRect.setFillColor(pressed ? pressColor : inactiveColor); }

    void move(float x, float y) override 
    { 
        posX += x; posY += y; 
        drawRect.setPosition(posX, posY); 
        text.setPosition(text.getPosition().x + x, text.getPosition().y + y);
    }

    bool isActivated(sf::Event&) override;
    void draw(sf::RenderTarget&, sf::RenderStates) const override;
};

Button::Button(float _posX, float _posY, int _width, int _height, sf::Font& font) : InteractionRect(_posX, _posY, _width, _height)
{
    drawRect.setOutlineColor(sf::Color::Black); drawRect.setOutlineThickness(2);
    drawRect.setFillColor(inactiveColor);

    text = sf::Text("", font, std::min(width * 8 / 10, height * 8 / 10));
    text.setPosition(drawRect.getPosition());
    text.setFillColor(sf::Color::Black);
    centerText();
}

bool Button::isActivated(sf::Event& event)
{
    if(event.type == sf::Event::MouseButtonPressed)
    {
        pressed = in(posX, posY, width, height, event.mouseButton);
        drawRect.setFillColor(pressed ? pressColor : inactiveColor);
        return false;
    }
    
    if(event.type == sf::Event::MouseButtonReleased && pressed)
    {
        pressed = false;
        drawRect.setFillColor(inactiveColor);
        return in(posX, posY, width, height, event.mouseButton);
    }

    return false;
}

void Button::centerText()
{
    if(text.getString().isEmpty())
        return;

    text.setPosition(posX + width / 2.f, posY + height / 2.f);
    text.setOrigin(text.getLocalBounds().left + text.getLocalBounds().width / 2.f, text.getLocalBounds().top + text.getLocalBounds().height / 2.f);
}

void Button::fitText()
{
    if(text.getString().isEmpty())
        return;

    std::vector<std::string> lines = splitString(text, '\n');
    int maxLength = 0;

    for(std::string s : lines)
        maxLength = s.size() > maxLength ? s.size() : maxLength;

    if(text.getGlobalBounds().width > width)
        text.setCharacterSize(2 * width / maxLength);

    while(width - text.getGlobalBounds().width <= 30 || height - text.getGlobalBounds().height <= 30)
        text.setCharacterSize(text.getCharacterSize() - 1);
}

void Button::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    target.draw(drawRect);
    target.draw(text);
}