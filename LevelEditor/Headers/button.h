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

    void setText(std::string str) { text.setString(str); fitText(); centerText(); }
    void centerText();
    void fitText();
    void scaleText(float);

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

    text = sf::Text("", font, std::min(width * 9 / 10, height * 9 / 10));
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
        return pressed;
    }
    
    if(event.type == sf::Event::MouseButtonReleased)
    {
        pressed = in(posX, posY, width, height, event.mouseButton);
        drawRect.setFillColor(inactiveColor);
        return pressed;
    }

    return false;
}

void Button::centerText()
{
    text.setPosition(drawRect.getPosition().x + drawRect.getGlobalBounds().width / 2 - text.getGlobalBounds().width / 2,
                     drawRect.getPosition().y + drawRect.getGlobalBounds().height / 2 - text.getGlobalBounds().height * 3 / 4);
}

void Button::fitText()
{
    std::vector<std::string> lines = splitString(text, '\n');
    int maxLength = 0;

    for(std::string s : lines)
    {
        if(s.size() > maxLength)
            maxLength = s.size();
    }
    
    text.setCharacterSize(width / maxLength);
}

void Button::scaleText(float coef)
{
    text.setCharacterSize(text.getCharacterSize() * coef);
}

void Button::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    target.draw(drawRect);
    target.draw(text);
}