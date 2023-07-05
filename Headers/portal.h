#include "init.h"
#include "button.h"
#include "text.h"
#include "location.h"

////////////////////////////////////////////////////////////
// Class
////////////////////////////////////////////////////////////

class Portal : public Rect {
public:
    sf::Texture texture;
    sf::Sprite sprite;
    Button yesButton, noButton;
    PlacedText question;
    void (*portalFunction)(void);

    Portal(float, float);
    void setPortalFunction(void (*)(void));
    bool isActivated(Rect, sf::Event&);
};

////////////////////////////////////////////////////////////
// Realization
////////////////////////////////////////////////////////////

Portal::Portal(float x, float y) {
    setPosition(x, y);
    texture.loadFromFile("sources/Portal.png");
    sprite.setTexture(texture);
    setSize(sprite.getGlobalBounds().width, sprite.getGlobalBounds().height);
    yesButton = Button("sources/BluePanel.png", "Yes", [](){});
    noButton = Button("sources/BluePanel.png", "No", [](){});
        question.setText("Do you want to go through this portal?");
}

void Portal::setPortalFunction(void (*function)(void)) {
    portalFunction = function;
}

bool Portal::isActivated(Rect rect, sf::Event& event) {
    if(intersect(rect) && event.type == sf::Event::KeyPressed && event.KeyPressed == sf::Keyboard::X) {
    }
}