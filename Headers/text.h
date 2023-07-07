#pragma once
#include "tools.h"

////////////////////////////////////////////////////////////
// Class
////////////////////////////////////////////////////////////

class PlacedText : public Rect {
public:
    sf::Font font;
    sf::Text text;
    std::vector<str> lines;

    PlacedText();
    void setPosition(float, float);
    void setCenter(float, float);
    virtual void draw(sf::RenderWindow& window) { window.draw(text); };
    void setText(str s) { text.setString(s); Width = text.getGlobalBounds().width; Height = text.getGlobalBounds().height; }
    void setCharacterSize(int x) { text.setCharacterSize(x); Width = text.getGlobalBounds().width; Height = text.getGlobalBounds().height; }
};

////////////////////////////////////////////////////////////
// Realization
////////////////////////////////////////////////////////////

PlacedText::PlacedText() {
    font.loadFromFile("sources/ljk_Inky Thin Pixels.otf");
    text.setFont(font);
}

void PlacedText::setPosition(float x, float y) {
    PosX = x; PosY = y;
    text.setPosition(PosX, PosY);
}

void PlacedText::setCenter(float x, float y) {
    PosX = x - Width / 2; PosY = y - Height / 2;
    text.setPosition(PosX, PosY);
}
