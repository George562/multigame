#pragma once
#include "../../SFML-2.5.1/include/SFML/Graphics.hpp"
#include "fontString.h"
#include "../Abstracts/UIElement.h"

//////////////////////////////////////////////////////////// Fonts
namespace Fonts {
    sf::Font ljk_Inky,
             consola
             ;
}

void loadFonts() {
    Fonts::ljk_Inky.loadFromFile("sources/fonts/Inky-Thin-Pixels.ttf");
    Fonts::consola .loadFromFile("sources/fonts/consola.ttf");
}

////////////////////////////////////////////////////////////
// Class
////////////////////////////////////////////////////////////

#pragma pack(push, 1)
class PlacedText : public UIElement {
public:
    sf::Text text;

    PlacedText() { text.setFont(Fonts::ljk_Inky); }
    PlacedText(std::string, FontString);
    PlacedText(std::string, UI::Anchor, UI::Anchor, FontString);
    PlacedText(std::string, sf::Vector2f, FontString);

    FontString getString() const {
        return FontString(text.getString(),
                          text.getCharacterSize(),
                          text.getFillColor());
    }
    std::string getText() const { return text.getString().toAnsiString(); }
    sf::FloatRect getGlobalBounds() { return text.getGlobalBounds(); }

    void setString(std::string s);
    void setFontString(FontString s);
    void addString(std::string s)           { setString(text.getString() + s); }
    void setCharacterSize(int x);
    void setOutlineThickness(int thickness) { text.setOutlineThickness(thickness); }
    void setOutlineColor(sf::Color color)   { text.setOutlineColor(color); }
    void setFillColor(sf::Color color)      { text.setFillColor(color); }
    void setPosition(float x, float y);
    void setPosition(sf::Vector2f v)        { setPosition(v.x, v.y); }
    //void setScale(sf::Vector2f);
    //void setScale(float, float);
    size_t TextSize()                       { return getString().charSize; }
    void insert(int pos, std::string t) {
        setString(getString().text.substring(0, pos) + t +
                  getString().text.substring(pos, TextSize()));
    }

    virtual void draw(sf::RenderTarget& target, sf::RenderStates states = sf::RenderStates::Default) const {
        target.draw(text, states);
    };
};
#pragma pack(pop)

////////////////////////////////////////////////////////////
// Realization
////////////////////////////////////////////////////////////

PlacedText::PlacedText(std::string name, FontString s = "") : PlacedText() {
    setName(name);
    setFontString(s);
}

PlacedText::PlacedText(std::string name, UI::Anchor anchor, UI::Anchor anchoringPoint,
    FontString s = "") : PlacedText(name, s) {
    setAnchors(anchor, anchoringPoint);
}

PlacedText::PlacedText(std::string name, sf::Vector2f pos,
    FontString s = "") : PlacedText(name, UI::none, UI::none, s) {
    setPosition(pos.x, pos.y);
}

void PlacedText::setString(std::string s) {
    int charSize = text.getCharacterSize();
    if (charSize == 0)
        charSize = 150;
    sf::Color fillColor = text.getFillColor();
    if (fillColor == sf::Color::White)
        fillColor = CommonColors::text;
    setFontString(FontString(s, charSize, fillColor));
}

void PlacedText::setFontString(FontString s) {
    text.setString(s);
    text.setCharacterSize(s.charSize);
    text.setFillColor(s.fillColor);
    setSize(text.getGlobalBounds().width, text.getGlobalBounds().height);
}

void PlacedText::setCharacterSize(int x) {
    text.setCharacterSize(x);
    setSize(text.getGlobalBounds().width, text.getGlobalBounds().height);
}

void PlacedText::setPosition(float x, float y) {
    UIRect::setPosition(x, y);
    text.setPosition(getPosition());
}
//
//void PlacedText::setScale(sf::Vector2f factors) {
//    text.setScale(factors);
//    setSize(Width * factors.x, Height * factors.y);
//}
//void PlacedText::setScale(float factorX, float factorY) {
//    text.setScale(factorX, factorY);
//    setSize(Width * factorX, Height * factorY);
//}
