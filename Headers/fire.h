#pragma once
#include "interactible.h"
#define _USE_MATH_DEFINES
#include <cmath>

class Fire : public Interactable {
public:
    sf::Clock* clock;
    sf::Time secs;

    Fire(sf::Time secs) {
        this->clock = new sf::Clock();
        this->secs = secs;
    }

    static bool PropagationAllowed(int y_ancestor, int x_ancestor, float pos_y, float pos_x, vvb walls) {
        //int y_ancestor = (int)PosY / size; int x_ancestor = (int)PosX / size;
        if (pos_y < 0 || pos_x < 0) return false; // если огонь пытается пересечь самые левые или самые верхние стены
        int y = (int)pos_y / size; int x = (int)pos_x / size;
        if (y_ancestor == y && x_ancestor == x) return true; // одинаковые тайлы
        // проверка на стену между тайлами
        if (y_ancestor == y && !walls[y * 2 + 1][std::max(x_ancestor, x)] || // соседние тайлы по горизонтали
            x_ancestor == x && !walls[std::max(y_ancestor, y) * 2][x])   // соседние тайлы по вертикали
                return true;
        else { return false; } // либо есть стены, либо соседи по диагонали (пока не проверяются на стены)
    }
};