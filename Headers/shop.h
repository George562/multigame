#pragma once
#include "inventory.h"

class Shop {
public:
    Inventory soldItems;
    std::vector<int> itemPrices;
    void (*buyFunction)();

    Shop() {
        itemPrices.assign(ItemID::NONE, 0);
    }
    Shop(std::vector<Item*>* stock, std::vector<int> prices) : Shop() {
        for (int i = 0; i < stock->size(); i++) {
            soldItems.addItem((*stock)[i]);
            itemPrices[(*stock)[i]->id] = prices[i];
        }
    }
    
    void setShop(std::vector<Item*>* stock, std::vector<int> prices) {
        for (int i = 0; i < stock->size(); i++) {
            soldItems.addItem((*stock)[i]);
            itemPrices[(*stock)[i]->id] = prices[i];
        }
    }

    void addItem(Item* newItem, int itemPrice) { soldItems.addItem(newItem); itemPrices[newItem->id] = itemPrice; }

    void setFunction(void (*func)()) { buyFunction = func; }
};