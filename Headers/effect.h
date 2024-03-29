#pragma once
#include "creature.h"

namespace Effects {
    using Type = sf::Uint8;
    enum : Type {
        Damage,
        Heal,
        HPRegen,
        Burn,
        NONE,
    };
};

class Effect {
public:
    Creature* owner;
    Effects::Type type;
    std::vector<float> parameters;
    bool active = false;

    sf::Clock* localClock = nullptr;
    sf::Clock* customTickClock = nullptr;
    sf::Time secs;
    sf::Time customTick;

    Effect(Creature* owner, Effects::Type type, std::vector<float> parameters, sf::Time secs, sf::Time customTick=sf::Time::Zero) {
        this->owner = owner;
        this->type = type;
        this->parameters = parameters;
        this->localClock = new sf::Clock();
        this->secs = secs;
        if (customTick != sf::Time::Zero) {
            this->customTick = customTick;
            this->customTickClock = new sf::Clock();
        }
    }
    
    ~Effect() {
        if (localClock) {
            delete localClock;
        }
        if (customTickClock) {
            delete customTickClock;
        }
    }
};