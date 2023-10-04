#include "tools.h"
#include "location.h"

#define COMMON_BULLET_RADIUS 7
#define COMMON_BULLET_PENETRATION 0

struct Bullet : public sf::Drawable, public Circle {
    enum Type : sf::Uint8 {
        Common,
        Bubble
    };

    sf::Vector2f Velocity;
    int penetration;
    float damage;
    sf::CircleShape circle;
    bool explode = false;
    sf::Time timer;
    Scale<float> ExplosionRadius = {1, 12, 1};
    Bullet::Type type;
    Fraction::Fraction fromWho;
    bool todel = false;
    sf::Clock* localClock;

    Bullet() {}
    Bullet(Fraction::Fraction f, sf::Vector2f pos, sf::Vector2f v, float dmg, int penetr = COMMON_BULLET_PENETRATION, Bullet::Type t = Bullet::Common, sf::Time time = sf::Time::Zero) {
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
                circle.setFillColor(sf::Color(rand() % 256, rand() % 256, rand() % 256));
                break;
            case Bullet::Bubble:
                circle.setFillColor(sf::Color(rand() % 256, rand() % 256, rand() % 256));
                break;
        }
        circle.setRadius(Radius);
        circle.setOrigin(Radius, Radius);
        localClock = new sf::Clock();
    }

    void draw(sf::RenderTarget& target, sf::RenderStates states = sf::RenderStates::Default) const {
        target.draw(circle, states);
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
                        circle.setFillColor(circle.getFillColor() - sf::Color(0, 0, 0, 4));
                        Radius = COMMON_BULLET_RADIUS * ExplosionRadius.cur;
                        circle.setRadius(Radius);
                        Circle::move(-COMMON_BULLET_RADIUS / 5, -COMMON_BULLET_RADIUS / 5);
                    } else todel = true;
                }
                break;
            case Bullet::Common:
                Circle::move(Velocity);
        }
        circle.setPosition(PosX, PosY);
    }
};

using vB = std::vector<Bullet>;
vB Bullets(0);

sf::Packet& operator<<(sf::Packet& packet, Bullet& b) {
    sf::Color clr = b.circle.getFillColor();
    return packet << b.PosX << b.PosY << b.Velocity.x << b.Velocity.y << b.penetration << clr << b.damage << b.timer.asSeconds() << b.type;
}
sf::Packet& operator>>(sf::Packet& packet, Bullet& b) {
    sf::Color clr;
    float timer;
    packet >> b.PosX >> b.PosY >> b.Velocity.x >> b.Velocity.y >> b.penetration >> clr >> b.damage >> timer;
    b.timer = sf::seconds(timer);
    sf::Uint8 t; packet >> t; b.type = Bullet::Type(t);
    b.circle.setFillColor(clr);
    b.circle.setRadius(b.Radius);
    return packet;
}
