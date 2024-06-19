#pragma once

namespace Effects {
    using Type = sf::Uint8;
    enum : Type {
        Damage,
        Heal,
        HPRegen,
        Burn,
        EffectCount,
    };
};

class Effect {
public:
    Effects::Type type;
    std::vector<float> parameters;
    bool active = false;

    sf::Clock* localClock = nullptr;
    sf::Clock* customTickClock = nullptr;
    sf::Time howLongToExist;
    sf::Time customTick;

    Effect(Effects::Type type, std::vector<float> parameters, sf::Time howLongToExist, sf::Time customTick=sf::Time::Zero) {
        this->type = type;
        this->parameters = parameters;
        this->localClock = new sf::Clock();
        this->howLongToExist = howLongToExist;
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