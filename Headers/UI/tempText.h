#pragma once
#include "PlacedText.h"

class TempText : public PlacedText {
public:
    sf::Time howLongToExist;
    sf::Clock* localClock = nullptr;

    TempText(sf::Time);
    ~TempText();
};

TempText::TempText(sf::Time howLongToExist) : PlacedText() {
    localClock = new sf::Clock();
    this->howLongToExist = howLongToExist;
}

TempText::~TempText() {
    if (localClock) {
        delete localClock;
    }
}
