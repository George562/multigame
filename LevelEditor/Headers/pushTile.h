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

    sf::Color activeColor = sf::Color::White;
    sf::Color inactiveColor = sf::Color(100, 100, 100);

public:
    PushTile() {}
    PushTile(float, float, float, float);

    int getState() { return state; }
    void setState(int val, sf::Color color = sf::Color::White)
    {
        state = val;
        if(state != 0)
        {
            activeColor = color;
            setFillColor(activeColor);
        }
        else setFillColor(inactiveColor);
    };

    void setActiveColor(sf::Color color) { activeColor = color; setFillColor(state != 0 ? activeColor : inactiveColor); }
    void setInactiveColor(sf::Color color) { inactiveColor = color; setFillColor(state != 0 ? activeColor : inactiveColor); }

    bool isActivated(sf::Event&) override;
    void draw(sf::RenderTarget&, sf::RenderStates) const override;
};

PushTile::PushTile(float _posX, float _posY, float _width, float _height) : InteractionRect(_posX, _posY, _width, _height)
{
    setOutlineColor(sf::Color::Black); setOutlineThickness(1);
    setFillColor(inactiveColor);
}

bool PushTile::isActivated(sf::Event& event)
{
    if(event.type == sf::Event::MouseButtonPressed && event.mouseButton.button != sf::Mouse::Button::Middle)
    {
        readPosition = true;
        lastButton = event.mouseButton.button;
        return in(*this, event.mouseButton) && lastButton != sf::Mouse::Button::Right;
    }

    if(event.type == sf::Event::MouseMoved && readPosition && in(*this, event.mouseMove))
    {
        readPosition = false;
        if(lastButton == sf::Mouse::Left)
            return true;
        else if(lastButton == sf::Mouse::Right)
        {
            setState(0, inactiveColor);
            return false;
        }
    }

    if(readPosition && event.type == sf::Event::MouseButtonReleased)
    {
        readPosition = false;
        if(in(*this, event.mouseButton))
        {
            if(lastButton == sf::Mouse::Left)
            {
                return true;
            }
            else if(lastButton == sf::Mouse::Right)
            {
                setState(0, inactiveColor);
                return false;
            }
        }
    }

    return false;
}

void PushTile::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    states.transform *= getTransform();
    target.draw(drawRect, states);
}