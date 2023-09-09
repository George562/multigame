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
            animationSprite->setPosition(getPosition());
            target.draw(*animationSprite, states);
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
    void SetAnimation(std::string TexturFileName, int FrameAmount, sf::Vector2f frameSize, sf::Time duration) {
        if (animationSprite != nullptr) {
            delete animationSprite;
        }
        animationSprite = new Animation(TexturFileName, FrameAmount, frameSize, duration);
        animationSprite->setSize({Radius * 2.f, Radius * 2.f});
        animationSprite->setOrigin(animationSprite->getLocalSize() / 2.f);
        animationSprite->play();
    };
    void ChangeWeapon(Weapon* to) { CurWeapon = to; }

    virtual void setTarget(sf::Vector2f target) { this->target = target; }
};

////////////////////////////////////////////////////////////
// Realization
////////////////////////////////////////////////////////////
