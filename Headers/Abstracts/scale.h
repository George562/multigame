#pragma once
#include "../../SFML-2.5.1/include/SFML/Graphics.hpp"
#include "../../nlohjson/json.hpp"
using nlohmann::json;

////////////////////////////////////////////////////////////
/// \param bottom smallest value
/// \param top highest value
/// \param cur current value
////////////////////////////////////////////////////////////
template <class T>
class Scale {
public:
    T bottom, top, cur;
    bool looped = false;
    T fromTop() { return top - cur; }
    T toBottom() { return cur - bottom; }
    float filling() { return float(cur - bottom) / float(top - bottom); };
    Scale<T>& operator=(T x) {
        this->cur = x; normalize(*this);
        return *this;
    }
    Scale<T>& operator=(std::vector<T> vals) {
        this->bottom = vals[0];
        this->top = vals[1];
        this->cur = vals[2];

        return *this;
    }

    Scale<T>& operator=(Scale<T> scale) {
        this->bottom = scale.bottom;
        this->top = scale.top;
        this->cur = scale.cur;
        
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
void to_json(json& j, const Scale<T>& s) {
	j = {
		{"bottom", s.bottom},
		{"top", s.top},
		{"cur", s.cur},
		{"looped", s.looped}
	};
}

template <class T>
void from_json(const json& j, Scale<T>& s) {
	j.at("bottom").get_to(s.bottom);
	j.at("top").get_to(s.top);
	j.at("cur").get_to(s.cur);
	j.at("looped").get_to(s.looped);
}

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

void normalize(Scale<sf::Vector2f>& scale) {
    scale.cur.x = std::clamp(scale.cur.x, scale.bottom.x, scale.top.x);
    scale.cur.y = std::clamp(scale.cur.y, scale.bottom.y, scale.top.y);
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

template <class T>
std::ostream& operator<<(std::ostream& stream, Scale<T>& a) {
    return stream << a.bottom << ' ' << a.top<< ' '  << a.cur;
}
template <class T>
std::istream& operator>>(std::istream& stream, Scale<T>& a) {
    return stream >> a.bottom >> a.top >> a.cur;
}