#pragma once
#include "creature.h"

std::vector<std::vector<sf::Vector2f>> TheWayToPlayer;

////////////////////////////////////////////////////////////
// Enemy
class Enemy : public Creature {
public:
    Enemy(std::string name) : Creature(name, faction::Enemy) {}
    ~Enemy() { if (CurWeapon) { delete CurWeapon; } }
};
////////////////////////////////////////////////////////////

// DistortedScientist
class DistortedScientist : public Enemy {
public:
    DistortedScientist() : Enemy("Distorted Scientist") {
		Health = { {0, 10 + 4.f * curLevel, 10 + 4.f * curLevel} };
        HealthRecovery = 1;
		Mana = { {0, 10, 10} };
        ManaRecovery   = 1;
        Armor  = { {0, 100, 100} };
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

    void shift(sf::Vector2f shift) override { Velocity += normalize(shift) * 4.f; }
    void shift(float x, float y) override { Velocity += normalize(sf::Vector2f(x, y)) * 4.f; }
};

// Distorted
class Distorted : public Enemy {
public:
    Distorted() : Enemy("Distorted") {
		Health = { {0, 10 + 4.f * curLevel, 10 + 4.f * curLevel} };
        HealthRecovery = 1;
		Mana = { {0, 10, 10} };
        ManaRecovery   = 1;
		Armor = { {0, 100, 100} };
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

    void shift(sf::Vector2f shift) override { Velocity += normalize(shift) * 4.f; }
    void shift(float x, float y) override { Velocity += normalize(sf::Vector2f(x, y)) * 4.f; }
};

sf::Packet& operator<<(sf::Packet& packet, Enemy e) {
    return packet << e.Name << e.hitbox.getCenter();
}
sf::Packet& operator>>(sf::Packet& packet, Enemy e) {
    sf::Vector2f v; packet >> v;
    e.hitbox.setCenter(v);
    return packet;
}