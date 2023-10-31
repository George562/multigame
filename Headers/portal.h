#pragma once
#include "interactible.h"

////////////////////////////////////////////////////////////
// Class
////////////////////////////////////////////////////////////

class Portal : public Interactable {
public:
    Portal() : Interactable() {}
    void setFunction(void (*func)(void)) { function = func; }
    bool isActivated(Circle&, sf::Event&);

    void setSize(float x, float y) { Rect::setSize(x, y); animation->setSize(sf::Vector2f(x, y)); }
    void setSize(sf::Vector2f v) { Rect::setSize(v); animation->setSize(v); }

    void setPosition(float x, float y) { PosX = x; PosY = y; animation->setPosition(PosX, PosY); }
    void setPosition(sf::Vector2f v) { setPosition(v.x, v.y); }

    void setCenter(float x, float y) { setPosition(x - animation->getGlobalSize().x / 2, y - animation->getGlobalSize().y / 2); }
    void setCenter(sf::Vector2f v) { setCenter(v.x, v.y); }
};

////////////////////////////////////////////////////////////
// Realization
////////////////////////////////////////////////////////////

bool Portal::isActivated(Circle& circle, sf::Event& event) {
    if (event.type == sf::Event::KeyPressed && event.key.code == ActivationButton) {
        function();
        return true;
    }
    return false;
}