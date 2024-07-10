
struct Margin {
        float left, right, top, bottom;

};

Margin operator+(Margin left, Margin right) {
    return { left.left + right.left, left.top + right.top,
             left.right + right.right, left.bottom + right.bottom };
}

Margin operator-(Margin m) {
    return { -m.left, -m.top, -m.right, -m.bottom };
}

Margin operator-(Margin left, Margin right) {
    return left + (-right);
}