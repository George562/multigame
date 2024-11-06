#pragma once
#include "../Systems/animation.h"
#include "../CollisionShapes/collisionRect.h"
#include "../CollisionShapes/collisionCircle.h"

#define ActivationButton sf::Keyboard::X
#define InformationButton sf::Keyboard::I

namespace DescriptionID {
    using Type = sf::Uint8;
    enum : Type {
        portal,
        box,
        shopSector,
        upgradeSector,
        artifact,
        DescriptionCount
    };
}

void displayDescription(DescriptionID::Type);

////////////////////////////////////////////////////////////
// Class
////////////////////////////////////////////////////////////

class Interactable : public sf::Drawable {
public:
    Animation* animation = nullptr;
    void (*function)(Interactable*);
    CollisionRect hitbox;
    DescriptionID::Type descriptionID;

    Interactable() {}
    Interactable(DescriptionID::Type id) : descriptionID(id) {}

    ~Interactable() {
        if (animation) {
            delete animation;
        }
    }

    void setFunction(void (*func)(Interactable*)) { function = func; }

    virtual bool isActivated(CollisionCircle&, sf::Event& event) {
        if (keyPressed(event, ActivationButton)) {
            function(this);
            return true;
        }
        if (keyPressed(event, InformationButton)) {
            displayDescription(descriptionID);
        }
        return false;
    }

    virtual bool CanBeActivated(CollisionCircle& circle) { return hitbox.intersect(circle); }

    void draw(sf::RenderTarget& target, sf::RenderStates states = sf::RenderStates::Default) const {
        if (animation != nullptr) {
            target.draw(*animation, states);
        }
    }

    void setAnimation(sf::Texture& texture, int FrameAmount, int maxLevel, sf::Time duration, sf::Shader *shader = nullptr) {
        if (animation != nullptr) {
            delete animation;
        }
        animation = new Animation("");
        animation->setAnimation(texture, FrameAmount, maxLevel, duration, shader);
        hitbox.setSize(animation->getGlobalSize());
        animation->play();
    };

    void setAnimation(sf::Texture& texture, sf::Shader *shader = nullptr) {
        if (animation != nullptr) {
            delete animation;
        }
        animation = new Animation("");
        animation->setTexture(texture, shader);
        hitbox.setSize(animation->getGlobalSize());
        animation->play();
    };

    void setSize(float w, float h) { hitbox.setSize(w, h); animation->setSize(sf::Vector2f(w, h)); }
    void setSize(sf::Vector2f v) { hitbox.setSize(v); animation->setSize(v); }

    void setPosition(float x, float y) { hitbox.setPosition(x, y); animation->setPosition(x, y); }
    void setPosition(sf::Vector2f v) { setPosition(v.x, v.y); }

    void setCenter(float x, float y) { setPosition(x - hitbox.getSize().x / 2, y - hitbox.getSize().y / 2); }
    void setCenter(sf::Vector2f v) { setCenter(v.x, v.y); }
};

sf::Packet& operator<<(sf::Packet& packet, Interactable* i) {
    return packet << i->hitbox.getCenter();
}
sf::Packet& operator>>(sf::Packet& packet, Interactable* i) {
    sf::Vector2f v; packet >> v;
    i->setCenter(v);
    return packet;
}