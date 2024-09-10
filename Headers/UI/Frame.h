#pragma once
#include "../Abstracts/UIElement.h"

class Frame : public UIElement {
public:
    sf::Sprite sprite;
    sf::ConvexShape shape;

    Frame() {}

    Frame(std::string name) {
        setName(name);
    }

    Frame(std::string name, UI::Anchor anchor, UI::Anchor anchoringPoint, sf::Vector2f size) : Frame(name) {
        setAnchors(anchor, anchoringPoint);
        setSize(size.x, size.y);
    }

    Frame(std::string name, sf::FloatRect rect) : Frame(name, UI::none, UI::none, { rect.width, rect.height }) {
        setPosition(rect.left, rect.top);
    }

    void setPosition(float x, float y) {
        UIRect::setPosition(x, y);
        sprite.setPosition(getPosition());
        shape.setPosition(getPosition());
    }

    void setTexture(sf::Texture& texture, UI::TextureResize mode = UI::element, sf::Vector2u size = { 0, 0 }) {
        if (texture.getSize() == sf::Vector2u{0, 0}) {
            sprite.setScale(0, 0);
            setSize(0, 0);
            return;
        }
        else sprite.setTexture(texture, true);
        sprite.setScale(1, 1);
        if (mode == UI::element)
            setSpriteSize(getSize());
        else if (mode == UI::texture)
            setSpriteSize(texture.getSize());
        else if (mode == UI::size && size != sf::Vector2u{0, 0})
            setSpriteSize(size);
    }

    void setSpriteSize(sf::Vector2u size) {
        if (sprite.getScale().x == 0 || sprite.getScale().y == 0)
            sprite.setScale(1.0, 1.0);
        sprite.setScale(
            float(size.x) / sprite.getTexture()->getSize().x,
            float(size.y) / sprite.getTexture()->getSize().y
        );
        setSize(size);
    }

    void setSpriteColor(sf::Color color) { sprite.setColor(color); }

    void draw(sf::RenderTarget& target, sf::RenderStates states) const {
        target.draw(sprite, states);
        target.draw(shape, states);
    }
};