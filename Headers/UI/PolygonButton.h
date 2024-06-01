#pragma once
#include "../../SFML-2.5.1/include/SFML/Network.hpp"
#include "text.h"
#include "../CollisionShapes/collisionRect.h"

////////////////////////////////////////////////////////////
// Class
////////////////////////////////////////////////////////////

#pragma pack(push, 1)
class PolygonButton : public sf::Drawable {
public:
    sf::Texture* texture, *pushedTexture;
    sf::Sprite sprite;
    PlacedText ButtonText;
    bool Pushed = false, visible = true;
    void (*buttonFunction)(void);
    CollisionShape hitbox;

    PolygonButton() {}
    PolygonButton(sf::String, void (*)(void));

    void setPosition(float, float);
    void setPosition(sf::Vector2f v) { setPosition(v.x, v.y); }
    void setCenter(float x, float y) { setPosition(x - hitbox.getSize().x / 2, y - hitbox.getSize().y / 2); }
    void setCenter(sf::Vector2f v) { setCenter(v.x, v.y); }
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

PolygonButton::PolygonButton(sf::String word, void (*foo)(void)) {
    ButtonText.setCharacterSize(150);
    ButtonText.setFillColor(sf::Color(199, 199, 199));
    setWord(word);
    buttonFunction = foo;
}

void PolygonButton::setTexture(sf::Texture& texture, sf::Texture& pushedTexture) {
    this->texture = &texture;
    this->pushedTexture = &pushedTexture;
    sprite.setTexture(texture);
}

void PolygonButton::setPosition(float x, float y) {
    hitbox.setPosition(x, y);
    sprite.setPosition(x, y);
    ButtonText.setCenter(hitbox.getCenter());
}

void PolygonButton::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    if (visible) {
        target.draw(sprite, states);
        target.draw(ButtonText, states);
    }
}

bool PolygonButton::isActivated(sf::Event& event) {
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
