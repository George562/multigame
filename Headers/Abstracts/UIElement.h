#pragma once
#include "../../SFML-2.5.1/include/SFML/Graphics.hpp"
#include "../CollisionShapes/collisionShape.h"
#include "../Utility/tools.h"
#include "../UI/UIRect.h"
#include "Margin.h"

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
        middLe
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
    switch (anchoringPoint)
    {
        case UI::T:
            return sf::Vector2f(-size.x / 2, 0);
            break;
        case UI::TR:
            return sf::Vector2f(-size.x, 0);
            break;
        case UI::BL:
            return sf::Vector2f(0, size.y);
            break;
        case UI::B:
            return sf::Vector2f(-size.x / 2, size.y);
            break;
        case UI::BR:
            return sf::Vector2f(-size.x, size.y);
            break;
        case UI::L:
            return sf::Vector2f(0, size.y / 2);
            break;
        case UI::R:
            return sf::Vector2f(-size.x, size.y / 2);
            break;
        case UI::center:
            return sf::Vector2f(-size.x / 2, size.y / 2);
        default:
            return sf::Vector2f(0, 0);
    }
}

sf::Vector2f getMarginShift(Margin thisMargin, Margin elemMargin, UI::Anchor anchor, UI::Anchor anchoringPoint) {
    sf::Vector2f res(0, 0);
    if (anchor % 3 == 0 && anchoringPoint % 3 == 1)
        res.x -= thisMargin.left - elemMargin.right;
    if (anchor % 3 == 1 && anchoringPoint % 3 == 0)
        res.x += thisMargin.right + elemMargin.left;
    if (anchor < 3 && 3 <= anchoringPoint && anchoringPoint < 6)
        res.y -= thisMargin.top - elemMargin.bottom;
    if (3 <= anchor && anchor < 6 && anchoringPoint < 3)
        res.y += thisMargin.bottom + elemMargin.bottom;
    return res;
}

sf::Vector2f getPaddingShift(Margin thisPadding, Margin elemMargin, UI::Anchor anchor, UI::Anchor anchoringPoint) {
    sf::Vector2f res(0, 0);
    if (anchor == UI::Anchor::L && anchoringPoint % 3 == 0)
        res.x += thisPadding.left + elemMargin.left;
    if (anchor == UI::Anchor::R && anchoringPoint % 3 == 1)
        res.x -= thisPadding.right - elemMargin.right;
    if (anchor == UI::Anchor::T && 0 <= anchoringPoint && anchoringPoint < 3)
        res.y += thisPadding.top + elemMargin.top;
    if (anchor == UI::Anchor::B && 3 <= anchoringPoint && anchoringPoint < 6)
        res.y -= thisPadding.bottom - elemMargin.bottom;
    if (anchor == anchoringPoint) {
        res.x += (thisPadding.left + elemMargin.left) * (anchor == UI::TL || anchor == UI::BL);
        res.x -= (thisPadding.right + elemMargin.right) * (anchor == UI::TR || anchor == UI::BR);
        res.y += (thisPadding.top + elemMargin.top) * (anchor == UI::TL || anchor == UI::TR);
        res.y -= (thisPadding.bottom + elemMargin.bottom) * (anchor == UI::BL || anchor == UI::BR);
    }
    return res;
}

bool isInside(UI::Anchor anchor, UI::Anchor anchoringPoint) {
    return (anchor == UI::Anchor::L && anchoringPoint % 3 == 0) ||
           (anchor == UI::Anchor::R && anchoringPoint % 3 == 1) ||
           (anchor == UI::Anchor::T && 0 <= anchoringPoint && anchoringPoint < 3) ||
           (anchor == UI::Anchor::B && 3 <= anchoringPoint && anchoringPoint < 6) ||
           (anchor == anchoringPoint);
}

class UIElement : public sf::Drawable, public UIRect {          ////////////////////////////////// WIP
private:
    void moveToAnchor(UIElement* elem) {
        if (elem->anchor == UI::none)
            return;
        sf::Vector2f posVec = UIRect::getPosition();

        if (elem->anchor == UI::Anchor::center) {
            posVec = getCenter();
        }

        posVec.x += getSize().x / 2 * (1 * (elem->anchor == UI::Anchor::T) + 2 * (elem->anchor == UI::Anchor::TR) +
                                       1 * (elem->anchor == UI::Anchor::B) + 2 * (elem->anchor == UI::Anchor::BR));
        posVec.y += getSize().y / 2 * (1 * (elem->anchor == UI::Anchor::L) + 2 * (elem->anchor == UI::Anchor::BL) +
                                       1 * (elem->anchor == UI::Anchor::R) + 2 * (elem->anchor == UI::Anchor::BR));

        posVec += getShiftByPoint(elem->getSize(), elem->anchoringPoint);

        if (isInside(elem->anchor, elem->anchoringPoint))
            posVec += getPaddingShift(this->padding, elem->margin, elem->anchor, elem->anchoringPoint);
        else
            posVec += getMarginShift(this->margin, elem->margin, elem->anchor, elem->anchoringPoint);
        elem->setPosition(posVec);
    }
protected:
    std::string name;
    UI::Anchor anchor = UI::Anchor::TL;
    UI::Anchor anchoringPoint = UI::Anchor::TL;

    CollisionShape hitbox;
    sf::Sprite sprite;
    sf::ConvexShape drawShape;

    Margin margin;    // How much free space to leave outside of this element
    Margin padding;   // How much free space to leave inside of this element
    bool visible = true;
    bool drawChildren = true;

    UIElement* parent = nullptr;
    std::map<std::string, int> nameMap;
    std::vector<UIElement*> children;
public:
    virtual ~UIElement() {}

    virtual void setPosition(float x, float y) {
        for (UIElement*& elem : this->children)
            elem->move(elem->UIRect::getPosition() - sf::Vector2f(x, y));
        UIRect::setPosition(x, y);
    }
    void setPosition(sf::Vector2f v) { setPosition(v.x, v.y); }
    void setName(std::string name) { this->name = name; }
    void setDrawChildren(bool drawChildren) { this->drawChildren = drawChildren; }
    void setAnchor(UI::Anchor anchor, bool update = false) { this->anchor = anchor; if (update && parent) parent->updateElement(this); }
    void setAnchoringPoint(UI::Anchor corner, bool update = false) { this->anchoringPoint = corner; if (update && parent) parent->updateElement(this); }
    void setAnchors(UI::Anchor anchor, UI::Anchor point, bool update = false) { setAnchor(anchor, update); setAnchoringPoint(point, update); }
    void setMargin(float l, float t, float r, float b) { margin = {l, t, r, b}; }
    void setPadding(float l, float t, float r, float b) { padding = {l, t, r, b}; }

    virtual void updateElement(UIElement* elem) { moveToAnchor(elem); }

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

    virtual void addElement(UIElement* elem, bool update = false) {
        elem->parent = this;
        this->children.push_back(elem);
        if (update) moveToAnchor(elem);
        this->nameMap[elem->name] = children.size() - 1;
    }
    void parentTo(UIElement* elem, bool update = false) { elem->addElement(this, update); }

    std::string getName() const { return name; }
    bool getDrawChildren() const { return drawChildren; }
    sf::Vector2f getHitboxPoint(int index) const { return hitbox.getPoint(index); }
    sf::Vector2f getDrawShapePoint(int index) const { return drawShape.getPoint(index); }
    std::vector<sf::Vector2f> getHitboxPoints() const { return hitbox.getPoints(); }
    std::vector<sf::Vector2f> getDrawShapePoints() const {
    std::vector<sf::Vector2f> output;
    for (int i = 0; i < drawShape.getPointCount(); i++)
        output.push_back(drawShape.getPoint(i));
    return output;
}

    void setTexture(sf::Texture& texture) { sprite.setTexture(texture); }
    void setSpritePos(float x, float y) { sprite.setPosition(x, y); }
    void setSpritePos(sf::Vector2f v) { setSpritePos(v); }
    void setSpriteScale(float x, float y) { sprite.setScale(x, y); }
    void setSpriteScale(sf::Vector2f v) { setScale(v); }
    void setSpriteRot(float angle) { sprite.setRotation(angle); }
    void setSpriteColor(sf::Color color) { sprite.setColor(color); }
    void setTextureRect(sf::IntRect r) { sprite.setTextureRect(r); }
    void setFillColor(sf::Color color) { drawShape.setFillColor(color); }
    void setOutlineColor(sf::Color color) { drawShape.setOutlineColor(color); }
    void setOutlineThickness(float thickness) { drawShape.setOutlineThickness(thickness); }

    void setHitboxPoints(std::vector<sf::Vector2f> points, bool updateDrawShape = false) {
        hitbox.setPoints(points);
        if (updateDrawShape)
            setDrawShapePoints(points);
    }

    void setDrawShapePoints(std::vector<sf::Vector2f> points) {
        drawShape.setPointCount(points.size());
        for (int i = 0; i < points.size(); i++)
            drawShape.setPoint(i, points[i]);
    }

    void show() { visible = true; }
    void hide() { visible = false; }
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const {
        std::string tabs = "";
        UIElement* parentIterator = this->parent;
        while (parentIterator) { tabs += "\t"; parentIterator = parentIterator->parent; }
        if (visible) {
            // DEBUG
            //if (coutClock->getElapsedTime() >= coutEvery) {
            //    std::cout << tabs << "Drawing UIName " << name << ".\n";
            //    std::cout << tabs << "\tInfo:\n";
            //    std::cout << tabs << "\t\tPos {" << this->getLeft() << "," << this->getTop() << "}\n";
            //    std::cout << tabs << "\t\tSize {" << this->getSize().x << "," << this->getSize().y << "}\n";
            //    std::cout << tabs << "\t\tIsVisible? {" << (visible ? "true" : "false")  << "}\n";
            //}
            target.draw(sprite);
            target.draw(drawShape);
        }
        if (drawChildren && children.size() != 0) {
            // DEBUG
            //if (coutClock->getElapsedTime() >= coutEvery)
            //    std::cout << tabs << "Drawing UIName " << name << "'s children\n";
            for (UIElement* elem : children)
                target.draw(*elem);
        }
    }

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