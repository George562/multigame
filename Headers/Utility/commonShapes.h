#pragma once
#include <vector>
#include "VectorTools.h"
#include "../../SFML-2.5.1/include/SFML/System.hpp"

namespace CommonShapes {
    std::vector<sf::Vector2f> starShape{
        sf::Vector2f(100, 200),  sf::Vector2f(150, 150),   sf::Vector2f(200, 150),
        sf::Vector2f(175, 100),   sf::Vector2f(200, 50), sf::Vector2f(150, 50),
        sf::Vector2f(100, 0), sf::Vector2f(50, 50), sf::Vector2f(0, 50),
        sf::Vector2f(25, 100),  sf::Vector2f(0, 150), sf::Vector2f(50, 150)
    };
    std::vector<sf::Vector2f> rectShape{
        sf::Vector2f(0, 0), sf::Vector2f(0, 100), sf::Vector2f(200, 100), sf::Vector2f(200, 0)
    };
    std::vector<sf::Vector2f> triangleShape{
        sf::Vector2f(0, 0), sf::Vector2f(0, 100), sf::Vector2f(100, 0)
    };
    std::vector<sf::Vector2f> frustumShape{
        sf::Vector2f(0, 0), sf::Vector2f(25, 150), sf::Vector2f(75, 150), sf::Vector2f(100, 0)
    };
}