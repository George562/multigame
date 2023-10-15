#pragma once
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
        Velocity = {0, 0}; MaxVelocity = 4.f;
        Acceleration = MaxVelocity / 0.6f;
        Radius = 60;
        CurWeapon = new Pistol();
        CurWeapon->damage = 0;

        SetAnimation(DistortedScientistTexture, 1, {650, 635}, sf::seconds(1));

        inventory.dropableItems[ItemID::regenDrug] = new Item(ItemID::regenDrug, 1);

        Name.ShowText = true;
    }
    void move(Location* location) override {
        VelocityBuff = 1;

        setTarget(TheWayToPlayer[int(PosY) / size][int(PosX) / size]);
        Creature::move(location);
    }
    void UpdateState() override {
        Mana += ManaRecovery * (localClock->getElapsedTime() - LastStateCheck).asSeconds();
        Health += HealthRecovery * (localClock->getElapsedTime() - LastStateCheck).asSeconds();
        LastStateCheck = localClock->getElapsedTime();
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
        Velocity = {0, 0}; MaxVelocity = 4.f;
        Acceleration = MaxVelocity / 0.6f;
        Radius = 60;
        CurWeapon = new Pistol();
        CurWeapon->damage = 0;

        SetAnimation(ScottpilgrimTexture, 8, {108, 140}, sf::seconds(1));

        inventory.dropableItems[ItemID::regenDrug] = new Item(ItemID::regenDrug, 1);

        Name.ShowText = true;
    }
    void move(Location* location) override {
        VelocityBuff = 1;

        setTarget(TheWayToPlayer[int(PosY) / size][int(PosX) / size]);
        animation->setAnimationLevel(target.x - PosX < 0);
        Creature::move(location);
    }
    void UpdateState() override {
        Mana += ManaRecovery * (localClock->getElapsedTime() - LastStateCheck).asSeconds();
        Health += HealthRecovery * (localClock->getElapsedTime() - LastStateCheck).asSeconds();
        LastStateCheck = localClock->getElapsedTime();
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
        Radius = 60;
        CurWeapon = new Pistol();
        CurWeapon->damage = 0;

        SetAnimation(RamonaFlowersTexture, 8, {125, 157}, sf::seconds(1));

        inventory.dropableItems[ItemID::regenDrug] = new Item(ItemID::regenDrug, 1);

        Name.ShowText = true;
    }
    void move(Location* location) override {
        VelocityBuff = 1;

        setTarget(TheWayToPlayer[int(PosY) / size][int(PosX) / size]);
        animation->setAnimationLevel(target.x - PosX < 0);
        Creature::move(location);
    }
    void UpdateState() override {
        Mana += ManaRecovery * (localClock->getElapsedTime() - LastStateCheck).asSeconds();
        Health += HealthRecovery * (localClock->getElapsedTime() - LastStateCheck).asSeconds();
        LastStateCheck = localClock->getElapsedTime();
    }
};
