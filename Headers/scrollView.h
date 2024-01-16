#pragma once
#include "panel.h"
#include "interactible.h"
#include "rect.h"

class ScrollView : public sf::Drawable, public Rect {
private:
    bool readInput = false;
    int scrollMultiplier, elementsScrollMultiplier, scrollValue;
    float minScrollY, maxScrollY;

    sf::View scrollView;
    Panel background;
    Interactable scrollbar;

    std::vector<Interactable*> activeElements;
    std::vector<sf::Drawable*> passiveElements;

    void updateScrollbar(Interactable&);

public:
    ScrollView() {};
    ScrollView(float, float, float, float, sf::FloatRect, int);

    void addActive(Interactable&, int);        // These elements can be interacted with
    void addPassive(sf::Drawable&, int);       // Stuff like simple text, panels, rectangles, etc. - lack an "isActive" function
    Panel* getBG() { return &background; }
    sf::View* getView() { return &scrollView; }
    sf::FloatRect getViewport() { return scrollView.getViewport(); }

    void setViewport(sf::FloatRect newViewport) { scrollView.setViewport(newViewport); }
    void setBG(Panel& newBG) { background = newBG; }
    void updateScrollbar(int);
    bool isActivated(sf::Event&);
    void draw(sf::RenderTarget& target, sf::RenderStates states = sf::RenderStates::Default) const;
};

ScrollView::ScrollView(float posX, float posY, float width, float height, sf::FloatRect viewport, int multiplier = 10) {
    setRect(posX, posY, width, height);
    scrollMultiplier = elementsScrollMultiplier = multiplier;
    scrollValue = 0;
    minScrollY = posY; maxScrollY = posY + height;

    scrollView = sf::View(sf::FloatRect(posX, posY, width, height));
    scrollView.setViewport(viewport);
}

void ScrollView::updateScrollbar(int lastBottom) {
    if(lastBottom > maxScrollY)
        maxScrollY = lastBottom + 10;

    if(maxScrollY > background.getPosition().y + background.sprite.getGlobalBounds().height) {
        scrollbar.setSize(scrollbar.getSize().x, std::pow(background.sprite.getGlobalBounds().height, 2.f) / maxScrollY);
        elementsScrollMultiplier = scrollMultiplier * background.sprite.getGlobalBounds().height / scrollbar.getSize().y;
    }
}

void ScrollView::addActive(Interactable& element, int bottom) {
    activeElements.push_back(&element);
    updateScrollbar(bottom);
}

void ScrollView::addPassive(sf::Drawable& element, int bottom) {
    passiveElements.push_back(&element);
    updateScrollbar(bottom);
}

bool ScrollView::isActivated(sf::Event& event) {
    if(event.type == sf::Event::MouseWheelScrolled && readInput) {
        if(scrollbar.getPosition().y - scrollMultiplier * event.mouseWheelScroll.delta >= background.getPosition().y &&
           scrollbar.getBottom() - scrollMultiplier * event.mouseWheelScroll.delta <= background.getPosition().y + background.sprite.getGlobalBounds().height - 4) {
            scrollbar.move(0, -scrollMultiplier * event.mouseWheelScroll.delta);
            for(Interactable *el : activeElements)
                el->move(0, elementsScrollMultiplier * event.mouseWheelScroll.delta);
        }

        return true;
    }

    if(event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Button::Left) {
        sf::Vector2f mousePos = sf::Vector2f(event.mouseButton.x, event.mouseButton.y);
        readInput = background.sprite.getGlobalBounds().contains(mousePos);
        bool elementInput = false;
        for(Interactable *el : activeElements)
            elementInput |= el->contains(mousePos);
        readInput = readInput & !elementInput;
        return readInput;
    }

    return false;
}

void ScrollView::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    sf::View tmpView = target.getView();

    target.setView(scrollView);
    target.draw(background);

    for(sf::Drawable *el : passiveElements)
        target.draw(*el);

    for(Interactable *el : activeElements)
        target.draw(*el);

    target.setView(tmpView);
}