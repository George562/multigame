#pragma once
#include "../SFML-2.5.1/include/SFML/Network.hpp"
#include "../SFML-2.5.1/include/SFML/Graphics.hpp"
#include "../SFML-2.5.1/include/SFML/Audio.hpp"
#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <set>
#include <string>
#include <ctime>
#include <queue>
#include <cmath>
#include <random>
#include "scale.h"
#include "rect.h"
#include "circle.h"
#include "animation.h"
#define M_PI       3.14159265358979323846   // pi

#define ActivationButton sf::Keyboard::X

using Point = sf::Vector2i;

using str = std::string;
using vb = std::vector<bool>;
using vvb = std::vector<vb>;

Point dirs[] = {{1, 0}, {0, 1}, {-1, 0}, {0, -1}};

namespace Fraction {
    enum Fraction : sf::Uint8 {
        Player,
        Enemy
    };
    std::map<Fraction, std::set<Fraction>> Friendship{
        {Player, {Player}},
        {Enemy, {Enemy}}
    };
};

namespace screens {
    enum screens : sf::Uint8 {  // the types of screen
        MainRoom,
        SetIP,
        Dungeon
    };
};

namespace pacetStates {
    enum pacetStates : sf::Uint8 {
        disconnect,         // client send to host
        PlayersAmount,      // to connecting client
        PlayerConnect,      // host sent to clients when someone connected
        PlayerDisconnect,   // host sent to clients when someone disconnected
        Labyrinth,          // for LabirintData
        PlayerPos,          // send from client to host than host sending others clients
        SetPos,             // for set pos of player, because PlayerPos just say positions of others players
        ChatEvent,
        Shooting,
    };
};

constexpr int scw = 1920; // screen width
constexpr int sch = 1080; // screen height

constexpr int size = 480, miniSize = 50; // map is matrix n x m cells with size of one; minisize for minimap
constexpr float ScaleParam = float(miniSize) / float(size);
int START_N = 15, START_M = 15;
float WallMinSize = size / 8, WallMaxSize = size;

sf::View GameView({0, 0, (float)scw, (float)sch});
sf::View InterfaceView({0, 0, (float)scw, (float)sch});
sf::View MiniMapView({0, 0, (float)scw, (float)sch});

sf::Clock* GameClock;

//////////////////////////////////////////////////////////// Tetures
sf::Texture ScottpilgrimTexture,
            RamonaFlowersTexture,
            DistortedScientistTexture,
            PlayerTexture,
            PortalTexture,
            PortalAnimation2Texture,
            WallTexture,
            XButtonTexture,
            BoxTexture,
            floor3xTexture,
//////////////////////////////////////////////////////////// Panel Tetures
            GreenPanelTexture,
            GreenPanelPushedTexture,
            RedPanelTexture,
            RedPanelPushedTexture,
            BluePanelTexture,
            BluePanelPushedTexture,
            YellowPanelTexture,
            YellowPanelPushedTexture,
            SteelFrameTexture
            ;

void loadTextures() {
    ScottpilgrimTexture       .loadFromFile("sources/textures/scottpilgrim_multiple.png");
    RamonaFlowersTexture      .loadFromFile("sources/textures/ramonaflowers_multiple.png");
    DistortedScientistTexture .loadFromFile("sources/textures/DistortedScientist.png");
    PlayerTexture             .loadFromFile("sources/textures/Player.png");
    PortalTexture             .loadFromFile("sources/textures/Portal.png");
    PortalAnimation2Texture   .loadFromFile("sources/PortalAnimation2.png");
    WallTexture               .loadFromFile("sources/textures/wall.png");
    XButtonTexture            .loadFromFile("sources/textures/XButton.png");
    BoxTexture                .loadFromFile("sources/textures/Box.png");
    floor3xTexture            .loadFromFile("sources/floor3x.png");

    GreenPanelTexture        .loadFromFile("sources/textures/Panels/GreenPanel.png");
    GreenPanelPushedTexture  .loadFromFile("sources/textures/Panels/GreenPanelPushed.png");
    RedPanelTexture          .loadFromFile("sources/textures/Panels/RedPanel.png");
    RedPanelPushedTexture    .loadFromFile("sources/textures/Panels/RedPanelPushed.png");
    BluePanelTexture         .loadFromFile("sources/textures/Panels/BluePanel.png");
    BluePanelPushedTexture   .loadFromFile("sources/textures/Panels/BluePanelPushed.png");
    YellowPanelTexture       .loadFromFile("sources/textures/Panels/YellowPanel.png");
    YellowPanelPushedTexture .loadFromFile("sources/textures/Panels/YellowPanelPushed.png");
    SteelFrameTexture        .loadFromFile("sources/textures/Panels/SteelFrame.png");
}

//////////////////////////////////////////////////////////// Fonts
sf::Font ljk_InkyFont;

void loadFonts() {
    ljk_InkyFont.loadFromFile("sources/ljk_Inky Thin Pixels.otf");
}

bool Rect::intersect(Circle& circle) {
    return LenOfVector(sf::Vector2f{std::max(std::abs(circle.PosX - (PosX + Width  / 2)) - Width / 2,  0.f),
                                    std::max(std::abs(circle.PosY - (PosY + Height / 2)) - Height / 2, 0.f)}) <= circle.Radius;
    // explanation: https://www.youtube.com/watch?v=62-pRVZuS5c&ab_channel=InigoQuilez
}

bool Circle::intersect(Rect& rect) {
    return rect.intersect(*this);
}