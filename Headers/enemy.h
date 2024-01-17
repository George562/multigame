#pragma once
#include "creature.h"

std::vector<std::vector<sf::Vector2f>> TheWayToPlayer;

////////////////////////////////////////////////////////////
// Enemy
class Enemy : public Creature {
public:
    Enemy(sf::String name) : Creature(name, faction::Enemy) {}
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
        Velocity = {0, 0}; MaxVelocity = 4.f;
        Acceleration = MaxVelocity / 0.6f;
        Radius = 60.f;
        CurWeapon = new Pistol();
        CurWeapon->damage = 0;

        setAnimation(Textures::DistortedScientist, 1, 1, sf::seconds(1), &Shaders::Map);

        inventory.dropableItems[ItemID::regenDrug] = new Item(ItemID::regenDrug, 1);

        Name.ShowText = true;
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
        Health = {0, 20, 20}; HealthRecovery = 1;
        Mana = {0, 100, 100}; ManaRecovery = 1;
        ManaRecovery = 1;
        Armor = {0, 100, 100};
        Money = rand() % 100;
        Velocity = {0, 0}; MaxVelocity = 4.f;
        Acceleration = MaxVelocity / 0.6f;
        Radius = 60.f;
        CurWeapon = new Pistol();
        CurWeapon->damage = 0;

        setAnimation(Textures::ScottPilgrim, 8, 2, sf::seconds(1), &Shaders::Map);

        inventory.dropableItems[ItemID::regenDrug] = new Item(ItemID::regenDrug, 1);

        Name.ShowText = true;
    }

    void move(Location* location) override {
        VelocityBuff = 1;

        setTarget(TheWayToPlayer[int(PosY) / size][int(PosX) / size]);
        animation->setAnimationLevel(target.x - PosX < 0);
        Creature::move(location);
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
        Velocity = {0, 0}; MaxVelocity = 4.f;
        Acceleration = MaxVelocity / 0.6f;
        Radius = 60.f;
        CurWeapon = new Pistol();
        CurWeapon->damage = 0;

        setAnimation(Textures::RamonaFlowers, 8, 2, sf::seconds(1), &Shaders::Map);

        inventory.dropableItems[ItemID::regenDrug] = new Item(ItemID::regenDrug, 1);

        // Name.ShowText = true;
    }

    void move(Location* location) override {
        VelocityBuff = 1;

        setTarget(TheWayToPlayer[int(PosY) / size][int(PosX) / size]);
        animation->setAnimationLevel(target.x - PosX < 0);
        Creature::move(location);
    }
};
