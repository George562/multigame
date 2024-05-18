#pragma once
#include "../../SFML-2.5.1/include/SFML/Network.hpp"
#include "shape.h"

class Rect : public Shape {
protected:
    sf::Vector2f size;
public:
    Rect() : Shape(4), size(sf::Vector2f(0, 0)) {}
    Rect(float x, float y, float w, float h) : Shape(4), size(sf::Vector2f(w, h)) {
        setPosition(sf::Vector2f(x, y));
    }
    Rect(sf::Vector2f position, sf::Vector2f size) : Shape(4), size(size) {
        setPosition(position);
    }
    float getRight() const { return points[2].x; }
    float getBottom() const { return points[2].y; }
    sf::Vector2f getRightTop() const { return points[1]; }
    sf::Vector2f getRightBottom() const { return points[2]; }
    sf::Vector2f getLeftBottom() const { return points[3]; }

    // Get and set position
    sf::Vector2f getPosition() const { return points[0]; }
    void setPosition(float x, float y) {
        points[0] = sf::Vector2f(x, y);
        points[1] = sf::Vector2f(x + size.x, y);
        points[2] = sf::Vector2f(x + size.x, y + size.y);
        points[3] = sf::Vector2f(x, y + size.y);
    }
    void setPosition(sf::Vector2f v) { setPosition(v.x, v.y); }

    // Set position of rect center
    sf::Vector2f getCenter() const { return points[0] + size / 2.f; }
    void setCenter(float x, float y) { setPosition(x - size.x / 2, y - size.y / 2); }
    void setCenter(sf::Vector2f v) { setCenter(v.x, v.y); }

    // Move the rect relatively to its current position
    void move(float x, float y) { setPosition(points[0].x + x, points[0].y + y); }
    void move(sf::Vector2f v) { setPosition(points[0].x + v.x, points[0].y + v.y); }

    // Get and set size
    sf::Vector2f getSize() const { return size; }
    void setSize(float w, float h) { size.x = w; size.y = h; }
    void setSize(sf::Vector2f v) { setSize(v.x, v.y); }

    // Set position and size
    void setRect(float x, float y, float w, float h) { setPosition(x, y); size.x = w; size.y = h; }
    void setRect(sf::Vector2f pos, sf::Vector2f size) { setPosition(pos); this->size = size; }
    void setRect(Rect rect) { setPosition(rect.getPosition()); size = rect.size; }
};

using vr = std::vector<Rect>;
using vvr = std::vector<vr>;

sf::Packet& operator<<(sf::Packet& packet, Rect& a) {
    return packet << a.getPosition().x << a.getPosition().y << a.getSize().x << a.getSize().y;
}
sf::Packet& operator>>(sf::Packet& packet, Rect& a) {
    sf::Vector2f pos, size;
    packet >> pos.x >> pos.y >> size.x >> size.y;
    a.setRect(pos, size);
    return packet;
}