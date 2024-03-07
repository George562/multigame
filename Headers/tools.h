#pragma once
#include "../SFML-2.5.1/include/SFML/Network.hpp"
#include "../SFML-2.5.1/include/SFML/Graphics.hpp"
#define _USE_MATH_DEFINES
#include <cmath>
#include <iostream>
#include <string>

std::string floatToString(float num) {
    int wholePart = int(num);
    int fracPart = int(std::abs(int(num * 10) % 10));
    if (fracPart == 0)
        return std::to_string(wholePart);
    else {
        return std::to_string(wholePart) + '.' + std::to_string(fracPart);
    }
}

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

template <typename T>
void DeletePointerFromVector(std::vector<T>& arr, size_t index) {
    delete arr[index];
    arr.erase(arr.begin() + index);
}

template <typename T>
void clearVectorOfPointer(std::vector<T>& arr) {
    for (size_t i = 0; i < arr.size(); i++) {
        delete arr[i];
    }
    arr.clear();
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
    return sf::Vector2f(a.x * b.x, a.y * b.y);
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
