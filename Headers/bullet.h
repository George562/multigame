#pragma once
#include "location.h"
#include "tools.h"
#include "animation.h"
#include "fire.h"

#define COMMON_BULLET_RADIUS 7
#define COMMON_BULLET_PENETRATION 0

sf::CircleShape circleShape;

////////////////////////////////////////////////////////////
// Class
////////////////////////////////////////////////////////////

#pragma pack(push, 1)
struct Bullet : public Circle, public sf::Drawable {
    enum Type : sf::Uint8 {
        Common,
        Bubble,
        WaterParticle,
        FireParticle
    };
    Bullet::Type type;

    bool explode = false;
    bool todel = false;
    float damage;
    int penetration;
    sf::Vector2f Velocity;
    Scale<float> ExplosionRadius = {1, 12, 1};
    faction::Type fromWho;
    sf::Clock* localClock = nullptr;
    sf::Time timer;
    sf::Color color;

    Bullet() {
        localClock = new sf::Clock();
    }

    Bullet(faction::Type faction, sf::Vector2f pos, sf::Vector2f velocity, float dmg, int penetr = COMMON_BULLET_PENETRATION,
           Bullet::Type type = Bullet::Common, sf::Time time = sf::Time::Zero) : Bullet() {
        this->type = type;
        PosX = pos.x; PosY = pos.y;
        damage = dmg;
        penetration = penetr;
        Velocity = velocity;
        fromWho = faction;
        timer = time;
        switch (type) {
            case Bullet::Common:
            case Bullet::Bubble:
                Radius = COMMON_BULLET_RADIUS;
                color = sf::Color(rand() % 256, rand() % 256, rand() % 256);
                break;
            case Bullet::WaterParticle:
                Radius = COMMON_BULLET_RADIUS * 3;
                color = sf::Color(0, 0, 255);
                break;
            case Bullet::FireParticle:
                Radius = COMMON_BULLET_RADIUS * 3;
                color = sf::Color(255, 0, 0);
                break;
            default:
                Radius = COMMON_BULLET_RADIUS;
                break;
        }
    }

    ~Bullet() {
        if (localClock) {
            delete localClock;
        }
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
                break;
            case Bullet::WaterParticle:
                Circle::move(Velocity);
                break;
            case Bullet::FireParticle:
                Circle::move(Velocity);
                break;
        }
    }
    void draw(sf::RenderTarget& target, sf::RenderStates states = sf::RenderStates::Default) const {
        circleShape.setFillColor(color);
        circleShape.setRadius(Radius);
        circleShape.setPosition(getPosition());
        circleShape.setOrigin(Radius, Radius);
        target.draw(circleShape);
    }
};
#pragma pack(pop)

using vB = std::vector<Bullet*>;
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
