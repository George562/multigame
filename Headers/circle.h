#pragma once
#include "../SFML-2.5.1/include/SFML/System.hpp"
#include <cmath>

struct Rect;

float distance(sf::Vector2f a, sf::Vector2f b) { return std::hypot(a.x - b.x, a.y - b.y); }
float LenOfVector(sf::Vector2f v) { return std::hypot(v.x, v.y); }

struct Circle {
    float PosX, PosY, Radius; // {x, y} of the center and the radius

    // Get and set position
    sf::Vector2f getPosition() const { return {PosX, PosY}; }
    void setPosition(float x, float y) { PosX = x; PosY = y; }
    void setPosition(sf::Vector2f v) { setPosition(v.x, v.y); }

    // Move the circle relatively to its current position
    void move(float x, float y) { setPosition(PosX + x, PosY + y); }
    void move(sf::Vector2f v) { setPosition(PosX + v.x, PosY + v.y); }

    // Get and set radius
    float getRadius() const { return Radius; }
    void setRadius(float r) { Radius = r; }

    // Set position and radius
    void setCircle(float x, float y, float r) { PosX = x; PosY = y; Radius = r; }
    void setCircle(sf::Vector2f pos, float r) { PosX = pos.x; PosY = pos.y; Radius = r; }

    // Check the intersection between two circles
    bool intersect(float x, float y, float r) const { // for future (if this part of game will be most slowed, then "float&")
        return distance(getPosition(), sf::Vector2f{x, y}) <= Radius + r;
    }
    bool intersect(Circle circle) const { return intersect(circle.PosX, circle.PosY, circle.Radius); }
    
    // Check if a point is inside the circle
    bool contains(float x, float y) const { return distance(getPosition(), sf::Vector2f{x, y}) <= Radius; }
    bool contains(sf::Vector2f point) const { return contains(point.x, point.y); }

    bool intersect(Rect rect);
};

sf::Packet& operator<<(sf::Packet& packet, Circle& a) {
    return packet << a.PosX << a.PosY << a.Radius;
}
sf::Packet& operator>>(sf::Packet& packet, Circle& a) {
    return packet >> a.PosX >> a.PosY >> a.Radius;
}