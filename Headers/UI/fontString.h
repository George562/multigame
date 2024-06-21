#pragma once
#include "../Utility/commonColors.h"
#include "../../SFML-2.5.1/include/SFML/Graphics.hpp"
#include <string>

struct FontString {
    sf::String text = "";
    int charSize = 150;
    sf::Color fillColor = CommonColors::text;

    FontString(sf::String text = "", int charSize = 150, sf::Color fillColor = CommonColors::text) {
        this->text = text;
        this->charSize = charSize;
        this->fillColor = fillColor;
    }
};