#pragma once
#include "init.h"

template <typename T>
bool in(std::vector<T>& arr, T x) {
    for (T& y: arr) if (y == x) return true;
    return false;
}

template <typename T>
bool DeleteFromVector(std::vector<T>& arr, T x) {
    for (size_t i = 0; i < arr.size(); i++)
        if (arr[i] == x) {
            arr.erase(arr.begin() + i);
            return true;
        }
    return false;
}

// {x = 1, y = -1} => collision at the y, up or down doesn't matter, because u know "dy" already
sf::Vector2i WillCollisionWithWalls(vvr& Walls, float& PosX, float& PosY, float& Width, float& Height, float& dx, float& dy) {
    sf::Vector2i res = {-1, -1};
    int y = int(PosY) / size, x = int(PosX) / size;
    int WallHight = Walls.size(), WallLength = Walls[0].size();
    float nextX = PosX + dx, nextY = PosY + dy;

    if (dy < 0) {
        if ((y * 2 - 1 < 0 || !Walls[y * 2 - 1][x].intersect(PosX, nextY, Width, Height)) &&
            (!Walls[y * 2][x].intersect(PosX, nextY, Width, Height)) &&
            (y * 2 - 1 < 0 || !Walls[y * 2 - 1][x + 1].intersect(PosX, nextY, Width, Height)) &&
            (x + 1 >= WallLength || !Walls[y * 2][x + 1].intersect(PosX, nextY, Width, Height)))
            res.y = 1;
    }
    if (dx < 0) {
        if ((x - 1 < 0 || !Walls[y * 2][x - 1].intersect(nextX, PosY, Width, Height)) &&
            (!Walls[y * 2 + 1][x].intersect(nextX, PosY, Width, Height)) &&
            (x - 1 < 0 || !Walls[y * 2 + 2][x - 1].intersect(nextX, PosY, Width, Height)) &&
            (y * 2 + 3 >= WallHight || !Walls[y * 2 + 3][x].intersect(nextX, PosY, Width, Height)))
            res.x = 1;
    }
    if (dy > 0) {
        if ((y * 2 + 3 >= WallHight || !Walls[y * 2 + 3][x].intersect(PosX, nextY, Width, Height)) &&
            (!Walls[y * 2 + 2][x].intersect(PosX, nextY, Width, Height)) &&
            (y * 2 + 3 >= WallHight || !Walls[y * 2 + 3][x + 1].intersect(PosX, nextY, Width, Height)) &&
            (x + 1 >= WallLength || !Walls[y * 2 + 2][x + 1].intersect(PosX, nextY, Width, Height)))
            res.y = 1;
    }
    if (dx > 0) {
        if ((x + 1 >= WallLength || !Walls[y * 2][x + 1].intersect(nextX, PosY, Width, Height)) &&
            (!Walls[y * 2 + 1][x + 1].intersect(nextX, PosY, Width, Height)) &&
            (x + 1 >= WallLength || !Walls[y * 2 + 2][x + 1].intersect(nextX, PosY, Width, Height)) &&
            (y * 2 + 3 >= WallHight || !Walls[y * 2 + 3][x + 1].intersect(nextX, PosY, Width, Height)))
            res.x = 1;
    }
    return res;  // if value of vector == -1 => there was collision
}
// {x = 1, y = -1} => collision at the y, up or down doesn't matter, because u know "dy" already
sf::Vector2i WillCollisionWithWalls(vvr& Walls, Rect& rect, sf::Vector2f d) {
    return WillCollisionWithWalls(Walls, rect.PosX, rect.PosY, rect.Width, rect.Height, d.x, d.y);
}

void RotateOn(float phi, float& x, float& y) {
    float oldX = x, OldY = y;
    x =   oldX * cos(phi) + OldY * sin(phi);
    y = - oldX * sin(phi) + OldY * cos(phi);
}
sf::Vector2f RotateOn(float phi, sf::Vector2f& a) {
    sf::Vector2f newA;
    newA.x =   a.x * cos(phi) + a.y * sin(phi);
    newA.y = - a.x * sin(phi) + a.y * cos(phi);
    return newA;
}

void RotateAround(float phi, float& x, float& y, float& X, float& Y) {
    float oldX = x, OldY = y;
    x =   (oldX - X) * cos(phi) + (OldY - Y) * sin(phi) + X;
    y = - (oldX - X) * sin(phi) + (OldY - Y) * cos(phi) + Y;
}
sf::Vector2f RotateAround(float phi, sf::Vector2f& a, float& X, float& Y) {
    sf::Vector2f newA;
    newA.x =   (a.x - X) * cos(phi) + (a.y - Y) * sin(phi) + X;
    newA.y = - (a.x - X) * sin(phi) + (a.y - Y) * cos(phi) + Y;
    return newA;
}

float distance(sf::Vector2f a, sf::Vector2f b) { return std::hypot(a.x - b.x, a.y - b.y); }
float LenOfVector(sf::Vector2f v) { return std::hypot(v.x, v.y); }

sf::Vector2f operator*(sf::Vector2f& a, sf::Vector2f& b) {
    return sf::Vector2f{a.x * b.x, a.y * b.y};
}

sf::Packet& operator<<(sf::Packet& packet, Point& a) {
    return packet << a.x << a.y;
}
sf::Packet& operator>>(sf::Packet& packet, Point& a) {
    return packet >> a.x >> a.y;
}

sf::Packet& operator<<(sf::Packet& packet, sf::Color& a) {
    return packet << a.r << a.g << a.b << a.a;
}
sf::Packet& operator>>(sf::Packet& packet, sf::Color& a) {
    return packet >> a.r >> a.g >> a.b >> a.a;
}
