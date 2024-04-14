#pragma once
#include "inventory.h"
#include "player.h"

class Shop {
public:
    Inventory soldItems;
    std::vector<int> itemPrices;
    void (*buyFunction)(Player*);

    Shop() {
        itemPrices = std::vector<int>(ItemID::NONE, 0);
    }
    Shop(std::vector<Item*>* stock, std::vector<int> prices) : Shop() {
        for (int i = 0; i < stock->size(); i++) {
            soldItems.addItem((*stock)[i]);
            itemPrices[(*stock)[i]->id] = prices[i];
        }
    }

    void addItem(Item* newItem, int itemPrice) { soldItems.addItem(newItem); itemPrices[newItem->id] = itemPrice; }

    void setFunction(void (*func)(Player*)) { buyFunction = func; }
};