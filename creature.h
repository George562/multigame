#include "weapon.h"

////////////////////////////////////////////////////////////
// Class
////////////////////////////////////////////////////////////

class Creature : public Rect {
public:
    Scale<float> Health; // if less than 0, creature will die
    Scale<float> Mana; // Used for everything
    float ManaRecovery;
    Scale<float> Armor; // blocking damage = min(1, damage - armor)
    float Money; // you can buy something with this
    Scale<sf::Vector2f> Velocity;
    float Acceleration;
    Point direction;
    sf::Clock* Clock;
    sf::Vector2f* Camera;
    sf::Texture texture;
    sf::Sprite rect;
    sf::CircleShape circle;
    float radius;
    Weapon *FirstWeapon, *SecondWeapon, *CurWeapon;
    sf::Time LastCheck;

    Creature() {};
    virtual void draw(sf::RenderWindow&, sf::Vector2f*) {};
    virtual void getDamage(float dmg) { Health -= dmg; }
    virtual void move(const vvr&, const int&, const int&) {}; // walls, size of walls, miniSize of walls
    virtual void update(vB&, sf::Event&) {};
    void SetTexture(str name) {
        texture.loadFromFile(name + ".png");
        texture.setSmooth(true);
        rect.setTexture(texture);
        if (rect.getGlobalBounds().width != Width)
            rect.setScale(Width / rect.getGlobalBounds().width, Height / rect.getGlobalBounds().height);
    };
};

////////////////////////////////////////////////////////////
// Realization
////////////////////////////////////////////////////////////
