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
    Upgradable(std::vector<T> stats) { setStats(stats); }

    T getStat() { return stats[curLevel]; }

    void setStats(std::vector<T> stats) {
        this->stats = stats;
        this->maxLevel = stats.size();
        curLevel = 0;
    }

    void addToStats(T val) {
        for (int i = 0; i < maxLevel; i++)
            stats[i] += val;
    }

    operator T() const { return stats[curLevel]; }
};


template <class T> T operator+(Upgradable<T> left, Upgradable<T> right) { return left.getStat() + right.getStat(); }
template <class T, class Q> Q operator+(Upgradable<T> upg, Q val) { return upg.getStat() + val; }
template <class T, class Q> Q operator+(Q val, Upgradable<T> upg) { return val + upg.getStat(); }
template <class T> Upgradable<T>& operator+=(Upgradable<T> upg, T val) {
    upg[upg.curLevel] += val;
    return upg;
}

template <class T> T operator-(Upgradable<T> left, Upgradable<T> right) { return left.getStat() - right.getStat(); }
template <class T, class Q> Q operator-(Upgradable<T> upg, Q val) { return upg.getStat() - val; }
template <class T, class Q> Q operator-(Q val, Upgradable<T> upg) { return val - upg.getStat(); }
template <class T> Upgradable<T>& operator-=(Upgradable<T> upg, T val) {
    upg[upg.curLevel] -= val;
    return upg;
}

template <class T, class Q> Q operator*(Upgradable<T> upg, Q val) { return upg.getStat() * val; }
template <class T, class Q> Q operator*(Q val, Upgradable<T> upg) { return val * upg.getStat(); }

template <class T, class Q> Q operator/(Upgradable<T> upg, Q val) { return upg.getStat() / val; }
template <class T, class Q> Q operator/(Q val, Upgradable<T> upg) { return val / upg.getStat(); }
float operator/(Upgradable<sf::Time> upg, sf::Time val) { return upg.getStat() / val; }
float operator/(sf::Time val, Upgradable<sf::Time> upg) { return val / upg.getStat(); }

template <class T> bool operator< (Upgradable<T> upg, T val) { return upg.getStat() < val;  }
template <class T> bool operator< (T val, Upgradable<T> upg) { return val < upg.getStat();  }
template <class T> bool operator<=(Upgradable<T> upg, T val) { return upg.getStat() <= val; }
template <class T> bool operator<=(T val, Upgradable<T> upg) { return val <= upg.getStat(); }
template <class T> bool operator> (Upgradable<T> upg, T val) { return upg.getStat() > val;  }
template <class T> bool operator> (T val, Upgradable<T> upg) { return val > upg.getStat();  }
template <class T> bool operator>=(Upgradable<T> upg, T val) { return upg.getStat() >= val; }
template <class T> bool operator>=(T val, Upgradable<T> upg) { return val >= upg.getStat(); }
template <class T> bool operator==(T val, Upgradable<T> upg) { return val == upg.getStat(); }

template <class T>
std::ostream& operator<<(std::ostream& stream, Upgradable<T>& upg) {
    stream << upg.maxLevel << ' ';
    for (int i = 0; i < upg.maxLevel; i++)
        stream << upg.stats[i] << ' ';
    return stream << upg.curLevel;
}
template <class T>
std::istream& operator>>(std::istream& stream, Upgradable<T>& upg) {
    stream >> upg.maxLevel;
    upg.stats.resize(upg.maxLevel);
    for (int i = 0; i < upg.maxLevel; i++)
        stream >> upg.stats[i];
    return stream >> upg.curLevel;
}

std::ostream& operator<<(std::ostream& stream, Upgradable<sf::Time>& upg) {
    stream << upg.maxLevel << ' ';
for (int i = 0; i < upg.maxLevel; i++)
        stream << upg.stats[i].asSeconds() << ' ';
    return stream << upg.curLevel;
}

std::istream& operator>>(std::istream& stream, Upgradable<sf::Time>& upg) {
    stream >> upg.maxLevel;
    upg.stats.resize(upg.maxLevel);
    for (int i = 0; i < upg.maxLevel; i++) {
        float newVal;
        stream >> newVal;
        upg.stats[i] = sf::seconds(newVal);
    }
    return stream >> upg.curLevel;
}