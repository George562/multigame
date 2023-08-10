#include "tools.h"
#include "location.h"

#define COMMON_BULLET_RADIUS 7

struct Bullet : public sf::Drawable {
    enum Type : sf::Uint8 {
        Common,
        Bubble
    };

    float PosX, PosY, dx, dy;
    int penetration;
    float damage;
    float radius = COMMON_BULLET_RADIUS;
    sf::CircleShape circle;
    bool exlpode = false;
    sf::Time timer;
    Scale<float> ExplosionRadius = {1, 12, 1};
    Bullet::Type type;
    bool todel = false;
    sf::Clock* localClock;

    Bullet() {}
    Bullet(sf::Vector2f pos, sf::Vector2f v, int penetr, float dmg, Bullet::Type t = Bullet::Common, sf::Time time = sf::Time::Zero) {
        PosX = pos.x; PosY = pos.y; 
        dx = v.x; dy = v.y;
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
        circle.setRadius(radius);
        localClock = new sf::Clock();
    }
    
    Rect getRect() { return {PosX, PosY, dx, dy}; }
    sf::Vector2f getPosition() { return {PosX, PosY}; }
    void setPosition(sf::Vector2f &v) { PosX = v.x; PosY = v.y; }
    void setPosition(float x, float y) { PosX = x; PosY = y; }
    sf::Vector2f getCenter() { return {PosX + radius / 2, PosY + radius / 2}; }

    void draw(sf::RenderTarget& target, sf::RenderStates states = sf::RenderStates::Default) const {
        target.draw(circle, states);
    }

    void move(Location& location) {
        if (dx != 0 || dy != 0) {
            sf::Vector2i res = WillCollisionWithWalls(location.wallsRect, PosX, PosY, radius, radius, dx, dy);
            if (res.x == -1 || res.y == -1) penetration--;
            dx *= res.x;
            dy *= res.y;
        }
        switch (type) {
            case Bullet::Bubble:
                PosX += dx * (timer - localClock->getElapsedTime()).asSeconds();
                PosY += dy * (timer - localClock->getElapsedTime()).asSeconds();
                if (timer < localClock->getElapsedTime()) { dy = 0; dx = 0; exlpode = true; }
                if (exlpode && !todel) {
                    if (ExplosionRadius.fromTop() > 0) {
                        ExplosionRadius += 1.f / 5;
                        circle.setFillColor(circle.getFillColor() - sf::Color(0, 0, 0, 4));
                        radius = COMMON_BULLET_RADIUS * ExplosionRadius.cur;
                        circle.setRadius(radius);
                        PosX -= COMMON_BULLET_RADIUS / 5;
                        PosY -= COMMON_BULLET_RADIUS / 5;
                    } else todel = true;
                }
                break;
            case Bullet::Common:
                PosX += dx;
                PosY += dy;
        }
        circle.setPosition(PosX, PosY);
    }
};

using vB = std::vector<Bullet>;
vB Bullets(0);

sf::Packet& operator<<(sf::Packet& packet, Bullet& b) {
    sf::Color clr = b.circle.getFillColor();
    return packet << b.PosX << b.PosY << b.dx << b.dy << b.penetration << clr << b.damage << b.timer.asSeconds() << b.type;
}
sf::Packet& operator>>(sf::Packet& packet, Bullet& b) {
    sf::Color clr;
    float timer;
    packet >> b.PosX >> b.PosY >> b.dx >> b.dy >> b.penetration >> clr >> b.damage >> timer;
    b.timer = sf::seconds(timer);
    sf::Uint8 t; packet >> t; b.type = Bullet::Type(t);
    b.circle.setFillColor(clr);
    b.circle.setRadius(b.radius);
    return packet;
}
