#pragma once
#include "../../SFML-2.5.1/include/SFML/Graphics.hpp"
#include <iostream>

class InteractionRect : public sf::Drawable, public sf::Transformable
{
protected:
    float posX, posY,width, height;
    sf::RectangleShape drawRect;
public:
    InteractionRect() {}
    InteractionRect(float _posX, float _posY, float _width, float _height)
    {
        posX = _posX; posY = _posY;
        width = _width; height = _height;

        drawRect = sf::RectangleShape(sf::Vector2f(width, height));
        drawRect.setPosition(posX, posY);
    }

    float getX() const { return posX; }
    float getY() const { return posY; }
    float getRight() const { return posX + width; }
    float getBottom() const { return posY + height; }
    float getWidth() const { return width; }
    float getHeight() const { return height; }
    sf::Vector2f getOrigin() const { return drawRect.getOrigin(); }

    void setSize(int _width, int _height) { width = _width; height = _height; drawRect.setSize(sf::Vector2f(width, height)); }
    void setOrigin(float x, float y) { drawRect.setOrigin(x, y); posX = drawRect.getPosition().x; posY = drawRect.getPosition().y; }

    void setFillColor(sf::Color color) { drawRect.setFillColor(color); }
    void setOutlineColor(sf::Color color) { drawRect.setOutlineColor(color); }
    void setOutlineThickness(int thickness) { drawRect.setOutlineThickness(thickness); }

    virtual void move(float deltaX, float deltaY) { posX += deltaX; posY += deltaY; drawRect.setPosition(posX, posY); }
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const { target.draw(drawRect, states); }
    virtual bool isActivated(sf::Event&) { return false; };
};