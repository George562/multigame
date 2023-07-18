#include "bullet.h"
#include "bar.h"

////////////////////////////////////////////////////////////
// Weapon
class Weapon {
public:
    Scale<int> ammunition;
    float ManaCost;
    sf::Time FireRate, lastShoot;
    float damage;
    float velocity;
    int count;
    float scatter; // degree
    bool lock;
    
    Bar<int> AmmoBar;
    PlacedText NameText;

    Weapon() {};
    Weapon(int MaxAmmo, float ManaCost, float FireRate, int dmg) {
        this->ammunition = {0, MaxAmmo, MaxAmmo};
        this->ManaCost = ManaCost;
        this->FireRate = sf::seconds(FireRate);
        this->damage = dmg;
        lastShoot = sf::seconds(0);
        lock = true;

        AmmoBar.setWidth(160);
        AmmoBar.setPosition(scw - AmmoBar.getSize().x - 10, 120);
        AmmoBar.value = &ammunition;
        AmmoBar.setColors(sf::Color(255, 255, 255, 160), sf::Color(128, 128, 128, 160), sf::Color(32, 32, 32, 160));

        NameText.setPosition(scw - AmmoBar.getSize().x - 10, AmmoBar.getPosition().y + AmmoBar.getSize().y);
        NameText.text.setFillColor(sf::Color(25, 192, 25, 160));
    }

    virtual void Update(sf::Event& event) {
        if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left)
            lock = false;
        if (event.type == sf::Event::MouseButtonReleased && event.mouseButton.button == sf::Mouse::Left)
            lock = true;
    };

    virtual void Update(Rect&) { AmmoBar.Update(); };

    virtual void Shoot(Rect& player, sf::Vector2f& direction) {
        sf::Vector2f d = direction - player.getPosition() - player.getSize() / 2.f + CameraPos;
        float len = hypotf(d.x, d.y);
        if (len == 0) return;
        d = RotateOn(-M_PI * (rand() % (int)scatter - scatter / 2) / 180, d) * velocity / len;
        sf::Vector2f SpawnPoint(player.getCenter() + sf::Vector2f{d.x * player.Width, d.y * player.Height} * 0.7f / velocity);
        Bullets.push_back(*(new Bullet(SpawnPoint, d, 1, damage)));
        ammunition -= 1;
        lastShoot = GlobalClock.getElapsedTime();
    }

    virtual void Reload(Scale<float>& Mana) {
        int x = std::min(int(Mana.cur / ManaCost), ammunition.fromTop());
        Mana -= ManaCost * x;
        ammunition += x;
    };

    virtual void interface(sf::RenderTarget& window) {
        window.draw(AmmoBar);
        window.draw(NameText);
    }
};
////////////////////////////////////////////////////////////

// Pistol
class Pistol : public Weapon {
public:
    Pistol() : Weapon(9, 1, 0.35, 2) { velocity = 10; count = 1;  scatter = 20; NameText.setText("Pistol"); }
    void Update(Rect& player) {
        if (!lock && ammunition.toBottom() != 0 && GlobalClock.getElapsedTime() - lastShoot > FireRate) {
            sf::Vector2f dir = sf::Vector2f(sf::Mouse::getPosition());
            Shoot(player, dir);
        } else if (ammunition.toBottom() == 0) {
            lock = true;
        }
        AmmoBar.Update();
    }
};

// Revolver
class Revolver : public Weapon {
public:
    Revolver() : Weapon(6, 2, 0, 5) { velocity = 16; count = 1;  scatter = 10; NameText.setText("Revolver"); }
    void Update(Rect& player) {
        if (!lock && ammunition.toBottom() != 0) {
            sf::Vector2f dir = sf::Vector2f(sf::Mouse::getPosition());
            Shoot(player, dir);
            lock = true;
        } else if (ammunition.toBottom() == 0) {
            lock = true;
        }
        AmmoBar.Update();
    }
};

// Shotgun
class Shotgun : public Weapon {
public:
    Shotgun() : Weapon(5, 5, 1, 3) { velocity = 10; count = 10;  scatter = 50; NameText.setText("Shotgun"); }
    void Update(Rect& player) {
        if (!lock && ammunition.toBottom() != 0 && GlobalClock.getElapsedTime() - lastShoot > FireRate) {
            Shoot(player);
            lock = true;
        } else if (ammunition.toBottom() == 0) {
            lock = true;
        }
        AmmoBar.Update();
    }
    void Shoot(Rect& player) {
        sf::Vector2f d = (sf::Vector2f)sf::Mouse::getPosition() - player.getPosition() - player.getSize() / 2.f + CameraPos;
        float len = hypotf(d.x, d.y);
        if (len == 0) return;
        d = RotateOn(-M_PI * scatter / float(180 * 2), d) * velocity / len;
        for (int i = 0; i < count; i++, d = RotateOn(M_PI * scatter / float(180 * count), d)) {
            sf::Vector2f SpawnPoint(player.getCenter() + sf::Vector2f{d.x * player.Width, d.y * player.Height} * 0.7f / velocity);
            Bullets.push_back(*(new Bullet(SpawnPoint, d, 1, damage)));
        }
        ammunition -= 1;
        lastShoot = GlobalClock.getElapsedTime();
    }
};

// Rifle
class Rifle : public Weapon {
public:
    Rifle() : Weapon(25, 1, 0.05, 2) { velocity = 16; count = 10;  scatter = 17; NameText.setText("Rifle"); }
    void Update(Rect& player) {
        if (!lock && ammunition.toBottom() != 0 && GlobalClock.getElapsedTime() - lastShoot > FireRate) {
            sf::Vector2f dir = sf::Vector2f(sf::Mouse::getPosition());
            Shoot(player, dir);
        } else if (ammunition.toBottom() == 0) {
            lock = true;
        }
        AmmoBar.Update();
    }
};

// Bubblegun
class Bubblegun : public Weapon {
public:
    sf::Vector2f position;
    Bubblegun() : Weapon(30, 1, 0.03, 2) { velocity = 3; count = 10;  scatter = 40; NameText.setText("Bubblegun"); }
    void Update(sf::Event& event) {
        if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
            lock = false;
            position = {0, 0};
        }
    };
    void Update(Rect& player) {
        if (!lock && ammunition.toBottom() != 0 && GlobalClock.getElapsedTime() - lastShoot > FireRate) {
            if (position.x == 0 && position.y == 0)
                position = sf::Vector2f(sf::Mouse::getPosition());
            Shoot(player, position);
            if ((--count) == 0) {
                count = 10;
                lock = true;
            }
        } else if (ammunition.toBottom() == 0) {
            lock = true;
        }
        AmmoBar.Update();
    }
    void Shoot(Rect& player, sf::Vector2f& direction) {
        sf::Vector2f d = direction - player.getPosition() - player.getSize() / 2.f + CameraPos;
        float len = hypotf(d.x, d.y);
        if (len == 0) return;
        d = RotateOn(-M_PI * (rand() % (int)scatter - scatter / 2) / 180, d) * velocity / len;
        sf::Vector2f SpawnPoint(player.getCenter() + sf::Vector2f{d.x * player.Width, d.y * player.Height} * 0.7f / velocity);
        Bullets.push_back(*(new Bullet(SpawnPoint, d, 1, damage, Bullet::Bubble, sf::seconds(3) + GlobalClock.getElapsedTime())));
        ammunition -= 1;
        lastShoot = GlobalClock.getElapsedTime();
    }
};

// Armagedon
class Armagedon : public Weapon {
public:
    Armagedon() : Weapon(300, 0.1, 1.f / 16, 3) { velocity = 3; NameText.setText("Armagedon"); }
    void Update(sf::Event& event) {
        if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
            count = 0;
            lock = false;
        } if (event.type == sf::Event::MouseButtonReleased && event.mouseButton.button == sf::Mouse::Left)
            lock = true;
    };
    void Update(Rect& player) {
        if (!lock && ammunition.toBottom() != 0 && GlobalClock.getElapsedTime() - lastShoot > FireRate)
            Shoot(player);
        else if (ammunition.toBottom() == 0)
            lock = true;
        AmmoBar.Update();
    }
    void Shoot(Rect& player) {
        sf::Vector2f d{0, velocity};
        d = RotateOn(float(-M_PI * count) / 12, d);
        sf::Vector2f SpawnPoint(player.getCenter() + sf::Vector2f{d.x * player.Width, d.y * player.Height} * 0.7f / velocity);
        Bullets.push_back(*(new Bullet(SpawnPoint, d, 1, damage)));
        ammunition -= 1;
        count++;
        lastShoot = GlobalClock.getElapsedTime();
    }
};

// Chaotic
class Chaotic : public Weapon {
public:
    Chaotic() : Weapon(300, 0.1, 1.f / 16, 3) { velocity = 3; NameText.setText("Chaotic"); }
    void Update(Rect& player) {
        if (!lock && ammunition.toBottom() != 0 && GlobalClock.getElapsedTime() - lastShoot > FireRate)
            Shoot(player);
        else if (ammunition.toBottom() == 0)
            lock = true;
        AmmoBar.Update();
    }
    void Shoot(Rect& player) {
        sf::Vector2f d{0, velocity};
        d = RotateOn(float(rand()), d);
        sf::Vector2f SpawnPoint(player.getCenter() + sf::Vector2f{d.x * player.Width, d.y * player.Height} * 0.7f / velocity);
        Bullets.push_back(*(new Bullet(SpawnPoint, d, 1, damage)));
        ammunition -= 1;
        lastShoot = GlobalClock.getElapsedTime();
    }
};
