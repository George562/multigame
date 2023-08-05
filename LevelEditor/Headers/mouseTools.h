#pragma once
#include "../../SFML-2.5.1/include/SFML/Graphics.hpp"

bool in(float posX, float posY, int width, int height, sf::Vector2f pos)
{
    return pos.x >= posX && pos.x <= posX + width && pos.y >= posY && pos.y <= posY + height;
}

bool in(float posX, float posY, int width, int height, sf::Vector2i pos)
{
    return pos.x >= posX && pos.x <= posX + width && pos.y >= posY && pos.y <= posY + height;
}

bool in(float posX, float posY, int width, int height, sf::Event::MouseButtonEvent& event)
{
    return event.x >= posX && event.x <= posX + width && event.y >= posY && event.y <= posY + height;
}

bool in(float posX, float posY, int width, int height, sf::Event::MouseMoveEvent& event)
{
    return event.x >= posX && event.x <= posX + width && event.y >= posY && event.y <= posY + height;
}

bool in(float posX, float posY, int width, int height, sf::Event::MouseWheelScrollEvent& event)
{
    return event.x >= posX && event.x <= posX + width && event.y >= posY && event.y <= posY + height;
}