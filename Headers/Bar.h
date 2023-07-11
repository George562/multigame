#include "init.h"

////////////////////////////////////////////////////////////
// Class
////////////////////////////////////////////////////////////

template <typename T>
class Bar : public sf::Drawable {
public:
    sf::RectangleShape background, foreground, wall;
    Scale<T>* value = nullptr;
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

    void Update() { foreground.setScale(((value != nullptr) ? value->filling() : 1), 1); }
    
    void draw(sf::RenderTarget& target, sf::RenderStates states = sf::RenderStates::Default) const {
        if (ShowWall) target.draw(wall);
        if (ShowBackground) target.draw(background);
        if (ShowForeground) target.draw(foreground);
    }
};

////////////////////////////////////////////////////////////
// Realization
////////////////////////////////////////////////////////////
