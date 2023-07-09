#include "../SFML-2.5.1/include/SFML/Graphics.hpp"
#include <vector>
struct Rect {
    float PosX, PosY, Width, Height;
    sf::Vector2f getPosition() { return {PosX, PosY}; }
    void setPosition(sf::Vector2f v) { PosX = v.x; PosY = v.y; }
    void setPosition(float x, float y) { PosX = x; PosY = y; }
    void setCenter(float x, float y) { PosX = x - Width / 2; PosY = y - Height / 2; }
    void setCenter(sf::Vector2f v) { PosX = v.x - Width / 2; PosY = v.y - Height / 2; }
    sf::Vector2f getSize() { return {Width, Height}; }
    void setSize(sf::Vector2f v) { setSize(v.x, v.y); }
    void setSize(float w, float h) { Width = w; Height = h; }
    void setRect(float x, float y, float w, float h) { PosX = x; PosY = y; Width = w; Height = h; }
    void setRect(Rect rect) { PosX = rect.PosX; PosY = rect.PosY; Width = rect.Width; Height = rect.Height; }
    bool intersect(float& x, float& y, float w, float h) {
        return x <= PosX + Width && PosX <= x + w && y <= PosY + Height && PosY <= y + h;
    }
    bool intersect(const Rect& rect) {
        return rect.PosX <= PosX + Width && PosX <= rect.PosX + rect.Width && rect.PosY <= PosY + Height && PosY <= rect.PosY + rect.Height;
    }
    bool intersect(const Rect& rect, Rect& intersection) {
        intersection.setPosition(std::max(PosX, rect.PosX), std::max(PosY, rect.PosY));
        intersection.setSize(std::min(PosX + Width, rect.PosX + rect.Width) - intersection.PosX,
                             std::min(PosY + Height, rect.PosY + rect.Height) - intersection.PosY);
        return intersect(rect);
    }
    bool contains(const float& x, const float& y) {
        return PosX <= x && x <= PosX + Width && PosY <= y && y <= PosY + Height;
    }
    bool contains(const sf::Vector2f& point) { return contains(point.x, point.y); }
};
using vr = std::vector<Rect>;
using vvr = std::vector<vr>;