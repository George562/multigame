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
    float velocity;
    int NumberOfBulletsPerShot;
    float scatter; // degree
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

    virtual void Update(Rect&) { };

    virtual void Shoot(Rect& player, sf::Vector2f& direction) {
        sf::Vector2f d = direction - player.getPosition() - player.getSize() / 2.f + CameraPos;
        float len = hypotf(d.x, d.y);
        if (len == 0) return;
        d = RotateOn(-M_PI_RAD * (rand() % (int)scatter - scatter / 2), d) * velocity / len;
        sf::Vector2f SpawnPoint(player.getCenter() + sf::Vector2f{d.x * player.Width, d.y * player.Height} * 0.7f / velocity);
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
    Pistol() : Weapon(9, 1, 0.35, 2) { velocity = 10; NumberOfBulletsPerShot = 1;  scatter = 20; Name = "Pistol"; }
    void Update(Rect& player) {
        if (!lock && AmountOfAmmunition.toBottom() != 0 && TimeFromLastShot->getElapsedTime() > FireRate) {
            sf::Vector2f dir = sf::Vector2f(sf::Mouse::getPosition());
            Shoot(player, dir);
        } else if (AmountOfAmmunition.toBottom() == 0) {
            lock = true;
        }
    }
};

// Revolver
class Revolver : public Weapon {
public:
    Revolver() : Weapon(6, 2, 0, 5) { velocity = 16; NumberOfBulletsPerShot = 1;  scatter = 10; Name = "Revolver"; }
    void Update(Rect& player) {
        if (!lock && AmountOfAmmunition.toBottom() != 0) {
            sf::Vector2f dir = sf::Vector2f(sf::Mouse::getPosition());
            Shoot(player, dir);
            lock = true;
        } else if (AmountOfAmmunition.toBottom() == 0) {
            lock = true;
        }
    }
};

// Shotgun
class Shotgun : public Weapon {
public:
    Shotgun() : Weapon(5, 5, 1, 3) { velocity = 10; NumberOfBulletsPerShot = 10;  scatter = 50; Name = "Shotgun"; }
    void Update(Rect& player) {
        if (!lock && AmountOfAmmunition.toBottom() != 0 && TimeFromLastShot->getElapsedTime() > FireRate) {
            Shoot(player);
            lock = true;
        } else if (AmountOfAmmunition.toBottom() == 0) {
            lock = true;
        }
    }
    void Shoot(Rect& player) {
        sf::Vector2f d = (sf::Vector2f)sf::Mouse::getPosition() - player.getPosition() - player.getSize() / 2.f + CameraPos;
        float len = hypotf(d.x, d.y);
        if (len == 0) return;
        d = RotateOn(-M_PI_RAD * scatter / 2.f, d) * velocity / len;
        for (int i = 0; i < NumberOfBulletsPerShot; i++, d = RotateOn(M_PI_RAD * scatter / (NumberOfBulletsPerShot - 1.f), d)) {
            sf::Vector2f SpawnPoint(player.getCenter() + sf::Vector2f{d.x * player.Width, d.y * player.Height} * 0.7f / velocity);
            Bullets.push_back(*(new Bullet(SpawnPoint, d, 1, damage)));
        }
        AmountOfAmmunition -= 1;
        TimeFromLastShot->restart();
    }
};

// Rifle
class Rifle : public Weapon {
public:
    Rifle() : Weapon(25, 1, 0.05, 2) { velocity = 16; NumberOfBulletsPerShot = 10;  scatter = 17; Name = "Rifle"; }
    void Update(Rect& player) {
        if (!lock && AmountOfAmmunition.toBottom() != 0 && TimeFromLastShot->getElapsedTime() > FireRate) {
            sf::Vector2f dir = sf::Vector2f(sf::Mouse::getPosition());
            Shoot(player, dir);
        } else if (AmountOfAmmunition.toBottom() == 0) {
            lock = true;
        }
    }
};

// Bubblegun
class Bubblegun : public Weapon {
public:
    sf::Vector2f position;
    Bubblegun() : Weapon(30, 1, 0.03, 2) { velocity = 3; NumberOfBulletsPerShot = 10;  scatter = 40; Name = "Bubblegun"; }
    void Update(sf::Event& event) {
        if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
            lock = false;
            position = {0, 0};
        }
    };
    void Update(Rect& player) {
        if (!lock && AmountOfAmmunition.toBottom() != 0 && TimeFromLastShot->getElapsedTime() > FireRate) {
            if (position.x == 0 && position.y == 0)
                position = sf::Vector2f(sf::Mouse::getPosition());
            Shoot(player, position);
            if ((--NumberOfBulletsPerShot) == 0) {
                NumberOfBulletsPerShot = 10;
                lock = true;
            }
        } else if (AmountOfAmmunition.toBottom() == 0) {
            lock = true;
        }
    }
    void Shoot(Rect& player, sf::Vector2f& direction) {
        sf::Vector2f d = direction - player.getPosition() - player.getSize() / 2.f + CameraPos;
        float len = hypotf(d.x, d.y);
        if (len == 0) return;
        d = RotateOn(-M_PI_RAD * (rand() % (int)scatter - scatter / 2), d) * velocity / len;
        sf::Vector2f SpawnPoint(player.getCenter() + sf::Vector2f{d.x * player.Width, d.y * player.Height} * 0.7f / velocity);
        Bullets.push_back(*(new Bullet(SpawnPoint, d, 1, damage, Bullet::Bubble, sf::seconds(3))));
        AmountOfAmmunition -= 1;
        TimeFromLastShot->restart();
    }
};

// Armagedon
class Armagedon : public Weapon {
public:
    Armagedon() : Weapon(300, 0.1, 1.f / 16, 3) { velocity = 3; Name = "Armagedon"; }
    void Update(sf::Event& event) {
        if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
            NumberOfBulletsPerShot = 0;
            lock = false;
        } if (event.type == sf::Event::MouseButtonReleased && event.mouseButton.button == sf::Mouse::Left)
            lock = true;
    };
    void Update(Rect& player) {
        if (!lock && AmountOfAmmunition.toBottom() != 0 && TimeFromLastShot->getElapsedTime() > FireRate)
            Shoot(player);
        else if (AmountOfAmmunition.toBottom() == 0)
            lock = true;
    }
    void Shoot(Rect& player) {
        sf::Vector2f d{0, velocity};
        d = RotateOn(float(-M_PI * NumberOfBulletsPerShot) / 12, d);
        sf::Vector2f SpawnPoint(player.getCenter() + sf::Vector2f{d.x * player.Width, d.y * player.Height} * 0.7f / velocity);
        Bullets.push_back(*(new Bullet(SpawnPoint, d, 1, damage)));
        AmountOfAmmunition -= 1;
        NumberOfBulletsPerShot++;
        TimeFromLastShot->restart();
    }
};

// Chaotic
class Chaotic : public Weapon {
public:
    Chaotic() : Weapon(300, 0.1, 1.f / 16, 3) { velocity = 3; Name = "Chaotic"; }
    void Update(Rect& player) {
        if (!lock && AmountOfAmmunition.toBottom() != 0 && TimeFromLastShot->getElapsedTime() > FireRate)
            Shoot(player);
        else if (AmountOfAmmunition.toBottom() == 0)
            lock = true;
    }
    void Shoot(Rect& player) {
        sf::Vector2f d{0, velocity};
        d = RotateOn(float(rand()), d);
        sf::Vector2f SpawnPoint(player.getCenter() + sf::Vector2f{d.x * player.Width, d.y * player.Height} * 0.7f / velocity);
        Bullets.push_back(*(new Bullet(SpawnPoint, d, 1, damage)));
        AmountOfAmmunition -= 1;
        TimeFromLastShot->restart();
    }
};
