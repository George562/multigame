#pragma once
#include "interactible.h"

////////////////////////////////////////////////////////////
// Class
////////////////////////////////////////////////////////////

class Portal : public Interactable {
public:
    Portal() : Interactable() {}
    void setFunction(void (*func)(Interactable*)) { function = func; }
    bool isActivated(Circle&, sf::Event&);
};

////////////////////////////////////////////////////////////
// Realization
////////////////////////////////////////////////////////////

bool Portal::isActivated(Circle& circle, sf::Event& event) {
    if (event.type == sf::Event::KeyPressed && event.key.code == ActivationButton) {
        function((Interactable*)this);
        return true;
    }
    return false;
}