#pragma once
#include "text.h"
#include "../../SFML-2.5.1/include/SFML/Graphics.hpp"

struct ItemSlot {
    sf::Sprite* background;
    PlacedText* amountText;
    bool isInitialized;  // A marker so that we don't have to use arbitrary checks for null pointers or something as dumb

    ItemSlot() {
        isInitialized = false;
    }
};