template <class T>
struct Scale {
    T bottom, top, cur;
    T fromTop() { return top - cur; }
    T toBottom() { return cur - bottom; }
    float filling() { return float(cur - bottom) / float(top - bottom); };
};

template <class T>
void normalize(Scale<T>& scale) {
    if (scale.cur > scale.top)    scale.cur = scale.top;
    if (scale.cur < scale.bottom) scale.cur = scale.bottom;
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
