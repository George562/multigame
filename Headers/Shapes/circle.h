#pragma once
#include "../../SFML-2.5.1/include/SFML/Network.hpp"
#include "shape.h"

class EllipseShape : public sf::ConvexShape
{
public :

    explicit EllipseShape(const float& radius = 0.f) :
    m_radius(radius)
    {
        update();
    }

    void setRadius(const float& radius)
    {
        m_radius = radius;
        update();
    }

    const float& getRadius() const
    {
        return m_radius;
    }

    virtual std::size_t getPointCount() const
    {
        return 30; // fixed, but could be an attribute of the class if needed
    }

    virtual sf::Vector2f getPoint(std::size_t index) const
    {
        static const float pi = 3.141592654f;

        float angle = index * 2 * pi / getPointCount() - pi / 2;
        float x = std::cos(angle) * m_radius;
        float y = std::sin(angle) * m_radius;

        return sf::Vector2f(m_radius + x, m_radius + y);
    }

private :

    float m_radius;
};