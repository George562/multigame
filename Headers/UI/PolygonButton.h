#pragma once
#include "text.h"
#include "../CollisionShapes/collisionRect.h"

////////////////////////////////////////////////////////////
// Class
////////////////////////////////////////////////////////////

#pragma pack(push, 1)
class PolygonButton : public sf::Drawable {
public:
    sf::Texture* texture, *pushedTexture;
    sf::Sprite sprite;
    PlacedText ButtonText;
    bool Pushed = false, visible = true;
    void (*buttonFunction)(void);
    CollisionShape hitbox;
    sf::ConvexShape drawShape;

    PolygonButton() {}
    PolygonButton(sf::String, void (*)(void));

    sf::Vector2f getHitboxPoint(int index) { return hitbox.getPoint(index); }
    sf::Vector2f getDrawShapePoint(int index) { return drawShape.getPoint(index); }
    std::vector<sf::Vector2f> getHitboxPoints() { return hitbox.getPoints(); }
    std::vector<sf::Vector2f> getDrawShapePoints();

    void setPosition(float, float);
    void setPosition(sf::Vector2f v) { setPosition(v.x, v.y); }
    void setCenter(float x, float y) { setPosition(x - hitbox.getSize().x / 2, y - hitbox.getSize().y / 2); }
    void setCenter(sf::Vector2f v) { setCenter(v.x, v.y); }
    void setHitboxPoints(std::vector<sf::Vector2f> points, bool updateDrawShape);
    void setDrawShapePoints(std::vector<sf::Vector2f> points);
    void setFillColor(sf::Color color) { drawShape.setFillColor(color); }
    void setSpriteColor(sf::Color color) { sprite.setColor(color); }
    void setOutlineColor(sf::Color color) { drawShape.setOutlineColor(color); }
    void setOutlineThickness(int thickness) { drawShape.setOutlineThickness(thickness); }

    void setFunction(void (*func)()) { buttonFunction = func; }
    void setWord(sf::String word) { ButtonText.setString(word); ButtonText.setCenter(hitbox.getCenter()); }
    void setTexture(sf::Texture&, sf::Texture&);
    void setCharacterSize(int size) { ButtonText.setCharacterSize(size); ButtonText.setCenter(hitbox.getCenter()); }
    virtual void draw(sf::RenderTarget&, sf::RenderStates = sf::RenderStates::Default) const;
    bool OnTheButton(int& x, int& y) { return hitbox.contains(x, y); }
    bool isActivated(sf::Event&);
};
#pragma pack(pop)

////////////////////////////////////////////////////////////
// Realization
////////////////////////////////////////////////////////////

PolygonButton::PolygonButton(sf::String word, void (*foo)(void)) {
    ButtonText.setCharacterSize(150);
    ButtonText.setFillColor(sf::Color(199, 199, 199));
    setWord(word);
    buttonFunction = foo;
}

std::vector<sf::Vector2f> PolygonButton::getDrawShapePoints() {
    std::vector<sf::Vector2f> output;
    for (int i = 0; i < drawShape.getPointCount(); i++)
        output.push_back(drawShape.getPoint(i));
    return output;
}

void PolygonButton::setTexture(sf::Texture& texture, sf::Texture& pushedTexture) {
    this->texture = &texture;
    this->pushedTexture = &pushedTexture;
    sprite.setTexture(texture);
}

void PolygonButton::setPosition(float x, float y) {
    hitbox.setPosition(x, y);
	setDrawShapePoints(hitbox.getPoints());
    sprite.setPosition(x, y);
    ButtonText.setCenter(hitbox.getCenter());
}

void PolygonButton::setHitboxPoints(std::vector<sf::Vector2f> points, bool updateDrawShape = false) {
    hitbox.setPoints(points);
    if (updateDrawShape)
        setDrawShapePoints(points);
}

void PolygonButton::setDrawShapePoints(std::vector<sf::Vector2f> points) {
    drawShape.setPointCount(points.size());
    for (int i = 0; i < points.size(); i++)
        drawShape.setPoint(i, points[i]);
}

void PolygonButton::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    if (visible) {
        target.draw(sprite, states);
        target.draw(ButtonText, states);
        target.draw(drawShape, states);
    }
}

bool PolygonButton::isActivated(sf::Event& event) {
    if (Pushed && event.type == sf::Event::MouseButtonReleased) {
        Pushed = false;
        sprite.setTexture(*texture);
        if (OnTheButton(event.mouseButton.x, event.mouseButton.y)) {
            buttonFunction();
            return true;
        }
    }
    if (event.type == sf::Event::MouseButtonPressed &&
        OnTheButton(event.mouseButton.x, event.mouseButton.y) && 
        event.mouseButton.button == sf::Mouse::Button::Left) {
            Pushed = true;
            sprite.setTexture(*pushedTexture);
            return true;
        }
    return false;
}
