#pragma once
#include "init.h"
#include "location.h"

// check existing element in vector
template <typename T>
bool in(const std::vector<T>& arr, const T& x) {
    for (const T& y: arr) if (y == x) return true;
    return false;
}

bool in(const std::vector<Rect>& arr, const float x, const float y, const float w = 0, const float h = 0) {
    for (const Rect& t: arr)
        if (t.PosX <= x + w && x <= t.PosX + t.Width && t.PosY <= y + h && y <= t.PosY + t.Height)
            return true;
    return false;
}
// Rect in Rect{x, y, w, h}
bool in(const Rect& t, const float& x, const float& y, const float& w = 0, const float& h = 0) {
    return t.PosX <= x + w && x <= t.PosX + t.Width && t.PosY <= y + h && y <= t.PosY + t.Height;
}
// Rect in Rect
bool in(const Rect& a, const Rect& b) {
    return a.PosX <= b.PosX + b.Width && b.PosX <= a.PosX + a.Width && a.PosY <= b.PosY + b.Height && b.PosY <= a.PosY + a.Height;
}
// Point in Rect
bool in(const sf::Vector2f& a, const float& x, const float& y, const float& w = 0, const float& h = 0) {
    return a.x <= x + w && x <= a.x && a.y <= y + h && y <= a.y;
}
bool in(const float px, const float py, const float& left, const float& top, const float& w = 0, const float& h = 0) {
    return px <= left + w && left <= px && py <= top + h && top <= py;
}

// {x = 1, y = -1} => collision at the y, up or down doesn't matter, because u know "dy" already
sf::Vector2i WillCollisionWithWalls(vvr Walls, float& PosX, float& PosY, float& Width, float& Height, float& dx, float& dy) {
    sf::Vector2i res = {-1, -1};
    int y = (int(PosY) / size) * 2, x = int(PosX) / size;
    int WallHight = Walls.size(), WallLength = Walls[0].size();
    if (0 > y || y > WallHight || 0 > x || x > WallLength) return res;

    float nextX = PosX + dx, nextY = PosY + dy;

    if (dy < 0) {
        if ((y <= 0 || !in(Walls[y - 1][x],              PosX, nextY, Width, Height)) &&
            (!in(Walls[y][x + 1],                        PosX, nextY, Width, Height)) &&
            (y <= 0 || !in(Walls[y - 1][x + 2],          PosX, nextY, Width, Height)) &&
            (x + 3 >= WallLength || !in(Walls[y][x + 3], PosX, nextY, Width, Height)))
            res.y = 1;
    }
    if (dx < 0) {
        if ((x <= 0 || !in(Walls[y][x - 1],             nextX, PosY, Width, Height)) &&
            (!in(Walls[y + 1][x],                       nextX, PosY, Width, Height)) &&
            (x <= 0 || !in(Walls[y + 2][x - 1],         nextX, PosY, Width, Height)) &&
            (y + 3 >= WallHight || !in(Walls[y + 3][x], nextX, PosY, Width, Height)))
            res.x = 1;
    }
    if (dy > 0) {
        if ((y + 3 >= WallHight || !in(Walls[y + 3][x],      PosX, nextY, Width, Height)) &&
            (!in(Walls[y + 2][x + 1],                        PosX, nextY, Width, Height)) &&
            (y + 3 >= WallHight || !in(Walls[y + 3][x + 2],  PosX, nextY, Width, Height)) &&
            (x + 3 >= WallLength || !in(Walls[y + 2][x + 3], PosX, nextY, Width, Height)))
            res.y = 1;
    }
    if (dx > 0) {
        if ((x + 3 >= WallLength || !in(Walls[y][x + 3],     nextX, PosY, Width, Height)) &&
            (!in(Walls[y + 1][x + 2],                        nextX, PosY, Width, Height)) &&
            (x + 3 >= WallLength || !in(Walls[y + 2][x + 3], nextX, PosY, Width, Height)) &&
            (y + 3 >= WallHight || !in(Walls[y + 3][x + 2],  nextX, PosY, Width, Height)))
            res.x = 1;
    }
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

void CreateMapRectByLocation(Location& arr, vvr& wallsRect, std::vector<sf::Sprite>& Sprites) {
    Sprites.clear();
    wallsRect.assign(arr.data.size(), vr(0));
    for (int i = 0; i < arr.data.size(); i++)
        for (int j = 0; j < arr[i].size(); j++)
            if (i % 2 == 1) // |
                wallsRect[i].push_back(Rect{size * j - WallMinSize / 2, float(size * i / 2) - WallMaxSize / 2, WallMinSize, WallMaxSize});
            else // -
                wallsRect[i].push_back(Rect{float(size * j), size * i / 2 - WallMinSize / 2, WallMaxSize, WallMinSize});
    // wallsRect[i][j].setRect(-1, -1, -1, -1);
    // if (arr[i][j] == Tiles::box) {
    //     sf::Texture* tempTexture = new sf::Texture;
    //     tempTexture->loadFromFile("sources/Box.png");
    //     sf::Sprite* tempSprite = new sf::Sprite;
    //     tempSprite->setTexture(*tempTexture);
    //     tempSprite->setScale(250 / tempSprite->getGlobalBounds().width, 250 / tempSprite->getGlobalBounds().height);
    //     tempSprite->setPosition(size * j / 2, size * i / 2);
    //     Sprites.push_back(*tempSprite);
    // }
}

Rect& operator+=(Rect& a, Point& b) { a.PosX += b.x; a.PosY += b.y; return a; } // summ position

sf::Packet& operator<<(sf::Packet& packet, Point& a) {
    return packet << a.x << a.y;
}
sf::Packet& operator>>(sf::Packet& packet, Point& a) {
    return packet >> a.x >> a.y;
}

sf::Packet& operator<<(sf::Packet& packet, Rect& a) {
    return packet << a.PosX << a.PosY << a.Width << a.Height;
}
sf::Packet& operator>>(sf::Packet& packet, Rect& a) {
    return packet >> a.PosX >> a.PosY >> a.Width >> a.Height;
}

sf::Packet& operator<<(sf::Packet& packet, sf::Color& a) {
    return packet << a.r << a.g << a.b << a.a;
}
sf::Packet& operator>>(sf::Packet& packet, sf::Color& a) {
    return packet >> a.r >> a.g >> a.b >> a.a;
}
