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
    Button yesButton("sources/BluePanel.png", "Yes", [](){});
    Button noButton("sources/RedPanel.png", "No", [](){});
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
}

void Portal::setPortalFunction(void (*function)(void)) {
    portalFunction = function;
}

bool Portal::isActivated(Rect rect, sf::Event& event, ) {
    if(intersect(rect) && event.type == sf::Event::KeyPressed && event.KeyPressed == sf::Keyboard::X) {
        PlacedText question = PlacedText();
        question.setText("Do you want to go through this portal?");
    }
}