#pragma once
#include "interactible.h"
#include "scale.h"

////////////////////////////////////////////////////////////
// Class
////////////////////////////////////////////////////////////

class Box : public Interactable {
public:
    Box() : Interactable() {}
    bool isActivated(Circle&, sf::Event&);
    void setFillingScale(Scale<float>& s);

    Scale<float>* fillingScale; // you need to pour in mana to open (at future)
};

////////////////////////////////////////////////////////////
// Realization
////////////////////////////////////////////////////////////

bool Box::isActivated(Circle& circle, sf::Event& event) {
    if (event.type == sf::Event::KeyPressed && event.key.code == ActivationButton) {
        function((Interactable*)this);
        return true;
    }
    return false;
}

void Box::setFillingScale(Scale<float>& s) { fillingScale = &s; }