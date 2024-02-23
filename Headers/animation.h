#pragma once
#include "../SFML-2.5.1/include/SFML/Graphics.hpp"

////////////////////////////////////////////////////////////
// Class
////////////////////////////////////////////////////////////

#pragma pack(push, 1)
class Animation : public sf::Drawable, public sf::Transformable {
private:
    mutable sf::Sprite sprite;
    int maxLevel, curLevel;
    sf::Clock* localClock = nullptr;
    sf::Time duration;
    mutable sf::Time curTime;
    int frameAmount;
    bool isPlaying;
    sf::Shader* shader = nullptr;

public:
    Animation();
    Animation(sf::Texture& texture, int FrameAmount, int maxLevel, sf::Time duration, sf::Shader* shader = nullptr);
    ~Animation();
    void draw(sf::RenderTarget& target, sf::RenderStates states = sf::RenderStates::Default) const;

    void play();
    void pause();
    void stop();

    void setTexture(sf::Texture& name);
    void setShader(sf::Shader* shader);
    void setAnimationLevel(int);
    void setSize(sf::Vector2f size);
    sf::Vector2f getGlobalSize() const;
    sf::Vector2f getLocalSize() const;
};
#pragma pack(pop)

////////////////////////////////////////////////////////////
// Realization
////////////////////////////////////////////////////////////

Animation::Animation() {
    this->localClock = new sf::Clock;
    this->isPlaying = false;
    this->curTime = sf::Time::Zero;
    curLevel = 0;
}

Animation::Animation(sf::Texture& texture, int FrameAmount, int maxLevel, sf::Time duration, sf::Shader* shader) : Animation() {
    this->sprite.setTexture(texture);
    this->frameAmount = FrameAmount;
    this->maxLevel = maxLevel;
    this->duration = duration;
    this->shader = shader;
}

Animation::~Animation() {
    if (localClock) {
        delete localClock;
    }
}

void Animation::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    if (isPlaying) {
        curTime = (curTime + localClock->restart()) % duration;
    }
    sprite.setTextureRect(
        sf::IntRect(
            sprite.getTexture()->getSize().x * int((curTime / duration) * frameAmount) / frameAmount,
            curLevel * sprite.getTexture()->getSize().y / maxLevel,
            sprite.getTexture()->getSize().x / frameAmount,
            sprite.getTexture()->getSize().y / maxLevel
        )
    );
    states.transform *= getTransform();
    states.shader = shader;
    target.draw(sprite, states);
}

void Animation::play() {
    isPlaying = true;
    localClock->restart();
}

void Animation::pause() {
    isPlaying = false;
}

void Animation::stop() {
    isPlaying = false;
    curTime = sf::Time::Zero;
}

void Animation::setTexture(sf::Texture& texture) {
    sprite.setTexture(texture);
}

void Animation::setShader(sf::Shader* shader) {
    this->shader = shader;
}

void Animation::setSize(sf::Vector2f size) {
    setScale(size.x / getLocalSize().x, size.y / getLocalSize().y);
}

sf::Vector2f Animation::getGlobalSize() const {
    return sf::Vector2f(getScale().x * sprite.getTexture()->getSize().x / frameAmount,
                        getScale().y * sprite.getTexture()->getSize().y / maxLevel);
}

sf::Vector2f Animation::getLocalSize() const {
    return sf::Vector2f(sprite.getTexture()->getSize().x / frameAmount,
                        sprite.getTexture()->getSize().y / maxLevel);
}

void Animation::setAnimationLevel(int level) {
    curLevel = level;
}