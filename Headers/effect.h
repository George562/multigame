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
    int tacts;

    Effect(Creature* owner, EffectType type, float parameter, int tacts) {
        this->owner = owner;
        this->type = type;
        this->parameter = parameter;
        this->tacts = tacts;
    }
};