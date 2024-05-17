#include "rect.h"
#include "circle.h"
#include <iostream>

int main () {
    Rect rect(sf::Vector2f(2, 2), sf::Vector2f(1, 2));
    Circle circle(sf::Vector2f(3, 1), 1.5);
    std::cout << rect.intersect(circle) << '\n';
    std::cout << circle.contains(rect.getPosition()) << '\n';
    std::cout << circle.contains(rect.getRightTop()) << '\n';
    std::cout << circle.contains(rect.getRightBottom()) << '\n';
    std::cout << circle.contains(rect.getLeftBottom()) << '\n';
    return 0;
}