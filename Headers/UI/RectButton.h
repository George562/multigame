#pragma once
#include "text.h"
#include "fontString.h"
#include "../CollisionShapes/collisionRect.h"

////////////////////////////////////////////////////////////
// Class
////////////////////////////////////////////////////////////

#pragma pack(push, 1)
class RectButton : public sf::Drawable {
public:
    sf::Texture* texture, *pushedTexture;
    sf::Sprite sprite;
    PlacedText ButtonText;
    bool Pushed = false, visible = true;
    void (*buttonFunction)(void);
    CollisionRect hitbox;

    RectButton() {}
    RectButton(sf::String, void (*)(void));
    RectButton(FontString, void (*)(void), sf::Texture&, sf::Texture&);

    sf::Vector2f getPosition() const { return hitbox.getPosition(); }
    sf::Vector2f getCenter() const { return hitbox.getCenter(); }
    sf::Vector2f getSize() const { return hitbox.getSize(); }
    sf::Vector2f getRightTop() const { return hitbox.getPosition() + sf::Vector2f(hitbox.getSize().x, 0); }
    sf::Vector2f getRightBottom() const { return hitbox.getPosition() + hitbox.getSize(); }

    void setPosition(float, float);
    void setPosition(sf::Vector2f v) { setPosition(v.x, v.y); }
    void setCenter(float x, float y) { setPosition(x - hitbox.getSize().x / 2, y - hitbox.getSize().y / 2); }
    void setCenter(sf::Vector2f v) { setCenter(v.x, v.y); }
    void setSize(float, float);
    void setSize(sf::Vector2f v) { setSize(v.x, v.y); }
    void setRect(float x, float y, float w, float h) { setPosition(x, y); setSize(w, h); }
    void setRect(sf::Vector2f pos, sf::Vector2f size) { setRect(pos.x, pos.y, size.x, size.y); }
    void setRect(sf::FloatRect rect) { setRect(rect.getPosition(), rect.getSize()); }

    void setFunction(void (*func)()) { buttonFunction = func; }
    void setWord(sf::String word) { ButtonText.setString(word); ButtonText.setCenter(hitbox.getCenter()); }
    void setTexture(sf::Texture&, sf::Texture&);
    void setCharacterSize(int size) { ButtonText.setCharacterSize(size); ButtonText.setCenter(hitbox.getCenter()); }
    virtual void draw(sf::RenderTarget&, sf::RenderStates = sf::RenderStates::Default) const;
    bool OnTheButton(int& x, int& y) { return hitbox.contains(x, y); }
    bool isActivated(sf::Event&);
};
#pragma pack(pop)

////////////////////////////////////////////////////////////
// Realization
////////////////////////////////////////////////////////////

RectButton::RectButton(sf::String word, void (*foo)(void)) {
    ButtonText.setCharacterSize(150);
    ButtonText.setFillColor(sf::Color(200, 200, 200));
    setWord(word);
    buttonFunction = foo;
}

RectButton::RectButton(FontString string, void (*foo)(void),
                       sf::Texture& texture, sf::Texture& pushedTexture) {
    ButtonText.setCharacterSize(string.charSize);
    ButtonText.setFillColor(string.fillColor);
    setWord(string.text);
    buttonFunction = foo;
    setTexture(texture, pushedTexture);
}

void RectButton::setTexture(sf::Texture& texture, sf::Texture& pushedTexture) {
    this->texture = &texture;
    this->pushedTexture = &pushedTexture;
    sprite.setTexture(texture);
    setSize(texture.getSize().x, texture.getSize().y);
}

void RectButton::setPosition(float x, float y) {
    hitbox.setPosition(x, y);
    sprite.setPosition(x, y);
    ButtonText.setCenter(hitbox.getCenter());
}

void RectButton::setSize(float w, float h) {
    if (hitbox.getSize().x != 0 && hitbox.getSize().y != 0) {
        sprite.setScale(w / hitbox.getSize().x, h / hitbox.getSize().y);
    }
    hitbox.setSize(w, h);
    ButtonText.setCenter(hitbox.getCenter());
}

void RectButton::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    if (visible) {
        target.draw(sprite, states);
        target.draw(ButtonText, states);
    }
}

bool RectButton::isActivated(sf::Event& event) {
    if (Pushed && event.type == sf::Event::MouseButtonReleased) {
        Pushed = false;
        sprite.setTexture(*texture);
        if (OnTheButton(event.mouseButton.x, event.mouseButton.y)) {
            buttonFunction();
            return true;
        }
    }
    if (event.type == sf::Event::MouseButtonPressed &&
        OnTheButton(event.mouseButton.x, event.mouseButton.y) &&
        event.mouseButton.button == sf::Mouse::Button::Left) {
            Pushed = true;
            sprite.setTexture(*pushedTexture);
            return true;
        }
    return false;
}
