#pragma once
#include "../SFML-2.5.1/include/SFML/Graphics.hpp"

////////////////////////////////////////////////////////////
// Class
////////////////////////////////////////////////////////////

class Animation : public sf::Drawable, public sf::Transformable {
public:
    sf::Texture texture;
    mutable sf::Sprite sprite;
    sf::Clock* localClock;
    sf::Time duration;
    mutable sf::Time curTime;
    int frameAmount;
    sf::Vector2f frameSize;
    bool isPlaying;
    int animationLevel;

    Animation(std::string TexturFileName, int FrameAmount, sf::Vector2f frameSize, sf::Time duration);
    void setTexture(std::string name);
    void draw(sf::RenderTarget& target, sf::RenderStates states = sf::RenderStates::Default) const;

    void play();
    void pause();
    void stop();

    void setSize(sf::Vector2f size);
    sf::Vector2f getSize() const;
    sf::Vector2f getGlobalSize() const;
    sf::Vector2f getLocalSize() const;

};

////////////////////////////////////////////////////////////
// Realization
////////////////////////////////////////////////////////////

Animation::Animation(std::string TexturFileName, int FrameAmount, sf::Vector2f frameSize, sf::Time duration) {
    this->texture.loadFromFile(TexturFileName);
    this->texture.setSmooth(true);
    this->frameAmount = FrameAmount;
    this->frameSize = frameSize;
    this->duration = duration;
    this->sprite.setTexture(this->texture);
    this->sprite.setTextureRect({0, 0, (int)frameSize.x, (int)frameSize.y});
    this->localClock = new sf::Clock;
    this->isPlaying = false;
    this->curTime = sf::Time::Zero;
    animationLevel = 0;
}

void Animation::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    if (isPlaying) {
        curTime = (curTime + localClock->restart()) % duration;
    }
    sprite.setTextureRect({(int)frameSize.x * int((curTime / duration) * frameAmount), animationLevel * (int)frameSize.y,
                           (int)frameSize.x, (int)frameSize.y});
    states.transform *= getTransform();
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

void Animation::setTexture(std::string name) {
    if (!texture.loadFromFile(name)) {
        std::cout << "Can't load texture" << name << '\n';
    } else {
        sprite.setTexture(texture);
    }
}

void Animation::setSize(sf::Vector2f size) {
    setScale(size.x / sprite.getTextureRect().width, size.y / sprite.getTextureRect().height);
}

sf::Vector2f Animation::getGlobalSize() const {
    return { sprite.getLocalBounds().width * getScale().x, sprite.getLocalBounds().height * getScale().y };
}

sf::Vector2f Animation::getLocalSize() const {
    return  {sprite.getLocalBounds().width, sprite.getLocalBounds().height };
}