#pragma once
#include "creature.h"

#define MAX_MANA 75.f
#define MAX_HEALTH 100.f

////////////////////////////////////////////////////////////
// Class
////////////////////////////////////////////////////////////

class Player : public Creature {
public:
    Player();
    void move(Location*) override;
};

////////////////////////////////////////////////////////////
// Realization
////////////////////////////////////////////////////////////

Player::Player() : Creature("Player", faction::Player) {
    Health = {0.f, MAX_HEALTH, MAX_HEALTH}; HealthRecovery = 7.f;
    Mana = {0.f, MAX_MANA, MAX_MANA}; ManaRecovery = 10.5f;
    Armor = {0.f, 0.f, 0.f};
    setRadius(60.f);
    Velocity = {0.f, 0.f}; MaxVelocity = 900.f;
    Acceleration = 10000.f;
    Name.setOutlineColor(sf::Color::Green);
    Name.setFillColor(sf::Color::Black);
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

    setTarget(getCenter() + sf::Vector2f(PressedKeys[3] - PressedKeys[1], PressedKeys[2] - PressedKeys[0]) * MaxVelocity * VelocityBuff);
    Creature::move(location);
}

sf::Packet& operator<<(sf::Packet& packet, Player& a) {
    return packet << a.getCenter().x << a.getCenter().y;
}
sf::Packet& operator>>(sf::Packet& packet, Player& a) {
    sf::Vector2f v;
    packet >> v.x >> v.y;
    a.setCenter(v);
    return packet;
}