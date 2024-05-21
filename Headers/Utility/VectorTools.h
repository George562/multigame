#pragma once
#include "../../SFML-2.5.1/include/SFML/System.hpp"
#include <cmath>

float cross(sf::Vector2f a, sf::Vector2f b) {
    return a.x * b.y - a.y * b.x;
}

float distance(sf::Vector2f a, sf::Vector2f b) {
    return std::hypot(a.x - b.x, a.y - b.y);
}

float LenOfVector(sf::Vector2f v) {
    return std::hypot(v.x, v.y);
}