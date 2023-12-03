#pragma once
#include "interactible.h"

////////////////////////////////////////////////////////////
// Class
////////////////////////////////////////////////////////////

class Puddle : public Interactable {
public:
    //Effect effect;
    Puddle() : Interactable() {}
    void setFunction(void (*func)(Interactable*)) { function = func; }
    bool isActivated(Circle&, sf::Event&);

    // void GiveEffect(Creature& creature);
};

////////////////////////////////////////////////////////////
// Realization
////////////////////////////////////////////////////////////

bool Puddle::isActivated(Circle& rect, sf::Event& event) {
    if (event.type == sf::Event::KeyPressed && event.key.code == ActivationButton) {
        function((Interactable*)this);
        return true;
    }
    return false;
}

// void Puddle::GiveEffect(Creature& creature) {
    //Effect effect();
    //creature.GetEffect(effect);
// }