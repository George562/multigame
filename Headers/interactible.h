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
    void (*function)(Interactable*);

    Interactable() : Rect() {}
    void setFunction(void (*func)(Interactable*)) { function = func; }
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
    
    void setSize(float x, float y) { Rect::setSize(x, y); animation->setSize(sf::Vector2f(x, y)); }
    void setSize(sf::Vector2f v) { Rect::setSize(v); animation->setSize(v); }

    void setPosition(float x, float y) { PosX = x; PosY = y; animation->setPosition(PosX, PosY); }
    void setPosition(sf::Vector2f v) { setPosition(v.x, v.y); }

    void setCenter(float x, float y) { setPosition(x - animation->getGlobalSize().x / 2, y - animation->getGlobalSize().y / 2); }
    void setCenter(sf::Vector2f v) { setCenter(v.x, v.y); }
};

////////////////////////////////////////////////////////////
// Realization
////////////////////////////////////////////////////////////