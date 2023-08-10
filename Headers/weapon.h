#include "bullet.h"
#include "bar.h"

#define M_PI_RAD M_PI / 180.f

////////////////////////////////////////////////////////////
// Weapon
class Weapon {
public:
    Scale<int> AmountOfAmmunition;
    float ManaCost;
    sf::Time FireRate;
    sf::Clock* TimeFromLastShot;
    float damage;
    float BulletVelocity;
    int NumberOfBulletsPerShot;
    float scatter; // at degree
    bool lock; // Bullets is like a stream and "lock" is blocking it stream
    
    str Name;

    Weapon() {};
    Weapon(int MaxAmmo, float ManaCost, float FireRate, float dmg) {
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
    };

    virtual void Shoot(Rect& shooter, sf::Vector2f direction = {0, 0}) {
        if (AmountOfAmmunition.toBottom() == 0) { lock = true; return; }
        if (lock || TimeFromLastShot->getElapsedTime() <= FireRate) return;

        sf::Vector2f d = direction - shooter.getPosition() - shooter.getSize() / 2.f;
        float len = hypotf(d.x, d.y);
        if (len == 0) return;
        d = RotateOn(-M_PI_RAD * (rand() % (int)scatter - scatter / 2), d) * BulletVelocity / len;
        sf::Vector2f SpawnPoint(shooter.getCenter() + sf::Vector2f{d.x * shooter.Width, d.y * shooter.Height} * 0.7f / BulletVelocity);
        Bullets.push_back(*(new Bullet(SpawnPoint, d, 1, damage)));
        AmountOfAmmunition -= 1;
        TimeFromLastShot->restart();
    }

    virtual void Reload(Scale<float>& Mana) {
        int x = std::min(int(Mana.cur / ManaCost), AmountOfAmmunition.fromTop());
        Mana -= ManaCost * x;
        AmountOfAmmunition += x;
    };
};
////////////////////////////////////////////////////////////

// Pistol
class Pistol : public Weapon {
public:
    Pistol() : Weapon(9, 1, 1, 2) { BulletVelocity = 10; NumberOfBulletsPerShot = 1;  scatter = 20; Name = "Pistol"; }
};

// Revolver
class Revolver : public Weapon {
public:
    Revolver() : Weapon(6, 2, 0, 5) { BulletVelocity = 16; NumberOfBulletsPerShot = 1;  scatter = 10; Name = "Revolver"; }
    void Shoot(Rect& shooter, sf::Vector2f direction = {0, 0}) {
        Weapon::Shoot(shooter, direction);
        lock = true;
    }
};

// Shotgun
class Shotgun : public Weapon {
public:
    Shotgun() : Weapon(5, 5, 1, 3) { BulletVelocity = 10; NumberOfBulletsPerShot = 10;  scatter = 50; Name = "Shotgun"; }
    void Shoot(Rect& shooter, sf::Vector2f direction = {0, 0}) {
        if (AmountOfAmmunition.toBottom() == 0) { lock = true; return; }
        if (lock || TimeFromLastShot->getElapsedTime() <= FireRate) return;

        sf::Vector2f d = direction - shooter.getPosition() - shooter.getSize() / 2.f;
        float len = hypotf(d.x, d.y);
        if (len == 0) return;
        d = RotateOn(-M_PI_RAD * scatter / 2.f, d) * BulletVelocity / len;
        for (int i = 0; i < NumberOfBulletsPerShot; i++, d = RotateOn(M_PI_RAD * scatter / (NumberOfBulletsPerShot - 1.f), d)) {
            sf::Vector2f SpawnPoint(shooter.getCenter() + sf::Vector2f{d.x * shooter.Width, d.y * shooter.Height} * 0.7f / BulletVelocity);
            Bullets.push_back(*(new Bullet(SpawnPoint, d, 1, damage)));
        }
        AmountOfAmmunition -= 1;
        TimeFromLastShot->restart();
        lock = true;
    }
};

// Rifle
class Rifle : public Weapon {
public:
    Rifle() : Weapon(25, 1, 0.05, 2) { BulletVelocity = 16; NumberOfBulletsPerShot = 10;  scatter = 17; Name = "Rifle"; }
};

// Bubblegun
class Bubblegun : public Weapon {
public:
    sf::Vector2f position;
    Bubblegun() : Weapon(30, 1, 0.03, 2) { BulletVelocity = 3; NumberOfBulletsPerShot = 10;  scatter = 40; Name = "Bubblegun"; }
    void Shoot(Rect& shooter, sf::Vector2f direction = {0, 0}) {
        if (AmountOfAmmunition.toBottom() == 0) { lock = true; return; }
        if (lock || TimeFromLastShot->getElapsedTime() <= FireRate) return;
        sf::Vector2f d = direction - shooter.getPosition() - shooter.getSize() / 2.f;
        float len = hypotf(d.x, d.y);
        if (len == 0) return;
        d = RotateOn(-M_PI_RAD * (rand() % (int)scatter - scatter / 2), d) * BulletVelocity / len;
        sf::Vector2f SpawnPoint(shooter.getCenter() + sf::Vector2f{d.x * shooter.Width, d.y * shooter.Height} * 0.7f / BulletVelocity);
        Bullets.push_back(*(new Bullet(SpawnPoint, d, 1, damage, Bullet::Bubble, sf::seconds(3))));
        AmountOfAmmunition -= 1;
        TimeFromLastShot->restart();
        if ((--NumberOfBulletsPerShot) == 0) {
            NumberOfBulletsPerShot = 10;
            lock = true;
        }
    }
};

// Armagedon
class Armagedon : public Weapon {
public:
    Armagedon() : Weapon(300, 0.1, 1.f / 16, 3) { BulletVelocity = 3; Name = "Armagedon"; }
    void Update(sf::Event& event) {
        if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
            NumberOfBulletsPerShot = 0;
            lock = false;
        } if (event.type == sf::Event::MouseButtonReleased && event.mouseButton.button == sf::Mouse::Left)
            lock = true;
    };
    void Shoot(Rect& shooter, sf::Vector2f direction = {0, 0}) {
        if (AmountOfAmmunition.toBottom() == 0) { lock = true; return; }
        if (lock || TimeFromLastShot->getElapsedTime() <= FireRate) return;
        
        sf::Vector2f d{0, BulletVelocity};
        d = RotateOn(float(-M_PI * NumberOfBulletsPerShot) / 12, d);
        sf::Vector2f SpawnPoint(shooter.getCenter() + sf::Vector2f{d.x * shooter.Width, d.y * shooter.Height} * 0.7f / BulletVelocity);
        Bullets.push_back(*(new Bullet(SpawnPoint, d, 1, damage)));
        AmountOfAmmunition -= 1;
        NumberOfBulletsPerShot++;
        TimeFromLastShot->restart();
    }
};

// Chaotic
class Chaotic : public Weapon {
public:
    Chaotic() : Weapon(300, 0.1, 1.f / 16, 3) { BulletVelocity = 3; Name = "Chaotic"; }
    void Shoot(Rect& shooter, sf::Vector2f direction = {0, 0}) {
        if (AmountOfAmmunition.toBottom() == 0) { lock = true; return; }
        if (lock || TimeFromLastShot->getElapsedTime() <= FireRate) return;
        
        sf::Vector2f d{0, BulletVelocity};
        d = RotateOn(float(rand()), d);
        sf::Vector2f SpawnPoint(shooter.getCenter() + sf::Vector2f{d.x * shooter.Width, d.y * shooter.Height} * 0.7f / BulletVelocity);
        Bullets.push_back(*(new Bullet(SpawnPoint, d, 1, damage)));
        AmountOfAmmunition -= 1;
        TimeFromLastShot->restart();
    }
};
