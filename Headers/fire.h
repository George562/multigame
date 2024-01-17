#pragma once
#include "rect.h"
#include "animation.h"
#include "init.h"

class Fire : public Rect, public sf::Drawable {
public:    
    Animation* animation = nullptr;
    // fire propagation
    int tacts;

    Fire* descendant1 = nullptr;
    Fire* descendant2 = nullptr;

    Fire(float PosX, float PosY, float Width, float Height, int tacts) {
        this->PosX = PosX;
        this->PosY = PosY;
        this->Width = Width;
        this->Height = Height;
        this->tacts = tacts;
        setAnimation(Textures::Fire, 1, 1, sf::seconds(1), &Shaders::Map);
    }

    void Propagation() {
        float dist = 3 * Width / 2;
        float angle = (rand() % 361) * M_PI / 180;
        descendant1 = new Fire(PosX + dist * std::sin(angle), PosY + dist * std::cos(angle), Width, Height, rand() % 1000 + 1);
        angle = (rand() % 360) * M_PI / 180;
        descendant2 = new Fire(PosX + dist * std::sin(angle), PosY + dist * std::cos(angle), Width, Height, rand() % 1000 + 1);
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