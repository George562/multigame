#pragma once
#include "weapon.h"
#include "text.h"


////////////////////////////////////////////////////////////
// Class
////////////////////////////////////////////////////////////

class Creature : public Circle, public sf::Drawable {
public:
    Fraction::Fraction fraction;
    Scale<float> Health;
    Scale<float> Mana;
    float ManaRecovery, HealthRecovery;
    Scale<float> Armor; // blocking damage = min(1, damage - armor)
    float Money;
    sf::Vector2f Velocity; float MaxVelocity, VelocityBuff;
    float Acceleration;
    sf::Vector2f target;
    Weapon *FirstWeapon, *SecondWeapon, *CurWeapon;
    sf::Time LastCheck;
    sf::Clock* localClock;
    sf::Texture texture;
    mutable sf::Sprite sprite;
    mutable PlacedText Name;
    Animation *animationSprite = nullptr;

    Creature(str name, Fraction::Fraction f) : Circle() {
        Name.setString(name);
        Name.setCharacterSize(25);
        Name.setFillColor(sf::Color::Green);
        Name.setOutlineThickness(2);
        Name.setOutlineColor(sf::Color::Red);
        Name.setFillColor(sf::Color::White);
        Name.ShowText = false;

        fraction = f;

        LastCheck = sf::seconds(0);
        localClock = new sf::Clock();
    };
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states = sf::RenderStates::Default) const {
        if (animationSprite != nullptr) {
            animationSprite->setPosition(getPosition() - animationSprite->getsize() / 2.f);
            target.draw(*animationSprite, states);
        } else {
            sprite.setPosition(PosX, PosY);
            target.draw(sprite, states);
        }
        Name.setPosition(PosX - Name.Width / 2.f, PosY - Radius - Name.Height);
        target.draw(Name, states);
    };
    virtual void getDamage(float dmg) { Health -= dmg; }
    virtual void move(Location& location) {
        sf::Vector2f dist = target - getPosition();
        sf::Vector2f VelocityTarget = {std::max(std::min(MaxVelocity * VelocityBuff, dist.x), -MaxVelocity * VelocityBuff),
                                       std::max(std::min(MaxVelocity * VelocityBuff, dist.y), -MaxVelocity * VelocityBuff)};
        sf::Vector2f Vdist = VelocityTarget - Velocity;
        sf::Vector2f Direction = {std::max(std::min(Acceleration, Vdist.x), -Acceleration),
                                  std::max(std::min(Acceleration, Vdist.y), -Acceleration)};
        Velocity += Direction;

        sf::Vector2i tempv = WillCollisionWithWalls(location.wallsRect, *this, Velocity);

        if (tempv.x == -1) Velocity.x = 0;
        if (tempv.y == -1) Velocity.y = 0;

        PosX += Velocity.x;
        PosY += Velocity.y;
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

    virtual void setTarget(sf::Vector2f target) { this->target = target; }
};

////////////////////////////////////////////////////////////
// Realization
////////////////////////////////////////////////////////////
