#pragma once
#include "item.h"

////////////////////////////////////////////////////////////
// Class
////////////////////////////////////////////////////////////

class Inventory {
public:
    std::vector<Item*> items;
    int money = 0;

    int find(int id) {
        for (int i = 0; i < items.size(); i++)
            if (items[i]->id == id)
                return i;
        return items.size();
    }

    Item* find(Item*& item) {
        for (int i = 0; i < items.size(); i++)
            if (items[i]->id == item->id)
                return items[i];
        return nullptr;
    }

    void addItem(Item*& item) {
        for (int i = 0; i < items.size(); i++) {
            if (items[i]->id == item->id) {
                items[i]->amount += item->amount;
                return;
            }
        }
        items.push_back(new Item(*item));
        items[items.size() - 1]->picked();
        std::sort(items.begin(), items.end(), [](Item*& left, Item*& right) { return left->id < right->id; });
    }

    void removeItem(Item*& item) {
        for (int i = 0; i < items.size(); i++) {
            if (items[i]->id == item->id && items[i]->amount != -1) {
                items[i]->amount -= item->amount;
                if (items[i]->amount == 0) {
                    items.erase(items.begin() + i);
                }
                return;
            }
        }
    }
};

////////////////////////////////////////////////////////////
// Realization
////////////////////////////////////////////////////////////