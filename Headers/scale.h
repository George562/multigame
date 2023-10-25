#pragma once
#include "../SFML-2.5.1/include/SFML/Graphics.hpp"

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

template <class T>
Scale<T>& operator+=(Scale<T>& scale, T x) {
    scale.cur += x;
    normalize(scale);
    return scale;
}

template <class T>
Scale<T>& operator-=(Scale<T>& scale, T x) {
    return scale += -x;
}

void normalize(Scale<sf::Vector2f>& scale) {
    scale.cur.x = std::clamp(scale.cur.x, scale.bottom.x, scale.top.x);
    scale.cur.y = std::clamp(scale.cur.y, scale.bottom.y, scale.top.y);
}
