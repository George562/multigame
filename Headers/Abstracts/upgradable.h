#pragma once
#include <vector>
#include "../../SFML-2.5.1/include/SFML/System.hpp"

template <class T>
class Upgradable {
public:
    std::vector<T> stats;
    int maxLevel;
    int curLevel;

    Upgradable() {}
    Upgradable(std::vector<T>);

    T getStat() { return stats[curLevel]; }

    void addToStats(T val) {
        for (int i = 0; i < maxLevel; i++)
            stats[i] += val;
    }

    void multiplyToStats(T val) {
        for (int i = 0; i < maxLevel; i++)
            stats[i] *= val;
    }
};

template <class T>
Upgradable<T>::Upgradable(std::vector<T> stats) {
    this->stats = stats;
    this->maxLevel = stats.size();
    curLevel = 0;
}

template <class T>
T operator+(Upgradable<T> upgVals, T val) { return upgVals.getStat() + val; }
template <class T>
Upgradable<T>& operator +=(Upgradable<T> upgVals, T val) {
    upgVals[upgVals.curLevel] += val;
    return upgVals;
}

template <class T>
T operator+(Upgradable<T> left, Upgradable<T> right) { return left.getStat() + right.getStat(); }

template <class T>
T operator-(Upgradable<T> upgVals, T val) { return upgVals.getStat() - val; }
template <class T>
Upgradable<T>& operator-=(Upgradable<T> upgVals, T val) {
    upgVals[upgVals.curLevel] -= val;
    return upgVals;
}

template <class T>
T operator-(Upgradable<T> left, Upgradable<T> right) { return left.getStat() - right.getStat(); }

template <class T>
T operator*(Upgradable<T> upgVals, T val) { return upgVals.getStat() * val; }
template <class T>
T operator/(Upgradable<T> upgVals, T val) { return upgVals.getStat() / val; }

template <class T>
std::ostream& operator<<(std::ostream& stream, Upgradable<T>& upgVals) {
    stream << upgVals.maxLevel << ' ';
    for (int i = 0; i < upgVals.maxLevel; i++)
        stream << upgVals.stats[i] << ' ';
    return stream << upgVals.curLevel;
}
template <class T>
std::istream& operator>>(std::istream& stream, Upgradable<T>& upgVals) {
    stream >> upgVals.maxLevel;
    upgVals.stats.resize(upgVals.maxLevel);
    for (int i = 0; i < upgVals.maxLevel; i++)
        stream >> upgVals.stats[i];
    return stream >> upgVals.curLevel;
}

std::ostream& operator<<(std::ostream& stream, Upgradable<sf::Time>& upgVals) {
    stream << upgVals.maxLevel << ' ';
for (int i = 0; i < upgVals.maxLevel; i++)
        stream << upgVals.stats[i].asSeconds() << ' ';
    return stream << upgVals.curLevel;
}

std::istream& operator>>(std::istream& stream, Upgradable<sf::Time>& upgVals) {
    stream >> upgVals.maxLevel;
    upgVals.stats.resize(upgVals.maxLevel);
    for (int i = 0; i < upgVals.maxLevel; i++) {
        float newVal;
        stream >> newVal;
        upgVals.stats[i] = sf::seconds(newVal);
    }
    return stream >> upgVals.curLevel;
}