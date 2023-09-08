#include "../SFML-2.5.1/include/SFML/Graphics.hpp"
#include "scale.h"

////////////////////////////////////////////////////////////
// Class
////////////////////////////////////////////////////////////

class Animation : public sf::Drawable {
public:
    sf::Texture texture;
    mutable sf::Sprite sprite;
    sf::Clock* localClock;
    sf::Time duration, curTime;
    int frameAmount;
    sf::Vector2f frameSize;
    bool isPlaying;

    Animation(std::string TexturFileName, int FrameAmount, sf::Vector2f frameSize, sf::Time duration);
    void draw(sf::RenderTarget& target, sf::RenderStates states = sf::RenderStates::Default) const;
    void play();
    void pause();
    void stop();
    void setPosition(sf::Vector2f position) { sprite.setPosition(position); }
};

////////////////////////////////////////////////////////////
// Realization
////////////////////////////////////////////////////////////

Animation::Animation(std::string TexturFileName, int FrameAmount, sf::Vector2f frameSize, sf::Time duration) {
    this->texture.loadFromFile(TexturFileName);
    this->frameAmount = FrameAmount;
    this->frameSize = frameSize;
    this->duration = duration;
    this->sprite.setTexture(this->texture);
    this->sprite.setTextureRect({0, 0, frameSize.x, frameSize.y});
    this->localClock = new sf::Clock;
    this->isPlaying = false;
    this->curTime = sf::Time::Zero;
}
void Animation::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    if (isPlaying) {
        curTime = (curTime + localClock->getElapsedTime()) % duration;
        localClock->restart();
    }
    sprite.setTextureRect({texture.getSize().x * int(curTime.asSeconds() * duration.asSeconds() / frameAmount),
                           0,
                           frameSize.x, frameSize.y});
    target.draw(sprite);
}

void Animation::play() {
    isPlaying = true;
}

void Animation::pause() {
    isPlaying = false;
}

void Animation::stop() {
    isPlaying = false;
    curTime = sf::Time::Zero;
}