#pragma once
#include "init.h"

void CreateImage() {
    sf::Image img, res;
    img.loadFromFile("sources/textures/floor1x.png");
    res.create(32 * 100, 32 * 100);
    for (int y = 0; y < 100; y++) {
        for (int x = 0; x < 100; x++) {
            res.copy(img, x * 32, y * 32, {(rand() % 5) * 32, 0, 32, 32});
        }
    }
    res.saveToFile("sources/other/res.png");
}

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
template <typename Obj>
sf::Vector2i WillCollisionWithWalls(vvr& Walls, Obj& obj, sf::Vector2f Velocity) {
    int y = int(obj.PosY) / size, x = int(obj.PosX) / size;
    sf::Vector2i res = {-1, -1};

    obj.PosY += Velocity.y;
    if (Velocity.y < 0) {
        if ((y * 2 - 1 < 0 || !Walls[y * 2 - 1][x].intersect(obj)) &&
            (!Walls[y * 2][x].intersect(obj)) &&
            (y * 2 - 1 < 0 || !Walls[y * 2 - 1][x + 1].intersect(obj)) &&
            (x + 1 >= Walls[0].size() || !Walls[y * 2][x + 1].intersect(obj)) &&
            (x - 1 < 0 || !Walls[y * 2][x - 1].intersect(obj)))
            res.y = 1;
    }
    if (Velocity.y > 0) {
        if ((y * 2 + 3 >= Walls.size() || !Walls[y * 2 + 3][x].intersect(obj)) &&
            (!Walls[y * 2 + 2][x].intersect(obj)) &&
            (y * 2 + 3 >= Walls.size() || !Walls[y * 2 + 3][x + 1].intersect(obj)) &&
            (x + 1 >= Walls[0].size() || !Walls[y * 2 + 2][x + 1].intersect(obj)) &&
            (x - 1 < 0 || !Walls[y * 2 + 2][x - 1].intersect(obj)))
            res.y = 1;
    }
    obj.PosY -= Velocity.y;
    obj.PosX += Velocity.x;
    if (Velocity.x < 0) {
        if ((x - 1 < 0 || !Walls[y * 2][x - 1].intersect(obj)) &&
            (!Walls[y * 2 + 1][x].intersect(obj)) &&
            (x - 1 < 0 || !Walls[y * 2 + 2][x - 1].intersect(obj)) &&
            (y * 2 + 3 >= Walls.size() || !Walls[y * 2 + 3][x].intersect(obj)) &&
            (y * 2 - 1 < 0 || !Walls[y * 2 - 1][x].intersect(obj)))
            res.x = 1;
    }
    if (Velocity.x > 0) {
        if ((x + 1 >= Walls[0].size() || !Walls[y * 2][x + 1].intersect(obj)) &&
            (!Walls[y * 2 + 1][x + 1].intersect(obj)) &&
            (x + 1 >= Walls[0].size() || !Walls[y * 2 + 2][x + 1].intersect(obj)) &&
            (y * 2 + 3 >= Walls.size() || !Walls[y * 2 + 3][x + 1].intersect(obj)) &&
            (y * 2 - 1 < 0 || !Walls[y * 2 - 1][x + 1].intersect(obj)))
            res.x = 1;
    }
    obj.PosX -= Velocity.x;
    return res;  // if value of vector == -1 => there was collision
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

sf::Vector2f operator*(sf::Vector2f a, sf::Vector2f b) {
    return sf::Vector2f{a.x * b.x, a.y * b.y};
}

sf::Packet& operator<<(sf::Packet& packet, sf::Vector2i& a) {
    return packet << a.x << a.y;
}
sf::Packet& operator>>(sf::Packet& packet, sf::Vector2i& a) {
    return packet >> a.x >> a.y;
}

sf::Packet& operator<<(sf::Packet& packet, sf::Color& a) {
    return packet << a.r << a.g << a.b << a.a;
}
sf::Packet& operator>>(sf::Packet& packet, sf::Color& a) {
    return packet >> a.r >> a.g >> a.b >> a.a;
}
