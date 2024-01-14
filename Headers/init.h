#pragma once
#include "../SFML-2.5.1/include/SFML/Network.hpp"
#include "../SFML-2.5.1/include/SFML/Graphics.hpp"
#include "../SFML-2.5.1/include/SFML/Audio.hpp"
#include <vector>
#include <map>
#include <set>
#include <ctime>
#define _USE_MATH_DEFINES
#include <cmath>
#include "scale.h"
#include "rect.h"
#include "circle.h"
#include "animation.h"
#include "text.h"

namespace faction {
    using Type = sf::Uint8;

    enum : Type {
        Player,
        Enemy
    };

    std::map<Type, std::set<Type>> Friendship{
        {Player, {Player}},
        {Enemy, {Enemy}}
    };

    bool friends(Type& left, Type& right) {
        return Friendship[left].count(right);
    }
};

namespace screens {
    using Type = sf::Uint8;
    enum : Type {  // the types of screen
        MainRoom,
        SetIP,
        Dungeon
    };
};

namespace pacetStates {
    sf::Uint8 curState;
    enum : sf::Uint8 {
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

namespace inventoryPage {
    using Type = sf::Uint8;
    enum : sf::Uint8 {
        Crafting,
        Weapons,
        Equipables,
        Perks,
        Stats
    };
}

constexpr int scw = 1920; // screen width
constexpr int sch = 1080; // screen height

constexpr int size = 480, miniSize = 50; // map is matrix n x m cells with size of one; minisize for minimap
constexpr float ScaleParam = float(miniSize) / float(size);
int START_N = 15, START_M = 15;
float WallMinSize = size / 8, WallMaxSize = size;

sf::View GameView({0, 0, (float)scw, (float)sch});
sf::View InterfaceView({0, 0, (float)scw, (float)sch});
sf::View MiniMapView({0, 0, (float)scw, (float)sch});
sf::View InventoryView({0, 0, (float)scw, (float)sch});

sf::Clock* GameClock;

//////////////////////////////////////////////////////////// Textures
sf::Texture ScottPilgrimTexture,
            RamonaFlowersTexture,
            DistortedScientistTexture,
            PlayerTexture,
            PortalTexture,
            PortalAnimation2Texture,
            WallTexture,
            XButtonTexture,
            BoxTexture,
            floor1xTexture,
            PuddleTexture,
            FireTexture,
//////////////////////////////////////////////////////////// Panel Tetures
            GreenPanelTexture,
            GreenPanelPushedTexture,
            RedPanelTexture,
            RedPanelPushedTexture,
            BluePanelTexture,
            BluePanelPushedTexture,
            YellowPanelTexture,
            YellowPanelPushedTexture,
            SteelFrameTexture,
            ItemPanelTexture
            ;

void loadTextures() {
    ScottPilgrimTexture       .loadFromFile("sources/textures/scottpilgrim_multiple.png");
    RamonaFlowersTexture      .loadFromFile("sources/textures/ramonaflowers_multiple_corrupted.png");
    DistortedScientistTexture .loadFromFile("sources/textures/DistortedScientist.png");
    PlayerTexture             .loadFromFile("sources/textures/Player.png");
    PortalTexture             .loadFromFile("sources/textures/Portal.png");
    PortalAnimation2Texture   .loadFromFile("sources/textures/PortalAnimation2.png");
    WallTexture               .loadFromFile("sources/textures/wall.png");
    XButtonTexture            .loadFromFile("sources/textures/XButton.png");
    BoxTexture                .loadFromFile("sources/textures/Box.png");
    floor1xTexture            .loadFromFile("sources/textures/floors/floor1x.png");
    PuddleTexture             .loadFromFile("sources/textures/Puddle.png");
    FireTexture               .loadFromFile("sources/textures/Fire.png");

    GreenPanelTexture        .loadFromFile("sources/textures/Panels/GreenPanel.png");
    GreenPanelPushedTexture  .loadFromFile("sources/textures/Panels/GreenPanelPushed.png");
    RedPanelTexture          .loadFromFile("sources/textures/Panels/RedPanel.png");
    RedPanelPushedTexture    .loadFromFile("sources/textures/Panels/RedPanelPushed.png");
    BluePanelTexture         .loadFromFile("sources/textures/Panels/BluePanel.png");
    BluePanelPushedTexture   .loadFromFile("sources/textures/Panels/BluePanelPushed.png");
    YellowPanelTexture       .loadFromFile("sources/textures/Panels/YellowPanel.png");
    YellowPanelPushedTexture .loadFromFile("sources/textures/Panels/YellowPanelPushed.png");
    SteelFrameTexture        .loadFromFile("sources/textures/Panels/SteelFrame.png");
    ItemPanelTexture         .loadFromFile("sources/textures/Panels/ItemPanel.png");
}

//////////////////////////////////////////////////////////// Shaders
sf::Shader MapShader,
           PlayerShader,
           PortalShader,
           PickupItemShader
           ;

void loadShaders() {
    MapShader        .loadFromFile("sources/shaders/terrain.vert",               "sources/shaders/terrain.frag"              );
    PlayerShader     .loadFromFile("sources/shaders/player/player.vert",         "sources/shaders/player/player.frag"        );
    PortalShader     .loadFromFile("sources/shaders/portal/portal.vert",         "sources/shaders/portal/portal.frag"        );
    PickupItemShader .loadFromFile("sources/shaders/pickupItem/pickupItem.vert", "sources/shaders/pickupItem/pickupItem.frag");
}

//////////////////////////////////////////////////////////// States
sf::RenderStates MapStates(&MapShader);

//////////////////////////////////////////////////////////// Music
sf::Music MainMenuMusic,
          FightMusic1,
          FightMusic2
          ;

void loadMusics() {
    MainMenuMusic .openFromFile("sources/music/MainMenuMusic.wav");
    FightMusic1   .openFromFile("sources/music/FightMusic1.flac");
    FightMusic2   .openFromFile("sources/music/FightMusic2.flac");
}

bool Rect::intersect(Circle& circle) {
    return LenOfVector(sf::Vector2f{std::max(std::abs(circle.PosX - (PosX + Width  / 2)) - Width / 2,  0.f),
                                    std::max(std::abs(circle.PosY - (PosY + Height / 2)) - Height / 2, 0.f)}) <= circle.Radius;
    // explanation: https://www.youtube.com/watch?v=62-pRVZuS5c&ab_channel=InigoQuilez
}

bool Circle::intersect(Rect& rect) {
    return rect.intersect(*this);
}

float random(sf::Vector2f v) {
    return std::modf( ( 1.f + std::sin( v.x * 12.9898f + v.y * 78.233f ) ) * 43758.5453123f, nullptr);
}