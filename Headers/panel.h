#pragma once
#include "text.h"

////////////////////////////////////////////////////////////
// Class
////////////////////////////////////////////////////////////

#pragma pack(push, 1)
class Panel : public sf::Transformable, public sf::Drawable {
public:
    sf::Sprite sprite;
    PlacedText text;
    std::vector<sf::String> lines;

    Panel(sf::String = "");
    void setTexture(sf::Texture&);
    void addWord(sf::String word)       { lines.push_back(word); text.setString(text.getString() + "\n" + word); }
    void removeWord(int index);
    void setWord(sf::String word)       { lines.clear(); lines.push_back(word); text.setString(word); }
    void setPosition(float, float);
    void setCenter(float x, float y);
    void setCenter(sf::Vector2f v)      { setCenter(v.x, v.y); }
    void draw(sf::RenderTarget& target, sf::RenderStates states = sf::RenderStates::Default) const;
    void clear()                        { lines.clear(); }
    size_t size()                       { return lines.size(); }
    sf::String& operator[](int index)   { return lines[index]; }
    void setSize(sf::Vector2f v)        { setSize(v.x, v.y); }
    void setSize(float, float);
};
#pragma pack(pop)

////////////////////////////////////////////////////////////
// Realization
////////////////////////////////////////////////////////////

Panel::Panel(sf::String word) {
    text.setCharacterSize(150);
    text.setFillColor(sf::Color(199, 199, 199));
    setWord(word);
}

void Panel::setTexture(sf::Texture& texture) {
    sprite.setTexture(texture);
}

void Panel::removeWord(int index) {
    sf::String res = text.getString();
    res.replace(sf::String(lines[index]), sf::String(""));
    text.setString(res);
    lines.erase(lines.begin() + index);
}

void Panel::setPosition(float x, float y) {
    sprite.setPosition(x, y);
    text.setPosition(x + 50, y + 50);
}
void Panel::setCenter(float x, float y) {
    setPosition(x - sprite.getGlobalBounds().width / 2, y - sprite.getGlobalBounds().height / 2);
}

void Panel::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    states.transform *= getTransform();
    target.draw(sprite, states);
    target.draw(text, states);
}

void Panel::setSize(float w, float h) {
    setScale(w / sprite.getGlobalBounds().width, h / sprite.getGlobalBounds().height);
}