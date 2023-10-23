#pragma once
#include "text.h"

////////////////////////////////////////////////////////////
// Class
////////////////////////////////////////////////////////////

#pragma pack(push, 1)
class Panel : public PlacedText {
public:
    sf::Sprite sprite;
    std::vector<str> lines;

    Panel(str = "");
    void setTexture(sf::Texture&);
    void addWord(str word)          { lines.push_back(word); }
    void removeWord(int index)      { lines.erase(lines.begin() + index); }
    void setWord(str word)          { lines.clear(); lines.push_back(word); }
    str getWord();
    void setPosition(float, float);
    void draw(sf::RenderWindow&);
    void setTextSize(int);
    void clear()                    { lines.clear(); }
    size_t size()                   { return lines.size(); }
    str& operator[](int index)      { return lines[index]; }
    void setSize(sf::Vector2f v)    { setSize(v.x, v.y); }
    void setSize(float, float);
};
#pragma pack(pop)

////////////////////////////////////////////////////////////
// Realization
////////////////////////////////////////////////////////////

Panel::Panel(str word) : PlacedText() {
    text.setCharacterSize(150);
    text.setFillColor(sf::Color(199, 199, 199));
    setWord(word);
}

void Panel::setTexture(sf::Texture& texture) {
    sprite.setTexture(texture);
    Width = texture.getSize().x; Height = texture.getSize().y;
}

str Panel::getWord() {
    str res;
    if (lines.size() != 0) res += lines[0];
    for (size_t i = 1; i < lines.size(); i++) res +=  "\n" + lines[i];
    return res;
}

void Panel::setPosition(float x, float y) {
    PosX = x; PosY = y;
    sprite.setPosition(x, y);
    text.setPosition(PosX + 50, PosY + 50);
}

void Panel::draw(sf::RenderWindow& window) {
    window.draw(sprite);
    if (size() == 1) {
        text.setString(lines[0]);
        window.draw(text);
    } else
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

void  Panel::setSize(float w, float h) {
    text.setScale(w / Width, h / Height);
    text.setPosition(PosX + 50 * w / Width, PosY + 50 * h / Height);
    sprite.setScale(w / Width, h / Height);
    Width = w; Height = h;
}