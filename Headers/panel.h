#pragma once
#include "text.h"

////////////////////////////////////////////////////////////
// Class
////////////////////////////////////////////////////////////

class Panel : public PlacedText {
public:
    sf::Texture texture;
    sf::Sprite rect;

    Panel() : PlacedText() {};
    Panel(str, str);
    void setTexture(str);
    void addWord(str);
    void removeWord(str);
    void removeWord(int);
    void setWord(str);
    str getWord();
    void setPosition(float, float);
    void draw(sf::RenderWindow&);
    void setTextSize(int);
    int getTextSize();
    void clear();
    int size();
    str& operator[](int);
    void setSize(sf::Vector2f v) { setSize(v.x, v.y); }
    void setSize(float w, float h) {
        Width = w; Height = h;
        text.setScale(w / rect.getGlobalBounds().width, h / rect.getGlobalBounds().height);
        text.setPosition(PosX + 50 * w / rect.getGlobalBounds().width, PosY + 50 * h / rect.getGlobalBounds().height);
        rect.setScale(w / rect.getGlobalBounds().width, h / rect.getGlobalBounds().height);
    }
};

////////////////////////////////////////////////////////////
// Realization
////////////////////////////////////////////////////////////

Panel::Panel(str name, str word = "") : PlacedText() {
    setTexture(name);
    text.setCharacterSize(150);
    text.setFillColor(sf::Color(199, 199, 199));
    Width = rect.getGlobalBounds().width; Height = rect.getGlobalBounds().height;
    setWord(word);
}

void Panel::setTexture(str name) {
    texture.loadFromFile(name + ".png");
    rect.setTexture(texture);
}

void Panel::addWord(str word) { lines.push_back(word); }

void Panel::removeWord(str word) {
    for (size_t i = 0; i < lines.size(); i++)
        if (lines[i] == word)
            lines.erase(lines.begin() + i);
}

void Panel::removeWord(int index) { lines.erase(lines.begin() + index); }

void Panel::setWord(str word) {
    lines.clear();
    lines.push_back(word);
}
str Panel::getWord() {
    str res;
    if (lines.size() != 0) res += lines[0];
    for (size_t i = 1; i < lines.size(); i++) res +=  "\n" + lines[i];
    return res;
}

void Panel::setPosition(float x, float y) {
    PosX = x; PosY = y;
    rect.setPosition(x, y);
    text.setPosition(PosX + 50, PosY + 50);
}

void Panel::draw(sf::RenderWindow& window) {
    window.draw(rect);
    if (size() == 1) {
        text.setString(lines[0]);
        window.draw(text);
    }
    else
        for (size_t i = 0; i < lines.size(); i++) {
            text.setString(lines[i]);
            text.setPosition(PosX + 50, PosY + 50 + i * (text.getCharacterSize() + 10));
            window.draw(text);
        }
}

void Panel::setTextSize(int size) {
    text.setCharacterSize(size);
    setPosition(PosX, PosY);
}
int Panel::getTextSize() { return text.getCharacterSize(); }

void Panel::clear() { lines.clear(); }
int Panel::size() { return lines.size(); }
str& Panel::operator[](int index) { return lines[index]; }
