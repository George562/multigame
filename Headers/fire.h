#pragma once
#include "rect.h"
#include "animation.h"
#define _USE_MATH_DEFINES
#include <cmath>

class Fire : public Rect, public sf::Drawable {
public:
    Animation* animation = nullptr;
    // fire propagation
    sf::Clock* clock;
    sf::Time secs;

    Location* loc;

    Fire* descendant1 = nullptr;
    Fire* descendant2 = nullptr;

    Fire(sf::Time secs, Location* loc) {
        this->clock = new sf::Clock();
        this->secs = secs;
        this->loc = loc;
    }

    void Propagation() {
        float dist = 3 * Width / 2;
        float angle;
        descendant1 = new Fire(sf::seconds(rand() % 4 + 100.f), loc);
        descendant1->setSize(Width, Height);
        do {
            angle = (rand() % 360) * M_PI / 180;
            descendant1->setPosition(PosX + dist * std::sin(angle), PosY + dist * std::cos(angle));
            descendant1->setAnimation(Textures::Fire, 1, 1, sf::seconds(1), &Shaders::Map);
        } while (!loc->EnableTiles[(int)descendant1->PosY / size][(int)descendant1->PosX / size]);
        descendant2 = new Fire(sf::seconds(rand() % 4 + 100.f), loc);
        descendant2->setSize(Width, Height);
        do {
            angle = (rand() % 360) * M_PI / 180;
            descendant2->setPosition(PosX + dist * std::sin(angle), PosY + dist * std::cos(angle));
            descendant2->setAnimation(Textures::Fire, 1, 1, sf::seconds(1), &Shaders::Map);
        } while (!loc->EnableTiles[(int)descendant2->PosY / size][(int)descendant2->PosX / size]);
    }

    void setAnimation(sf::Texture& texture, int FrameAmount, int maxLevel, sf::Time duration, sf::Shader *shader = nullptr) {
        if (animation != nullptr) {
            delete animation;
        }
        animation = new Animation(texture, FrameAmount, maxLevel, duration, shader);
        animation->setSize({Width, Height});
        animation->setOrigin(animation->getLocalSize() / 2.f);
        animation->play();
    };

    void draw(sf::RenderTarget& target, sf::RenderStates states = sf::RenderStates::Default) const {
        if (animation != nullptr) {
            animation->setPosition(getPosition());
            target.draw(*animation, states);
        }
    };
};