#pragma once
#include "weapon.h"
#include "text.h"

////////////////////////////////////////////////////////////
// Class
////////////////////////////////////////////////////////////

class Creature : public Circle, public sf::Drawable {
public:
    Scale<float> Health;
    Scale<float> Mana;
    float ManaRecovery, HealthRecovery;
    Scale<float> Armor; // blocking damage = min(1, damage - armor)
    float Money;
    sf::Vector2f Velocity; float MaxVelocity, VelocityBuf;
    float Acceleration;
    sf::Vector2f target;
    sf::Texture texture;
    sf::Sprite sprite;
    Weapon *FirstWeapon, *SecondWeapon, *CurWeapon;
    sf::Time LastCheck;
    sf::Clock* localClock;
    PlacedText Name;

    Creature(str name) : Circle() {
        Name.setString(name);
        Name.setCharacterSize(25);
        Name.setFillColor(sf::Color::Green);
        Name.setOutlineThickness(2);
        Name.setOutlineColor(sf::Color::Red);
        Name.setFillColor(sf::Color::White);
        Name.ShowText = false;

        LastCheck = sf::seconds(0);
        localClock = new sf::Clock();
    };
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states = sf::RenderStates::Default) const {
        target.draw(sprite, states);
        target.draw(Name, states);
    };
    virtual void getDamage(float dmg) { Health -= dmg; }
    virtual void move(Location& location) {
        sf::Vector2f dist = target - getPosition();
        sf::Vector2f VelocityTarget = {std::max(std::min(MaxVelocity * VelocityBuf, dist.x), -MaxVelocity * VelocityBuf),
                                       std::max(std::min(MaxVelocity * VelocityBuf, dist.y), -MaxVelocity * VelocityBuf)};
        sf::Vector2f Vdist = (VelocityTarget - Velocity);
        sf::Vector2f Direction = {std::max(std::min(Acceleration, Vdist.x), -Acceleration),
                                  std::max(std::min(Acceleration, Vdist.y), -Acceleration)};
        Velocity += Direction;

        sf::Vector2i tempv = WillCollisionWithWalls(location.wallsRect, *this, Velocity);

        if (tempv.x == -1) Velocity.x = 0;
        if (tempv.y == -1) Velocity.y = 0;

        setPosition(getPosition() + Velocity);
    };
    virtual void UpdateState() {};
    void SetTexture(str name) {
        if (!texture.loadFromFile(name + ".png"))
            std::cout << "Can't load texture" << name << ".png\n";
        texture.setSmooth(true);
        sprite.setTexture(texture);
        if (Radius == 0)
            setRadius(sprite.getGlobalBounds().width / 2.f);
        else
            sprite.setScale(Radius * 2.f / sprite.getGlobalBounds().width, Radius * 2.f / sprite.getGlobalBounds().height);
        sprite.setOrigin(sprite.getLocalBounds().width / 2.f, sprite.getLocalBounds().height / 2.f);
    };
    void ChangeWeapon(Weapon* to) { CurWeapon = to; }

    void setPosition(float x, float y) {
        PosX = x; PosY = y;
        sprite.setPosition(PosX, PosY);
        Name.setPosition(sf::Vector2f{PosX + (Radius - Name.Width) / 2.f, PosY - Name.Height});
    }
    void setPosition(sf::Vector2f v) { setPosition(v.x, v.y); }
    void setCenter(float x, float y) { setPosition(x - Radius / 2, y - Radius / 2); }
    void setCenter(sf::Vector2f v) { setCenter(v.x, v.y); }
    void move(float x, float y) { setPosition(PosX + x, PosY + y); }
    void move(sf::Vector2f v) { setPosition(PosX + v.x, PosY + v.y); }

    void setTarget(sf::Vector2f target) { this->target = target; }
};

////////////////////////////////////////////////////////////
// Realization
////////////////////////////////////////////////////////////
