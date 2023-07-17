#pragma once
#include "../../SFML-2.5.1/include/SFML/Graphics.hpp"
#include "interactionRect.h"
#include "mouseTools.h"
#include "numBox.h"

class ScrollContainer : public InteractionRect
{
private:
    bool readInput = false;
    int scrollMultiplier, elementsScrollMultiplier, scrollValue;
    float minScrollY, maxScrollY;

    sf::Color inactiveColor = sf::Color(220, 220, 220);
    sf::Color activeColor = sf::Color::White;
    InteractionRect scrollRect, scrollBoundRect;

    std::vector<InteractionRect*> elements;

    void updateScrollbar(InteractionRect&);

public:
    ScrollContainer() {};
    ScrollContainer(float, float, int, int, int);
    
    void addElement(InteractionRect&);

    bool isActivated(sf::Event&) override;
    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
};

ScrollContainer::ScrollContainer(float _posX, float _posY, int _width, int _height, int multiplier = 10) : InteractionRect(_posX, _posY, _width, _height)
{
    drawRect.setOutlineColor(sf::Color::Black); drawRect.setOutlineThickness(2);
    drawRect.setFillColor(inactiveColor);

    scrollMultiplier = elementsScrollMultiplier = multiplier;
    scrollValue = 0;
    minScrollY = posY; maxScrollY = getBottom();

    scrollRect = InteractionRect(posX + 4, posY + 4, 10, height - 8);
    scrollRect.setFillColor(sf::Color::Black);

    scrollBoundRect = InteractionRect(posX, posY, 18, height);
    scrollBoundRect.setFillColor(sf::Color(170, 170, 170));
}

void ScrollContainer::updateScrollbar(InteractionRect& element)
{
    if(element.getBottom() > maxScrollY)
        maxScrollY = element.getBottom() + element.getHeight() / 2;

    if(maxScrollY > getBottom())
    {
        scrollRect.setSize(scrollRect.getWidth(), height * height / maxScrollY);
        elementsScrollMultiplier = scrollMultiplier * height / scrollRect.getHeight();
    }
}

void ScrollContainer::addElement(InteractionRect& element)
{
    elements.push_back(&element);
    updateScrollbar(element);
}

bool ScrollContainer::isActivated(sf::Event& event)
{
    if(event.type == sf::Event::MouseWheelScrolled && readInput)
    {
        if(scrollRect.getY() - scrollMultiplier * event.mouseWheelScroll.delta >= posY &&
           scrollRect.getBottom() - scrollMultiplier * event.mouseWheelScroll.delta <= getBottom() - 4)
        {
            scrollRect.move(0, -scrollMultiplier * event.mouseWheelScroll.delta);
            for(InteractionRect *ir : elements)
                ir->move(0, elementsScrollMultiplier * event.mouseWheelScroll.delta);
        }

        return true;
    }

    if(event.type == sf::Event::MouseButtonPressed)
    {
        readInput = in(posX, posY, width, height, event.mouseButton);
        drawRect.setFillColor(readInput ? activeColor : inactiveColor);
        return true;
    }

    if(event.type == sf::Event::MouseButtonReleased)
    {
        readInput = in(posX, posY, width, height, event.mouseButton);
        drawRect.setFillColor(readInput ? activeColor : inactiveColor);
        return true;
    }

    return false;
}

void ScrollContainer::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    target.draw(drawRect);
    scrollBoundRect.draw(target, states);
    scrollRect.draw(target, states);

    for(InteractionRect *ir : elements)
        if(ir->getY() >= posY && ir->getBottom() <= getBottom())
            ir->draw(target, states);
}