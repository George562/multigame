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
    int mode;
    
    Bar<int> AmmoBar;
    PlacedText NameText;
    Bullet* newBullet;

    Weapon() {};
    Weapon(int MaxAmmo, float ManaCost, float FireRate, int dmg) {
        this->ammunition = {0, MaxAmmo, MaxAmmo};
        this->ManaCost = ManaCost;
        this->FireRate = sf::seconds(FireRate);
        this->damage = dmg;
        lastShoot = sf::seconds(0);
        lock = true;
        mode = 1;

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

    virtual void Update(vB&, Rect&) { AmmoBar.Update(); };

    virtual void Shoot(vB& Bullets, Rect& player, sf::Vector2f& direction) {
        float dx = direction.x - player.PosX - player.Width  / 2 + CameraPos.x,
              dy = direction.y - player.PosY - player.Height / 2 + CameraPos.y;
        float len = hypotf(dx, dy);
        if (len == 0) return;
        RotateOn(-M_PI * (rand() % (int)scatter - scatter / 2) / 180, dx, dy);
        dx *= velocity / len; dy *= velocity / len;
        sf::Vector2f SpawnPoint(player.getCenter() + sf::Vector2f{dx * player.Width, dy * player.Height} * 0.7f / velocity);
        newBullet = new Bullet(SpawnPoint, {dx, dy}, sf::Color(rand() % 256, rand() % 256, rand() % 256),
                               1, damage);
        Bullets.push_back(*newBullet);
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
    void Update(vB& Bullets, Rect& player) {
        if (!lock && ammunition.toBottom() != 0 && GlobalClock.getElapsedTime() - lastShoot > FireRate) {
            sf::Vector2f dir = sf::Vector2f(sf::Mouse::getPosition());
            Shoot(Bullets, player, dir);
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
    void Update(vB& Bullets, Rect& player) {
        if (!lock && ammunition.toBottom() != 0) {
            sf::Vector2f dir = sf::Vector2f(sf::Mouse::getPosition());
            Shoot(Bullets, player, dir);
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
    void Update(vB& Bullets, Rect& player) {
        if (!lock && ammunition.toBottom() != 0 && GlobalClock.getElapsedTime() - lastShoot > FireRate) {
            Shoot(Bullets, player);
            lock = true;
        } else if (ammunition.toBottom() == 0) {
            lock = true;
        }
        AmmoBar.Update();
    }
    void Shoot(vB& Bullets, Rect& player) {
        float dx = sf::Mouse::getPosition().x - player.PosX - player.Width  / 2 + CameraPos.x,
              dy = sf::Mouse::getPosition().y - player.PosY - player.Height / 2 + CameraPos.y;
        float len = hypotf(dx, dy);
        if (len == 0) return;
        dx *= velocity / len; dy *= velocity / len;
        RotateOn(-M_PI * scatter / float(180 * 2), dx, dy);
        for (int i = 0; i < count; i++, RotateOn(M_PI * scatter / float(180 * count), dx, dy)) {
            sf::Vector2f SpawnPoint(player.getCenter() + sf::Vector2f{dx * player.Width, dy * player.Height} * 0.7f / velocity);
            newBullet = new Bullet(SpawnPoint, {dx, dy}, sf::Color(rand() % 256, rand() % 256, rand() % 256),
                                   1, damage);
            Bullets.push_back(*newBullet);
        }
        ammunition -= 1;
        lastShoot = GlobalClock.getElapsedTime();
    }
};

// Rifle
class Rifle : public Weapon {
public:
    Rifle() : Weapon(25, 1, 0.05, 2) { velocity = 16; count = 10;  scatter = 17; NameText.setText("Rifle"); }
    void Update(vB& Bullets, Rect& player) {
        if (!lock && ammunition.toBottom() != 0 && GlobalClock.getElapsedTime() - lastShoot > FireRate) {
            sf::Vector2f dir = sf::Vector2f(sf::Mouse::getPosition());
            Shoot(Bullets, player, dir);
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
    void Update(vB& Bullets, Rect& player) {
        if (!lock && ammunition.toBottom() != 0 && GlobalClock.getElapsedTime() - lastShoot > FireRate) {
            if (position.x == 0 && position.y == 0)
                position = sf::Vector2f(sf::Mouse::getPosition());
            Shoot(Bullets, player, position);
            if ((--count) == 0) {
                count = 10;
                lock = true;
            }
        } else if (ammunition.toBottom() == 0) {
            lock = true;
        }
        AmmoBar.Update();
    }
    void Shoot(vB& Bullets, Rect& player, sf::Vector2f& direction) {
        float dx = direction.x - player.PosX - player.Width  / 2 + CameraPos.x,
              dy = direction.y - player.PosY - player.Height / 2 + CameraPos.y;
        float len = hypotf(dx, dy);
        if (len == 0) return;
        RotateOn(-M_PI * (rand() % (int)scatter - scatter / 2) / 180, dx, dy);
        dx *= velocity / len; dy *= velocity / len;
        sf::Vector2f SpawnPoint(player.getCenter() + sf::Vector2f{dx * player.Width, dy * player.Height} * 0.7f / velocity);
        newBullet = new Bullet(SpawnPoint, {dx, dy}, sf::Color(rand() % 256, rand() % 256, rand() % 256),
                               1, damage, Bullet::Bubble, sf::seconds(3) + GlobalClock.getElapsedTime());
        Bullets.push_back(*newBullet);
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
    void Update(vB& Bullets, Rect& player) {
        if (!lock && ammunition.toBottom() != 0 && GlobalClock.getElapsedTime() - lastShoot > FireRate)
            Shoot(Bullets, player);
        else if (ammunition.toBottom() == 0)
            lock = true;
        AmmoBar.Update();
    }
    void Shoot(vB& Bullets, Rect& player) {
        float dx = 0, dy = velocity;
        float len = hypotf(dx, dy);
        RotateOn(float(-M_PI * count) / 12, dx, dy);
        sf::Vector2f SpawnPoint(player.getCenter() + sf::Vector2f{dx * player.Width, dy * player.Height} * 0.7f / velocity);
        newBullet = new Bullet(SpawnPoint, {dx, dy}, sf::Color(rand() % 256, rand() % 256, rand() % 256),
                               1, damage);
        Bullets.push_back(*newBullet);
        ammunition -= 1;
        count++;
        lastShoot = GlobalClock.getElapsedTime();
    }
};

// Chaotic
class Chaotic : public Weapon {
public:
    Chaotic() : Weapon(300, 0.1, 1.f / 16, 3) { velocity = 3; NameText.setText("Chaotic"); }
    void Update(vB& Bullets, Rect& player) {
        if (!lock && ammunition.toBottom() != 0 && GlobalClock.getElapsedTime() - lastShoot > FireRate)
            Shoot(Bullets, player);
        else if (ammunition.toBottom() == 0)
            lock = true;
        AmmoBar.Update();
    }
    void Shoot(vB& Bullets, Rect& player) {
        float dx = 0, dy = velocity;
        float len = hypotf(dx, dy);
        RotateOn(float(rand()), dx, dy);
        sf::Vector2f SpawnPoint(player.getCenter() + sf::Vector2f{dx * player.Width, dy * player.Height} * 0.7f / velocity);
        newBullet = new Bullet(SpawnPoint, {dx, dy}, sf::Color(rand() % 256, rand() % 256, rand() % 256),
                               1, damage);
        Bullets.push_back(*newBullet);
        ammunition -= 1;
        lastShoot = GlobalClock.getElapsedTime();
    }
};
