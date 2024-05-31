#pragma once
#include "../../SFML-2.5.1/include/SFML/Graphics.hpp"
#include "interactionRect.h"
#include "mouseTools.h"
#include "numBox.h"

class ScrollContainer
{
private:
    bool readInput = false;
    int scrollMultiplier, elementsScrollMultiplier, scrollValue;
    float minScrollY, maxScrollY;

    sf::View scrollView;
    sf::FloatRect scrollViewport;
    sf::Color inactiveColor = sf::Color(220, 220, 220);
    sf::Color activeColor = sf::Color::White;
    InteractionRect backRect, scrollRect, scrollBoundRect;

    std::vector<InteractionRect*> elements;

    void updateScrollbar(InteractionRect&);

public:
    ScrollContainer() {};
    ScrollContainer(float, float, float, float, sf::FloatRect, int);

    void addElement(InteractionRect&);
    InteractionRect* getRect() { return &backRect; }
    sf::View* getView() { return &scrollView; }
    sf::FloatRect getViewport() { return scrollViewport; }

    void setViewport(sf::FloatRect newViewport) { scrollViewport = newViewport; scrollView.setViewport(newViewport); }
    bool isActivated(sf::Event&);
    void draw(sf::RenderTarget& target, sf::RenderStates states) const;
};

ScrollContainer::ScrollContainer(float _posX, float _posY, float _width, float _height,
                                 sf::FloatRect viewport, int multiplier = 10)
{
    backRect = InteractionRect(_posX, _posY, _width, _height);
    backRect.setOutlineColor(sf::Color::Black); backRect.setOutlineThickness(2);
    backRect.setFillColor(inactiveColor);

    scrollMultiplier = elementsScrollMultiplier = multiplier;
    scrollValue = 0;
    minScrollY = _posY; maxScrollY = _posY + _height;

    scrollRect = InteractionRect(_posX + 4, _posY + 4, 10, _height - 8);
    scrollRect.setFillColor(sf::Color::Black);

    scrollBoundRect = InteractionRect(_posX, _posY, 18, _height);
    scrollBoundRect.setFillColor(sf::Color(170, 170, 170));

    scrollViewport = viewport;
    scrollView = sf::View(sf::FloatRect(_posX, _posY, _width, _height));
    scrollView.setViewport(viewport);
}

void ScrollContainer::updateScrollbar(InteractionRect& element)
{
    if(element.getBottom() > maxScrollY)
        maxScrollY = element.getBottom() + 10;

    if(maxScrollY > backRect.getBottom())
    {
        scrollRect.setSize(scrollRect.getWidth(), backRect.getHeight() * backRect.getHeight() / maxScrollY);
        elementsScrollMultiplier = scrollMultiplier * backRect.getHeight() / scrollRect.getHeight();
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
        if(scrollRect.getY() - scrollMultiplier * event.mouseWheelScroll.delta >= backRect.getY() &&
           scrollRect.getBottom() - scrollMultiplier * event.mouseWheelScroll.delta <= backRect.getBottom() - 4)
        {
            scrollRect.move(0, -scrollMultiplier * event.mouseWheelScroll.delta);
            for(InteractionRect *ir : elements)
                ir->move(0, elementsScrollMultiplier * event.mouseWheelScroll.delta);
        }

        return true;
    }

    if(event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Button::Left)
    {
        readInput = in(backRect, event.mouseButton);
        bool elementInput = false;
        for(InteractionRect *ir : elements)
            elementInput |= in(*ir, event.mouseButton);
        readInput = readInput & !elementInput;
        backRect.setFillColor(readInput ? activeColor : inactiveColor);
        return readInput;
    }

    return false;
}

void ScrollContainer::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    sf::View tmpView = target.getView();

    target.draw(backRect);
    target.setView(scrollView);

    target.draw(scrollBoundRect);
    target.draw(scrollRect);

    for(InteractionRect *ir : elements)
        target.draw(*ir);
    target.setView(tmpView);
}