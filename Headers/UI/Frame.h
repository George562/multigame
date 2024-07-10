#pragma once
#include "../Abstracts/UIElement.h"

/*
Frame - the most basic UIElement. It is a combination of a sprite, a hitbox and a drawShape (outline or full SFML shape)
*/

class Frame : public UIElement {
public:
    Frame() {}

    Frame(std::string name) {
        setName(name);
    }

    Frame(std::string name, UI::Anchor anchor, UI::Anchor anchoringPoint, int h = 0, int w = 0) {
        setName(name);
        setAnchors(anchor, anchoringPoint);
        setRect(0, 0, h, w);
    }

    Frame(std::string name, int x, int y, int h, int w) {
        setName(name);
        setAnchors(UI::none, UI::none);
        setRect(x, y, h, w);
    }
};