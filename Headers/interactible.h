#pragma once
#include "animation.h"
#include "rect.h"

#define ActivationButton sf::Keyboard::X

////////////////////////////////////////////////////////////
// Class
////////////////////////////////////////////////////////////

class Interactable : public Rect, public sf::Drawable {
public:
    Animation* animation = nullptr;
    void (*function)(void);

    Interactable() : Rect() {}
    void setFunction(void (*func)(void)) { function = func; }
    virtual bool isActivated(Circle&, sf::Event&) { return false; }
    virtual bool CanBeActivated(Circle& circle) { return intersect(circle); }
    void draw(sf::RenderTarget& target, sf::RenderStates states = sf::RenderStates::Default) const {
        target.draw(*animation, states);
    }
    void setAnimation(sf::Texture& texture, int FrameAmount, int maxLevel, sf::Time duration, sf::Shader *shader = nullptr) {
        if (animation != nullptr) {
            delete animation;
        }
        animation = new Animation(texture, FrameAmount, maxLevel, duration, shader);
        setSize(animation->getGlobalSize());
        animation->play();
    };
};

////////////////////////////////////////////////////////////
// Realization
////////////////////////////////////////////////////////////