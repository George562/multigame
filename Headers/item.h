#pragma once
#include "itemHelper.h"
#include "interactible.h"

////////////////////////////////////////////////////////////
// Class
////////////////////////////////////////////////////////////

class Item : public Interactible {
public:
    ItemID id = none;
    PickupItemID pickupId = nonePickup;
    int amount = 0;
    bool isDropable = true;
    bool isEquippable = false;
    bool isKeyItem = false;

    bool isInInventory = true;

    Item() {}
    Item(ItemID, int, PickupItemID = nonePickup, bool = true, bool = true, bool = false, bool = false);
    Item operator +(Item);

    void setPosition(sf::Vector2f&);
    bool isActivated(Circle&, sf::Event&, std::vector<sf::Drawable*>&) override;
    bool CanBeActivated(Circle&) override;

    virtual void draw(sf::RenderTarget& target, sf::RenderStates states = sf::RenderStates::Default) const {
        target.draw(sprite, states);
    }
};

////////////////////////////////////////////////////////////
// Realization
////////////////////////////////////////////////////////////

Item::Item(ItemID _id, int _amount, PickupItemID _pickupId, bool _isInInventory, bool _isDropable, bool _isKeyItem,
           bool _isEquippable)
{
    id = _id;
    amount = _amount;
    pickupId = _pickupId;
    isDropable = _isDropable;
    isInInventory = _isInInventory;
    isEquippable = _isEquippable;
    isKeyItem = _isKeyItem;

    texture.loadFromFile("sources/textures/" + 
                         (isInInventory ? itemTextureName[id] : pickupItemTextureName[pickupId]) + ".png");
    sprite.setTexture(texture);
    setSize(sprite.getGlobalBounds().width, sprite.getGlobalBounds().height);
    sprite.setPosition(0, 0);
}

Item Item::operator +(Item sumItem)
{
    if(id == sumItem.id && pickupId == sumItem.pickupId && isKeyItem == sumItem.isKeyItem)
    {
        return Item(id, amount + sumItem.amount, pickupId, isDropable, isEquippable, isKeyItem, isInInventory);
    }
    if(id == none)
    {
        sumItem.isInInventory = true;
        return sumItem;
    }
    return *this;
}

void Item::setPosition(sf::Vector2f& v)
{
    PosX = v.x; PosY = v.y;
    sprite.setPosition(v);
}

bool Item::CanBeActivated(Circle& player)
{
    if(pickupId != nonePickup && !isInInventory)
        return intersect(player);
    return false;
}

bool Item::isActivated(Circle& player, sf::Event& event, std::vector<sf::Drawable*>& InterfaceStuff)
{
    if(pickupId != nonePickup && !isInInventory)
        return intersect(player);
    return false;
}