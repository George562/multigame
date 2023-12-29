#pragma once
#include "../../SFML-2.5.1/include/SFML/Network.hpp"
#include "shape.h"

struct Circle {
public:
    // Get and set position
    sf::Vector2f getPosition() const { return position; }
    void setPosition(float x, float y) { position.x = x; position.y = y; }
    void setPosition(sf::Vector2f v) { setPosition(v.x, v.y); }

    // Move the circle by a given offset
    void move(float offsetX, float offsetY) { setPosition(position.x + offsetX, position.y + offsetY); }
    void move(sf::Vector2f offset) { setPosition(position + offset); }

    // Get and set radius
    float getRadius() const { return radius; }
    void setRadius(float r) { radius = r; }

    // Set position and radius
    void setCircle(float x, float y, float r) { setPosition(x, y); setRadius(r); }
    void setCircle(sf::Vector2f pos, float r) { setPosition(pos); setRadius(r); }

private:
    float radius;
    sf::Vector2f position;
};

sf::Packet& operator<<(sf::Packet& packet, Circle& a) {
    return packet << a.getPosition().x << a.getPosition().y << a.getRadius();
}
sf::Packet& operator>>(sf::Packet& packet, Circle& a) {
    float x, y, r;
    return packet >> x >> y >> r;
    a.setCircle(x, y, r);
}