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

Player::Player() : Creature("Player", Fraction::Player) {
    Health = {0, MAX_HEALTH, MAX_HEALTH}; HealthRecovery = 10;
    Mana = {0, MAX_MANA, MAX_MANA}; ManaRecovery = 10.5;
    Armor = {0, 0, 0};
    Money = 0;
    Radius = 60;
    Velocity = {0, 0}; MaxVelocity = 6;
    Acceleration = 0.6;

    SetAnimation("sources/textures/Player.png", 1, {485, 485}, sf::seconds(1));
}

void Player::move(Location& location) {
    std::vector<float> PressedKeys = {
        sf::Keyboard::isKeyPressed(sf::Keyboard::W) ? 1.f : 0.f,
        sf::Keyboard::isKeyPressed(sf::Keyboard::A) ? 1.f : 0.f,
        sf::Keyboard::isKeyPressed(sf::Keyboard::S) ? 1.f : 0.f,
        sf::Keyboard::isKeyPressed(sf::Keyboard::D) ? 1.f : 0.f
    };

    VelocityBuff = 1;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::LShift))
        VelocityBuff *= 2;
    
    setTarget(getPosition() + sf::Vector2f{PressedKeys[3] - PressedKeys[1], PressedKeys[2] - PressedKeys[0]} * MaxVelocity * VelocityBuff);
    Creature::move(location);
}

void Player::UpdateState() {
    Mana += ManaRecovery * (localClock->getElapsedTime() - LastCheck).asSeconds();
    Health += HealthRecovery * (localClock->getElapsedTime() - LastCheck).asSeconds();
    LastCheck = localClock->getElapsedTime();
}

sf::Packet& operator<<(sf::Packet& packet, Player& a) { return packet << a.PosX << a.PosY; }
sf::Packet& operator>>(sf::Packet& packet, Player& a) { return packet >> a.PosX >> a.PosY; }
