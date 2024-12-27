#pragma once
#include "PlacedText.h"
#include "../CollisionShapes/collisionRect.h"
#include "../Abstracts/UIElement.h"
#include "../Utility/commonShapes.h"

////////////////////////////////////////////////////////////
// Class
////////////////////////////////////////////////////////////

#pragma pack(push, 1)
class PolygonButton : public UIElement {
public:
    sf::Texture *texture, *pushedTexture;
    PlacedText ButtonText;
    CollisionShape hitbox;
    sf::Sprite sprite;
    sf::ConvexShape shape;
    bool showShape = false;
    bool Pushed = false;
    void (*buttonFunction)(void);

    PolygonButton() {}
    PolygonButton(std::string, FontString = "", void (*)(void) = nullptr);
    PolygonButton(std::string, UI::Anchor, UI::Anchor, sf::Vector2f = { 0, 0 }, FontString = FontString(""), void (*)(void) = nullptr);
    PolygonButton(std::string, sf::FloatRect, FontString = FontString(""), void (*)(void) = nullptr);

    virtual sf::FloatRect getGlobalBounds() { return hitbox.getBounds(); };

    void setPosition(float, float);

    void setFunction(void (*func)()) { buttonFunction = func; }
    void setWord(FontString word) { ButtonText.setFontString(word); ButtonText.setCenter(getCenter()); }
    void setFillColor(sf::Color color) { ButtonText.setFillColor(color); }
    void setTexture(sf::Texture&, sf::Texture&, UI::TextureResize, sf::Vector2u);
    void setSpriteSize(sf::Vector2u);
    void setSpriteColor(sf::Color color) { sprite.setColor(color); }
    void setCharacterSize(int size) { ButtonText.setCharacterSize(size); ButtonText.setCenter(getCenter()); }
    void setHitboxPoints(std::vector<sf::Vector2f> points) {
        hitbox.setPoints(points);
        hitbox.move(getPosition());
        setSize((sf::Vector2u)hitbox.getSize());
    }
    void setShapeThickness(int thickness) { shape.setOutlineThickness(thickness); }
    void setShapeFillColor(sf::Color color) { shape.setFillColor(color); }
    void setShapeOutlineColor(sf::Color color) { shape.setOutlineColor(color); }
    void setShape(sf::Color color, sf::Color outlineColor, int thickness) {
        shape.setFillColor(color);
        shape.setOutlineColor(outlineColor);
        setShapeThickness(thickness);
    }
    void enableShape(bool updateToHitbox = false) {
        showShape = true;
        if (updateToHitbox) setConvexShape(shape, hitbox.getPoints());
    }

    virtual void draw(sf::RenderTarget&, sf::RenderStates = sf::RenderStates::Default) const;
    bool OnTheButton(int& x, int& y) { return hitbox.contains(x, y); }
    bool isActivated(sf::Event&);
};
#pragma pack(pop)

////////////////////////////////////////////////////////////
// Realization
////////////////////////////////////////////////////////////

PolygonButton::PolygonButton(std::string name, FontString word, void (*foo)(void)) {
    setName(name);
    setWord(word);
    buttonFunction = foo;
}

PolygonButton::PolygonButton(std::string name, UI::Anchor anchor, UI::Anchor anchoringPoint,
                             sf::Vector2f size, FontString word, void (*foo)(void))
    : PolygonButton(name, word, foo) {
    setAnchors(anchor, anchoringPoint);
    setSize(size.x, size.y);
}

PolygonButton::PolygonButton(std::string name, sf::FloatRect rect, FontString word, void (*foo)(void))
    : PolygonButton(name, UI::none, UI::none, { rect.width, rect.height }, word, foo) {
    setPosition(rect.left, rect.top);
}

void PolygonButton::setTexture(sf::Texture& texture, sf::Texture& pushedTexture,
                               UI::TextureResize mode = UI::element, sf::Vector2u size = {0, 0}) {
    this->texture = &texture;
    this->pushedTexture = &pushedTexture;
    if (texture.getSize() == sf::Vector2u{0, 0}) {
        sprite.setScale(0, 0);
        setSize(0, 0);
        return;
    }
    else sprite.setTexture(texture, true);
    if (mode == UI::element)
        setSpriteSize(getSize());
    else if (mode == UI::texture)
        setSpriteSize(texture.getSize());
    else if (mode == UI::size && size != sf::Vector2u{0, 0})
        setSpriteSize(size);
}
void PolygonButton::setSpriteSize(sf::Vector2u size) {
    if (sprite.getScale().x == 0 || sprite.getScale().y == 0)
        sprite.setScale(sf::Vector2f(1.0, 1.0));
    sprite.setScale(sf::Vector2f{
            float(size.x) / sprite.getTexture()->getSize().x,
            float(size.y) / sprite.getTexture()->getSize().y
    });
    setSize(size);
}

void PolygonButton::setPosition(float x, float y) {
    UIElement::setPosition(x, y);
    hitbox.setPosition(x, y);
	setConvexShape(shape, hitbox.getPoints());
    ButtonText.setCenter(getCenter());
}

void PolygonButton::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    states.transform *= getTransform();
    if (showShape) target.draw(shape);
    target.draw(sprite, states);
    target.draw(ButtonText);
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
    if (mouseButtonPressed(event, sf::Mouse::Left) && OnTheButton(event.mouseButton.x, event.mouseButton.y)) {
            Pushed = true;
            sprite.setTexture(*pushedTexture);
            return true;
        }
    return false;
}


class RectButton : public PolygonButton {
public:
    RectButton(std::string name, FontString word = "", void (*foo)(void) = nullptr) : PolygonButton(name, word, foo) {}

    RectButton(std::string name, UI::Anchor anchor, UI::Anchor anchoringPoint,
        sf::Vector2f size = { 0, 0 }, FontString word = "", void (*foo)(void) = nullptr)
        : PolygonButton(name, word, foo) {
        setAnchors(anchor, anchoringPoint);
        setSize(size.x, size.y);
        if (size != sf::Vector2f(0, 0))
            setHitboxPoints({
                { getLeftTop() },
                { getRightTop() },
                { getRightBottom() },
                { getLeftBottom() }
            }
        );
    }

    RectButton(std::string name, sf::FloatRect rect, FontString word = "", void (*foo)(void) = nullptr)
        : RectButton(name, UI::none, UI::none, { rect.width, rect.height }, word, foo) {
        setPosition(rect.left, rect.top);
    }
};

// class provide monitoring for holding buttons
class ButtonsHolder {
public:
    std::vector<PolygonButton*> buttons;

    ButtonsHolder() {}
    ButtonsHolder(std::vector<PolygonButton*> buttons) : buttons(buttons) {}

    void addButton(PolygonButton* button) { buttons.push_back(button); }
    void activateButton(int index) {
        buttons[index]->buttonFunction();
        buttons[index]->sprite.setTexture(*(buttons[index]->pushedTexture));
    }
    bool isButtonActive(PolygonButton* button, sf::Event& event) {
        if (button->isActivated(event)) {
            button->sprite.setTexture(*(button->pushedTexture));
            for (PolygonButton* b : buttons) {
                if (b != button) {
                    b->sprite.setTexture(*(b->texture));
                }
            }
            return true;
        }
        return false;
    }

    bool isButtonsActive(sf::Event& event) {
        for (PolygonButton* b : buttons) {
            if (isButtonActive(b, event)) {
                return true;
            }
        }
        return false;
    }
};