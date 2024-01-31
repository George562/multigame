#pragma once
#include "item.h"

////////////////////////////////////////////////////////////
// Class
////////////////////////////////////////////////////////////

class Inventory {
public:
    std::map<ItemID::Type, Item*> items; // All items that can be dropped.

    void addItem(Item* item) {
        if (!items[item->id]) {
            items[item->id] = new Item(*item);
            items[item->id]->picked();
        } else {
            *(items[item->id]) += *item;
        }
        item->isInInventory = true;
    }
};

////////////////////////////////////////////////////////////
// Realization
////////////////////////////////////////////////////////////