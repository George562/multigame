#include "interactible.h"
#include "button.h"

////////////////////////////////////////////////////////////
// Class
////////////////////////////////////////////////////////////

class Portal : public Interactible {
public:
    Portal();
    bool IsPlayerOnPortal = false;
    void setFunction(void (*func)(void)) { function = func; }
    bool CanBeActivated(Circle&);
    bool isActivated(Circle&, sf::Event&, std::vector<sf::Drawable*>&);

    void setPosition(float x, float y) { PosX = x; PosY = y; sprite.setPosition(PosX, PosY); }
    void setPosition(sf::Vector2f v) { setPosition(v.x, v.y); }

    void setCenter(float x, float y) { setPosition(x - Width / 2, y - Height / 2); }
    void setCenter(sf::Vector2f v) { setCenter(v.x, v.y); }

    virtual void draw(sf::RenderTarget& target, sf::RenderStates states = sf::RenderStates::Default) const {
        target.draw(sprite, states);
    }
};

////////////////////////////////////////////////////////////
// Realization
////////////////////////////////////////////////////////////

Portal::Portal() {
    texture.loadFromFile("sources/textures/Portal.png");
    sprite.setTexture(texture);
    setSize(sprite.getGlobalBounds().width, sprite.getGlobalBounds().height);
    sprite.setPosition(PosX, PosY);
}

bool Portal::CanBeActivated(Circle& rect) { return intersect(rect); }

bool Portal::isActivated(Circle& rect, sf::Event& event, std::vector<sf::Drawable*>& InterfaceStuff) {
    if (event.type == sf::Event::KeyPressed && event.key.code == ActivationButton) {
        function();
        return true;
    }
    return false;
}