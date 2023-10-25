#pragma once
#include "interactible.h"

////////////////////////////////////////////////////////////
// Class
////////////////////////////////////////////////////////////

class Portal : public Interactible {
public:
    Portal();
    void setFunction(void (*func)(void)) { function = func; }
    bool CanBeActivated(Circle&);
    bool isActivated(Circle&, sf::Event&);

    void setPosition(float x, float y) { PosX = x; PosY = y; animation->setPosition(PosX, PosY); }
    void setPosition(sf::Vector2f v) { setPosition(v.x, v.y); }

    void setCenter(float x, float y) { setPosition(x - animation->getGlobalSize().x / 2, y - animation->getGlobalSize().y / 2); }
    void setCenter(sf::Vector2f v) { setCenter(v.x, v.y); }
};

////////////////////////////////////////////////////////////
// Realization
////////////////////////////////////////////////////////////

Portal::Portal() {
    // SetAnimation(PortalAnimation2Texture, 1, {135, 270}, sf::seconds(1));
    SetAnimation(PortalAnimation2Texture, 9, {340, 640}, sf::seconds(1));
    animation->setSize(sf::Vector2f{170.f, 320.f});
}

bool Portal::CanBeActivated(Circle& rect) {
    Width = animation->getGlobalSize().x;
    Height = animation->getGlobalSize().y;
    return intersect(rect); }

bool Portal::isActivated(Circle& rect, sf::Event& event) {
    if (event.type == sf::Event::KeyPressed && event.key.code == ActivationButton) {
        function();
        return true;
    }
    return false;
}