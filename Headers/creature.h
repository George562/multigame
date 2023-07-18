#include "weapon.h"

////////////////////////////////////////////////////////////
// Class
////////////////////////////////////////////////////////////

class Creature : public Rect, public sf::Drawable {
public:
    Scale<float> Health;
    Scale<float> Mana; // Used for everything
    float ManaRecovery;
    Scale<float> Armor; // blocking damage = min(1, damage - armor)
    float Money;
    Scale<sf::Vector2f> Velocity; // [bottom{x,y}, top{x,y}, cur{x,y}]
    float Acceleration;
    sf::Texture texture;
    sf::Sprite rect;
    float radius;
    Weapon *FirstWeapon, *SecondWeapon, *CurWeapon;
    sf::Time LastCheck;

    Creature() : Rect() {};
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states = sf::RenderStates::Default) const {
        target.draw(rect, states);
    };
    virtual void getDamage(float dmg) { Health -= dmg; }
    virtual void move(vvr&) {};
    virtual void update() {};
    virtual void update(sf::Event&, bool&) {};
    void SetTexture(str name) {
        texture.loadFromFile(name + ".png");
        texture.setSmooth(true);
        rect.setTexture(texture);
        if (rect.getGlobalBounds().width != Width)
            rect.setScale(Width / rect.getGlobalBounds().width, Height / rect.getGlobalBounds().height);
    };
    void ChangeWeapon(Weapon* to) {
        CurWeapon = to;
    }
};

////////////////////////////////////////////////////////////
// Realization
////////////////////////////////////////////////////////////
