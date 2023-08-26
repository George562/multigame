#include "creature.h"

////////////////////////////////////////////////////////////
// Enemy
class Enemy : public Creature {
public:
    std::vector<sf::Vector2f> TheWay;

    Enemy(str name) : Creature(name, Fraction::Enemy) {}
    virtual void move(Location&) {}
    virtual void UpdateState() {}
};
////////////////////////////////////////////////////////////

// DistortedScientist
class DistortedScientist : public Enemy {
public:
    DistortedScientist() : Enemy("Distorted Scientist") {
        Health = {0, 20, 20}; HealthRecovery = 1;
        Mana = {0, 100, 100}; ManaRecovery = 1;
        ManaRecovery = 1;
        Armor = {0, 100, 100};
        Money = rand() % 100;
        Velocity = {0, 0}; MaxVelocity = 4;
        Acceleration = 0.3;
        Radius = 60;
        CurWeapon = new Pistol();

        SetTexture("sources/textures/DistortedScientist");

        Name.ShowText = true;
    }
    void move(Location& location) override {
        VelocityBuff = 1;

        // if (TheWay.size() == 0)
        //     FindTheWay(location, getPosition(), target, TheWay);
        // if (sf::Vector2i(getPosition()) / size == sf::Vector2i(TheWay[TheWay.size() - 1]) / size)
            FindTheWay(location, getPosition(), target, TheWay);

        sf::Vector2f wasTarget = target;
        setTarget(TheWay.back() + sf::Vector2f{float(rand() % 20), float(rand() % 20)});
        Creature::move(location);
        setTarget(wasTarget);
    }
    void UpdateState() override {

    }
};
