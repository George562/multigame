#pragma once
#include "interactible.h"

////////////////////////////////////////////////////////////
// Class
////////////////////////////////////////////////////////////

class Puddle : public Interactible {
public:
    //Effect effect;
    Puddle();
    void setFunction(void (*func)(void)) { function = func; }
    bool CanBeActivated(Circle&);
    bool isActivated(Circle&, sf::Event&);

    void setPosition(float x, float y) { PosX = x; PosY = y; animation->setPosition(PosX, PosY); }
    void setPosition(sf::Vector2f v) { setPosition(v.x, v.y); }

    void setCenter(float x, float y) { setPosition(x - animation->getGlobalSize().x / 2, y - animation->getGlobalSize().y / 2); }
    void setCenter(sf::Vector2f v) { setCenter(v.x, v.y); }
    // void GiveEffect(Creature& creature);
};

////////////////////////////////////////////////////////////
// Realization
////////////////////////////////////////////////////////////

Puddle::Puddle() {
    SetAnimation(PuddleTexture, 9, {100, 100}, sf::seconds(1));
    animation->setSize(sf::Vector2f{90.f, 90.f});
}

bool Puddle::CanBeActivated(Circle& rect) {
    Width = animation->getGlobalSize().x;
    Height = animation->getGlobalSize().y;
    return intersect(rect);
}

bool Puddle::isActivated(Circle& rect, sf::Event& event) {
    if (event.type == sf::Event::KeyPressed && event.key.code == ActivationButton) {
        function();
        return true;
    }
    return false;
}

// void Puddle::GiveEffect(Creature& creature) {
    //Effect effect();
    //creature.GetEffect(effect);
// }