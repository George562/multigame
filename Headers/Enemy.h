#include "creature.h"

////////////////////////////////////////////////////////////
// Enemy
class Enemy : public Creature {
public:
    std::vector<sf::Vector2f> TheWay;

    Enemy(str name) : Creature(name) {}
    virtual void move(Location&) {}
    virtual void UpdateState() {}
};
////////////////////////////////////////////////////////////

// Scientific
class Scientific : public Enemy {
public:
    Scientific() : Enemy("Scientific") {
        Health = {0, 20, 20}; HealthRecovery = 1;
        Mana = {0, 100, 100}; ManaRecovery = 1;
        ManaRecovery = 1;
        Armor = {0, 100, 100};
        Money = rand() % 100;
        Velocity = {0, 0}; MaxVelocity = 4;
        Acceleration = 0.3;
        radius = 60;
        Width = Height = radius * 2;
        CurWeapon = new Pistol();

        SetTexture("sources/textures/Scientific");

        Name.ShowText = true;
    }
    void move(Location& location) override {
        VelocityBuf = 1;

        if (TheWay.size() == 0)
            FindTheWay(location, getCenter(), target, TheWay);
        if (sf::Vector2i(getCenter()) / size == sf::Vector2i(TheWay[TheWay.size() - 1]) / size)
            FindTheWay(location, getCenter(), target, TheWay);
        
        sf::Vector2f dist = TheWay[TheWay.size() - 1] - getCenter();
        sf::Vector2f wasTarget = target;
        setTarget(TheWay[TheWay.size() - 1] + sf::Vector2f{float(rand() % 20), float(rand() % 20)});
        Creature::move(location);
        setTarget(wasTarget);
    }
    void UpdateState() override {

    }
};
