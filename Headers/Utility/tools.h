#pragma once
#include "../../SFML-2.5.1/include/SFML/Network.hpp"
#include "../../SFML-2.5.1/include/SFML/Graphics.hpp"
#define _USE_MATH_DEFINES
#include <cmath>
#include <iostream>
#include <string>

std::string floatToString(float num) {
    int wholePart = std::floor(num);
    int fracPart = (num - wholePart) * 10;
    return std::to_string(wholePart) + '.' + std::to_string(fracPart);
}

std::string textWrap(std::string text, int wrapEvery) {
    std::string wrappedText;
    int charCount = 0;
    for (int i = 0; i < text.size(); i++) {
        if (text[i] == '\n')
            charCount = 0;
        if (charCount % wrapEvery == 0 and charCount != 0)
            wrappedText.push_back('\n');
        wrappedText.push_back(text.at(i));
        charCount++;
    }
    return wrappedText;
}

std::string stringLower(std::string text) {
    std::string loweredText;
    int uppercaseCount = 0;
    for (int i = 0; i < text.size(); i++)
        if ('A' <= text[i] && text[i] <= 'Z')
            uppercaseCount++;
    if (uppercaseCount > 1) // If it's an acronym - do not lower
        return text;
    for (int i = 0; i < text.size(); i++)
        loweredText.push_back(std::tolower(text[i]));
    return loweredText;
}

void CreateImage() {
    sf::Image img, res;
    img.loadFromFile("sources/textures/floor.png");
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

enum timeType {
    seconds,
    milliseconds,
    microseconds
};

std::vector<float> timeToFloat(std::vector<sf::Time> arr, timeType type) {
    std::vector<float> floatArr(arr.size());
    for (int i = 0; i < arr.size(); i++) {
        switch(type) {
            case seconds:
                floatArr[i] = arr[i].asSeconds();
                break;
            case milliseconds:
                floatArr[i] = arr[i].asMilliseconds();
                break;
            case microseconds:
                floatArr[i] = arr[i].asMicroseconds();
                break;
        }
    }
    return floatArr;
}

std::vector<sf::Time> floatToTime(std::vector<float> arr, timeType type) {
    std::vector<sf::Time> timeArr(arr.size());
    for (int i = 0; i < arr.size(); i++) {
        switch(type) {
            case seconds:
                timeArr[i] = sf::seconds(arr[i]);
                break;
            case milliseconds:
                timeArr[i] = sf::seconds(arr[i]);
                break;
            case microseconds:
                timeArr[i] = sf::microseconds(arr[i]);
                break;
        }
    }
    return timeArr;
}

sf::Vector2f operator*(sf::Vector2f a, sf::Vector2f b) {
    return sf::Vector2f(a.x * b.x, a.y * b.y);
}

template <typename T>
std::ostream& operator<<(std::ostream& stream, sf::Vector2<T>& a) {
    return stream << a.x << ", " << a.y;
}
template <typename T>
std::istream& operator>>(std::istream& stream, sf::Vector2<T>& a) {
    return stream >> a.x >> a.y;
}

template <typename T>
sf::Packet& operator<<(sf::Packet& packet, sf::Vector2<T>& a) {
    return packet << a.x << a.y;
}
template <typename T>
sf::Packet& operator>>(sf::Packet& packet, sf::Vector2<T>& a) {
    return packet >> a.x >> a.y;
}

sf::Packet& operator<<(sf::Packet& packet, sf::Color& a) {
    return packet << a.r << a.g << a.b << a.a;
}
sf::Packet& operator>>(sf::Packet& packet, sf::Color& a) {
    return packet >> a.r >> a.g >> a.b >> a.a;
}
