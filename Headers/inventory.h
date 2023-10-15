#pragma once
#include "item.h"

class Inventory {
public:
    int slotAmount = -1; // -1 means unlimited (for enemies only).

    std::map<ItemID, Item*> dropableItems; // All items that can be dropped.
                                                     // Mostly materials from enemies or pickups.

    std::map<ItemID, Item*> safeItems; // Cannot be dropped by dying, but can be removed 
                                                 // if the player wishes to.

    std::map<ItemID, Item*> keyItems; // Items for quests, story progression etc. 
                                                // So it is mostly items that cannot be removed by any normal means.
                                                // Due to this, they will not occupy general inventory slots.

    std::map<ItemID, Item*> equipItems; // Stuff like armor, accessories etc.

    void addToDropable(Item* item) {
        if (!item->isDropable)
            return;
        if (!dropableItems[item->id]) {
            dropableItems[item->id] = new Item(*item);
        }
        *(dropableItems[item->id]) += *item;
    }

    void addToSafe(Item* item) {
        if (item->isDropable)
            return;
        if (!safeItems[item->id]) {
            safeItems[item->id] = new Item(*item);
        }
        *(safeItems[item->id]) += *item;
    }

    void addToKey(Item* item) {
        if (!item->isKeyItem)
            return;
        if (!keyItems[item->id]) {
            keyItems[item->id] = new Item(*item);
        }
        *(keyItems[item->id]) += *item;
    }

    void addToEquip(Item* item) {
        if (!item->isEquippable)
            return;
        if (!equipItems[item->id]) {
            equipItems[item->id] = new Item(*item);
        }
        *(equipItems[item->id]) += *item;
    }


    float money = 0;
};