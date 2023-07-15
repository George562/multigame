#include "Headers/NumBox.h"
#include "Headers/button.h"
#include "Headers/pushTile.h"
#include <fstream>
#include <utility>

#define vvpt std::vector<std::vector<PushTile*>>

enum item
{
    nothing,
    wall,
    box
};

void generateMatrix(int n, int m, vvpt& matrix, float posX, float posY, int width, int height);

int main()
{
    int scrW = sf::VideoMode::getDesktopMode().width;
    int scrH = sf::VideoMode::getDesktopMode().height;
    sf::RenderWindow mainWindow(sf::VideoMode(scrW / 2, scrH * 2 / 3), "Level Edit");

    int wndW = mainWindow.getSize().x;
    int wndH = mainWindow.getSize().y;

    NumBox levelSizeBoxes[2]{NumBox((float)wndW / 10, (float)wndH / 10, 150, 50), 
                             NumBox((float)wndW / 10, (float)wndH * 2 / 10 + 50, 150, 50)};
    Button genButton((float)wndW / 10, (float)wndH * 7 / 10, 200, 100);
    genButton.setText("    Generate\n      Matrix");

    int levelMatrixN = 1, levelMatrixM = 1;
    vvpt levelMatrix;
    levelMatrix.resize(0);

    int editorRectX = levelSizeBoxes[0].getX() + levelSizeBoxes[0].getWidth() + 100;
    int editorRectY = levelSizeBoxes[0].getY();
    int editorRectWidth = wndW - editorRectX - wndW / 10;
    int editorRectHeight = wndH - editorRectY - (wndH - genButton.getY() - genButton.getHeight());
    
    sf::RectangleShape editorRect(sf::Vector2f(editorRectWidth, editorRectHeight));
    editorRect.setPosition(editorRectX, editorRectY);
    editorRect.setFillColor(sf::Color(200, 200, 200));
    editorRect.setOutlineColor(sf::Color::Black);
    editorRect.setOutlineThickness(3);

    while(mainWindow.isOpen())
    {
        mainWindow.clear(sf::Color::White);

        sf::Event event;
        while (mainWindow.pollEvent(event))
        {
            levelSizeBoxes[0].isActivated(event);
            levelSizeBoxes[1].isActivated(event);

            if(genButton.isActivated(event))
                generateMatrix(levelMatrixN, levelMatrixM, levelMatrix, editorRectX, editorRectY, editorRectWidth, editorRectHeight);

            if(!levelSizeBoxes[0].getText().empty())
                levelMatrixN = std::stoi(levelSizeBoxes[0].getText());
            if(!levelSizeBoxes[1].getText().empty())
                levelMatrixM = std::stoi(levelSizeBoxes[1].getText());
            
            if(!levelMatrix.empty())
                for(int i = 0; i < levelMatrix.size(); i++)
                    for(int j = 0; j < levelMatrix[0].size(); j++)
                        if(levelMatrix[i][j]->isActivated(event))
                        {
                            levelMatrix[i][j]->setState(wall);
                        }

            if(event.type == sf::Event::Closed)
                mainWindow.close();
        }
        levelSizeBoxes[0].draw(mainWindow, sf::RenderStates::Default);
        levelSizeBoxes[1].draw(mainWindow, sf::RenderStates::Default);
        genButton.draw(mainWindow, sf::RenderStates::Default);
        mainWindow.draw(editorRect);
        if(!levelMatrix.empty())
            for(int i = 0; i < levelMatrix.size(); i++)
                for(int j = 0; j < levelMatrix[0].size(); j++)
                    levelMatrix[i][j]->draw(mainWindow, sf::RenderStates::Default);
        mainWindow.display();
    }

    std::ofstream levelFile;
    levelFile.open(".\\Levels\\level.txt");
    for(int i = 0; i < levelMatrix.size(); i++)
    {
        for(int j = 0; j < levelMatrix[i].size(); j++)
        {
            levelFile << levelMatrix[j][i]->getState();
            if(j != levelMatrix[i].size() - 1)
                levelFile << ' ';
        }
        levelFile << "\n";
    }
    levelFile.close();
}

void generateMatrix(int n, int m, vvpt& matrix, float posX, float posY, int width, int height)
{
    if(!matrix.empty())
    {
        for(int i = 0; i < matrix.size(); i++)
        {
            for(int j = 0; j < matrix[0].size(); j++)
                matrix[i].pop_back();
            matrix[i].clear();
        }
        matrix.resize(0);
    }

    if(n == 0 || m == 0)
        return;

    matrix.resize(n);
    for(int i = 0; i < n; i++)
    {
        matrix[i].resize(m);
        for(int j = 0; j < m; j++)
            matrix[i][j] = new PushTile(posX + width * i / n, posY + height * j / m, width / n, height / m);
    }
}