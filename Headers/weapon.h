#pragma once
#include "bullet.h"

#define M_PI_RAD M_PI / 180.f

////////////////////////////////////////////////////////////
// Weapon
#pragma pack(push, 1)
class Weapon {
public:
    sf::String Name;
    Scale<int> AmountOfAmmunition;
    sf::Clock* TimeFromLastShot;
    sf::Time FireRate;
    int NumberOfBulletsPerShot;
    float ManaCost;
    float damage;
    float BulletVelocity;
    float scatter; // at degree
    bool lock; // Bullets is like a stream and "lock" is blocking it stream

    Weapon() {}
    Weapon(sf::String name, int MaxAmmo, float ManaCost, float FireRate, float dmg) {
        Name = name;
        AmountOfAmmunition = {0, MaxAmmo, MaxAmmo};
        this->ManaCost = ManaCost;
        this->FireRate = sf::seconds(FireRate);
        damage = dmg;
        lock = true;
        TimeFromLastShot = new sf::Clock();
    }

    virtual void Update(sf::Event& event) {
        if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left)
            lock = false;
        if (event.type == sf::Event::MouseButtonReleased && event.mouseButton.button == sf::Mouse::Left)
            lock = true;
    }

    virtual void Shoot(Circle& shooter, sf::Vector2f direction, faction::Type f) {
        if (AmountOfAmmunition.toBottom() == 0) { lock = true; return; }
        if (lock || TimeFromLastShot->getElapsedTime() <= FireRate) return;

        sf::Vector2f d = direction - shooter.getPosition();
        float len = hypotf(d.x, d.y);
        if (len == 0) return;
        d = RotateOn(-M_PI_RAD * (rand() % (int)scatter - scatter / 2), d) * BulletVelocity / len;
        sf::Vector2f SpawnPoint(shooter.getPosition() + d * (shooter.Radius * 1.4f) / BulletVelocity);
        Bullets.push_back(new Bullet(f, SpawnPoint, d, damage));
        AmountOfAmmunition -= 1;
        TimeFromLastShot->restart();
    }

    virtual void Reload(Scale<float>& Mana) {
        int x = std::min(int(Mana.cur / ManaCost), AmountOfAmmunition.fromTop());
        Mana -= ManaCost * x;
        AmountOfAmmunition += x;
    }
};
#pragma pack(pop)
////////////////////////////////////////////////////////////

// Pistol
class Pistol : public Weapon {
public:
    Pistol() : Weapon("Pistol", 9, 1, 0.4, 2) { BulletVelocity = 10; NumberOfBulletsPerShot = 1;  scatter = 20; }
};

// Revolver
class Revolver : public Weapon {
public:
    Revolver() : Weapon("Revolver", 6, 2, 0, 5) { BulletVelocity = 16; NumberOfBulletsPerShot = 1;  scatter = 10; }
    void Shoot(Circle& shooter, sf::Vector2f direction, faction::Type f) {
        Weapon::Shoot(shooter, direction, f);
        lock = true;
    }
};

// Shotgun
class Shotgun : public Weapon {
public:
    Shotgun() : Weapon("Shotgun", 5, 10, 1, 3) { BulletVelocity = 10; NumberOfBulletsPerShot = 10;  scatter = 50; }
    void Shoot(Circle& shooter, sf::Vector2f direction, faction::Type f) {
        if (AmountOfAmmunition.toBottom() == 0) { lock = true; return; }
        if (lock || TimeFromLastShot->getElapsedTime() <= FireRate) return;

        sf::Vector2f d = direction - shooter.getPosition();
        float len = hypotf(d.x, d.y);
        if (len == 0) return;
        d = RotateOn(-M_PI_RAD * scatter / 2.f, d) * BulletVelocity / len;
        for (int i = 0; i < NumberOfBulletsPerShot; i++, d = RotateOn(M_PI_RAD * scatter / (NumberOfBulletsPerShot - 1.f), d)) {
            sf::Vector2f SpawnPoint(shooter.getPosition() + d * (shooter.Radius * 1.4f) / BulletVelocity);
            Bullets.push_back(new Bullet(f, SpawnPoint, d, damage));
        }
        AmountOfAmmunition -= 1;
        TimeFromLastShot->restart();
        lock = true;
    }
};

// Rifle
class Rifle : public Weapon {
public:
    Rifle() : Weapon("Rifle", 25, 1.5f, 0.05, 2) { BulletVelocity = 16; NumberOfBulletsPerShot = 10;  scatter = 17; }
};

// Bubblegun
class Bubblegun : public Weapon {
public:
    Bubblegun() : Weapon("Bubblegun", 30, 3, 0.03, 2) { BulletVelocity = 9; NumberOfBulletsPerShot = 10;  scatter = 40; }
    void Update(sf::Event& event) {
        if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left)
            lock = false;
    }
    void Shoot(Circle& shooter, sf::Vector2f direction, faction::Type f) {
        if (AmountOfAmmunition.toBottom() == 0) { lock = true; return; }
        if (lock || TimeFromLastShot->getElapsedTime() <= FireRate) return;
        sf::Vector2f d = direction - shooter.getPosition();
        float len = hypotf(d.x, d.y);
        if (len == 0) return;
        d = RotateOn(-M_PI_RAD * (rand() % (int)scatter - scatter / 2), d) * BulletVelocity / len;
        sf::Vector2f SpawnPoint(shooter.getPosition() + d * (shooter.Radius * 1.4f) / BulletVelocity);
        Bullets.push_back(new Bullet(f, SpawnPoint, d, damage, COMMON_BULLET_PENETRATION, Bullet::Bubble, sf::seconds(1)));
        AmountOfAmmunition -= 1;
        TimeFromLastShot->restart();
        if (--NumberOfBulletsPerShot == 0) {
            NumberOfBulletsPerShot = 10;
            lock = true;
        }
    }
};

// Armageddon
class Armageddon : public Weapon {
public:
    Armageddon() : Weapon("Armageddon", 300, 0.1, 1.f / 16, 3) { BulletVelocity = 3; }
    void Update(sf::Event& event) {
        if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
            NumberOfBulletsPerShot = 0;
            lock = false;
        } if (event.type == sf::Event::MouseButtonReleased && event.mouseButton.button == sf::Mouse::Left)
            lock = true;
    }
    void Shoot(Circle& shooter, sf::Vector2f direction, faction::Type f) {
        if (AmountOfAmmunition.toBottom() == 0) { lock = true; return; }
        if (lock || TimeFromLastShot->getElapsedTime() <= FireRate) return;
        
        sf::Vector2f d{0, BulletVelocity};
        d = RotateOn(float(-M_PI * NumberOfBulletsPerShot) / 12, d);
        sf::Vector2f SpawnPoint(shooter.getPosition() + d * (shooter.Radius * 1.4f) / BulletVelocity);
        Bullets.push_back(new Bullet(f, SpawnPoint, d, damage));
        AmountOfAmmunition -= 1;
        NumberOfBulletsPerShot++;
        TimeFromLastShot->restart();
    }
};

// Chaotic
class Chaotic : public Weapon {
public:
    Chaotic() : Weapon("Chaotic", 300, 0.1, 1.f / 16, 3) { BulletVelocity = 3; }
    void Shoot(Circle& shooter, sf::Vector2f direction, faction::Type f) {
        if (AmountOfAmmunition.toBottom() == 0) { lock = true; return; }
        if (lock || TimeFromLastShot->getElapsedTime() <= FireRate) return;
        
        sf::Vector2f d{0, BulletVelocity};
        d = RotateOn(float(rand()), d);
        sf::Vector2f SpawnPoint(shooter.getPosition() + d * (shooter.Radius * 1.4f) / BulletVelocity);
        Bullets.push_back(new Bullet(f, SpawnPoint, d, damage));
        AmountOfAmmunition -= 1;
        TimeFromLastShot->restart();
    }
};

class FireHose : public Weapon {
public:
    FireHose() : Weapon("Fire hose", 100, 0, 1.f / 10, 0) { BulletVelocity = 20; }
    void Shoot(Circle& shooter, sf::Vector2f direction, faction::Type f) {
        if (AmountOfAmmunition.toBottom() == 0) { lock = true; return; }
        if (lock || TimeFromLastShot->getElapsedTime() <= FireRate) return;
        
        sf::Vector2f d = direction - shooter.getPosition();
        float len = hypotf(d.x, d.y);
        if (len == 0) return;
        d = d * BulletVelocity / len;
        sf::Vector2f SpawnPoint(shooter.getPosition() + d * (shooter.Radius * 1.4f) / BulletVelocity);
        Bullets.push_back(new Bullet(f, SpawnPoint, d, damage, 0, Bullet::Type::WaterParticle, sf::Time::Zero,
                            new Animation(Textures::Puddle, 1, 1, sf::seconds(1), &Shaders::Map)));
        AmountOfAmmunition -= 1;
        TimeFromLastShot->restart();
    }
    void Reload(Scale<float>& Mana) {}
};
