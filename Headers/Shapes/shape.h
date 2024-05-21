#pragma once
#include <vector>
#include "VectorTools.h"

class Shape {
protected:
    std::vector<sf::Vector2f> points;
    size_t pointCount;
    virtual void updateShape() = 0;
public:

    Shape() : pointCount(0) {}
    Shape(size_t count) : pointCount(count) { points.resize(count); }
    Shape(std::vector<sf::Vector2f> points) : points(points), pointCount(points.size()) {}

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

    bool intersect(Shape& shape) {
        sf::Vector2f pos  =       getPosition(), size  =       getSize(),
                     pos2 = shape.getPosition(), size2 = shape.getSize();
        if (!(pos2.x <= pos.x + size.x && pos.x <= pos2.x + size2.x && pos2.y <= pos.y + size.y && pos.y <= pos2.y + size2.y)) {
            return false;
        }
        sf::Vector2f a, b, c, d, intersectionPoint;
        bool pointInside;
        for (int i = 0; i < pointCount; i++) {
            a = points[i]; b = points[(i + 1) % pointCount];
            pointInside = true;
            for (int j = 0; j < shape.pointCount; j++) {
                c = shape.points[j]; d = shape.points[(j + 1) % shape.pointCount];
                if (cross(points[(i + 1) % pointCount] - points[i], c - points[i]) < 0) {
                    pointInside = false;
                }
                if ((a.x - b.x) * (c.y - d.y) - (a.y - b.y) * (c.x - d.x) == 0) {
                    if (((a.y - b.y == 0) && std::abs(a.y - c.y) < 0.01 && std::min(a.x, b.x) <= std::max(c.x, d.x) && std::min(c.x, d.x) <= std::max(a.x, b.x)) ||
                       ((a.x - b.x == 0) && std::abs(a.x - c.x) < 0.01 && std::min(a.y, b.y) <= std::max(c.y, d.y) && std::min(c.y, d.y) <= std::max(a.y, b.y))) {
                        return true;
                    }
                }
                intersectionPoint = sf::Vector2f(
                (cross(a, b) * (c.x - d.x) - (a.x - b.x) * cross(c, d)) / ((a.x - b.x) * (c.y - d.y) - (a.y - b.y) * (c.x - d.x)),
                (cross(a, b) * (c.y - d.y) - (a.y - b.y) * cross(c, d)) / ((a.x - b.x) * (c.y - d.y) - (a.y - b.y) * (c.x - d.x)));
                if (std::min(a.x, b.x) - 0.01 <= intersectionPoint.x && intersectionPoint.x <= std::max(a.x, b.x) + 0.01 &&
                    std::min(a.y, b.y) - 0.01 <= intersectionPoint.y && intersectionPoint.y <= std::max(a.y, b.y) + 0.01 &&
                    std::min(c.x, d.x) - 0.01 <= intersectionPoint.x && intersectionPoint.x <= std::max(c.x, d.x) + 0.01 &&
                    std::min(c.y, d.y) - 0.01 <= intersectionPoint.y && intersectionPoint.y <= std::max(c.y, d.y) + 0.01) {
                    return true;
                }
            }
            if (pointInside) {
                return true;
            }
        }
        return false;
    }
};
