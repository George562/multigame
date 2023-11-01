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
    void addWord(str word)          { lines.push_back(word); setString(getString() + "\n" + word); }
    void removeWord(int index);
    void setWord(str word)          { lines.clear(); lines.push_back(word); setString(word); }
    void setPosition(float, float);
    void draw(sf::RenderTarget& target, sf::RenderStates states = sf::RenderStates::Default) const;
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
    setCharacterSize(150);
    setFillColor(sf::Color(199, 199, 199));
    setWord(word);
}

void Panel::setTexture(sf::Texture& texture) {
    sprite.setTexture(texture);
    Width = texture.getSize().x; Height = texture.getSize().y;
}

void Panel::removeWord(int index) {
    sf::String res = getString();
    res.replace(sf::String(lines[index]), sf::String(""));
    setString(res);
    lines.erase(lines.begin() + index);
}

void Panel::setPosition(float x, float y) {
    sprite.setPosition(x, y);
    sf::Text::setPosition(x + 50, y + 50);
}

void Panel::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    target.draw(sprite, states);
    target.draw((sf::Text)(*this), states);
}

void Panel::setSize(float w, float h) {
    setScale(w / Width, h / Height);
    Width = w; Height = h;
}