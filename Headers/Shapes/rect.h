#pragma once
#include "../../SFML-2.5.1/include/SFML/Network.hpp"
#include "shape.h"

struct Rect {
public:
    // Get and set position
    sf::Vector2f getPosition() { return position; }
    void setPosition(float x, float y) { position.x = x; position.y = y; }
    void setPosition(sf::Vector2f v) { setPosition(v.x, v.y); }

    // Get right and bottom
    float getRight() const { return position.x + size.x; }
    float getBottom() const { return position.y + size.y; }

    // Get corners
    sf::Vector2f getRightTop() const { return sf::Vector2f(position.x + size.x, position.y); }
    sf::Vector2f getRightBottom() const { return sf::Vector2f(position.x + size.x, position.y + size.y); }
    sf::Vector2f getLeftBottom() const { return sf::Vector2f(position.x, position.y + size.y); }

    // Get and set center
    void setCenter(float x, float y) { setPosition(x - size.x / 2, y - size.y / 2); }
    void setCenter(sf::Vector2f v) { setCenter(v.x, v.y); }
    sf::Vector2f getCenter() const { return position + size / 2.f; }

    // Get and set size
    sf::Vector2f getSize() const { return size; }
    void setSize(float w, float h) { size.x = w; size.y = h; }
    void setSize(sf::Vector2f v) { setSize(v.x, v.y); }

    // Set position and size
    void setRect(float x, float y, float w, float h) { setPosition(x, y); setSize(w, h); }
    void setRect(sf::Vector2f pos, sf::Vector2f size) { setPosition(pos); setSize(size); }
    void setRect(Rect rect) { setPosition(rect.getPosition()); setSize(rect.getSize()); }

private:
    sf::Vector2f size, position;
};

using vr = std::vector<Rect>;
using vvr = std::vector<vr>;

sf::Packet& operator<<(sf::Packet& packet, Rect& a) {
    return packet << a.getPosition().x << a.getPosition().y << a.getSize().x << a.getSize().y;
}
sf::Packet& operator>>(sf::Packet& packet, Rect& a) {
    sf::Vector2f v1, v2;
    packet >> v1.x >> v1.y >> v2.x >> v2.y;
    a.setRect(v1, v2);
    return packet;
}