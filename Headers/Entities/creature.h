#pragma once
#include "../Systems/weapon.h"
#include "../Systems/inventory.h"
#include "../Systems/effect.h"
#include "../UI/text.h"

////////////////////////////////////////////////////////////
// Class
////////////////////////////////////////////////////////////

#pragma pack(push, 1)
class Creature : public sf::Drawable {
public:
    faction::Type faction;

    Scale<float> Health;
    bool HealthRecoveryActive = true;
    float HealthRecovery;
    Scale<float> Mana;
    bool ManaRecoveryActive = true;
    float ManaRecovery;
    Scale<float> Armor; // blocking damage = min(1, damage - armor)

    sf::Vector2f Velocity;
    float MaxVelocity;
    float VelocityBuff;
    float Acceleration;
    sf::Vector2f target; // target point to move towards
    bool makeADash = false;

    Weapon *CurWeapon = nullptr; // ref on exist weapon from Weapons

    sf::Time LastStateCheck;
    sf::Time LastMoveCheck;
    sf::Clock* localClock = nullptr;

    mutable PlacedText Name;
    Animation *animation = nullptr;

    bool dropInventory;
    Inventory inventory;

    std::vector<Effect*> effects;
    std::vector<int> effectStacks;

    CollisionCircle hitbox;

    Creature(sf::String name, faction::Type f) {
        Name.setString(name);
        Name.setCharacterSize(25);
        Name.setOutlineThickness(2);
        Name.setOutlineColor(sf::Color::Red);
        Name.setFillColor(sf::Color::White);

        faction = f;

        LastStateCheck = sf::Time::Zero;
        LastMoveCheck = sf::Time::Zero;
        localClock = new sf::Clock();
        animation = nullptr;
        dropInventory = true;

        effectStacks.assign(Effects::EffectCount, 0);
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
            animation->setPosition(hitbox.getCenter());
            target.draw(*animation, states);
        }
        Name.setPosition(hitbox.getCenter().x - Name.Width / 2.f, hitbox.getPosition().y - Name.Height);
        target.draw(Name, states);
    }

    virtual void getDamage(float dmg) { Health -= dmg; }

    virtual void move(Location* location) {
        float ElapsedTimeAsSecond = std::min((localClock->getElapsedTime() - LastMoveCheck).asSeconds(), oneOverSixty);
        if (!makeADash) {
            sf::Vector2f Difference = target - hitbox.getCenter() - Velocity;
            if (length(Difference) >= Acceleration * ElapsedTimeAsSecond) {
                sf::Vector2f Direction = sf::Vector2f(sign(Difference)) * Acceleration;
                Velocity += Direction * ElapsedTimeAsSecond;
            } else {
                Velocity += Difference;
            }
        } else {
            Velocity = normalize(target - hitbox.getCenter()) * MaxVelocity * VelocityBuff;
            makeADash = false;
        }

        sf::Vector2i tempv = WillCollisionWithWalls(location->wallsRect, hitbox, Velocity * ElapsedTimeAsSecond);

        if (tempv.x == -1) Velocity.x = 0;
        if (tempv.y == -1) Velocity.y = 0;

        hitbox.move(Velocity * ElapsedTimeAsSecond);
        LastMoveCheck = localClock->getElapsedTime();
    }

    virtual void shift(sf::Vector2f shift) {}
    virtual void shift(float x, float y) {}

    virtual void UpdateState() {
        Mana += ManaRecovery * ManaRecoveryActive * (localClock->getElapsedTime() - LastStateCheck).asSeconds();
        Health += HealthRecovery * HealthRecoveryActive * (localClock->getElapsedTime() - LastStateCheck).asSeconds();
        LastStateCheck = localClock->getElapsedTime();
    }

    void setAnimation(sf::Texture& texture, int FrameAmount, int maxLevel, sf::Time duration, sf::Shader *shader = nullptr) {
        if (animation != nullptr) {
            delete animation;
        }
        animation = new Animation("");
        animation->setAnimation(texture, FrameAmount, maxLevel, duration, shader);
        animation->setSize(hitbox.getSize());
        animation->setOrigin(animation->getLocalSize() / 2.f);
        animation->play();
    }

    void setAnimation(sf::Texture& texture, sf::Shader *shader = nullptr) {
        if (animation != nullptr) {
            delete animation;
        }
        animation = new Animation("");
        animation->setTexture(texture, shader);
        animation->setSize(hitbox.getSize());
        animation->setOrigin(animation->getLocalSize() / 2.f);
        animation->play();
    };

    void ChangeWeapon(Weapon* to) { CurWeapon = to; }

    virtual void setTarget(sf::Vector2f target) { this->target = target; }

    void addItem(Item* item) {
        inventory.addItem(item);
    }

    bool isAlive() {
        return Health.toBottom() > 0;
    }
};
#pragma pack(pop)

////////////////////////////////////////////////////////////
// Realization
////////////////////////////////////////////////////////////
