#pragma once
#include "location.h"
#include "tools.h"

#define COMMON_BULLET_RADIUS 7
#define COMMON_BULLET_PENETRATION 0

////////////////////////////////////////////////////////////
// Class
////////////////////////////////////////////////////////////

#pragma pack(push, 1)
struct Bullet : public Circle {
    enum Type : sf::Uint8 {
        Common,
        Bubble
    };
    Bullet::Type type;

    bool explode = false;
    bool todel = false;
    float damage;
    int penetration;
    sf::Vector2f Velocity;
    Scale<float> ExplosionRadius = {1, 12, 1};
    fraction::fractionType fromWho;
    sf::Clock* localClock;
    sf::Time timer;
    sf::Color color;

    Bullet() {}
    Bullet(fraction::fractionType f, sf::Vector2f pos, sf::Vector2f v, float dmg, int penetr = COMMON_BULLET_PENETRATION, Bullet::Type t = Bullet::Common, sf::Time time = sf::Time::Zero) {
        fromWho = f;
        PosX = pos.x; PosY = pos.y;
        Velocity = v;
        Radius = COMMON_BULLET_RADIUS;
        penetration = penetr;
        damage = dmg;
        timer = time;
        type = t;
        switch (t) {
            case Bullet::Common:
                color = sf::Color(rand() % 256, rand() % 256, rand() % 256);
                break;
            case Bullet::Bubble:
                color = sf::Color(rand() % 256, rand() % 256, rand() % 256);
                break;
        }
        localClock = new sf::Clock();
    }

    void move(Location* location) {
        if (LenOfVector(Velocity) != 0) {
            sf::Vector2i res = WillCollisionWithWalls(location->wallsRect, *this, Velocity);
            if (res.x == -1 || res.y == -1) {
                penetration--;
            }
            Velocity = Velocity * (sf::Vector2f)res;
        }
        switch (type) {
            case Bullet::Bubble:
                PosX += Velocity.x * (timer - localClock->getElapsedTime()).asSeconds();
                PosY += Velocity.y * (timer - localClock->getElapsedTime()).asSeconds();
                if (!explode && timer < localClock->getElapsedTime()) {
                    Velocity = {0.f, 0.f};
                    explode = true;
                    localClock->restart();
                }
                if (explode && !todel) {
                    if (ExplosionRadius.fromTop() > 0) {
                        ExplosionRadius += localClock->restart().asSeconds() * 8.f;
                        color -= sf::Color(0, 0, 0, 4);
                        Radius = COMMON_BULLET_RADIUS * ExplosionRadius.cur;
                        Circle::move(-COMMON_BULLET_RADIUS / 5, -COMMON_BULLET_RADIUS / 5);
                    } else todel = true;
                }
                break;
            case Bullet::Common:
                Circle::move(Velocity);
        }
    }
};
#pragma pack(pop)

using vB = std::vector<Bullet>;
vB Bullets(0);

sf::Packet& operator<<(sf::Packet& packet, Bullet& b) {
    return packet << b.PosX << b.PosY << b.Velocity.x << b.Velocity.y << b.penetration << b.color << b.damage << b.timer.asSeconds() << b.type;
}
sf::Packet& operator>>(sf::Packet& packet, Bullet& b) {
    float timer;
    packet >> b.PosX >> b.PosY >> b.Velocity.x >> b.Velocity.y >> b.penetration >> b.color >> b.damage >> timer;
    b.timer = sf::seconds(timer);
    sf::Uint8 t; packet >> t; b.type = Bullet::Type(t);
    return packet;
}
