#pragma once
#include <unordered_map>
#include "item.h"

class Inventory
{
public:
    int slotAmount = -1; // -1 means unlimited (for enemies only).

    std::unordered_map<ItemID, Item*> dropableItems; // All items that can be dropped.
                                                     // Mostly materials from enemies or pickups.

    std::unordered_map<ItemID, Item*> safeItems; // Cannot be dropped by dying, but can be removed 
                                                 // if the player wishes to.

    std::unordered_map<ItemID, Item*> keyItems; // Items for quests, story progression etc. 
                                                // So it is mostly items that cannot be removed by any normal means.
                                                // Due to this, they will not occupy general inventory slots.

    std::unordered_map<ItemID, Item*> equipItems; // Stuff like armor, accessories etc.

    void addToDropable(Item* item) {
        if (!item->isDropable)
            return;
        if (dropableItems[item->id]) {
            *(dropableItems[item->id]) += *item;
        } else {
            dropableItems[item->id] = item;
        }
    }

    void addToSafe(Item* item) {
        if (item->isDropable)
            return;
        if (dropableItems[item->id]) {
            *(dropableItems[item->id]) += *item;
        } else {
            dropableItems[item->id] = item;
        }
    }

    void addToKey(Item* item) {
        if (!item->isKeyItem)
            return;
        if (dropableItems[item->id]) {
            *(dropableItems[item->id]) += *item;
        } else {
            dropableItems[item->id] = item;
        }
    }

    void addToEquip(Item* item) {
        if (!item->isEquippable)
            return;
        if (dropableItems[item->id]) {
            *(dropableItems[item->id]) += *item;
        } else {
            dropableItems[item->id] = item;
        }
    }


    float money = 0;
};