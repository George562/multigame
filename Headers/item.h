#pragma once
#include "interactible.h"

enum ItemID : sf::Uint8 {
    gunParts,
    repairKit,
    keyCard,
    protectiveSuit,
    generic,
    medkit,
    regenDrug
};

//////////////////////////////////////////////////////////// Item Tetures
std::map<ItemID, sf::Texture*> itemTextureName {
    {gunParts, new sf::Texture},
    {repairKit, new sf::Texture},
    {keyCard, new sf::Texture},
    {protectiveSuit, new sf::Texture},
    {regenDrug, new sf::Texture}
};

std::map<ItemID, sf::Texture*> pickupItemTextureName {
    {generic, new sf::Texture},
    {medkit, new sf::Texture},
    {regenDrug, new sf::Texture}
};

void loadItemTextures() {
    // itemTextureName[gunParts]       ->loadFromFile("sources/textures/gunPartsItem.png");
    // itemTextureName[repairKit]      ->loadFromFile("sources/textures/repairKitItem.png");
    // itemTextureName[keyCard]        ->loadFromFile("sources/textures/keyCardItem.png");
    // itemTextureName[protectiveSuit] ->loadFromFile("sources/textures/protectiveSuitItem.png");
    itemTextureName[regenDrug]      ->loadFromFile("sources/textures/regenDrugItem.png");

    // pickupItemTextureName[generic]   ->loadFromFile("sources/textures/genericPickup.png");
    // pickupItemTextureName[medkit]    ->loadFromFile("sources/textures/medkitPickup.png");
    pickupItemTextureName[regenDrug] ->loadFromFile("sources/textures/regenDrugPickup.png");
}

////////////////////////////////////////////////////////////
// Class
////////////////////////////////////////////////////////////

#pragma pack(push, 1)
class Item : public Interactable {
public:
    ItemID id;
    int amount = 0;
    bool pickupable = true;
    bool isDropable = true;
    bool isEquippable = false;
    bool isKeyItem = false;
    bool isInInventory = true;

    Item(ItemID, int, bool = true, bool = true, bool = true, bool = false, bool = false);

    void setPosition(sf::Vector2f);
    bool isActivated(Circle&, sf::Event&) override;
    bool CanBeActivated(Circle&) override;
    void dropTo(sf::Vector2f);
};
#pragma pack(pop)

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

    setAnimation(*itemTextureName[id], 1, 1, sf::seconds(1), &MapShader);
    setSize(animation->getGlobalSize());
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
    setAnimation(*pickupItemTextureName[id], 1, 1, sf::seconds(1), &MapShader);
    setPosition(pos);
    isInInventory = false;
}

void operator +=(Item& sumTo, Item& sumItem) {
    if (sumTo.id == sumItem.id) {
        sumTo.amount += sumItem.amount;
    }
}