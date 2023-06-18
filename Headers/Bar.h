#include "tools.h"

////////////////////////////////////////////////////////////
// Class
////////////////////////////////////////////////////////////

template <typename T>
class Bar {
public:
    sf::RectangleShape background, foreground, wall;
    Scale<T>* scale = nullptr;
    sf::Vector2f Position;
    sf::Vector2f Size;
    bool ShowWall = true, ShowBackground = true, ShowForeground = true;

    Bar() {}
    sf::Vector2f getSize() { return wall.getSize(); }
    void setSize(float x, float y) {
        wall.setSize({x, y});
        background.setSize({x - 10, y - 10});
        foreground.setSize({x - 10, y - 10});
    }
    sf::Vector2f getPosition() { return wall.getPosition(); }
    void setPosition(float x, float y) {
        wall.setPosition(x, y);
        background.setPosition(x + 5, y + 5);
        foreground.setPosition(x + 5, y + 5);
    }
    void setColors(sf::Color wallColor, sf::Color foregroundColor, sf::Color backgroundColor) {
        wall.setFillColor(wallColor);
        background.setFillColor(backgroundColor);
        foreground.setFillColor(foregroundColor);
    }
    void draw(sf::RenderWindow &window) {
        foreground.setScale(((scale) ?scale->filling() : 1), 1);
        
        if (ShowWall) window.draw(wall);
        if (ShowBackground) window.draw(background);
        if (ShowForeground) window.draw(foreground);
    }
};

////////////////////////////////////////////////////////////
// Realization
////////////////////////////////////////////////////////////
