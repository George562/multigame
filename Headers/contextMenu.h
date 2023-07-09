#include "panel.h"
#include "button.h"

////////////////////////////////////////////////////////////
// Class
////////////////////////////////////////////////////////////

class ContextMenu : public Panel {
public:
    std::vector<Panel> *elements;
    bool isInterfaceDrawn = false;

    ContextMenu() : Panel() {};
    ContextMenu(str, str, std::vector<Panel>*, sf::Vector2f, float, float);
    void addElement(Panel* elem);
    void removeElement(int index) { elements->erase(elements->begin() + index); }
    void draw(sf::RenderWindow&);
    bool isActive(sf::Event&);
};

////////////////////////////////////////////////////////////
// Realization
////////////////////////////////////////////////////////////

ContextMenu::ContextMenu(str textureName, str name, std::vector<Panel> *elems = nullptr,
                         sf::Vector2f size = sf::Vector2f(0, 0), float posX = 0, float posY = 0) : Panel(textureName, name) {
    elements = elems;

    if (elements == nullptr) {
        elements = new std::vector<Panel>();
        return;
    }

    for(int i = 0; i < elements->size(); i++) {
        elements[0][i].setPosition(elements[0][i].PosX + PosX, elements[0][i].PosY + PosY);

        elements[0][i].setSize(elements[0][i].getSize().x * rect.getGlobalBounds().width / 1920,
                               elements[0][i].getSize().y * rect.getGlobalBounds().height / 1080);
    }

    if(size.x == 0 && size.y == 0)
        return;
    setSize(size);
    
    if(posX == 0 && posY == 0)
        return;
    setPosition(posX, posY);
}

void ContextMenu::addElement(Panel *elem) {
    elements->push_back(*elem); 

    elements[0][elements->size() - 1].setPosition(elements[0][elements->size() - 1].PosX + PosX, elements[0][elements->size() - 1].PosY + PosY);
    // elements[0][elements->size() - 1].setSize(elements[0][elements->size() - 1].getSize().x * rect.getGlobalBounds().width / 1920,
    //                        elements[0][elements->size() - 1].getSize().y * rect.getGlobalBounds().height / 1080);
}

void ContextMenu::draw(sf::RenderWindow& window) {
    if(!isInterfaceDrawn || elements->empty())
        return;
    window.draw(rect);
    for(int i = 0; i < elements->size(); i++)
        elements[0][i].draw(window);
}

bool ContextMenu::isActive(sf::Event& event) {
    if (isInterfaceDrawn) {
        if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape) {
            isInterfaceDrawn = false;
            return !(event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape);
        }
    }

    return false;
}