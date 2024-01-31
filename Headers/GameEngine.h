#include "enemy.h"
#include "chat.h"
#include "contextMenu.h"
#include "bar.h"
#include "player.h"
#include "client.h"
#include "effect.h"
#include "fire.h"
#include "tempText.h"

//////////////////////////////////////////////////////////// Settings of the game
bool IsDrawMinimap       = true;
bool IsDrawInterface     = true;
bool MiniMapHoldOnPlayer = true;

//////////////////////////////////////////////////////////// Stuff for work with system and screen
sf::ContextSettings settings;

sf::RenderWindow window(sf::VideoMode(scw, sch), "multigame", sf::Style::Fullscreen, settings);
float MiniMapZoom = 1.f;
bool MiniMapActivated, EscapeMenuActivated, IsDrawInventory;
screens::Type screen = screens::MainRoom;
std::vector<sf::Drawable*> DrawableStuff, InterfaceStuff;
std::vector<Interactable*> InteractibeStuff;

std::vector<Item*> PickupStuff;

//////////////////////////////////////////////////////////// DrawableStuff
sf::Sprite WallRect, FLoorTileSprite;
std::vector<sf::Texture> FloorTextureRects;
sf::CircleShape circleShape;
std::vector<TempText*> TempTextsOnScreen, TempTextsOnGround;
Bar<float> EnemyHealthBar;

//////////////////////////////////////////////////////////// MiniMapStuff
sf::CircleShape MMPlayerCircle; // MM - MiniMap prefix
sf::RectangleShape MMPortalRect, MMBoxRect, MMPuddleRect, MMArtifact;

//////////////////////////////////////////////////////////// InterfaceStuff
Bar<float> ManaBar, HpBar;
Bar<int> AmmoBar;
PlacedText WeaponNameText;
PlacedText ReloadWeaponText;
sf::Sprite XButtonSprite;

//////////////////////////////////////////////////////////// InventoryStuff
inventoryPage::Type activeInventoryPage = inventoryPage::Items;
std::vector<sf::Drawable*> inventoryElements; // These elements appear on every page

Button backButton("Back", [](){
    IsDrawInventory = false;
});
Button itemsPageButton("Items", [](){
    activeInventoryPage = inventoryPage::Items;
});
Button weaponsPageButton("Weapons", [](){
    activeInventoryPage = inventoryPage::Weapons;
});
Button equipablesPageButton("Equipables", [](){
    activeInventoryPage = inventoryPage::Equipables;
});
Button perksPageButton("Perks", [](){
    activeInventoryPage = inventoryPage::Perks;
});
Button statsPageButton("Stats", [](){
    activeInventoryPage = inventoryPage::Stats;
});
Panel invBackground;
Panel itemListBG;

std::map<inventoryPage::Type, std::vector<sf::Drawable*>> inventoryPageElements; // These elements only appear on certain pages
std::map<ItemID::Type, std::vector<sf::Drawable*>> itemSlotsElements; // Elements that comprise an inventory slot - the background texture and the amount text
std::map<ItemID::Type, Rect*> itemSlotsRects; // The slot itself. This is what activates when a player clicks on an item.

std::vector<std::map<ItemID::Type, Item*>*> currInventory; // Basically a cache for the inventory. Updates only when a change happens for a player, e.g. picking or using an item

Panel statsPlayerImage;

bool isItemDescDrawn = false;
bool isInventoryUpToDate = true;
PlacedText itemDescText;
ItemID::Type prevItemDescID;

//////////////////////////////////////////////////////////// Online tools
sf::TcpListener listener;
sf::Packet ReceivePacket, SendPacket;
std::vector<Client*> clients;
sf::SocketSelector selector;
std::string ClientState, IPOfHost, MyIP, PacetData;
Client MySocket; // this computer socket
sf::Int32 ComputerID;
sf::Mutex mutex;
bool ClientFuncRun, HostFuncRun;

//////////////////////////////////////////////////////////// Interactables
Interactable portal,
             puddle;
std::vector<Interactable*> listOfBox,
                           listOfArtifact;

//////////////////////////////////////////////////////////// Fire
std::vector<Fire*> FireSet;

//////////////////////////////////////////////////////////// Locations
Location* CurLocation = nullptr;
Location LabyrinthLocation, WaitingRoomLoaction, MainMenuLocation;

void loadLocations() {
    WaitingRoomLoaction.LoadFromFile("sources/locations/WaitingRoom.txt");
    MainMenuLocation.LoadFromFile("sources/locations/MainMenu.txt");
}

//////////////////////////////////////////////////////////// Players
Player player;
std::vector<Player> ConnectedPlayers;

//////////////////////////////////////////////////////////// Chat
Chat chat(scw, sch);

//////////////////////////////////////////////////////////// Other stuff
sf::Vector2i MouseBuffer;
Bullet tempBullet;

//////////////////////////////////////////////////////////// Weapons
Pistol pistol;
Shotgun shotgun;
Revolver revolver;
Rifle rifle;
Bubblegun bubblegun;
Armageddon armageddon;
Chaotic chaotic;
std::vector<Weapon*> weapons = {
    &pistol,
    &shotgun,
    &revolver,
    &rifle,
    &bubblegun
};
Scale<int> CurWeapon{0, 4, 0};

//////////////////////////////////////////////////////////// Enemies
std::vector<Enemy*> Enemies;

//////////////////////////////////////////////////////////// Effects
std::vector<Effect*> AllEffects;

//////////////////////////////////////////////////////////// functions
void draw();
void drawFloor();
void drawWalls();
void drawMiniMap();
void drawInterface();
void drawInventory();
void LevelGenerate(int, int);
void LoadMainMenu();
void init();
void initInventory();
void EventHandler();
bool useItem(ItemID::Type);
void createSlotRects();
void updateBullets();
void MainLoop();
bool IsSomeOneCanBeActivated();
void FillFloorRects();
void updateShaders();
void processEffects();
void setBox(Interactable*&);
void setArtifact(Interactable*&);
template <typename T> void clearVectorOfPointer(std::vector<T>&);

//////////////////////////////////////////////////////////// Server-Client functions
void ClientConnect();
void ClientDisconnect(int);
void SelfDisconnect();
void SendToClients(sf::Packet&, int=-1);
void funcOfHost();
void funcOfClient();

//////////////////////////////////////////////////////////// Threads
sf::Thread HostTread(funcOfHost);
sf::Thread ClientTread(funcOfClient);
sf::Thread FillFloorRectsThread(FillFloorRects);

//////////////////////////////////////////////////////////// Pannels
Panel IPPanel("IP:");
Panel ListOfPlayers;

//////////////////////////////////////////////////////////// Buttons
Button HostButton("Host", [](){
    listener.listen(53000);
    selector.add(listener);
    ListOfPlayers.setWord(MyIP);
    ComputerID = 0;
    ConnectedPlayers.push_back(*(new Player()));
    HostFuncRun = true;
    HostTread.launch();
});

Button EscapeButton("Exit", [](){
    if (HostFuncRun) {
        mutex.lock();
        SendPacket << pacetStates::disconnect;
        SendToClients(SendPacket);
        SendPacket.clear();
        mutex.unlock();
        clients.clear();
        selector.clear();
        listener.close();
        HostFuncRun = false;
        ConnectedPlayers.clear();
    } else if (ClientFuncRun) {
        SelfDisconnect();
    }
    screen = screens::MainRoom;
    EscapeMenuActivated = false;
    ListOfPlayers.clear();
    Bullets.clear();
    clearVectorOfPointer(Enemies);
    clearVectorOfPointer(FireSet);
    clearVectorOfPointer(TempTextsOnGround);
    clearVectorOfPointer(TempTextsOnScreen);
    player.CurWeapon->lock = true;
    LoadMainMenu();
});

//////////////////////////////////////////////////////////// functions
void draw() {
    window.setView(GameView);
    window.clear(sf::Color::Black);

    updateShaders();

    drawFloor();
    drawWalls();

    for (int i = 0; i < CurLocation->objects.size(); i++) {
        if (CurLocation->objects[i].id == Tiles::portal) {
            portal.setPosition(CurLocation->objects[i].pos);
            window.draw(portal);
        }
        if (CurLocation->objects[i].id == Tiles::puddle) {
            puddle.setPosition(CurLocation->objects[i].pos);
            window.draw(puddle, MapStates);
        }
    }

    for (sf::Drawable* d: DrawableStuff) {
        window.draw(*d);
    }

    for (Enemy*& enemy: Enemies) {
        EnemyHealthBar.setPosition(enemy->getPosition() - sf::Vector2f(EnemyHealthBar.getSize().x / 2.f, enemy->getRadius() + 50.f));
        EnemyHealthBar.setValue(enemy->Health);
        window.draw(EnemyHealthBar);
    }

    for (int i = 0; i < Bullets.size(); i++) {
        circleShape.setFillColor(Bullets[i].color);
        circleShape.setRadius(Bullets[i].Radius);
        circleShape.setPosition(Bullets[i].getPosition());
        circleShape.setOrigin(Bullets[i].Radius, Bullets[i].Radius);
        window.draw(circleShape);
    }

    for (TempText* d: TempTextsOnGround) {
        if (d->localClock->getElapsedTime() < d->howLongToExist) {
            window.draw(*d);
        } else {
            DeleteFromVector(TempTextsOnGround, d);
            delete d;
        }
    }

    if (IsDrawMinimap) {
        drawMiniMap();
    }
    if (IsDrawInterface) {
        drawInterface();
    }
    if (IsDrawInventory) {
        drawInventory();
    }
    window.display();
}

void drawFloor() {
    std::vector<sf::Texture>::iterator it = FloorTextureRects.begin();
    for (int i = 0; i < CurLocation->n; i++) {
        for (int j = 0; j < CurLocation->m; j++) {
            if (CurLocation->EnableTiles[i][j]) {
                FLoorTileSprite.setTexture(*it, true);
                FLoorTileSprite.setPosition(size * j, size * i);
                FLoorTileSprite.setScale(3.f, 3.f);
                window.draw(FLoorTileSprite, MapStates);
                it++;
            }
        }
    }
}

sf::Vector2f CameraPos;
void drawWalls() {
    CameraPos = GameView.getCenter() - GameView.getSize() / 2.f;
    for (int i = std::max(0, 2 * int(CameraPos.y / size) - 1);
            i <= std::min(int(CurLocation->walls.size() - 1), 2 * int((CameraPos.y + sch + WallMinSize) / size) + 1); i++) {
        for (int j = std::max(0, int(CameraPos.x / size) - 1);
                j <= std::min(int(CurLocation->walls[i].size() - 1), int((CameraPos.x + scw + WallMinSize) / size) + 1); j++) {
            if (CurLocation->wallsRect[i][j].intersect(CameraPos.x, CameraPos.y, static_cast<float>(scw), static_cast<float>(sch))) {
                CurLocation->SeenWalls[i][j] = true;
            }
            if (CurLocation->walls[i][j]) {
                WallRect.setPosition(CurLocation->wallsRect[i][j].getPosition());
                WallRect.setTextureRect(sf::IntRect(
                        (Textures::Wall.getSize().x - CurLocation->wallsRect[i][j].Width) * random(sf::Vector2f(i, j)),
                        (Textures::Wall.getSize().y - CurLocation->wallsRect[i][j].Height) * random(sf::Vector2f(j, i)),
                        CurLocation->wallsRect[i][j].Width,
                        CurLocation->wallsRect[i][j].Height));
                window.draw(WallRect, MapStates);
            }
        }
    }
}

void drawMiniMap() {
    if (MiniMapHoldOnPlayer) {
        if (!sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
            MiniMapView.setCenter(player.getPosition() * ScaleParam);
        }
    }

    // draw walls
    window.setView(MiniMapView);
    sf::VertexArray line(sf::Lines, 2);
    for (int i = 0; i < CurLocation->walls.size(); i++) {
        for (int j = 0; j < CurLocation->walls[i].size(); j++) {
            if (CurLocation->walls[i][j] && CurLocation->SeenWalls[i][j]) {
                if (i % 2 == 1) { // |
                    line[0] = sf::Vertex(sf::Vector2f(miniSize * j, miniSize * (i - 1) / 2), sf::Color::White);
                    line[1] = sf::Vertex(sf::Vector2f(miniSize * j, miniSize * (i + 1) / 2), sf::Color::White);
                } else { // -
                    line[0] = sf::Vertex(sf::Vector2f(miniSize * j, miniSize * i / 2));
                    line[1] = sf::Vertex(sf::Vector2f(miniSize * (j + 1), miniSize * i / 2));
                }
                window.draw(line);
            }
        }
    }

    // draw location objects
    for (int i = 0; i < CurLocation->objects.size(); i++) {
        if (CurLocation->objects[i].id == Tiles::portal) {
            MMPortalRect.setPosition(CurLocation->objects[i].pos * ScaleParam);
            window.draw(MMPortalRect);
        }
        if (CurLocation->objects[i].id == Tiles::box) {
            MMBoxRect.setPosition(CurLocation->objects[i].pos * ScaleParam);
            window.draw(MMBoxRect);
        }
        if (CurLocation->objects[i].id == Tiles::puddle) {
            MMPuddleRect.setPosition(CurLocation->objects[i].pos * ScaleParam);
            window.draw(MMPuddleRect);
        }
        if (CurLocation->objects[i].id == Tiles::artifact) {
            MMArtifact.setPosition(CurLocation->objects[i].pos * ScaleParam);
            window.draw(MMArtifact);
        }
    }

    // draw players
    if (ClientFuncRun || HostFuncRun) {
        for (Player& p: ConnectedPlayers) {
            MMPlayerCircle.setPosition(p.getPosition() * ScaleParam);
            window.draw(MMPlayerCircle);
        }
    } else {
        MMPlayerCircle.setPosition(player.getPosition() * ScaleParam);
        window.draw(MMPlayerCircle);
    }
    window.setView(GameView);
}

sf::Clock ClockFPS; int FPSCounter;
PlacedText TextFPS;
void drawInterface() {
    window.setView(InterfaceView);
    for (sf::Drawable* d: InterfaceStuff) {
        window.draw(*d);
    }

    if (player.CurWeapon != nullptr && player.CurWeapon->AmountOfAmmunition.toBottom() <= 0) {
        window.draw(ReloadWeaponText);
    }

    for (int i = 0; i < weapons.size(); i++) {
        if (weapons[i] == player.CurWeapon) {
            WeaponNameText.setFillColor(sf::Color::White);
            WeaponNameText.setOutlineThickness(3);
        } else {
            WeaponNameText.setFillColor(sf::Color(25, 192, 25, 160));
            WeaponNameText.setOutlineThickness(1);
        }
        AmmoBar.setValue(weapons[i]->AmountOfAmmunition);
        AmmoBar.setPosition(20, sch - 20 - (weapons.size() - i) * (AmmoBar.getSize().y + 10));
        WeaponNameText.setString(weapons[i]->Name);
        WeaponNameText.setPosition(35 + AmmoBar.getSize().x, AmmoBar.getPosition().y + WeaponNameText.Height / 4);
        window.draw(AmmoBar);
        window.draw(WeaponNameText);
    }

    if (EscapeMenuActivated) {
        window.draw(ListOfPlayers);
        window.draw(EscapeButton);
    }

    FPSCounter++;
    if (ClockFPS.getElapsedTime() >= sf::seconds(1)) {
        TextFPS.setString(std::to_string(FPSCounter) + " FPS");
        FPSCounter = 0;
        ClockFPS.restart();
    }
    window.draw(TextFPS);

    for (TempText* d: TempTextsOnScreen) {
        if (d->localClock->getElapsedTime() < d->howLongToExist) {
            window.draw(*d);
        } else {
            DeleteFromVector(TempTextsOnScreen, d);
            delete d;
        }
    }

    window.setView(GameView);
}

void drawInventory() {
    window.setView(InventoryView);
    for (sf::Drawable* elem : inventoryElements)
        window.draw(*elem);

    for (sf::Drawable* elem : inventoryPageElements[activeInventoryPage])
        window.draw(*elem);

    std::vector<std::map<ItemID::Type, Item*>*> currInventory
    {
        &player.inventory.keyItems,
        &player.inventory.safeItems,
        &player.inventory.equipItems,
        &player.inventory.dropableItems
    };

    if (!isInventoryUpToDate) {
        std::cout << "Inventory needs update. Clearing items\n";
        if (!itemSlotsElements.empty()) {
            for (ItemID::Type id = 0; id != ItemID::NONE; id++) {
                if (itemSlotsElements.find(id) != itemSlotsElements.end()) {
                    for (sf::Drawable* &elem : itemSlotsElements[id])
                        delete elem;
                    itemSlotsElements[id].clear();
                }
            }
        }
        itemSlotsElements.clear();
    }

    for (int i = 0; i < 4; i++) {
        if (player.inventory.itemAmount() != 0 && activeInventoryPage != inventoryPage::Stats) {
            int slotNumber = 0;
            for (ItemID::Type id = 0; id != ItemID::NONE; id++) {
                if ((currInventory[i]->find(id) != currInventory[i]->end()) &&
                    (*currInventory[i])[id]->amount > 0) {

                    Item* drawnItem = (*currInventory[i])[id];

                    if (!isInventoryUpToDate) {
                        float itemX = int(slotNumber % 6) * 150 + itemListBG.getPosition().x + 50;
                        float itemY = int(slotNumber / 6) * 150 + itemListBG.getPosition().y + 50;
                        drawnItem->setCenter(itemX + 75, itemY + 75);

                        Panel* bgPanel = new Panel();
                        bgPanel->setPosition(itemX, itemY);
                        bgPanel->setScale(0.5, 0.5);
                        bgPanel->setTexture(Textures::ItemPanel);
                        itemSlotsElements[id].push_back(bgPanel);

                        PlacedText* itemAmountText = new PlacedText();
                        itemAmountText->setCharacterSize(16);
                        itemAmountText->setString(std::to_string(drawnItem->amount));
                        itemAmountText->setPosition(sf::Vector2f(itemX + 125, itemY + 125));

                        if (drawnItem->amount > 1)
                            itemSlotsElements[id].push_back(itemAmountText);
                        
                    }

                    for (sf::Drawable* &elem : itemSlotsElements[id])
                        window.draw(*elem);
                    window.draw(*drawnItem);

                    slotNumber++;
                }
            }
        }
    }

    isInventoryUpToDate = true;

    if (isItemDescDrawn) {
        itemDescText.setPosition(sf::Mouse::getPosition(window).x + 100, sf::Mouse::getPosition(window).y);
        window.draw(itemDescText);
    }

    window.setView(InterfaceView);
}

void LevelGenerate(int n, int m) {
    Bullets.clear();

    MiniMapView.zoom(1 / MiniMapZoom);
    MiniMapZoom = 1;

    LabyrinthLocation.GenerateLocation(n, m, player.getPosition() / float(size));

    FillFloorRectsThread.launch();

    LabyrinthLocation.AddObject({Tiles::portal, player.getPosition() - portal.getSize() / 2.f});
    LabyrinthLocation.AddObject({Tiles::puddle, player.getPosition() - portal.getSize() / 2.f + sf::Vector2f(size, size)});

    clearVectorOfPointer(listOfBox);
    for (int i = 0; i < 10; i++) {
        listOfBox.push_back(new Interactable());
        setBox(listOfBox[i]);
        do {
            listOfBox[i]->setPosition(sf::Vector2f(rand() % m, rand() % n) * (float)size +
            sf::Vector2f(rand() % int(size - listOfBox[i]->getSize().x), rand() % int(size - listOfBox[i]->getSize().y)));
        } while (!LabyrinthLocation.EnableTiles[(int)listOfBox[i]->PosY / size][(int)listOfBox[i]->PosX / size]);

        LabyrinthLocation.AddObject({Tiles::box, listOfBox[i]->getPosition()});
        InteractibeStuff.push_back(listOfBox[i]);
        DrawableStuff.push_back(listOfBox[i]);
    }

    clearVectorOfPointer(listOfArtifact);
    for (int i = 0; i < 10; i++) {
        listOfArtifact.push_back(new Interactable());
        setArtifact(listOfArtifact[i]);
        do {
            listOfArtifact[i]->setPosition(sf::Vector2f(rand() % m, rand() % n) * (float)size +
            sf::Vector2f(rand() % (size - Textures::Box.getSize().x / 4), rand() % (size - Textures::Box.getSize().y / 4)));
        } while (!LabyrinthLocation.EnableTiles[(int)listOfArtifact[i]->PosY / size][(int)listOfArtifact[i]->PosX / size]);

        LabyrinthLocation.AddObject({Tiles::artifact, listOfArtifact[i]->getPosition()});
        InteractibeStuff.push_back(listOfArtifact[i]);
        DrawableStuff.push_back(listOfArtifact[i]);
    }

    clearVectorOfPointer(FireSet);
    for (int i = 0; i < 1; i++) {
        FireSet.push_back(new Fire((player.getPosition() - portal.getSize() / 2.f - sf::Vector2f(size, size)).x,
                                   (player.getPosition() - portal.getSize() / 2.f + sf::Vector2f(size, size)).y, 90.f, 90.f,
                                   sf::seconds(10.f)));
        FireSet[FireSet.size() - 1]->setAnimation(Textures::Fire, 1, 1, sf::seconds(1), &Shaders::Map);
    }

    clearVectorOfPointer(Enemies);
    for (int i = 0; i < 5; i++) {
        Enemies.push_back(new DistortedScientist());
        Enemies.push_back(new ScottPilgrim());
        Enemies.push_back(new RamonaFlowers());
    }

    for (int i = 0; i < Enemies.size(); i++) {
        do {
            Enemies[i]->setPosition(sf::Vector2f((rand() % m) + 0.5f, (rand() % n) + 0.5f) * (float)size);
        } while (!LabyrinthLocation.EnableTiles[(int)Enemies[i]->PosY / size][(int)Enemies[i]->PosX / size] ||
                 distance(Enemies[i]->getPosition(), player.getPosition()) < size * 3);
    }
    FillFloorRectsThread.wait();
}

void LoadMainMenu() {
    CurLocation = &MainMenuLocation;

    player.setPosition(3.5f * size, 2.5f * size);
    FindAllWaysTo(CurLocation, player.getPosition(), TheWayToPlayer);
    player.CurWeapon = nullptr;

    sf::Vector2f PlayerPos = player.getPosition() / (float)size;
    CurLocation->FindEnableTilesFrom(PlayerPos);
    FillFloorRectsThread.launch();

    portal.setFunction([](Interactable* i){
        clearVectorOfPointer(PickupStuff);

        DrawableStuff.clear();
        InterfaceStuff.clear();
        InteractibeStuff.clear();

        player.setPosition(sf::Vector2f((START_M / 2 + 0.5f) * size, (START_N / 2 + 0.5f) * size));
        player.ChangeWeapon(weapons[CurWeapon.cur]);

        screen = screens::Dungeon;

        MiniMapActivated = false;
        EscapeMenuActivated = false;

        MiniMapView.setViewport(sf::FloatRect(0.f, 0.f, 0.25f, 0.25f));
        MiniMapView.setCenter(player.getPosition() * ScaleParam);

        Musics::MainMenu.pause();
        if (Musics::Fight1.getStatus() != sf::Music::Playing && Musics::Fight2.getStatus() != sf::Music::Playing) {
            Musics::Fight1.play();
        }

        CurLocation = &LabyrinthLocation;
        LevelGenerate(START_N++, START_M++);
        FindAllWaysTo(CurLocation, player.getPosition(), TheWayToPlayer);

        DrawableStuff.push_back(&player);
        for (Enemy* &enemy: Enemies) {
            DrawableStuff.push_back(enemy);
        }
        for (Fire* &fire: FireSet) {
            DrawableStuff.push_back(fire);
        }

        InterfaceStuff.push_back(&ManaBar);
        InterfaceStuff.push_back(&HpBar);
        InterfaceStuff.push_back(&AmmoBar);
        InterfaceStuff.push_back(&WeaponNameText);
        InterfaceStuff.push_back(&chat);

        InteractibeStuff.push_back(&puddle);
    });

    puddle.setFunction([](Interactable* i){
        player.getDamage(5.f);
    });

    // Set cameras
    GameView.setCenter(player.getPosition());
    MiniMapView.setCenter(player.getPosition() * ScaleParam);
    InterfaceView.setCenter({scw / 2.f, sch / 2.f});

    Musics::Fight1.stop();
    Musics::Fight2.stop();
    Musics::MainMenu.play();

    DrawableStuff.clear();
    DrawableStuff.push_back(&player);

    InterfaceStuff.clear();
    InterfaceStuff.push_back(&ManaBar);
    InterfaceStuff.push_back(&HpBar);
    InterfaceStuff.push_back(&chat);

    InteractibeStuff.push_back(&portal);
    InteractibeStuff.push_back(&puddle);

    Item* newItem = new Item(ItemID::regenDrug, 1);
    newItem->setAnimation(*itemTextureName[ItemID::regenDrug], 1, 1, sf::seconds(1), &Shaders::Map);
    PickupStuff.push_back(newItem);
    DrawableStuff.push_back(PickupStuff[0]);
    PickupStuff[0]->dropTo(player.getPosition() + sf::Vector2f(100, 100));

    listOfBox.push_back(new Interactable());
    setBox(listOfBox[0]);
    listOfBox[0]->setPosition(1912.5, 1545);

    InteractibeStuff.push_back(listOfBox[0]);
    DrawableStuff.push_back(listOfBox[0]);

    listOfArtifact.push_back(new Interactable());
    setArtifact(listOfArtifact[0]);
    listOfArtifact[0]->setPosition(1312.5, 1545);

    InteractibeStuff.push_back(listOfArtifact[0]);
    DrawableStuff.push_back(listOfArtifact[0]);


    FillFloorRectsThread.wait();
}

void init() {
    setlocale(LC_ALL, "rus");

    window.setVerticalSyncEnabled(true);
    settings.antialiasingLevel = 8;
    window.setView(GameView);

    MiniMapView.setViewport(sf::FloatRect(0.f, 0.f, 0.25f, 0.25f));
    GameClock = new sf::Clock;

    // Load locations
    WaitingRoomLoaction.LoadFromFile("sources/locations/WaitingRoom.txt");
    MainMenuLocation.LoadFromFile("sources/locations/MainMenu.txt");

    loadTextures();
    loadItemTextures();
    loadFonts();
    loadShaders();
    loadMusics();

    Musics::MainMenu.setLoop(true);
    Musics::MainMenu.setVolume(20);
    Musics::Fight1.setVolume(20);
    Musics::Fight2.setVolume(20);

    portal.setAnimation(Textures::PortalAnimation2, 9, 1, sf::seconds(1), &Shaders::Portal);
    portal.setSize(170.f, 320.f);
    player.setAnimation(Textures::Player, 1, 1, sf::seconds(1), &Shaders::Player);
    puddle.setAnimation(Textures::Puddle, 1, 1, sf::seconds(1), &Shaders::Map);
    puddle.setSize(90.f, 90.f);

    Shaders::Map.setUniform("u_resolution", sf::Vector2f(static_cast<float>(scw), static_cast<float>(sch)));
    Shaders::Map.setUniform("u_playerRadius", player.Radius);

    Shaders::Player.setUniform("u_resolution", sf::Vector2f(static_cast<float>(scw), static_cast<float>(sch)));

    Shaders::Portal.setUniform("u_resolution", sf::Vector2f(static_cast<float>(scw), static_cast<float>(sch)));
    Shaders::Portal.setUniform("u_playerRadius", player.Radius);

    Shaders::Architect.setUniform("u_resolution", sf::Vector2f(static_cast<float>(scw), static_cast<float>(sch)));
    Shaders::Architect.setUniform("u_playerRadius", player.Radius);

    IPPanel       .setTexture(Textures::YellowPanel);
    ListOfPlayers .setTexture(Textures::SteelFrame);

    EscapeButton .setTexture(Textures::RedPanel, Textures::RedPanelPushed);
    HostButton   .setTexture(Textures::GreenPanel, Textures::GreenPanelPushed);

    WallRect.setTexture(Textures::Wall);

    CurWeapon.looped = true;

    listener.setBlocking(false);
    MyIP = MySocket.getRemoteAddress().getLocalAddress().toString();
    std::cout << "LocalAddress: " << MyIP << "\nPublicAddress: " << MySocket.getRemoteAddress().getPublicAddress().toString() << '\n';

    EscapeButton.setCharacterSize(110);
    IPPanel.text.setCharacterSize(80);
    ListOfPlayers.text.setCharacterSize(60);

    EscapeButton.setCenter  (scw / 2, sch * 3 / 4);

    ListOfPlayers.setCenter (scw / 2, sch / 4);

    MMPlayerCircle.setRadius(9);
    MMPlayerCircle.setFillColor(sf::Color(0, 180, 0));
    MMPlayerCircle.setOrigin(MMPlayerCircle.getRadius(), MMPlayerCircle.getRadius());

    player.FirstWeapon  = &pistol;
    player.SecondWeapon = &shotgun;

    HpBar.setSize(360, 50);
    HpBar.setPosition(scw - HpBar.getSize().x - 10, 20);
    HpBar.setValue(player.Health);
    HpBar.setColors(sf::Color(255, 255, 255, 160), sf::Color(192, 0, 0, 160), sf::Color(32, 32, 32, 160));

    ManaBar.setSize(240, 50);
    ManaBar.setPosition(scw - ManaBar.getSize().x - 10, HpBar.getPosition().y + HpBar.getSize().y);
    ManaBar.setValue(player.Mana);
    ManaBar.setColors(sf::Color(255, 255, 255, 160), sf::Color(0, 0, 192, 160), sf::Color(32, 32, 32, 160));

    AmmoBar.setSize(160, 50);
    AmmoBar.setColors(sf::Color(255, 255, 255, 160), sf::Color(128, 128, 128, 160), sf::Color(32, 32, 32, 160));

    WeaponNameText.setFillColor(sf::Color(25, 192, 25, 160));
    WeaponNameText.setOutlineColor(sf::Color::Black);

    ReloadWeaponText.setFillColor(sf::Color(255, 20, 20));
    ReloadWeaponText.setCharacterSize(100);

    XButtonSprite.setTexture(Textures::XButton);
    XButtonSprite.setPosition(scw / 2.f - XButtonSprite.getGlobalBounds().width / 2.f, sch * 3.f / 4.f - XButtonSprite.getGlobalBounds().height / 2.f);

    MMPortalRect.setSize(portal.getSize() * ScaleParam);
    MMPortalRect.setFillColor(sf::Color(200, 0, 200, 200));

    MMBoxRect.setSize(sf::Vector2f(105.f, 117.f) * ScaleParam);
    MMBoxRect.setFillColor(sf::Color(252, 108, 24, 200));

    MMPuddleRect.setSize(puddle.getSize() * ScaleParam);
    MMPuddleRect.setFillColor(sf::Color(0, 0, 255, 200));

    MMArtifact.setSize(sf::Vector2f(150.f, 105.f) * ScaleParam);
    MMArtifact.setFillColor(sf::Color::White);

    EnemyHealthBar.setColors(sf::Color(255, 255, 255, 160), sf::Color(192, 0, 0, 160), sf::Color(32, 32, 32, 160));
    EnemyHealthBar.setSize(125.f, 15.f);
    EnemyHealthBar.setWallWidth(1);
    EnemyHealthBar.ShowText = false;

    initInventory();

    LoadMainMenu();
}

void initInventory() {
    invBackground.setTexture(Textures::SteelFrame);
    invBackground.setScale((float)scw / Textures::SteelFrame.getSize().x, (float)sch / Textures::SteelFrame.getSize().y);
    invBackground.setPosition(0, 0);

    int xOffset = 50, yOffset = 50;

    backButton.setTexture(Textures::RedPanel, Textures::RedPanelPushed);
    backButton.setCharacterSize(52);
    backButton.setPosition(xOffset, yOffset);
    backButton.setSize(300, 150);

    itemsPageButton.setTexture(Textures::YellowPanel, Textures::YellowPanelPushed);
    itemsPageButton.setCharacterSize(32);
    itemsPageButton.setPosition(xOffset, (sch - yOffset) * 0.9);
    itemsPageButton.setSize((scw - xOffset) * 0.2, (sch - yOffset) * 0.1);

    weaponsPageButton.setTexture(Textures::RedPanel, Textures::RedPanelPushed);
    weaponsPageButton.setCharacterSize(32);
    weaponsPageButton.setPosition((scw - xOffset) * 0.2, (sch - yOffset) * 0.9);
    weaponsPageButton.setSize((scw - xOffset) * 0.2, (sch - yOffset) * 0.1);

    equipablesPageButton.setTexture(Textures::GreenPanel, Textures::GreenPanelPushed);
    equipablesPageButton.setCharacterSize(32);
    equipablesPageButton.setPosition((scw - xOffset) * 0.4, (sch - yOffset) * 0.9);
    equipablesPageButton.setSize((scw - xOffset) * 0.2, (sch - yOffset) * 0.1);

    perksPageButton.setTexture(Textures::BluePanel, Textures::BluePanelPushed);
    perksPageButton.setCharacterSize(32);
    perksPageButton.setPosition((scw - xOffset) * 0.6, (sch - yOffset) * 0.9);
    perksPageButton.setSize((scw - xOffset) * 0.2, (sch - yOffset) * 0.1);

    statsPageButton.setTexture(Textures::ItemPanel, Textures::ItemPanel);
    statsPageButton.setCharacterSize(32);
    statsPageButton.setPosition((scw - xOffset) * 0.8, (sch - yOffset) * 0.9);
    statsPageButton.setSize((scw - xOffset) * 0.2, (sch - yOffset) * 0.1);

    itemListBG.setTexture(Textures::SteelFrame);
    itemListBG.setScale((float)(scw - 300) / Textures::SteelFrame.getSize().x, (float)sch / Textures::SteelFrame.getSize().y * 0.6);
    itemListBG.setPosition(150, sch * 0.2);

    inventoryElements.push_back(&invBackground);
    inventoryElements.push_back(&backButton);
    inventoryElements.push_back(&itemsPageButton);
    inventoryElements.push_back(&weaponsPageButton);
    inventoryElements.push_back(&equipablesPageButton);
    inventoryElements.push_back(&perksPageButton);
    inventoryElements.push_back(&statsPageButton);

    statsPlayerImage.setTexture(Textures::Player);
    statsPlayerImage.setCenter(5 * scw / 6, sch / 2);

    inventoryPageElements[inventoryPage::Items].push_back(&itemListBG);
    inventoryPageElements[inventoryPage::Stats].push_back(&statsPlayerImage);
}

void EventHandler() {
    sf::Event event;
    while (window.pollEvent(event)) {
        if (chat.InputText(event)) {
            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Enter) {
                mutex.lock();
                SendPacket << pacetStates::ChatEvent << chat.Last();
                if (HostFuncRun) {
                    SendToClients(SendPacket);
                } else if (ClientFuncRun) {
                    MySocket.send(SendPacket);
                }
                SendPacket.clear();
                mutex.unlock();
            }
        } else if (EscapeMenuActivated) {
            EscapeButton.isActivated(event);
            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape) {
                EscapeMenuActivated = !EscapeMenuActivated;
            }
        } else if (IsDrawInventory) {

            if (event.key.code == sf::Keyboard::Escape) {
                IsDrawInventory = false;
            }
            backButton.isActivated(event);
            itemsPageButton.isActivated(event);
            weaponsPageButton.isActivated(event);
            equipablesPageButton.isActivated(event);
            perksPageButton.isActivated(event);
            statsPageButton.isActivated(event);

            std::vector<std::map<ItemID::Type, Item*>*> currInventory
            {
                &player.inventory.keyItems,
                &player.inventory.safeItems,
                &player.inventory.equipItems,
                &player.inventory.dropableItems
            };

            bool isAnythingHovered = false;
            for (int i = 0; i < 4; i++) {
                if (player.inventory.itemAmount() != 0 &&
                    activeInventoryPage != inventoryPage::Stats) {
                    for (ItemID::Type id = 0; id != ItemID::NONE; id++) {
                        if ((currInventory[i]->find(id) != currInventory[i]->end()) &&
                            (*currInventory[i])[id]->amount > 0) {
                            if (!itemSlotsRects.empty() &&
                                itemSlotsRects.find(id) != itemSlotsRects.end() &&
                                itemSlotsRects[id]->contains(sf::Mouse::getPosition().x, sf::Mouse::getPosition().y)) {
                                if (id != prevItemDescID) {
                                    prevItemDescID = ItemID::NONE;
                                    isItemDescDrawn = false;
                                }
                                isAnythingHovered = true;
                                if ((event.type == sf::Event::MouseButtonPressed) ||
                                    (event.type == sf::Event::MouseButtonReleased)) {
                                    if (event.mouseButton.button == sf::Mouse::Button::Right) {
                                        isItemDescDrawn = true;
                                        itemDescText.setString(itemDesc[id]);
                                        prevItemDescID = id;
                                    }
                                    if (event.type == sf::Event::MouseButtonPressed &&
                                        event.mouseButton.button == sf::Mouse::Button::Left) {
                                        bool itemUsed = useItem(id);
                                        if (itemUsed) {
                                            (*currInventory[i])[id]->amount = ((*currInventory[i])[id]->amount == 1) ? 0 : (*currInventory[i])[id]->amount - 1;
                                            isItemDescDrawn = false;
                                            isInventoryUpToDate = false;
                                            createSlotRects();
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
            if (!isAnythingHovered)
                isItemDescDrawn = false;

        } else {
            if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::Escape) {
                    if (MiniMapActivated) {
                        MiniMapActivated = false;
                        MiniMapView.setViewport(sf::FloatRect(0.f, 0.f, 0.25f, 0.25f));
                        continue;
                    }
                }
                if (event.key.code == sf::Keyboard::Tab) {
                    MiniMapActivated = !MiniMapActivated;
                    if (MiniMapActivated) {
                        MiniMapView.setViewport(sf::FloatRect(0.f, 0.f, 1.f, 1.f));
                    } else {
                        MiniMapView.setViewport(sf::FloatRect(0.f, 0.f, 0.25f, 0.25f));
                    }
                }
                if (event.key.code == sf::Keyboard::Space) {
                    if (MiniMapActivated) {
                        MiniMapHoldOnPlayer = !MiniMapHoldOnPlayer;
                    }
                }
                if (event.key.code == sf::Keyboard::Tilde) {

                    IsDrawInventory = true;
                    createSlotRects();
                }
            }
            if (event.type == sf::Event::MouseWheelMoved) {
                if (MiniMapActivated) {
                    if (event.mouseWheel.delta < 0) {
                        MiniMapView.zoom(1.1f);
                        MiniMapZoom *= 1.1f;
                    } else {
                        MiniMapView.zoom(1.f / 1.1f);
                        MiniMapZoom /= 1.1f;
                    }
                }
            }

            if (IsSomeOneCanBeActivated()) {
                if (!in(InterfaceStuff, static_cast<sf::Drawable*>(&XButtonSprite)))
                    InterfaceStuff.push_back(&XButtonSprite);
            } else {
                DeleteFromVector(InterfaceStuff, static_cast<sf::Drawable*>(&XButtonSprite));
            }

            for (Interactable*& x: InteractibeStuff) {
                if (x->CanBeActivated(player)) {
                    if (x->isActivated(player, event)) {
                        break;
                    }
                }
            }

            for (int i = 0; i < PickupStuff.size(); i++) {
                if (PickupStuff[i]->CanBeActivated(player)) {
                    if (PickupStuff[i]->isActivated(player, event)) {
                        player.AddItem(PickupStuff[i]);
                        isInventoryUpToDate = false;
                        DeleteFromVector(DrawableStuff, static_cast<sf::Drawable*>(PickupStuff[i]));
                        delete PickupStuff[i];
                        PickupStuff.erase(PickupStuff.begin() + i--);
                    }
                }
            }

            switch (screen) {
                case screens::MainRoom:
                    if (event.type == sf::Event::KeyPressed) {
                        if (event.key.code == sf::Keyboard::Escape) {
                            Musics::MainMenu.pause();
                            window.close();
                        }
                    }
                    break;

                case screens::Dungeon:
                    if (player.CurWeapon != nullptr && !MiniMapActivated) {
                        player.CurWeapon->Update(event);
                        if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::R) {
                            player.CurWeapon->Reload(player.Mana);
                        }
                    }
                    if (event.type == sf::Event::KeyPressed) {
                        if (event.key.code == sf::Keyboard::Escape) {
                            EscapeMenuActivated = true;
                        }
                        if (event.key.code == sf::Keyboard::H) {
                            player.setPosition(size, size);
                            CurLocation = &WaitingRoomLoaction;
                        }
                    }
                    if (event.type == sf::Event::MouseWheelScrolled) {
                        if (!MiniMapActivated) {
                            CurWeapon -= (int)event.mouseWheelScroll.delta;
                            player.ChangeWeapon(weapons[CurWeapon.cur]);

                            std::string reloadStr = player.CurWeapon->Name + " is out of ammo!";
                            ReloadWeaponText.setString(reloadStr);
                            ReloadWeaponText.setCenter(sf::Vector2f(scw / 2, sch / 4));
                        }
                    }
                    break;

                case screens::SetIP:
                    if (event.type == sf::Event::TextEntered) {
                        sf::Uint32 code = event.text.unicode;
                        if (46 == code || (48 <= code && code <= 57)) {
                            IPOfHost.push_back(code);
                            IPPanel.setWord("IP:" + IPOfHost);
                        }
                    }
                    if (event.type == sf::Event::KeyPressed) {
                        if (event.key.code == sf::Keyboard::Enter) {
                            IPPanel.setWord("Connecting...");
                            draw();
                            if (MySocket.connect(IPOfHost, 53000, sf::milliseconds(300)) == sf::Socket::Done) {
                                selector.add(MySocket);

                                if (selector.wait(sf::seconds(1)) && selector.isReady(MySocket) &&
                                    MySocket.receive(ReceivePacket) == sf::Socket::Done) {
                                    while (!ReceivePacket.endOfPacket()) {
                                        ReceivePacket >> pacetStates::curState;
                                        switch (pacetStates::curState) {
                                            case pacetStates::PlayersAmount:
                                                ReceivePacket >> ComputerID;
                                                std::cout << "My ID = " << ComputerID << '\n';
                                                break;
                                            case pacetStates::PlayerConnect:
                                                ReceivePacket >> PacetData;
                                                ListOfPlayers.addWord(PacetData);
                                                ConnectedPlayers.push_back(*(new Player()));
                                                std::cout << PacetData << " connected\n";
                                                break;
                                            case pacetStates::SetPos:
                                                for (Player& x: ConnectedPlayers) {
                                                    ReceivePacket >> x;
                                                }
                                                player.setPosition(ConnectedPlayers[ComputerID].getPosition());
                                        }
                                    }
                                }
                                ClientFuncRun = true;
                                ClientTread.launch();
                            }
                            IPPanel.setWord("IP:" + IPOfHost);
                        } else if (event.key.code == sf::Keyboard::BackSpace && IPOfHost.size() > 0) {
                            IPOfHost.pop_back();
                            IPPanel.setWord("IP:" + IPOfHost);
                        }
                    }
                    break;
                }
        }
    }
}

bool useItem(ItemID::Type id) {
    ///////////////////////////////////// PLACEHOLDER ACTIONS. ALL ITEM FUNCTIONS ARE NOT FINAL
    switch (id) {
        case ItemID::regenDrug:
            AllEffects.push_back(new Effect(&player, Effects::Heal, 60.f, sf::seconds(1.f)));
            return true;

        default:
            return false;
    }
}

void createSlotRects() {
    std::vector<std::map<ItemID::Type, Item*>*> currInventory
    {
        &player.inventory.keyItems,
        &player.inventory.safeItems,
        &player.inventory.equipItems,
        &player.inventory.dropableItems
    };

    if (!itemSlotsRects.empty()) {
        for (ItemID::Type id = 0; id != ItemID::NONE; id++) {
            if (itemSlotsRects.find(id) != itemSlotsRects.end()) {
                delete itemSlotsRects[id];
            }
        }
        itemSlotsRects.clear();
    }

    for (int i = 0; i < 4; i++) {
        int slotNumber = 0;
        for (ItemID::Type id = 0; id != ItemID::NONE; id++) {
            if ((currInventory[i]->find(id) != currInventory[i]->end()) &&
                (*currInventory[i])[id]->amount > 0) {

                float itemX = int(slotNumber % 6) * 150 + itemListBG.getPosition().x + 50;
                float itemY = int(slotNumber / 6) * 150 + itemListBG.getPosition().y + 50;

                Rect* itemActivationRect = new Rect();
                itemActivationRect->setPosition(itemX, itemY);
                itemActivationRect->setSize(Textures::ItemPanel.getSize().x / 2.0f, Textures::ItemPanel.getSize().y / 2.0f);
                itemSlotsRects[id] = itemActivationRect;

                slotNumber++;
            }
        }
    }
}

void updateBullets() {
    for (int i = 0; i < Bullets.size(); i++) {
        if (Bullets[i].penetration < 0 || Bullets[i].todel) {
            Bullets.erase(Bullets.begin() + i--);
        } else {
            Bullets[i].move(CurLocation);
            if (!faction::friends(Bullets[i].fromWho, player.faction) && player.intersect(Bullets[i])) {
                player.getDamage(Bullets[i].damage);
                Bullets[i].penetration--;
            }
            for (Enemy* &enemy: Enemies) {
                if (!faction::friends(Bullets[i].fromWho, enemy->faction) && enemy->intersect(Bullets[i])) {
                    enemy->getDamage(Bullets[i].damage);
                    Bullets[i].penetration--;
                    TempText* tempText = new TempText(sf::seconds(1.5f));
                    tempText->setCharacterSize(30);
                    tempText->setOutlineColor(sf::Color::White);
                    tempText->setOutlineThickness(3);
                    tempText->setString(std::to_string(int(Bullets[i].damage)));
                    tempText->setFillColor(sf::Color(250, 50, 50, 200));
                    tempText->setCenter(enemy->getPosition());
                    TempTextsOnGround.push_back(tempText);
                }
            }
        }
    }
}

void MainLoop() {
    while (window.isOpen()) {

        if (player.Health.toBottom() == 0) {
            EscapeButton.buttonFunction();
        }

        for (int i = 0; i < Enemies.size(); i++) {
            if (Enemies[i]->Health.toBottom() == 0) {
                if (Enemies[i]->dropInventory) {
                    for (std::map<ItemID::Type, Item*>::iterator it = Enemies[i]->inventory.dropableItems.begin();
                        it != Enemies[i]->inventory.dropableItems.end(); it++) {
                        it->second->dropTo(Enemies[i]->getPosition());

                        PickupStuff.push_back(it->second);
                        DrawableStuff.push_back(it->second);
                    }
                }
                Enemies[i]->inventory.dropableItems.clear();

                DeleteFromVector(DrawableStuff, static_cast<sf::Drawable*>(Enemies[i]));
                delete Enemies[i];
                Enemies.erase(Enemies.begin() + i--);
            } else {
                Enemies[i]->setTarget(player.getPosition());
                Enemies[i]->move(CurLocation);
                Enemies[i]->UpdateState();
                Enemies[i]->CurWeapon->lock = false;
                Enemies[i]->CurWeapon->Shoot(*Enemies[i], player.getPosition(), Enemies[i]->faction);
                Enemies[i]->CurWeapon->Reload(Enemies[i]->Mana);
            }
        }

        for (int i = 0; i < FireSet.size(); i++) {
            if (FireSet[i]->clock->getElapsedTime() >= FireSet[i]->secs) {
                FireSet[i]->Propagation();
                bool add1 = true;
                bool add2 = true;
                for (int j = 0; j < FireSet.size(); j++) {
                    if (FireSet[i]->descendant1->intersect(*FireSet[j])) add1 = false;
                    if (FireSet[i]->descendant2->intersect(*FireSet[j])) add2 = false;
                }
                if (add1) {
                    FireSet.push_back(FireSet[i]->descendant1);
                    FireSet[FireSet.size() - 1]->setAnimation(Textures::Fire, 1, 1, sf::seconds(1), &Shaders::Map);
                    DrawableStuff.push_back(FireSet[i]->descendant1);
                } else {
                    delete FireSet[i]->descendant1;
                }
                if (add2) {
                    FireSet.push_back(FireSet[i]->descendant2);
                    FireSet[FireSet.size() - 1]->setAnimation(Textures::Fire, 1, 1, sf::seconds(1), &Shaders::Map);
                    DrawableStuff.push_back(FireSet[i]->descendant2);
                } else {
                    delete FireSet[i]->descendant2;
                }
                DeleteFromVector(DrawableStuff, static_cast<sf::Drawable*>(FireSet[i]));
                delete FireSet[i];
                FireSet.erase(FireSet.begin() + i--);
            }
        }

        player.UpdateState();
        if (screen == screens::Dungeon) {
            if (Musics::Fight1.getDuration() - Musics::Fight1.getPlayingOffset() < sf::seconds(0.3f)) {
                Musics::Fight2.play();
            }
            if (Musics::Fight2.getDuration() - Musics::Fight2.getPlayingOffset() < sf::seconds(0.3f)) {
                Musics::Fight1.play();
            }
        }
        if (!window.hasFocus()) {
            if (player.CurWeapon != nullptr) {
                player.CurWeapon->Shoot(player, window.mapPixelToCoords(sf::Mouse::getPosition()), player.faction);
            }
            updateBullets();

            sf::Event event;
            while (window.pollEvent(event)) {}

            if (HostFuncRun) {
                mutex.lock();
                SendPacket << pacetStates::PlayerPos;
                for (Player& x: ConnectedPlayers) {
                    SendPacket << x;
                }
                SendToClients(SendPacket);
                SendPacket.clear();
                mutex.unlock();
            }
        } else {
            if (!chat.inputted) {
                player.move(CurLocation);
                GameView.setCenter(player.getPosition() + static_cast<sf::Vector2f>((sf::Mouse::getPosition() - sf::Vector2i{scw, sch} / 2) / 8));
                FindAllWaysTo(CurLocation, player.getPosition(), TheWayToPlayer);
            }
            int wasBulletsSize = Bullets.size();
            if (player.CurWeapon != nullptr) {
                player.CurWeapon->Shoot(player, window.mapPixelToCoords(sf::Mouse::getPosition()), player.faction);
            }

            if (wasBulletsSize < Bullets.size() && (HostFuncRun || ClientFuncRun)) {
                mutex.lock();
                SendPacket << pacetStates::Shooting << (int)Bullets.size() - wasBulletsSize;
                for (; wasBulletsSize < Bullets.size(); wasBulletsSize++) {
                    SendPacket << Bullets[wasBulletsSize];
                }
                if (HostFuncRun) {
                    SendToClients(SendPacket);
                } else if (ClientFuncRun) {
                    MySocket.send(SendPacket);
                }
                SendPacket.clear();
                mutex.unlock();
            }

            updateBullets();

            if (HostFuncRun || ClientFuncRun) {
                ConnectedPlayers[ComputerID].setPosition(player.getPosition());
                mutex.lock();
                SendPacket << pacetStates::PlayerPos;
                if (HostFuncRun) {
                    for (Player& x: ConnectedPlayers) {
                        SendPacket << x;
                    }
                    SendToClients(SendPacket);
                } else if (ClientFuncRun) {
                    SendPacket << player;
                    MySocket.send(SendPacket);
                }
                SendPacket.clear();
                mutex.unlock();
            }

            if (MiniMapActivated) {
                if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
                    MiniMapView.move(-sf::Vector2f(sf::Mouse::getPosition() - MouseBuffer) * MiniMapZoom);
                }
            }
            MouseBuffer = sf::Mouse::getPosition();

            if (screen == screens::Dungeon) {
                if (Enemies.size() == 0 && !in(InteractibeStuff, (Interactable*)&portal)) {
                    InteractibeStuff.push_back(&portal);
                }
            }
        }

        draw();

        if (puddle.intersect(player))
            AllEffects.push_back(new Effect(&player, Effects::Heal, 30.f, sf::seconds(1.5f)));

        for (Fire* &fire: FireSet) {
            if (fire->intersect(player))
                AllEffects.push_back(new Effect(&player, Effects::Damage, 0.1f, sf::seconds(2.f)));
        }
        processEffects();

        EventHandler();
    }
}

bool IsSomeOneCanBeActivated() {
    for (Interactable*& x: InteractibeStuff) {
        if (x->CanBeActivated(player)) {
            return true;
        }
    }
    return false;
}

void FillFloorRects() {
    sf::Image res, src = Textures::floor1x.copyToImage();
    auto CreateOneFLoor = [&src](sf::Image& res){
        for (int y = 0; y < 5; y++) {
            for (int x = 0; x < 5; x++) {
                res.copy(src, x * 32, y * 32, {(rand() % 5) * 32, 0, 32, 32});
            }
        }
    };
    res.create(160, 160);
    FloorTextureRects.clear();
    for (int i = 0; i < CurLocation->n; i++) {
        for (int j = 0; j < CurLocation->m; j++) {
            if (CurLocation->EnableTiles[i][j]) {
                CreateOneFLoor(res);
                FloorTextureRects.push_back(sf::Texture());
                FloorTextureRects.rbegin()->loadFromImage(res); // MEMORY LEAK!
            }
        }
    }
}

void updateShaders() {
    sf::Vector2f u_mouse(sf::Mouse::getPosition());
    float u_time = GameClock->getElapsedTime().asSeconds();
    sf::Vector2f u_playerPosition(window.mapCoordsToPixel(player.getPosition()));

    Shaders::Map.setUniform("u_mouse", u_mouse);
    Shaders::Map.setUniform("u_time", u_time);
    Shaders::Map.setUniform("u_playerPosition", u_playerPosition);

    Shaders::Player.setUniform("u_mouse", u_mouse);
    Shaders::Player.setUniform("u_time", u_time);
    Shaders::Player.setUniform("u_playerPosition", u_playerPosition);

    Shaders::Portal.setUniform("u_mouse", u_mouse);
    Shaders::Portal.setUniform("u_time", u_time);
    Shaders::Portal.setUniform("u_playerPosition", u_playerPosition);

    Shaders::Architect.setUniform("u_mouse", u_mouse);
    Shaders::Architect.setUniform("u_time", u_time);
    Shaders::Architect.setUniform("u_playerPosition", u_playerPosition);

    Shaders::PickupItem.setUniform("u_time", u_time);
}

void processEffects() {
    for (int i = AllEffects.size() - 1; i >= 0; i--) {
        if (AllEffects[i]->secs <= sf::Time::Zero) {
            delete AllEffects[i];
            std::swap(AllEffects[i], AllEffects[AllEffects.size() - 1]);
            AllEffects.pop_back();
        } else {
            float t = std::min(AllEffects[i]->clock->restart().asSeconds(), AllEffects[i]->secs.asSeconds());
            AllEffects[i]->secs -= sf::seconds(t);
            switch (AllEffects[i]->type) {
                case Effects::Damage:
                    AllEffects[i]->owner->getDamage(AllEffects[i]->parameter * t);
                    break;
                case Effects::Heal:
                    AllEffects[i]->owner->getDamage(-AllEffects[i]->parameter * t);
                    break;
                default:
                    break;
            }
        }
    }
}

void setBox(Interactable*& box) {
    box->setAnimation(Textures::Box, 1, 1, sf::seconds(1.f), &Shaders::Map);
    box->setSize(105.f, 117.f);
    box->setFunction([](Interactable* i){
        if (player.Mana.cur >= 20) {
            player.Mana -= 20.f;
            player.AddItem(new Item(ItemID::coin, rand() % 5));
            isInventoryUpToDate = false;
            DeleteFromVector(listOfBox, i);
            DeleteFromVector(DrawableStuff, (sf::Drawable*)i);
            DeleteFromVector(InteractibeStuff, i);
            LabyrinthLocation.DelObject({Tiles::box, i->getPosition()});
            delete i;
        }
    });
}

void setArtifact(Interactable*& artifact) {
    artifact->setAnimation(Textures::Architect, 1, 1, sf::seconds(1), &Shaders::Architect);
    artifact->setSize(150.f, 150.f);
    artifact->setFunction([](Interactable* i){
        TempText* tempText = new TempText(sf::seconds(2.5f));
        tempText->setCharacterSize(50);
        tempText->setOutlineColor(sf::Color::White);
        tempText->setOutlineThickness(3);
        switch (rand() % 4) {
            case 0:
                player.Health.top += 20;
                tempText->setString("You have found an Artifact\nHealth increased");
                tempText->setFillColor(sf::Color(250, 50, 50));
                break;
            case 1:
                player.Mana.top += 20;
                tempText->setString("You have found an Artifact\nMana increased");
                tempText->setFillColor(sf::Color(50, 50, 250));
                break;
            case 2:
                player.HealthRecovery += 2;
                tempText->setString("You have found an Artifact\nHealth Recovery increased");
                tempText->setFillColor(sf::Color(250, 50, 50));
                break;
            case 3:
                player.ManaRecovery += 2;
                tempText->setString("You have found an Artifact\nMana Recovery increased");
                tempText->setFillColor(sf::Color(50, 50, 250, 200));
                break;
            default: break;
        }
        tempText->setCenter(scw / 2.f, sch / 2.f - 165.f);

        TempTextsOnScreen.push_back(tempText);
        DeleteFromVector(listOfArtifact, i);
        DeleteFromVector(DrawableStuff, (sf::Drawable*)i);
        DeleteFromVector(InteractibeStuff, i);
        CurLocation->DelObject({Tiles::artifact, i->getPosition()});
        delete i;
    });
}

template <typename T> void clearVectorOfPointer(std::vector<T>& arr) {
    for (size_t i = 0; i < arr.size(); i++) {
        delete arr[i];
    }
    arr.clear();
}

//////////////////////////////////////////////////////////// Server-Client functions
void ClientConnect() {
    Client* client = new Client;
    if (listener.accept(*client) == sf::Socket::Done) {
        mutex.lock();

        std::string ConnectedClientIP = (*client).getRemoteAddress().toString();
        SendPacket << pacetStates::PlayerConnect << ConnectedClientIP;
        SendToClients(SendPacket);
        SendPacket.clear();

        std::cout << ConnectedClientIP << " connected\n";
        ListOfPlayers.addWord(ConnectedClientIP);
        std::cout << "list of players:\n" << std::string(ListOfPlayers.text.getString()) << '\n';

        clients.push_back(client);
        selector.add(*client);

        ConnectedPlayers.push_back(*(new Player()));
        ConnectedPlayers[ConnectedPlayers.size() - 1].setPosition(float(CurLocation->m) * size / 2, float(CurLocation->n) * size / 2);
        SendPacket << pacetStates::PlayersAmount << (sf::Int32)ConnectedPlayers.size() - 1;

        DrawableStuff.push_back(&(ConnectedPlayers[ConnectedPlayers.size() - 1]));

        for (int i = 0; i < ListOfPlayers.size(); i++) {
            SendPacket << pacetStates::PlayerConnect << ListOfPlayers[i];
        }

        SendPacket << pacetStates::Shooting << (sf::Int32)Bullets.size();
        std::cout << "bullets: " << Bullets.size() << "\n";
        for (int i = 0; i < Bullets.size(); i++) {
            SendPacket << Bullets[i];
        }

        std::cout << "amount players = " << ConnectedPlayers.size() - 1 << '\n';
        SendPacket << pacetStates::SetPos;
        for (Player& x: ConnectedPlayers) {
            SendPacket << x;
        }

        if (client->send(SendPacket) == sf::Socket::Done) {
            std::cout << "SendPacket was sended\n";
        } else {
            std::cout << "SendPacket didn't sended\n";
        }

        SendPacket.clear();
        SendPacket << LabyrinthLocation;
        if (client->send(SendPacket) == sf::Socket::Done) {
            std::cout << "Labyrinth walls sended\n";
        } else {
            std::cout << "Labyrinth walls didn't sended\n";
        }
        SendPacket.clear();

        mutex.unlock();
    } else delete client;
}

void ClientDisconnect(int i) {
    selector.remove(*clients[i]);
    std::cout << (*clients[i]).getRemoteAddress().toString() << " disconnected; number = " << i << "\n";
    delete clients[i];
    clients.erase(clients.begin() + i);
    ConnectedPlayers.erase(ConnectedPlayers.begin() + i + 1);
    ListOfPlayers.removeWord(i);

    std::cout << "amount of clients = " << clients.size() << "\n";
    mutex.lock();
    SendPacket << pacetStates::PlayerDisconnect << i;
    SendToClients(SendPacket);
    SendPacket.clear();
    mutex.unlock();
}

void SelfDisconnect() {
    std::cout << "SelfDisconnect\n";
    ClientFuncRun = false;
    screen = screens::MainRoom;
    LoadMainMenu();
    mutex.lock();
    SendPacket << pacetStates::disconnect;
    MySocket.send(SendPacket);
    SendPacket.clear();
    mutex.unlock();
    MySocket.disconnect();
    selector.clear();
    ConnectedPlayers.clear();
}

void SendToClients(sf::Packet& pac, int ExceptOf) {
    for (int i = 0; i < clients.size(); i++) {
        if (i != ExceptOf && clients[i]->send(pac) != sf::Socket::Done) {}
    }
}

void funcOfHost() {
    std::cout << "Server turn on\n";
    while (HostFuncRun) {
        if (selector.wait(sf::seconds(1))) {
            if (selector.isReady(listener)) {
                ClientConnect();
            } else {
                for (int i = 0; i < clients.size(); i++) {
                    if (selector.isReady(*clients[i]) && clients[i]->receive(ReceivePacket) == sf::Socket::Done) {
                        while (!ReceivePacket.endOfPacket()) {
                            ReceivePacket >> pacetStates::curState;
                            switch (pacetStates::curState) {
                                case pacetStates::disconnect:
                                    std::cout << "client self disconect\n";
                                    ClientDisconnect(i--);
                                    break;
                                case pacetStates::PlayerPos:
                                    ReceivePacket >> ConnectedPlayers[i + 1];
                                    break;
                                case pacetStates::ChatEvent:
                                    ReceivePacket >> PacetData;
                                    chat.addLine(PacetData);
                                    mutex.lock();
                                    SendPacket << pacetStates::ChatEvent << PacetData;
                                    SendToClients(SendPacket, i);
                                    SendPacket.clear();
                                    mutex.unlock();
                                    break;
                                case pacetStates::Shooting: {
                                    mutex.lock();
                                    int i; ReceivePacket >> i;
                                    SendPacket << pacetStates::Shooting << i;
                                    for (; i > 0; i--) {
                                        ReceivePacket >> tempBullet;
                                        Bullets.push_back(tempBullet);
                                        SendPacket << tempBullet;
                                    }
                                    SendToClients(SendPacket, i);
                                    SendPacket.clear();
                                    mutex.unlock();
                                    break;
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    std::cout << "Server turn off\n";
}

void funcOfClient() {
    while (ClientFuncRun) {
        if (selector.wait(sf::seconds(1))) {
            if (selector.isReady(MySocket) && MySocket.receive(ReceivePacket) == sf::Socket::Done) {
                while (!ReceivePacket.endOfPacket()) {
                    ReceivePacket >> pacetStates::curState;
                    switch (pacetStates::curState) {
                        case pacetStates::disconnect:
                            SelfDisconnect();
                            break;
                        case pacetStates::PlayerConnect:
                            ReceivePacket >> PacetData;
                            ListOfPlayers.addWord(PacetData);
                            ConnectedPlayers.push_back(*(new Player()));
                            std::cout << PacetData + " connected\n";
                            break;
                        case pacetStates::PlayerDisconnect:
                            int index;
                            ReceivePacket >> index;
                            std::cout << std::string(ListOfPlayers[index]) << " disconnected\n";
                            if (index < ComputerID) ComputerID--;
                            ListOfPlayers.removeWord(index);
                            ConnectedPlayers.erase(ConnectedPlayers.begin() + index);
                            break;
                        case pacetStates::Labyrinth:
                            std::cout << "Labyrinth receiving\n";
                            ReceivePacket >> LabyrinthLocation;
                            std::cout << "Labyrinth receive\n";
                            break;
                        case pacetStates::PlayerPos:
                            for (int i = 0; i < ConnectedPlayers.size(); i++) {
                                if (i != ComputerID)
                                    ReceivePacket >> ConnectedPlayers[i];
                                else {
                                    sf::Vector2i tempPoint;
                                    ReceivePacket >> tempPoint;
                                }
                            }
                            break;
                        case pacetStates::SetPos:
                            for (Player& x: ConnectedPlayers) {
                                ReceivePacket >> x;
                            }
                            player.setPosition(ConnectedPlayers[ComputerID].getPosition());
                            GameView.setCenter(player.getPosition());
                            break;
                        case pacetStates::ChatEvent:
                            ReceivePacket >> PacetData;
                            chat.addLine(PacetData);
                            break;
                        case pacetStates::Shooting: {
                            int i; ReceivePacket >> i;
                            for (; i > 0; i--) {
                                ReceivePacket >> tempBullet;
                                Bullets.push_back(tempBullet);
                            }
                            break;
                        }
                    }
                }
            }
        }
    }
}
