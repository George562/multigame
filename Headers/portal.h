#include "interactible.h"
#include "button.h"

////////////////////////////////////////////////////////////
// Class
////////////////////////////////////////////////////////////

class Portal : public Interactible {
public:
    Button *yesButton, *noButton;
    PlacedText questionText;

    Portal();
    bool isActivated(Rect, sf::Event&);
    bool isInterfaceDrawn = false;
    void setFunction(void (*func)(void)) { yesButton->buttonFunction = func; }
    void draw(sf::RenderWindow&, sf::Vector2f&);
};

////////////////////////////////////////////////////////////
// Realization
////////////////////////////////////////////////////////////

Portal::Portal() {
    texture.loadFromFile("sources/textures/Portal.png");
    sprite.setTexture(texture);
    setSize(sprite.getGlobalBounds().width, sprite.getGlobalBounds().height);

    yesButton = new Button("sources/textures/BluePanel", "Yes", [](){});
    yesButton->setSize(200, 100);
    yesButton->setPosition(scw / 3 - yesButton->Width / 2, 680);

    noButton = new Button("sources/textures/YellowPanel", "No", [](){});
    noButton->setSize(200, 100);
    noButton->setPosition(scw * 2 / 3 - noButton->Width / 2, 680);

    questionText.setText("Do you want to go through this portal?");
    questionText.setCenter(scw / 2, 200);
}

void Portal::draw(sf::RenderWindow& window, sf::Vector2f& cam) {
    sprite.setPosition(getPosition() - cam);
    window.draw(sprite);
    if (isInterfaceDrawn) {
        yesButton->draw(window);
        noButton->draw(window);
        questionText.draw(window);
    }
}

bool Portal::isActivated(Rect rect, sf::Event& event) {
    if (isInterfaceDrawn) {
        if ((event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape) || noButton->isActivated(event) || yesButton->isActivated(event)) {
            isInterfaceDrawn = false;
            return !(event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape);
        }
    }
    
    if (intersect(rect) && event.type == sf::Event::KeyPressed && event.key.code == ActivationButton) {
        isInterfaceDrawn = true;
        return true;
    }

    return false;
}