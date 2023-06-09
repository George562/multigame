#pragma once
#include "panel.h"

////////////////////////////////////////////////////////////
// Class
////////////////////////////////////////////////////////////

class Button : public Panel {
public:
    sf::Texture pushedTexture;
    bool Pushed;
    void (*buttonFunction)(void);

    Button() : Panel() {}
    Button(str, str, void (*)(void));
    void setWord(str);
    str getWord();
    void setPosition(float, float);
    virtual void draw(sf::RenderTarget&, sf::RenderStates = sf::RenderStates::Default) const;
    bool OnTheButton(int&, int&);
    bool isActivated(sf::Event&);
};

////////////////////////////////////////////////////////////
// Realization
////////////////////////////////////////////////////////////

Button::Button(str name, str word, void (*foo)(void)) : Panel(name, word) {
    pushedTexture.loadFromFile(name + "Pushed.png");
    setWord(word);
    buttonFunction = foo;
}

void Button::setWord(str word) {
    text.setString(word);
    text.setPosition(PosX + Width / 2 - text.getGlobalBounds().width / 2,
                     PosY + Height / 2 - text.getGlobalBounds().height / 2);
}
str Button::getWord() { return text.getString(); }

void Button::setPosition(float x, float y) {
    PosX = x; PosY = y;
    rect.setPosition(x, y);
    text.setPosition(x + Width / 2 - text.getGlobalBounds().width / 2,
                     y + Height / 2 - text.getGlobalBounds().height / 2);
}

void Button::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    target.draw(rect);
    target.draw(text);
}

bool Button::OnTheButton(int& x, int& y) {
    return contains(x, y);
}

bool Button::isActivated(sf::Event& event) {
    if (Pushed && event.type == sf::Event::MouseButtonReleased) {
        Pushed = false;
        rect.setTexture(texture);
        if (OnTheButton(event.mouseButton.x, event.mouseButton.y)) {
            buttonFunction();
            return true;
        }
    }
    if (event.type == sf::Event::MouseButtonPressed &&
        OnTheButton(event.mouseButton.x, event.mouseButton.y) && 
        event.mouseButton.button == sf::Mouse::Button::Left) {
            Pushed = true;
            rect.setTexture(pushedTexture);
        }
    return false;
}
