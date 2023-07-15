#include "../SFML-2.5.1/include/SFML/Graphics.hpp"
#include <iostream>

int main()
{
    sf::Window mainWindow(sf::VideoMode(sf::VideoMode::getDesktopMode().width / 2, sf::VideoMode::getDesktopMode().height * 2 / 3), "Level Edit");
    while(mainWindow.isOpen())
    {

        sf::Event event;
        while (mainWindow.pollEvent(event))
        {
            if(event.type == sf::Event::Closed)
                mainWindow.close();
        }
        
    }
    return 0;
}