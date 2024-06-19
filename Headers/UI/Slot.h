#pragma once
#include "text.h"
#include "../../SFML-2.5.1/include/SFML/Graphics.hpp"

class ItemSlot {
public:
    sf::Sprite* background;
    PlacedText* amountText;
    bool isInitialized;  // A marker so that we don't have to use arbitrary checks for null pointers or something as dumb

    ItemSlot() {
        isInitialized = false;
    }

    virtual void init() {
        background = new sf::Sprite();
        amountText = new PlacedText();
        isInitialized = true;
    };
};

class ShopSlot : public ItemSlot {
public:
    PlacedText* priceText;

    ShopSlot() : ItemSlot() {}
    void init() {
        priceText = new PlacedText();
        ItemSlot::init();
    };
};