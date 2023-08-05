////////////////////////////////////////////////////////////
/// \param bottom smallest value
/// \param top highest value
/// \param cur current value
////////////////////////////////////////////////////////////
template <class T>
struct Scale {
    T bottom, top, cur;
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
    scale.cur = std::max(std::min(scale.top, scale.cur), scale.bottom);
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
