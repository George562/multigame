#pragma once
#include "../../SFML-2.5.1/include/SFML/Graphics.hpp"
#include "../../SFML-2.5.1/include/SFML/System.hpp"
#include <vector>
#include <cmath>

struct Shape : public sf::Shape {    
    // Check if a point is inside the rectangle's area
    virtual bool contains(float x, float y) const {}
    bool contains(sf::Vector2f& point) const { return contains(point.x, point.y); }

    // Check the intersection between two shapes
    virtual bool intersect(Shape& shape) {}
};
    