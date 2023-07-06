#pragma once
#include "../SFML-2.5.1/include/SFML/Network.hpp"
#include "../SFML-2.5.1/include/SFML/Graphics.hpp"
#include "../SFML-2.5.1/include/SFML/Audio.hpp"
#include "../rapidxml-1.13/rapidxml.hpp"
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <map>
#include <cstring>
#include <string>
#include <ctime>
#include <queue>
#include <cmath>
#include "scale.h"
#define M_PI       3.14159265358979323846   // pi

using Point = sf::Vector2i;
using vp = std::vector<Point>;
using vvp = std::vector<vp>;
struct Rect {
    float PosX, PosY, Width, Height;
    sf::Vector2f getPosition() { return {PosX, PosY}; }
    void setPosition(sf::Vector2f v) { PosX = v.x; PosY = v.y; }
    void setPosition(float x, float y) { PosX = x; PosY = y; }
    sf::Vector2f getSize() { return {Width, Height}; }
    void setSize(sf::Vector2f v) { setSize(v.x, v.y); }
    void setSize(float w, float h) { Width = w; Height = h; }
    void setRect(float x, float y, float w, float h) { PosX = x; PosY = y; Width = w; Height = h; }
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
struct Circle { float PosX, PosY, Radius; };

using str = std::string;
using vb = std::vector<bool>;
using vvb = std::vector<vb>;

Point dirs[] = {{1, 0}, {0, 1}, {-1, 0}, {0, -1}};

namespace screens {
    enum screens {  // the types of screen
        Main,       // main menu                -> Solo, Coop
        Coop,       // coop                     -> Host, Connect
        Host,       // choose be a host         -> .
        SetIP,      // input IP to connect      -> Connect
        Connect,    // Wellcome to the room     -> .
        Solo,       // start the game alone     -> New Game, Continue
        New_Game,   // Start new game           ->
        Continue,   // Continue old game        ->
        EscOfCoop   // if turn esc in game      -> Main
    };
};

int scw = sf::VideoMode::getDesktopMode().width; // screen width
int sch = sf::VideoMode::getDesktopMode().height; // screen height

int size, miniSize; // map is matrix n x m cells with size of one; minisize for minimap
int START_N, START_M;
float WallMinSize = size / 8, WallMaxSize = size;

void SetSettings() {
    rapidxml::xml_document<> settings;
    std::ifstream file("config.xml");
    std::stringstream buffer;
    buffer << file.rdbuf();
    file.close();
    std::string content(buffer.str());
    settings.parse<0>(&content[0]);
    rapidxml::xml_node<>* root = settings.first_node();
    for (rapidxml::xml_node<>* node = root->first_node(); node; node = node->next_sibling()) {
        if (strcmp(node->name(), "size") == 0)
            size = stoi(std::string (node->value()));
        else
        if (strcmp(node->name(), "miniSize") == 0)
            miniSize = stoi(std::string (node->value()));
        else
        if (strcmp(node->name(), "START_N") == 0)
            START_N = stoi(std::string (node->value()));
        else
        if (strcmp(node->name(), "START_M") == 0)
            START_M = stoi(std::string (node->value()));
    }
    settings.clear();
}

sf::Vector2f CameraPos, miniCameraPos;

void init() {}