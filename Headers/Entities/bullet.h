#pragma once
#include "../LevelSystem/location.h"

#define COMMON_BULLET_RADIUS 10
#define COMMON_BULLET_PENETRATION 0

sf::CircleShape circleShape;

////////////////////////////////////////////////////////////
// Class
////////////////////////////////////////////////////////////

#pragma pack(push, 1)
class Bullet : public sf::Drawable {
public:
    bool todel = false;
    float damage;
    int penetration;
    sf::Vector2f Velocity;
    faction::Type fromWho;
    sf::Clock* localClock = nullptr;
    sf::Time timer;
    sf::Color color;
    CollisionCircle hitbox;

    Bullet() {
        localClock = new sf::Clock();
    }

    Bullet(faction::Type faction, sf::Vector2f pos, sf::Vector2f velocity, float dmg,
           int penetr = COMMON_BULLET_PENETRATION, sf::Time time = sf::Time::Zero) : Bullet() {
        hitbox.setCenter(pos);
        damage = dmg;
        penetration = penetr;
        Velocity = velocity;
        fromWho = faction;
        timer = time;
        hitbox.setRadius(COMMON_BULLET_RADIUS);
        switch (faction) {
            case faction::Player: color = sf::Color(30, 195, 255); break;
            case faction::Enemy:  color = sf::Color(72,  61, 139); break;
        }
    }

    ~Bullet() {
        if (localClock) {
            delete localClock;
        }
    }

    void move(Location* location) {
        float elapsedTime = std::min(localClock->restart().asSeconds(), oneOverSixty);
        if (Velocity.x != 0 && Velocity.y != 0) {
            sf::Vector2i res = WillCollisionWithWalls(location->wallsRect, hitbox, Velocity * elapsedTime);
            if (res.x == -1 || res.y == -1) {
                penetration--;
            }
            Velocity = Velocity * (sf::Vector2f)res;
        }
        hitbox.move(Velocity * elapsedTime);
    }
    void draw(sf::RenderTarget& target, sf::RenderStates states = sf::RenderStates::Default) const {
        circleShape.setFillColor(color);
        circleShape.setRadius(hitbox.getRadius());
        circleShape.setPosition(hitbox.getPosition());
        target.draw(circleShape);
    }
};
#pragma pack(pop)

std::vector<Bullet*> Bullets(0);

sf::Packet& operator<<(sf::Packet& packet, Bullet& b) {
    return packet << b.fromWho << b.hitbox.getCenter().x << b.hitbox.getCenter().y << b.Velocity.x << b.Velocity.y << b.penetration << b.color << b.damage << b.timer.asSeconds();
}
sf::Packet& operator>>(sf::Packet& packet, Bullet& b) {
    float timer;
    sf::Vector2f pos;
    packet >> b.fromWho >> pos.x >> pos.y >> b.Velocity.x >> b.Velocity.y >> b.penetration >> b.color >> b.damage >> timer;
    b.hitbox.setCenter(pos);
    b.timer = sf::seconds(timer);
    return packet;
}
