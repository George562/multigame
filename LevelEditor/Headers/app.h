#include "numBox.h"
#include "textBox.h"
#include "button.h"
#include "pushTile.h"
#include <fstream>
#include <iostream>
#include <utility>

#define vvpt std::vector<std::vector<PushTile*>>

enum item
{
    nothing,
    wall,
    box
};

class App
{
private:
    sf::RenderWindow *mainWindow;
    Button genButton, saveButton;
    NumBox levelSizeBoxes[2];
    TextBox nameBox;
    sf::Font font;
    sf::RectangleShape editorRect;
    int levelMatrixN, levelMatrixM, editorRectX, editorRectY, editorRectWidth, editorRectHeight;
    int scrW, scrH, wndW, wndH;
    vvpt levelMatrix;

    std::vector<sf::Drawable*> drawnElements;

    void init();
    void loop();
    void generateMatrix(int, int, vvpt&, float, float, int, int);
    void saveMap(std::string, int, int, vvpt&);
public:
    App(sf::RenderWindow&);
    void start(){ init(); loop(); }
};

App::App(sf::RenderWindow& window) 
{
    mainWindow = &window;

    wndW = mainWindow->getSize().x;
    wndH = mainWindow->getSize().y;
}

void App::init()
{
    font.loadFromFile("Resources/Fonts/consola.ttf");

    levelSizeBoxes[0] = NumBox((float)wndW / 10, (float)wndH / 10, 150, 50, font);
    levelSizeBoxes[1] = NumBox((float)wndW / 10, (float)wndH * 2 / 10 + 50, 150, 50, font);

    genButton = Button((float)wndW / 10, (float)wndH * 3 / 10 + 50, 200, 100, font);
    genButton.setText("Generate\nMatrix");

    nameBox = TextBox((float)wndW / 10, (float)wndH * 4 / 10 + 100, 200, 50, font);

    saveButton = Button((float)wndW / 10, (float)wndH * 6 / 10 + 50, 200, 100, font);
    saveButton.setText("Save\nLevel");

    levelMatrixN = 1, levelMatrixM = 1;
    levelMatrix.resize(0);

    editorRectX = levelSizeBoxes[0].getX() + levelSizeBoxes[0].getWidth() + 100;
    editorRectY = levelSizeBoxes[0].getY();
    editorRectWidth = wndW - editorRectX - wndW / 10;
    editorRectHeight = wndH - editorRectY - wndW / 10;
    
    editorRect = sf::RectangleShape(sf::Vector2f(editorRectWidth, editorRectHeight));
    editorRect.setPosition(editorRectX, editorRectY);
    editorRect.setFillColor(sf::Color(200, 200, 200));
    editorRect.setOutlineColor(sf::Color::Black);
    editorRect.setOutlineThickness(3);

    drawnElements.push_back(&levelSizeBoxes[0]); drawnElements.push_back(&levelSizeBoxes[1]);
    drawnElements.push_back(&nameBox);
    drawnElements.push_back(&genButton); drawnElements.push_back(&saveButton);
    drawnElements.push_back(&editorRect);
}

void App::loop()
{
    while(mainWindow->isOpen())
    {
        mainWindow->clear(sf::Color::White);
        sf::Event event;
        while (mainWindow->pollEvent(event))
        {
            levelSizeBoxes[0].isActivated(event);
            levelSizeBoxes[1].isActivated(event);
            nameBox.isActivated(event);

            if(genButton.isActivated(event))
                generateMatrix(levelMatrixN, levelMatrixM, levelMatrix, editorRectX, editorRectY, editorRectWidth, editorRectHeight);

            if(saveButton.isActivated(event))
                saveMap(nameBox.getText(), levelMatrixN, levelMatrixM, levelMatrix);

            if(!levelSizeBoxes[0].getText().empty())
                levelMatrixN = std::stoi(levelSizeBoxes[0].getText());
            if(!levelSizeBoxes[1].getText().empty())
                levelMatrixM = std::stoi(levelSizeBoxes[1].getText());
            
            if(!levelMatrix.empty())
                for(int i = 0; i < levelMatrix.size(); i++)
                    for(int j = 0; j < levelMatrix[0].size(); j++)
                        if(levelMatrix[i][j]->isActivated(event))
                            levelMatrix[i][j]->setState(wall);

            if(event.type == sf::Event::Closed)
                mainWindow->close();
        }

        for(int i = 0; i < drawnElements.size(); i++)
            mainWindow->draw(*drawnElements[i]);

        if(!levelMatrix.empty())
            for(int i = 0; i < levelMatrix.size(); i++)
                for(int j = 0; j < levelMatrix[0].size(); j++)
                    levelMatrix[i][j]->draw(*mainWindow, sf::RenderStates::Default);

        mainWindow->display();
    }
}

void App::generateMatrix(int n, int m, vvpt& matrix, float posX, float posY, int width, int height)
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

    matrix.resize(m);
    for(int i = 0; i < m; i++)
    {
        matrix[i].resize(n);
        for(int j = 0; j < n; j++)
            matrix[i][j] = new PushTile(posX + width * i / m, posY + height * j / n, width / m, height / n);
    }
}

void App::saveMap(std::string filename, int n, int m, vvpt& matrix)
{
    std::ofstream levelFile;
    levelFile.open(".\\Levels\\" + filename + ".txt");
    levelFile << n << ' ' << m << '\n';
    for(int i = 0; i < n; i++)
    {
        for(int j = 0; j < m; j++)
        {
            levelFile << matrix[j][i]->getState();
            if(j != m - 1)
                levelFile << ' ';
        }
        levelFile << "\n";
    }
    levelFile.close();
}