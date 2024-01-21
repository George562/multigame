#pragma once
#include "rect.h"
#include "animation.h"
#define _USE_MATH_DEFINES
#include <cmath>

class Fire : public Rect, public sf::Drawable {
public:
    Animation* animation = nullptr;
    // fire propagation
    sf::Clock clock;
    sf::Time secs;

    Fire* descendant1 = nullptr;
    Fire* descendant2 = nullptr;

    Fire(float PosX, float PosY, float Width, float Height, sf::Clock clock, sf::Time secs) {
        this->PosX = PosX;
        this->PosY = PosY;
        this->Width = Width;
        this->Height = Height;
        this->clock = clock;
        clock.restart();
        this->secs = secs;
    }

    void Propagation() {
        float dist = 3 * Width / 2;
        float angle = (rand() % 361) * M_PI / 180;
        descendant1 = new Fire(PosX + dist * std::sin(angle), PosY + dist * std::cos(angle), Width, Height, sf::Clock(), sf::seconds(rand() % 4 + 1));
        angle = (rand() % 360) * M_PI / 180;
        descendant2 = new Fire(PosX + dist * std::sin(angle), PosY + dist * std::cos(angle), Width, Height, sf::Clock(), sf::seconds(rand() % 4 + 1));
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