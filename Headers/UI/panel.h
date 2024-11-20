#pragma once
#include "text.h"
#include "../Abstracts/UIElement.h"

////////////////////////////////////////////////////////////
// Class
////////////////////////////////////////////////////////////

#pragma pack(push, 1)
class Panel : public UIElement {
public:
    PlacedText text;
    sf::Sprite sprite;
    std::vector<std::string> lines;

    Panel();
    Panel(std::string, std::string = "");
    Panel(std::string, UI::Anchor, UI::Anchor, sf::Vector2f = { 0, 0 }, std::string = "");
    Panel(std::string, sf::FloatRect, std::string = "");
    sf::Vector2f getCenter() const      { return getPosition() + sf::Vector2f(getSize() / 2u); }

    void setPosition(float x, float y);

    void addWord(std::string word) {
        lines.push_back(word);
        text.addString("\n" + word);
        text.setCenter(getCenter());
    }
    void removeWord(int index);

    void setString(std::string word)     { lines.clear(); lines.push_back(word); text.setString(word); text.setCenter(getCenter()); }
    void setFontString(FontString word) { lines.clear(); lines.push_back(word); text.setFontString(word); text.setCenter(getCenter()); }
    void clearText()                    { lines.clear(); }
    void setCharacterSize(int charSize) { text.setCharacterSize(charSize); }
    void setTextColor(sf::Color color)  { text.setFillColor(color); }

    size_t getLineCount()               { return lines.size(); }

    void setTexture(sf::Texture&, UI::TextureResize, sf::Vector2u);
    void setSpriteSize(sf::Vector2u);
    void setSpriteColor(sf::Color color) { sprite.setColor(color); }

    std::string& operator[](int index)   { return lines[index]; }
    void draw(sf::RenderTarget& target, sf::RenderStates states = sf::RenderStates::Default) const;
};
#pragma pack(pop)

////////////////////////////////////////////////////////////
// Realization
////////////////////////////////////////////////////////////

Panel::Panel() {}

Panel::Panel(std::string name, std::string word) : Panel() {
    setName(name);
    if (word != "") {
        text.setCharacterSize(30);
        text.setFillColor(sf::Color::White);
        setString(word);
        text.setCenter(getCenter());
    }
}

Panel::Panel(std::string name, UI::Anchor anchor, UI::Anchor anchoringPoint,
             sf::Vector2f size, std::string word) : Panel(name, word) {
    setAnchors(anchor, anchoringPoint);
    setRect(0, 0, size.x, size.y);
}

Panel::Panel(std::string name, sf::FloatRect rect, std::string word)
    : Panel(name, UI::none, UI::none, { rect.width, rect.height }, word) {
    setPosition(rect.left, rect.top);
}

void Panel::setPosition(float x, float y) {
    UIElement::setPosition(x, y);
    text.setCenter(getCenter());
}

void Panel::setTexture(sf::Texture& texture, UI::TextureResize mode = UI::element, sf::Vector2u size = { 0, 0 }) {
    sprite.setTexture(texture, true);
    if (mode == UI::element)
        setSpriteSize(getSize());
    else if (mode == UI::texture)
        setSpriteSize(texture.getSize());
    else if (mode == UI::size && size != sf::Vector2u{0, 0})
        setSpriteSize(size);
}

void Panel::setSpriteSize(sf::Vector2u size) {
    sprite.setScale(sf::Vector2f{
        float(size.x) / sprite.getTexture()->getSize().x,
        float(size.y) / sprite.getTexture()->getSize().y
    });
}

void Panel::removeWord(int index) {
    sf::String res = text.getString();
    res.replace(std::string(lines[index]), std::string(""));
    text.setString(res);
    lines.erase(lines.begin() + index);
}

void Panel::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    target.draw(sprite, states.transform * getTransform());
    target.draw(text);
}