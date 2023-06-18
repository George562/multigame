#include <SFML\Network.hpp>
#include <SFML\Graphics.hpp>
#include <iostream>
#include <stdio.h>
#include <fstream>
#include <vector>
#include <map>
#include <string>
#include <ctime>
#include <queue>
#include <cmath>
#include <math.h>
#include <conio.h>
#include <thread>
#include <chrono>
#define M_PI       3.14159265358979323846   // pi

int scw = sf::VideoMode::getDesktopMode().width; // screen width
int sch = sf::VideoMode::getDesktopMode().height; // screen height

int size = 540, miniSize = 50, n = 15, m = 15; // map is matrix n x m cells with size of one; minisize for minimap
#define BigN n * 2 + 1
#define BigM m * 2 + 1

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