#pragma once
#include "../Systems/animation.h"
#include "../CollisionShapes/collisionRect.h"
#include "../CollisionShapes/collisionCircle.h"

#define ActivationButton sf::Keyboard::X

////////////////////////////////////////////////////////////
// Class
////////////////////////////////////////////////////////////

class Interactable : public CollisionRect, public sf::Drawable {
public:
    Animation* animation = nullptr;
    void (*function)(Interactable*);

    Interactable() : CollisionRect() {}

    ~Interactable() {
        if (animation) {
            delete animation;
        }
    }

    void setFunction(void (*func)(Interactable*)) { function = func; }

    virtual bool isActivated(CollisionCircle&, sf::Event& event) {
        if (event.type == sf::Event::KeyPressed && event.key.code == ActivationButton) {
            function(this);
            return true;
        }
        return false;
    }

    virtual bool CanBeActivated(CollisionCircle& circle) { return intersect(circle); }

    void draw(sf::RenderTarget& target, sf::RenderStates states = sf::RenderStates::Default) const {
        if (animation != nullptr) {
            target.draw(*animation, states);
        }
    }

    void setAnimation(sf::Texture& texture, int FrameAmount, int maxLevel, sf::Time duration, sf::Shader *shader = nullptr) {
        if (animation != nullptr) {
            delete animation;
        }
        animation = new Animation(texture, FrameAmount, maxLevel, duration, shader);
        CollisionRect::setSize(animation->getGlobalSize());
        animation->play();
    };

    void setAnimation(sf::Texture& texture, sf::Shader *shader = nullptr) {
        if (animation != nullptr) {
            delete animation;
        }
        animation = new Animation(texture, shader);
        CollisionRect::setSize(animation->getGlobalSize());
        animation->play();
    };

    void setSize(float w, float h) { CollisionRect::setSize(w, h); animation->setSize(sf::Vector2f(w, h)); }
    void setSize(sf::Vector2f v) { CollisionRect::setSize(v); animation->setSize(v); }

    void setPosition(float x, float y) { CollisionRect::setPosition(x, y); animation->setPosition(x, y); }
    void setPosition(sf::Vector2f v) { setPosition(v.x, v.y); }

    void setCenter(float x, float y) { setPosition(x - getSize().x / 2, y - getSize().y / 2); }
    void setCenter(sf::Vector2f v) { setCenter(v.x, v.y); }
};

////////////////////////////////////////////////////////////
// Realization
////////////////////////////////////////////////////////////