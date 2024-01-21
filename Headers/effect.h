#pragma once
#include "creature.h"

enum EffectType {
    Damage,
    Heal
};

class Effect {
public:
    Creature* owner;
    EffectType type;
    float parameter;

    sf::Clock clock;
    sf::Time secs;

    Effect(Creature* owner, EffectType type, float parameter, sf::Clock clock, sf::Time secs) {
        this->owner = owner;
        this->type = type;
        this->parameter = parameter;
        this->clock = clock;
        clock.restart();
        this->secs = secs;
    }
};