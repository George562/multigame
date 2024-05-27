#pragma once
#include "../LevelSystem/location.h"

#define COMMON_BULLET_RADIUS 10
#define COMMON_BULLET_PENETRATION 0

sf::CircleShape circleShape;

////////////////////////////////////////////////////////////
// Class
////////////////////////////////////////////////////////////

#pragma pack(push, 1)
struct Bullet : public CollisionCircle, public sf::Drawable {
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

    Bullet() : CollisionCircle() {
        localClock = new sf::Clock();
    }

    Bullet(faction::Type faction, sf::Vector2f pos, sf::Vector2f velocity, float dmg, int penetr = COMMON_BULLET_PENETRATION,
           Bullet::Type type = Bullet::Common, sf::Time time = sf::Time::Zero) : Bullet() {
        this->type = type;
        setCenter(pos);
        damage = dmg;
        penetration = penetr;
        Velocity = velocity;
        fromWho = faction;
        timer = time;
        switch (type) {
            case Bullet::Common:
            case Bullet::Bubble:
                setRadius(COMMON_BULLET_RADIUS);
                switch (faction) {
                    case faction::Player: color = sf::Color(30, 195, 255); break;
                    case faction::Enemy:  color = sf::Color(72,  61, 139); break;
                }
                break;
            case Bullet::WaterParticle:
                setRadius(COMMON_BULLET_RADIUS * 3);
                color = sf::Color(0, 0, 255);
                break;
            case Bullet::FireParticle:
                setRadius(COMMON_BULLET_RADIUS * 3);
                color = sf::Color(255, 0, 0);
                break;
            default:
                setRadius(COMMON_BULLET_RADIUS);
                break;
        }
    }

    ~Bullet() {
        if (localClock) {
            delete localClock;
        }
    }

    void move(Location* location) {
        float elapsedTime = std::min(localClock->restart().asSeconds(), oneOverSixty);
        if (LenOfVector(Velocity) != 0) {
            sf::Vector2i res;
            switch (type) {
                case Bullet::Bubble:
                    res = WillCollisionWithWalls(location->wallsRect, *this, Velocity * timer.asSeconds() * elapsedTime);
                    break;
                case Bullet::Common:
                case Bullet::WaterParticle:
                case Bullet::FireParticle:
                    res = WillCollisionWithWalls(location->wallsRect, *this, Velocity * elapsedTime);
                    break;
            }
            if (res.x == -1 || res.y == -1) {
                penetration--;
            }
            Velocity = Velocity * (sf::Vector2f)res;
        }
        switch (type) {
            case Bullet::Bubble:
                timer -= sf::seconds(elapsedTime);
                CollisionCircle::move(Velocity * timer.asSeconds() * elapsedTime);
                if (!explode && timer <= sf::Time::Zero) {
                    Velocity = {0.f, 0.f};
                    explode = true;
                }
                if (explode && !todel) {
                    if (ExplosionRadius.fromTop() > 0) {
                        ExplosionRadius += elapsedTime * 8.f;
                        color -= sf::Color(0, 0, 0, 4);
                        setRadius(COMMON_BULLET_RADIUS * ExplosionRadius.cur);
                        CollisionCircle::move(-COMMON_BULLET_RADIUS / 5, -COMMON_BULLET_RADIUS / 5);
                    } else todel = true;
                }
                break;
            case Bullet::Common:
            case Bullet::WaterParticle:
            case Bullet::FireParticle:
                CollisionCircle::move(Velocity * elapsedTime);
                break;
        }
    }
    void draw(sf::RenderTarget& target, sf::RenderStates states = sf::RenderStates::Default) const {
        circleShape.setFillColor(color);
        circleShape.setRadius(getRadius());
        circleShape.setPosition(getPosition());
        target.draw(circleShape);
    }
};
#pragma pack(pop)

using vB = std::vector<Bullet*>;
vB Bullets(0);

sf::Packet& operator<<(sf::Packet& packet, Bullet& b) {
    return packet << b.getCenter().x << b.getCenter().y << b.Velocity.x << b.Velocity.y << b.penetration << b.color << b.damage << b.timer.asSeconds() << b.type;
}
sf::Packet& operator>>(sf::Packet& packet, Bullet& b) {
    float timer;
    sf::Vector2f pos;
    packet >> pos.x >> pos.y >> b.Velocity.x >> b.Velocity.y >> b.penetration >> b.color >> b.damage >> timer;
    b.setCenter(pos);
    b.timer = sf::seconds(timer);
    sf::Uint8 t; packet >> t; b.type = Bullet::Type(t);
    return packet;
}
