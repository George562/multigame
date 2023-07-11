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
    bool isInterfaceDrawn = false;
    void setFunction(void (*func)(void)) { yesButton->buttonFunction = func; }
    void interface(sf::RenderTarget&);
    virtual void draw(sf::RenderTarget&, sf::RenderStates = sf::RenderStates::Default) const;
    bool isActivated(Rect, sf::Event&);

    void setPosition(sf::Vector2f v) { PosX = v.x; PosY = v.y; sprite.setPosition(PosX, PosY); }
    void setPosition(float x, float y) { PosX = x; PosY = y; sprite.setPosition(PosX, PosY); }

    void setCenter(float x, float y) { setPosition(x - Width / 2, y - Height / 2); }
    void setCenter(sf::Vector2f v) { setCenter(v.x, v.y); }
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

    sprite.setPosition(PosX, PosY);
}

void Portal::interface(sf::RenderTarget& target) {
    if (isInterfaceDrawn) {
        target.draw(*yesButton);
        target.draw(*noButton);
        target.draw(questionText);
    }
}

void Portal::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    target.draw(sprite);
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