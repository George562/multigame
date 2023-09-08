#include "creature.h"

std::vector<std::vector<sf::Vector2f>> TheWayToPlayer;

////////////////////////////////////////////////////////////
// Enemy
class Enemy : public Creature {
public:
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

        // if (TheWayToPlayer.size() != 0) {
        //     std::cout << TheWayToPlayer.size() << ' ' << int(PosY) / size << ' ' << TheWayToPlayer[0].size() << ' ' << int(PosX) / size << '\n';
        //     std::cout << TheWayToPlayer[int(PosY) / size][int(PosX) / size].x << ' ' << TheWayToPlayer[int(PosY) / size][int(PosX) / size].x << '\n';
        // }
        setTarget(TheWayToPlayer[int(PosY) / size][int(PosX) / size]);
        // setTarget({PosX, PosY});
        Creature::move(location);
    }
    void UpdateState() override {

    }
};
