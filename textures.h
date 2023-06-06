#include <SFML\Graphics.hpp>

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