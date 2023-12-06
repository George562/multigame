#pragma once
#include "../../SFML-2.5.1/include/SFML/Graphics.hpp"
#include <iostream>
#include "mouseTools.h"

class InteractionRect : public sf::Drawable, public sf::Transformable
{
protected:
    float width, height;
    sf::RectangleShape drawRect;
    mutable sf::Sprite sprite;
public:
    bool hasTexture = false;

    InteractionRect() {}
    InteractionRect(float _posX, float _posY, float _width, float _height)
    {
        setPosition(_posX, _posY);
        width = _width; height = _height;
        drawRect = sf::RectangleShape(sf::Vector2f(width, height));
    }

    float getX() const { return getPosition().x; }
    float getY() const { return getPosition().y; }
    float getRight() const { return getPosition().x + width * getScale().x; }
    float getBottom() const { return getPosition().y + height * getScale().y; }
    float getWidth() const { return width * getScale().x; }
    float getHeight() const { return height * getScale().y; }
    sf::Color getFillColor() const { return drawRect.getFillColor(); }

    void setSize(int _width, int _height) { width = _width; height = _height; drawRect.setSize(sf::Vector2f(width, height)); }
    void setTexture(sf::Texture& _texture)
    {        
        sprite.setTexture(_texture);
        sprite.setScale(width / sprite.getTexture()->getSize().x, height / sprite.getTexture()->getSize().y);
        hasTexture = true;
    }
    void setFillColor(sf::Color color) { drawRect.setFillColor(color); }
    void setSpriteColor(sf::Color color) { sprite.setColor(color); }
    void setOutlineColor(sf::Color color) { drawRect.setOutlineColor(color); }
    void setOutlineThickness(int thickness) { drawRect.setOutlineThickness(thickness); }

    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const
    {
        states.transform *= getTransform();
        if (hasTexture)
            target.draw(sprite, states);
        else target.draw(drawRect, states);
    }
    virtual bool isActivated(sf::Event&) { return false; };
};


bool in(InteractionRect& interactionRect, sf::Vector2f pos)
{
    return in(interactionRect.getX(), interactionRect.getY(), interactionRect.getWidth(), interactionRect.getHeight(), pos);
}

bool in(InteractionRect& interactionRect, sf::Vector2i pos)
{
    return in(interactionRect.getX(), interactionRect.getY(), interactionRect.getWidth(), interactionRect.getHeight(), pos);
}

bool in(InteractionRect& interactionRect, sf::Event::MouseButtonEvent& event)
{
    return in(interactionRect.getX(), interactionRect.getY(), interactionRect.getWidth(), interactionRect.getHeight(), event);
}

bool in(InteractionRect& interactionRect, sf::Event::MouseMoveEvent& event)
{
    return in(interactionRect.getX(), interactionRect.getY(), interactionRect.getWidth(), interactionRect.getHeight(), event);
}

bool in(InteractionRect& interactionRect, sf::Event::MouseWheelScrollEvent& event)
{
    return in(interactionRect.getX(), interactionRect.getY(), interactionRect.getWidth(), interactionRect.getHeight(), event);
}