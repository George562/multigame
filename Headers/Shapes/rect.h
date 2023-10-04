#pragma once
#include "../../SFML-2.5.1/include/SFML/Network.hpp"
#include "shape.h"

struct Circle;


struct Rect : public sf::Shape {
    float Width, Height;
    float getRight() const { return getPosition().x + Width; }
    float getBottom() const { return getPosition().y + Height; }
    sf::Vector2f getRightTop() const { return sf::Vector2f{getPosition().x + Width, getPosition().y}; }
    sf::Vector2f getRightBottom() const { return sf::Vector2f{getPosition().x + Width, getPosition().y + Height}; }
    sf::Vector2f getLeftBottom() const { return sf::Vector2f{getPosition().x, getPosition().y + Height}; }

    // Set position of rect center
    void setCenter(float x, float y) { setPosition(x - Width / 2, y - Height / 2); }
    void setCenter(sf::Vector2f& v) { setCenter(v.x, v.y); }
    sf::Vector2f getCenter() const { return {getPosition().x + Width / 2, getPosition().y + Height / 2}; }

    // Get and set size
    sf::Vector2f getSize() const { return {Width, Height}; }
    void setSize(float w, float h) { Width = w; Height = h; }
    void setSize(sf::Vector2f& v) { setSize(v.x, v.y); }

    // Set position and size
    void setRect(float x, float y, float w, float h) { setPosition(x, y); Width = w; Height = h; }
    void setRect(Rect& rect) { setPosition(rect.getPosition()); Width = rect.Width; Height = rect.Height; }

    // Check the intersection between two rectangles
    bool intersect(float x, float y, float w, float h) const {
        return x <= getPosition().x + Width && getPosition().x <= x + w && y <= getPosition().y + Height && getPosition().y <= y + h;
    }
    bool intersect(Rect& rect) const {
        return rect.getPosition().x <= getPosition().x + Width && getPosition().x <= rect.getPosition().x + rect.Width && rect.getPosition().y <= getPosition().y + Height && getPosition().y <= rect.getPosition().y + rect.Height;
    }
    bool intersect(Rect& rect, Rect& intersection) const {
        intersection.setPosition(std::max(getPosition().x, rect.getPosition().x), std::max(getPosition().y, rect.getPosition().y));
        intersection.setSize(std::min(getPosition().x + Width, rect.getPosition().x + rect.Width) - intersection.getPosition().x,
                             std::min(getPosition().y + Height, rect.getPosition().y + rect.Height) - intersection.getPosition().y);
        return intersect(rect);
    }
    
    // Check if a point is inside the rectangle's area
    bool contains(float x, float y) const {
        return getPosition().x <= x && x <= getPosition().x + Width && getPosition().y <= y && y <= getPosition().y + Height;
    }
    bool contains(sf::Vector2f& point) const { return contains(point.x, point.y); }

    bool intersect(Circle& circle);
};

using vr = std::vector<Rect>;
using vvr = std::vector<vr>;

sf::Packet& operator<<(sf::Packet& packet, Rect& a) {
    return packet << a.getPosition().x << a.getPosition().y << a.Width << a.Height;
}
sf::Packet& operator>>(sf::Packet& packet, Rect& a) {
    sf::Vector2f vf;
    packet >> vf.x >> vf.y >> a.Width >> a.Height;
    a.setPosition(vf);
    return packet;
}