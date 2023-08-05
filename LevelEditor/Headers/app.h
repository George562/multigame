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
#define vvir std::vector<std::vector<InteractionRect*>>

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
    float levelMatrixHeight, levelMatrixWidth, editorViewX, editorViewY, editorViewWidth, editorViewHeight;
    float wndW, wndH;

    bool wheelPressed = false;
    float zoomFactor = 1.0f;
    sf::Vector2f prevPos = sf::Vector2f(0, 0);
    windowMode mode = editing;

    mapItem selectedObject;
    std::map<mapItem, sf::Color> mapItemColor;
    std::map<mapItem, std::string> mapItemName;

    sf::RenderWindow *mainWindow;
    sf::View mainView;
    sf::Font font;

    ScrollContainer optionsContainer, itemContainer;
    NumBox levelHeightBox, levelWidthBox;
    Label mapWidthLabel, mapHeightLabel, itemMessageLabel, systemMessageLabel;
    Button genButton, saveModeButton;
    InteractionRect itemMessageRect, systemMessageRect, editorRect;
    vvpt levelMatrix;
    std::vector<Button*> itemButtonsList;
    sf::View editorView;

    Label fileNameLabel, filePathLabel;
    TextBox filePathBox, fileNameBox;
    Button saveFileButton, backButton, overWriteButton;

    std::vector<InteractionRect*> universalDrawnElements, editorDrawnElements, saveDrawnElements;
    vvir levelMatrixSquares;

    void init();
    void loop();
    void poll(sf::Event&);
    void interactionViewPoll(sf::Event&);
    void draw();
    std::string generateMatrix(int, int);
    std::string saveMap(std::string, std::string, int, int, bool);

public:
    App(sf::RenderWindow&);
    void start(){ init(); loop(); }
};

App::App(sf::RenderWindow& window) 
{
    mainWindow = &window;
    mainView = window.getView();

    wndW = mainWindow->getSize().x;
    wndH = mainWindow->getSize().y;
}

void App::init()
{
    levelMatrixHeight = 0, levelMatrixWidth = 0;
    levelMatrix.resize(0);
    levelMatrixSquares.resize(0);

    selectedObject = wall;

    mapItemColor[nothing] = sf::Color(100, 100, 100);
    mapItemColor[wall] = sf::Color::White;
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

    editorViewX = optionsContainer.getRight() + 20;
    editorViewY = optionsContainer.getY();
    editorViewWidth = wndW - editorViewX - wndW / 10;
    editorViewHeight = wndH - editorViewY - wndW * 3 / 20;
    
    editorRect = InteractionRect(editorViewX, editorViewY, editorViewWidth, editorViewHeight);
    editorRect.setFillColor(sf::Color(0, 0, 100));
    editorRect.setOutlineColor(sf::Color::Black);
    editorRect.setOutlineThickness(2);

    editorView = sf::View(sf::FloatRect(0, 0, editorViewWidth, editorViewHeight));
    editorView.setViewport(sf::FloatRect(editorViewX / wndW, editorViewY / wndH, editorViewWidth / wndW, editorViewHeight / wndH));

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
                systemMessageLabel.setText(generateMatrix(levelMatrixHeight, levelMatrixWidth));

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
                interactionViewPoll(event);

            break;
        
        case saving:
            fileNameBox.isActivated(event); 
            filePathBox.isActivated(event);

            if(saveFileButton.isActivated(event))
                systemMessageLabel.setText(fitText(saveMap(filePathBox.getText(), fileNameBox.getText(), levelMatrixHeight, levelMatrixWidth, false), 
                                                systemMessageLabel.getCharacterSize(), systemMessageRect.getWidth(), systemMessageRect.getHeight()));

            if(overWriteButton.isActivated(event))
                systemMessageLabel.setText(fitText(saveMap(filePathBox.getText(), fileNameBox.getText(), levelMatrixHeight, levelMatrixWidth, true), 
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
            mainWindow->setView(mainView);
    }
}

void App::interactionViewPoll(sf::Event& event)
{
    if(!in(wndW * editorView.getViewport().left, wndH * editorView.getViewport().top,
           wndW * editorView.getViewport().width, wndH * editorView.getViewport().height, mainWindow->mapPixelToCoords(sf::Mouse::getPosition(*mainWindow), mainView)))
    {
        wheelPressed = false;
        return;
    }

    sf::Event viewEvent = event;
    viewEvent.type = event.type;

    if(viewEvent.type == sf::Event::MouseButtonPressed || viewEvent.type == sf::Event::MouseButtonReleased)
    {
        viewEvent.mouseButton.button = event.mouseButton.button;
        viewEvent.mouseButton.x = ((sf::RenderTarget*)mainWindow)->mapPixelToCoords(sf::Vector2i(event.mouseButton.x, event.mouseButton.y), editorView).x;
        viewEvent.mouseButton.y = ((sf::RenderTarget*)mainWindow)->mapPixelToCoords(sf::Vector2i(event.mouseButton.x, event.mouseButton.y), editorView).y;
    }

    if(viewEvent.type == sf::Event::MouseMoved)
    {
        viewEvent.mouseMove.x = ((sf::RenderTarget*)mainWindow)->mapPixelToCoords(sf::Vector2i(event.mouseMove.x, event.mouseMove.y), editorView).x;
        viewEvent.mouseMove.y = ((sf::RenderTarget*)mainWindow)->mapPixelToCoords(sf::Vector2i(event.mouseMove.x, event.mouseMove.y), editorView).y;
    }
    if(viewEvent.type == sf::Event::MouseWheelMoved)
    {
        viewEvent.mouseWheel.delta = event.mouseWheel.delta;
        viewEvent.mouseWheel.x = ((sf::RenderTarget*)mainWindow)->mapPixelToCoords(sf::Vector2i(event.mouseWheel.x, event.mouseWheel.y), editorView).x;
        viewEvent.mouseWheel.y = ((sf::RenderTarget*)mainWindow)->mapPixelToCoords(sf::Vector2i(event.mouseWheel.x, event.mouseWheel.y), editorView).y;
    }
    if(viewEvent.type == sf::Event::MouseWheelScrolled)
    {
        viewEvent.mouseWheelScroll.delta = event.mouseWheelScroll.delta;
        viewEvent.mouseWheelScroll.wheel = event.mouseWheelScroll.wheel;
        viewEvent.mouseWheelScroll.x = ((sf::RenderTarget*)mainWindow)->mapPixelToCoords(sf::Vector2i(event.mouseWheelScroll.x, event.mouseWheelScroll.y), editorView).x;
        viewEvent.mouseWheelScroll.y = ((sf::RenderTarget*)mainWindow)->mapPixelToCoords(sf::Vector2i(event.mouseWheelScroll.x, event.mouseWheelScroll.y), editorView).y;
    }

    if(viewEvent.type == sf::Event::MouseButtonPressed && viewEvent.mouseButton.button == sf::Mouse::Middle)
    {
        wheelPressed = true;
        prevPos = sf::Vector2f(event.mouseWheel.x, event.mouseWheel.y);
    }

    if(viewEvent.type == sf::Event::MouseButtonReleased && viewEvent.mouseButton.button == sf::Mouse::Middle)
        wheelPressed = false;

    if(viewEvent.type == sf::Event::MouseMoved && wheelPressed)
    {
        float deltaX = (prevPos.x - event.mouseMove.x) * zoomFactor;
        float deltaY = (prevPos.y - event.mouseMove.y) * zoomFactor;
        editorView.move(deltaX, deltaY);
        prevPos = sf::Vector2f(event.mouseMove.x, event.mouseMove.y);
    }

    if(viewEvent.type == sf::Event::MouseWheelScrolled)
    {
        zoomFactor *= viewEvent.mouseWheelScroll.delta < 0 ? 1.1f : 0.9f;
        editorView.zoom(viewEvent.mouseWheelScroll.delta < 0 ? 1.1f : 0.9f);
    }

    for(int i = 0; i < levelMatrix.size(); i++)
        for(int j = 0; j < levelMatrix[i].size(); j++)
            if(levelMatrix[i][j]->isActivated(viewEvent))
                levelMatrix[i][j]->setState(selectedObject, mapItemColor[selectedObject]);
}

void App::draw()
{
    switch (mode)
    {
    case editing:
       for(int i = 0; i < editorDrawnElements.size(); i++)
            editorDrawnElements[i]->draw(*mainWindow, sf::RenderStates::Default);

        if(!levelMatrix.empty())
        {
            mainWindow->setView(editorView);
            for(int i = 0; i < levelMatrix.size(); i++)
                    for(int j = 0; j < levelMatrix[i].size(); j++)
                            levelMatrix[i][j]->draw(*mainWindow, sf::RenderStates::Default);
            
            for(int i = 0; i < levelMatrixSquares.size(); i++)
                    for(int j = 0; j < levelMatrixSquares[i].size(); j++)
                            levelMatrixSquares[i][j]->draw(*mainWindow, sf::RenderStates::Default);
            mainWindow->setView(mainView);
        }
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

std::string App::generateMatrix(int n, int m)
{
    float columnWidth = 50, rowWidth = 2 * columnWidth;
    float rowHeight = 50, columnHeight = 2 * rowHeight;

    if(!levelMatrix.empty())
    {
        for(int i = 0; i < levelMatrix.size(); i++)
        {
            for(int j = 0; j < levelMatrix[0].size(); j++)
                levelMatrix[i].pop_back();
            levelMatrix[i].clear();
        }
        for(int i = 0; i < levelMatrixSquares.size(); i++)
        {
            for(int j = 0; j < levelMatrixSquares[0].size(); j++)
                levelMatrixSquares[i].pop_back();
            levelMatrixSquares[i].clear();
        }
        levelMatrixSquares.resize(0);

        editorView.zoom(1 / zoomFactor);
        editorView.setCenter(editorViewX, editorViewY);
        zoomFactor = 1.0f;
    }

    if(n == 0 || m == 0)
        return "Please, input sizes greater than 0!";

    levelMatrix.resize(2 * n + 1);
    levelMatrixSquares.resize(n + 1);
    for(int i = 0; i < levelMatrix.size(); i++)
    {
        levelMatrix[i].resize(m + (i % 2 != 0));
        for(int j = 0; j < levelMatrix[i].size(); j++)
        {
            if(i % 2 == 0)
                levelMatrix[i][j] = new PushTile(editorView.getViewport().left + (j + 1) * columnWidth + j * rowWidth,
                                                 editorView.getViewport().top + i / 2 * (columnHeight + rowHeight), rowWidth, rowHeight);
            else
                levelMatrix[i][j] = new PushTile(editorView.getViewport().left + j * (columnWidth + rowWidth), editorView.getViewport().top + 
                                                 (i + 1) / 2 * rowHeight + (i - 1) / 2 * columnHeight, columnWidth, columnHeight);
            
            levelMatrix[i][j]->setOutlineColor(sf::Color::Black);
            levelMatrix[i][j]->setOutlineThickness(1);
        }
    }

    for(int i = 0; i < levelMatrixSquares.size(); i++)
    {
        levelMatrixSquares[i].resize(m + 1);
        for(int j = 0; j < levelMatrixSquares[i].size(); j++)
        {
            levelMatrixSquares[i][j] = new InteractionRect(j * (columnWidth + rowWidth), i * (columnHeight + rowHeight),
                                            columnWidth, rowHeight);
            levelMatrixSquares[i][j]->setFillColor(mapItemColor[wall]);
            levelMatrixSquares[i][j]->setOutlineThickness(1);
            levelMatrixSquares[i][j]->setOutlineColor(sf::Color::Black);
        }
    }

    return "Successfully generated a " + std::to_string(n) + " by " + std::to_string(m) + " cell level";
}

std::string App::saveMap(std::string path, std::string fileName, int n, int m, bool forceOverwrite)
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

    for(int i = 0; i < levelMatrix.size(); i++)
    {
        stateMatrix.resize(levelMatrix.size() + 1);
        for(int j = 0; j < levelMatrix[i].size(); j++)
            stateMatrix[i].push_back(levelMatrix[i][j]->getState());
    }

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