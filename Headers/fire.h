#pragma once
#include "interactible.h"

class Fire : public Interactable {
public:
    sf::Clock* localClock = nullptr;
    sf::Time secs;

    Fire(sf::Time secs) : Interactable() {
        this->localClock = new sf::Clock();
        this->secs = secs;
    }
    ~Fire() { if (localClock) delete localClock; }
};