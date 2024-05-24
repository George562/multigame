#pragma once
#include <vector>
#include "../Utility/VectorTools.h"

class CollisionShape {
protected:
    std::vector<sf::Vector2f> points;
    size_t pointCount;
    virtual void updateShape() = 0;
public:

    CollisionShape() : pointCount(0) {}
    CollisionShape(size_t count) : pointCount(count) { points.resize(count); }
    CollisionShape(std::vector<sf::Vector2f> points) : points(points), pointCount(points.size()) {}

    virtual sf::Vector2f getPosition() const = 0;

    virtual sf::Vector2f getCenter() const = 0;

    virtual sf::Vector2f getSize() const = 0;
    virtual void move(float x, float y) = 0;
    virtual void move(sf::Vector2f v) = 0;

    bool contains(float x, float y) const {
        return contains(sf::Vector2f(x, y));
    }
    bool contains(sf::Vector2f point) const {
        for (int i = 0; i < pointCount; i++) {
            if (cross(points[(i + 1) % pointCount] - points[i], point - points[i]) < 0) {
                return false;
            }
        }
        return true;
    }

    bool intersect(CollisionShape& shape) {
        sf::Vector2f pos  =       getPosition(), size  =       getSize(),
                     pos2 = shape.getPosition(), size2 = shape.getSize();
        if (!(pos2.x <= pos.x + size.x && pos.x <= pos2.x + size2.x && pos2.y <= pos.y + size.y && pos.y <= pos2.y + size2.y)) {
            return false;
        }
        for (int i = 0; i < shape.pointCount; i++) {
            if (contains(shape.points[i])) {
                return true;
            }
        }
        sf::Vector2f a, b, ab, c, d, cd, intersectionPoint;
        for (int i = 0; i < pointCount; i++) {
            a = points[i]; b = points[(i + 1) % pointCount]; ab = a - b;
            for (int j = 0; j < shape.pointCount; j++) {
                c = shape.points[j]; d = shape.points[(j + 1) % shape.pointCount]; cd = c - d;
                if (ab.x * cd.y - ab.y * cd.x == 0) {
                    if ((ab.y == 0 && std::abs(a.y - c.y) < 0.01 && std::min(a.x, b.x) <= std::max(c.x, d.x) && std::min(c.x, d.x) <= std::max(a.x, b.x)) ||
                       (ab.x == 0 && std::abs(a.x - c.x) < 0.01 && std::min(a.y, b.y) <= std::max(c.y, d.y) && std::min(c.y, d.y) <= std::max(a.y, b.y))) {
                        return true;
                    }
                }
                intersectionPoint = sf::Vector2f((cross(a, b) * cd.x - ab.x * cross(c, d)) / (ab.x * cd.y - ab.y * cd.x),
                                                 (cross(a, b) * cd.y - ab.y * cross(c, d)) / (ab.x * cd.y - ab.y * cd.x));
                if (std::min(a.x, b.x) - 0.01 <= intersectionPoint.x && intersectionPoint.x <= std::max(a.x, b.x) + 0.01 &&
                    std::min(a.y, b.y) - 0.01 <= intersectionPoint.y && intersectionPoint.y <= std::max(a.y, b.y) + 0.01 &&
                    std::min(c.x, d.x) - 0.01 <= intersectionPoint.x && intersectionPoint.x <= std::max(c.x, d.x) + 0.01 &&
                    std::min(c.y, d.y) - 0.01 <= intersectionPoint.y && intersectionPoint.y <= std::max(c.y, d.y) + 0.01) {
                    return true;
                }
            }
        }
        return false;
    }
};
