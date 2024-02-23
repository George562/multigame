#pragma once
#include "creature.h"

namespace Effects {
    using Type = sf::Uint8;
    enum : Type {
        Damage,
        Heal
    };
};

class Effect {
public:
    Creature* owner;
    Effects::Type type;
    float parameter;

    sf::Clock* localClock = nullptr;
    sf::Time secs;

    Effect(Creature* owner, Effects::Type type, float parameter, sf::Time secs) {
        this->owner = owner;
        this->type = type;
        this->parameter = parameter;
        this->localClock = new sf::Clock();
        this->secs = secs;
    }
    
    ~Effect() {
        if (localClock) {
            delete localClock;
        }
    }
};