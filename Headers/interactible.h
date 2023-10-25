#pragma once
#include "init.h"

////////////////////////////////////////////////////////////
// Class
////////////////////////////////////////////////////////////

class Interactible : public Rect, public sf::Drawable {
public:
    Animation* animation = nullptr;
    void (*function)(void);

    Interactible() {}
    void setFunction(void (*func)(void)) { function = func; }
    virtual bool isActivated(Circle&, sf::Event&) { return false; }
    virtual bool CanBeActivated(Circle&) { return false; }
    void draw(sf::RenderTarget& target, sf::RenderStates states = sf::RenderStates::Default) const {
        target.draw(*animation, states);
    }
    void SetAnimation(sf::Texture& texture, int FrameAmount, sf::Vector2f frameSize, sf::Time duration) {
        if (animation != nullptr) {
            delete animation;
        }
        animation = new Animation(texture, FrameAmount, frameSize, duration);
        setSize(animation->getGlobalSize());
        animation->play();
    };
};

////////////////////////////////////////////////////////////
// Realization
////////////////////////////////////////////////////////////