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
    The class below tries to emulate HTML by using paddings and margins to shift elements, but it also
    takes some inspiration from anchors and packs from tkInter. It also combines all
    related UIElements into a tree hierarchy for easy manipulations on groups of UI elements.

    Anchor locations:
                    UL----------U----------UR
                    |                       |
                    |                       |
                    L         Center        R
                    |                       |
                    |                       |
                    BL----------B-----------BR

    Anchor is a point, where the added element wants to be put. As default - it cares only about it's upper-left corner.
    anchoringPoint - a parameter that will put the corresponding corner on the anchor. For example:
    Anchor = BL, anchoringPoint = BL:
                    UL----------U----------UR
                    |                       |
                    /------\                |
                    |      |  Center        R
                    |      |                |
                    |      |                |
                    \------/---------------BR

    Anchor = UR, anchoringPoint = BL:
                                           /-------\
                                           |       |
                                           |       |
                                           |       |
                    UL----------U----------\-------/
                    |                       |
                    |                       |
                    L         Center        R
                    |                       |
                    |                       |
                    BL----------B-----------BR
    
    Anchor = B, anchoringPoint = UL:
                    UL----------U----------UR
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

    Spacing it out is handLed by the element's margin.

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

class UIElement : public sf::Drawable, public UIRect {          ////////////////////////////////// WIP
private:
    void moveToAnchor(UIElement* elem, sf::Vector2f offset = {0, 0}) { // why this function is inside of class?
        if (elem->anchor == UI::none)
            return;
        sf::Vector2f posVec = UIRect::getPosition();
        if (elem->anchor == UI::Anchor::center) {
            posVec = getCenter();
        }
        else posVec -= getShiftByPoint(sf::Vector2f(getGlobalBounds().getSize()), elem->anchor);
        posVec += getShiftByPoint(sf::Vector2f(elem->getGlobalBounds().getSize()), elem->anchoringPoint);
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

    virtual void updateElement(UIElement* elem) { moveToAnchor(elem); }
    virtual void updateChildren() {
        for (UIElement*& child : children)
            moveToAnchor(child);
    }

    virtual void centerOnAnchor(std::string name, UI::Centering mode) {
        UIElement* child = children[nameMap[name]];
        bool xCentered = child->getCenter().x == this->getCenter().x;
        if (mode == UI::Centering::x && !xCentered) {
            child->move(this->getCenter().x - child->getCenter().x, 0);
            return;
        }
        bool yCentered = children[nameMap[name]]->getCenter().y == this->getCenter().y;
        if (mode == UI::Centering::y && !yCentered) {
            child->move(this->getCenter().x - child->getCenter().x, 0);
            return;
        }
        if (!xCentered || !yCentered) {
            child->move(this->getCenter().x - child->getCenter().x, this->getCenter().y - child->getCenter().y);
            return;
        }
    }

    virtual void addElement(UIElement* elem, bool update = false, sf::Vector2f offset = {0, 0}) {
        elem->parent = this;
        this->children.push_back(elem);
        if (update) moveToAnchor(elem, offset);
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