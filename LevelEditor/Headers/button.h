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
    Button(float, float, float, float, sf::Font&);

    int getCharacterSize() { return text.getCharacterSize(); }

    void setText(std::string str) { text.setString(str); fitText(); centerText(); }
    void centerText();
    void fitText();
    void setPressColor(sf::Color color) { pressColor = color; setFillColor(pressed ? pressColor : inactiveColor); }
    void setInactiveColor(sf::Color color) { inactiveColor = color; setFillColor(pressed ? pressColor : inactiveColor); }

    bool isActivated(sf::Event&) override;
    void draw(sf::RenderTarget&, sf::RenderStates) const override;
};

Button::Button(float _posX, float _posY, float _width, float _height, sf::Font& font) : InteractionRect(_posX, _posY, _width, _height)
{
    setOutlineColor(sf::Color::Black); setOutlineThickness(2);
    setFillColor(inactiveColor);

    text = sf::Text("", font, std::min(width * 8 / 10, height * 8 / 10));
    text.setFillColor(sf::Color::Black);
    centerText();
}

bool Button::isActivated(sf::Event& event)
{
    if(event.type == sf::Event::MouseButtonPressed)
    {
        pressed = in(*this, event.mouseButton);
        setFillColor(pressed ? pressColor : inactiveColor);
        return false;
    }

    if(event.type == sf::Event::MouseButtonReleased && pressed)
    {
        pressed = false;
        setFillColor(inactiveColor);
        return in(*this, event.mouseButton);
    }

    return false;
}

void Button::centerText()
{
    if(text.getString().isEmpty())
        return;

    text.setPosition(width / 2.f, height / 2.f);
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
    states.transform *= getTransform();
    target.draw(drawRect, states);
    target.draw(text, states);
}