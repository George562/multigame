#include "Headers/app.h"

int main()
{
    int scrW = sf::VideoMode::getDesktopMode().width;
    int scrH = sf::VideoMode::getDesktopMode().height;

    sf::RenderWindow window(sf::VideoMode(scrW / 2, scrH * 2 / 3), "Level Edit");

    App *app = new App(window);
    app->start();
}