#pragma once
#include "../../SFML-2.5.1/include/SFML/Graphics.hpp"
#include "../Utility/tools.h"
#include "../UI/UIRect.h"

namespace UI {
    enum Anchor {
        TL,
        T,
        TR,
        BL,
        B,
        BR,
        L,
        R,
        center,
        none
    };

    enum Centering {
        x,
        y,
        middle
    };

    enum TextureResize {
        element,
        texture,
        size
    };

        /*
    The class below takes some inspiration from anchors and packs from tkInter. It also combines all
    related UIElements into a tree hierarchy for easy manipulations on groups of UI elements.

    Anchor locations:
                    TL----------T----------TR
                    |                       |
                    |                       |
                    L         Center        R
                    |                       |
                    |                       |
                    BL----------B-----------BR

    Anchor is a point, where the added element wants to be put. As default - it cares only about it's upper-left corner.
    anchoringPoint - a parameter that will put the corresponding corner on the anchor. For example:
    Anchor = BL, anchoringPoint = BL:
                    TL----------T----------TR
                    |                       |
                    /------\                |
                    |      |  Center        R
                    |      |                |
                    |      |                |
                    \------/---------------BR

    Anchor = TR, anchoringPoint = BL:
                                           /-------\
                                           |       |
                                           |       |
                                           |       |
                    TL----------T----------\-------/
                    |                       |
                    |                       |
                    L         Center        R
                    |                       |
                    |                       |
                    BL----------B-----------BR

    Anchor = B, anchoringPoint = TL:
                    TL----------T----------TR
                    |                       |
                    |                       |
                    L         Center        R
                    |                       |
                    |                       |
                    BL----------/-------\---BR
                                |       |
                                |       |
                                |       |
                                \-------/

    Making an anchor none means it will not be aligned with the parent container.
    Useful for placing the element on specific coordinates.

    Spacing it out is handLed by the offset in moveAnchor.

    The centering enum is used for centering whenever the user calls the CenterOnAnchor function.
    It centers the chosen coordinate. It will not do anything if the element is already centered.

    All in all this class is mostly for ease of initialization. All internal elements of UIElement's inheritants are handled by them
    and will not get use of this class' methods and automatic alligning. It is either unneeded due to rarity
    or it will be plain too difficult to realize.
    If the programmer wishes to align internal elements of a UIElement - they will have to make use of SFML's functions:
    free, unanchored positioning with something like setPosition() or move().
    */
}

sf::Vector2f getShiftByPoint(sf::Vector2f size, UI::Anchor anchoringPoint) {
    sf::Vector2f shift(0, 0);
    if (anchoringPoint == UI::center)
        return { -size.x / 2, -size.y / 2 };
    if (UI::BL <= anchoringPoint && anchoringPoint <= UI::BR)
        shift.y -= size.y;
    if (anchoringPoint == UI::L || anchoringPoint == UI::R)
        shift.y -= size.y / 2;
    if (anchoringPoint == UI::TR || anchoringPoint == UI::BR || anchoringPoint == UI::R)
        shift.x -= size.x;
    if (anchoringPoint == UI::T || anchoringPoint == UI::B)
        shift.x -= size.x / 2;
    return shift;
}

class UIElement : public sf::Drawable, public UIRect {
private:
    void anchorChildToParent(UIElement* elem, UI::Anchor anchor, UI::Anchor anchoringPoint,
                             sf::Vector2f offset = { 0, 0 }) {
        if (anchor == UI::none)
            return;
        sf::Vector2f posVec = UIRect::getPosition();
        if (anchor == UI::Anchor::center) posVec = { getCenter().x * getScale().x,
                                                     getCenter().y * getScale().y };
        else posVec -= getShiftByPoint(getGlobalBounds().getSize(), anchor);
        posVec += getShiftByPoint(elem->getGlobalBounds().getSize(), anchoringPoint);
        elem->setPosition(posVec + offset);
    }
protected:
    std::string name;
    UI::Anchor anchor = UI::Anchor::TL;
    UI::Anchor anchoringPoint = UI::Anchor::TL;

    UIElement* parent = nullptr;
    std::map<std::string, int> nameMap;
    std::vector<UIElement*> children;
public:
    virtual ~UIElement() {
        for (UIElement*& elem : children)
            elem->parent = nullptr;
    }

    virtual sf::FloatRect getGlobalBounds() { return UIRect::getGlobalBounds(); }

    virtual void setPosition(float x, float y) {
        for (UIElement*& elem : this->children)
            elem->move(elem->UIRect::getPosition() - sf::Vector2f(x, y));
        UIRect::setPosition(x, y);
    }
    void setPosition(sf::Vector2f v) { setPosition(v.x, v.y); }

    void setName(std::string name) { this->name = name; }
    void setAnchor(UI::Anchor anchor, bool update = false) { this->anchor = anchor; if (update && parent) parent->updateElement(this); }
    void setAnchoringPoint(UI::Anchor corner, bool update = false) { this->anchoringPoint = corner; if (update && parent) parent->updateElement(this); }
    void setAnchors(UI::Anchor anchor, UI::Anchor point, bool update = false) { setAnchor(anchor, update); setAnchoringPoint(point, update); }

    void updateAnchor(sf::Vector2f offset = { 0, 0 }) {
        this->parent->anchorChildToParent(this, this->anchor, this->anchoringPoint, offset);
    }

    void moveToAnchor(UIElement* parentElem, sf::Vector2f offset = { 0, 0 }) {
        parentElem->anchorChildToParent(this, this->anchor, this->anchoringPoint, offset);
    }

    void moveToAnchor(UIElement* parentElem, UI::Anchor anchor, UI::Anchor anchoringPoint,
                      sf::Vector2f offset = { 0, 0 }) {
        parentElem->anchorChildToParent(this, anchor, anchoringPoint, offset);
    }

    virtual void updateElement(UIElement* elem) { moveToAnchor(elem); }
    virtual void updateChildren() {
        for (UIElement*& child : children)
            moveToAnchor(child);
    }

    virtual void centerOnAnchor(UIElement* elem, UI::Centering mode) {
        bool xCentered = elem->getCenter().x == getCenter().x;
        if (mode == UI::Centering::x && !xCentered) {
            move(elem->getCenter().x - getCenter().x, 0);
            return;
        }
        bool yCentered = elem->getCenter().y == getCenter().y;
        if (mode == UI::Centering::y && !yCentered) {
            move(elem->getCenter().x - getCenter().x, 0);
            return;
        }
        if (!xCentered || !yCentered) {
            move(elem->getCenter().x - getCenter().x, elem->getCenter().y - getCenter().y);
            return;
        }
    }

    virtual void addElement(UIElement* elem, bool update = false, sf::Vector2f offset = {0, 0}) {
        elem->parent = this;
        this->children.push_back(elem);
        if (update) anchorChildToParent(elem, elem->anchor, elem->anchoringPoint, offset);
        this->nameMap[elem->name] = children.size() - 1;
    }
    virtual void parentTo(UIElement* elem, bool update = false, sf::Vector2f offset = {0, 0}) {
        elem->addElement(this, update, offset);
    }

    std::string getName() const { return name; }

    virtual void move(float x, float y) {
        setPosition(getPosition() + sf::Vector2f(x, y));
        for (UIElement* elem : children)
            elem->move(x, y);
    }
    void move(sf::Vector2f vec) { move(vec.x, vec.y); }

    std::vector<UIElement*>& getChildren() { return children; }

    UIElement* operator[](std::string name) { 
        if (nameMap.find(name) != nameMap.end())
            return children[nameMap[name]];
        else return nullptr;
    }
};