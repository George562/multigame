#pragma once
#include "../Utility/ColorTools.h"
#include "../../SFML-2.5.1/include/SFML/Graphics.hpp"
#include <string>

struct FontString {
    sf::String text = "";
    int charSize = 150;
    sf::Color fillColor = CommonColors::text;

    FontString(const char text[] = "", int charSize = 150, sf::Color fillColor = CommonColors::text) {
        if (text)
            this->text = text;
        this->charSize = charSize;
        this->fillColor = fillColor;
    }

    FontString(std::string text = "", int charSize = 150, sf::Color fillColor = CommonColors::text) {
        this->text = text;
        this->charSize = charSize;
        this->fillColor = fillColor;
    }

    FontString(sf::String text = "", int charSize = 150, sf::Color fillColor = CommonColors::text) {
        this->text = text.toAnsiString();
        this->charSize = charSize;
        this->fillColor = fillColor;
    }

    std::string substring(int start, int len = 0) { return text.substring(start, len); }

    operator std::string() { return text.toAnsiString(); }
    operator sf::String()  { return text; }

    FontString operator=(std::string s) const { return FontString(s); }
    FontString operator=(char* s) const { return FontString(std::string(s)); }
};