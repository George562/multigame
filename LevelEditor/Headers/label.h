#pragma once
#include "../../SFML-2.5.1/include/SFML/Graphics.hpp"
#include "interactionRect.h"

class Label : public InteractionRect
{
private:
    sf::Text text;
    sf::Font font;

public:
    Label() {}
    Label(float, float, int, sf::Font&);

    int getCharacterSize() { return text.getCharacterSize(); }

    std::string getText() { return text.getString(); }
    void setText(std::string val) { text.setString(val); width = text.getGlobalBounds().width; height = text.getGlobalBounds().height; }
    void setCharacterSize(int val) { text.setCharacterSize(val); width = text.getGlobalBounds().width; height = text.getGlobalBounds().height; }

    void move(float x, float y) override
    { 
        posX += x; posY += y; 
        drawRect.setPosition(posX, posY);
        text.setPosition(text.getPosition().x + x, text.getPosition().y + y);
    }

    void draw(sf::RenderTarget&, sf::RenderStates) const override;
};

Label::Label(float _posX, float _posY, int characterSize, sf::Font& _font) : InteractionRect(_posX, _posY, 0, 0) 
{
    drawRect.setFillColor(sf::Color::Transparent); drawRect.setOutlineThickness(0);

    font = _font;
    text = sf::Text("", _font, characterSize);
    text.setPosition(drawRect.getPosition());
    text.setFillColor(sf::Color::Black);
}

void Label::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    target.draw(text, states);
}