#pragma once
#include "interactible.h"

class Fire : public Interactable {
public:
    sf::Clock* localClock = nullptr;
    sf::Time howLongToExist;

    Fire(sf::Time howLongToExist) : Interactable() {
        this->localClock = new sf::Clock();
        this->howLongToExist = howLongToExist;
    }
    ~Fire() {
        if (localClock) {
            delete localClock;
        }
    }
};