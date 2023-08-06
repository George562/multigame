#pragma once
#include "../SFML-2.5.1/include/SFML/System.hpp"
#include <vector>

struct Rect {
    float PosX, PosY, Width, Height;

    // Get and set position
    sf::Vector2f getPosition() { return {PosX, PosY}; }
    void setPosition(sf::Vector2f v) { PosX = v.x; PosY = v.y; }
    void setPosition(float x, float y) { PosX = x; PosY = y; }

    // Set position of rect center
    void setCenter(float x, float y) { setPosition(x - Width / 2, y - Height / 2); }
    void setCenter(sf::Vector2f v) { setCenter(v.x, v.y); }
    sf::Vector2f getCenter() { return {PosX + Width / 2, PosY + Height / 2}; }

    // Move the rect relatively to its current position
    void move(float x, float y) { PosX += x; PosY += y; }
    void move(sf::Vector2f v) { PosX += v.x; PosY += v.y; }

    // Get and set size
    sf::Vector2f getSize() { return {Width, Height}; }
    void setSize(float w, float h) { Width = w; Height = h; }
    void setSize(sf::Vector2f v) { setSize(v.x, v.y); }

    // Get and set position and size
    void setRect(float x, float y, float w, float h) { PosX = x; PosY = y; Width = w; Height = h; }
    void setRect(Rect rect) { PosX = rect.PosX; PosY = rect.PosY; Width = rect.Width; Height = rect.Height; }

    // Check the intersection between two rectangles
    bool intersect(float x, float y, float w, float h) {
        return x <= PosX + Width && PosX <= x + w && y <= PosY + Height && PosY <= y + h;
    }
    bool intersect(Rect rect) {
        return rect.PosX <= PosX + Width && PosX <= rect.PosX + rect.Width && rect.PosY <= PosY + Height && PosY <= rect.PosY + rect.Height;
    }
    bool intersect(Rect& rect, Rect& intersection) {
        intersection.setPosition(std::max(PosX, rect.PosX), std::max(PosY, rect.PosY));
        intersection.setSize(std::min(PosX + Width, rect.PosX + rect.Width) - intersection.PosX,
                             std::min(PosY + Height, rect.PosY + rect.Height) - intersection.PosY);
        return intersect(rect);
    }
    
    // Check if a point is inside the rectangle's area
    bool contains(float x, float y) {
        return PosX <= x && x <= PosX + Width && PosY <= y && y <= PosY + Height;
    }
    bool contains(sf::Vector2f point) { return contains(point.x, point.y); }
};

using vr = std::vector<Rect>;
using vvr = std::vector<vr>;

sf::Packet& operator<<(sf::Packet& packet, Rect& a) {
    return packet << a.PosX << a.PosY << a.Width << a.Height;
}
sf::Packet& operator>>(sf::Packet& packet, Rect& a) {
    return packet >> a.PosX >> a.PosY >> a.Width >> a.Height;
}