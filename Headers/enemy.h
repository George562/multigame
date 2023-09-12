#include "creature.h"

std::vector<std::vector<sf::Vector2f>> TheWayToPlayer;

////////////////////////////////////////////////////////////
// Enemy
class Enemy : public Creature {
public:
    Enemy(str name) : Creature(name, Fraction::Enemy) {}
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
        Acceleration = 0.25;
        Radius = 60;
        CurWeapon = new Pistol();
        CurWeapon->damage = 0;

        SetAnimation("sources/textures/DistortedScientist.png", 1, {650, 635}, sf::seconds(1));

        Name.ShowText = true;
    }
    void move(Location& location) override {
        VelocityBuff = 1;

        setTarget(TheWayToPlayer[int(PosY) / size][int(PosX) / size]);
        Creature::move(location);
    }
    void UpdateState() override {
        Mana += ManaRecovery * (localClock->getElapsedTime() - LastCheck).asSeconds();
        Health += HealthRecovery * (localClock->getElapsedTime() - LastCheck).asSeconds();
        LastCheck = localClock->getElapsedTime();
    }
};

// ScottPiligrim
class ScottPiligrim : public Enemy {
public:
    ScottPiligrim() : Enemy("Scott Piligrim") {
        Health = {0, 20, 20}; HealthRecovery = 1;
        Mana = {0, 100, 100}; ManaRecovery = 1;
        ManaRecovery = 1;
        Armor = {0, 100, 100};
        Money = rand() % 100;
        Velocity = {0, 0}; MaxVelocity = 4;
        Acceleration = 0.3;
        Radius = 60;
        CurWeapon = new Pistol();
        CurWeapon->damage = 0;

        SetAnimation("sources/textures/scottpilgrim_multiple.png", 8, {108, 140}, sf::seconds(1));

        Name.ShowText = true;
    }
    void move(Location& location) override {
        VelocityBuff = 1;

        setTarget(TheWayToPlayer[int(PosY) / size][int(PosX) / size]);
        animationSprite->animationLevel = target.x - PosX < 0;
        Creature::move(location);
    }
    void UpdateState() override {
        Mana += ManaRecovery * (localClock->getElapsedTime() - LastCheck).asSeconds();
        Health += HealthRecovery * (localClock->getElapsedTime() - LastCheck).asSeconds();
        LastCheck = localClock->getElapsedTime();
    }
};

// RamonaFlowers
class RamonaFlowers : public Enemy {
public:
    RamonaFlowers() : Enemy("Ramona Flowers") {
        Health = {0, 20, 20}; HealthRecovery = 1;
        Mana = {0, 100, 100}; ManaRecovery = 1;
        ManaRecovery = 1;
        Armor = {0, 100, 100};
        Money = rand() % 100;
        Velocity = {0, 0}; MaxVelocity = 4;
        Acceleration = 0.3;
        Radius = 60;
        CurWeapon = new Pistol();
        CurWeapon->damage = 0;

        SetAnimation("sources/textures/ramonaflowers_multiple.png", 8, {125, 157}, sf::seconds(1));

        Name.ShowText = true;
    }
    void move(Location& location) override {
        VelocityBuff = 1;

        setTarget(TheWayToPlayer[int(PosY) / size][int(PosX) / size]);
        animationSprite->animationLevel = target.x - PosX < 0;
        Creature::move(location);
    }
    void UpdateState() override {
        Mana += ManaRecovery * (localClock->getElapsedTime() - LastCheck).asSeconds();
        Health += HealthRecovery * (localClock->getElapsedTime() - LastCheck).asSeconds();
        LastCheck = localClock->getElapsedTime();
    }
};