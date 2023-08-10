#include "creature.h"

#define MAX_MANA 75
#define MAX_HEALTH 10

////////////////////////////////////////////////////////////
// Class
////////////////////////////////////////////////////////////

class Player : public Creature {
public:
    Player();
    void move(Location&) override;
    void UpdateState() override;
};

////////////////////////////////////////////////////////////
// Realization
////////////////////////////////////////////////////////////

Player::Player() : Creature("Player") {
    Health = {0, MAX_HEALTH, MAX_HEALTH}; HealthRecovery = 1;
    Mana = {0, MAX_MANA, MAX_MANA}; ManaRecovery = 10.5;
    Armor = {0, 0, 0};
    Money = 0;
    radius = 60;
    Width = Height = radius * 2;
    Velocity = {0, 0}; MaxVelocity = 6;
    Acceleration = 0.6;

    SetTexture("sources/textures/Player");
}

void Player::move(Location& location) {
    std::map<char, bool> PressedKeys = {
        {'w', sf::Keyboard::isKeyPressed(sf::Keyboard::W)},
        {'a', sf::Keyboard::isKeyPressed(sf::Keyboard::A)},
        {'s', sf::Keyboard::isKeyPressed(sf::Keyboard::S)},
        {'d', sf::Keyboard::isKeyPressed(sf::Keyboard::D)}
    };

    VelocityBuf = 1;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::LShift))
        VelocityBuf *= 2;
    
    setTarget(getCenter() + sf::Vector2f{(PressedKeys['a'] || PressedKeys['d']) ? (PressedKeys['d'] ? 1.f : -1.f) : 0.f,
                                         (PressedKeys['w'] || PressedKeys['s']) ? (PressedKeys['s'] ? 1.f : -1.f) : 0.f} * MaxVelocity * VelocityBuf);
    Creature::move(location);
}

void Player::UpdateState() {
    Mana += ManaRecovery * (localClock->getElapsedTime() - LastCheck).asSeconds();
    Health += HealthRecovery * (localClock->getElapsedTime() - LastCheck).asSeconds();
    LastCheck = localClock->getElapsedTime();
}

sf::Packet& operator<<(sf::Packet& packet, Player& a) { return packet << a.PosX << a.PosY; }
sf::Packet& operator>>(sf::Packet& packet, Player& a) { return packet >> a.PosX >> a.PosY; }
