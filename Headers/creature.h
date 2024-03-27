#pragma once
#include "weapon.h"
#include "inventory.h"
#include "text.h"

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
    sf::Vector2f Velocity; float MaxVelocity, VelocityBuff;
    float Acceleration;
    sf::Vector2f target;
    Weapon *FirstWeapon = nullptr, *SecondWeapon = nullptr, *CurWeapon = nullptr;
    sf::Time LastStateCheck, LastMoveCheck;
    sf::Clock* localClock = nullptr;
    mutable PlacedText Name;
    Animation *animation = nullptr;

    bool dropInventory;
    Inventory inventory;

    std::vector<int> effectStacks;

    Creature(sf::String name, faction::Type f) : Circle() {
        Name.setString(name);
        Name.setCharacterSize(25);
        Name.setOutlineThickness(2);
        Name.setOutlineColor(sf::Color::Red);
        Name.setFillColor(sf::Color::White);

        faction = f;

        LastStateCheck = sf::seconds(0);
        LastMoveCheck = sf::seconds(0);
        localClock = new sf::Clock();
        animation = nullptr;
        dropInventory = true;

        effectStacks.assign(5, 0);
    }
    ~Creature() {
        if (localClock) {
            delete localClock;
        }
        if (animation) {
            delete animation;
        }
    }

    virtual void draw(sf::RenderTarget& target, sf::RenderStates states = sf::RenderStates::Default) const {
        if (animation != nullptr) {
            animation->setPosition(getPosition());
            target.draw(*animation, states);
        }
        Name.setPosition(PosX - Name.Width / 2.f, PosY - Radius - Name.Height);
        target.draw(Name, states);
    }

    virtual void getDamage(float dmg) { Health -= dmg; }

    virtual void move(Location* location) {
        float ElapsedTimeAsSecond = std::min((localClock->getElapsedTime() - LastMoveCheck).asSeconds(), 1.f / 60.f);
        sf::Vector2f dist = target - getPosition();
        float len = std::sqrt(dist.x * dist.x + dist.y * dist.y);
        sf::Vector2f VelocityTarget(std::clamp(dist.x, -std::abs(dist.x) * MaxVelocity * VelocityBuff / len, std::abs(dist.x) * MaxVelocity * VelocityBuff / len),
                                    std::clamp(dist.y, -std::abs(dist.y) * MaxVelocity * VelocityBuff / len, std::abs(dist.y) * MaxVelocity * VelocityBuff / len));
        sf::Vector2f Vdist = VelocityTarget - Velocity;
        len = std::sqrt(Vdist.x * Vdist.x + Vdist.y * Vdist.y);
        sf::Vector2f Direction(std::clamp(Vdist.x, -std::abs(Vdist.x) * Acceleration / len, std::abs(Vdist.x) * Acceleration / len),
                               std::clamp(Vdist.y, -std::abs(Vdist.y) * Acceleration / len, std::abs(Vdist.y) * Acceleration / len));
        Velocity += Direction * ElapsedTimeAsSecond;
        // visualization https://www.desmos.com/calculator/oumleenz1s

        sf::Vector2i tempv = WillCollisionWithWalls(location->wallsRect, *this, Velocity * ElapsedTimeAsSecond);

        if (tempv.x == -1) Velocity.x = 0;
        if (tempv.y == -1) Velocity.y = 0;

        PosX += Velocity.x * ElapsedTimeAsSecond;
        PosY += Velocity.y * ElapsedTimeAsSecond;
        LastMoveCheck = localClock->getElapsedTime();
    }

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
    }

    void ChangeWeapon(Weapon* to) { CurWeapon = to; }

    virtual void setTarget(sf::Vector2f target) { this->target = target; }

    void AddItem(Item* item) {
        inventory.addItem(item);
    }
};
#pragma pack(pop)

////////////////////////////////////////////////////////////
// Realization
////////////////////////////////////////////////////////////
