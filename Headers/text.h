#pragma once
#include "init.h"

////////////////////////////////////////////////////////////
// Class
////////////////////////////////////////////////////////////

class PlacedText : public Rect, public sf::Drawable {
public:
    sf::Font font;
    sf::Text text;
    std::vector<str> lines;

    PlacedText();
    void setPosition(float, float);
    void setCenter(float, float);
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states = sf::RenderStates::Default) const { target.draw(text); };
    void setText(str s);
    void setCharacterSize(int x);
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

void PlacedText::setText(str s) {
    text.setString(s);
    Width = text.getGlobalBounds().width;
    Height = text.getGlobalBounds().height;
}

void PlacedText::setCharacterSize(int x) {
    text.setCharacterSize(x);
    Width = text.getGlobalBounds().width;
    Height = text.getGlobalBounds().height;
}
