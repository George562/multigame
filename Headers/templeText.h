#pragma once
#include "text.h"

class TempleText : public PlacedText {
public:
    sf::Time howLongToExist;
    sf::Clock* localClock = nullptr;

    TempleText(sf::Time);
    ~TempleText();
};

TempleText::TempleText(sf::Time howLongToExist) : PlacedText() {
    localClock = new sf::Clock();
    this->howLongToExist = howLongToExist;
}

TempleText::~TempleText() {
    if (localClock) {
        delete localClock;
    }
}
