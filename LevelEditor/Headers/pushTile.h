#pragma once
#include "../../SFML-2.5.1/include/SFML/Graphics.hpp"
#include "interactionRect.h"
#include "mouseTools.h"

class PushTile : public InteractionRect
{
private:
    int state = 0;
    bool readPosition = false;
    int lastButton = 0;

    sf::Color activeColor = sf::Color(0, 175, 100);
    sf::Color inactiveColor = sf::Color::White;

public:
    PushTile() {}
    PushTile(float, float, int, int);

    int getState() { return state; }
    void setState(int val) { state = val; };
    void centerText();

    void move(float x, float y) override { posX += x; posY += y; drawRect.setPosition(posX, posY); }

    bool isActivated(sf::Event&) override;
    void draw(sf::RenderTarget&, sf::RenderStates) const override;
};

PushTile::PushTile(float _posX, float _posY, int _width, int _height) : InteractionRect(_posX, _posY, _width, _height)
{
    drawRect.setOutlineColor(sf::Color::Black); drawRect.setOutlineThickness(1);
    drawRect.setFillColor(inactiveColor);
}

bool PushTile::isActivated(sf::Event& event)
{
    if(event.type == sf::Event::MouseButtonPressed)
    {
        readPosition = true;
        lastButton = event.mouseButton.button;
        return in(posX, posY, width, height, event.mouseButton);
    }

    if(readPosition && event.type == sf::Event::MouseButtonReleased)
    {
        readPosition = false;
        if(in(posX, posY, width, height, event.mouseButton))
        {
            if(lastButton == sf::Mouse::Left)
            {
                setState(1);
                drawRect.setFillColor(activeColor);
            }
            else if(lastButton == sf::Mouse::Right)
            {
                setState(0);
                drawRect.setFillColor(inactiveColor);
            }

            return true;
        }
    }

    if(readPosition && in(posX, posY, width, height, event.mouseMove))
    {
        readPosition = false;
        if(lastButton == sf::Mouse::Left)
        {
            setState(1);
            drawRect.setFillColor(activeColor);
        }
        else if(lastButton == sf::Mouse::Right)
        {
            setState(0);
            drawRect.setFillColor(inactiveColor);
        }

        return true;
    }

    return false;
}

void PushTile::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    target.draw(drawRect);
}