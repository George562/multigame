#pragma once

#include "interactible.h"
#include "location.h"
#include "tools.h"
#include "enemy.h"

class WaterParticle : public Interactable {
public:
    sf::Clock* localClock;
    sf::Vector2f Velocity; float MaxVelocity, VelocityBuff;
    float Acceleration;
    sf::Vector2f target;
    sf::Time LastMoveCheck;

    WaterParticle();
    void setTarget(sf::Vector2f target) { this->target = target; }
    void move(Location* location);
};

WaterParticle::WaterParticle() : Interactable() {
    LastMoveCheck = sf::seconds(0);
    localClock = new sf::Clock();

    Velocity = {0.f, 0.f}; MaxVelocity = 6.f;
    Acceleration = 0.6f;
    VelocityBuff = 1;
}

// нужна тонкая настройка полёта частиц воды
void WaterParticle::move(Location* location) {
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
