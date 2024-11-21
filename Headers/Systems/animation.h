#pragma once
#include "../../SFML-2.5.1/include/SFML/Graphics.hpp"
#include "../Abstracts/UIElement.h"

////////////////////////////////////////////////////////////
// Class
////////////////////////////////////////////////////////////

#pragma pack(push, 1)
class Animation : public UIElement {
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
    Animation(std::string);
    Animation(std::string, UI::Anchor, UI::Anchor);
    Animation(std::string, sf::Vector2f);
    ~Animation();
    void draw(sf::RenderTarget& target, sf::RenderStates states = sf::RenderStates::Default) const;

    void play();
    void pause();
    void stop();

    void setTexture(sf::Texture& name, sf::Shader* shader = nullptr);
    void setShader(sf::Shader* shader);
    void setAnimation(sf::Texture& texture, int FrameAmount, int maxLevel, sf::Time duration, sf::Shader* shader = nullptr);
    void setAnimationLevel(int);
    void setSize(float w, float h);
    void setSize(sf::Vector2f size) { setSize(size.x, size.y); }
    void setPosition(float x, float y);
    void setPosition(sf::Vector2f pos) { setPosition(pos.x, pos.y); }
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

Animation::Animation(std::string name) : Animation() {
    setName(name);
}

Animation::Animation(std::string name, UI::Anchor anchor, UI::Anchor anchoringPoint) : Animation(name) {
    setAnchors(anchor, anchoringPoint);
}

Animation::Animation(std::string name, sf::Vector2f pos) : Animation(name, UI::none, UI::none) {
    setPosition(pos.x, pos.y);
}

Animation::~Animation() {
    if (localClock) {
        delete localClock;
    }
}

void Animation::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    if (duration != sf::Time::Zero) {
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
    }
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

void Animation::setTexture(sf::Texture& texture, sf::Shader* shader) {
    sprite.setTexture(texture);
    this->frameAmount = 1;
    this->maxLevel = 1;
    this->duration = sf::Time::Zero;
    this->isPlaying = true;
    this->shader = shader;
    UIRect::setSize(texture.getSize().x, texture.getSize().y);
}

void Animation::setAnimation(sf::Texture& texture, int FrameAmount, int maxLevel, sf::Time duration, sf::Shader* shader) {
    this->sprite.setTexture(texture);
    this->frameAmount = FrameAmount;
    this->maxLevel = maxLevel;
    this->duration = duration;
    this->shader = shader;
    UIRect::setSize(texture.getSize().x / frameAmount, texture.getSize().y / maxLevel);
}

void Animation::setShader(sf::Shader* shader) {
    this->shader = shader;
}

void Animation::setSize(float w, float h) {
    UIRect::setSize(w / frameAmount, h / maxLevel);
    setScale(w / getLocalSize().x, h / getLocalSize().y);
}

void Animation::setPosition(float x, float y) {
    UIRect::setPosition(x, y);
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