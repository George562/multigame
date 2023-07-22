#pragma once
#include "text.h"

////////////////////////////////////////////////////////////
// Class
////////////////////////////////////////////////////////////

class Button : public Rect, public sf::Drawable {
public:
    sf::Texture texture, pushedTexture;
    sf::Sprite rect;
    PlacedText ButtonText;
    bool Pushed = false, ShowText = true;
    void (*buttonFunction)(void);

    Button() {}
    Button(str, str, void (*)(void));
    void setWord(str);
    void setTexture(str);
    void setPosition(float, float);
    void setSize(float, float);
    void setSize(sf::Vector2f v) { setSize(v.x, v.y); }
    void setCharacterSize(int);
    virtual void draw(sf::RenderTarget&, sf::RenderStates = sf::RenderStates::Default) const;
    bool OnTheButton(int& x, int& y)  { return contains(x, y); }
    bool isActivated(sf::Event&);
};

////////////////////////////////////////////////////////////
// Realization
////////////////////////////////////////////////////////////

Button::Button(str TextureFile, str word, void (*foo)(void)) {
    ButtonText.setCharacterSize(150);
    ButtonText.setFillColor(sf::Color(199, 199, 199));
    setTexture(TextureFile);
    setSize(rect.getGlobalBounds().width, rect.getGlobalBounds().height);
    setWord(word);
    buttonFunction = foo;
}

void Button::setWord(str word) {
    ButtonText.setString(word);
    ButtonText.setCenter(getCenter());
}

void Button::setTexture(str TextureFile) {
    texture.loadFromFile(TextureFile + ".png");
    pushedTexture.loadFromFile(TextureFile + "Pushed.png");
    rect.setTexture(texture);
}

void Button::setPosition(float x, float y) {
    PosX = x; PosY = y;
    rect.setPosition(x, y);
    ButtonText.setCenter(getCenter());
}

void Button::setSize(float w, float h) {
    if (Width != 0 && Height != 0) {
        ButtonText.setScale(w / Width, h / Height);
        ButtonText.setCenter(getCenter());
        rect.setScale(w / Width, h / Height);
    }
    Width = w; Height = h;
}

void Button::setCharacterSize(int size) {
    ButtonText.setCharacterSize(size);
    setPosition(PosX, PosY);
}

void Button::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    target.draw(rect);
    if (ShowText) target.draw(ButtonText);
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
