#pragma once
#include "text.h"

////////////////////////////////////////////////////////////
// Class
////////////////////////////////////////////////////////////

#pragma pack(push, 1)
class Panel : public sf::Transformable, public sf::Drawable {
public:
    bool visible = true;
    char anchor = 'C';      // How to horizontally align text. C = center, L = left, R = right

    sf::Sprite sprite;
    PlacedText text;
    std::vector<sf::String> lines;

    Panel(sf::String = "");
    sf::Vector2f getSize() const        { return sprite.getGlobalBounds().getSize() * getScale(); }
    sf::Vector2f getCenter() const      { return getPosition() + getSize() / 2.f; }
    void setCenter(float x, float y);
    void setCenter(sf::Vector2f v)      { setCenter(v.x, v.y); }
    void setSize(float w, float h)      { setScale(w / sprite.getTexture()->getSize().x, h / sprite.getTexture()->getSize().y); }
    void setSize(sf::Vector2f s)        { setSize(s.x, s.y); }

    void setTexture(sf::Texture&);

    void addWord(sf::String word)       { lines.push_back(word); text.setString(text.getString() + "\n" + word); text.setCenter(getCenter()); }
    void removeWord(int index);
    void setString(sf::String word)     { lines.clear(); lines.push_back(word); text.setString(word); text.setCenter(getCenter()); }
    void clearText()                    { lines.clear(); }
    void setCharacterSize(int charSize) { text.setCharacterSize(charSize); }
    void setTextColor(sf::Color color)  { text.setFillColor(color); }
    size_t getLineCount()               { return lines.size(); }

    sf::String& operator[](int index)   { return lines[index]; }
    void draw(sf::RenderTarget& target, sf::RenderStates states = sf::RenderStates::Default) const;
};
#pragma pack(pop)

////////////////////////////////////////////////////////////
// Realization
////////////////////////////////////////////////////////////

Panel::Panel(sf::String word) {
    text.setCharacterSize(30);
    text.setFillColor(sf::Color::White);
    setString(word);
    text.setCenter(getCenter());
}

void Panel::setTexture(sf::Texture& texture) {
    if (sprite.getTexture() != nullptr) {
        sf::Vector2f curSize(getScale() * ((sf::Vector2f) sprite.getTexture()->getSize()));
        sprite.setTexture(texture, true);
        setSize(curSize);
    }
    else sprite.setTexture(texture);
}

void Panel::removeWord(int index) {
    sf::String res = text.getString();
    res.replace(sf::String(lines[index]), sf::String(""));
    text.setString(res);
    lines.erase(lines.begin() + index);
}

void Panel::setCenter(float x, float y) {
    setPosition(x - sprite.getGlobalBounds().width / 2, y - sprite.getGlobalBounds().height / 2);
}

void Panel::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    if (visible) {
        states.transform *= getTransform();
        target.draw(sprite, states);
        target.draw(text);
    }
}