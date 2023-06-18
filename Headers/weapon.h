#include "bullet.h"
#include "Bar.h"

////////////////////////////////////////////////////////////
// Weapon
class Weapon {
public:
    Scale<int> ammunition;
    float cost;
    sf::Time FireRate, lastShoot;
    float damage;
    float velocity;
    int count;
    float scatter; // degree
    bool lock;
    int mode;
    
    sf::RectangleShape AmmoRect[3];
    Bar<int> AmmoBar;
    PlaccedText NameText, AmmoText;

    Weapon() {};
    Weapon(int MaxAmmo, float cost, float time, int dmg) {
        this->ammunition = {0, MaxAmmo, MaxAmmo};
        this->cost = cost;
        this->FireRate = sf::seconds(time);
        this->damage = dmg;
        lastShoot = sf::seconds(0);
        lock = true;
        mode = 1;

        AmmoRect[0].setSize({160, 40});
        AmmoRect[1].setSize({150, 30});
        AmmoRect[0].setPosition(scw - AmmoRect[0].getSize().x - 10, 120);
        AmmoRect[1].setPosition(AmmoRect[0].getPosition().x + 5, AmmoRect[0].getPosition().y + 5);
        AmmoRect[0].setFillColor(sf::Color(255, 255, 255, 160));
        AmmoRect[1].setFillColor(sf::Color(128, 128, 128, 160));

        AmmoRect[2] = AmmoRect[1];
        AmmoRect[2].setFillColor(sf::Color(32, 32, 32, 160));

        AmmoBar.setSize(160, 40);
        AmmoBar.setPosition(scw - AmmoBar.getSize().x - 10, 120);
        AmmoBar.scale = &ammunition;
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

    virtual void Update(vB&, Rect&, sf::Clock*, sf::Vector2f*) {};

    virtual void Shoot(vB& Bullets, Rect& player, sf::Clock* clock, sf::Vector2f* camera, sf::Vector2f& direction) {
        float dx = direction.x - player.PosX - player.Width  / 2 + camera->x,
              dy = direction.y - player.PosY - player.Height / 2 + camera->y;
        float len = hypotf(dx, dy);
        if (len == 0) return;
        RotateOn(-M_PI * (rand() % (int)scatter - scatter / 2) / 180, dx, dy);
        dx *= velocity / len; dy *= velocity / len;
        Bullet* newBullet = new Bullet(player.PosX + player.Width / 2, player.PosY + player.Height / 2, dx, dy,
                                       sf::Color(rand() % 256, rand() % 256, rand() % 256), 1, damage, 7, 0);
        Bullets.push_back(*newBullet);
        delete newBullet;
        ammunition -= 1;
        lastShoot = clock->getElapsedTime();
    }

    virtual void Reload(Scale<float>& Mana) {
        int x = std::min(int(Mana.cur / cost), ammunition.fromTop());
        Mana -= cost * x;
        ammunition += x;
    };

    virtual void interface(sf::RenderWindow& window) {
        AmmoText.setText(std::to_string((int)ammunition.cur));
        AmmoText.setPosition(AmmoBar.getPosition().x + AmmoBar.getSize().x / 2 - AmmoText.Width  / 2,
                             AmmoBar.getPosition().y + AmmoBar.getSize().y / 2 - AmmoText.Height / 2);

        AmmoBar.draw(window);
        AmmoText.draw(window);
        NameText.draw(window);
    }
};
////////////////////////////////////////////////////////////

// Pistol
class Pistol : public Weapon {
public:
    Pistol() : Weapon(9, 1, 0.35, 2) { velocity = 10; count = 1;  scatter = 20; NameText.setText("Pistol"); }
    void Update(vB& Bullets, Rect& player, sf::Clock* clock, sf::Vector2f* camera) {
        if (!lock && ammunition.toBottom() != 0 && clock->getElapsedTime() - lastShoot > FireRate) {
            sf::Vector2f dir = sf::Vector2f(sf::Mouse::getPosition());
            Shoot(Bullets, player, clock, camera, dir);
        } else if (ammunition.toBottom() == 0) {
            lock = true;
        }
    }
};

// Revolver
class Revolver : public Weapon {
public:
    Revolver() : Weapon(6, 2, 0, 5) { velocity = 16; count = 1;  scatter = 10; NameText.setText("Revolver"); }
    void Update(vB& Bullets, Rect& player, sf::Clock* clock, sf::Vector2f* camera) {
        if (!lock && ammunition.toBottom() != 0) {
            sf::Vector2f dir = sf::Vector2f(sf::Mouse::getPosition());
            Shoot(Bullets, player, clock, camera, dir);
            lock = true;
        } else if (ammunition.toBottom() == 0) {
            lock = true;
        }
    }
};

// Shotgun
class Shotgun : public Weapon {
public:
    Shotgun() : Weapon(5, 5, 1, 3) { velocity = 10; count = 10;  scatter = 50; NameText.setText("Shotgun"); }
    void Update(vB& Bullets, Rect& player, sf::Clock* clock, sf::Vector2f* camera) {
        if (!lock && ammunition.toBottom() != 0 && clock->getElapsedTime() - lastShoot > FireRate) {
            Shoot(Bullets, player, clock, camera);
            lock = true;
        } else if (ammunition.toBottom() == 0) {
            lock = true;
        }
    }
    void Shoot(vB& Bullets, Rect& player, sf::Clock* clock, sf::Vector2f* camera) {
        float dx = sf::Mouse::getPosition().x - player.PosX - player.Width  / 2 + camera->x,
              dy = sf::Mouse::getPosition().y - player.PosY - player.Height / 2 + camera->y;
        float len = hypotf(dx, dy);
        if (len == 0) return;
        dx *= velocity / len; dy *= velocity / len;
        RotateOn(-M_PI * scatter / float(180 * 2), dx, dy);
        for (int i = 0; i < count; i++, RotateOn(M_PI * scatter / float(180 * count), dx, dy)) {
            Bullet* newBullet = new Bullet(player.PosX + player.Width / 2, player.PosY + player.Height / 2, dx, dy,
                                           sf::Color(rand() % 256, rand() % 256, rand() % 256), 1, damage, 7, 0);
            Bullets.push_back(*newBullet);
            delete newBullet;
        }
        ammunition -= 1;
        lastShoot = clock->getElapsedTime();
    }
};

// Rifle
class Rifle : public Weapon {
public:
    Rifle() : Weapon(25, 1, 0.05, 2) { velocity = 16; count = 10;  scatter = 17; NameText.setText("Rifle"); }
    void Update(vB& Bullets, Rect& player, sf::Clock* clock, sf::Vector2f* camera) {
        if (!lock && ammunition.toBottom() != 0 && clock->getElapsedTime() - lastShoot > FireRate) {
            sf::Vector2f dir = sf::Vector2f(sf::Mouse::getPosition());
            Shoot(Bullets, player, clock, camera, dir);
        } else if (ammunition.toBottom() == 0) {
            lock = true;
        }
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
    void Update(vB& Bullets, Rect& player, sf::Clock* clock, sf::Vector2f* camera) {
        if (!lock && ammunition.toBottom() != 0 && clock->getElapsedTime() - lastShoot > FireRate) {
            if (position.x == 0 && position.y == 0)
                position = sf::Vector2f(sf::Mouse::getPosition());
            Shoot(Bullets, player, clock, camera, position);
            if ((--count) == 0) {
                count = 10;
                lock = true;
            }
        } else if (ammunition.toBottom() == 0) {
            lock = true;
        }
    }
    void Shoot(vB& Bullets, Rect& player, sf::Clock* clock, sf::Vector2f* camera, sf::Vector2f& direction) {
        float dx = direction.x - player.PosX - player.Width  / 2 + camera->x,
              dy = direction.y - player.PosY - player.Height / 2 + camera->y;
        float len = hypotf(dx, dy);
        if (len == 0) return;
        RotateOn(-M_PI * (rand() % (int)scatter - scatter / 2) / 180, dx, dy);
        dx *= velocity / len; dy *= velocity / len;
        Bullet* newBullet = new Bullet(player.PosX + player.Width / 2, player.PosY + player.Height / 2, dx, dy,
sf::Color(rand() % 256, rand() % 256, rand() % 256), 1, damage, 7, 3 + clock->getElapsedTime().asSeconds(), BulletType::Bubble);
        Bullets.push_back(*newBullet);
        delete newBullet;
        ammunition -= 1;
        lastShoot = clock->getElapsedTime();
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
    void Update(vB& Bullets, Rect& player, sf::Clock* clock, sf::Vector2f* camera) {
        if (!lock && ammunition.toBottom() != 0 && clock->getElapsedTime() - lastShoot > FireRate)
            Shoot(Bullets, player, clock, camera);
        else if (ammunition.toBottom() == 0)
            lock = true;
    }
    void Shoot(vB& Bullets, Rect& player, sf::Clock* clock, sf::Vector2f* camera) {
        float dx = 0, dy = velocity;
        RotateOn(float(-M_PI * count) / 12, dx, dy);
        Bullet* newBullet = new Bullet(player.PosX + player.Width / 2, player.PosY + player.Height / 2, dx, dy,
                                       sf::Color(rand() % 256, rand() % 256, rand() % 256), 1, damage, 7, 0);
        Bullets.push_back(*newBullet);
        delete newBullet;
        ammunition -= 1;
        count++;
        lastShoot = clock->getElapsedTime();
    }
};

// Chaotic
class Chaotic : public Weapon {
public:
    Chaotic() : Weapon(300, 0.1, 1.f / 16, 3) { velocity = 3; NameText.setText("Chaotic"); }
    void Update(vB& Bullets, Rect& player, sf::Clock* clock, sf::Vector2f* camera) {
        if (!lock && ammunition.toBottom() != 0 && clock->getElapsedTime() - lastShoot > FireRate)
            Shoot(Bullets, player, clock, camera);
        else if (ammunition.toBottom() == 0)
            lock = true;
    }
    void Shoot(vB& Bullets, Rect& player, sf::Clock* clock, sf::Vector2f* camera) {
        float dx = 0, dy = velocity;
        RotateOn(float(rand()), dx, dy);
        Bullet* newBullet = new Bullet(player.PosX + player.Width / 2, player.PosY + player.Height / 2, dx, dy,
                                       sf::Color(rand() % 256, rand() % 256, rand() % 256), 1, damage, 7, 0);
        Bullets.push_back(*newBullet);
        delete newBullet;
        ammunition -= 1;
        lastShoot = clock->getElapsedTime();
    }
};
