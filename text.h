#pragma once
#include "tools.h"

////////////////////////////////////////////////////////////
// Class
////////////////////////////////////////////////////////////

class PlaccedText : public Rect {
public:
    sf::Font font;
    sf::Text text;
    std::vector<str> lines;

    PlaccedText();
    void setPosition(float, float);
    virtual void draw(sf::RenderWindow& window) { window.draw(text); };
    void setText(str s) { text.setString(s); Width = text.getGlobalBounds().width; Height = text.getGlobalBounds().height; }
    void setCharacterSize(int x) { text.setCharacterSize(x); Width = text.getGlobalBounds().width; Height = text.getGlobalBounds().height; }
};

////////////////////////////////////////////////////////////
// Realization
////////////////////////////////////////////////////////////

PlaccedText::PlaccedText() {
    font.loadFromFile("sources/ljk_Inky Thin Pixels.otf");
    text.setFont(font);
}

void PlaccedText::setPosition(float x, float y) {
    PosX = x; PosY = y;
    text.setPosition(x, y);
}
