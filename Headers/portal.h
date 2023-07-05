#include "init.h"
#include "interactible.h"
#include "button.h"
#include "text.h"

////////////////////////////////////////////////////////////
// Class
////////////////////////////////////////////////////////////

class Portal : public Interactible {
public:
    Button yesButton, noButton;
    PlacedText questionText;

    Portal(float, float);
    bool isActivated(Rect, sf::Event&);
    bool isInterfaceDrawn = false;
    void setFunction(void (*func)(void)) { yesButton.buttonFunction = func; }
    void draw(sf::RenderWindow&);
};

////////////////////////////////////////////////////////////
// Realization
////////////////////////////////////////////////////////////

Portal::Portal(float x, float y) {
    setPosition(x, y);
    texture.loadFromFile("sources/textures/Portal.png");
    sprite.setTexture(texture);
    setSize(sprite.getGlobalBounds().width, sprite.getGlobalBounds().height);
    yesButton = Button("sources/textures/BluePanel.png", "Yes", [](){});
    noButton = Button("sources/textures/YellowPanel.png", "No", [](){});
    questionText.setText("Do you want to go through this portal?");
}

void Portal::draw(sf::RenderWindow& window) {
    window.draw(sprite);
    if (isInterfaceDrawn) {
        yesButton.draw(window);
        noButton.draw(window);
        questionText.draw(window);
    }
}

bool Portal::isActivated(Rect rect, sf::Event& event) {
    if (isInterfaceDrawn) {
        noButton.isActivated(event);
        yesButton.isActivated(event);
    }
    
    if (intersect(rect) && event.type == sf::Event::KeyPressed && event.KeyPressed == sf::Keyboard::X)
        isInterfaceDrawn = true;

    if (isInterfaceDrawn && ((event.type == sf::Event::KeyPressed && event.KeyPressed == sf::Keyboard::Escape) || noButton.isActivated(event)))
        isInterfaceDrawn = false;
}