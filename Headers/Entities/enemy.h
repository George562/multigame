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
        Health = {0, 10 + 4.f * curLevel, 10 + 4.f * curLevel};
        HealthRecovery = 1;
        Mana   = {0, 10, 10};
        ManaRecovery   = 1;
        Armor  = {0, 100, 100};
        Velocity = {0, 0}; MaxVelocity = 400.f;
        Acceleration = MaxVelocity * 5.f;
        hitbox.setRadius(60.f);
        CurWeapon = new Pistol();
        CurWeapon->ManaStorage.top = 100.f * CurWeapon->ManaCostOfBullet;
        CurWeapon->ManaStorage.cur = 100.f * CurWeapon->ManaCostOfBullet;
        CurWeapon->ManaCostOfBullet.stats[CurWeapon->ManaCostOfBullet.curLevel] += curLevel;

        setAnimation(Textures::DistortedScientist);

        addItem(new Item(ItemID::regenDrug, 1));
    }

    void move(Location* location) override {
        VelocityBuff = 1;

        setTarget(TheWayToPlayer[int(hitbox.getCenter().y) / size][int(hitbox.getCenter().x) / size]);
        Creature::move(location);
    }
};

// Distorted
class Distorted : public Enemy {
public:
    Distorted() : Enemy("Distorted") {
        Health = {0, 10 + 4.f * curLevel, 10 + 4.f * curLevel};
        HealthRecovery = 1;
        Mana   = {0, 10, 10};
        ManaRecovery   = 1;
        Armor  = {0, 100, 100};
        Velocity = {0, 0}; MaxVelocity = 400.f;
        Acceleration = MaxVelocity * 5.f;
        hitbox.setRadius(60.f);
        CurWeapon = new Pistol();
        CurWeapon->ManaStorage.top = 100.f * CurWeapon->ManaCostOfBullet;
        CurWeapon->ManaStorage.cur = 100.f * CurWeapon->ManaCostOfBullet;
        CurWeapon->ManaCostOfBullet.stats[CurWeapon->ManaCostOfBullet.curLevel] += curLevel;

        setAnimation(Textures::Distorted, &Shaders::Distortion2);

        addItem(new Item(ItemID::regenDrug, 1));
    }

    void move(Location* location) override {
        VelocityBuff = 1;

        setTarget(TheWayToPlayer[int(hitbox.getCenter().y) / size][int(hitbox.getCenter().x) / size]);
        Creature::move(location);
    }
};
