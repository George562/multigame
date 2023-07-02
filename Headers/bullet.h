#include "text.h"

#define COMMON_BULLET_RADIUS 7

struct Bullet {
    enum Type {
        Common,
        Bubble,
        // Explosion
    };

    float PosX, PosY, dx, dy;
    sf::Color color;
    int penetration;
    float damage;
    float radius = COMMON_BULLET_RADIUS;
    sf::CircleShape* circle;
    bool exlpode = false;
    sf::Time timer;
    Scale<float> ExplosionRadius = {1, 12, 1};
    Bullet::Type type;
    bool todel = false;

    Bullet() {}
    Bullet(float x, float y, float vx, float vy, sf::Color c, int pen, float d, float time, Bullet::Type t = Bullet::Common) {
        PosX = x; PosY = y; dx = vx; dy = vy;
        color = c;
        penetration = pen;
        damage = d;
        circle = new sf::CircleShape();
        timer = sf::seconds(time);
        type = t;
        UpdateCircleShape();
    }

    virtual void UpdateCircleShape() {
        circle->setRadius(radius);
        circle->setFillColor(color);
    }
    
    virtual sf::Rect<float> getRect() { return {PosX, PosY, dx, dy}; }
    virtual sf::Vector2f getPosition() { return {PosX, PosY}; }
    virtual void setPosition(sf::Vector2f &v) { PosX = v.x; PosY = v.y; }
    virtual void setPosition(float x, float y) { PosX = x; PosY = y; }

    virtual void draw(sf::RenderWindow& window, sf::Vector2f& camera) {
        switch (type) {
        case Bullet::Bubble:
            if (exlpode && !todel) {
                if (ExplosionRadius.fromTop() > 0) {
                    ExplosionRadius += 1.f / 5;
                    circle->setFillColor(circle->getFillColor() - sf::Color(0, 0, 0, 4));
                    circle->setRadius(radius * ExplosionRadius.cur);
                    PosX -= radius / 5;
                    PosY -= radius / 5;
                } else todel = true;
            }
        case Bullet::Common:
            if (in(PosX, PosY, camera.x, camera.y, scw, sch)) {
                circle->setPosition(PosX - camera.x, PosY - camera.y);
                window.draw(*circle);
            }
        }
    }

    virtual void move(const vvr& wallsRect, sf::Clock& clock) {
        if (dx == 0 && dy == 0) return;
        sf::Vector2i res = WillCollisionWithWalls(wallsRect, PosX, PosY, radius, radius, dx, dy);
        dx *= res.x;
        dy *= res.y;
        switch (type) {
        case Bullet::Bubble:
            PosX += dx * (timer - clock.getElapsedTime()).asSeconds();
            PosY += dy * (timer - clock.getElapsedTime()).asSeconds();
            if (timer < clock.getElapsedTime()) { dy = 0; dx = 0; exlpode = true; }
            break;
        case Bullet::Common:
            PosX += dx;
            PosY += dy;
        }
    }
};

using vB = std::vector<Bullet>;
vB Bullets(0);

sf::Packet& operator<<(sf::Packet& packet, Bullet& b) {
    return packet << b.PosX << b.PosY << b.dx << b.dy << b.color << b.damage << b.type;
}
sf::Packet& operator>>(sf::Packet& packet, Bullet& b) {
    packet >> b.PosX >> b.PosY >> b.dx >> b.dy >> b.color >> b.damage;
    int t; packet >> t; b.type = Bullet::Type(t);
    return packet;
}
