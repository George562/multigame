#pragma once
#include "creature.h"

#define MAX_MANA 75.f
#define MAX_HEALTH 1000.f

////////////////////////////////////////////////////////////
// Class
////////////////////////////////////////////////////////////

class Player : public Creature {
public:
    Player();
    void move(Location*) override;
    void UpdateState() override;

    bool in_the_hands = false;
    Item* hand_item = nullptr;
    void GiveItem(Item* item);
    Item* TakeItem();
};

////////////////////////////////////////////////////////////
// Realization
////////////////////////////////////////////////////////////

Player::Player() : Creature("Player", faction::Player) {
    Health = {0.f, MAX_HEALTH, MAX_HEALTH}; HealthRecovery = 7.f;
    Mana = {0.f, MAX_MANA, MAX_MANA}; ManaRecovery = 10.5f;
    Armor = {0.f, 0.f, 0.f};
    Money = 0.f;
    Radius = 60.f;
    Velocity = {0.f, 0.f}; MaxVelocity = 10.f;
    Acceleration = 0.6f;
}

void Player::move(Location* location) {
    std::vector<float> PressedKeys = {
        sf::Keyboard::isKeyPressed(sf::Keyboard::W) ? 1.f : 0.f,
        sf::Keyboard::isKeyPressed(sf::Keyboard::A) ? 1.f : 0.f,
        sf::Keyboard::isKeyPressed(sf::Keyboard::S) ? 1.f : 0.f,
        sf::Keyboard::isKeyPressed(sf::Keyboard::D) ? 1.f : 0.f
    };

    VelocityBuff = 1;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::LShift))
        VelocityBuff *= 2;
    
    setTarget(getPosition() + sf::Vector2f(PressedKeys[3] - PressedKeys[1], PressedKeys[2] - PressedKeys[0]) * MaxVelocity * VelocityBuff);
    Creature::move(location);
}

void Player::UpdateState() {
    Mana += ManaRecovery * (localClock->getElapsedTime() - LastStateCheck).asSeconds();
    Health += HealthRecovery * (localClock->getElapsedTime() - LastStateCheck).asSeconds();
    LastStateCheck = localClock->getElapsedTime();
}

void Player::GiveItem(Item* item) {
    if (!in_the_hands) {
        hand_item = item;
        in_the_hands = true;
    }
}

Item* Player::TakeItem() {
    if (in_the_hands) {
        in_the_hands = false;
        return hand_item;
    }
    return nullptr;
}

sf::Packet& operator<<(sf::Packet& packet, Player& a) { return packet << a.PosX << a.PosY; }
sf::Packet& operator>>(sf::Packet& packet, Player& a) { return packet >> a.PosX >> a.PosY; }
