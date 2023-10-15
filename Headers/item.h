#pragma once
#include "interactible.h"

enum ItemID {
    gunParts,
    repairKit,
    keyCard,
    protectiveSuit,
    generic,
    medkit,
    regenDrug
};

std::map<ItemID, std::string> itemTextureName {
    {gunParts, "gunPartsItem"},
    {repairKit, "repairKitItem"},
    {keyCard, "keyCardItem"},
    {protectiveSuit, "protectiveSuitItem"},
    {regenDrug, "regenDrugItem"}
};

std::map<ItemID, std::string> pickupItemTextureName {
    {generic, "genericPickup"},
    {medkit, "medkitPickup"},
    {regenDrug, "regenDrugPickup"}
};

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
    
    sf::Texture texture, pickupTexture;

    Item() {}
    Item(ItemID, int, bool = true, bool = true, bool = true, bool = false, bool = false);

    void setPosition(sf::Vector2f);
    bool isActivated(Circle&, sf::Event&) override;
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
    animation->setTexture(texture);
    SetAnimation(texture, 1, static_cast<sf::Vector2f>(texture.getSize()), sf::seconds(1));
    setSize(static_cast<sf::Vector2f>(texture.getSize()));
}

void Item::setPosition(sf::Vector2f v) {
    PosX = v.x; PosY = v.y;
    animation->setPosition(v);
}

bool Item::CanBeActivated(Circle& player) {
    if (pickupable && !isInInventory)
        return intersect(player);
    return false;
}

bool Item::isActivated(Circle& player, sf::Event& event) {
    if (pickupable && !isInInventory)
        return intersect(player);
    return false;
}

void Item::dropTo(sf::Vector2f pos) {
    setPosition(pos);
    isInInventory = false;
}

void operator +=(Item& sumTo, Item& sumItem) {
    if (sumTo.id == sumItem.id) {
        sumTo.amount += sumItem.amount;
    }
}