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
        Health = {0, 20 + 5.f * curLevel, 20 + 5.f * curLevel};
        HealthRecovery = 1;
        Mana   = {0, 100, 100};
        ManaRecovery   = 1;
        Armor  = {0, 100, 100};
        Velocity = {0, 0}; MaxVelocity = 400.f;
        Acceleration = MaxVelocity * 5.f;
        setRadius(60.f);
        CurWeapon = new Pistol();
        CurWeapon->damage += curLevel;

        setAnimation(Textures::DistortedScientist, &Shaders::Map);

        addItem(new Item(ItemID::regenDrug, 1));
    }

    void move(Location* location) override {
        VelocityBuff = 1;

        setTarget(TheWayToPlayer[int(getCenter().y) / size][int(getCenter().x) / size]);
        Creature::move(location);
    }
};

// ScottPilgrim
class ScottPilgrim : public Enemy {
public:
    ScottPilgrim() : Enemy("Scott Pilgrim") {
        Health = {0, 20 + 5.f * curLevel, 20 + 5.f * curLevel};
        HealthRecovery = 1;
        Mana   = {0, 100, 100};
        ManaRecovery   = 1;
        Armor  = {0, 100, 100};
        Velocity = {0, 0}; MaxVelocity = 400.f;
        Acceleration = MaxVelocity * 5.f;
        setRadius(60.f);
        CurWeapon = new Pistol();
        CurWeapon->damage += curLevel;

        setAnimation(Textures::ScottPilgrim, 8, 2, sf::seconds(1), &Shaders::Map);

        addItem(new Item(ItemID::regenDrug, 1));
    }

    void move(Location* location) override {
        VelocityBuff = 1;

        setTarget(TheWayToPlayer[int(getCenter().y) / size][int(getCenter().x) / size]);
        animation->setAnimationLevel(target.x - getCenter().x < 0);
        Creature::move(location);
    }
};

// Distorted
class Distorted : public Enemy {
public:
    Distorted() : Enemy("Distorted") {
        Health = {0, 20 + 5.f * curLevel, 20 + 5.f * curLevel};
        HealthRecovery = 1;
        Mana   = {0, 100, 100};
        ManaRecovery   = 1;
        Armor  = {0, 100, 100};
        Velocity = {0, 0}; MaxVelocity = 400.f;
        Acceleration = MaxVelocity * 5.f;
        setRadius(60.f);
        CurWeapon = new Pistol();
        CurWeapon->damage += curLevel;

        setAnimation(Textures::Distorted, &Shaders::Distortion2);

        addItem(new Item(ItemID::regenDrug, 1));
    }

    void move(Location* location) override {
        VelocityBuff = 1;

        setTarget(TheWayToPlayer[int(getCenter().y) / size][int(getCenter().x) / size]);
        Creature::move(location);
    }
};
