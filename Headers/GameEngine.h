#pragma once
#include "Entities/enemy.h"
#include "Entities/player.h"
#include "Multiplayer/chat.h"
#include "Multiplayer/client.h"
#include "UI/button.h"
#include "UI/panel.h"
#include "UI/bar.h"
#include "UI/tempText.h"
#include "UI/ItemSlot.h"
#include "UI/shopSlot.h"
#include "Systems/effect.h"
#include "Systems/shop.h"
#include "LevelSystem/fire.h"


//////////////////////////////////////////////////////////// Settings of the game
bool IsDrawMinimap       = true;
bool IsDrawInterface     = true;
bool MiniMapHoldOnPlayer = true;


//////////////////////////////////////////////////////////// Stuff for work with system and screen
sf::ContextSettings settings;


sf::RenderWindow window(sf::VideoMode(scw, sch), "multigame", sf::Style::Fullscreen, settings);
sf::RenderTexture preRenderTexture;
sf::Sprite preRenderSprite;
float MiniMapZoom = 1.f;
bool MiniMapActivated, EscapeMenuActivated, isDrawInventory, isDrawShop;
std::vector<sf::Drawable*> DrawableStuff, InterfaceStuff; // references to objects that exist somewhere
std::vector<Interactable*> InteractibeStuff; // references to objects that exist somewhere

std::vector<Item*> PickupStuff;


//////////////////////////////////////////////////////////// Players
Player player;
std::vector<Player> ConnectedPlayers;


//////////////////////////////////////////////////////////// DrawableStuff
sf::Sprite WallRect, FLoorTileSprite;
std::vector<sf::Texture> FloorTextureRects;
std::vector<TempText*> TempTextsOnScreen, TempTextsOnGround, DamageText, MessageText;
Bar<float> EnemyHealthBar;
sf::Sprite undergroundBG;


//////////////////////////////////////////////////////////// MiniMapStuff
sf::CircleShape MMPlayerCircle, MMEnemyCircle; // MM - MiniMap prefix
sf::RectangleShape MMPortalRect, MMBoxRect, MMPuddleRect, MMArtifact;


//////////////////////////////////////////////////////////// InterfaceStuff
Bar<float> ManaBar, HpBar;
Bar<float> AmmoBar;
PlacedText WeaponNameText;
PlacedText ReloadWeaponText;
sf::Sprite XButtonSprite;

std::vector<sf::Sprite*> effectIcons(numberOfEffects);
std::vector<TempText*> effectIconsTimers(numberOfEffects);


//////////////////////////////////////////////////////////// InventoryStuff
inventoryPage::Type activeInventoryPage = inventoryPage::Items;
std::vector<sf::Drawable*> invCommonElements; // These elements appear on every page
int prevItemTypeCount = 0;

Button invBackButton("Back", [](){
    isDrawInventory = false;
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
sf::Sprite invBackground;
sf::Sprite itemListBG;

Animation playerCoinSprite;
ItemSlot playerCoinSlot;   // Special slot, so that it can be conveniently used other than in the inventory

std::vector<std::vector<sf::Drawable*>> invPageElements; // These elements only appear on certain pages
std::vector<ItemSlot> invItemSlotsElements; // Elements that comprise an inventory slot - the background texture and the amount text
std::vector<sf::FloatRect*> invItemSlotsRects; // The slot itself. This is what activates when a player clicks on an item.

sf::Sprite statsPlayerImage;
Bar<float> statsHPBar;
Bar<float> statsMPBar;

PlacedText statsArmorText,
           statsHPText,
           statsMPText,
           statsHPRegenText,
           statsMPRegenText,
           statsCompletedLevelsText,
           statsCurLevelsText;

bool isItemDescDrawn = false;
std::vector<bool> doInventoryUpdate(inventoryPage::NONE, false);
PlacedText itemDescText;
ItemID::Type prevItemDescID;


//////////////////////////////////////////////////////////// Shop Interface
Shop mainMenuShop;
Shop* curShop = nullptr;
Item* shopSelectedItem = nullptr;

std::vector<sf::Drawable*> shopUIElements;
std::vector<ShopSlot> shopItemSlotsElements; // Analogous to the inventory itemSlotsElements + price
std::vector<ShopSlot> shopPlayerSlotsElements; // Analogous to the inventory itemSlotsElements + price
std::vector<sf::FloatRect*> shopItemSlotsRects; // Analogous to the inventory itemSlotsRects

sf::Sprite shopBG, shopBGPattern;
sf::Sprite shopNPCTextFrame;
sf::Sprite shopItemsFrame, shopPlayerInventoryFrame, shopItemStatsFrame;
sf::Sprite shopItemSpriteFrame, shopItemSprite;

sf::Sprite shopNPCSprite;
Animation shopItemCoinsSprite;

float shopItemsViewSizeX, shopItemsViewSizeY;
float shopPlayerInvViewSizeX, shopPlayerInvViewSizeY;

PlacedText shopNPCName, shopNPCText, shopItemCoins, shopItemStats, shopPlayerCoinsText;



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
             puddle,
             shopSector;
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


//////////////////////////////////////////////////////////// Chat
Chat chat(scw, sch);


//////////////////////////////////////////////////////////// Other stuff
sf::Vector2i MouseBuffer;
Bullet* tempBullet;


//////////////////////////////////////////////////////////// Weapons
Pistol pistol;
Shotgun shotgun;
Revolver revolver;
Rifle rifle;
Bubblegun bubblegun;
Armageddon armageddon;
Chaotic chaotic;
FireHose fireHose;
Flamethrower flamethrower;
std::vector<Weapon*> arsenal = {
    &pistol,
    &shotgun,
    &rifle,
};
Scale<int> CurWeapon{0, 2, 0};


//////////////////////////////////////////////////////////// Enemies
std::vector<Enemy*> Enemies;


//////////////////////////////////////////////////////////// functions

//---------------------------- INITS
void init();
void initInventory();
void initShop();
//----------------------------


//---------------------------- DRAW FUNCTIONS
void draw();
void drawFloor();
void drawWalls();
void drawMiniMap();
void drawInterface();
void drawEffects();
void drawInventory();
void drawShop();
//----------------------------


//---------------------------- UI UPDATERS/CREATORS
void updateInventoryUI();
void createSlotRects();

void updateShopUI();
void createShopSlotsRects();
//----------------------------


//---------------------------- EVENT HANDLERS
void EventHandler();
void inventoryHandler(sf::Event&);
void shopHandler(sf::Event&);
//----------------------------


//---------------------------- LEVEL GENERATION FUNCTIONS
void LevelGenerate(int, int);
void LoadMainMenu();
void FillFloorRects();

void setBox(Interactable*&);
void setArtifact(Interactable*&);
//----------------------------


//---------------------------- GAME STATE FUNCTIONS
void updateBullets();

bool useItem(Item*&);

void processEffects();
void updateEffects(Creature*);
void applyEffect(Creature&, Effect*);
void clearEffect(Creature&, Effect*);

void fireUpdate();
bool firePropagationAllowed(sf::Vector2i&, sf::Vector2i);
//----------------------------


//---------------------------- GAME GRAPHICS FUNCTIONS
void updateShaders();
//----------------------------


//---------------------------- HELPER FUNCTIONS
bool CanSomethingBeActivated();

void saveGame();
void loadSaves();
//----------------------------


void MainLoop(); // SELF-EXPLANATORY


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


//////////////////////////////////////////////////////////// Panels
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
    EscapeMenuActivated = false;
    ListOfPlayers.clear();
    clearVectorOfPointer(Bullets);
    clearVectorOfPointer(Enemies);
    clearVectorOfPointer(FireSet);
    clearVectorOfPointer(TempTextsOnGround);
    clearVectorOfPointer(TempTextsOnScreen);
    clearVectorOfPointer(DamageText);
    clearVectorOfPointer(MessageText);
    clearVectorOfPointer(listOfBox);
    clearVectorOfPointer(listOfArtifact);
    clearVectorOfPointer(PickupStuff);
    player.CurWeapon->lock = true;
    if (DeleteFromVector(arsenal, (Weapon*)&fireHose))     CurWeapon.top -= 1;
    if (DeleteFromVector(arsenal, (Weapon*)&flamethrower)) CurWeapon.top -= 1;
    normalize(CurWeapon);
    LoadMainMenu();
    saveGame();
});

Button shopBackButton("Back", [](){
    isDrawShop = false;
    shopSelectedItem = nullptr;
});
Button shopBuyButton("Buy", [](){
    curShop->buyFunction();
});


//////////////////////////////////////////////////////////// functions realizations

//============================================================================================== INITS
void init() {
    setlocale(LC_ALL, "rus");

    window.setVerticalSyncEnabled(true);
    settings.antialiasingLevel = 8;
    window.setView(GameView);

    preRenderTexture.create(scw, sch);
    preRenderTexture.setView(GameView);
    preRenderSprite.setTexture(preRenderTexture.getTexture());

    MiniMapView.setViewport(sf::FloatRect(0.f, 0.f, 0.25f, 0.25f));
    MiniMapZoom = std::pow(1.1, -10);
    MiniMapView.zoom(MiniMapZoom);
    GameClock = new sf::Clock;

    // Load locations
    WaitingRoomLoaction.LoadFromFile("sources/locations/WaitingRoom.txt");
    MainMenuLocation.LoadFromFile("sources/locations/MainMenu.txt");

    loadTextures();
    loadItemTextures();
    loadFonts();
    loadShaders();
    loadMusics();
    loadSaves();

    Musics::MainMenu.setLoop(true);
    Musics::MainMenu.setVolume(5);
    Musics::Fight1.setVolume(5);
    Musics::Fight2.setVolume(5);

    portal.setAnimation(Textures::PortalAnimation2, 9, 1, sf::seconds(1), &Shaders::Portal);
    portal.setSize(170.f, 320.f);
    player.setAnimation(Textures::Player, &Shaders::Player);
    puddle.setAnimation(Textures::Puddle, &Shaders::Flashlight);
    puddle.setSize(90.f, 90.f);
    shopSector.setAnimation(Textures::INVISIBLE);
    shopSector.setPosition(0, 2 * size);

    Shaders::Flashlight.setUniform("uResolution", sf::Vector2f(scw, sch));
    Shaders::Flashlight.setUniform("u_playerRadius", player.getRadius());

    Shaders::Distortion1.setUniform("noise_png", Textures::Noise);

    Shaders::Distortion2.setUniform("noise_png", Textures::Noise);

    Shaders::Outline.setUniform("uResolution", sf::Vector2f(scw, sch));

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

    MMEnemyCircle.setRadius(9);
    MMEnemyCircle.setFillColor(sf::Color(180, 0, 0));
    MMEnemyCircle.setOrigin(MMEnemyCircle.getRadius(), MMEnemyCircle.getRadius());

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

    FLoorTileSprite.setScale(3.f, 3.f);

    undergroundBG.setTexture(Textures::Noise);
    undergroundBG.setPosition(0, 0);
    undergroundBG.setScale(scw / undergroundBG.getLocalBounds().width, sch / undergroundBG.getLocalBounds().height);

    for (int i = 0; i < numberOfEffects; i++) {
        effectIconsTimers[i] = new TempText(sf::Time::Zero);
        effectIcons[i] = new sf::Sprite();
    }
    effectIcons[2]->setTexture(Textures::Eff_HPRegen);
    effectIcons[3]->setTexture(Textures::Eff_Burn);

    initInventory();
    initShop();

    LoadMainMenu();
}

void initInventory() {
    invPageElements.resize(inventoryPage::NONE);
    invItemSlotsElements.resize(MaxItemID, ItemSlot());
    invItemSlotsRects.resize(MaxItemID);

    invBackground.setTexture(Textures::SteelFrame);
    invBackground.setScale((float)scw / Textures::SteelFrame.getSize().x, (float)sch / Textures::SteelFrame.getSize().y);
    invBackground.setPosition(0, 0);

    int xOffset = 50, yOffset = 50;

    invBackButton.setTexture(Textures::RedPanel, Textures::RedPanelPushed);
    invBackButton.setCharacterSize(52);
    invBackButton.setPosition(xOffset, yOffset);
    invBackButton.setSize(300, 150);

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

    invCommonElements.push_back(&invBackground);
    invCommonElements.push_back(&invBackButton);
    invCommonElements.push_back(&itemsPageButton);
    invCommonElements.push_back(&weaponsPageButton);
    invCommonElements.push_back(&equipablesPageButton);
    invCommonElements.push_back(&perksPageButton);
    invCommonElements.push_back(&statsPageButton);


    statsPlayerImage.setTexture(Textures::Player);
    statsPlayerImage.setPosition(5 * scw / 6 - Textures::Player.getSize().x / 2, sch / 2 - Textures::Player.getSize().y / 2);

    statsHPBar.setValue(player.Health);
    statsHPBar.setSize(scw / 10, sch / 20);
    statsHPBar.setPosition(3 * scw / 10, 2 * sch / 10);
    statsHPBar.setColors(sf::Color(255, 255, 255, 160), sf::Color(192, 0, 0, 160), sf::Color(32, 32, 32, 160));
    statsHPText.setCharacterSize(24);
    statsHPText.setString("Health");
    statsHPText.setPosition(scw / 10, 2 * sch / 10);

    statsMPBar.setValue(player.Mana);
    statsMPBar.setSize(scw / 10, sch / 20);
    statsMPBar.setPosition(3 * scw / 10, 3 * sch / 10);
    statsMPBar.setColors(sf::Color(255, 255, 255, 160), sf::Color(0, 0, 192, 160), sf::Color(32, 32, 32, 160));
    statsMPText.setCharacterSize(24);
    statsMPText.setString("Mana");
    statsMPText.setPosition(scw / 10, 3 * sch / 10);

    statsHPRegenText.setCharacterSize(24);
    statsHPRegenText.setString("Health regen: " + floatToString(player.HealthRecovery));
    statsHPRegenText.setPosition(scw / 10, 4 * sch / 10);

    statsMPRegenText.setCharacterSize(24);
    statsMPRegenText.setString("Mana regen: " + floatToString(player.ManaRecovery));
    statsMPRegenText.setPosition(scw / 10, 5 * sch / 10);

    statsArmorText.setCharacterSize(24);
    statsArmorText.setString("Armor: " + floatToString(player.Armor.cur));
    statsArmorText.setPosition(scw / 10, 6 * sch / 10);

    statsCompletedLevelsText.setCharacterSize(24);
    statsCompletedLevelsText.setString("Completed Levels: " + std::to_string(completedLevels));
    statsCompletedLevelsText.setPosition(scw / 10, 7 * sch / 10);

    statsCurLevelsText.setCharacterSize(24);
    statsCurLevelsText.setString("Current Levels: " + std::to_string(curLevel));
    statsCurLevelsText.setPosition(scw / 10, 8 * sch / 10);

    playerCoinSprite = Animation(*itemTextureName[ItemID::coin], itemTextureFrameAmount[ItemID::coin],
                                  1, itemTextureDuration[ItemID::coin]);
    playerCoinSprite.play();
    playerCoinSlot.background = new sf::Sprite();
    playerCoinSlot.amountText = new PlacedText();
    playerCoinSlot.isInitialized = true;

    invPageElements[inventoryPage::Items].push_back(&itemListBG);
    invPageElements[inventoryPage::Items].push_back(&playerCoinSprite);
    invPageElements[inventoryPage::Stats].push_back(&statsPlayerImage);
    invPageElements[inventoryPage::Stats].push_back(&statsHPBar);
    invPageElements[inventoryPage::Stats].push_back(&statsMPBar);
    invPageElements[inventoryPage::Stats].push_back(&statsHPText);
    invPageElements[inventoryPage::Stats].push_back(&statsMPText);
    invPageElements[inventoryPage::Stats].push_back(&statsHPRegenText);
    invPageElements[inventoryPage::Stats].push_back(&statsMPRegenText);
    invPageElements[inventoryPage::Stats].push_back(&statsArmorText);
    invPageElements[inventoryPage::Stats].push_back(&statsCompletedLevelsText);
    invPageElements[inventoryPage::Stats].push_back(&statsCurLevelsText);

    doInventoryUpdate[inventoryPage::Stats] = true;
}

void initShop() {
    shopItemSlotsElements.resize(MaxItemID, ShopSlot());
    shopPlayerSlotsElements.resize(MaxItemID, ShopSlot());
    shopItemSlotsRects.resize(MaxItemID);
    mainMenuShop.setShop(new std::vector<Item*>{new Item(ItemID::regenDrug, 100),
                                                new Item(ItemID::flamethrower, 1),
                                                new Item(ItemID::fireHose, 1)},
                         std::vector<int>{20, 100, 199});
    mainMenuShop.setFunction([](){
        if (shopSelectedItem != nullptr) {
            if (player.inventory.money >= mainMenuShop.itemPrices[shopSelectedItem->id]) {
                Item* boughtItem = new Item(*shopSelectedItem);
                boughtItem->amount = 1;
                player.addItem(boughtItem);
                player.inventory.money -= mainMenuShop.itemPrices[shopSelectedItem->id];
                shopSelectedItem->amount--;
                mainMenuShop.soldItems.removeItem(shopSelectedItem, false);
                shopNPCText.setString("Thank you for buying a " + stringLower(itemName[shopSelectedItem->id]) + "!");
                if (!mainMenuShop.soldItems.find(shopSelectedItem)) {
                    shopItemSprite.setTexture(Textures::INVISIBLE);
                    shopSelectedItem = nullptr;
                    createShopSlotsRects();
                }
            } else {
                shopNPCText.setString("Sorry, but you cannot afford a " + stringLower(itemName[shopSelectedItem->id]) + ".");
            }
        }
    });

    int xOffset = 50, yOffset = 50;

    shopBG.setTexture(Textures::SteelFrame);
    shopBG.setScale((float) scw / Textures::SteelFrame.getSize().x, (float) sch / Textures::SteelFrame.getSize().y);
    shopBGPattern.setPosition(0, 0);

    shopBGPattern.setTexture(Textures::ShopBG);
    shopBGPattern.setColor(sf::Color(0x10, 0xBB, 0xFF));
    shopBGPattern.setScale((float) (scw  - 2 * xOffset) / Textures::ShopBG.getSize().x, (float) (sch - 2 * yOffset) / Textures::ShopBG.getSize().y);
    shopBGPattern.setPosition(xOffset, yOffset);

    shopBackButton.setTexture(Textures::RedPanel, Textures::RedPanelPushed);
    shopBackButton.setCharacterSize(36);
    shopBackButton.setPosition(xOffset, yOffset);
    shopBackButton.setSize(250, yOffset);

    shopNPCTextFrame.setTexture(Textures::NPCDialogueFrame_Wide);
    shopNPCTextFrame.setColor(sf::Color(0x10, 0xBB, 0xFF));
    shopNPCTextFrame.setScale((float) (scw - 2 * xOffset) / Textures::NPCDialogueFrame_Wide.getSize().x,
                              200.0 / Textures::NPCDialogueFrame_Wide.getSize().y);
    shopNPCTextFrame.setPosition(xOffset, shopBackButton.getBottom() + yOffset / 5);

    shopNPCSprite.setTexture(Textures::DistortedScientist);
    shopNPCSprite.setScale((shopNPCTextFrame.getGlobalBounds().height - 2 * yOffset) / shopNPCSprite.getTexture()->getSize().x,
                           (shopNPCTextFrame.getGlobalBounds().height - 2 * yOffset) / shopNPCSprite.getTexture()->getSize().y);
    shopNPCSprite.setPosition(shopNPCTextFrame.getPosition() + sf::Vector2f(2 * xOffset, yOffset));

    shopNPCName.setCharacterSize(32);
    shopNPCName.setPosition(shopNPCSprite.getPosition().x + shopNPCSprite.getGlobalBounds().width + xOffset,
                            shopNPCSprite.getPosition().y + shopNPCSprite.getGlobalBounds().height / 2 - yOffset / 2);
    shopNPCName.setString(textWrap("Shop keeper", 20));

    shopNPCText.setCharacterSize(32);
    shopNPCText.setPosition(shopNPCTextFrame.getPosition().x + shopNPCTextFrame.getGlobalBounds().width * 0.25,
                            shopNPCTextFrame.getPosition().y + yOffset);

    shopItemsFrame.setTexture(Textures::ShopSectionFrame);
    shopItemsFrame.setColor(sf::Color(0xCC, 0xAA, 0x11));
    shopItemsFrame.setScale((0.6 * scw - 2 * xOffset) / (Textures::ShopSectionFrame.getSize().x),
                            (0.35 * sch - yOffset) / (Textures::ShopSectionFrame.getSize().y));
    shopItemsFrame.setPosition(xOffset, shopBackButton.getBottom() + 200.0 + yOffset / 2);

    shopItemsViewSizeX = (0.6 * scw - 2 * xOffset) / scw;
    shopItemsViewSizeY = (0.35 * sch - yOffset) / sch;
    ShopStockView.setViewport(sf::FloatRect((shopItemsFrame.getPosition().x + xOffset) / scw,
                                            (shopItemsFrame.getPosition().y + yOffset / 3) / sch,
                                            shopItemsViewSizeX, shopItemsViewSizeY));

    shopPlayerInventoryFrame.setTexture(Textures::ShopSectionFrame);
    shopPlayerInventoryFrame.setColor(sf::Color(0xBB, 0x40, 0x40));
    shopPlayerInventoryFrame.setScale((0.4 * scw - xOffset) / (Textures::ShopSectionFrame.getSize().x),
                                      (0.35 * sch - yOffset) / (Textures::ShopSectionFrame.getSize().y));
    shopPlayerInventoryFrame.setPosition(0.6 * scw, shopItemsFrame.getPosition().y);

    shopPlayerInvViewSizeX = (0.6 * scw - 2 * xOffset) / scw;
    shopPlayerInvViewSizeY = (0.35 * sch - yOffset) / sch;
    ShopPlayerInvView.setViewport(sf::FloatRect((shopPlayerInventoryFrame.getPosition().x + xOffset) / scw,
                                                (shopPlayerInventoryFrame.getPosition().y + yOffset) / sch,
                                                shopPlayerInvViewSizeX, shopPlayerInvViewSizeY));

    shopItemStatsFrame.setTexture(Textures::ShopSectionFrame);
    shopItemStatsFrame.setColor(sf::Color(0xCC, 0xAA, 0x11));
    shopItemStatsFrame.setScale((0.6 * scw - 2 * xOffset) / (Textures::ShopSectionFrame.getSize().x),
                                (0.35 * sch - yOffset) / (Textures::ShopSectionFrame.getSize().y));
    shopItemStatsFrame.setPosition(xOffset, shopItemsFrame.getPosition().y + 0.35 * sch - yOffset + yOffset / 2);

    shopItemSpriteFrame.setTexture(Textures::ShopItemPhotoFrame);
    shopItemSpriteFrame.setScale(0.5 * shopItemStatsFrame.getGlobalBounds().height / Textures::ShopItemPhotoFrame.getSize().x,
                                 0.5 * shopItemStatsFrame.getGlobalBounds().height / Textures::ShopItemPhotoFrame.getSize().y);
    shopItemSpriteFrame.setPosition(shopItemStatsFrame.getPosition().x + xOffset,
                                    shopItemStatsFrame.getPosition().y + yOffset * 0.3);
    shopItemSpriteFrame.setColor(sf::Color(0xAA, 0x88, 0x00));

    shopItemSprite.setTexture(Textures::INVISIBLE, true);
    shopItemSprite.setScale(2.5 * shopItemSpriteFrame.getGlobalBounds().width / Textures::INVISIBLE.getSize().x,
                            2.5 * shopItemSpriteFrame.getGlobalBounds().height / Textures::INVISIBLE.getSize().y);
    shopItemSprite.setPosition(shopItemSpriteFrame.getPosition().x,
                               shopItemSpriteFrame.getPosition().y);

    shopItemCoins.setCharacterSize(40);
    shopItemCoins.setPosition(shopItemSpriteFrame.getPosition().x + shopItemSpriteFrame.getGlobalBounds().width * 0.3,
                              shopItemSpriteFrame.getPosition().y + shopItemSpriteFrame.getGlobalBounds().height * 1.2);

    shopItemCoinsSprite = Animation(*itemTextureName[ItemID::coin], itemTextureFrameAmount[ItemID::coin],
                                    1, itemTextureDuration[ItemID::coin]);
    shopItemCoinsSprite.setPosition(shopItemSpriteFrame.getPosition().x + shopItemSpriteFrame.getGlobalBounds().width * 0.5,
                                    shopItemSpriteFrame.getPosition().y + shopItemSpriteFrame.getGlobalBounds().height);
    shopItemCoinsSprite.setSize(sf::Vector2f(100.0, 100.0));
    shopItemCoinsSprite.play();

    shopItemStats.setCharacterSize(30);
    shopItemStats.setPosition(shopItemSpriteFrame.getPosition().x + shopItemSpriteFrame.getGlobalBounds().width + 50,
                              shopItemSpriteFrame.getPosition().y + 25);

    shopBuyButton.setTexture(Textures::YellowPanel, Textures::YellowPanelPushed);
    shopBuyButton.setCharacterSize(70);
    shopBuyButton.setSize(400, 150);
    shopBuyButton.setPosition(shopPlayerInventoryFrame.getPosition().x + (0.4 * scw - xOffset) / 5,
                              sch - 150 - 1.5 * yOffset);
    
    shopPlayerCoinsText.setCharacterSize(40);
    shopPlayerCoinsText.setPosition(shopBuyButton.getPosition() - sf::Vector2f(0, 100));

    shopUIElements.push_back(&shopBG);
    shopUIElements.push_back(&shopBGPattern);
    shopUIElements.push_back(&shopBackButton);
    shopUIElements.push_back(&shopNPCTextFrame);
    shopUIElements.push_back(&shopNPCSprite);
    shopUIElements.push_back(&shopNPCName);
    shopUIElements.push_back(&shopNPCText);
    shopUIElements.push_back(&shopItemsFrame);
    shopUIElements.push_back(&shopPlayerInventoryFrame);
    shopUIElements.push_back(&shopItemStatsFrame);
    shopUIElements.push_back(&shopItemSpriteFrame);
    shopUIElements.push_back(&shopBuyButton);
    shopUIElements.push_back(&shopPlayerCoinsText);

    shopUIElements.push_back(&playerCoinSprite);
    shopUIElements.push_back(playerCoinSlot.background);  // It is invisible for now, but may be changed in the future for something visible
    shopUIElements.push_back(playerCoinSlot.amountText);
}
//==============================================================================================



//============================================================================================== DRAW FUNCTIONS
void draw() {
    updateShaders();
    if (isDrawInventory) {
        drawInventory();
    } else if (isDrawShop) {
        drawShop();
    } else {
        preRenderTexture.setView(InterfaceView);
        preRenderTexture.draw(undergroundBG, &Shaders::Distortion1);

        preRenderTexture.setView(GameView);

        drawFloor();
        drawWalls();

        for (sf::Drawable*& d: DrawableStuff) {
            preRenderTexture.draw(*d);
        }

        for (Enemy*& enemy: Enemies) {
            EnemyHealthBar.setPosition(enemy->getPosition() - sf::Vector2f(EnemyHealthBar.getSize().x / 2.f, enemy->getRadius() + 50.f));
            EnemyHealthBar.setValue(enemy->Health);
            preRenderTexture.draw(EnemyHealthBar);
        }

        for (int i = 0; i < Bullets.size(); i++) {
            preRenderTexture.draw(*Bullets[i]);
        }

        preRenderTexture.display();

        window.setView(InterfaceView);
        window.draw(preRenderSprite, &Shaders::Flashlight);

        for (size_t i = 0; i < TempTextsOnGround.size(); i++) {
            if (TempTextsOnGround[i]->localClock->getElapsedTime() < TempTextsOnGround[i]->howLongToExist) {
                window.draw(*TempTextsOnGround[i]);
            } else {
                DeletePointerFromVector(TempTextsOnGround, i--);
            }
        }

        for (size_t i = 0; i < DamageText.size(); i++) {
            if (DamageText[i]->localClock->getElapsedTime() < DamageText[i]->howLongToExist) {
                Shaders::FloatingUp.setUniform("uTime", DamageText[i]->localClock->getElapsedTime().asSeconds());
                window.draw(*DamageText[i], &Shaders::FloatingUp);
            } else {
                DeletePointerFromVector(DamageText, i--);
            }
        }

        if (IsDrawMinimap) {
            drawMiniMap();
        }
        if (IsDrawInterface) {
            drawInterface();
        }
    }
    window.display();
}

void drawFloor() {
    std::vector<sf::Texture>::iterator it = FloorTextureRects.begin();
    for (int i = 0; i < CurLocation->n; i++) {
        for (int j = 0; j < CurLocation->m; j++) {
            if (CurLocation->EnableTiles[i][j]) {
                FLoorTileSprite.setTexture(*it);
                FLoorTileSprite.setPosition(size * j, size * i);
                preRenderTexture.draw(FLoorTileSprite);
                it++;
            }
        }
    }
}

sf::Vector2f CameraPos;
CollisionRect CameraRect(0, 0, scw, sch);
void drawWalls() {
    CameraPos = GameView.getCenter() - GameView.getSize() / 2.f;
    CameraRect.setPosition(CameraPos);
    for (int i = std::max(0, 2 * int((CameraPos.y - WallMinSize / 2) / size) + 1);
            i <= std::min(int(CurLocation->walls.size() - 1), 2 * int((CameraPos.y + sch + WallMinSize / 2) / size) + 1); i++) {
        for (int j = std::max(0, int(CameraPos.x / size));
                j <= std::min(int(CurLocation->walls[i].size() - 1), int((CameraPos.x + scw + WallMinSize) / size)); j++) {
            if (CurLocation->walls[i][j]) {
                if (CameraRect.intersect(CurLocation->wallsRect[i][j])) {
                    CurLocation->SeenWalls[i][j] = true;
                }
                WallRect.setPosition(CurLocation->wallsRect[i][j].getPosition());
                WallRect.setTextureRect(sf::IntRect(
                        (Textures::Wall.getSize().x - CurLocation->wallsRect[i][j].getSize().x) * random(sf::Vector2f(i, j)),
                        (Textures::Wall.getSize().y - CurLocation->wallsRect[i][j].getSize().y) * random(sf::Vector2f(j, i)),
                        CurLocation->wallsRect[i][j].getSize().x,
                        CurLocation->wallsRect[i][j].getSize().y));
                preRenderTexture.draw(WallRect);
            }
        }
    }
}

void drawMiniMap() {
    if (MiniMapHoldOnPlayer) {
        if (!sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
            MiniMapView.setCenter(player.getCenter() * ScaleParam);
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

    MMPortalRect.setPosition(portal.getPosition() * ScaleParam);
    window.draw(MMPortalRect);

    for (Interactable*& i: listOfBox) {
        MMBoxRect.setPosition(i->getPosition() * ScaleParam);
        window.draw(MMBoxRect);
    }
    
    MMPuddleRect.setPosition(puddle.getPosition() * ScaleParam);
    window.draw(MMPuddleRect);

    for (Interactable*& i: listOfArtifact) {
        MMArtifact.setPosition(i->getPosition() * ScaleParam);
        window.draw(MMArtifact);
    }

    for (Enemy*& enemy: Enemies) {
        MMEnemyCircle.setPosition(enemy->getPosition() * ScaleParam);
        window.draw(MMEnemyCircle);
    }

    // draw players
    if (ClientFuncRun || HostFuncRun) {
        for (Player& p: ConnectedPlayers) {
            MMPlayerCircle.setPosition(p.getPosition() * ScaleParam);
            window.draw(MMPlayerCircle);
        }
    } else {
        MMPlayerCircle.setPosition(player.getCenter() * ScaleParam);
        window.draw(MMPlayerCircle);
    }
    window.setView(GameView);
}

sf::Clock ClockFPS; int FPSCounter;
PlacedText TextFPS;
void drawInterface() {
    window.setView(InterfaceView);
    for (sf::Drawable*& d: InterfaceStuff) {
        window.draw(*d);
    }

    if (player.CurWeapon != nullptr && player.CurWeapon->ManaStorage.toBottom() < player.CurWeapon->ManaCostOfBullet) {
        window.draw(ReloadWeaponText);
    }

    for (int i = 0; i < arsenal.size(); i++) {
        if (arsenal[i] == player.CurWeapon) {
            WeaponNameText.setFillColor(sf::Color::White);
            WeaponNameText.setOutlineThickness(3);
        } else {
            WeaponNameText.setFillColor(sf::Color(25, 192, 25, 160));
            WeaponNameText.setOutlineThickness(1);
        }
        AmmoBar.setValue(arsenal[i]->ManaStorage);
        AmmoBar.setPosition(20, sch - 20 - (arsenal.size() - i) * (AmmoBar.getSize().y + 10));

        if (arsenal[i]->holstered) {
            float holsterPercent = std::min(arsenal[i]->HolsterTimer->getElapsedTime().asSeconds() / arsenal[i]->TimeToHolster, 1.0f);
            sf::Color wallColor(255 - 155 * holsterPercent, 255 - 155 * holsterPercent, 255, 160);
            sf::Color foreColor(128 - 96 * holsterPercent, 128 - 96 * holsterPercent, 128, 160);
            sf::Color backColor(32 - 32 * holsterPercent, 32 - 32 * holsterPercent, 32 - 32 * holsterPercent, 160);
            AmmoBar.setColors(wallColor, foreColor, backColor);
        }
        else {
            float dispatchPercent = std::min(arsenal[i]->DispatchTimer->getElapsedTime().asSeconds() / arsenal[i]->TimeToDispatch, 1.0f);
            sf::Color wallColor(100 + 155 * dispatchPercent, 100 + 155 * dispatchPercent, 255, 160);
            sf::Color foreColor(32 + 96 * dispatchPercent, 32 + 96 * dispatchPercent, 128, 160);
            sf::Color backColor(32 * dispatchPercent, 32 * dispatchPercent, 32 * dispatchPercent, 160);
            AmmoBar.setColors(wallColor, foreColor, backColor);
        }

        WeaponNameText.setString(arsenal[i]->Name);
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

    for (size_t i = 0; i < TempTextsOnScreen.size(); i++) {
        if (TempTextsOnScreen[i]->localClock->getElapsedTime() < TempTextsOnScreen[i]->howLongToExist) {
            window.draw(*TempTextsOnScreen[i]);
        } else {
            DeletePointerFromVector(TempTextsOnScreen, i--);
        }
    }

    for (size_t i = 0; i < MessageText.size(); i++) {
        if (MessageText[i]->localClock->getElapsedTime() < MessageText[i]->howLongToExist) {
            Shaders::FloatingUp.setUniform("uTime", MessageText[i]->localClock->getElapsedTime().asSeconds());
            window.draw(*MessageText[i], &Shaders::FloatingUp);
        } else {
            DeletePointerFromVector(MessageText, i--);
        }
    }

    drawEffects();

    window.setView(GameView);
}

void drawEffects() {
    int count = 0;
    int xOffset = 175, yOffset = 175;
    std::vector<int> seenEffects(numberOfEffects, 0);
    std::vector<sf::Time> effectTimersTimes(numberOfEffects, sf::Time::Zero);
    for (Effect* eff : player.effects) {
        if (eff->type != Effects::Heal && eff->type != Effects::Damage && eff->active) {
            if (seenEffects[eff->type] == 0) {
                effectIcons[eff->type]->setScale(0.5, 0.5);
                effectIcons[eff->type]->setPosition(ManaBar.getPosition().x - 300 + xOffset * (count % 3),
                                ManaBar.getPosition().y + ManaBar.getSize().y + 20 + yOffset * (count / 3));

                effectIconsTimers[eff->type]->setPosition(effectIcons[eff->type]->getPosition() + sf::Vector2f(0, yOffset * 2 / 3));
                effectIconsTimers[eff->type]->setCharacterSize(28);
                window.draw(*effectIcons[eff->type]);

                count++;
            }
            seenEffects[eff->type] += 1;
            effectTimersTimes[eff->type] = std::max(effectTimersTimes[eff->type], eff->howLongToExist);
        }
    }
    for (int i = 0; i < effectTimersTimes.size(); i++) {
        if (seenEffects[i] > 0) {
            TempText* txt = effectIconsTimers[i];
            sf::Time timeDif = txt->howLongToExist - txt->localClock->getElapsedTime();
            if (txt->localClock->getElapsedTime() > txt->howLongToExist || effectTimersTimes[i] > timeDif) {
                txt->howLongToExist = effectTimersTimes[i];
                txt->localClock->restart();
            }
            txt->setString("x" + std::to_string(seenEffects[i]) + "\t\t\t\t" +
                           floatToString(timeDif.asSeconds()));
            window.draw(*txt);
        }
    }
}

void drawInventory() {
    window.setView(InventoryView);

    updateInventoryUI();

    for (sf::Drawable*& elem : invCommonElements)
        window.draw(*elem);
    switch (activeInventoryPage) {
        case inventoryPage::Items:
            for (sf::Drawable*& elem : invPageElements[activeInventoryPage])
                window.draw(*elem);

            for (Item*& item : player.inventory.items) {
                if (invItemSlotsElements[item->id].isInitialized) {
                    window.draw(*invItemSlotsElements[item->id].background);

                    if (item->amount >= 1) window.draw(*invItemSlotsElements[item->id].amountText);
                }
                window.draw(*item);
            }
            window.draw(*playerCoinSlot.amountText);

            if (isItemDescDrawn) {
                itemDescText.setPosition(sf::Mouse::getPosition(window).x + 100, sf::Mouse::getPosition(window).y);
                window.draw(itemDescText);
            }
            break;

        case inventoryPage::Stats:
            for (sf::Drawable*& elem : invPageElements[inventoryPage::Stats])
                window.draw(*elem);
            break;

        default:
            break;
    }

    window.setView(InterfaceView);
}

void drawShop() {
    window.setView(ShopView);

    updateShopUI();

    for (sf::Drawable*& elem : shopUIElements)
        window.draw(*elem);
    window.draw(shopItemSprite);
    if (shopSelectedItem != nullptr) {
        window.draw(shopItemCoins);
        window.draw(shopItemCoinsSprite);
        window.draw(shopItemStats);
    }

    window.draw(playerCoinSprite);
    window.draw(*playerCoinSlot.amountText);
    
    window.setView(ShopStockView);
    sf::Transform viewTransform = sf::Transform::Identity;
    viewTransform = viewTransform.scale(1, 1 / shopItemsViewSizeY);
    for (Item*& item : curShop->soldItems.items) {
        if (shopItemSlotsElements[item->id].isInitialized) {
            window.draw(*shopItemSlotsElements[item->id].background, viewTransform);
            if (item->amount >= 1) window.draw(*shopItemSlotsElements[item->id].amountText, viewTransform);
            window.draw(*shopItemSlotsElements[item->id].priceText, viewTransform);
        }
        window.draw(*item, viewTransform);
    }

    window.setView(ShopPlayerInvView);
    viewTransform = sf::Transform::Identity;
    viewTransform = viewTransform.scale(1, 1 / shopPlayerInvViewSizeY);
    for (Item*& item : player.inventory.items) {
        if (shopPlayerSlotsElements[item->id].isInitialized) {
            window.draw(*shopPlayerSlotsElements[item->id].background, viewTransform);
            if (item->amount >= 1) window.draw(*shopPlayerSlotsElements[item->id].amountText, viewTransform);
            window.draw(*shopPlayerSlotsElements[item->id].priceText, viewTransform);
        }
        window.draw(*item, viewTransform);
    }

    window.setView(InterfaceView);
}
//==============================================================================================



//============================================================================================== UI UPDATERS/CREATORS
void updateInventoryUI() {
    if (doInventoryUpdate[inventoryPage::Items]) {
        int slotNumber = 0;
        for (Item*& item : player.inventory.items) {
            Item* drawnItem = item;
            drawnItem->animation->setScale(0.75, 0.75);

            float itemX = (slotNumber % 6) * 200 + itemListBG.getPosition().x + 50;
            float itemY = (slotNumber / 6) * 200 + itemListBG.getPosition().y + 50;

            if (!invItemSlotsElements[drawnItem->id].isInitialized) {
                invItemSlotsElements[drawnItem->id].background = new sf::Sprite();
                invItemSlotsElements[drawnItem->id].amountText = new PlacedText();
                invItemSlotsElements[drawnItem->id].isInitialized = true;
            }
            sf::Sprite& slotBG = *invItemSlotsElements[drawnItem->id].background;
            slotBG.setPosition(itemX, itemY);
            slotBG.setScale(0.5, 0.5);
            slotBG.setTexture(Textures::ItemPanel);

            drawnItem->setPosition(itemX, itemY);

            PlacedText& itemAmountText = *invItemSlotsElements[drawnItem->id].amountText;
            itemAmountText.setCharacterSize(20);
            itemAmountText.setString(std::to_string(drawnItem->amount));
            itemAmountText.setPosition(sf::Vector2f(itemX + slotBG.getGlobalBounds().width,
                                                    itemY + slotBG.getGlobalBounds().height));

            slotNumber++;
        }

        playerCoinSprite.setScale(1, 1);
        playerCoinSprite.setPosition(invBackButton.getRight() + 100, invBackButton.getPosition().y - 20);
        playerCoinSlot.background->setTexture(Textures::INVISIBLE);
        playerCoinSlot.amountText->setString(std::to_string(player.inventory.money));
        playerCoinSlot.amountText->setCharacterSize(40);
        playerCoinSlot.amountText->setPosition(playerCoinSprite.getPosition().x + playerCoinSprite.getGlobalSize().x,
                                               playerCoinSprite.getPosition().y + playerCoinSprite.getGlobalSize().y / 2 - 20);

        doInventoryUpdate[inventoryPage::Items] = false;
    }
    statsHPRegenText.setString("Health regen: " + floatToString(player.HealthRecovery));
    statsMPRegenText.setString("Mana regen: " + floatToString(player.ManaRecovery));
    statsArmorText.setString("Armor: " + floatToString(player.Armor.cur));
    statsCompletedLevelsText.setString("Completed Levels: " + std::to_string(completedLevels));
    statsCurLevelsText.setString("Current Level: " + std::to_string(curLevel));
}

void updateShopUI() {
    int slotNumber = 0;
    for (Item*& drawnItem : curShop->soldItems.items) {
        drawnItem->animation->setScale(0.5, 0.5);

        float itemX = (slotNumber % 5) * 200 + 30;
        float itemY = (slotNumber / 5) * 200 + 20;

        if (!shopItemSlotsElements[drawnItem->id].isInitialized) {
            shopItemSlotsElements[drawnItem->id].background = new sf::Sprite();
            shopItemSlotsElements[drawnItem->id].amountText = new PlacedText();
            shopItemSlotsElements[drawnItem->id].priceText = new PlacedText();
            shopItemSlotsElements[drawnItem->id].isInitialized = true;
        }
        sf::Sprite& slotBG = *shopItemSlotsElements[drawnItem->id].background;
        slotBG.setPosition(itemX, itemY);
        slotBG.setTexture(Textures::ItemPanel, true);
        slotBG.setScale(0.333, 0.333);

        drawnItem->setPosition(itemX, itemY);

        PlacedText& itemAmountText = *shopItemSlotsElements[drawnItem->id].amountText;
        itemAmountText.setCharacterSize(20);
        itemAmountText.setString(std::to_string(drawnItem->amount));
        itemAmountText.setPosition(sf::Vector2f(itemX + slotBG.getGlobalBounds().width,
                                                itemY + slotBG.getGlobalBounds().height));

        PlacedText& itemPriceText = *shopItemSlotsElements[drawnItem->id].priceText;
        itemPriceText.setCharacterSize(20);
        itemPriceText.setString(std::to_string(curShop->itemPrices[drawnItem->id]) + " C.");
        itemPriceText.setPosition(sf::Vector2f(itemX - slotBG.getGlobalBounds().width / 10,
                                               itemY + slotBG.getGlobalBounds().height));

        slotNumber++;
    }

    slotNumber = 0;
    for (Item*& drawnItem : player.inventory.items) {
        drawnItem->animation->setScale(0.5, 0.5);

        float itemX = (slotNumber % 3) * 200 + 30;
        float itemY = (slotNumber / 3) * 200 + 20;
        if (!shopPlayerSlotsElements[drawnItem->id].isInitialized) {
            shopPlayerSlotsElements[drawnItem->id].background = new sf::Sprite();
            shopPlayerSlotsElements[drawnItem->id].amountText = new PlacedText();
            shopPlayerSlotsElements[drawnItem->id].priceText = new PlacedText();
            shopPlayerSlotsElements[drawnItem->id].isInitialized = true;
        }
        sf::Sprite& slotBG = *shopPlayerSlotsElements[drawnItem->id].background;
        slotBG.setPosition(itemX, itemY);
        slotBG.setTexture(Textures::ItemPanel, true);
        slotBG.setScale(0.333, 0.333);

        drawnItem->setPosition(itemX, itemY);

        PlacedText& playerAmountText = *shopPlayerSlotsElements[drawnItem->id].amountText;
        playerAmountText.setCharacterSize(20);
        playerAmountText.setString(std::to_string(drawnItem->amount));
        playerAmountText.setPosition(sf::Vector2f(itemX + slotBG.getGlobalBounds().width,
                                                  itemY + slotBG.getGlobalBounds().height));

        PlacedText& playerPriceText = *shopPlayerSlotsElements[drawnItem->id].priceText;
        playerPriceText.setCharacterSize(20);
        playerPriceText.setString(std::to_string(curShop->itemPrices[drawnItem->id]) + " C.");
        playerPriceText.setPosition(sf::Vector2f(itemX - slotBG.getGlobalBounds().width / 10,
                                                 itemY + slotBG.getGlobalBounds().height));

        slotNumber++;
    }
    playerCoinSprite.setScale(0.75, 0.75);
    playerCoinSprite.setPosition(shopPlayerCoinsText.getPosition().x + shopPlayerCoinsText.getGlobalBounds().width - 20,
                                 shopPlayerCoinsText.getPosition().y - playerCoinSprite.getGlobalSize().y / 3);
    playerCoinSlot.amountText->setString(std::to_string(player.inventory.money));
    playerCoinSlot.amountText->setCharacterSize(40);
    playerCoinSlot.amountText->setPosition(playerCoinSprite.getPosition().x + playerCoinSprite.getGlobalSize().x - 20,
                                           shopPlayerCoinsText.getPosition().y);
}

void createSlotRects() {
    for (int i = 0; i < invItemSlotsRects.size(); i++)
        DeletePointerFromVector(invItemSlotsRects, i--);
    invItemSlotsRects.resize(MaxItemID);

    int slotNumber = 0;
    for (Item*& item : player.inventory.items) {
        float itemX = (slotNumber % 6) * 200 + itemListBG.getPosition().x + 50;
        float itemY = (slotNumber / 6) * 200 + itemListBG.getPosition().y + 50;

        sf::FloatRect* itemActivationRect = new sf::FloatRect(itemX, itemY,
                                                              Textures::ItemPanel.getSize().x / 2,
                                                              Textures::ItemPanel.getSize().y / 2);
        invItemSlotsRects[item->id] = itemActivationRect;

        slotNumber++;
    }
}

void createShopSlotsRects() {
    for (int i = 0; i < shopItemSlotsRects.size(); i++)
        DeletePointerFromVector(shopItemSlotsRects, i--);
    shopItemSlotsRects.resize(MaxItemID);

    int slotNumber = 0;
    for (Item*& item : curShop->soldItems.items) {
        float itemX = ((slotNumber % 5) * 200 + 30);
        float itemY = ((slotNumber / 5) * 200 + 20) / shopItemsViewSizeY;

        sf::FloatRect* itemActivationRect = new sf::FloatRect(itemX, itemY,
                                                              Textures::ItemPanel.getSize().x * 0.4,
                                                              Textures::ItemPanel.getSize().y * 0.4 / shopItemsViewSizeY);
        shopItemSlotsRects[item->id] = itemActivationRect;

        slotNumber++;
    }
}
//==============================================================================================



//============================================================================================== EVENT HANDLERS
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
        } else if (isDrawInventory) {
            inventoryHandler(event);
        } else if (isDrawShop) {
            shopHandler(event);
        } else {
            if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::Escape) {
                    if (MiniMapActivated) {
                        MiniMapActivated = false;
                        MiniMapView.setViewport(sf::FloatRect(0.f, 0.f, 0.25f, 0.25f));
                        continue;
                    }
                }
                if (event.key.code == sf::Keyboard::M) {
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
                if (event.key.code == sf::Keyboard::Tab) {
                    isDrawInventory = true;
                    doInventoryUpdate[inventoryPage::Items] = true;
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

            if (CanSomethingBeActivated()) {
                if (!in(InterfaceStuff, static_cast<sf::Drawable*>(&XButtonSprite))) {
                    InterfaceStuff.push_back(&XButtonSprite);
                }
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
                        player.addItem(PickupStuff[i]);
                        doInventoryUpdate[inventoryPage::Items] = true;
                        DeleteFromVector(DrawableStuff, static_cast<sf::Drawable*>(PickupStuff[i]));
                        DeletePointerFromVector(PickupStuff, i--);
                    }
                }
            }

            if (event.type == sf::Event::MouseWheelScrolled) {
                if (!MiniMapActivated) {
                    CurWeapon -= (int)event.mouseWheelScroll.delta;
                    player.ChangeWeapon(arsenal[CurWeapon.cur]);

                    std::string reloadStr = player.CurWeapon->Name + " is out of ammo!";
                    ReloadWeaponText.setString(reloadStr);
                    ReloadWeaponText.setCenter(sf::Vector2f(scw / 2, sch / 4));
                }
            }

            if (player.CurWeapon != nullptr && !MiniMapActivated) {
                player.CurWeapon->Update(event);
            }

            if (CurLocation == &MainMenuLocation) {
                if (event.type == sf::Event::KeyPressed) {
                    if (event.key.code == sf::Keyboard::Escape) {
                        Musics::MainMenu.pause();
                        window.close();
                    }
                }
            } else if (CurLocation ==  &LabyrinthLocation) {
                if (event.type == sf::Event::KeyPressed) {
                    if (event.key.code == sf::Keyboard::Escape) {
                        EscapeMenuActivated = true;
                    } else if (event.key.code == sf::Keyboard::H) {
                        player.setCenter(size, size);
                        CurLocation = &WaitingRoomLoaction;
                    }
                }
            }

            // case screens::SetIP:
            //     if (event.type == sf::Event::TextEntered) {
            //         sf::Uint32 code = event.text.unicode;
            //         if (46 == code || (48 <= code && code <= 57)) {
            //             IPOfHost.push_back(code);
            //             IPPanel.setWord("IP:" + IPOfHost);
            //         }
            //     }
            //     if (event.type == sf::Event::KeyPressed) {
            //         if (event.key.code == sf::Keyboard::Enter) {
            //             IPPanel.setWord("Connecting...");
            //             draw();
            //             if (MySocket.connect(IPOfHost, 53000, sf::milliseconds(300)) == sf::Socket::Done) {
            //                 selector.add(MySocket);

            //                 if (selector.wait(sf::seconds(1)) && selector.isReady(MySocket) &&
            //                     MySocket.receive(ReceivePacket) == sf::Socket::Done) {
            //                     while (!ReceivePacket.endOfPacket()) {
            //                         ReceivePacket >> pacetStates::curState;
            //                         switch (pacetStates::curState) {
            //                             case pacetStates::PlayersAmount:
            //                                 ReceivePacket >> ComputerID;
            //                                 std::cout << "My ID = " << ComputerID << '\n';
            //                                 break;
            //                             case pacetStates::PlayerConnect:
            //                                 ReceivePacket >> PacetData;
            //                                 ListOfPlayers.addWord(PacetData);
            //                                 ConnectedPlayers.push_back(*(new Player()));
            //                                 std::cout << PacetData << " connected\n";
            //                                 break;
            //                             case pacetStates::SetPos:
            //                                 for (Player& x: ConnectedPlayers) {
            //                                     ReceivePacket >> x;
            //                                 }
            //                                 player.setCenter(ConnectedPlayers[ComputerID].getPosition());
            //                         }
            //                     }
            //                 }
            //                 ClientFuncRun = true;
            //                 ClientTread.launch();
            //             }
            //             IPPanel.setWord("IP:" + IPOfHost);
            //         } else if (event.key.code == sf::Keyboard::BackSpace && IPOfHost.size() > 0) {
            //             IPOfHost.pop_back();
            //             IPPanel.setWord("IP:" + IPOfHost);
            //         }
            //     }
            //     break;
        }
    }
}

void inventoryHandler(sf::Event& event) {
    if (event.key.code == sf::Keyboard::Escape) {
        isDrawInventory = false;
    }
    invBackButton.isActivated(event);
    itemsPageButton.isActivated(event);
    weaponsPageButton.isActivated(event);
    equipablesPageButton.isActivated(event);
    perksPageButton.isActivated(event);
    statsPageButton.isActivated(event);

    // if ((player.ManaRecovery != 0 && player.Mana.fromTop() != 0) ||
    //     (player.HealthRecovery != 0 && player.Health.fromTop() != 0)) {
    //     doInventoryUpdate[inventoryPage::Stats] = true;
    // }

    bool isAnythingHovered = false;
    if (activeInventoryPage != inventoryPage::Stats) {
        int itemTypeCount = 0;
        for (Item*& item : player.inventory.items) {
            if (!invItemSlotsRects.empty() && invItemSlotsRects[item->id] != nullptr &&
                invItemSlotsRects[item->id]->contains(sf::Vector2f(sf::Mouse::getPosition()))) {
                if (item->id != prevItemDescID) {
                    prevItemDescID = ItemID::NONE;
                    isItemDescDrawn = false;
                }
                isAnythingHovered = true;
                if (event.mouseButton.button == sf::Mouse::Button::Right) {
                    isItemDescDrawn = true;
                    itemDescText.setString(itemDesc[item->id]);
                    prevItemDescID = item->id;
                }
                if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Button::Left && useItem(item)) {
                    if (item->amount <= 0) {
                        itemTypeCount--;
                        isItemDescDrawn = false;
                        player.inventory.removeItem(item, false);
                    }
                    doInventoryUpdate[inventoryPage::Items] = true;
                }
            }
            itemTypeCount++;
        }
        if (itemTypeCount != prevItemTypeCount) createSlotRects();
        prevItemTypeCount = itemTypeCount;
    }
    if (!isAnythingHovered)
        isItemDescDrawn = false;
}

void shopHandler(sf::Event& event) {
    if (event.key.code == sf::Keyboard::Escape) {
        isDrawShop = false;
    }
    shopBackButton.isActivated(event);
    shopBuyButton.isActivated(event);

    if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Button::Left) {

        window.setView(ShopStockView);
        sf::Vector2f viewPos = window.mapPixelToCoords(sf::Mouse::getPosition());

        for (Item*& item : curShop->soldItems.items)
            if (!shopItemSlotsRects.empty() && shopItemSlotsRects[item->id] != nullptr &&
                shopItemSlotsRects[item->id]->contains(viewPos)) {

                shopSelectedItem = item;
                shopItemCoins.setString(sf::String(std::to_string(curShop->itemPrices[item->id])));
                shopItemStats.setString(textWrap(itemDesc[item->id], 65));
                shopItemSprite.setTexture(*itemTextureName[item->id], true);
            }
    }
    window.setView(ShopView);
}
//==============================================================================================



//============================================================================================== LEVEL GENERATION FUNCTIONS
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

void setBox(Interactable*& box) {
    box->setAnimation(Textures::Box, &Shaders::Flashlight);
    box->setSize(105.f, 117.f);
    box->setFunction([](Interactable* i){
        if (player.Mana.cur >= 20) {
            player.Mana -= 20.f;

            TempText* tempText = new TempText(sf::seconds(2.5f));
            tempText->setCharacterSize(50);
            tempText->setOutlineColor(sf::Color(120, 120, 120));
            tempText->setOutlineThickness(3);

            rand(); int r = 1 + rand() % 5;
            tempText->setString("Money + " + std::to_string(r));
            tempText->setFillColor(sf::Color(255, 170, 29));
            tempText->setCenter(scw / 2.f, sch / 2.f - 165.f);

            MessageText.push_back(tempText);
            player.inventory.money += r;
            doInventoryUpdate[inventoryPage::Items] = true;
            DeleteFromVector(listOfBox, i);
            DeleteFromVector(DrawableStuff, (sf::Drawable*)i);
            DeleteFromVector(InteractibeStuff, i);
            delete i;
        }
    });
}

std::vector<sf::String> artifactText = {
    "Health limit +2",
    "Mana limit +1",
    "Health Recovery +0.4",
    "Mana Recovery +0.2",
    "Reload Speed +1",
    "Cur Weapon Mana Storage +1"
};
std::vector<sf::Color> artifactColors = {
    sf::Color(250, 50, 50),
    sf::Color(50, 50, 250),
    sf::Color(250, 80, 80),
    sf::Color(80, 80, 250),
    sf::Color(90, 90, 90),
    sf::Color(160, 160, 160)
};
void setArtifact(Interactable*& artifact) {
    artifact->setAnimation(Textures::Architect, &Shaders::Architect);
    artifact->setSize(150.f, 150.f);
    artifact->setFunction([](Interactable* i){
        TempText* tempText = new TempText(sf::seconds(2.5f));
        tempText->setCharacterSize(50);
        tempText->setOutlineColor(sf::Color::White);
        tempText->setOutlineThickness(3);

        rand(); int r = rand() % 6;
        tempText->setString(artifactText[r]);
        tempText->setFillColor(artifactColors[r]);
        switch (r) {
            case 0: player.Health.top += 2; break;
            case 1: player.Mana.top += 1; break;
            case 2: player.HealthRecovery += 0.4; break;
            case 3: player.ManaRecovery += 0.1; break;
            case 4: player.CurWeapon->ReloadSpeed += 1; break;
            case 5: player.CurWeapon->ManaStorage.top += 1.f; break;
            default: break;
        }
        tempText->setCenter(scw / 2.f, sch / 2.f - 165.f);

        MessageText.push_back(tempText);
        DeleteFromVector(listOfArtifact, i);
        DeleteFromVector(DrawableStuff, (sf::Drawable*)i);
        DeleteFromVector(InteractibeStuff, i);
        delete i;
    });
}

void LevelGenerate(int n, int m) {
    MiniMapView.zoom(1 / MiniMapZoom);
    MiniMapZoom = std::pow(1.1, -10);
    MiniMapView.zoom(MiniMapZoom);

    LabyrinthLocation.GenerateLocation(n, m, player.getCenter() / float(size));

    FillFloorRectsThread.launch();

    clearVectorOfPointer(listOfBox);
    for (int i = 0; i < 10; i++) {
        listOfBox.push_back(new Interactable());
        setBox(listOfBox[i]);
        do {
            listOfBox[i]->setPosition(sf::Vector2f(rand() % m, rand() % n) * (float)size +
            sf::Vector2f(rand() % int(size - listOfBox[i]->getSize().x), rand() % int(size - listOfBox[i]->getSize().y)));
        } while (!LabyrinthLocation.EnableTiles[(int)listOfBox[i]->getPosition().y / size][(int)listOfBox[i]->getPosition().x / size]);

        InteractibeStuff.push_back(listOfBox[i]);
        DrawableStuff.push_back(listOfBox[i]);
    }

    clearVectorOfPointer(listOfArtifact);
    for (int i = 0; i < 10; i++) {
        listOfArtifact.push_back(new Interactable());
        setArtifact(listOfArtifact[i]);
        do {
            listOfArtifact[i]->setPosition(sf::Vector2f(rand() % m, rand() % n) * (float)size +
            sf::Vector2f(rand() % (size - Textures::Architect.getSize().x / 4), rand() % (size - Textures::Architect.getSize().y / 4)));
        } while (!LabyrinthLocation.EnableTiles[(int)listOfArtifact[i]->getPosition().y / size][(int)listOfArtifact[i]->getPosition().x / size]);

        InteractibeStuff.push_back(listOfArtifact[i]);
        DrawableStuff.push_back(listOfArtifact[i]);
    }

    clearVectorOfPointer(FireSet);
    // for (int i = 0; i < 1; i++) {
    //     FireSet.push_back(new Fire(sf::seconds(15.f)));
    //     FireSet[i]->setAnimation(Textures::Fire, 4, 1, sf::seconds(1), &Shaders::Flashlight);
    //     FireSet[i]->setSize(50.f, 50.f);
    //     // do {
    //         //FireSet[i]->setPosition(sf::Vector2f((rand() % m) + 0.5f, (rand() % n) + 0.5f) * (float)size);
    //     // } while (!LabyrinthLocation.EnableTiles[(int)FireSet[i]->PosY / size][(int)FireSet[i]->PosX / size]);
    //     if (CurLocation->room.x != -1 && CurLocation->room.y != -1) {
    //         FireSet[i]->setPosition(sf::Vector2f(CurLocation->room.x + 0.5f, CurLocation->room.y + 0.5f) * (float)size);
    //     }
    // }

    clearVectorOfPointer(Enemies);
    int amountOfEveryEnemiesOnLevel = curLevel > completedLevels ? 4 : 2;
    for (int i = 0; i < amountOfEveryEnemiesOnLevel; i++) {
        Enemies.push_back(new DistortedScientist());
        // Enemies.push_back(new ScottPilgrim());
        Enemies.push_back(new Distorted());
    }

    for (int i = 0; i < Enemies.size(); i++) {
        do {
            Enemies[i]->setPosition(sf::Vector2f((rand() % m) + 0.5f, (rand() % n) + 0.5f) * (float)size);
        } while (!LabyrinthLocation.EnableTiles[(int)Enemies[i]->getPosition().y / size][(int)Enemies[i]->getPosition().x / size] ||
                 distance(Enemies[i]->getPosition(), player.getCenter()) < size * 3);
    }
    FillFloorRectsThread.wait();
}

void LoadMainMenu() {
    CurLocation = &MainMenuLocation;

    player.setCenter(3.5f * size, 2.5f * size);
    FindAllWaysTo(CurLocation, player.getCenter(), TheWayToPlayer);
    player.ChangeWeapon(arsenal[CurWeapon.cur]);

    portal.setPosition(1612.5, 1545);
    puddle.setPosition(1012.5, 1545);

    sf::Vector2f PlayerPos = player.getCenter() / (float)size;
    CurLocation->FindEnableTilesFrom(PlayerPos);
    FillFloorRectsThread.launch();

    portal.setFunction([](Interactable* i){
        clearVectorOfPointer(PickupStuff);
        clearVectorOfPointer(Bullets);

        DrawableStuff.clear();
        InterfaceStuff.clear();
        InteractibeStuff.clear();

        player.setCenter(sf::Vector2f((START_M / 2 + 0.5f) * size, (START_N / 2 + 0.5f) * size));

        MiniMapActivated = false;
        EscapeMenuActivated = false;

        MiniMapView.setViewport(sf::FloatRect(0.f, 0.f, 0.25f, 0.25f));
        MiniMapView.setCenter(player.getCenter() * ScaleParam);

        Musics::MainMenu.pause();
        if (Musics::Fight1.getStatus() != sf::Music::Playing && Musics::Fight2.getStatus() != sf::Music::Playing) {
            Musics::Fight1.play();
        }
        if (CurLocation != &LabyrinthLocation) {
            CurLocation = &LabyrinthLocation;
        } else {
            completedLevels = std::max(curLevel, completedLevels);
            curLevel++;
        }
        LevelGenerate(START_N, START_M);
        FindAllWaysTo(CurLocation, player.getCenter(), TheWayToPlayer);

        DrawableStuff.push_back(&player);
        DrawableStuff.push_back(&portal);
        DrawableStuff.push_back(&puddle);
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
        saveGame();
    });

    puddle.setFunction([](Interactable* i){
        player.getDamage(5.f);
    });

    shopSector.setFunction([](Interactable* i){
        isDrawShop = true;
        shopPlayerCoinsText.setString("You have:");
        shopNPCText.setString(textWrap("Hello! Welcome to the \"We are literally standing near a phenomenon beyond our"
                                       " entire plane of existence in a literal motherfucking sense, that we have no fucking clue on"
                                       " how it fucking works why did we decide it was a good idea to station a shop exactly here\?!\?\?!\?!\?\" shop!", 94));
        createShopSlotsRects();
    });

    // Set cameras
    GameView.setCenter(player.getCenter());
    MiniMapView.setCenter(player.getCenter() * ScaleParam);
    InterfaceView.setCenter({scw / 2.f, sch / 2.f});

    Musics::Fight1.stop();
    Musics::Fight2.stop();
    Musics::MainMenu.play();

    DrawableStuff.clear();
    DrawableStuff.push_back(&player);
    DrawableStuff.push_back(&portal);
    DrawableStuff.push_back(&puddle);

    InterfaceStuff.clear();
    InterfaceStuff.push_back(&ManaBar);
    InterfaceStuff.push_back(&HpBar);
    InterfaceStuff.push_back(&chat);

    InteractibeStuff.clear();
    InteractibeStuff.push_back(&portal);
    InteractibeStuff.push_back(&puddle);
    InteractibeStuff.push_back(&shopSector);

    Item* newItem = new Item(ItemID::regenDrug, 1);
    newItem->setAnimation(*itemTextureName[ItemID::regenDrug], &Shaders::Flashlight);
    PickupStuff.push_back(newItem);
    DrawableStuff.push_back(PickupStuff[0]);
    PickupStuff[0]->dropTo(player.getCenter() + sf::Vector2f(100, 100));

    Item* fireHosePickup = new Item(ItemID::fireHose, 1);
    fireHosePickup->setAnimation(*itemTextureName[ItemID::fireHose], &Shaders::Flashlight);
    PickupStuff.push_back(fireHosePickup);
    DrawableStuff.push_back(PickupStuff[1]);
    PickupStuff[1]->dropTo(player.getCenter() + sf::Vector2f(400, 300));

    Item* flamethrowerPickup = new Item(ItemID::flamethrower, 1);
    flamethrowerPickup->setAnimation(*itemTextureName[ItemID::flamethrower], &Shaders::Flashlight);
    PickupStuff.push_back(flamethrowerPickup);
    DrawableStuff.push_back(PickupStuff[2]);
    PickupStuff[2]->dropTo(player.getCenter() + sf::Vector2f(-550, -120));

    curShop = &mainMenuShop;

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
//==============================================================================================



//============================================================================================== GAME STATE FUNCTIONS
void updateBullets() {
    for (int i = 0; i < Bullets.size(); i++) {
        if (Bullets[i]->penetration < 0 || Bullets[i]->todel) {
            if (Bullets[i]->type == Bullet::FireParticle) {
                Fire* fire = new Fire(sf::seconds(10));
                fire->setAnimation(Textures::Fire, 4, 1, sf::seconds(1), &Shaders::Flashlight);
                fire->setSize(50.f, 50.f);
                fire->setPosition(Bullets[i]->getPosition());
                FireSet.push_back(fire);
                DrawableStuff.push_back(fire);
            }
            DeletePointerFromVector(Bullets, i--);
        } else {
            Bullets[i]->move(CurLocation);
            if (!faction::friends(Bullets[i]->fromWho, player.faction) && player.intersect(*Bullets[i])) {
                player.getDamage(Bullets[i]->damage);
                Bullets[i]->penetration--;
            }
            if (Bullets[i]->type == Bullet::WaterParticle) {
                for (int j = 0; j < FireSet.size(); j++) {
                    if (FireSet[j]->intersect(*Bullets[i])) {
                        DeleteFromVector(DrawableStuff, static_cast<sf::Drawable*>(FireSet[j]));
                        DeletePointerFromVector(FireSet, j--);
                        Bullets[i]->todel = true;
                        break;
                    }
                }
            } else {
                for (Enemy*& enemy: Enemies) {
                    if (!faction::friends(Bullets[i]->fromWho, enemy->faction) && enemy->intersect(*Bullets[i])) {
                        enemy->getDamage(Bullets[i]->damage);
                        Bullets[i]->penetration--;
                        TempText* tempText = new TempText(sf::seconds(1.5f));
                        tempText->setCharacterSize(30);
                        tempText->setOutlineColor(sf::Color::White);
                        tempText->setOutlineThickness(3);
                        tempText->setString(std::to_string(int(Bullets[i]->damage)));
                        tempText->setFillColor(sf::Color(250, 50, 50, 200));
                        tempText->setCenter(enemy->getPosition());
                        DamageText.push_back(tempText);
                        break;
                    }
                }
            }
        }
    }
}

void processEffects() {
    updateEffects(&player);
    for (Enemy*& enemy : Enemies)
        updateEffects(enemy);
}

void updateEffects(Creature* creature) {
    std::vector<Effect*>& effectVec = creature->effects;
    for (int i = 0; i < effectVec.size(); i++) {
        if (effectVec[i]->howLongToExist <= sf::Time::Zero) {
            clearEffect(*creature, effectVec[i]);
            DeletePointerFromVector(effectVec, i--);
        } else {
            float t = std::min(effectVec[i]->localClock->restart().asSeconds(), effectVec[i]->howLongToExist.asSeconds());
            effectVec[i]->howLongToExist -= sf::seconds(t);
            switch (effectVec[i]->type) {
                case Effects::Damage:
                    creature->getDamage(effectVec[i]->parameters[0] * t);
                    break;
                case Effects::Heal:
                    creature->getDamage(-effectVec[i]->parameters[0] * t);
                    break;
                case Effects::HPRegen:
                    if (!effectVec[i]->active) {
                        creature->HealthRecovery += effectVec[i]->parameters[0];
                        effectVec[i]->active = true;
                    }
                    break;
                case Effects::Burn:
                    if (!effectVec[i]->active) {
                        effectVec[i]->parameters[1] += 1;
                        creature->getDamage(creature->HealthRecovery +
                                            effectVec[i]->parameters[0] * effectVec[i]->parameters[1]);
                        effectVec[i]->active = true;
                    }
                    if (effectVec[i]->customTickClock->getElapsedTime().asSeconds() >= effectVec[i]->customTick.asSeconds()) {
                        creature->getDamage(creature->HealthRecovery +
                                            effectVec[i]->parameters[0] * effectVec[i]->parameters[1]);
                        effectVec[i]->parameters[1] += 1;
                        effectVec[i]->customTickClock->restart();
                    }
                    break;
                default:
                    break;
            }
        }
    }
}

void applyEffect(Creature& owner, Effect* effect) {
    switch (effect->type) {
        case Effects::Burn:
            if (owner.effectStacks[effect->type] < 1) {
                owner.effects.push_back(effect);
                owner.effectStacks[effect->type] = 1;
            }
            break;

        case Effects::HPRegen:
            owner.effects.push_back(effect);
            owner.effectStacks[effect->type] += 1;
            break;

        case Effects::Heal:
            owner.effects.push_back(effect);
            break;

        case Effects::Damage:
            owner.effects.push_back(effect);
            break;

        default:
            owner.effects.push_back(effect);
            owner.effectStacks[effect->type] += 1;
            break;
    }
}

void clearEffect(Creature& owner, Effect* effect) {
    switch (effect->type) {
        case Effects::HPRegen:
            owner.HealthRecovery -= effect->parameters[0];
            break;
        default:
            owner.effectStacks[effect->type] -= 1;
            break;
    }
}

bool useItem(Item*& item) {
    if (item->amount <= 0)
        return false;
    item->amount--;
    switch (item->id) {
        case ItemID::regenDrug:
            applyEffect(player, new Effect(Effects::HPRegen, std::vector<float>{1.0f}, sf::seconds(10.f)));
            return true;
        case ItemID::fireHose:
            fireHose.ManaStorage = 100;
            arsenal.push_back(&fireHose);
            CurWeapon.top += 1;
            return true;
        case ItemID::flamethrower:
            flamethrower.ManaStorage = 100;
            arsenal.push_back(&flamethrower);
            CurWeapon.top +=1;
            return true;
        default:
            return false;
    }
}

bool firePropagationAllowed(sf::Vector2i& ancestor, sf::Vector2i pos) {
    if (pos.y < 0 || pos.x < 0) return false; // если огонь пытается пересечь самые левые или самые верхние стены
    ancestor /= size;
    pos /= size;
    if (ancestor.y == pos.y && ancestor.x == pos.x) return true; // одинаковые тайлы
    // проверка на стену между тайлами
    return ancestor.y == pos.y && !CurLocation->walls[pos.y * 2 + 1][std::max(ancestor.x, pos.x)] || // соседние тайлы по горизонтали
           ancestor.x == pos.x && !CurLocation->walls[std::max(ancestor.y, pos.y) * 2][pos.x];       // соседние тайлы по вертикали
}

void fireUpdate() {
    float angle;
    for (int i = 0; i < FireSet.size(); i++) {
        float dist = 2.5 * FireSet[i]->getSize().y;
        if (FireSet[i]->localClock->getElapsedTime() >= FireSet[i]->howLongToExist) {
            FireSet[i]->localClock->restart();
            FireSet[i]->howLongToExist = sf::seconds(rand() % 4 + 15.f);

            Fire* descendant1 = new Fire(sf::seconds(rand() % 4 + 15.f));
            descendant1->setAnimation(Textures::Fire, 4, 1, sf::seconds(1), &Shaders::Flashlight);
            descendant1->setSize(50.f, 50.f);
            sf::Vector2i posFather = sf::Vector2i(FireSet[i]->getCenter());
            sf::Vector2f posSon = descendant1->getCenter();
            do {
                angle = (rand() % 360) * M_PI / 180;
                posSon = sf::Vector2f(posFather) + dist * sf::Vector2f(std::sin(angle), std::cos(angle));
            } while (!CurLocation->EnableTiles[posSon.y / size][posSon.x / size] ||
                     !firePropagationAllowed(posFather, sf::Vector2i(posSon)));
            descendant1->setCenter(posSon);
            FireSet.push_back(descendant1);
            DrawableStuff.push_back(descendant1);
        }
    }
}
//==============================================================================================



//============================================================================================== GAME GRAPHICS FUNCTIONS
void updateShaders() {
    sf::Vector2f uMouse(sf::Mouse::getPosition());
    float uTime = GameClock->getElapsedTime().asSeconds();
    sf::Vector2f uPlayerPosition(window.mapCoordsToPixel(player.getCenter()));

    Shaders::Flashlight.setUniform("uMouse", uMouse);
    Shaders::Flashlight.setUniform("uPlayerPosition", uPlayerPosition);

    Shaders::Portal.setUniform("uTime", uTime);

    Shaders::Architect.setUniform("uTime", uTime);

    Shaders::Distortion1.setUniform("uTime", uTime);
    Shaders::Distortion2.setUniform("uTime", uTime);
}
//==============================================================================================



//============================================================================================== HELPER FUNCTIONS
bool CanSomethingBeActivated() {
    for (Interactable*& x: InteractibeStuff) {
        if (x->CanBeActivated(player)) {
            return true;
        }
    }
    return false;
}

void saveGame() {
    std::ofstream fileToSave("save.save");
    fileToSave << player.Name.getString().toAnsiString() << '\n';
    fileToSave << player.Health << '\n';
    fileToSave << player.HealthRecovery << '\n';
    fileToSave << player.Mana << '\n';
    fileToSave << player.ManaRecovery << '\n';
    fileToSave << player.inventory.money << '\n';
    fileToSave << completedLevels;

    fileToSave.close();
}

void loadSaves() {
    std::ifstream fileToSave("save.save");
    if (!fileToSave.is_open()) {
        std::rand();
        player.Name.setString("Employee " + std::to_string(1 + (size_t(std::rand()) * 8645) % 999));
    } else {
        char name[13]; fileToSave.getline(name, 13);
        player.Name.setString(name);
        fileToSave >> player.Health;
        fileToSave >> player.HealthRecovery;
        fileToSave >> player.Mana;
        fileToSave >> player.ManaRecovery;
        fileToSave >> player.inventory.money;
        fileToSave >> completedLevels;
    }
    fileToSave.close();
}
//==============================================================================================



//============================================================================================== MAIN LOOP
void MainLoop() {
    while (window.isOpen()) {

        if (player.Health.toBottom() == 0) {
            EscapeButton.buttonFunction();
        }

        for (int i = 0; i < Enemies.size(); i++) {
            if (Enemies[i]->Health.toBottom() == 0) {
                if (Enemies[i]->dropInventory) {
                    for (Item*& item : Enemies[i]->inventory.items) {
                        item->dropTo(Enemies[i]->getPosition());

                        PickupStuff.push_back(item);
                        DrawableStuff.push_back(item);
                    }
                }
                Enemies[i]->inventory.items.clear();

                DeleteFromVector(DrawableStuff, static_cast<sf::Drawable*>(Enemies[i]));
                DeletePointerFromVector(Enemies, i--);

                if (Enemies.size() == 0) {
                    TempText* enemiesKilledText = new TempText(sf::seconds(10));
                    enemiesKilledText->setCharacterSize(40);
                    enemiesKilledText->setString("      All enemies cleared!\nPortal to the next area has now opened.");
                    enemiesKilledText->setCenter(scw / 2.0f, sch / 4.0f);
                    TempTextsOnScreen.push_back(enemiesKilledText);

                    if (CurLocation == &LabyrinthLocation) {
                        if (!in(InteractibeStuff, (Interactable*)&portal)) {
                            InteractibeStuff.push_back(&portal);
                        }
                    }
                }
            } else {
                Enemies[i]->setTarget(player.getCenter());
                Enemies[i]->move(CurLocation);
                Enemies[i]->UpdateState();
                Enemies[i]->CurWeapon->lock = false;
                Enemies[i]->CurWeapon->Shoot(*Enemies[i], player.getCenter(), Enemies[i]->faction);
                Enemies[i]->CurWeapon->Reload(Enemies[i]->Mana);
            }
        }

        fireUpdate();

        player.UpdateState();
    
        if (CurLocation == &LabyrinthLocation) {
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
            if (!chat.inputted && !isDrawInventory && !isDrawShop) {
                player.move(CurLocation);
                GameView.setCenter(player.getCenter() + static_cast<sf::Vector2f>((sf::Mouse::getPosition() - sf::Vector2i(scw, sch) / 2) / 8));
                FindAllWaysTo(CurLocation, player.getCenter(), TheWayToPlayer);
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::R)) {
                    player.CurWeapon->HolsterAction();
                }
            }
            int wasBulletsSize = Bullets.size();

            if (player.CurWeapon != nullptr) {
                player.CurWeapon->Shoot(player, window.mapPixelToCoords(sf::Mouse::getPosition()), player.faction);
                if (player.CurWeapon == &fireHose && player.CurWeapon->ManaStorage.toBottom() < player.CurWeapon->ManaCostOfBullet) {
                    DeleteFromVector(arsenal, (Weapon*)&fireHose);
                    CurWeapon -= 1;
                    CurWeapon.top -= 1;
                    player.ChangeWeapon(arsenal[CurWeapon.cur]);
                }
                if (player.CurWeapon == &flamethrower && player.CurWeapon->ManaStorage.toBottom() < player.CurWeapon->ManaCostOfBullet) {
                    DeleteFromVector(arsenal, (Weapon*)&flamethrower);
                    CurWeapon -= 1;
                    CurWeapon.top -= 1;
                    player.ChangeWeapon(arsenal[CurWeapon.cur]);
                }
            }
            for (Weapon*& weapon : arsenal)
                if (weapon->holstered) weapon->Reload(player.Mana);

            if (wasBulletsSize < Bullets.size() && (HostFuncRun || ClientFuncRun)) {
                mutex.lock();
                SendPacket << pacetStates::Shooting << (int)Bullets.size() - wasBulletsSize;
                for (; wasBulletsSize < Bullets.size(); wasBulletsSize++) {
                    SendPacket << *Bullets[wasBulletsSize];
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
                ConnectedPlayers[ComputerID].setPosition(player.getCenter());
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
        }

        draw();

        if (puddle.intersect(player))
            applyEffect(player, new Effect(Effects::Heal, std::vector<float>{30.f}, sf::seconds(1.5f)));

        for (int i = 0; i < FireSet.size(); i++) {
            if (FireSet[i]->intersect(player))
                applyEffect(player, new Effect(Effects::Burn, std::vector<float>{5.f}, sf::seconds(5.f), sf::seconds(1.f)));
        }
        processEffects();

        EventHandler();
    }
    saveGame();
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
            SendPacket << *Bullets[i];
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
    DeletePointerFromVector(clients, i);
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
                                        tempBullet = new Bullet();
                                        ReceivePacket >> *tempBullet;
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
                            player.setCenter(ConnectedPlayers[ComputerID].getPosition());
                            GameView.setCenter(player.getCenter());
                            break;
                        case pacetStates::ChatEvent:
                            ReceivePacket >> PacetData;
                            chat.addLine(PacetData);
                            break;
                        case pacetStates::Shooting: {
                            int i; ReceivePacket >> i;
                            for (; i > 0; i--) {
                                tempBullet = new Bullet();
                                ReceivePacket >> *tempBullet;
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
