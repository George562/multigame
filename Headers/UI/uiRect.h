#pragma once
#include "../SFML-2.5.1/include/SFML/Network.hpp"
#include <vector>

struct UIRect {
    float PosX, PosY, Width, Height;

    float getRight() const { return PosX + Width; }
    float getBottom() const { return PosY + Height; }
    sf::Vector2f getRightTop() const { return sf::Vector2f(PosX + Width, PosY); }
    sf::Vector2f getRightBottom() const { return sf::Vector2f(PosX + Width, PosY + Height); }
    sf::Vector2f getLeftBottom() const { return sf::Vector2f(PosX, PosY + Height); }

    // Get and set position
    sf::Vector2f getPosition() const { return {PosX, PosY}; }
    void setPosition(float x, float y) { PosX = x; PosY = y; }
    void setPosition(sf::Vector2f v) { setPosition(v.x, v.y); }

    // Set position of rect center
    void setCenter(float x, float y) { setPosition(x - Width / 2, y - Height / 2); }
    void setCenter(sf::Vector2f v) { setCenter(v.x, v.y); }
    sf::Vector2f getCenter() const { return {PosX + Width / 2, PosY + Height / 2}; }

    // Move the rect relatively to its current position
    void move(float x, float y) { setPosition(PosX + x, PosY + y); }
    void move(sf::Vector2f v) { setPosition(PosX + v.x, PosY + v.y); }

    // Get and set size
    sf::Vector2f getSize() const { return {Width, Height}; }
    void setSize(float w, float h) { Width = w; Height = h; }
    void setSize(sf::Vector2f v) { setSize(v.x, v.y); }

    // Set position and size
    void setRect(float x, float y, float w, float h) { PosX = x; PosY = y; Width = w; Height = h; }
    void setRect(UIRect rect) { PosX = rect.PosX; PosY = rect.PosY; Width = rect.Width; Height = rect.Height; }

    // Check the intersection between two rectangles
    bool intersect(float x, float y, float w, float h) const {
        return x <= PosX + Width && PosX <= x + w && y <= PosY + Height && PosY <= y + h;
    }
    bool intersect(UIRect& rect) const {
        return rect.PosX <= PosX + Width && PosX <= rect.PosX + rect.Width && rect.PosY <= PosY + Height && PosY <= rect.PosY + rect.Height;
    }
    bool intersect(UIRect& rect, UIRect& intersection) const {
        intersection.setPosition(std::max(PosX, rect.PosX), std::max(PosY, rect.PosY));
        intersection.setSize(std::min(PosX + Width, rect.PosX + rect.Width) - intersection.PosX,
                             std::min(PosY + Height, rect.PosY + rect.Height) - intersection.PosY);
        return intersect(rect);
    }
    
    // Check if a point is inside the rectangle's area
    bool contains(float x, float y) const {
        return PosX <= x && x <= PosX + Width && PosY <= y && y <= PosY + Height;
    }
    bool contains(sf::Vector2f point) const { return contains(point.x, point.y); }
};

using vr = std::vector<UIRect>;
using vvr = std::vector<vr>;

sf::Packet& operator<<(sf::Packet& packet, UIRect& a) {
    return packet << a.PosX << a.PosY << a.Width << a.Height;
}
sf::Packet& operator>>(sf::Packet& packet, UIRect& a) {
    return packet >> a.PosX >> a.PosY >> a.Width >> a.Height;
}
