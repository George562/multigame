#pragma once
#include "creature.h"

std::vector<std::vector<sf::Vector2f>> TheWayToPlayer;

////////////////////////////////////////////////////////////
// Enemy
class Enemy : public Creature {
public:
    Enemy(sf::String name) : Creature(name, faction::Enemy) {}
    ~Enemy() { if (CurWeapon) { delete CurWeapon; } }
};
////////////////////////////////////////////////////////////

// DistortedScientist
class DistortedScientist : public Enemy {
public:
    DistortedScientist() : Enemy("Distorted Scientist") {
        Health = {0,  20,  20}; HealthRecovery = 1;
        Mana   = {0, 100, 100}; ManaRecovery   = 1;
        Armor  = {0, 100, 100};
        Velocity = {0, 0}; MaxVelocity = 400.f;
        Acceleration = MaxVelocity * 5.f;
        Radius = 60.f;
        CurWeapon = new Pistol();

        setAnimation(Textures::DistortedScientist, &Shaders::Map);

        inventory.items[ItemID::regenDrug] = new Item(ItemID::regenDrug, 1);
    }

    void move(Location* location) override {
        VelocityBuff = 1;

        setTarget(TheWayToPlayer[int(PosY) / size][int(PosX) / size]);
        Creature::move(location);
    }
};

// ScottPilgrim
class ScottPilgrim : public Enemy {
public:
    ScottPilgrim() : Enemy("Scott Pilgrim") {
        Health = {0,  20,  20}; HealthRecovery = 1;
        Mana   = {0, 100, 100}; ManaRecovery   = 1;
        Armor  = {0, 100, 100};
        Velocity = {0, 0}; MaxVelocity = 400.f;
        Acceleration = MaxVelocity * 5.f;
        Radius = 60.f;
        CurWeapon = new Pistol();

        setAnimation(Textures::ScottPilgrim, 8, 2, sf::seconds(1), &Shaders::Map);

        inventory.items[ItemID::regenDrug] = new Item(ItemID::regenDrug, 1);
    }

    void move(Location* location) override {
        VelocityBuff = 1;

        setTarget(TheWayToPlayer[int(PosY) / size][int(PosX) / size]);
        animation->setAnimationLevel(target.x - PosX < 0);
        Creature::move(location);
    }
};

// Distorted
class Distorted : public Enemy {
public:
    Distorted() : Enemy("Distorted") {
        Health = {0,  20,  20}; HealthRecovery = 1;
        Mana   = {0, 100, 100}; ManaRecovery   = 1;
        Armor  = {0, 100, 100};
        Velocity = {0, 0}; MaxVelocity = 400.f;
        Acceleration = MaxVelocity * 5.f;
        Radius = 60.f;
        CurWeapon = new Pistol();

        setAnimation(Textures::Distorted, &Shaders::Distortion2);

        inventory.items[ItemID::regenDrug] = new Item(ItemID::regenDrug, 1);
    }

    void move(Location* location) override {
        VelocityBuff = 1;

        setTarget(TheWayToPlayer[int(PosY) / size][int(PosX) / size]);
        Creature::move(location);
    }
};
