#include "interactible.h"
#include "itemHelper.h"

////////////////////////////////////////////////////////////
// Class
////////////////////////////////////////////////////////////

class PickupItem : public Interactible {
public:
    PickupItem(PickupItemType);
    PickupItemType type = none;
    int quantity;

    bool CanBeActivated(Circle&);
    bool isActivated(Circle&, sf::Event&, std::vector<sf::Drawable*>&);

    virtual void draw(sf::RenderTarget& target, sf::RenderStates states = sf::RenderStates::Default) const {
        target.draw(sprite, states);
    }
};

////////////////////////////////////////////////////////////
// Realization
////////////////////////////////////////////////////////////

PickupItem::PickupItem(PickupItemType _type)
{
    texture.loadFromFile(pickupItemTextureName[_type]);
    sprite.setTexture(texture);
    setSize(sprite.getGlobalBounds().width, sprite.getGlobalBounds().height);
    sprite.setPosition(PosX, PosY);
    
    type = _type;
}

// Item activates on intersection with the Player, so both canBeActivated and isActivated functions
// have the same realization. The game itself will decide what kind of item should be added and how much
// based on this class' parameters
bool PickupItem::CanBeActivated(Circle& player)
{
    return intersect(player);
}

bool PickupItem::isActivated(Circle& player, sf::Event& event, std::vector<sf::Drawable*>& InterfaceStuff)
{
    return intersect(player);
}