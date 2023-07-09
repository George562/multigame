#include "panel.h"
#include "button.h"

////////////////////////////////////////////////////////////
// Class
////////////////////////////////////////////////////////////

class ContextMenu : public Panel {
public:
    std::vector<Button> elements;
    bool isMenuActive = false;

    ContextMenu() : Panel() {}
    ContextMenu(str, str, Rect);
    void addElement(str, str, void (*)(void), sf::Vector2f, sf::Vector2f);
    void removeElement(int index) { elements.erase(elements.begin() + index); }
    void draw(sf::RenderWindow&);
    void update(sf::Event&);
};

////////////////////////////////////////////////////////////
// Realization
////////////////////////////////////////////////////////////

ContextMenu::ContextMenu(str textureName, str name, Rect rect = Rect{0, 0, 600, 600}) : Panel(textureName, name) {
    elements.resize(0);
    setPosition(rect.PosX, rect.PosY);
    setSize(rect.Width, rect.Height);
}

void ContextMenu::addElement(str name, str text, void (*func)(void), sf::Vector2f elemPos, sf::Vector2f elemSize) {
    elements.push_back(*(new Button(name, text, func)));
    elements[elements.size() - 1].setPosition(elemPos.x, elemPos.y);
    elements[elements.size() - 1].setSize(elemSize);
}

void ContextMenu::draw(sf::RenderWindow& window) {
    if(isMenuActive) {
        window.draw(rect);
        for(int i = 0; i < elements.size(); i++)
            elements[i].draw(window);
    }
}

void ContextMenu::update(sf::Event& event) {
    if(isMenuActive)
        for(int i = 0; i < elements.size(); i++)
            elements[i].isActivated(event);
}