#include "numBox.h"
#include "textBox.h"
#include "button.h"
#include "pushTile.h"
#include "label.h"
#include "scrollContainer.h"
#include <fstream>
#include <algorithm>
#include <utility>

#define vvi std::vector<std::vector<int>>
#define vvpt std::vector<std::vector<PushTile*>>

enum mapItem
{
    nothing,
    wall,
    box
};

class App
{
private:
    int levelMatrixHeight, levelMatrixWidth, editorRectX, editorRectY, editorRectWidth, editorRectHeight;
    int scrW, scrH, wndW, wndH;

    mapItem selectedObject;
    std::map<mapItem, sf::Color> mapItemColor;
    std::map<mapItem, std::string> mapItemName;

    sf::RenderWindow *mainWindow;
    sf::Font font;

    ScrollContainer optionsContainer, itemContainer;
    NumBox levelHeightBox, levelWidthBox;
    Label mapWidthLabel, mapHeightLabel, fileNameLabel, itemMessageLabel, systemMessageLabel;
    Button genButton, saveButton;
    TextBox nameBox;
    InteractionRect editorRect, itemMessageRect, systemMessageRect;

    vvpt levelMatrix;
    std::vector<Button*> itemButtonsList;
    std::vector<InteractionRect*> drawnElements;

    void init();
    void loop();
    void poll(sf::Event&);
    void draw();
    std::string generateMatrix(int, int, vvpt&, float, float, int, int);
    std::string saveMap(std::string, int, int, vvpt&);
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
    levelMatrixHeight = 0, levelMatrixWidth = 0;
    levelMatrix.resize(0);

    selectedObject = wall;

    mapItemColor[nothing] = sf::Color::White;
    mapItemColor[wall] = sf::Color(100, 100, 100);
    mapItemColor[box] = sf::Color(255, 228, 205);

    mapItemName[nothing] = "None (eraser)";
    mapItemName[wall] = "Wall";
    mapItemName[box] = "Box";

    font.loadFromFile("Resources/Fonts/consola.ttf");

    optionsContainer = ScrollContainer((float)wndW / 20, (float)wndH / 20, wndW / 4, wndH * 7 / 20);

    levelHeightBox = NumBox(optionsContainer.getX() + optionsContainer.getWidth() * 3 / 5, optionsContainer.getY() + 10, 60, 50, font, 2);
    levelWidthBox = NumBox(levelHeightBox.getX(), levelHeightBox.getBottom() + 20, 60, 50, font, 2);

    mapHeightLabel = Label(optionsContainer.getX() + 24, levelHeightBox.getY() + 10, 20, font);
    mapHeightLabel.setText("Map height");

    mapWidthLabel = Label(optionsContainer.getX() + 24, levelWidthBox.getY() + 10, 20, font);
    mapWidthLabel.setText("Map width");

    genButton = Button(optionsContainer.getX() + optionsContainer.getWidth() * 2 / 10, levelWidthBox.getBottom() + 15, 150, 100, font);
    genButton.setText("Generate\nMatrix");

    fileNameLabel = Label(genButton.getX() - 5, genButton.getBottom() + 30, 20, font);
    fileNameLabel.setText("Level file name");

    nameBox = TextBox(genButton.getX(), fileNameLabel.getBottom() + 20, 150, 50, font);

    saveButton = Button(nameBox.getX(), nameBox.getBottom() + 20, 150, 100, font);
    saveButton.setText("Save\nLevel");

    optionsContainer.addElement(levelHeightBox); optionsContainer.addElement(levelWidthBox);
    optionsContainer.addElement(mapWidthLabel); optionsContainer.addElement(mapHeightLabel); optionsContainer.addElement(fileNameLabel);
    optionsContainer.addElement(genButton);
    optionsContainer.addElement(nameBox); optionsContainer.addElement(saveButton);

    editorRectX = optionsContainer.getRight() + 20;
    editorRectY = optionsContainer.getY();
    editorRectWidth = wndW - editorRectX - wndW / 10;
    editorRectHeight = wndH - editorRectY - wndW * 3 / 20;
    
    editorRect = InteractionRect(editorRectX, editorRectY, editorRectWidth, editorRectHeight);
    editorRect.setFillColor(sf::Color(200, 200, 200));
    editorRect.setOutlineColor(sf::Color::Black);
    editorRect.setOutlineThickness(3);

    itemContainer = ScrollContainer(optionsContainer.getX(), optionsContainer.getBottom() + 20,
                                    optionsContainer.getWidth(), editorRect.getHeight() - optionsContainer.getHeight() - 20);

    itemButtonsList.push_back(new Button(itemContainer.getX() + 24, itemContainer.getY() + 10, 200, 50, font));
    itemButtonsList[0]->setText(mapItemName[(mapItem)0]);
    itemButtonsList[0]->setInactiveColor(mapItemColor[(mapItem)0]);
    for(int i = 1; i < mapItemName.size(); i++)
    {
        itemButtonsList.push_back(new Button(itemButtonsList[0]->getX(), itemButtonsList[i - 1]->getBottom() + 10, 200, 50, font));
        itemButtonsList[itemButtonsList.size() - 1]->setText(mapItemName[(mapItem)i]);
        itemButtonsList[itemButtonsList.size() - 1]->setInactiveColor(mapItemColor[(mapItem)i]);
    }

    for(int i = 0; i < itemButtonsList.size(); i++)
        itemContainer.addElement(*itemButtonsList[i]);

    itemMessageRect = InteractionRect(itemContainer.getX(), itemContainer.getBottom() + 10, wndW * 4 / 10, wndH / 10);
    itemMessageRect.setOutlineThickness(1); itemMessageRect.setOutlineColor(sf::Color::Black);

    systemMessageRect = InteractionRect(itemMessageRect.getRight() + 20, itemMessageRect.getY(), wndW - itemMessageRect.getRight() - 20 - wndW / 10, wndH / 10);
    systemMessageRect.setOutlineThickness(1); systemMessageRect.setOutlineColor(sf::Color::Black);

    itemMessageLabel = Label(itemMessageRect.getX() + 10, itemMessageRect.getY() + 10, 15, font);
    itemMessageLabel.setText("Item related messages will be displayed here");

    systemMessageLabel = Label(systemMessageRect.getX() + 10, systemMessageRect.getY() + 10, 15, font);
    systemMessageLabel.setText("System related messages will be displayed here");

    drawnElements.push_back(&optionsContainer);
    drawnElements.push_back(&itemContainer);
    drawnElements.push_back(&editorRect);
    drawnElements.push_back(&itemMessageRect);
    drawnElements.push_back(&systemMessageRect);
    drawnElements.push_back(&itemMessageLabel);
    drawnElements.push_back(&systemMessageLabel);
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
        itemContainer.isActivated(event);

        if(levelHeightBox.getY() >= optionsContainer.getY() && levelHeightBox.getBottom() <= optionsContainer.getBottom())
            levelHeightBox.isActivated(event);

        if(levelWidthBox.getY() >= optionsContainer.getY() && levelWidthBox.getBottom() <= optionsContainer.getBottom())
            levelWidthBox.isActivated(event);

        if(nameBox.getY() >= optionsContainer.getY() && nameBox.getBottom() <= optionsContainer.getBottom())
            nameBox.isActivated(event); 

        if(genButton.isActivated(event) && genButton.getY() >= optionsContainer.getY() && genButton.getBottom() <= optionsContainer.getBottom())
            systemMessageLabel.setText(generateMatrix(levelMatrixHeight, levelMatrixWidth, levelMatrix, editorRectX, editorRectY, editorRectWidth, editorRectHeight));

        if(saveButton.isActivated(event) && saveButton.getY() >= optionsContainer.getY() && saveButton.getBottom() <= optionsContainer.getBottom())
            systemMessageLabel.setText(fitText(saveMap(nameBox.getText(), levelMatrixHeight, levelMatrixWidth, levelMatrix), 
                                               systemMessageLabel.getCharacterSize(), systemMessageRect.getWidth(), systemMessageRect.getHeight()));

        for(int i = 0; i < itemButtonsList.size(); i++)
            if(itemButtonsList[i]->isActivated(event))
            {
                selectedObject = (mapItem)i;
                itemMessageLabel.setText("Selected item \"" + mapItemName[(mapItem)i] + "\"");
            }

        if(!levelHeightBox.getText().empty())
            levelMatrixHeight = std::stoi(levelHeightBox.getText());
        if(!levelWidthBox.getText().empty())
            levelMatrixWidth = std::stoi(levelWidthBox.getText());
        
        if(!levelMatrix.empty())
            for(int i = 0; i < levelMatrix.size(); i++)
                for(int j = 0; j < levelMatrix[i].size(); j++)
                    if(levelMatrix[i][j]->isActivated(event))
                        levelMatrix[i][j]->setState(selectedObject, mapItemColor[selectedObject]);

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
            for(int j = 0; j < levelMatrix[i].size(); j++)
                levelMatrix[i][j]->draw(*mainWindow, sf::RenderStates::Default);

    if(nameBox.getRight() >= optionsContainer.getRight())
        nameBox.draw(*mainWindow, sf::RenderStates::Default);
}

std::string App::generateMatrix(int n, int m, vvpt& matrix, float posX, float posY, int width, int height)
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
        return "Please, input sizes greater than 0!";

    matrix.resize(2 * n + 1);
    for(int i = 0; i < matrix.size(); i++)
    {
        matrix[i].resize(m + (i % 2 != 0));
        for(int j = 0; j < matrix[i].size(); j++)
            matrix[i][j] = new PushTile(posX + width * j / (m + (i % 2 != 0)), posY + height * i / (2 * n + 1),
                                        width / (m + (i % 2 != 0)), height / (2 * n + 1));
    }

    return "Successfully generated a " + std::to_string(n) + " by " + std::to_string(m) + " cell level";
}

std::string App::saveMap(std::string filename, int n, int m, vvpt& matrix)
{
    if(filename.empty())
        return "Input the name for your map file!";

    std::ofstream levelFile;
    vvi stateMatrix;
    bool wallEncountered;

    for(int i = 0; i < matrix.size(); i++)
    {
        stateMatrix.resize(matrix.size() + 1);
        for(int j = 0; j < matrix[i].size(); j++)
            stateMatrix[i].push_back(matrix[i][j]->getState());
    }

    levelFile.open(".\\Levels\\" + filename + ".txt");
    levelFile << n << ' ' << m << '\n';
    for(int i = 0; i < stateMatrix.size(); i++)
    {
        for(int j = 0; j < stateMatrix[i].size(); j++)
        {
            bool doSpace = (j < stateMatrix[i].size() - 1) && stateMatrix[i][j + 1] != -1;
            if(stateMatrix[i][j] == -1)
                continue;
            levelFile << stateMatrix[i][j];
            if(doSpace)
                levelFile << ' ';
        }
        if(i < stateMatrix.size() - 1)
            levelFile << "\n";
    }
    levelFile.close();

    return "Map saved as " + nameBox.getText() + ".txt at .\\Levels\\";
}