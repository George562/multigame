#pragma once
#include "itemHelper.h"
#include "interactible.h"

////////////////////////////////////////////////////////////
// Class
////////////////////////////////////////////////////////////

class Item : public Interactible {
public:
    ItemID id;
    int amount = 0;
    bool pickupable = true;
    bool isDropable = true;
    bool isEquippable = false;
    bool isKeyItem = false;
    bool isInInventory = true;
    
    sf::Texture pickupTexture;

    Item() {}
    Item(ItemID, int, bool = true, bool = true, bool = true, bool = false, bool = false);
    Item operator +(Item);

    void setPosition(sf::Vector2f);
    bool isActivated(Circle&, sf::Event&, std::vector<sf::Drawable*>&) override;
    bool CanBeActivated(Circle&) override;
    void dropTo(sf::Vector2f);
};

////////////////////////////////////////////////////////////
// Realization
////////////////////////////////////////////////////////////

Item::Item(ItemID _id, int _amount, bool _pickapable, bool _isInInventory, bool _isDropable, bool _isKeyItem,
           bool _isEquippable) {
    id            = _id;
    amount        = _amount;
    pickupable    = _pickapable;
    isDropable    = _isDropable;
    isInInventory = _isInInventory;
    isEquippable  = _isEquippable;
    isKeyItem     = _isKeyItem;

    texture.loadFromFile("sources/textures/" + itemTextureName[id] + ".png");
    pickupTexture.loadFromFile("sources/textures/" + pickupItemTextureName[id] + ".png");
    sprite.setTexture(texture);
    setSize(sprite.getGlobalBounds().width, sprite.getGlobalBounds().height);
}

Item Item::operator +(Item sumItem) {
    if (id == sumItem.id && isKeyItem == sumItem.isKeyItem) {
        return Item(id, amount + sumItem.amount, isDropable, isEquippable, isKeyItem, isInInventory);
    }
    return *this;
}

void Item::setPosition(sf::Vector2f v) {
    PosX = v.x; PosY = v.y;
    sprite.setPosition(v);
}

bool Item::CanBeActivated(Circle& player) {
    if (pickupable && !isInInventory)
        return intersect(player);
    return false;
}

bool Item::isActivated(Circle& player, sf::Event& event, std::vector<sf::Drawable*>& InterfaceStuff) {
    if (pickupable && !isInInventory)
        return intersect(player);
    return false;
}

void Item::dropTo(sf::Vector2f pos) {
    setPosition(pos);
    isInInventory = false;
}

void operator +=(Item sumTo, Item sumItem) {
    if (sumTo.id == sumItem.id && sumTo.isKeyItem == sumItem.isKeyItem) {
        sumTo.amount += sumItem.amount;
    }
}