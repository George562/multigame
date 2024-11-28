#pragma once
#include "../../SFML-2.5.1/include/SFML/Network.hpp"
#include "collisionShape.h"
#define _USE_MATH_DEFINES
#include <cmath>

class CollisionCircle : public CollisionShape {
private:
    sf::Vector2f center;
    float Radius;
    void updateShape() override {
        for (int i = 0; i < pointCount; i++) {
            points[i] = sf::Vector2f(center.x + Radius * std::cos(i * 2 * M_PI / pointCount),
                                     center.y + Radius * std::sin(i * 2 * M_PI / pointCount));
        }
    }
public:
    CollisionCircle() : CollisionShape(20), Radius(0), center(0, 0) {}
    CollisionCircle(sf::Vector2f position, float radius) : CollisionShape(20), Radius(radius), center(position) {
        updateShape();
    }
    sf::Vector2f getPosition() const { return center - sf::Vector2f(Radius, Radius); }
    void setPosition(float x, float y) { setCenter(x + Radius, y + Radius); }
    void setPosition(sf::Vector2f v) { setCenter(v.x + Radius, v.y + Radius); }

    sf::Vector2f getCenter() const { return center; }
    void setCenter(float x, float y) { center = sf::Vector2f(x, y); updateShape(); }
    void setCenter(sf::Vector2f v) { setCenter(v.x, v.y); }

    // Move the circle relatively to its current position
    void move(float x, float y) { setCenter(center.x + x, center.y + y); }
    void move(sf::Vector2f v) { setCenter(center + v); }

    // Get size
    sf::Vector2f getSize() const { return sf::Vector2f(Radius * 2, Radius * 2); }
    void setSize(float x, float y) {
        Radius = std::hypot(x, y) / 2;
        updateShape();
    }

    // Get and set radius
    float getRadius() const { return Radius; }
    void setRadius(float r) { Radius = r; updateShape(); }

    // Set position and radius
    void setCircle(float x, float y, float r) { center.x = x; center.y = y; Radius = r; updateShape(); }
    void setCircle(sf::Vector2f pos, float r) { center = pos; Radius = r; updateShape(); }
    void setCircle(CollisionCircle circle) { center = circle.center; Radius = circle.Radius; updateShape(); }

    sf::FloatRect getBounds() const { return sf::FloatRect(getPosition(), getSize()); }
};


sf::Packet& operator<<(sf::Packet& packet, CollisionCircle& a) {
    return packet << a.getPosition().x << a.getPosition().y << a.getRadius();
}
sf::Packet& operator>>(sf::Packet& packet, CollisionCircle& a) {
    float x, y, r;
    return packet >> x >> y >> r;
    a.setCircle(x, y, r);
}