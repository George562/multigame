#pragma once
#include "../../SFML-2.5.1/include/SFML/Graphics.hpp"
#include "../Utility/tools.h"

namespace UI {
    enum Align {
        left,
        right,
        center
    };

    enum Fill {
        x,
        y,
        both,
        none
    };
}

class UIElement : public sf::Transformable, public sf::Drawable {
public:
    std::string name;
    UI::Align align = UI::center;
    UI::Fill fill = UI::none;
    bool visible = true;

    UIElement* parent;
    std::map<std::string, int> nameMap;
    std::vector<UIElement*> self;
    std::vector<UIElement*> children;

    virtual ~UIElement() {
        if (parent) {
            parent->children[parent->nameMap[this->name]] = nullptr;
            parent->nameMap.erase(this->name);
        }
        if (children.size() != 0) {
            for (UIElement*& elem : children)
                delete elem;
        }
        if (self.size() != 0) {
            for (UIElement*& elem : self)
                delete elem;
        }
    }

    virtual void addElement(UIElement* elem) {
        elem->parent = this;
        this->children.push_back(elem);
        this->nameMap[elem->name] = children.size() - 1;
    }

    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const {
        if (!visible)
            return;
        for (UIElement* elem : self) {
            if (elem->visible)
                target.draw(*elem, states);
        }
        for (UIElement* elem : children) {
            if (elem->visible)
                target.draw(*elem, states);
        }
    }

    virtual void move(float x, float y) {
        for (UIElement* elem : self)
            elem->move(x, y);
        for (UIElement* elem : children)
            elem->move(x, y);
    }
    void move(sf::Vector2f vec) { move(vec.x, vec.y); }
};