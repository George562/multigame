#pragma once
#include "item.h"

////////////////////////////////////////////////////////////
// Class
////////////////////////////////////////////////////////////

class Inventory {
public:
    float money = 0;
    int slotAmount = -1; // -1 means unlimited (for enemies only).

    std::map<ItemID::Type, Item*> dropableItems; // All items that can be dropped.
                                           // Mostly materials from enemies or pickups.

    std::map<ItemID::Type, Item*> safeItems; // Cannot be dropped by dying, but can be removed
                                       // if the player wishes to.

    std::map<ItemID::Type, Item*> keyItems; // Items for quests, story progression etc.
                                      // So it is mostly items that cannot be removed by any normal means.
                                      // Due to this, they will not occupy general inventory slots.

    std::map<ItemID::Type, Item*> equipItems; // Stuff like armor, accessories etc.

    void addItem(Item* item) {
        if      (!addToDropable(item)) {}
        else if (!addToEquip   (item)) {}
        else if (!addToKey     (item)) {}
        else if (!addToSafe    (item)) {}
        item->isInInventory = true;
    }

    bool addToDropable(Item* item) {
        if (!item->isDropable)
            return false;
        if (!dropableItems[item->id]) {
            dropableItems[item->id] = new Item(*item);
        } else {
            *(dropableItems[item->id]) += *item;
        }
        return true;
    }

    bool addToSafe(Item* item) {
        if (item->isDropable)
            return false;
        if (!safeItems[item->id]) {
            safeItems[item->id] = new Item(*item);
        } else {
            *(safeItems[item->id]) += *item;
        }
        return true;
    }

    bool addToKey(Item* item) {
        if (!item->isKeyItem)
            return false;
        if (!keyItems[item->id]) {
            keyItems[item->id] = new Item(*item);
        } else {
            *(keyItems[item->id]) += *item;
        }
        return true;
    }

    bool addToEquip(Item* item) {
        if (!item->isEquippable)
            return false;
        if (!equipItems[item->id]) {
            equipItems[item->id] = new Item(*item);
        } else {
            *(equipItems[item->id]) += *item;
        }
        return true;
    }

    int itemAmount() {
        return dropableItems.size() + safeItems.size() + keyItems.size() + equipItems.size();
    }
};

////////////////////////////////////////////////////////////
// Realization
////////////////////////////////////////////////////////////