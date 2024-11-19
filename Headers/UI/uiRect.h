#pragma once
#include "../../SFML-2.5.1/include/SFML/System.hpp"
#include "../../SFML-2.5.1/include/SFML/Network.hpp"
#include <vector>

struct UIRect : public sf::Transformable {
    unsigned int Width = 0, Height = 0;

    float getTop() const { return getPosition().y; }
    float getRight() const { return getPosition().x + Width; }
    float getBottom() const { return getPosition().y + Height; }
    float getLeft() const { return getPosition().x; }
    sf::Vector2f getRightTop() const { return sf::Vector2f(getRight(), getTop()); }
    sf::Vector2f getRightBottom() const { return sf::Vector2f(getRight(), getBottom()); }
    sf::Vector2f getLeftBottom() const { return sf::Vector2f(getLeft(), getBottom()); }
    sf::Vector2f getLeftTop() const { return getPosition(); }

    // Get and set position
    sf::Vector2f getPosition() const { return sf::Transformable::getPosition(); }
    virtual void setPosition(float x, float y) { sf::Transformable::setPosition(x, y); }
    void setPosition(sf::Vector2f v) { setPosition(v.x, v.y); }

    // Set position of rect center
    virtual void setCenter(float x, float y) { setPosition(x - Width / 2, y - Height / 2); }
    void setCenter(sf::Vector2f v) { setCenter(v.x, v.y); }
    sf::Vector2f getCenter() const { return { getLeft() + Width / 2, getTop() + Height / 2}; }

    // Move the rect relatively to its current position
    virtual void move(float x, float y) { setPosition(getLeft() + x, getTop() + y); }
    void move(sf::Vector2f v) { setPosition(getLeft() + v.x, getTop() + v.y); }

    // Get and set size
    sf::Vector2u getSize() const { return {Width, Height}; }
    virtual void setSize(float w, float h) { Width = w; Height = h; }
    void setSize(sf::Vector2u v) { setSize(v.x, v.y); }

    // Get and set position and size
    sf::FloatRect getRect() { return { getLeft(), getTop(), 1.0f * Width, 1.0f * Height }; }
    virtual void setRect(float x, float y, unsigned int w, unsigned int h) { setPosition(x, y); setSize(w, h); }
    void setRect(sf::Vector2f pos, sf::Vector2u size) { setRect(pos.x, pos.y, size.x, pos.y); }
    void setRect(UIRect rect) { setRect(rect.getLeft(), rect.getTop(), rect.Width, rect.Height); }

    sf::FloatRect getGlobalBounds() {
        return sf::FloatRect(getPosition(),
                             sf::Vector2f(getSize().x * getScale().x,
                                          getSize().y * getScale().y));
    }

    // Check the intersection between two rectangles
    virtual bool intersect(float x, float y, float w, float h) const {
        return x <= getLeft() + Width && getLeft() <= x + w && y <= getTop() + Height && getTop() <= y + h;
    }
    virtual bool intersect(UIRect& rect) const {
        return rect.getLeft() <= getLeft() + Width && getLeft() <= rect.getLeft() + rect.Width && rect.getTop() <= getTop() + Height && getTop() <= rect.getTop() + rect.Height;
    }
    virtual bool intersect(UIRect& rect, UIRect& intersection) const {
        intersection.setPosition(std::max(getLeft(), rect.getLeft()), std::max(getTop(), rect.getTop()));
        intersection.setSize(std::min(getLeft() + Width, rect.getLeft() + rect.Width) - intersection.getLeft(),
                             std::min(getTop() + Height, rect.getTop() + rect.Height) - intersection.getTop());
        return intersect(rect);
    }

    // Check if a point is inside the rectangle's area
    virtual bool contains(float x, float y) const {
        return getLeft() <= x && x <= getLeft() + Width && getTop() <= y && y <= getTop() + Height;
    }
    virtual bool contains(sf::Vector2f point) const { return contains(point.x, point.y); }
};

sf::Packet& operator<<(sf::Packet& packet, UIRect& a) {
    return packet << a.getLeft() << a.getTop() << a.Width << a.Height;
}
// sf::Packet& operator>>(sf::Packet& packet, UIRect& a) {
//     return packet >> a.getLeft() >> a.getTop() >> a.Width >> a.Height;
// }
