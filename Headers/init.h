#pragma once
#include "tools.h"
#include "scale.h"
#include "rect.h"
#include "circle.h"
#include "../SFML-2.5.1/include/SFML/Audio.hpp"
#include <vector>
#include <map>
#include <set>
#include <ctime>

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
    enum : Type {
        Items,
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
int START_N = 9, START_M = 6;
float WallMinSize = size / 8, WallMaxSize = size;

sf::View GameView({0, 0, (float)scw, (float)sch});
sf::View InterfaceView({0, 0, (float)scw, (float)sch});
sf::View MiniMapView({0, 0, (float)scw, (float)sch});

sf::View InventoryView({0, 0, (float)scw, (float)sch});

sf::Clock* GameClock;

namespace Textures {
//////////////////////////////////////////////////////////// Textures
    sf::Texture ScottPilgrim,
                Distorted,
                DistortedScientist,
                Player,
                Portal,
                PortalAnimation2,
                Wall,
                XButton,
                Box,
                floor1x,
                Puddle,
                Fire,
                Fireball,
                Architect,
                Noise,
//////////////////////////////////////////////////////////// Panel Tetures
                GreenPanel,
                GreenPanelPushed,
                RedPanel,
                RedPanelPushed,
                BluePanel,
                BluePanelPushed,
                YellowPanel,
                YellowPanelPushed,
                SteelFrame,
                ItemPanel,
                Frame4
                ;
}

void loadTextures() {
    Textures::ScottPilgrim       .loadFromFile("sources/textures/scottpilgrim_multiple.png");
    Textures::Distorted          .loadFromFile("sources/textures/Distorted.png");
    Textures::DistortedScientist .loadFromFile("sources/textures/DistortedScientist.png");
    Textures::Player             .loadFromFile("sources/textures/Player.png");
    Textures::Portal             .loadFromFile("sources/textures/Portal.png");
    Textures::PortalAnimation2   .loadFromFile("sources/textures/PortalAnimation2.png");
    Textures::Wall               .loadFromFile("sources/textures/wall.png");
    Textures::XButton            .loadFromFile("sources/textures/XButton.png");
    Textures::Box                .loadFromFile("sources/textures/Box.png");
    Textures::floor1x            .loadFromFile("sources/textures/floors/floor1x.png");
    Textures::Puddle             .loadFromFile("sources/textures/Puddle.png");
    Textures::Fire               .loadFromFile("sources/textures/Fire.png");
    Textures::Fireball           .loadFromFile("sources/textures/Fireball.png");
    Textures::Architect          .loadFromFile("sources/textures/Architect.png");
    Textures::Noise              .loadFromFile("sources/textures/noise.png");

    Textures::GreenPanel        .loadFromFile("sources/textures/Panels/GreenPanel.png");
    Textures::GreenPanelPushed  .loadFromFile("sources/textures/Panels/GreenPanelPushed.png");
    Textures::RedPanel          .loadFromFile("sources/textures/Panels/RedPanel.png");
    Textures::RedPanelPushed    .loadFromFile("sources/textures/Panels/RedPanelPushed.png");
    Textures::BluePanel         .loadFromFile("sources/textures/Panels/BluePanel.png");
    Textures::BluePanelPushed   .loadFromFile("sources/textures/Panels/BluePanelPushed.png");
    Textures::YellowPanel       .loadFromFile("sources/textures/Panels/YellowPanel.png");
    Textures::YellowPanelPushed .loadFromFile("sources/textures/Panels/YellowPanelPushed.png");
    Textures::SteelFrame        .loadFromFile("sources/textures/Panels/SteelFrame.png");
    Textures::ItemPanel         .loadFromFile("sources/textures/Panels/ItemPanel.png");
    Textures::Frame4            .loadFromFile("sources/textures/Panels/frame4.png");
}

namespace Shaders {
//////////////////////////////////////////////////////////// Shaders
    sf::Shader Map,
               Player,
               Portal,
               PickupItem,
               Architect,
               DmgText,     // text moving up
               Distortion1, // linear left move
               Distortion2  // circle move
               ;
}

void loadShaders() {
    Shaders::Map         .loadFromFile("sources/shaders/terrain.vert",               "sources/shaders/terrain.frag"               );
    Shaders::Player      .loadFromFile("sources/shaders/player/player.vert",         "sources/shaders/player/player.frag"         );
    Shaders::Portal      .loadFromFile("sources/shaders/portal/portal.vert",         "sources/shaders/portal/portal.frag"         );
    Shaders::PickupItem  .loadFromFile("sources/shaders/pickupItem/pickupItem.vert", "sources/shaders/pickupItem/pickupItem.frag" );
    Shaders::Architect   .loadFromFile("sources/shaders/Architect/Architect.vert",   "sources/shaders/Architect/Architect.frag"   );
    Shaders::DmgText     .loadFromFile("sources/shaders/dmgText/dmgText.vert",       "sources/shaders/dmgText/dmgText.frag"       );
    Shaders::Distortion1 .loadFromFile("sources/shaders/distorted1/distorted1.vert", "sources/shaders/distorted1/distorted1.frag" );
    Shaders::Distortion2 .loadFromFile("sources/shaders/distorted2/distorted2.vert", "sources/shaders/distorted2/distorted2.frag" );
}

namespace States {
//////////////////////////////////////////////////////////// States
    sf::RenderStates Map(&Shaders::Map),
                     DmgText(&Shaders::DmgText),
                     Distortion1(&Shaders::Distortion1),
                     Distortion2(&Shaders::Distortion2)
                     ;
}

namespace Musics {
//////////////////////////////////////////////////////////// Musics
    sf::Music MainMenu,
              Fight1, Fight2
              ;
}

void loadMusics() {
    Musics::MainMenu .openFromFile("sources/music/MainMenu.wav");
    Musics::Fight1   .openFromFile("sources/music/Fight1.flac");
    Musics::Fight2   .openFromFile("sources/music/Fight2.flac");
}

bool Rect::intersect(Circle& circle) {
    return LenOfVector(sf::Vector2f(std::max(std::abs(circle.PosX - (PosX + Width  / 2)) - Width / 2,  0.f),
                                    std::max(std::abs(circle.PosY - (PosY + Height / 2)) - Height / 2, 0.f))) <= circle.Radius;
    // explanation: https://www.youtube.com/watch?v=62-pRVZuS5c&ab_channel=InigoQuilez
}

bool Circle::intersect(Rect& rect) {
    return rect.intersect(*this);
}

float random(sf::Vector2f v) {
    return std::modf( ( 1.f + std::sin( v.x * 12.9898f + v.y * 78.233f ) ) * 43758.5453123f, nullptr);
}