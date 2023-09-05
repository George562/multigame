#include "numBox.h"
#include "textBox.h"
#include "button.h"
#include "pushTile.h"
#include "label.h"
#include "scrollContainer.h"
#include <fstream>
#include <filesystem>
#include <algorithm>
#include <vector>
#include <map>
#include <utility>

#define vvi std::vector<std::vector<int>>
#define mmivpt std::map<mapItem, std::vector<PushTile*>>
#define vvpt std::vector<std::vector<PushTile*>>
#define vvir std::vector<std::vector<InteractionRect*>>

enum mapItem
{
    nothing,
    wall,
    box,
    portal
};

enum windowMode
{
    editing,
    saving
};

class App
{
private:
    float levelMatrixHeight, levelMatrixWidth, editorViewX, editorViewY, editorViewWidth, editorViewHeight, itemOptionX, itemOptionY, itemOptionWidth, itemOptionHeight;
    float wndW, wndH;
    float tileWidth = 60, tileHeight = 60, columnHeight = 360, rowWidth = 360, columnWidth = 60, rowHeight = 60;
    std::vector<int> arrowMoveAmount;
    int arrowMoveAmountIndex = 0;

    float zoomFactor = 1.0f;
    sf::Vector2i prevPos = sf::Vector2i(0, 0);
    windowMode mode = editing;
    bool isMapItemSelected = false;
    bool isItemHeld = false;

    mapItem selectedObject;
    std::map<mapItem, sf::Color> mapItemColor;
    std::map<mapItem, std::string> mapItemName;

    sf::RenderWindow *mainWindow;
    sf::View mainView;
    sf::Font font;

    ScrollContainer optionsContainer, itemContainer;
    NumBox levelHeightBox, levelWidthBox;
    Label mapWidthLabel, mapHeightLabel, itemMessageLabel, systemMessageLabel, itemOptionLabelX, itemOptionLabelY, itemEnterHintLabel;
    Button genButton, saveModeButton;
    InteractionRect itemMessageRect, systemMessageRect, editorRect, itemOptionRect;
    TextBox itemOptionXBox, itemOptionYBox;
    std::pair<mapItem, PushTile*> selectedMapItem;
    mmivpt mapItemDict;
    vvpt levelMatrix;
    std::vector<Button*> itemButtonsList;
    sf::View editorView;

    Label fileNameLabel, filePathLabel;
    TextBox filePathBox, fileNameBox;
    Button saveFileButton, backButton, overWriteButton;

    std::vector<InteractionRect*> universalDrawnElements, editorDrawnElements, saveDrawnElements, editorItemPropertyElements;
    vvir levelMatrixSquares;

    void init();
    void loop();
    void poll(sf::Event&);
    sf::Event convertToViewEvent(sf::Event&, sf::View);
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
    arrowMoveAmount.push_back(1);
    arrowMoveAmount.push_back(5);
    arrowMoveAmount.push_back(50);
    arrowMoveAmount.push_back(200);

    selectedObject = wall;

    mapItemColor[nothing] = sf::Color(100, 100, 100);
    mapItemColor[wall] = sf::Color(200, 200, 200);
    mapItemColor[box] = sf::Color(255, 228, 205);
    mapItemColor[portal] = sf::Color(150, 0, 200);

    mapItemName[nothing] = "None (eraser)";
    mapItemName[wall] = "Wall";
    mapItemName[box] = "Box";
    mapItemName[portal] = "Portal";

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


    itemOptionWidth = editorRect.getWidth() / 2;
    itemOptionHeight = editorRect.getHeight() / 5;
    itemOptionX = editorRect.getRight() - itemOptionWidth;
    itemOptionY = editorRect.getY();
    
    itemOptionRect = InteractionRect(itemOptionX, itemOptionY, itemOptionWidth, itemOptionHeight);
    itemOptionRect.setFillColor(sf::Color::White);
    itemOptionRect.setOutlineColor(sf::Color::Black);
    itemOptionRect.setOutlineThickness(2);

    itemOptionLabelX = Label(itemOptionRect.getX() + 10, itemOptionRect.getY() + 10, itemOptionHeight / 3, font);
    itemOptionLabelX.setText("X:");
    itemOptionXBox = TextBox(itemOptionLabelX.getRight() + 10, itemOptionLabelX.getY() + 5,
                             itemOptionRect.getWidth() / 4, itemOptionHeight / 3, font);

    itemOptionLabelY = Label(itemOptionXBox.getRight() + 30, itemOptionRect.getY() + 10, itemOptionHeight / 3, font);
    itemOptionLabelY.setText("Y:");
    itemOptionYBox = TextBox(itemOptionLabelY.getRight() + 10, itemOptionLabelY.getY() + 5,
                             itemOptionRect.getWidth() / 4, itemOptionHeight / 3, font);
    itemEnterHintLabel = Label(itemOptionRect.getX() + 10, itemOptionYBox.getBottom() + 10, itemOptionHeight / 7, font);
    itemEnterHintLabel.setText("Press \"Enter\" to set the\nobject's coordinates");


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

    editorItemPropertyElements.push_back(&itemOptionRect);
    editorItemPropertyElements.push_back(&itemOptionLabelX);
    editorItemPropertyElements.push_back(&itemOptionXBox);
    editorItemPropertyElements.push_back(&itemOptionLabelY);
    editorItemPropertyElements.push_back(&itemOptionYBox);
    editorItemPropertyElements.push_back(&itemEnterHintLabel);

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

            if(isMapItemSelected)
            {
                itemOptionXBox.isActivated(event);
                itemOptionYBox.isActivated(event);
            }

            if(!levelMatrix.empty() && 
                (!isMapItemSelected || !in(itemOptionRect.getX(), itemOptionRect.getY(), itemOptionRect.getWidth(), itemOptionRect.getHeight(), event.mouseButton)))
                interactionViewPoll(event);

            if(event.type == sf::Event::KeyPressed)
            {
                arrowMoveAmountIndex += event.key.code == sf::Keyboard::Key::Add;
                arrowMoveAmountIndex -= event.key.code == sf::Keyboard::Key::Subtract;
                if(arrowMoveAmountIndex >= (int)(arrowMoveAmount.size()))
                    arrowMoveAmountIndex = 0;
                if(arrowMoveAmountIndex < 0)
                    arrowMoveAmountIndex = (int)(arrowMoveAmount.size()) - 1;
                if(event.key.code == sf::Keyboard::Key::Add || event.key.code == sf::Keyboard::Key::Subtract)
                    systemMessageLabel.setText("Changed arrow move amount to " + 
                                            std::to_string(arrowMoveAmount[arrowMoveAmountIndex]) + " pixels.");
            }

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

sf::Event App::convertToViewEvent(sf::Event& event, sf::View view)
{
    if(!in(wndW * view.getViewport().left, wndH * editorView.getViewport().top,
           wndW * editorView.getViewport().width, wndH * editorView.getViewport().height, mainWindow->mapPixelToCoords(sf::Mouse::getPosition(*mainWindow), mainView)))
        return sf::Event();

    sf::Event viewEvent = event;

    if(viewEvent.type == sf::Event::MouseButtonPressed || viewEvent.type == sf::Event::MouseButtonReleased)
    {
        viewEvent.mouseButton.x = mainWindow->mapPixelToCoords(sf::Vector2i(event.mouseButton.x, event.mouseButton.y), editorView).x;
        viewEvent.mouseButton.y = mainWindow->mapPixelToCoords(sf::Vector2i(event.mouseButton.x, event.mouseButton.y), editorView).y;
    }

    if(viewEvent.type == sf::Event::MouseMoved)
    {
        viewEvent.mouseMove.x = mainWindow->mapPixelToCoords(sf::Vector2i(event.mouseMove.x, event.mouseMove.y), editorView).x;
        viewEvent.mouseMove.y = mainWindow->mapPixelToCoords(sf::Vector2i(event.mouseMove.x, event.mouseMove.y), editorView).y;
    }
    if(viewEvent.type == sf::Event::MouseWheelMoved)
    {
        viewEvent.mouseWheel.x = mainWindow->mapPixelToCoords(sf::Vector2i(event.mouseWheel.x, event.mouseWheel.y), editorView).x;
        viewEvent.mouseWheel.y = mainWindow->mapPixelToCoords(sf::Vector2i(event.mouseWheel.x, event.mouseWheel.y), editorView).y;
    }
    if(viewEvent.type == sf::Event::MouseWheelScrolled)
    {
        viewEvent.mouseWheelScroll.x = mainWindow->mapPixelToCoords(sf::Vector2i(event.mouseWheelScroll.x, event.mouseWheelScroll.y), editorView).x;
        viewEvent.mouseWheelScroll.y = mainWindow->mapPixelToCoords(sf::Vector2i(event.mouseWheelScroll.x, event.mouseWheelScroll.y), editorView).y;
    }

    return viewEvent;
}

void App::interactionViewPoll(sf::Event& event)
{
    sf::Event viewEvent = convertToViewEvent(event, editorView);

    if(viewEvent.type == sf::Event::MouseMoved && sf::Mouse::isButtonPressed(sf::Mouse::Middle))
        editorView.move(-sf::Vector2f(sf::Mouse::getPosition() - prevPos) * zoomFactor);
    prevPos = sf::Mouse::getPosition();

    if(viewEvent.type == sf::Event::MouseWheelScrolled)
    {
        zoomFactor *= viewEvent.mouseWheelScroll.delta < 0 ? 1.1f : (1.f / 1.1f);
        editorView.zoom(viewEvent.mouseWheelScroll.delta < 0 ? 1.1f : (1.f / 1.1f));
    }

    if(viewEvent.type == sf::Event::MouseButtonPressed)
    {
        if(viewEvent.mouseButton.button == sf::Mouse::Button::Left || viewEvent.mouseButton.button == sf::Mouse::Button::Right)
        {
            isMapItemSelected = false;
            selectedMapItem.first = nothing;
            selectedMapItem.second = nullptr;
        }

        for(int item = 2; item < mapItemName.size(); item++)
        {
            for(int i = 0; i < mapItemDict[(mapItem)item].size(); i++)
            if(in(mapItemDict[(mapItem)item][i]->getX(), mapItemDict[(mapItem)item][i]->getY(),
                  mapItemDict[(mapItem)item][i]->getWidth(), mapItemDict[(mapItem)item][i]->getHeight(), viewEvent.mouseButton))
            {
                selectedMapItem = std::make_pair((mapItem)item, mapItemDict[(mapItem)item][i]);
                break;
            }
        }

        if(viewEvent.mouseButton.button == sf::Mouse::Left)
        {
            if(selectedMapItem.second == nullptr && selectedObject != wall && selectedObject != nothing)
            {
                mapItemDict[selectedObject].push_back(new PushTile(viewEvent.mouseButton.x, viewEvent.mouseButton.y, tileWidth, tileHeight));
                mapItemDict[selectedObject][mapItemDict[selectedObject].size() - 1]->setState(selectedObject, mapItemColor[selectedObject]);
                systemMessageLabel.setText("Added a \"" + mapItemName[selectedObject] + "\" object");
            }
            else if(selectedMapItem.second != nullptr)
            {
                itemOptionXBox.setText(std::to_string((int)selectedMapItem.second->getX()));
                itemOptionYBox.setText(std::to_string((int)selectedMapItem.second->getY()));
                isMapItemSelected = true;
                isItemHeld = true;
                systemMessageLabel.setText("Selected a \"" + mapItemName[selectedMapItem.first] + "\" object at\n[X: " +
                std::to_string(selectedMapItem.second->getX()) + ", Y: " + std::to_string(selectedMapItem.second->getY()) + "]");
            }
        }

        if(selectedMapItem.second != nullptr && viewEvent.mouseButton.button == sf::Mouse::Right)
        {
            int tmpCoordX = selectedMapItem.second->getX(), tmpCoordY = selectedMapItem.second->getY();
            mapItemDict[selectedMapItem.first].erase(std::find(mapItemDict[selectedMapItem.first].begin(), mapItemDict[selectedMapItem.first].end(), selectedMapItem.second));
            systemMessageLabel.setText("Successfully erased a \"" + mapItemName[selectedMapItem.first] +
                                       "\" object at\n[X: " + std::to_string(tmpCoordX) + ", Y: " + std::to_string(tmpCoordY) + "]");
        }
    }

    if(selectedMapItem.second != nullptr && viewEvent.type == sf::Event::KeyPressed)
    {
        if(viewEvent.key.code == sf::Keyboard::Key::Enter)
        {
            try
            {
                if(!itemOptionXBox.getText().empty() && !itemOptionYBox.getText().empty())
                {
                    selectedMapItem.second->setPos(std::stof(itemOptionXBox.getText()), std::stof(itemOptionYBox.getText()));
                    systemMessageLabel.setText("Successfully moved a \"" + mapItemName[selectedMapItem.first] +
                                            "\" object to\n[X: " + std::to_string(selectedMapItem.second->getX()) + ", Y: " + std::to_string(selectedMapItem.second->getY()) + "]");
                }
                else systemMessageLabel.setText("Failed to move a \"" + mapItemName[selectedMapItem.first] + " object.\nOne of the coordinates weren't filled in.");
            }
            catch(const std::invalid_argument e)
            {
                systemMessageLabel.setText("Failed to move a \"" + mapItemName[selectedMapItem.first] + " object.\nPlease, input the coordinates as integers.");
            }
        }
        bool moveUp = viewEvent.key.code == sf::Keyboard::Key::Up;
        bool moveDown = viewEvent.key.code == sf::Keyboard::Key::Down;
        bool moveLeft = viewEvent.key.code == sf::Keyboard::Key::Left;
        bool moveRight = viewEvent.key.code == sf::Keyboard::Key::Right;
        if(moveUp || moveDown || moveLeft || moveRight)
        {
            selectedMapItem.second->move((moveRight - moveLeft) * arrowMoveAmount[arrowMoveAmountIndex], 
                                         (moveDown - moveUp) * arrowMoveAmount[arrowMoveAmountIndex]);
            itemOptionXBox.setText(std::to_string((int)selectedMapItem.second->getX()));
            itemOptionYBox.setText(std::to_string((int)selectedMapItem.second->getY()));
            systemMessageLabel.setText("Successfully moved a \"" + mapItemName[selectedMapItem.first] +
                                            "\" object to\n[X: " + std::to_string(selectedMapItem.second->getX()) + ", Y: " + std::to_string(selectedMapItem.second->getY()) + "]");
        }
    }

    if(isItemHeld && viewEvent.type == sf::Event::MouseMoved)
    {
        selectedMapItem.second->setPos(viewEvent.mouseMove.x, viewEvent.mouseMove.y);
        itemOptionXBox.setText(std::to_string((int)selectedMapItem.second->getX()));
        itemOptionYBox.setText(std::to_string((int)selectedMapItem.second->getY()));
        systemMessageLabel.setText("Successfully moved a \"" + mapItemName[selectedMapItem.first] +
                                    "\" object to\n[X: " + std::to_string(selectedMapItem.second->getX()) + ", Y: " + std::to_string(selectedMapItem.second->getY()) + "]");
    }

    if(viewEvent.type == sf::Event::MouseButtonReleased && viewEvent.mouseButton.button == sf::Mouse::Left)
        isItemHeld = false;

    for(int i = 0; i < levelMatrix.size(); i++)
        for(int j = 0; j < levelMatrix[i].size(); j++)
            if(levelMatrix[i][j]->isActivated(viewEvent) && (selectedObject == wall || selectedObject == nothing))
                levelMatrix[i][j]->setState(selectedObject, mapItemColor[selectedObject]);
}

void App::draw()
{

    switch (mode)
    {
    case editing:

        for(int i = 0; i < editorDrawnElements.size(); i++)
            mainWindow->draw(*(editorDrawnElements[i]));

        if(!levelMatrix.empty())
        {
            mainWindow->setView(editorView);

            for(int i = 0; i < levelMatrix.size(); i++)
                for(int j = 0; j < levelMatrix[i].size(); j++)
                    mainWindow->draw(*(levelMatrix[i][j]));
            
            for(int i = 0; i < levelMatrixSquares.size(); i++)
                for(int j = 0; j < levelMatrixSquares[i].size(); j++)
                    mainWindow->draw(*(levelMatrixSquares[i][j]));

            for(int item = 2; item < mapItemName.size(); item++)
                for(int i = 0; i < mapItemDict[(mapItem)item].size(); i++)
                    mainWindow->draw(*mapItemDict[(mapItem)item][i]);

            mainWindow->setView(mainView);
        }

        break;

    case saving:
        for(int i = 0; i < saveDrawnElements.size(); i++)
            mainWindow->draw(*(saveDrawnElements[i]));
        break;

    default:
        break;
    }

    for(int i = 0; i < universalDrawnElements.size(); i++)
        mainWindow->draw(*(universalDrawnElements[i]));
    
    if(isMapItemSelected)
        for(int i = 0; i < editorItemPropertyElements.size(); i++)
            mainWindow->draw(*(editorItemPropertyElements[i]));
}

std::string App::generateMatrix(int n, int m)
{
    float ResWidth = columnWidth * (m + 1) + rowWidth * m, ResHeight = columnHeight * n + rowHeight * (n + 1);

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
    }

    editorView.zoom(1 / zoomFactor);
    editorView.setCenter(ResWidth / 2, ResHeight / 2);
    zoomFactor = std::max(ResWidth / (editorView.getViewport().width * wndW), ResHeight / (editorView.getViewport().height * wndH));
    editorView.zoom(zoomFactor);

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
                levelMatrix[i][j] = new PushTile((j + 1) * columnWidth + j * rowWidth, i / 2 * (columnHeight + rowHeight), rowWidth, rowHeight);
            else
                levelMatrix[i][j] = new PushTile(j * (columnWidth + rowWidth), (i + 1) / 2 * rowHeight + (i - 1) / 2 * columnHeight, columnWidth, columnHeight);
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
    int totalItemCount = 0;
    for(int item = 2; item < mapItemName.size(); item++)
        totalItemCount += mapItemDict[(mapItem)item].size();
    levelFile << totalItemCount << "\n";
    for(int item = 2; item < mapItemName.size(); item++)
        if(mapItemDict[(mapItem)item].size() != 0)
            for(PushTile* tile : mapItemDict[(mapItem)item])
                levelFile << item - 1 << ' ' << tile->getX() - tileWidth / 2 << ' ' 
                << tile->getY() - tileWidth / 2 << "\n";
    
    levelFile.close();

    return "Map saved as " + fileName + ".txt at \"" + filePath + "\"";
}