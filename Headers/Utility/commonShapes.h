#pragma once
#include <vector>
#include "VectorTools.h"
#include "../../SFML-2.5.1/include/SFML/System.hpp"

namespace CommonShapes {
    std::vector<sf::Vector2f> starShape{
        sf::Vector2f(0, 100),  sf::Vector2f(50, 50),   sf::Vector2f(100, 50),
        sf::Vector2f(75, 0),   sf::Vector2f(100, -50), sf::Vector2f(50, -50),
        sf::Vector2f(0, -100), sf::Vector2f(-50, -50), sf::Vector2f(-100, -50),
        sf::Vector2f(-75, 0),  sf::Vector2f(-100, 50), sf::Vector2f(-50, 50)
    };
    std::vector<sf::Vector2f> rectShape{
        sf::Vector2f(-100, -50), sf::Vector2f(-100, 50), sf::Vector2f(100, 50), sf::Vector2f(100, -50)
    };
    std::vector<sf::Vector2f> triangleShape{
        sf::Vector2f(-75, -75), sf::Vector2f(-75, 75), sf::Vector2f(75, -75)
    };
    std::vector<sf::Vector2f> frustumShape{
        sf::Vector2f(-75, -75), sf::Vector2f(-30, 75), sf::Vector2f(30, 75), sf::Vector2f(75, -75)
    };
}