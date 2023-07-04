#pragma once
#include "../SFML-2.5.1/include/SFML/Network.hpp"
#include "../SFML-2.5.1/include/SFML/Graphics.hpp"
#include "../SFML-2.5.1/include/SFML/Audio.hpp"
#include <iostream>
#include <fstream>
#include <vector>
#include <map>
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
    bool intersect(const Rect& rect) {
        return rect.PosX <= PosX + Width && PosX <= rect.PosX + rect.Width && rect.PosY <= PosY + Height && PosY <= rect.PosY + rect.Height;
    }
    bool intersect(const Rect& rect, Rect& intersection) {
        if(rect.PosX <= PosX + Width && PosX <= rect.PosX + rect.Width && rect.PosY <= PosY + Height && PosY <= rect.PosY + rect.Height)
        {
            intersection.setRect(std::max(PosX, rect.PosX),         // The X coordinate of the intersection rectangle's topleft corner is always
                                                                    // the X coordinate of the rightmost topleft corner of the compared rectangles
                                 std::max(PosY, rect.PosY),         // Same logic as with the X coordinate, but it's the lowest point now

                                 std::min(PosX + Width, rect.PosX + rect.Width) - std::max(PosX, rect.PosX),        // Width is just a difference between the X coordinates
                                                                                                                    // of the bottom-right and top-left corners
                                 std::min(PosY + Height, rect.PosY + rect.Height) - std::max(PosY, rect.PosY));     // Same logic as with width, but now it's the Y coordinates
            return true;
        }
        else
        {
            intersection.setRect(0, 0, 0, 0);
            return false;
        }
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

int size = 540, miniSize = 50; // map is matrix n x m cells with size of one; minisize for minimap
// int n = 15, m = 15;
// #define BigN n * 2 + 1
// #define BigM m * 2 + 1
size_t START_N = 15, START_M = 15;
float WallMinSize = size / 8, WallMaxSize = size;

sf::Vector2f CameraPos, miniCameraPos;

sf::Texture Box;
sf::Texture RedPanel;
sf::Texture GreenPanel;
sf::Texture BluePanel;
sf::Texture YellowPanel;
sf::Texture SteelFrame;
sf::Texture PlayerTexture;

void LoadAllTextures() {
    Box.            loadFromFile("sources/Box.png"              );
    RedPanel.       loadFromFile("sources/RedPanel.png"         );
    RedPanel.       loadFromFile("sources/RedPanelPushed.png"   );
    GreenPanel.     loadFromFile("sources/GreenPanel.png"       );
    GreenPanel.     loadFromFile("sources/GreenPanelPushed.png" );
    BluePanel.      loadFromFile("sources/BluePanel.png"        );
    BluePanel.      loadFromFile("sources/BluePanelPushed.png"  );
    YellowPanel.    loadFromFile("sources/YellowPanel.png"      );
    YellowPanel.    loadFromFile("sources/YellowPanelPushed.png");
    SteelFrame.     loadFromFile("sources/SteelFrame.png"       );
    PlayerTexture.  loadFromFile("sources/Player.png"           );
}

void init() {
    LoadAllTextures();
}