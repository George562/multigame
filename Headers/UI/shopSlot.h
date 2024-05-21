#pragma once
#include "text.h"
#include "../../SFML-2.5.1/include/SFML/Graphics.hpp"

struct ShopSlot {
    sf::Sprite* background;
    PlacedText* amountText;
    PlacedText* priceText;
    bool isInitialized;  // A marker so that we don't have to use arbitrary checks for null pointers or something as dumb

    ShopSlot() {
        isInitialized = false;
    }
};