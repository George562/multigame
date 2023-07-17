#pragma once
#include "../../SFML-2.5.1/include/SFML/Graphics.hpp"
#include "interactionRect.h"

class Label : public InteractionRect
{
private:
    sf::Text text;

public:
    Label() {}
    Label(float, float, int, int, sf::Font&);

    std::string getText() { return text.getString(); }
    void setText(std::string val) { text.setString(val); }
    void setCharacterSize(int val) { text.setCharacterSize(val); }

    void move(float x, float y) override
    { 
        posX += x; posY += y; 
        drawRect.setPosition(posX, posY);
        text.setPosition(text.getPosition().x + x, text.getPosition().y + y);
    }

    void draw(sf::RenderTarget&, sf::RenderStates) const override;
};

Label::Label(float _posX, float _posY, int _width, int _height, sf::Font& font) : InteractionRect(_posX, _posY, _width, _height) 
{
    text = sf::Text("", font, std::min(width * 9 / 10, height * 9 / 10));
    text.setPosition(drawRect.getPosition() + sf::Vector2f(5, 0));
    text.setFillColor(sf::Color::Black);
}

void Label::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    target.draw(text, states);
}