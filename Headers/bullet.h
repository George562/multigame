#include "text.h"

#define COMMON_BULLET_RADIUS 7

namespace BulletType {
    enum Type {
        Common,
        Bubble,
        // Explosion
    };
}

struct Bullet {
    float PosX, PosY, dx, dy;
    sf::Color color;
    int penetration;
    float damage;
    float radius = COMMON_BULLET_RADIUS;
    sf::CircleShape* circle;
    bool exlpode = false;
    sf::Time timer;
    Scale<float> ExplosionRadius = {1, 12, 0};
    BulletType::Type type;
    bool todel = false;

    Bullet() {}
    Bullet(float x, float y, float vx, float vy, sf::Color c, int pen, float d, float time, BulletType::Type t = BulletType::Common) {
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
        case BulletType::Bubble:
            if (exlpode && !todel) {
                if (ExplosionRadius.fromTop() > 0) {
                    ExplosionRadius += 1.f / 5;
                    circle->setFillColor(circle->getFillColor() - sf::Color(0, 0, 0, 4));
                    circle->setRadius(radius * ExplosionRadius.cur);
                    PosX -= radius / 5;
                    PosY -= radius / 5;
                } else todel = true;
            }
        case BulletType::Common:
            if (in(PosX, PosY, camera.x, camera.y, scw, sch)) {
                circle->setPosition(PosX - camera.x, PosY - camera.y);
                window.draw(*circle);
            }
        }
    }

    virtual void move(const vvr& wallsRect, int& size, sf::Clock& clock) {
        if (dx == 0 && dy == 0) return;
        sf::Vector2i res = WillCollisionWithWalls(wallsRect, size, PosX, PosY, radius, radius, dx, dy);
        dx *= res.x;
        dy *= res.y;
        switch (type) {
        case BulletType::Bubble:
            PosX += dx * (timer - clock.getElapsedTime()).asSeconds();
            PosY += dy * (timer - clock.getElapsedTime()).asSeconds();
            if (timer < clock.getElapsedTime()) { dy = 0; dx = 0; exlpode = true; }
            break;
        case BulletType::Common:
            PosX += dx;
            PosY += dy;
        }
    }
};

using vB = std::vector<Bullet>;
vB Bullets(0);

sf::Packet& operator<<(sf::Packet& packet, Bullet& b) {
    return packet << b.PosX << b.PosY << b.dx << b.dy << b.color << b.damage << b.radius;
}
sf::Packet& operator>>(sf::Packet& packet, Bullet& b) {
    return packet >> b.PosX >> b.PosY >> b.dx >> b.dy >> b.color >> b.damage >> b.radius;
}
