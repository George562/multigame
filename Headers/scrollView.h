#pragma once
#include "panel.h"
#include "interactible.h"

class ScrollView
{
private:
    bool readInput = false;
    int scrollMultiplier, elementsScrollMultiplier, scrollValue;
    float posX, posY, width, height, minScrollY, maxScrollY;

    sf::View scrollView;
    sf::FloatRect scrollViewport;
    Panel background;
    Interactable scrollbar;

    std::vector<Interactable*> activeElements;
    std::vector<sf::Drawable*> passiveElements;

    void updateScrollbar(Interactable&);

public:
    ScrollView() {};
    ScrollView(float, float, float, float, sf::FloatRect, int);
    
    sf::Vector2f getPosition() { return sf::Vector2f(posX, posY); }
    sf::Vector2f getSize() { return sf::Vector2f(width, height); }
    void addActive(Interactable&, int);        // These elements can be interacted with
    void addPassive(sf::Drawable&, int);       // Stuff like simple text, panels, rectangles, etc. - lack an "isActive" function
    Panel* getBG() { return &background; }
    sf::View* getView() { return &scrollView; }
    sf::FloatRect getViewport() { return scrollViewport; }

    void setViewport(sf::FloatRect newViewport) { scrollViewport = newViewport; scrollView.setViewport(newViewport); }
    void setBG(Panel& newBG) { background = newBG; }
    void updateScrollbar(int);
    bool isActivated(sf::Event&);
    void draw(sf::RenderTarget& target, sf::RenderStates states) const;
};

ScrollView::ScrollView(float _posX, float _posY, float _width, float _height, 
                       sf::FloatRect viewport, int multiplier = 10)
{
    posX = _posX; posY = _posY; width = _width; height = _height;
    scrollMultiplier = elementsScrollMultiplier = multiplier;
    scrollValue = 0;
    minScrollY = _posY; maxScrollY = _posY + _height;

    scrollViewport = viewport;
    scrollView = sf::View(sf::FloatRect(_posX, _posY, _width, _height));
    scrollView.setViewport(scrollViewport);
}

void ScrollView::updateScrollbar(int lastBottom)
{
    if(lastBottom > maxScrollY)
        maxScrollY = lastBottom + 10;

    if(maxScrollY > background.getPosition().y + background.sprite.getGlobalBounds().height)
    {
        scrollbar.setSize(scrollbar.getSize().x, background.sprite.getGlobalBounds().height * background.sprite.getGlobalBounds().height/ maxScrollY);
        elementsScrollMultiplier = scrollMultiplier * background.sprite.getGlobalBounds().height / scrollbar.getSize().y;
    }
}

void ScrollView::addActive(Interactable& element, int bottom)
{
    activeElements.push_back(&element);
    updateScrollbar(bottom);
}

void ScrollView::addPassive(sf::Drawable& element, int bottom)
{
    passiveElements.push_back(&element);
    updateScrollbar(bottom);
}

bool ScrollView::isActivated(sf::Event& event)
{
    if(event.type == sf::Event::MouseWheelScrolled && readInput)
    {
        if(scrollbar.getPosition().y - scrollMultiplier * event.mouseWheelScroll.delta >= background.getPosition().y &&
           scrollbar.getBottom() - scrollMultiplier * event.mouseWheelScroll.delta <= background.getPosition().y + background.sprite.getGlobalBounds().height - 4)
        {
            scrollbar.move(0, -scrollMultiplier * event.mouseWheelScroll.delta);
            for(Interactable *el : activeElements)
                el->move(0, elementsScrollMultiplier * event.mouseWheelScroll.delta);
        }

        return true;
    }

    if(event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Button::Left)
    {
        sf::Vector2f mousePos = sf::Vector2f(event.mouseButton.x, event.mouseButton.y);
        readInput = mousePos.x >= background.getPosition().x && mousePos.y >= background.getPosition().y &&
                    mousePos.x <= background.getPosition().x + background.sprite.getGlobalBounds().width &&
                    mousePos.y <= background.getPosition().y + background.sprite.getGlobalBounds().height;
        bool elementInput = false;
        for(Interactable *el : activeElements)
            elementInput |= el->contains(mousePos);
        readInput = readInput & !elementInput;
        return readInput;
    }

    return false;
}

void ScrollView::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    sf::View tmpView = target.getView();
    
    target.setView(scrollView);
    target.draw(background);

    for(sf::Drawable *el : passiveElements)
        target.draw(*el);

    for(Interactable *el : activeElements)
        target.draw(*el);
    
    target.setView(tmpView);
}