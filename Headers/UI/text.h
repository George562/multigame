#pragma once
#include "../../SFML-2.5.1/include/SFML/Graphics.hpp"
#include "fontString.h"

//////////////////////////////////////////////////////////// Fonts
sf::Font ljk_InkyFont;

void loadFonts() {
    ljk_InkyFont.loadFromFile("sources/fonts/ljk_Inky Thin Pixels.otf");
}

////////////////////////////////////////////////////////////
// Class
////////////////////////////////////////////////////////////

#pragma pack(push, 1)
class PlacedText : public sf::Text {
public:
    bool visible = true;
    float Width, Height;

    PlacedText();
    PlacedText(sf::String);
    PlacedText(FontString);

    void setCenter(float x, float y)    { Text::setPosition(x - Width / 2, y - Height / 2); }
    void setCenter(sf::Vector2f v)      { setCenter(v.x, v.y); }

    sf::Vector2f getSize() const        { return {Width, Height}; }
    sf::Vector2f getRight() const       { return {getPosition().x + getSize().x, getPosition().y}; }
    void setSize(float w, float h)      { Width = w; Height = h; }
    void setSize(sf::Vector2f v)        { setSize(v.x, v.y); }

    void setString(sf::String s);
    void setString(FontString s);
    void setCharacterSize(int x);
    void setScale(sf::Vector2f);
    void setScale(float, float);
    size_t TextSize()                   { return getString().getSize(); }
    void insert(int pos, sf::String t)  { setString(getString().substring(0, pos) + t + getString().substring(pos, TextSize())); }

    virtual void draw(sf::RenderTarget& target, sf::RenderStates states = sf::RenderStates::Default) const {
        if (visible) {
            target.draw(static_cast<sf::Text>(*this), states);
        }
    };
};
#pragma pack(pop)

////////////////////////////////////////////////////////////
// Realization
////////////////////////////////////////////////////////////

PlacedText::PlacedText() : sf::Text() {
    setFont(ljk_InkyFont);
}

PlacedText::PlacedText(sf::String s) : sf::Text() {
    setFont(ljk_InkyFont);
    setString(s);
}

PlacedText::PlacedText(FontString s) : sf::Text() {
    setFont(ljk_InkyFont);
    setString(s);
}

void PlacedText::setString(sf::String s) {
    sf::Text::setString(s);
    setSize(getGlobalBounds().width, getGlobalBounds().height);
}

void PlacedText::setString(FontString s) {
    sf::Text::setString(s.text);
    setCharacterSize(s.charSize);
    sf::Text::setFillColor(s.fillColor);
    setSize(getGlobalBounds().width, getGlobalBounds().height);
}

void PlacedText::setCharacterSize(int x) {
    sf::Text::setCharacterSize(x);
    setSize(getGlobalBounds().width, getGlobalBounds().height);
}

void PlacedText::setScale(sf::Vector2f factors) {
    sf::Text::setScale(factors);
    setSize(Width * factors.x, Height * factors.y);
}
void PlacedText::setScale(float factorX, float factorY) {
    sf::Text::setScale(factorX, factorY);
    setSize(Width * factorX, Height * factorY);
}
