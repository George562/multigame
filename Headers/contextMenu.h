#pragma once
#include "button.h"
#include "panel.h"

////////////////////////////////////////////////////////////
// Class
////////////////////////////////////////////////////////////

class ContextMenu : public Panel {
public:
    std::vector<Button> elements;
    bool isMenuActive = false;

    ContextMenu() : Panel() {}
    ContextMenu(str, Rect);
    void addElement(sf::Texture&, sf::Texture&, str, void (*)(void), sf::Vector2f, sf::Vector2f);
    void removeElement(int index) { elements.erase(elements.begin() + index); }
    virtual void draw(sf::RenderTarget&, sf::RenderStates = sf::RenderStates::Default) const;
    void update(sf::Event&);
};

////////////////////////////////////////////////////////////
// Realization
////////////////////////////////////////////////////////////

ContextMenu::ContextMenu(str name, Rect rect = Rect{0, 0, 600, 600}) : Panel(name) {
    elements.resize(0);
    setPosition(rect.PosX, rect.PosY);
    setSize(rect.Width, rect.Height);
}

void ContextMenu::addElement(sf::Texture& texture, sf::Texture& pushedTexture, str text, void (*func)(void), sf::Vector2f elemPos, sf::Vector2f elemSize) {
    elements.push_back(*(new Button(text, func)));
    elements[elements.size() - 1].setTexture(texture, pushedTexture);
    elements[elements.size() - 1].setPosition(elemPos.x, elemPos.y);
    elements[elements.size() - 1].setSize(elemSize);
}

void ContextMenu::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    if (isMenuActive) {
        target.draw(sprite, states);
        for (int i = 0; i < elements.size(); i++)
            target.draw(elements[i], states);
    }
}

void ContextMenu::update(sf::Event& event) {
    if (isMenuActive)
        for (int i = 0; i < elements.size(); i++)
            elements[i].isActivated(event);
}