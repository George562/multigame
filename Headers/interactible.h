#pragma once
#include "animation.h"
#include "rect.h"
#include "circle.h"

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

    virtual bool isActivated(Circle&, sf::Event& event) {
        if (event.type == sf::Event::KeyPressed && event.key.code == ActivationButton) {
            function(this);
            return true;
        }
        return false;
    }

    virtual bool CanBeActivated(Circle& circle) { return intersect(circle); }

    void draw(sf::RenderTarget& target, sf::RenderStates states = sf::RenderStates::Default) const {
        if (animation != nullptr) {
            animation->setPosition(getPosition());
            target.draw(*animation, states);
        }
    }

    void setAnimation(sf::Texture& texture, int FrameAmount, int maxLevel, sf::Time duration, sf::Shader *shader = nullptr) {
        if (animation != nullptr) {
            delete animation;
        }
        animation = new Animation(texture, FrameAmount, maxLevel, duration, shader);
        Rect::setSize(animation->getGlobalSize());
        animation->play();
    };

    void setSize(float w, float h) { Rect::setSize(w, h); animation->setSize(sf::Vector2f(w, h)); }
    void setSize(sf::Vector2f v) { Rect::setSize(v); animation->setSize(v); }

    void setPosition(float x, float y) { Rect::setPosition(x, y); animation->setPosition(x, y); }
    void setPosition(sf::Vector2f v) { setPosition(v.x, v.y); }

    void setCenter(float x, float y) { setPosition(x - Width / 2, y - Height / 2); }
    void setCenter(sf::Vector2f v) { setCenter(v.x, v.y); }
};

////////////////////////////////////////////////////////////
// Realization
////////////////////////////////////////////////////////////