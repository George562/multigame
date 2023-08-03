#include "numBox.h"
#include "textBox.h"
#include "button.h"
#include "pushTile.h"
#include "label.h"
#include "scrollContainer.h"
#include <fstream>
#include <filesystem>
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

enum windowMode
{
    editing,
    saving
};

class App
{
private:
    int levelMatrixHeight, levelMatrixWidth, editorRectX, editorRectY, editorRectWidth, editorRectHeight;
    int scrW, scrH, wndW, wndH;
    windowMode mode = editing;

    mapItem selectedObject;
    std::map<mapItem, sf::Color> mapItemColor;
    std::map<mapItem, std::string> mapItemName;

    sf::RenderWindow *mainWindow;
    sf::Font font;

    ScrollContainer optionsContainer, itemContainer;
    NumBox levelHeightBox, levelWidthBox;
    Label mapWidthLabel, mapHeightLabel, itemMessageLabel, systemMessageLabel;
    Button genButton, saveModeButton;
    InteractionRect editorRect, itemMessageRect, systemMessageRect;
    vvpt levelMatrix;
    std::vector<Button*> itemButtonsList;

    Label fileNameLabel, filePathLabel;
    TextBox filePathBox, fileNameBox;
    Button saveFileButton, backButton, overWriteButton;

    std::vector<InteractionRect*> universalDrawnElements, editorDrawnElements, saveDrawnElements;

    void init();
    void loop();
    void poll(sf::Event&);
    void draw();
    std::string generateMatrix(int, int, vvpt&, float, float, int, int);
    std::string saveMap(std::string, std::string, int, int, vvpt&, bool);
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

    // [Editor window elements]. All you need for actual level editing
    optionsContainer = ScrollContainer((float)wndW / 20, (float)wndH / 20, wndW / 4, wndH * 7 / 20);

    levelHeightBox = NumBox(optionsContainer.getX() + optionsContainer.getWidth() * 3 / 5, optionsContainer.getY() + 10, 60, 50, font, 2);
    levelWidthBox = NumBox(levelHeightBox.getX(), levelHeightBox.getBottom() + 20, 60, 50, font, 2);

    mapHeightLabel = Label(optionsContainer.getX() + 24, levelHeightBox.getY() + 10, 20, font);
    mapHeightLabel.setText("Map height");

    mapWidthLabel = Label(optionsContainer.getX() + 24, levelWidthBox.getY() + 10, 20, font);
    mapWidthLabel.setText("Map width");

    genButton = Button(optionsContainer.getX() + optionsContainer.getWidth() * 2 / 10, levelWidthBox.getBottom() + 15, 150, 100, font);
    genButton.setText("Generate\nMatrix");

    saveModeButton = Button(genButton.getX(), genButton.getBottom() + 20, 150, 100, font);
    saveModeButton.setText("Save\nLevel");

    optionsContainer.addElement(levelHeightBox); optionsContainer.addElement(levelWidthBox);
    optionsContainer.addElement(mapWidthLabel); optionsContainer.addElement(mapHeightLabel);
    optionsContainer.addElement(genButton);
    optionsContainer.addElement(saveModeButton);

    editorRectX = optionsContainer.getRight() + 20;
    editorRectY = optionsContainer.getY();
    editorRectWidth = wndW - editorRectX - wndW / 10;
    editorRectHeight = wndH - editorRectY - wndW * 3 / 20;
    
    editorRect = InteractionRect(editorRectX, editorRectY, editorRectWidth, editorRectHeight);
    editorRect.setFillColor(sf::Color(0, 0, 128));
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

    editorDrawnElements.push_back(&optionsContainer);
    editorDrawnElements.push_back(&itemContainer);
    editorDrawnElements.push_back(&editorRect);
    editorDrawnElements.push_back(&itemMessageRect);
    editorDrawnElements.push_back(&itemMessageLabel);

    universalDrawnElements.push_back(&systemMessageRect);
    universalDrawnElements.push_back(&systemMessageLabel);
    // Done [Editor window elements] //

    // [Save window elements]. Simple enough if you know at what path you want to save your file beforehand. 
    // A graphical solution would be way too overkill for what is a very situational app.
    fileNameLabel = Label(wndW / 10, wndH / 10, 20, font);
    fileNameLabel.setText("Level file name");
    
    fileNameBox = TextBox(fileNameLabel.getX(), fileNameLabel.getY() + 30, wndW / 2 - wndW / 10, 40, font);

    filePathLabel = Label(fileNameBox.getX(), fileNameBox.getBottom() + 40, 20, font);
    filePathLabel.setText("Level file path");

    filePathBox = TextBox(filePathLabel.getX(), filePathLabel.getBottom() + 30, wndW * 3 / 4 - wndW / 10, 25, font);

    saveFileButton = Button(fileNameBox.getX(), filePathBox.getBottom() + 40, 200, 100, font);
    saveFileButton.setText("Save\nfile");

    backButton = Button(wndW / 2, saveFileButton.getY() + 50, 300, 120, font);
    backButton.setText("Back");

    overWriteButton = Button(saveFileButton.getX(), saveFileButton.getBottom() + 20, 200, 100, font);
    overWriteButton.setText("Overwrite file");

    saveDrawnElements.push_back(&fileNameLabel);
    saveDrawnElements.push_back(&fileNameBox);
    saveDrawnElements.push_back(&filePathLabel);
    saveDrawnElements.push_back(&filePathBox);
    saveDrawnElements.push_back(&saveFileButton);
    saveDrawnElements.push_back(&backButton);
    saveDrawnElements.push_back(&overWriteButton);
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
        switch (mode)
        {
        case editing:
            optionsContainer.isActivated(event);
            itemContainer.isActivated(event);

            if(levelHeightBox.getY() >= optionsContainer.getY() && levelHeightBox.getBottom() <= optionsContainer.getBottom())
                levelHeightBox.isActivated(event);

            if(levelWidthBox.getY() >= optionsContainer.getY() && levelWidthBox.getBottom() <= optionsContainer.getBottom())
                levelWidthBox.isActivated(event);

            if(genButton.isActivated(event) && genButton.getY() >= optionsContainer.getY() && genButton.getBottom() <= optionsContainer.getBottom())
                systemMessageLabel.setText(generateMatrix(levelMatrixHeight, levelMatrixWidth, levelMatrix, editorRectX, editorRectY, editorRectWidth, editorRectHeight));

            if(saveModeButton.isActivated(event) && saveModeButton.getY() >= optionsContainer.getY() && saveModeButton.getBottom() <= optionsContainer.getBottom())
                mode = saving;

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
            break;
        
        case saving:
            fileNameBox.isActivated(event); 
            filePathBox.isActivated(event);

            if(saveFileButton.isActivated(event))
                systemMessageLabel.setText(fitText(saveMap(filePathBox.getText(), fileNameBox.getText(), levelMatrixHeight, levelMatrixWidth, levelMatrix, false), 
                                                systemMessageLabel.getCharacterSize(), systemMessageRect.getWidth(), systemMessageRect.getHeight()));

            if(overWriteButton.isActivated(event))
                systemMessageLabel.setText(fitText(saveMap(filePathBox.getText(), fileNameBox.getText(), levelMatrixHeight, levelMatrixWidth, levelMatrix, true), 
                                                systemMessageLabel.getCharacterSize(), systemMessageRect.getWidth(), systemMessageRect.getHeight()));

            if(backButton.isActivated(event))
                mode = editing;
            break;

        default:
            break;
        }

        if(event.type == sf::Event::Closed)
            mainWindow->close();

        if(event.type == sf::Event::Resized)
            mainWindow->setView(sf::View(sf::FloatRect(0, 0, event.size.width, event.size.height)));
    }
}

void App::draw()
{
    switch (mode)
    {
    case editing:
       for(int i = 0; i < editorDrawnElements.size(); i++)
            editorDrawnElements[i]->draw(*mainWindow, sf::RenderStates::Default);

        if(!levelMatrix.empty())
            for(int i = 0; i < levelMatrix.size(); i++)
                for(int j = 0; j < levelMatrix[i].size(); j++)
                    levelMatrix[i][j]->draw(*mainWindow, sf::RenderStates::Default);
        break;

    case saving:
        for(int i = 0; i < saveDrawnElements.size(); i++)
            saveDrawnElements[i]->draw(*mainWindow, sf::RenderStates::Default);
        break;

    default:
        break;
    }

    for(int i = 0; i < universalDrawnElements.size(); i++)
        universalDrawnElements[i]->draw(*mainWindow, sf::RenderStates::Default);
}

std::string App::generateMatrix(int n, int m, vvpt& matrix, float posX, float posY, int width, int height)
{
    float columnWidth = width / (3 * m + 1), rowWidth = 2 * columnWidth;
    float rowHeight = height / (3 * n + 1), columnHeight = 2 * rowHeight;

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
        {
            if(i % 2 == 0)
                matrix[i][j] = new PushTile(posX + (j + 1) * columnWidth + j * rowWidth, posY + i / 2 * (columnHeight + rowHeight),
                                            rowWidth, rowHeight);
            else
                matrix[i][j] = new PushTile(posX + j * (columnWidth + rowWidth), posY + (i + 1) / 2 * rowHeight + (i - 1) / 2 * columnHeight,
                                            columnWidth, columnHeight);
            matrix[i][j]->setOutlineColor(sf::Color::Black);
            matrix[i][j]->setOutlineThickness(1);
        }
    }

    return "Successfully generated a " + std::to_string(n) + " by " + std::to_string(m) + " cell level";
}

std::string App::saveMap(std::string path, std::string fileName, int n, int m, vvpt& matrix, bool forceOverwrite)
{
    char forbiddenChars[9]{'\\', '/', ':', '\"', '*', '?', '<', '>', '|'};

    if(fileName.empty())
        return "Input the name for your map file!";
    
    for(int i = 0; i < 9; i++)
        if(std::find(fileName.begin(), fileName.end(), forbiddenChars[i]) != fileName.end())
            return "File name cannot have any of theese symbols: \\ / : \" * ? < > |";

    std::string filePath = path;
    std::replace(filePath.begin(), filePath.end(), '/', '\\');

    if(!std::filesystem::exists(std::filesystem::path(filePath)))
        return "Input path does not exist!";

    if(std::filesystem::exists(std::filesystem::path(filePath + "\\" + fileName + ".txt")) && !forceOverwrite)
        return "File already exists! If you wish to overwrite, click the \"Overwrite\" button.";

    std::ofstream levelFile;
    vvi stateMatrix;
    bool wallEncountered;

    for(int i = 0; i < matrix.size(); i++)
    {
        stateMatrix.resize(matrix.size() + 1);
        for(int j = 0; j < matrix[i].size(); j++)
            stateMatrix[i].push_back(matrix[i][j]->getState());
    }

    // levelFile.open(".\\Levels\\" + fileName + ".txt");
    levelFile.open(filePath + "\\" + fileName + ".txt");
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

    return "Map saved as " + fileName + ".txt at \"" + filePath + "\"";
}