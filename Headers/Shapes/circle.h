#pragma once
#include "../../SFML-2.5.1/include/SFML/Network.hpp"
#include "shape.h"
#define _USE_MATH_DEFINES
#include <cmath>

class Circle : public Shape {
private:
    sf::Vector2f center;
    float Radius;
    void updateShape() {
        for (int i = 0; i < 30; i++) {
            points[i] = sf::Vector2f(center.x + Radius * std::cos(i * 2 * M_PI / 30),
                                     center.y + Radius * std::sin(i * 2 * M_PI / 30));
        }
    }
public:
    Circle() : Shape(30), Radius(0), center(0, 0) {}
    Circle(sf::Vector2f position, float radius) : Shape(30), Radius(radius), center(position) {
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

    // Get and set radius
    float getRadius() const { return Radius; }
    void setRadius(float r) { Radius = r; updateShape(); }

    // Set position and radius
    void setCircle(float x, float y, float r) { center.x = x; center.y = y; Radius = r; updateShape(); }
    void setCircle(sf::Vector2f pos, float r) { center = pos; Radius = r; updateShape(); }
    void setCircle(Circle circle) { center = circle.center; Radius = circle.Radius; updateShape(); }
};


sf::Packet& operator<<(sf::Packet& packet, Circle& a) {
    return packet << a.getPosition().x << a.getPosition().y << a.getRadius();
}
sf::Packet& operator>>(sf::Packet& packet, Circle& a) {
    float x, y, r;
    return packet >> x >> y >> r;
    a.setCircle(x, y, r);
}