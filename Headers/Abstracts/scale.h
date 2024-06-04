#pragma once
#include "../../SFML-2.5.1/include/SFML/Graphics.hpp"

////////////////////////////////////////////////////////////
/// \param bottom smallest value
/// \param top highest value
/// \param cur current value
////////////////////////////////////////////////////////////
template <class T>
struct Scale {
    T bottom, top, cur;
    bool looped = false;
    T fromTop() { return top - cur; }
    T toBottom() { return cur - bottom; }
    float filling() { return float(cur - bottom) / float(top - bottom); };
    Scale<T>& operator=(T x) {
        this->cur = x; normalize(*this);
        return *this;
    }
    void setScale(T bottom, T top, T cur) {
        this->bottom = bottom;
        this->top = top;
        this->cur = cur;
        normalize(*this);
    }   
};

template <class T>
void normalize(Scale<T>& scale) {
    if (!scale.looped) {
        scale.cur = std::clamp(scale.cur, scale.bottom, scale.top);
    } else {
        if (scale.cur > scale.top) {
            scale.cur = scale.cur - (scale.top - scale.bottom + 1);
        } else if (scale.cur < scale.bottom) {
            scale.cur = scale.cur + (scale.top - scale.bottom + 1);
        }
    }
}

template <class T, class Q>
Scale<T>& operator+=(Scale<T>& scale, Q x) {
    scale.cur += x;
    normalize(scale);
    return scale;
}

template <class T, class Q>
Scale<T>& operator-=(Scale<T>& scale, Q x) {
    return scale += -x;
}

void normalize(Scale<sf::Vector2f>& scale) {
    scale.cur.x = std::clamp(scale.cur.x, scale.bottom.x, scale.top.x);
    scale.cur.y = std::clamp(scale.cur.y, scale.bottom.y, scale.top.y);
}

template <class T>
std::ostream& operator<<(std::ostream& stream, Scale<T>& a) {
    return stream << a.bottom << ' ' << a.top<< ' '  << a.cur;
}
template <class T>
std::istream& operator>>(std::istream& stream, Scale<T>& a) {
    return stream >> a.bottom >> a.top >> a.cur;
}