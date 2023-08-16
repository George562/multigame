#pragma once
#include "text.h"

////////////////////////////////////////////////////////////
// Class
////////////////////////////////////////////////////////////

class Button : public Rect, public sf::Drawable {
public:
    sf::Texture texture, pushedTexture;
    sf::Sprite sprite;
    PlacedText ButtonText;
    bool Pushed = false, ShowButton = true;
    void (*buttonFunction)(void);

    Button() {}
    Button(str, str, void (*)(void));

    void setPosition(float, float);
    void setPosition(sf::Vector2f v) { setPosition(v.x, v.y); }
    void setCenter(float x, float y) { setPosition(x - Width / 2, y - Height / 2); }
    void setCenter(sf::Vector2f v) { setCenter(v.x, v.y); }

    void setSize(float, float);
    void setSize(sf::Vector2f v) { setSize(v.x, v.y); }

    void setWord(str word) { ButtonText.setString(word); ButtonText.setCenter(getCenter()); }
    void setTexture(str);
    void setCharacterSize(int size) { ButtonText.setCharacterSize(size); ButtonText.setCenter(getCenter()); }
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
    setSize(sprite.getGlobalBounds().width, sprite.getGlobalBounds().height);
    setWord(word);
    buttonFunction = foo;
}

void Button::setTexture(str TextureFile) {
    texture.loadFromFile(TextureFile + ".png");
    pushedTexture.loadFromFile(TextureFile + "Pushed.png");
    sprite.setTexture(texture);
}

void Button::setPosition(float x, float y) {
    PosX = x; PosY = y;
    sprite.setPosition(x, y);
    ButtonText.setCenter(getCenter());
}

void Button::setSize(float w, float h) {
    if (Width != 0 && Height != 0) {
        ButtonText.setScale(w / Width, h / Height);
        ButtonText.setCenter(getCenter());
        sprite.setScale(w / Width, h / Height);
    }
    Width = w; Height = h;
}

void Button::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    if (ShowButton) {
        target.draw(sprite, states);
        target.draw(ButtonText, states);
    }
}

bool Button::isActivated(sf::Event& event) {
    if (Pushed && event.type == sf::Event::MouseButtonReleased) {
        Pushed = false;
        sprite.setTexture(texture);
        if (OnTheButton(event.mouseButton.x, event.mouseButton.y)) {
            buttonFunction();
            return true;
        }
    }
    if (event.type == sf::Event::MouseButtonPressed &&
        OnTheButton(event.mouseButton.x, event.mouseButton.y) && 
        event.mouseButton.button == sf::Mouse::Button::Left) {
            Pushed = true;
            sprite.setTexture(pushedTexture);
            return true;
        }
    return false;
}
