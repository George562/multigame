#include "numBox.h"
#include "textBox.h"
#include "button.h"
#include "pushTile.h"
#include "label.h"
#include "scrollContainer.h"
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
    int levelMatrixN, levelMatrixM, editorRectX, editorRectY, editorRectWidth, editorRectHeight;
    int scrW, scrH, wndW, wndH;
    vvpt levelMatrix;

    sf::RenderWindow *mainWindow;
    sf::Font font;

    ScrollContainer optionsContainer;
    NumBox levelSizeBoxes[2];
    Label mapWidthText, mapHeightText;
    Button genButton, saveButton;
    TextBox nameBox;
    InteractionRect editorRect;

    std::vector<InteractionRect*> drawnElements;

    void init();
    void loop();
    void poll(sf::Event&);
    void draw();
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
    levelMatrixN = 1, levelMatrixM = 1;
    levelMatrix.resize(0);

    font.loadFromFile("Resources/Fonts/consola.ttf");

    optionsContainer = ScrollContainer((float)wndW / 20, (float)wndH / 20, wndW / 4, wndH * 18 / 20);

    levelSizeBoxes[0] = NumBox((float)wndW * 2 / 10, (float)wndH / 10, 60, 50, font, 2);
    levelSizeBoxes[1] = NumBox((float)wndW * 2 / 10, levelSizeBoxes[0].getBottom() + 20, 60, 50, font, 2);

    mapWidthText = Label(optionsContainer.getX() + 24, levelSizeBoxes[0].getY() + 10, 180, 20, font);
    mapWidthText.setText("Map width"); mapWidthText.setCharacterSize(mapWidthText.getHeight());

    mapHeightText = Label(optionsContainer.getX() + 24, levelSizeBoxes[1].getY() + 10, 200, 20, font);
    mapHeightText.setText("Map height"); mapHeightText.setCharacterSize(mapHeightText.getHeight());

    genButton = Button((float)wndW / 10, levelSizeBoxes[1].getBottom() + 40, 150, 100, font);
    genButton.setText("Generate\nMatrix");

    nameBox = TextBox((float)wndW / 10, genButton.getBottom() + 80, 150, 50, font);

    saveButton = Button((float)wndW / 10, nameBox.getBottom() + 20, 150, 100, font);
    saveButton.setText("Save\nLevel");

    optionsContainer.addElement(levelSizeBoxes[0]); optionsContainer.addElement(levelSizeBoxes[1]);
    optionsContainer.addElement(mapWidthText); optionsContainer.addElement(mapHeightText);
    optionsContainer.addElement(genButton);
    optionsContainer.addElement(nameBox); optionsContainer.addElement(saveButton);

    editorRectX = levelSizeBoxes[0].getX() + levelSizeBoxes[0].getWidth() + 100;
    editorRectY = levelSizeBoxes[0].getY();
    editorRectWidth = wndW - editorRectX - wndW / 10;
    editorRectHeight = wndH - editorRectY - wndW / 10;
    
    editorRect = InteractionRect(editorRectX, editorRectY, editorRectWidth, editorRectHeight);
    editorRect.setFillColor(sf::Color(200, 200, 200));
    editorRect.setOutlineColor(sf::Color::Black);
    editorRect.setOutlineThickness(3);

    drawnElements.push_back(&optionsContainer);
    drawnElements.push_back(&editorRect);
}

void App::loop()
{
    while(mainWindow->isOpen())
    {
        mainWindow->clear(sf::Color::White);

        sf::Event event;
        poll(event);

        draw();
        mainWindow->display();
    }
}

void App::poll(sf::Event& event)
{
    while (mainWindow->pollEvent(event))
    {
        optionsContainer.isActivated(event);

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
}

void App::draw()
{
    for(int i = 0; i < drawnElements.size(); i++)
        drawnElements[i]->draw(*mainWindow, sf::RenderStates::Default);

    if(!levelMatrix.empty())
        for(int i = 0; i < levelMatrix.size(); i++)
            for(int j = 0; j < levelMatrix[0].size(); j++)
                levelMatrix[i][j]->draw(*mainWindow, sf::RenderStates::Default);

    if(nameBox.getRight() >= optionsContainer.getRight())
        nameBox.draw(*mainWindow, sf::RenderStates::Default);
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