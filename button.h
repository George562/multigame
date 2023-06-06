#include "panel.h"

////////////////////////////////////////////////////////////
// Class
////////////////////////////////////////////////////////////

class Button : public Panel {
public:
    sf::Texture pushedTexture;
    bool Pushed;

    Button(str, str);
    void setWord(str);
    str getWord();
    void setPosition(float, float);
    void draw(sf::RenderWindow&);
    bool OnTheButton(int, int);
    bool isActivated(sf::Event&);
};

////////////////////////////////////////////////////////////
// Realization
////////////////////////////////////////////////////////////

Button::Button(str name, str word = "") : Panel(name, word) {
    pushedTexture.loadFromFile(name + "Pushed.png");
    setWord(word);
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

void Button::draw(sf::RenderWindow& window) {
    window.draw(rect);
    window.draw(text);
}

bool Button::OnTheButton(int x, int y) {
    return PosX <= x && x <= PosX + Width && PosY <= y && y <= PosY + Height;
}

bool Button::isActivated(sf::Event& event) {
    if (Pushed && event.type == sf::Event::MouseButtonReleased) {
        Pushed = false;
        rect.setTexture(texture);
        if (OnTheButton(event.mouseButton.x, event.mouseButton.y))
            return true;
    }
    if (event.type == sf::Event::MouseButtonPressed &&
        OnTheButton(event.mouseButton.x, event.mouseButton.y) && 
        event.mouseButton.button == sf::Mouse::Button::Left) {
            Pushed = true;
            rect.setTexture(pushedTexture);
        }
    return false;
}
