#pragma once
#include "weapon.h"
#include "inventory.h"

////////////////////////////////////////////////////////////
// Class
////////////////////////////////////////////////////////////

#pragma pack(push, 1)
class Creature : public Circle, public sf::Drawable {
public:
    faction::Type faction;
    Scale<float> Health;
    Scale<float> Mana;
    float ManaRecovery, HealthRecovery;
    Scale<float> Armor; // blocking damage = min(1, damage - armor)
    float Money;
    sf::Vector2f Velocity; float MaxVelocity, VelocityBuff;
    float Acceleration;
    sf::Vector2f target;
    Weapon *FirstWeapon, *SecondWeapon, *CurWeapon;
    sf::Time LastStateCheck, LastMoveCheck;
    sf::Clock* localClock;
    mutable PlacedText Name;
    Animation *animation;

    bool dropInventory;
    Inventory inventory;

    Creature(sf::String name, faction::Type f) : Circle() {
        Name.setString(name);
        Name.setCharacterSize(25);
        Name.setOutlineThickness(2);
        Name.setOutlineColor(sf::Color::Red);
        Name.setFillColor(sf::Color::White);
        Name.ShowText = false;

        faction = f;

        LastStateCheck = sf::seconds(0);
        LastMoveCheck = sf::seconds(0);
        localClock = new sf::Clock();
        animation = nullptr;
        dropInventory = true;
    };

    virtual void draw(sf::RenderTarget& target, sf::RenderStates states = sf::RenderStates::Default) const {
        if (animation != nullptr) {
            animation->setPosition(getPosition());
            target.draw(*animation, states);
        }
        Name.setPosition(PosX - Name.Width / 2.f, PosY - Radius - Name.Height);
        target.draw(Name, states);
    };

    virtual void getDamage(float dmg) { Health -= dmg; }

    virtual void move(Location* location) {
        float ElapsedTimeAsSecond = std::min((localClock->getElapsedTime() - LastMoveCheck).asSeconds(), 0.0167f) * 60.f;
        sf::Vector2f dist = target - getPosition();
        sf::Vector2f VelocityTarget = {std::clamp(dist.x, -MaxVelocity * VelocityBuff, MaxVelocity * VelocityBuff),
                                       std::clamp(dist.y, -MaxVelocity * VelocityBuff, MaxVelocity * VelocityBuff)};
        sf::Vector2f Vdist = VelocityTarget - Velocity;
        sf::Vector2f Direction = {std::clamp(Vdist.x, -Acceleration, Acceleration),
                                  std::clamp(Vdist.y, -Acceleration, Acceleration)};
        Velocity += Direction * ElapsedTimeAsSecond;

        sf::Vector2i tempv = WillCollisionWithWalls(location->wallsRect, *this, Velocity * ElapsedTimeAsSecond);

        if (tempv.x == -1) Velocity.x = 0;
        if (tempv.y == -1) Velocity.y = 0;

        PosX += Velocity.x * ElapsedTimeAsSecond;
        PosY += Velocity.y * ElapsedTimeAsSecond;
        LastMoveCheck = localClock->getElapsedTime();
    };

    virtual void UpdateState() {
        Mana += ManaRecovery * (localClock->getElapsedTime() - LastStateCheck).asSeconds();
        Health += HealthRecovery * (localClock->getElapsedTime() - LastStateCheck).asSeconds();
        LastStateCheck = localClock->getElapsedTime();
    }

    void setAnimation(sf::Texture& texture, int FrameAmount, int maxLevel, sf::Time duration, sf::Shader *shader = nullptr) {
        if (animation != nullptr) {
            delete animation;
        }
        animation = new Animation(texture, FrameAmount, maxLevel, duration, shader);
        animation->setSize({Radius * 2.f, Radius * 2.f});
        animation->setOrigin(animation->getLocalSize() / 2.f);
        animation->play();
    };

    void ChangeWeapon(Weapon* to) { CurWeapon = to; }

    virtual void setTarget(sf::Vector2f target) { this->target = target; }

    void AddItem(Item* item) {
        inventory.addToDropable(item);
        inventory.addToEquip(item);
        inventory.addToKey(item);
        inventory.addToSafe(item);
    }
};
#pragma pack(pop)

////////////////////////////////////////////////////////////
// Realization
////////////////////////////////////////////////////////////
