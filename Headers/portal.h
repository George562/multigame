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
    bool isActivated(Rect, sf::Event&);

    void setPosition(float x, float y) { PosX = x; PosY = y; sprite.setPosition(PosX, PosY); }
    void setPosition(sf::Vector2f v) { setPosition(v.x, v.y); }

    void setCenter(float x, float y) { setPosition(x - Width / 2, y - Height / 2); }
    void setCenter(sf::Vector2f v) { setCenter(v.x, v.y); }

    virtual void draw(sf::RenderTarget& target, sf::RenderStates states = sf::RenderStates::Default) const {
        target.draw(sprite, states);
        if (isInterfaceDrawn) {
            target.setView(InterfaceView);
            target.draw(*yesButton);
            target.draw(*noButton);
            target.draw(questionText);
            target.setView(GameView);
        }
    }
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

    questionText.setString("Do you want to go through this portal?");
    questionText.setCenter(scw / 2, 200);

    sprite.setPosition(PosX, PosY);
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