#pragma once
#include "Entities/enemy.h"
#include "Entities/player.h"
#include "Multiplayer/chat.h"
#include "Multiplayer/client.h"
#include "UI/RectButton.h"
#include "UI/PolygonButton.h"
#include "UI/panel.h"
#include "UI/bar.h"
#include "UI/tempText.h"
#include "UI/Slot.h"
#include "UI/Frame.h"
#include "Systems/effect.h"
#include "Systems/shop.h"


//////////////////////////////////////////////////////////// Settings of the game
bool IsDrawMinimap = true;
bool IsdrawHUD = true;
bool MiniMapHoldOnPlayer = true;


//////////////////////////////////////////////////////////// Stuff for work with system and screen
sf::ContextSettings settings;


sf::RenderWindow window(sf::VideoMode(scw, sch), "multigame", sf::Style::Fullscreen, settings);
sf::RenderTexture preRenderTexture, outlineRenderTexture;
sf::Sprite preRenderSprite, outlineRenderSprite;
float MiniMapZoom = 1.f;
bool MiniMapActivated, EscapeMenuActivated, isDrawInventory, isDrawShop, isDrawUpgradeInterface;
std::vector<sf::Drawable*> DrawableStuff, InterfaceStuff; // references to objects that exist somewhere
std::vector<Interactable*> InteractibeStuff; // references to objects that exist somewhere
Interactable* CurInteractable;

std::vector<Item*> PickupStuff;


//////////////////////////////////////////////////////////// Players
Player player;
std::vector<Player> ConnectedPlayers;

//////////////////////////////////////////////////////////// Weapons
Pistol pistol;
Shotgun shotgun;
// Revolver revolver;
Rifle rifle;
// Bubblegun bubblegun;
// Armageddon armageddon;
// Chaotic chaotic;
std::vector<Weapon*> arsenal = {
    &pistol,
    &shotgun,
    &rifle,
};
Scale<int> CurWeapon;

//////////////////////////////////////////////////////////// DrawableStuff
sf::Sprite WallRect, FLoorTileSprite;
std::vector<TempText*> TempTextsOnScreen, TempTextsOnGround, DamageText, MessageText;
Bar<float> EnemyHealthBar;
sf::Sprite undergroundBG;


//////////////////////////////////////////////////////////// MiniMapStuff
sf::CircleShape MMPlayerCircle, MMEnemyCircle; // MM - MiniMap prefix
sf::RectangleShape MMPortalRect, MMBoxRect, MMPuddleRect, MMArtifact;


//////////////////////////////////////////////////////////// HUDStuff
Frame HUDFrame("HUD", 0, 0, scw, sch);
Bar<float> HPBar("HPBar", UI::TR, UI::TR, 360, 50), MPBar("MPBar", UI::BL, UI::TL, 240, 50);
std::vector<Bar<float>*> AmmoBars;
std::vector<PlacedText*> WeaponNameTexts;
PlacedText ReloadWeaponText;
sf::Sprite XButtonSprite;

std::vector<sf::Sprite*> effectIcons(Effects::EffectCount);
std::vector<TempText*> effectIconsTimers(Effects::EffectCount);


//////////////////////////////////////////////////////////// InventoryStuff
namespace inventoryInterface {
    std::vector<bool> doInventoryUpdate(inventoryPage::PageCount);
    inventoryPage::Type activePage = inventoryPage::Items;
    std::vector<sf::Drawable*> commonElements; // These elements appear on every page
    std::vector<std::vector<sf::Drawable*>> pageElements; // These elements only appear on certain pages
    std::vector<sf::Drawable*> compUpgElements;

    Frame inventoryFrame("InventoryFrame", 0, 0, scw, sch);
    Frame itemsFrame("PageFrame_items", 0, 0, scw, sch);
    Frame weaponsFrame("PageFrame_weapons", 0, 0, scw, sch);
    Frame statsFrame("PageFrame_stats", 0, 0, scw, sch);

    RectButton backButton("Back", []() {
        isDrawInventory = false;
        inventoryFrame.hide();
    });
    RectButton itemsButton("Items", []() {
        activePage = inventoryPage::Items;
        for (UIElement*& elem : inventoryFrame.getChildren()) {
            if (elem->getName().substr(0, 9) == "PageFrame")
                elem->hide();
        }
        itemsFrame.show();
    });
    RectButton weaponsButton("Weapons", []() {});
    RectButton equipablesButton("Equipables", []() {
        activePage = inventoryPage::Equipables;
        for (UIElement*& elem : inventoryFrame.getChildren()) {
            if (elem->getName().substr(0, 9) == "PageFrame")
                elem->hide();
        }
    });
    RectButton perksButton("Perks", []() {
        activePage = inventoryPage::Perks;
        for (UIElement*& elem : inventoryFrame.getChildren()) {
            if (elem->getName().substr(0, 9) == "PageFrame")
                elem->hide();
        }
    });
    RectButton statsButton("Stats", []() {
        activePage = inventoryPage::Stats;
        for (UIElement*& elem : inventoryFrame.getChildren()) {
            if (elem->getName().substr(0, 9) == "PageFrame")
                elem->hide();
        }
        statsFrame.show();
    });

    // ITEM INVENTORY ELEMENTS
    int prevItemTypeCount = 0;
    bool isItemDescDrawn = false;

    sf::Sprite itemListBG;

    Animation coinSprite;
    ItemSlot coinSlot;        // Special slot, so that it can be conveniently used other than in the inventory

    std::vector<ItemSlot> itemSlotsElements; // Elements that comprise an inventory slot - the background texture and the amount text
    std::vector<sf::FloatRect*> itemSlotsRects; // The slot itself. This is what activates when a player clicks on an item.

    sf::Sprite statsPlayerImage;
    Bar<float> statsHPBar("statsHPBar", 0, 0, scw / 10.f, sch / 20);
    Bar<float> statsMPBar("statsMPBar", 0, 0, scw / 10.f, sch / 20);

    PlacedText statsArmorText,
        statsHPText,
        statsMPText,
        statsHPRegenText,
        statsMPRegenText,
        statsCompletedLevelsText,
        statsCurLevelsText;

    PlacedText itemDescText;
    ItemID::Type prevItemDescID;
}

// UPGRADE SHOP ELEMENTS
namespace upgradeInterface {
    std::vector<sf::Drawable*> UIElements;

    sf::Sprite BG;
    sf::Sprite weaponImg;
    sf::Sprite weaponDescFrame;

    RectButton backButton("Back", []() {
        isDrawUpgradeInterface = false;
        });
    PolygonButton switchGunLBtn;
    PolygonButton switchGunRBtn;

    PlacedText weaponDescText;

    PolygonButton generatorBtn,
                  formFactorBtn,
                  converterBtn,
                  targetingBtn;

    // UPGRADE SHOP COMPONENT CHOICE ELEMENTS
    std::vector<sf::Drawable*> choiceUIElements;
    Panel listTextPanel, listFade, listBG;
    Panel imgPanel;
    std::vector<std::vector<RectButton*>> compUpgBtns;
    std::vector<std::vector<PlacedText*>> compUpgCosts;
    std::vector<std::vector<PlacedText*>> compUpgStats;
    std::vector<std::vector<PlacedText*>> compUpgCount;
    PlacedText playerCoinAmount;

    bool isChoosingComponent = false;
    int compType = 0;

}


//////////////////////////////////////////////////////////// Shop Interface
Shop mainMenuShop;
Shop* curShop = nullptr;
Item* shopSelectedItem = nullptr;

RectButton shopBackButton("Back", []() {
    isDrawShop = false;
    shopSelectedItem = nullptr;
    });
RectButton shopBuyButton("Buy", []() {
    curShop->buyFunction();
    });

namespace MenuShop {
    std::vector<sf::Drawable*> UIElements;
    std::vector<ShopSlot> slotsElements; // Analogous to the inventory itemSlotsElements + price
    std::vector<ShopSlot> playerSlotsElements; // Analogous to the inventory itemSlotsElements + price
    std::vector<sf::FloatRect*> slotsRects; // Analogous to the inventory itemSlotsRects

    sf::Sprite BG;
    sf::Sprite NPCTextFrame;
    sf::Sprite itemsFrame, playerInvFrame, itemStatsFrame;
    sf::Sprite itemSpriteFrame, itemSprite;

    sf::Sprite NPCSprite;
    Animation itemCoinsSprite;

    float itemsViewSizeX, itemsViewSizeY;
    float playerInvViewSizeX, playerInvViewSizeY;

    PlacedText NPCName, NPCText, itemCoins, itemStats, playerCoinsText;
}

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
shopSector,
upgradeSector;
std::vector<Interactable*> listOfBox,
listOfArtifact,
listOfFire;


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




//////////////////////////////////////////////////////////// Enemies
std::vector<Enemy*> Enemies;


//////////////////////////////////////////////////////////// functions

//---------------------------- INITS
void init();
void initInventory();
void initShops();
void initInterface();
//----------------------------


//---------------------------- DRAW FUNCTIONS
void draw();
void drawFloor();
void drawWalls();
void drawMiniMap();
void drawHUD();
void drawEffects();
void drawInventory();               // Temporary solution. Any interface demands it's own function. Not very pretty (code duplication). Better to make a class
void drawShop();                    // Temporary solution. Any interface demands it's own function. Not very pretty (code duplication). Better to make a class
void drawUpgradeInterface();        // Temporary solution. Any interface demands it's own function. Not very pretty (code duplication). Better to make a class
//----------------------------


//---------------------------- UI UPDATERS/CREATORS
void updateInventoryUI();
void createSlotRects();

void updateShopUI();                // Temporary solution. Any interface demands it's own function. Not very pretty (code duplication). Better to make a class
void createShopSlotsRects();        // Temporary solution. Any interface demands it's own function. Not very pretty (code duplication). Better to make a class

void updateUpgradeInterfaceUI();    // Temporary solution. Any interface demands it's own function. Not very pretty (code duplication). Better to make a class
//----------------------------


//---------------------------- EVENT HANDLERS
void EventHandler();
void inventoryHandler(sf::Event&);
void shopHandler(sf::Event&);
void upgradeInterfaceHandler(sf::Event&);
//----------------------------


//---------------------------- LEVEL GENERATION FUNCTIONS
void LevelGenerate(int, int);
void LoadMainMenu();

void setBox(Interactable*&);
void setArtifact(Interactable*&);
void setFire(Interactable*&);
//----------------------------


//---------------------------- GAME STATE FUNCTIONS
void updateBullets();
void updateEnemies();
void updateUpgradeShopStats();
void openUpgradeShop();

bool useItem(Item*&);

void processEffects();
void updateEffects(Creature*);
void applyEffect(Creature&, Effect*);
void clearEffect(Creature&, Effect*);
//----------------------------


//---------------------------- GAME GRAPHICS FUNCTIONS
void updateShaders();
//----------------------------


//---------------------------- HELPER FUNCTIONS
bool CanSomethingBeActivated();
template <class T> void updateCostsText(PlacedText*, Upgradable<T>*, int);
template <class T> void updateCountText(PlacedText* counterText, Upgradable<T>* stat);
template <class T> void updateStatsText(PlacedText* statText, Upgradable<T>* stat);
template <class T> void upgradeStat(int, Upgradable<T>*,
                                    PlacedText* costText = nullptr,
                                    PlacedText* = nullptr,
                                    PlacedText* = nullptr);

void saveGame();
void loadSave();
//----------------------------


void MainLoop(); // SELF-EXPLANATORY


//////////////////////////////////////////////////////////// Server-Client functions
void ClientConnect();
void ClientDisconnect(int);
void SelfDisconnect();
void SendToClients(sf::Packet&, int = -1);
void funcOfHost();
void funcOfClient();


//////////////////////////////////////////////////////////// Threads
sf::Thread HostTread(funcOfHost);
sf::Thread ClientTread(funcOfClient);

//////////////////////////////////////////////////////////// Panels
Panel IPPanel("IP:");
Panel ListOfPlayers;


//////////////////////////////////////////////////////////// Buttons
RectButton HostButton("Host", []() {
    listener.listen(53000);
    selector.add(listener);
    ListOfPlayers.setString(MyIP);
    ComputerID = 0;
    ConnectedPlayers.push_back(*(new Player()));
    HostFuncRun = true;
    HostTread.launch();
    });

RectButton EscapeButton("Exit", []() {
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
    }
    else if (ClientFuncRun) {
        SelfDisconnect();
    }
    EscapeMenuActivated = false;
    ListOfPlayers.clearText();
    clearVectorOfPointer(Bullets);
    clearVectorOfPointer(Enemies);
    clearVectorOfPointer(TempTextsOnGround);
    clearVectorOfPointer(TempTextsOnScreen);
    clearVectorOfPointer(DamageText);
    clearVectorOfPointer(MessageText);
    clearVectorOfPointer(listOfBox);
    clearVectorOfPointer(listOfArtifact);
    clearVectorOfPointer(listOfFire);
    clearVectorOfPointer(PickupStuff);
    player.CurWeapon->lock = true;
    normalize(CurWeapon);
    LoadMainMenu();
    saveGame();
    });


//////////////////////////////////////////////////////////// functions realizations

//============================================================================================== INITS
void init() {
    setlocale(LC_ALL, "rus");

    window.setVerticalSyncEnabled(true);
    settings.antialiasingLevel = 8;
    window.setView(GameView);

    preRenderTexture.create(scw, sch);
    preRenderSprite.setTexture(preRenderTexture.getTexture());

    outlineRenderTexture.create(scw, sch);
    outlineRenderSprite.setTexture(outlineRenderTexture.getTexture());

    MiniMapView.setViewport(sf::FloatRect(0.f, 0.f, 0.25f, 0.25f));
    MiniMapZoom = std::pow(1.1, -10);
    MiniMapView.zoom(MiniMapZoom);
    GameClock = new sf::Clock;
    coutClock = new sf::Clock;

    // Load locations
    WaitingRoomLoaction.LoadFromFile("sources/locations/WaitingRoom.txt");
    MainMenuLocation.LoadFromFile("sources/locations/MainMenu.txt");

    loadTextures();
    loadItemTextures();
    loadFonts();
    loadShaders();
    loadMusics();
    loadSave();

    Musics::MainMenu.setLoop(true);
    Musics::MainMenu.setVolume(5);
    Musics::Fight1.setVolume(5);
    Musics::Fight2.setVolume(5);

    portal.setAnimation(Textures::Portal, 9, 1, sf::seconds(1), &Shaders::Portal);
    portal.setSize(170.f, 320.f);
    player.setAnimation(Textures::Player, &Shaders::Player);
    puddle.setAnimation(Textures::Puddle);
    puddle.setSize(90.f, 90.f);
    shopSector.setAnimation(Textures::INVISIBLE);
    shopSector.setPosition(0, 2 * size);

    upgradeSector.setAnimation(Textures::INVISIBLE);
    upgradeSector.setPosition(6 * size, 2 * size);

    Shaders::Flashlight.setUniform("uResolution", sf::Vector2f(scw, sch));
    Shaders::Flashlight.setUniform("u_playerRadius", player.hitbox.getRadius());

    Shaders::Distortion1.setUniform("noise_png", Textures::Noise);

    Shaders::Distortion2.setUniform("noise_png", Textures::Noise);

    Shaders::Outline.setUniform("uResolution", sf::Vector2f(scw, sch));

    Shaders::Bullet.setUniform("noise_png", Textures::Noise);

    Shaders::Fire.setUniform("noise_png", Textures::Noise);

    IPPanel.setTexture(Textures::YellowPanel);
    ListOfPlayers.setTexture(Textures::GradientFrameAlpha);

    EscapeButton.setTexture(Textures::RedPanel, Textures::RedPanelPushed);
    HostButton.setTexture(Textures::GreenPanel, Textures::GreenPanelPushed);

    CurWeapon.looped = true;

    listener.setBlocking(false);
    MyIP = MySocket.getRemoteAddress().getLocalAddress().toString();
    std::cout << "LocalAddress: " << MyIP << "\nPublicAddress: " << MySocket.getRemoteAddress().getPublicAddress().toString() << '\n';

    initInterface();

    for (int i = 0; i < Effects::EffectCount; i++) {
        effectIconsTimers[i] = new TempText(sf::Time::Zero);
        effectIcons[i] = new sf::Sprite();
    }
    effectIcons[2]->setTexture(Textures::Eff_HPRegen);
    effectIcons[3]->setTexture(Textures::Eff_Burn);

    CurWeapon = { {0, 2, 0} };

    initInventory();
    initShops();

    LoadMainMenu();
}

void initInventory() {
    {
        using namespace inventoryInterface;
        pageElements.resize(inventoryPage::PageCount);
        itemSlotsElements.resize(ItemID::ItemCount, ItemSlot());
        itemSlotsRects.resize(ItemID::ItemCount);

        inventoryFrame.setTexture(Textures::GridBG);
        inventoryFrame.setDrawChildren(false);
        inventoryFrame.setSpritePos(0, 0);

        statsFrame.parentTo(&inventoryFrame);
        statsFrame.hide();

        int xOffset = 50, yOffset = 50;

        backButton.setTexture(Textures::RedPanel, Textures::RedPanelPushed);
        backButton.setCharacterSize(52);
        backButton.setPosition(0, 0);
        backButton.setSize(300, 150);

        itemsButton.setTexture(Textures::YellowButton, Textures::YellowButtonPushed);
        itemsButton.setCharacterSize(32);
        itemsButton.setPosition(0, sch * 0.9);
        itemsButton.setSize(scw * 0.2, sch * 0.1);

        weaponsButton.setTexture(Textures::RedButton, Textures::RedButtonPushed);
        weaponsButton.setCharacterSize(32);
        weaponsButton.setPosition(scw * 0.2, sch * 0.9);
        weaponsButton.setSize(scw * 0.2, sch * 0.1);
        weaponsButton.setFunction([](){
            activePage = inventoryPage::Arsenal;
            for (UIElement*& elem : inventoryFrame.getChildren()) {
                if (elem->getName().substr(0, 9) == "PageFrame")
                    elem->hide();
            }
            weaponsFrame.show();
            openUpgradeShop();
        });

        equipablesButton.setTexture(Textures::GreenButton, Textures::GreenButtonPushed);
        equipablesButton.setCharacterSize(32);
        equipablesButton.setPosition(scw * 0.4, sch * 0.9);
        equipablesButton.setSize(scw * 0.2, sch * 0.1);

        perksButton.setTexture(Textures::BlueButton, Textures::BlueButtonPushed);
        perksButton.setCharacterSize(32);
        perksButton.setPosition(scw * 0.6, sch * 0.9);
        perksButton.setSize(scw * 0.2, sch * 0.1);

        statsButton.setTexture(Textures::GreyButton, Textures::GreyButtonPushed);
        statsButton.setCharacterSize(32);
        statsButton.setPosition(scw * 0.8, sch * 0.9);
        statsButton.setSize(scw * 0.2, sch * 0.1);

        itemListBG.setTexture(Textures::GradientFrameAlpha);
        itemListBG.setScale((float)(scw - 300) / Textures::GradientFrameAlpha.getSize().x, (float)sch / Textures::GradientFrameAlpha.getSize().y * 0.6);
        itemListBG.setPosition(150, sch * 0.2);

        commonElements.push_back(&inventoryFrame);
        commonElements.push_back(&backButton);
        commonElements.push_back(&itemsButton);
        commonElements.push_back(&weaponsButton);
        commonElements.push_back(&equipablesButton);
        commonElements.push_back(&perksButton);
        commonElements.push_back(&statsButton);


        statsPlayerImage.setTexture(Textures::Player);
        statsPlayerImage.setPosition(5 * scw / 6 - Textures::Player.getSize().x / 2, sch / 2 - Textures::Player.getSize().y / 2);

        statsHPBar.parentTo(&statsFrame);
        statsHPBar.setValue(player.Health);
        statsHPBar.setPosition(3 * scw / 10.f, 2 * sch / 10.f);
        statsHPBar.setColors(CommonColors::barWall, sf::Color(192, 0, 0, 160), CommonColors::barBG);
        statsHPText.setCharacterSize(24);
        statsHPText.setString("Health");
        statsHPText.setPosition(scw / 10.f, 2 * sch / 10.f);

        statsMPBar.parentTo(&statsFrame);
        statsMPBar.setValue(player.Mana);
        statsMPBar.setPosition(3 * scw / 10.f, 3 * sch / 10.f);
        statsMPBar.setColors(CommonColors::barWall, sf::Color(0, 0, 192, 160), CommonColors::barBG);
        statsMPText.setCharacterSize(24);
        statsMPText.setString("Mana");
        statsMPText.setPosition(scw / 10.f, 3 * sch / 10.f);

        statsHPRegenText.setCharacterSize(24);
        statsHPRegenText.setString("Health regen: " + floatToString(player.HealthRecovery));
        statsHPRegenText.setPosition(scw / 10.f, 4 * sch / 10.f);

        statsMPRegenText.setCharacterSize(24);
        statsMPRegenText.setString("Mana regen: " + floatToString(player.ManaRecovery));
        statsMPRegenText.setPosition(scw / 10.f, 5 * sch / 10.f);

        statsArmorText.setCharacterSize(24);
        statsArmorText.setString("Armor: " + floatToString(player.Armor.cur));
        statsArmorText.setPosition(scw / 10.f, 6 * sch / 10.f);

        statsCompletedLevelsText.setCharacterSize(24);
        statsCompletedLevelsText.setString("Completed Levels: " + std::to_string(completedLevels));
        statsCompletedLevelsText.setPosition(scw / 10.f, 7 * sch / 10.f);

        statsCurLevelsText.setCharacterSize(24);
        statsCurLevelsText.setString("Current Levels: " + std::to_string(curLevel));
        statsCurLevelsText.setPosition(scw / 10.f, 8 * sch / 10.f);

        coinSprite.setAnimation(*itemTexture[ItemID::coin], itemTextureFrameAmount[ItemID::coin],
            1, itemTextureDuration[ItemID::coin]);
        coinSprite.play();
        coinSlot.init();

        pageElements[inventoryPage::Items].push_back(&itemListBG);
        pageElements[inventoryPage::Items].push_back(&coinSprite);

        pageElements[inventoryPage::Stats].push_back(&statsFrame);
        pageElements[inventoryPage::Stats].push_back(&statsPlayerImage);
        pageElements[inventoryPage::Stats].push_back(&statsHPText);
        pageElements[inventoryPage::Stats].push_back(&statsMPText);
        pageElements[inventoryPage::Stats].push_back(&statsHPRegenText);
        pageElements[inventoryPage::Stats].push_back(&statsMPRegenText);
        pageElements[inventoryPage::Stats].push_back(&statsArmorText);
        pageElements[inventoryPage::Stats].push_back(&statsCompletedLevelsText);
        pageElements[inventoryPage::Stats].push_back(&statsCurLevelsText);
        {
            using namespace upgradeInterface;

            pageElements[inventoryPage::Arsenal].push_back(&weaponImg);
            pageElements[inventoryPage::Arsenal].push_back(&switchGunLBtn);
            pageElements[inventoryPage::Arsenal].push_back(&switchGunRBtn);
            pageElements[inventoryPage::Arsenal].push_back(&weaponDescFrame);
            pageElements[inventoryPage::Arsenal].push_back(&weaponDescText);
            pageElements[inventoryPage::Arsenal].push_back(&generatorBtn);
            pageElements[inventoryPage::Arsenal].push_back(&formFactorBtn);
            pageElements[inventoryPage::Arsenal].push_back(&converterBtn);
            pageElements[inventoryPage::Arsenal].push_back(&targetingBtn);
            compUpgElements.push_back(&listFade);
            compUpgElements.push_back(&listBG);
            compUpgElements.push_back(&listTextPanel);
            compUpgElements.push_back(&imgPanel);
        }
        doInventoryUpdate[inventoryPage::Stats] = true;
        doInventoryUpdate[inventoryPage::Arsenal] = true;
    }
}

void initShops() {
    {
        using namespace MenuShop;
        slotsElements.resize(ItemID::ItemCount);
        playerSlotsElements.resize(ItemID::ItemCount);
        slotsRects.resize(ItemID::ItemCount);
        mainMenuShop.setShop(new std::vector<Item*>{ new Item(ItemID::regenDrug, 100) },
            std::vector<int>{20});
        mainMenuShop.setFunction([]() {
            if (shopSelectedItem != nullptr) {
                if (player.inventory.money >= mainMenuShop.itemPrices[shopSelectedItem->id]) {
                    Item* boughtItem = new Item(*shopSelectedItem);
                    boughtItem->amount = 1;
                    player.addItem(boughtItem);
                    player.inventory.money -= mainMenuShop.itemPrices[shopSelectedItem->id];
                    shopSelectedItem->amount--;
                    mainMenuShop.soldItems.removeItem(shopSelectedItem, false);
                    NPCText.setString("Thank you for buying a " + stringLower(itemName[shopSelectedItem->id]) + "!");
                    if (!mainMenuShop.soldItems.find(shopSelectedItem)) {
                        itemSprite.setTexture(Textures::INVISIBLE);
                        shopSelectedItem = nullptr;
                        createShopSlotsRects();
                    }
                }
                else {
                    NPCText.setString("Sorry, but you cannot afford a " + stringLower(itemName[shopSelectedItem->id]) + ".");
                }
            }
            });

        BG.setTexture(Textures::GridBG);

        shopBackButton.setTexture(Textures::RedPanel, Textures::RedPanelPushed);
        shopBackButton.setCharacterSize(36);
        shopBackButton.setSize(250, 50);

        NPCTextFrame.setTexture(Textures::GradientFrameAlpha);
        NPCTextFrame.setColor(sf::Color(0x10, 0xBB, 0xFF));
        NPCTextFrame.setScale((float)(scw - 100) / Textures::GradientFrameAlpha.getSize().x,
            200.0 / Textures::GradientFrameAlpha.getSize().y);
        NPCTextFrame.setPosition(50, shopBackButton.hitbox.getBottom() + 50 / 5);

        NPCSprite.setTexture(Textures::DistortedScientist);
        NPCSprite.setScale((NPCTextFrame.getGlobalBounds().height - 100) / NPCSprite.getTexture()->getSize().x,
            (NPCTextFrame.getGlobalBounds().height - 100) / NPCSprite.getTexture()->getSize().y);
        NPCSprite.setPosition(NPCTextFrame.getPosition() + sf::Vector2f(100, 50));

        NPCName.setCharacterSize(32);
        NPCName.setPosition(NPCSprite.getPosition().x + NPCSprite.getGlobalBounds().width + 50,
            NPCSprite.getPosition().y + NPCSprite.getGlobalBounds().height / 2 - 50 / 2);
        NPCName.setString(textWrap("Shop keeper", 20));

        NPCText.setCharacterSize(32);
        NPCText.setPosition(NPCTextFrame.getPosition().x + NPCTextFrame.getGlobalBounds().width * 0.25,
            NPCTextFrame.getPosition().y + 50);

        itemsFrame.setTexture(Textures::GradientFrameAlpha);
        itemsFrame.setColor(sf::Color(0xCC, 0xAA, 0x11));
        itemsFrame.setScale((0.6 * scw - 100) / (Textures::GradientFrameAlpha.getSize().x),
            (0.35 * sch - 50) / (Textures::GradientFrameAlpha.getSize().y));
        itemsFrame.setPosition(50, shopBackButton.hitbox.getBottom() + 200.0 + 50 / 2);

        itemsViewSizeX = (0.6 * scw - 100) / scw;
        itemsViewSizeY = (0.35 * sch - 50) / sch;
        ShopStockView.setViewport(sf::FloatRect((itemsFrame.getPosition().x + 50) / scw,
            (itemsFrame.getPosition().y + 50 / 3) / sch,
            itemsViewSizeX, itemsViewSizeY));

        playerInvFrame.setTexture(Textures::GradientFrameAlpha);
        playerInvFrame.setColor(sf::Color(0xBB, 0x40, 0x40));
        playerInvFrame.setScale((0.4 * scw - 50) / (Textures::GradientFrameAlpha.getSize().x),
            (0.35 * sch - 50) / (Textures::GradientFrameAlpha.getSize().y));
        playerInvFrame.setPosition(0.6 * scw, itemsFrame.getPosition().y);

        playerInvViewSizeX = (0.6 * scw - 100) / scw;
        playerInvViewSizeY = (0.35 * sch - 50) / sch;
        ShopPlayerInvView.setViewport(sf::FloatRect((playerInvFrame.getPosition().x + 50) / scw,
            (playerInvFrame.getPosition().y + 50) / sch,
            playerInvViewSizeX, playerInvViewSizeY));

        itemStatsFrame.setTexture(Textures::GradientFrameAlpha);
        itemStatsFrame.setColor(sf::Color(0xCC, 0xAA, 0x11));
        itemStatsFrame.setScale((0.6 * scw - 100) / (Textures::GradientFrameAlpha.getSize().x),
            (0.35 * sch - 50) / (Textures::GradientFrameAlpha.getSize().y));
        itemStatsFrame.setPosition(50, itemsFrame.getPosition().y + 0.35 * sch - 50 + 50 / 2);

        itemSpriteFrame.setTexture(Textures::ItemPanel);
        itemSpriteFrame.setScale(0.5 * itemStatsFrame.getGlobalBounds().height / Textures::ItemPanel.getSize().x,
            0.5 * itemStatsFrame.getGlobalBounds().height / Textures::ItemPanel.getSize().y);
        itemSpriteFrame.setPosition(itemStatsFrame.getPosition().x + 50,
            itemStatsFrame.getPosition().y + 50 * 0.3);
        itemSpriteFrame.setColor(sf::Color(0xAA, 0x88, 0x00));

        itemSprite.setTexture(Textures::INVISIBLE, true);
        itemSprite.setScale(2.5 * itemSpriteFrame.getGlobalBounds().width / Textures::INVISIBLE.getSize().x,
            2.5 * itemSpriteFrame.getGlobalBounds().height / Textures::INVISIBLE.getSize().y);
        itemSprite.setPosition(itemSpriteFrame.getPosition().x,
            itemSpriteFrame.getPosition().y);

        itemCoins.setCharacterSize(40);
        itemCoins.setPosition(itemSpriteFrame.getPosition().x + itemSpriteFrame.getGlobalBounds().width * 0.3,
            itemSpriteFrame.getPosition().y + itemSpriteFrame.getGlobalBounds().height * 1.2);

        itemCoinsSprite.setAnimation(*itemTexture[ItemID::coin], itemTextureFrameAmount[ItemID::coin],
            1, itemTextureDuration[ItemID::coin]);
        itemCoinsSprite.setPosition(itemSpriteFrame.getPosition().x + itemSpriteFrame.getGlobalBounds().width * 0.5 + 50,
            itemSpriteFrame.getPosition().y + itemSpriteFrame.getGlobalBounds().height + 25);
        itemCoinsSprite.setSize(sf::Vector2f(100.0, 100.0));
        itemCoinsSprite.play();

        itemStats.setCharacterSize(30);
        itemStats.setPosition(itemSpriteFrame.getPosition().x + itemSpriteFrame.getGlobalBounds().width + 50,
            itemSpriteFrame.getPosition().y + 25);

        shopBuyButton.setTexture(Textures::YellowPanel, Textures::YellowPanelPushed);
        shopBuyButton.setCharacterSize(70);
        shopBuyButton.setSize(400, 150);
        shopBuyButton.setPosition(playerInvFrame.getPosition().x + (0.4 * scw - 50) / 5,
            sch - 150 - 1.5 * 50);

        playerCoinsText.setCharacterSize(40);
        playerCoinsText.setPosition(shopBuyButton.hitbox.getPosition() - sf::Vector2f(0, 100));

        UIElements.push_back(&BG);
        UIElements.push_back(&shopBackButton);
        UIElements.push_back(&NPCTextFrame);
        UIElements.push_back(&NPCSprite);
        UIElements.push_back(&NPCName);
        UIElements.push_back(&NPCText);
        UIElements.push_back(&itemsFrame);
        UIElements.push_back(&playerInvFrame);
        UIElements.push_back(&itemStatsFrame);
        UIElements.push_back(&itemSpriteFrame);
        UIElements.push_back(&shopBuyButton);
        UIElements.push_back(&playerCoinsText);

        UIElements.push_back(&inventoryInterface::coinSprite);
        UIElements.push_back(inventoryInterface::coinSlot.background);  // It is invisible for now, but may be changed in the future for something visible
        UIElements.push_back(inventoryInterface::coinSlot.amountText);
    }


    {
        using namespace upgradeInterface;
        BG.setTexture(Textures::GridBG);

        backButton.setTexture(Textures::RedPanel, Textures::RedPanelPushed);
        backButton.setCharacterSize(36);
        backButton.setSize(250, 50);

        weaponImg.setTexture(Textures::PH_gun);
        weaponImg.setPosition((scw - Textures::PH_gun.getSize().x) / 2,
            (sch - Textures::PH_gun.getSize().y) / 2 + 75.f);

        switchGunLBtn.setHitboxPoints(std::vector<sf::Vector2f>{{0, 50}, { 0, -50 }, { -50, 0 }}, true);
        switchGunLBtn.setFillColor(CommonColors::text);
        switchGunLBtn.setOutlineColor(sf::Color::Black);
        switchGunLBtn.setOutlineThickness(3);
        switchGunLBtn.setTexture(Textures::INVISIBLE, Textures::INVISIBLE);
        switchGunLBtn.setCenter(weaponImg.getPosition() +
            sf::Vector2f(-300, weaponImg.getGlobalBounds().height / 2));
        switchGunLBtn.setFunction([]() {
            int ind = (std::find(arsenal.begin(), arsenal.end(), player.CurWeapon) - arsenal.begin() - 1);
            player.CurWeapon = arsenal[ind + arsenal.size() * (ind < 0)];
            openUpgradeShop();
            });

        switchGunRBtn.setHitboxPoints(std::vector<sf::Vector2f>{{0, 50}, { 0, -50 }, { 50, 0 }}, true);
        switchGunRBtn.setFillColor(CommonColors::text);
        switchGunRBtn.setOutlineColor(sf::Color::Black);
        switchGunRBtn.setOutlineThickness(3);
        switchGunRBtn.setTexture(Textures::INVISIBLE, Textures::INVISIBLE);
        switchGunRBtn.setCenter(weaponImg.getPosition() +
            sf::Vector2f(weaponImg.getGlobalBounds().width + 300, weaponImg.getGlobalBounds().height / 2));
        switchGunRBtn.setFunction([]() {
            int ind = (std::find(arsenal.begin(), arsenal.end(), player.CurWeapon) - arsenal.begin() + 1);
            player.CurWeapon = arsenal[ind % arsenal.size()];
            openUpgradeShop();
            });

        weaponDescText.setCharacterSize(30);
        weaponDescText.setPosition(scw / 2 - Textures::PH_gun.getSize().x - 75, 80);

        weaponDescFrame.setTexture(Textures::GradientFrameAlpha);
        weaponDescFrame.setColor(sf::Color::Black);
        weaponDescFrame.setScale((0.5f * scw - 50.f) / Textures::GradientFrameAlpha.getSize().x,
            475.0 / Textures::GradientFrameAlpha.getSize().y);
        weaponDescFrame.setPosition(scw / 2 - Textures::PH_gun.getSize().x - 100, 75);

        sf::Texture* fadeTexture = new sf::Texture();
        sf::Image fadeTexturePixels = sf::Image();
        fadeTexturePixels.create(scw, sch, sf::Color(255, 255, 255, 128));
        for (int i = 0; i < scw; i++)
            for (int j = 0; j < sch; j++)
                if (i >= scw / 8 && i <= scw - scw / 8 &&
                    j >= sch / 4 && j <= sch - sch / 4) {
                    float gradVal = 25 * (((int)(i - scw / 8 + j - sch / 4) / (255)) % 25);
                    fadeTexturePixels.setPixel(i, j, sf::Color(gradVal, gradVal, gradVal, 255));
                }
        fadeTexture->create(scw, sch);
        fadeTexture->update(fadeTexturePixels);
        listFade.sprite.setTexture(*fadeTexture);

        listTextPanel.setTexture(Textures::GradientFrame);
        listTextPanel.setSize(scw - 2 * scw / 8, sch / 8);
        listTextPanel.setPosition(scw / 8, 100);
        listTextPanel.setString("Choose a component upgrade");
        listTextPanel.setCharacterSize(40);

        listBG.setTexture(Textures::GradientFrameAlpha);
        listBG.setSize(scw - 2 * scw / 8, sch - 2 * sch / 4);
        listBG.setPosition(scw / 8, sch / 4);

        imgPanel.setTexture(Textures::PH_FormFactor);
        imgPanel.setSize(scw / 4, sch / 4);
        imgPanel.setPosition(listBG.getPosition() + sf::Vector2f(listBG.getSize().x, 0) -
                             sf::Vector2f(imgPanel.getSize().x, 0) -
                             sf::Vector2f(25, -25));

        playerCoinAmount.setString(FontString("You have:", 50, sf::Color(200, 20, 200)));
        playerCoinAmount.setPosition(listBG.getPosition() + listBG.getSize() - sf::Vector2f(450, 100));

        generatorBtn.setTexture(Textures::INVISIBLE, Textures::INVISIBLE);
        generatorBtn.setHitboxPoints(CommonShapes::starShape, true);
        generatorBtn.setPosition(0.1 * scw, sch / 2 + 100.f);
        generatorBtn.setFunction([]() {
            isChoosingComponent = true;
            compType = 0;
            if (isDrawInventory)
                listTextPanel.setString(FontString("You can only upgrade at a special upgrade shop.", 50, CommonColors::warning));
            else listTextPanel.setString(FontString("Choose a Coch Generator upgrade   (press RMB to close)", 50, CommonColors::text));
            imgPanel.setTexture(Textures::PH_CochGen);
            });

        generatorBtn.setFillColor(sf::Color::Transparent);
        generatorBtn.setOutlineThickness(3);
        generatorBtn.setOutlineColor(sf::Color(192, 192, 255, 255));


        formFactorBtn.setTexture(Textures::INVISIBLE, Textures::INVISIBLE);
        formFactorBtn.setHitboxPoints(CommonShapes::rectShape, true);
        formFactorBtn.setPosition(scw / 3, 0.7 * sch + 50.f);
        formFactorBtn.setFunction([]() {
            isChoosingComponent = true;
            compType = 1;
            if (isDrawInventory)
                listTextPanel.setString(FontString("You can only upgrade at a special upgrade shop.", 50, CommonColors::warning));
            else listTextPanel.setString(FontString("Choose a Form-factor upgrade   (press RMB to close)", 50, CommonColors::text));
            imgPanel.setTexture(Textures::PH_FormFactor);
            });

        formFactorBtn.setFillColor(sf::Color::Transparent);
        formFactorBtn.setOutlineThickness(3);
        formFactorBtn.setOutlineColor(sf::Color(255, 255, 192, 255));


        converterBtn.setTexture(Textures::INVISIBLE, Textures::INVISIBLE);
        converterBtn.setHitboxPoints(RotateOn(-30, CommonShapes::triangleShape), true);
        converterBtn.setPosition(0.6 * scw, 0.65 * sch + 50.f);
        converterBtn.setFunction([]() {
            isChoosingComponent = true;
            compType = 2;
            if (isDrawInventory)
                listTextPanel.setString(FontString("You can only upgrade at a special upgrade shop.", 50, CommonColors::warning));
            else listTextPanel.setString(FontString("Choose a Converter upgrade   (press RMB to close)", 50, CommonColors::text));
            imgPanel.setTexture(Textures::PH_Converter);
            });

        converterBtn.setFillColor(sf::Color::Transparent);
        converterBtn.setOutlineThickness(3);
        converterBtn.setOutlineColor(sf::Color(192, 255, 192, 255));


        targetingBtn.setTexture(Textures::INVISIBLE, Textures::INVISIBLE);
        targetingBtn.setHitboxPoints(RotateOn(60, CommonShapes::frustumShape), true);
        targetingBtn.setPosition(0.825 * scw, sch / 2 + 100.f);
        targetingBtn.setFunction([]() {
            isChoosingComponent = true;
            compType = 3;
            if (isDrawInventory)
                listTextPanel.setString(FontString("You can only upgrade at a special upgrade shop.", 50, CommonColors::warning));
            else listTextPanel.setString(FontString("Choose a Targeting mechanism upgrade   (press RMB to close)", 50, CommonColors::text));
            imgPanel.setTexture(Textures::PH_Targeting);
            });

        targetingBtn.setFillColor(sf::Color::Transparent);
        targetingBtn.setOutlineThickness(3);
        targetingBtn.setOutlineColor(sf::Color(255, 192, 192, 255));


        compUpgBtns.resize(4);
        compUpgCosts.resize(4);
        compUpgStats.resize(4);
        compUpgCount.resize(4);


        UIElements.push_back(&BG);
        UIElements.push_back(&backButton);
        UIElements.push_back(&switchGunLBtn);
        UIElements.push_back(&switchGunRBtn);
        UIElements.push_back(&weaponImg);
        UIElements.push_back(&weaponDescFrame);
        UIElements.push_back(&weaponDescText);
        UIElements.push_back(&generatorBtn);
        UIElements.push_back(&formFactorBtn);
        UIElements.push_back(&converterBtn);
        UIElements.push_back(&targetingBtn);
        choiceUIElements.push_back(&listFade);
        choiceUIElements.push_back(&listBG);
        choiceUIElements.push_back(&listTextPanel);
        choiceUIElements.push_back(&imgPanel);
        choiceUIElements.push_back(&inventoryInterface::coinSprite);
        choiceUIElements.push_back(&playerCoinAmount);
    }
}

void initInterface() {
    EscapeButton.setCharacterSize(110);
    IPPanel.text.setCharacterSize(80);
    ListOfPlayers.text.setCharacterSize(60);

    EscapeButton.setCenter(scw / 2, sch * 3 / 4);

    ListOfPlayers.setCenter(scw / 2, sch / 4);

    MMPlayerCircle.setRadius(9);
    MMPlayerCircle.setFillColor(sf::Color(0, 180, 0));
    MMPlayerCircle.setOrigin(MMPlayerCircle.getRadius(), MMPlayerCircle.getRadius());

    MMEnemyCircle.setRadius(9);
    MMEnemyCircle.setFillColor(sf::Color(180, 0, 0));
    MMEnemyCircle.setOrigin(MMEnemyCircle.getRadius(), MMEnemyCircle.getRadius());

    HUDFrame.setPadding(0, 30, 20, 0);

    HPBar.parentTo(&HUDFrame, true);
    HPBar.setValue(player.Health);
    HPBar.setColors(CommonColors::barWall, sf::Color(192, 0, 0, 160), CommonColors::barBG);

    MPBar.parentTo(&HPBar, true);
    MPBar.setValue(player.Mana);
    MPBar.setColors(CommonColors::barWall, sf::Color(0, 0, 192, 160), CommonColors::barBG);

    for (int i = 0; i < arsenal.size(); i++) {
        AmmoBars.push_back(new Bar<float>("AmmoBar_" + arsenal[i]->Name,
                                          20,
                                          sch - 20 - (arsenal.size() - i) * 60,
                                          160,
                                          50));
        AmmoBars[i]->parentTo(&HUDFrame);
        AmmoBars[i]->setColors(CommonColors::barWall, sf::Color(128, 128, 128, 160), CommonColors::barBG);

        WeaponNameTexts.push_back(new PlacedText(arsenal[i]->Name));
        WeaponNameTexts[i]->setPosition(35 + AmmoBars[i]->getSize().x,
                                        AmmoBars[i]->getPosition().y + WeaponNameTexts[0]->Height / 4);
        WeaponNameTexts[i]->setFillColor(sf::Color(25, 192, 25, 160));
        WeaponNameTexts[i]->setOutlineColor(sf::Color::Black);
    }

    ReloadWeaponText.setFillColor(sf::Color(255, 20, 20));
    ReloadWeaponText.setCharacterSize(100);

    XButtonSprite.setTexture(Textures::XButton);
    XButtonSprite.setPosition(scw / 2.f - XButtonSprite.getGlobalBounds().width / 2.f, sch * 3.f / 4.f - XButtonSprite.getGlobalBounds().height / 2.f);

    MMPortalRect.setSize(portal.hitbox.getSize() * ScaleParam);
    MMPortalRect.setFillColor(sf::Color(200, 0, 200, 200));

    MMBoxRect.setSize(sf::Vector2f(105.f, 117.f) * ScaleParam);
    MMBoxRect.setFillColor(sf::Color(252, 108, 24, 200));

    MMPuddleRect.setSize(puddle.hitbox.getSize() * ScaleParam);
    MMPuddleRect.setFillColor(sf::Color(0, 0, 255, 200));

    MMArtifact.setSize(sf::Vector2f(150.f, 105.f) * ScaleParam);
    MMArtifact.setFillColor(sf::Color::White);

    EnemyHealthBar.setColors(CommonColors::barWall, sf::Color(192, 0, 0, 160), CommonColors::barBG);
    EnemyHealthBar.setSize(125.f, 15.f);
    EnemyHealthBar.setWallWidth(1);
    EnemyHealthBar.ShowText = false;

    FLoorTileSprite.setScale(5.f, 5.f);
    FLoorTileSprite.setTexture(Textures::floor);

    undergroundBG.setTexture(Textures::Noise);
    undergroundBG.setPosition(0, 0);
    undergroundBG.setScale(scw / undergroundBG.getLocalBounds().width, sch / undergroundBG.getLocalBounds().height);
}
//==============================================================================================



//============================================================================================== DRAW FUNCTIONS
void draw() {
    window.clear(sf::Color::Transparent);
    updateShaders();
    if (isDrawInventory) {
        drawInventory();
    }
    else if (isDrawShop) {
        drawShop();
    }
    else if (isDrawUpgradeInterface) {
        drawUpgradeInterface();
    }
    else {
        preRenderTexture.clear(sf::Color::Transparent);
        preRenderTexture.setView(HUDView);
        preRenderTexture.draw(undergroundBG, &Shaders::Distortion1);

        preRenderTexture.setView(GameView);
        outlineRenderTexture.setView(GameView);
        outlineRenderSprite.setPosition(GameView.getCenter() - GameView.getSize() / 2.f);

        drawFloor();
        drawWalls();

        for (sf::Drawable*& d : DrawableStuff) {
            if (d == CurInteractable) {
                outlineRenderTexture.clear(sf::Color::Transparent);
                outlineRenderTexture.draw(*d); outlineRenderTexture.display();
                preRenderTexture.draw(outlineRenderSprite, &Shaders::Outline);
            }
            else {
                preRenderTexture.draw(*d);
            }
        }

        for (Enemy*& enemy : Enemies) {
            EnemyHealthBar.setPosition(enemy->hitbox.getCenter() - sf::Vector2f(EnemyHealthBar.getSize().x / 2.f, enemy->hitbox.getRadius() + 50.f));
            EnemyHealthBar.setValue(enemy->Health);
            preRenderTexture.draw(EnemyHealthBar);
        }

        preRenderTexture.display();

        window.setView(HUDView);
        window.draw(preRenderSprite, &Shaders::Flashlight);

        preRenderTexture.clear(sf::Color::Transparent);
        for (int i = 0; i < Bullets.size(); i++) {
            preRenderTexture.draw(*Bullets[i]);
        }
        preRenderTexture.display();
        window.draw(preRenderSprite, &Shaders::Bullet);

        window.setView(GameView);

        for (size_t i = 0; i < TempTextsOnGround.size(); i++) {
            if (TempTextsOnGround[i]->localClock->getElapsedTime() < TempTextsOnGround[i]->howLongToExist) {
                window.draw(*TempTextsOnGround[i]);
            }
            else {
                DeletePointerFromVector(TempTextsOnGround, i--);
            }
        }

        for (size_t i = 0; i < DamageText.size(); i++) {
            if (DamageText[i]->localClock->getElapsedTime() < DamageText[i]->howLongToExist) {
                Shaders::FloatingUp.setUniform("uTime", DamageText[i]->localClock->getElapsedTime().asSeconds());
                window.draw(*DamageText[i], &Shaders::FloatingUp);
            }
            else {
                DeletePointerFromVector(DamageText, i--);
            }
        }

        if (IsDrawMinimap) {
            drawMiniMap();
        }
        if (IsdrawHUD) {
            drawHUD();
        }
    }
    window.display();

    // DEBUG
    //if (coutClock->getElapsedTime() >= coutEvery) {
    //    std::cout << "\n[END OF DRAW CALL #" << ++drawCount << "]\n------------------------------------------\n";
    //    coutClock->restart();
    //}
}

void drawFloor() {
    for (int i = 0; i < CurLocation->n; i++) {
        for (int j = 0; j < CurLocation->m; j++) {
            if (CurLocation->EnableTiles[i][j]) {
                FLoorTileSprite.setPosition(size * j, size * i);
                preRenderTexture.draw(FLoorTileSprite);
            }
        }
    }
}

sf::Vector2f CameraPos;
CollisionRect CameraRect(0, 0, scw, sch);
void drawWalls() {
    CameraPos = GameView.getCenter() - GameView.getSize() / 2.f;
    CameraRect.setPosition(CameraPos);
    for (int i = std::max(0, 2 * int((CameraPos.y - WallMinSize / 2) / size));
        i <= std::min(int(CurLocation->walls.size() - 1), 2 * int((CameraPos.y + sch + WallMinSize / 2) / size) + 1); i++) {
        for (int j = std::max(0, int(CameraPos.x / size));
            j <= std::min(int(CurLocation->walls[i].size() - 1), int((CameraPos.x + scw + WallMinSize) / size)); j++) {
            if (CurLocation->walls[i][j]) {
                CurLocation->SeenWalls[i][j] = CurLocation->SeenWalls[i][j] || CameraRect.intersect(CurLocation->wallsRect[i][j]);
                WallRect.setPosition(CurLocation->wallsRect[i][j].getPosition());
                WallRect.setTexture((i % 2 == 1) ? Textures::WallV : Textures::WallG, true);
                preRenderTexture.draw(WallRect);
            }
        }
    }
}

void drawMiniMap() {
    if (MiniMapHoldOnPlayer) {
        if (!sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
            MiniMapView.setCenter(player.hitbox.getCenter() * ScaleParam);
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
                }
                else { // -
                    line[0] = sf::Vertex(sf::Vector2f(miniSize * j, miniSize * i / 2));
                    line[1] = sf::Vertex(sf::Vector2f(miniSize * (j + 1), miniSize * i / 2));
                }
                window.draw(line);
            }
        }
    }

    MMPortalRect.setPosition(portal.hitbox.getPosition() * ScaleParam);
    window.draw(MMPortalRect);

    for (Interactable*& i : listOfBox) {
        MMBoxRect.setPosition(i->hitbox.getPosition() * ScaleParam);
        window.draw(MMBoxRect);
    }

    MMPuddleRect.setPosition(puddle.hitbox.getPosition() * ScaleParam);
    window.draw(MMPuddleRect);

    for (Interactable*& i : listOfArtifact) {
        MMArtifact.setPosition(i->hitbox.getPosition() * ScaleParam);
        window.draw(MMArtifact);
    }

    for (Enemy*& enemy : Enemies) {
        MMEnemyCircle.setPosition(enemy->hitbox.getPosition() * ScaleParam);
        window.draw(MMEnemyCircle);
    }

    // draw players
    if (ClientFuncRun || HostFuncRun) {
        for (Player& p : ConnectedPlayers) {
            MMPlayerCircle.setPosition(p.hitbox.getPosition() * ScaleParam);
            window.draw(MMPlayerCircle);
        }
    }
    else {
        MMPlayerCircle.setPosition(player.hitbox.getCenter() * ScaleParam);
        window.draw(MMPlayerCircle);
    }
    window.setView(GameView);
}

sf::Clock ClockFPS; int FPSCounter;
PlacedText TextFPS;
void drawHUD() {
    window.setView(HUDView);
    for (sf::Drawable*& d : InterfaceStuff) {
        window.draw(*d);
    }

    if (player.CurWeapon != nullptr && player.CurWeapon->ManaStorage.toBottom() < player.CurWeapon->ManaCostOfBullet) {
        window.draw(ReloadWeaponText);
    }

    for (int i = 0; i < arsenal.size(); i++) {
        if (arsenal[i] == player.CurWeapon) {
            WeaponNameTexts[i]->setFillColor(sf::Color::White);
            WeaponNameTexts[i]->setOutlineThickness(3);
        }
        else {
            WeaponNameTexts[i]->setFillColor(sf::Color(25, 192, 25, 160));
            WeaponNameTexts[i]->setOutlineThickness(1);
        }
        AmmoBars[i]->setValue(arsenal[i]->ManaStorage);
        AmmoBars[i]->setPosition(20, sch - 20 - (arsenal.size() - i) * (AmmoBars[i]->getSize().y + 10));

        if (arsenal[i]->holstered) {
            float holsterPercent = std::min(arsenal[i]->HolsterTimer->getElapsedTime() /
                arsenal[i]->TimeToHolster, 1.0f);
            sf::Color wallColor(255 - 155 * holsterPercent, 255 - 155 * holsterPercent, 255, 160);
            sf::Color foreColor(128 - 96 * holsterPercent, 128 - 96 * holsterPercent, 128, 160);
            sf::Color backColor(32 - 32 * holsterPercent, 32 - 32 * holsterPercent, 32 - 32 * holsterPercent, 160);
            AmmoBars[i]->setColors(wallColor, foreColor, backColor);
        }
        else {
            float dispatchPercent = std::min(arsenal[i]->DispatchTimer->getElapsedTime() /
                arsenal[i]->TimeToDispatch, 1.0f);
            sf::Color wallColor(100 + 155 * dispatchPercent, 100 + 155 * dispatchPercent, 255, 160);
            sf::Color foreColor(32 + 96 * dispatchPercent, 32 + 96 * dispatchPercent, 128, 160);
            sf::Color backColor(32 * dispatchPercent, 32 * dispatchPercent, 32 * dispatchPercent, 160);
            AmmoBars[i]->setColors(wallColor, foreColor, backColor);
        }

        window.draw(*WeaponNameTexts[i]);
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
        }
        else {
            DeletePointerFromVector(TempTextsOnScreen, i--);
        }
    }

    for (size_t i = 0; i < MessageText.size(); i++) {
        if (MessageText[i]->localClock->getElapsedTime() < MessageText[i]->howLongToExist) {
            Shaders::FloatingUp.setUniform("uTime", MessageText[i]->localClock->getElapsedTime().asSeconds());
            window.draw(*MessageText[i], &Shaders::FloatingUp);
        }
        else {
            DeletePointerFromVector(MessageText, i--);
        }
    }

    drawEffects();

    window.setView(GameView);
}

void drawEffects() {
    int count = 0;
    int xOffset = 175, yOffset = 175;
    std::vector<int> seenEffects(Effects::EffectCount, 0);
    std::vector<sf::Time> effectTimersTimes(Effects::EffectCount, sf::Time::Zero);
    for (Effect* eff : player.effects) {
        if (eff->type != Effects::Heal && eff->type != Effects::Damage && eff->active) {
            if (seenEffects[eff->type] == 0) {
                effectIcons[eff->type]->setScale(0.5, 0.5);
                effectIcons[eff->type]->setPosition(MPBar.getPosition().x - 300 + xOffset * (count % 3),
                    MPBar.getPosition().y + MPBar.getSize().y + 20 + yOffset * (count / 3));

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

    {
        using namespace inventoryInterface;

        for (sf::Drawable*& elem : commonElements)
            window.draw(*elem);
        switch (activePage) {
        case inventoryPage::Items:
            for (sf::Drawable*& elem : pageElements[activePage])
                window.draw(*elem);

            for (Item*& item : player.inventory.items) {
                if (itemSlotsElements[item->id].isInitialized) {
                    window.draw(*itemSlotsElements[item->id].background);

                    if (item->amount >= 1) window.draw(*itemSlotsElements[item->id].amountText);
                }
                window.draw(*item);
            }
            window.draw(*coinSlot.amountText);

            if (isItemDescDrawn) {
                itemDescText.setPosition(sf::Mouse::getPosition(window).x + 100, sf::Mouse::getPosition(window).y);
                window.draw(itemDescText);
            }
            break;

        case inventoryPage::Arsenal:
        {
            for (sf::Drawable*& elem : pageElements[activePage])
                window.draw(*elem);
            float listBGVal = 128 * std::pow(std::sin(GameClock->getElapsedTime().asSeconds()), 2);
            if (upgradeInterface::isChoosingComponent) {
                switch (upgradeInterface::compType) {
                case 0:
                    upgradeInterface::listBG.sprite.setColor(sf::Color(0, 0, listBGVal));
                    break;
                case 1:
                    upgradeInterface::listBG.sprite.setColor(sf::Color(listBGVal, listBGVal, 0));
                    break;
                case 2:
                    upgradeInterface::listBG.sprite.setColor(sf::Color(0, listBGVal, 0));
                    break;
                case 3:
                    upgradeInterface::listBG.sprite.setColor(sf::Color(listBGVal, 0, 0));
                    break;
                }
                for (sf::Drawable*& elem : compUpgElements)
                    window.draw(*elem);
                for (sf::Drawable* elem : upgradeInterface::compUpgBtns[upgradeInterface::compType])
                    window.draw(*elem);
                for (sf::Drawable* elem : upgradeInterface::compUpgCosts[upgradeInterface::compType])
                    window.draw(*elem);
                for (sf::Drawable* elem : upgradeInterface::compUpgStats[upgradeInterface::compType])
                    window.draw(*elem);
                for (sf::Drawable* elem : upgradeInterface::compUpgCount[upgradeInterface::compType])
                    window.draw(*elem);
            }
            break;
        }

        case inventoryPage::Stats:
            for (sf::Drawable*& elem : pageElements[inventoryPage::Stats])
                window.draw(*elem);
            break;

        default:
            break;
        }
    }
    window.setView(HUDView);
}

void drawShop() {
    window.setView(InterfaceView);

    updateShopUI();

    for (sf::Drawable*& elem : MenuShop::UIElements)
        window.draw(*elem);
    window.draw(MenuShop::itemSprite);
    if (shopSelectedItem != nullptr) {
        window.draw(MenuShop::itemCoins);
        window.draw(MenuShop::itemCoinsSprite);
        window.draw(MenuShop::itemStats);
    }

    window.draw(inventoryInterface::coinSprite);
    window.draw(*inventoryInterface::coinSlot.amountText);

    window.setView(ShopStockView);
    sf::Transform viewTransform = sf::Transform::Identity;
    viewTransform = viewTransform.scale(1, 1 / MenuShop::itemsViewSizeY);
    for (Item*& item : curShop->soldItems.items) {
        if (MenuShop::slotsElements[item->id].isInitialized) {
            window.draw(*MenuShop::slotsElements[item->id].background, viewTransform);
            if (item->amount >= 1) window.draw(*MenuShop::slotsElements[item->id].amountText, viewTransform);
            window.draw(*MenuShop::slotsElements[item->id].priceText, viewTransform);
        }
        window.draw(*item, viewTransform);
    }

    window.setView(ShopPlayerInvView);
    viewTransform = sf::Transform::Identity;
    viewTransform = viewTransform.scale(1, 1 / MenuShop::playerInvViewSizeY);
    for (Item*& item : player.inventory.items) {
        if (MenuShop::playerSlotsElements[item->id].isInitialized) {
            window.draw(*MenuShop::playerSlotsElements[item->id].background, viewTransform);
            if (item->amount >= 1) window.draw(*MenuShop::playerSlotsElements[item->id].amountText, viewTransform);
            window.draw(*MenuShop::playerSlotsElements[item->id].priceText, viewTransform);
        }
        window.draw(*item, viewTransform);
    }

    window.setView(HUDView);
}

void drawUpgradeInterface() {
    window.setView(InterfaceView);

    updateUpgradeInterfaceUI();
    {
        using namespace upgradeInterface;

        for (sf::Drawable*& elem : UIElements)
            window.draw(*elem);

        float listBGVal = 128 * std::pow(std::sin(GameClock->getElapsedTime().asSeconds()), 2);
        if (isChoosingComponent) {
            switch (compType) {
            case 0:
                listBG.sprite.setColor(sf::Color(0, 0, listBGVal));
                break;
            case 1:
                listBG.sprite.setColor(sf::Color(listBGVal, listBGVal, 0));
                break;
            case 2:
                listBG.sprite.setColor(sf::Color(0, listBGVal, 0));
                break;
            case 3:
                listBG.sprite.setColor(sf::Color(listBGVal, 0, 0));
                break;
            }
            for (sf::Drawable*& elem : choiceUIElements)
                window.draw(*elem);

            for (sf::Drawable* elem : compUpgBtns[compType])
                window.draw(*elem);
            for (sf::Drawable* elem : compUpgCosts[compType])
                window.draw(*elem);
            for (sf::Drawable* elem : compUpgStats[compType])
                window.draw(*elem);
            for (sf::Drawable* elem : compUpgCount[compType])
                window.draw(*elem);
        }
    }

    window.setView(HUDView);
}
//==============================================================================================



//============================================================================================== UI UPDATERS/CREATORS
void updateInventoryUI() {
    {
        using namespace inventoryInterface;

        if (doInventoryUpdate[inventoryPage::Items]) {
            int slotNumber = 0;
            for (Item*& item : player.inventory.items) {
                Item* drawnItem = item;
                drawnItem->animation->setScale(0.75, 0.75);

                float itemX = (slotNumber % 6) * 200 + itemListBG.getPosition().x + 50;
                float itemY = (slotNumber / 6) * 200 + itemListBG.getPosition().y + 50;

                if (!itemSlotsElements[drawnItem->id].isInitialized) itemSlotsElements[drawnItem->id].init();

                sf::Sprite& slotBG = *itemSlotsElements[drawnItem->id].background;
                slotBG.setPosition(itemX, itemY);
                slotBG.setScale(0.5, 0.5);
                slotBG.setTexture(Textures::ItemPanel);

                drawnItem->setPosition(itemX, itemY);

                PlacedText& itemAmountText = *itemSlotsElements[drawnItem->id].amountText;
                itemAmountText.setCharacterSize(20);
                itemAmountText.setString(std::to_string(drawnItem->amount));
                itemAmountText.setPosition(sf::Vector2f(itemX + slotBG.getGlobalBounds().width,
                    itemY + slotBG.getGlobalBounds().height));

                slotNumber++;
            }

            coinSprite.setScale(1, 1);
            coinSprite.setPosition(backButton.hitbox.getRight() + 200, backButton.hitbox.getPosition().y + 30);
            coinSlot.background->setTexture(Textures::INVISIBLE);
            coinSlot.amountText->setString(std::to_string(player.inventory.money));
            coinSlot.amountText->setCharacterSize(40);
            coinSlot.amountText->setPosition(coinSprite.getPosition().x + coinSprite.getGlobalSize().x + 50,
                coinSprite.getPosition().y + coinSprite.getGlobalSize().y / 2 - 20);

            doInventoryUpdate[inventoryPage::Items] = false;
        }
        statsHPRegenText.setString("Health regen: " + floatToString(player.HealthRecovery));
        statsMPRegenText.setString("Mana regen: " + floatToString(player.ManaRecovery));
        statsArmorText.setString("Armor: " + floatToString(player.Armor.cur));
        statsCompletedLevelsText.setString("Completed Levels: " + std::to_string(completedLevels));
        statsCurLevelsText.setString("Current Level: " + std::to_string(curLevel));
    }
}

void updateShopUI() {
    int slotNumber = 0;
    for (Item*& drawnItem : curShop->soldItems.items) {
        drawnItem->animation->setScale(0.5, 0.5);

        float itemX = (slotNumber % 5) * 200 + 30;
        float itemY = (slotNumber / 5) * 200 + 20;

        if (!MenuShop::slotsElements[drawnItem->id].isInitialized) MenuShop::slotsElements[drawnItem->id].init();

        sf::Sprite& slotBG = *MenuShop::slotsElements[drawnItem->id].background;
        slotBG.setPosition(itemX, itemY);
        slotBG.setTexture(Textures::ItemPanel, true);
        slotBG.setScale(0.333, 0.333);

        drawnItem->setPosition(itemX, itemY);

        PlacedText& itemAmountText = *MenuShop::slotsElements[drawnItem->id].amountText;
        itemAmountText.setCharacterSize(20);
        itemAmountText.setString(std::to_string(drawnItem->amount));
        itemAmountText.setPosition(sf::Vector2f(itemX + slotBG.getGlobalBounds().width,
            itemY + slotBG.getGlobalBounds().height));

        PlacedText& itemPriceText = *MenuShop::slotsElements[drawnItem->id].priceText;
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
        if (!MenuShop::playerSlotsElements[drawnItem->id].isInitialized) MenuShop::playerSlotsElements[drawnItem->id].init();

        sf::Sprite& slotBG = *MenuShop::playerSlotsElements[drawnItem->id].background;
        slotBG.setPosition(itemX, itemY);
        slotBG.setTexture(Textures::ItemPanel, true);
        slotBG.setScale(0.333, 0.333);

        drawnItem->setPosition(itemX, itemY);

        PlacedText& playerAmountText = *MenuShop::playerSlotsElements[drawnItem->id].amountText;
        playerAmountText.setCharacterSize(20);
        playerAmountText.setString(std::to_string(drawnItem->amount));
        playerAmountText.setPosition(sf::Vector2f(itemX + slotBG.getGlobalBounds().width,
            itemY + slotBG.getGlobalBounds().height));

        PlacedText& playerPriceText = *MenuShop::playerSlotsElements[drawnItem->id].priceText;
        playerPriceText.setCharacterSize(20);
        playerPriceText.setString(std::to_string(curShop->itemPrices[drawnItem->id]) + " C.");
        playerPriceText.setPosition(sf::Vector2f(itemX - slotBG.getGlobalBounds().width / 10,
            itemY + slotBG.getGlobalBounds().height));

        slotNumber++;
    }
    {
        using namespace inventoryInterface;
        coinSprite.setScale(0.75, 0.75);
        coinSprite.setPosition(MenuShop::playerCoinsText.getPosition().x + MenuShop::playerCoinsText.getGlobalBounds().width + 80,
                               MenuShop::playerCoinsText.getPosition().y - coinSprite.getGlobalSize().y / 3);
        coinSlot.amountText->setString(std::to_string(player.inventory.money));
        coinSlot.amountText->setCharacterSize(40);
        coinSlot.amountText->setPosition(coinSprite.getPosition().x + coinSprite.getGlobalSize().x - 120,
                                         MenuShop::playerCoinsText.getPosition().y);
    }
}

void updateUpgradeInterfaceUI() {
    upgradeInterface::playerCoinAmount.setString(FontString("You have: " + std::to_string(player.inventory.money),
        50, sf::Color(200, 200, 200)));
    inventoryInterface::coinSprite.setScale(0.5, 0.5);
    inventoryInterface::coinSprite.setPosition(upgradeInterface::playerCoinAmount.getRight() + sf::Vector2f(20, -50));
}

void createSlotRects() {
    {
        using namespace inventoryInterface;
        for (int i = 0; i < itemSlotsRects.size(); i++)
            DeletePointerFromVector(itemSlotsRects, i--);
        itemSlotsRects.resize(ItemID::ItemCount);

        int slotNumber = 0;
        for (Item*& item : player.inventory.items) {
            float itemX = (slotNumber % 6) * 200 + itemListBG.getPosition().x + 50;
            float itemY = (slotNumber / 6) * 200 + itemListBG.getPosition().y + 50;

            sf::FloatRect* itemActivationRect = new sf::FloatRect(itemX, itemY,
                Textures::ItemPanel.getSize().x / 2,
                Textures::ItemPanel.getSize().y / 2);
            itemSlotsRects[item->id] = itemActivationRect;

            slotNumber++;
        }
    }
}

void createShopSlotsRects() {
    for (int i = 0; i < MenuShop::slotsRects.size(); i++)
        DeletePointerFromVector(MenuShop::slotsRects, i--);
    MenuShop::slotsRects.resize(ItemID::ItemCount);

    int slotNumber = 0;
    for (Item*& item : curShop->soldItems.items) {
        float itemX = ((slotNumber % 5) * 200 + 30);
        float itemY = ((slotNumber / 5) * 200 + 20) / MenuShop::itemsViewSizeY;

        sf::FloatRect* itemActivationRect = new sf::FloatRect(itemX, itemY,
            Textures::ItemPanel.getSize().x * 0.4,
            Textures::ItemPanel.getSize().y * 0.4 / MenuShop::itemsViewSizeY);
        MenuShop::slotsRects[item->id] = itemActivationRect;

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
                }
                else if (ClientFuncRun) {
                    MySocket.send(SendPacket);
                }
                SendPacket.clear();
                mutex.unlock();
            }
        }
        else if (EscapeMenuActivated) {
            EscapeButton.isActivated(event);
            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape) {
                EscapeMenuActivated = !EscapeMenuActivated;
            }
        }
        else if (isDrawInventory) {
            inventoryHandler(event);
        }
        else if (isDrawShop) {
            shopHandler(event);
        }
        else if (isDrawUpgradeInterface) {
            upgradeInterfaceHandler(event);
        }
        else {
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
                    }
                    else {
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
                    inventoryInterface::inventoryFrame.show();
                    inventoryInterface::doInventoryUpdate[inventoryPage::Items] = true;
                }
                if (sf::Keyboard::Num1 <= event.key.code && event.key.code <= sf::Keyboard::Num3) {
                    if (!MiniMapActivated) {
                        CurWeapon = event.key.code - sf::Keyboard::Num1;
                        player.ChangeWeapon(arsenal[CurWeapon.cur]);

                        std::string reloadStr = player.CurWeapon->Name + " is out of ammo!";
                        ReloadWeaponText.setString(reloadStr);
                        ReloadWeaponText.setCenter(sf::Vector2f(scw / 2, sch / 4));
                    }
                }
            }
            if (event.type == sf::Event::MouseWheelMoved) {
                if (MiniMapActivated) {
                    if (event.mouseWheel.delta < 0) {
                        MiniMapView.zoom(1.1f);
                        MiniMapZoom *= 1.1f;
                    }
                    else {
                        MiniMapView.zoom(1.f / 1.1f);
                        MiniMapZoom /= 1.1f;
                    }
                }
            }

            for (Interactable*& x : InteractibeStuff) {
                if (x->CanBeActivated(player.hitbox)) {
                    x->isActivated(player.hitbox, event);
                    break;
                }
            }

            for (int i = 0; i < PickupStuff.size(); i++) {
                if (PickupStuff[i]->CanBeActivated(player.hitbox)) {
                    if (PickupStuff[i]->isActivated(player.hitbox, event)) {
                        player.addItem(PickupStuff[i]);
                        inventoryInterface::doInventoryUpdate[inventoryPage::Items] = true;
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
                        return;
                    }
                }
            }
            else if (CurLocation == &LabyrinthLocation) {
                if (event.type == sf::Event::KeyPressed) {
                    if (event.key.code == sf::Keyboard::Escape) {
                        EscapeMenuActivated = true;
                    }
                    else if (event.key.code == sf::Keyboard::H) {
                        player.hitbox.setCenter(size, size);
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
    {
        using namespace inventoryInterface;

        if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape) {
            if (upgradeInterface::isChoosingComponent) {
                upgradeInterface::isChoosingComponent = false;
                return;
            }
            backButton.buttonFunction();
            return;
        }

        if (!upgradeInterface::isChoosingComponent) {
            backButton.isActivated(event);
            itemsButton.isActivated(event);
            weaponsButton.isActivated(event);
            equipablesButton.isActivated(event);
            perksButton.isActivated(event);
            statsButton.isActivated(event);
        }

        bool isAnythingHovered = false;
        int itemTypeCount = 0;
        if (activePage == inventoryPage::Items) {
            for (Item*& item : player.inventory.items) {
                if (!itemSlotsRects.empty() && itemSlotsRects[item->id] != nullptr &&
                    itemSlotsRects[item->id]->contains(sf::Vector2f(sf::Mouse::getPosition()))) {
                    if (item->id != prevItemDescID) {
                        prevItemDescID = ItemID::ItemCount;
                        isItemDescDrawn = false;
                    }
                    isAnythingHovered = true;
                    if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Button::Right) {
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
            if (!isAnythingHovered) isItemDescDrawn = false;
        }
        if (activePage == inventoryPage::Arsenal) {
            for (int i = 0; i < upgradeInterface::compUpgBtns.size(); i++) {
                for (int j = 0; j < upgradeInterface::compUpgBtns[i].size(); j++)
                    upgradeInterface::compUpgBtns[i][j]->sprite.setColor(sf::Color(128, 128, 128));
            }
            upgradeInterfaceHandler(event);
        }
    }
}

void shopHandler(sf::Event& event) {
    if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape) {
        shopBackButton.buttonFunction();
        return;
    }
    if (shopBackButton.isActivated(event)) return;
    shopBuyButton.isActivated(event);

    if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Button::Left) {

        window.setView(ShopStockView);
        sf::Vector2f viewPos = window.mapPixelToCoords(sf::Mouse::getPosition());

        for (Item*& item : curShop->soldItems.items)
            if (!MenuShop::slotsRects.empty() && MenuShop::slotsRects[item->id] != nullptr &&
                MenuShop::slotsRects[item->id]->contains(viewPos)) {

                shopSelectedItem = item;
                MenuShop::itemCoins.setString(sf::String(std::to_string(curShop->itemPrices[item->id])));
                MenuShop::itemStats.setString(textWrap(itemDesc[item->id], 65));
                MenuShop::itemSprite.setTexture(*itemTexture[item->id], true);
            }
    }
    window.setView(InterfaceView);
}

void upgradeInterfaceHandler(sf::Event& event) {
    {
        using namespace upgradeInterface;

        if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape) {
            if (!isChoosingComponent)
                backButton.buttonFunction();
            else isChoosingComponent = false;
            return;
        }

        if (!isChoosingComponent) {
            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Left) {
                switchGunLBtn.buttonFunction();
            }
            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Right) {
                switchGunRBtn.buttonFunction();
            }
            switchGunLBtn.isActivated(event);
            switchGunRBtn.isActivated(event);
            backButton.isActivated(event);
            generatorBtn.isActivated(event);
            formFactorBtn.isActivated(event);
            converterBtn.isActivated(event);
            targetingBtn.isActivated(event);
        }

        if (event.type == sf::Event::MouseButtonPressed &&
            event.mouseButton.button == sf::Mouse::Right && isChoosingComponent)
            isChoosingComponent = false;

        if (!isDrawInventory && isChoosingComponent) {
            for (RectButton*& btn : compUpgBtns[compType]) {
                if (btn->isActivated(event))
                    updateUpgradeShopStats();
            }
        }
    }
}
//==============================================================================================



//============================================================================================== LEVEL GENERATION FUNCTIONS
void setBox(Interactable*& box) {
    box->setAnimation(Textures::Box);
    box->setSize(105.f, 117.f);
    box->setFunction([](Interactable* i) {
        if (player.Mana.cur >= 20) {
            player.Mana -= 20.f;

            TempText* tempText = new TempText(sf::seconds(2.5f));
            tempText->setCharacterSize(50);
            tempText->setOutlineColor(sf::Color(120, 120, 120));
            tempText->setOutlineThickness(3);

            std::rand(); int r = 1 + std::rand() % 5;
            tempText->setString("Money + " + std::to_string(r));
            tempText->setFillColor(sf::Color(255, 170, 29));
            tempText->setCenter(scw / 2.f, sch / 2.f - 165.f);

            MessageText.push_back(tempText);
            player.inventory.money += r;
            inventoryInterface::doInventoryUpdate[inventoryPage::Items] = true;
            DeleteFromVector(listOfBox, i);
            DeleteFromVector(DrawableStuff, (sf::Drawable*)i);
            DeleteFromVector(InteractibeStuff, i);
            delete i;
        }
        else {
            TempText* tempText = new TempText(sf::seconds(2.5f));
            tempText->setCharacterSize(50);
            tempText->setOutlineColor(sf::Color::White);
            tempText->setOutlineThickness(3);
            tempText->setString("Not enough Mana: " + std::to_string((int)player.Mana.cur) + "/20");
            tempText->setFillColor(sf::Color(255, 0, 0));
            tempText->setCenter(scw / 2.f, sch / 2.f - 165.f);
            MessageText.push_back(tempText);
        }
        });
}

std::vector<sf::String> artifactText = {
    "Health limit +2",
    "Mana limit +1",
    "Health Recovery +0.4",
    "Mana Recovery +0.2"
};
std::vector<sf::Color> artifactColors = {
    sf::Color(250, 50, 50),
    sf::Color(50, 50, 250),
    sf::Color(250, 80, 80),
    sf::Color(80, 80, 250)
};
void setArtifact(Interactable*& artifact) {
    artifact->setAnimation(Textures::Architect, &Shaders::Architect);
    artifact->setSize(150.f, 150.f);
    artifact->setFunction([](Interactable* i) {
        TempText* tempText = new TempText(sf::seconds(2.5f));
        tempText->setCharacterSize(50);
        tempText->setOutlineColor(sf::Color::White);
        tempText->setOutlineThickness(3);

        std::rand(); int r = std::rand() % 4;
        tempText->setString(artifactText[r]);
        tempText->setFillColor(artifactColors[r]);
        switch (r) {
        case 0: player.Health.top += 2; break;
        case 1: player.Mana.top += 1; break;
        case 2: player.HealthRecovery += 0.4; break;
        case 3: player.ManaRecovery += 0.1; break;
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

void setFire(Interactable*& fire) {
    fire->setAnimation(Textures::Fire, &Shaders::Fire);
    fire->setSize(70.f, 70.f);
}

void LevelGenerate(int n, int m) {
    MiniMapView.zoom(1 / MiniMapZoom);
    MiniMapZoom = std::pow(1.1, -10);
    MiniMapView.zoom(MiniMapZoom);

    LabyrinthLocation.GenerateLocation(n, m, player.hitbox.getCenter() / float(size));

    portal.setCenter(player.hitbox.getCenter());
    puddle.setCenter(player.hitbox.getCenter() + sf::Vector2f(size, size));

    clearVectorOfPointer(listOfBox);
    for (int i = 0; i < 10; i++) {
        listOfBox.push_back(new Interactable());
        setBox(listOfBox[i]);
        do {
            listOfBox[i]->setPosition(sf::Vector2f(std::rand() % m, std::rand() % n) * (float)size +
                sf::Vector2f(std::rand() % int(size - listOfBox[i]->hitbox.getSize().x), std::rand() % int(size - listOfBox[i]->hitbox.getSize().y)));
        } while (!LabyrinthLocation.EnableTiles[(int)listOfBox[i]->hitbox.getPosition().y / size][(int)listOfBox[i]->hitbox.getPosition().x / size]);

        InteractibeStuff.push_back(listOfBox[i]);
        DrawableStuff.push_back(listOfBox[i]);
    }

    clearVectorOfPointer(listOfArtifact);
    for (int i = 0; i < 10; i++) {
        listOfArtifact.push_back(new Interactable());
        setArtifact(listOfArtifact[i]);
        do {
            listOfArtifact[i]->setPosition(sf::Vector2f(std::rand() % m, std::rand() % n) * (float)size +
                sf::Vector2f(std::rand() % (size - Textures::Architect.getSize().x / 4), std::rand() % (size - Textures::Architect.getSize().y / 4)));
        } while (!LabyrinthLocation.EnableTiles[(int)listOfArtifact[i]->hitbox.getPosition().y / size][(int)listOfArtifact[i]->hitbox.getPosition().x / size]);

        InteractibeStuff.push_back(listOfArtifact[i]);
        DrawableStuff.push_back(listOfArtifact[i]);
    }

    clearVectorOfPointer(listOfFire);
    for (int i = 0; i < 2; i++) {
        listOfFire.push_back(new Interactable());
        setFire(listOfFire[i]);
        do {
            listOfFire[i]->setPosition(sf::Vector2f(std::rand() % m, std::rand() % n) * (float)size +
                sf::Vector2f(std::rand() % (size - Textures::Fire.getSize().x / 4), std::rand() % (size - Textures::Fire.getSize().y / 4)));
        } while (!LabyrinthLocation.EnableTiles[(int)listOfFire[i]->hitbox.getPosition().y / size][(int)listOfFire[i]->hitbox.getPosition().x / size]);

        InteractibeStuff.push_back(listOfFire[i]);
        DrawableStuff.push_back(listOfFire[i]);
    }

    clearVectorOfPointer(Enemies);
    int amountOfEveryEnemiesOnLevel = curLevel > completedLevels ? 4 : 2;
    for (int i = 0; i < amountOfEveryEnemiesOnLevel; i++) {
        Enemies.push_back(new DistortedScientist());
        Enemies.push_back(new Distorted());
    }

    for (int i = 0; i < Enemies.size(); i++) {
        do {
            Enemies[i]->hitbox.setPosition(sf::Vector2f((std::rand() % m) + 0.5f, (std::rand() % n) + 0.5f) * (float)size);
        } while (!LabyrinthLocation.EnableTiles[(int)Enemies[i]->hitbox.getPosition().y / size][(int)Enemies[i]->hitbox.getPosition().x / size] ||
            distance(Enemies[i]->hitbox.getPosition(), player.hitbox.getCenter()) < size * 3);
    }
}

void LoadMainMenu() {
    CurLocation = &MainMenuLocation;

    player.hitbox.setCenter(3.5f * size, 2.5f * size);
    FindAllWaysTo(CurLocation, player.hitbox.getCenter(), TheWayToPlayer);
    player.ChangeWeapon(arsenal[CurWeapon.cur]);

    portal.setPosition(1612.5, 1545);
    puddle.setPosition(1012.5, 1545);

    sf::Vector2f PlayerPos = player.hitbox.getCenter() / (float)size;
    CurLocation->FindEnableTilesFrom(PlayerPos);

    portal.setFunction([](Interactable* i) {
        clearVectorOfPointer(PickupStuff);
        clearVectorOfPointer(Bullets);

        DrawableStuff.clear();
        InterfaceStuff.clear();
        InteractibeStuff.clear();

        player.hitbox.setCenter(sf::Vector2f((START_M / 2 + 0.5f) * size, (START_N / 2 + 0.5f) * size));

        MiniMapActivated = false;
        EscapeMenuActivated = false;

        MiniMapView.setViewport(sf::FloatRect(0.f, 0.f, 0.25f, 0.25f));
        MiniMapView.setCenter(player.hitbox.getCenter() * ScaleParam);

        Musics::MainMenu.pause();
        if (Musics::Fight1.getStatus() != sf::Music::Playing && Musics::Fight2.getStatus() != sf::Music::Playing) {
            Musics::Fight1.play();
        }
        if (CurLocation != &LabyrinthLocation) {
            CurLocation = &LabyrinthLocation;
        }
        else {
            completedLevels = std::max(curLevel, completedLevels);
            curLevel++;
        }
        LevelGenerate(START_N + curLevel * 2, START_M + curLevel);
        FindAllWaysTo(CurLocation, player.hitbox.getCenter(), TheWayToPlayer);

        DrawableStuff.push_back(&player);
        DrawableStuff.push_back(&portal);
        DrawableStuff.push_back(&puddle);
        for (Enemy*& enemy : Enemies) {
            DrawableStuff.push_back(enemy);
        }

        InterfaceStuff.push_back(&HUDFrame);
        for (int i = 0; i < WeaponNameTexts.size(); i++)
            InterfaceStuff.push_back(WeaponNameTexts[i]);
        InterfaceStuff.push_back(&chat);

        InteractibeStuff.push_back(&puddle);
        saveGame();
        });

    puddle.setFunction([](Interactable* i) {
        player.getDamage(5.f);
        });

    shopSector.setFunction([](Interactable* i) {
        isDrawShop = true;
        MenuShop::playerCoinsText.setString("You have:");
        MenuShop::NPCText.setString(textWrap("Hello! Welcome to the \"We are literally standing near a phenomenon beyond our"
            " entire plane of existence in a literal motherfucking sense, that we have no fucking clue on"
            " how it fucking works why did we decide it was a good idea to station a shop exactly here\?!\?\?!\?!\?\" shop!", 94));
        createShopSlotsRects();
        });

    upgradeSector.setFunction([](Interactable* i) {
        isDrawUpgradeInterface = true;
        openUpgradeShop();
        });

    // Set cameras
    GameView.setCenter(player.hitbox.getCenter());
    MiniMapView.setCenter(player.hitbox.getCenter() * ScaleParam);
    HUDView.setCenter({ scw / 2.f, sch / 2.f });

    Musics::Fight1.stop();
    Musics::Fight2.stop();
    Musics::MainMenu.play();

    DrawableStuff.clear();
    DrawableStuff.push_back(&player);
    DrawableStuff.push_back(&portal);
    DrawableStuff.push_back(&puddle);

    InterfaceStuff.clear();
    InterfaceStuff.push_back(&chat);
    InterfaceStuff.push_back(&HUDFrame);

    InteractibeStuff.clear();
    InteractibeStuff.push_back(&portal);
    InteractibeStuff.push_back(&puddle);
    InteractibeStuff.push_back(&shopSector);
    InteractibeStuff.push_back(&upgradeSector);

    Item* newItem = new Item(ItemID::regenDrug, 1);
    newItem->setAnimation(*itemTexture[ItemID::regenDrug]);
    PickupStuff.push_back(newItem);
    DrawableStuff.push_back(PickupStuff[0]);
    PickupStuff[0]->dropTo(player.hitbox.getCenter() + sf::Vector2f(100, 100));

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
}
//==============================================================================================



//============================================================================================== GAME STATE FUNCTIONS
void updateBullets() {
    for (int i = 0; i < Bullets.size(); i++) {
        if (Bullets[i]->penetration < 0 || Bullets[i]->todel) {
            DeletePointerFromVector(Bullets, i--);
        }
        else {
            Bullets[i]->move(CurLocation);
            if (!faction::friends(Bullets[i]->fromWho, player.faction) && player.hitbox.intersect(Bullets[i]->hitbox)) {
                player.getDamage(Bullets[i]->damage);
                Bullets[i]->penetration--;
            }
            else {
                for (Enemy*& enemy : Enemies) {
                    if (!faction::friends(Bullets[i]->fromWho, enemy->faction) && enemy->hitbox.intersect(Bullets[i]->hitbox)) {
                        enemy->getDamage(Bullets[i]->damage);
                        Bullets[i]->penetration--;
                        TempText* tempText = new TempText(sf::seconds(1.5f));
                        tempText->setCharacterSize(30);
                        tempText->setOutlineColor(sf::Color::White);
                        tempText->setOutlineThickness(3);
                        tempText->setString(std::to_string(int(Bullets[i]->damage)));
                        tempText->setFillColor(sf::Color(250, 50, 50, 200));
                        tempText->setCenter(enemy->hitbox.getPosition());
                        DamageText.push_back(tempText);
                        break;
                    }
                }
            }
        }
    }
}

void updateEnemies() {
    for (int i = 0; i < Enemies.size(); i++) {
        if (Enemies[i]->Health.toBottom() == 0) {
            if (Enemies[i]->dropInventory) {
                for (Item*& item : Enemies[i]->inventory.items) {
                    item->dropTo(Enemies[i]->hitbox.getPosition());

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
        }
        else {
            sf::Vector2f bullet_path;
            for (int j = 0; j < Bullets.size(); j++) {
                if (!faction::friends(Bullets[j]->fromWho, Enemies[i]->faction)) {
                    bullet_path = Bullets[j]->hitbox.getCenter() + distance(Bullets[j]->hitbox.getCenter(), Enemies[i]->hitbox.getCenter()) * normalize(Bullets[j]->Velocity);
                    if (Enemies[i]->hitbox.contains(bullet_path)) {
                        sf::Vector2f toRotate90 = Bullets[j]->hitbox.getCenter() - Enemies[i]->hitbox.getCenter();
                        Enemies[i]->shift(sf::Vector2f(toRotate90.y, -toRotate90.x) * (random(i, i) < 0.5f ? 1.f : -1.f));
                        break;
                    }
                }
            }
            Enemies[i]->move(CurLocation);
            Enemies[i]->UpdateState();
            Enemies[i]->CurWeapon->lock = false;
            Enemies[i]->CurWeapon->Shoot(Enemies[i]->hitbox, player.hitbox.getCenter(), Enemies[i]->faction);
            Enemies[i]->CurWeapon->Reload(Enemies[i]->Mana);
        }
    }
}

void updateUpgradeShopStats() {
    {
        using namespace upgradeInterface;

        weaponDescText.setString(player.CurWeapon->Name + '\n');
        weaponDescText.addString(weaponDesc[player.CurWeapon->Name] + '\n');
        weaponDescText.addString("\nStats:");

        weaponDescText.addString("\nMana storage: " + floatToString(player.CurWeapon->MaxManaStorage));
        weaponDescText.addString("\t\t\tReload Speed: " + floatToString(player.CurWeapon->ReloadSpeed.getStat()) + " mana/sec");
        weaponDescText.addString("\n\nTime To Holster: " + floatToString(player.CurWeapon->TimeToHolster.getStat().asSeconds()) + " sec");
        weaponDescText.addString("\t\t\tTime To Dispatch: " + floatToString(player.CurWeapon->TimeToDispatch.getStat().asSeconds()) + " sec");
        weaponDescText.addString("\n\nDamage: " + floatToString(player.CurWeapon->ManaCostOfBullet));
        if (player.CurWeapon->Multishot != 1) {
            weaponDescText.addString("\t\t\tBullet per shot: " + std::to_string(player.CurWeapon->Multishot));
        }
        weaponDescText.addString("\t\t\tRate of fire: " + floatToString(1 / player.CurWeapon->FireRate.getStat().asSeconds()) + " shots/sec");
        weaponDescText.addString("\n\nVelocity of Bullet: " + floatToString(player.CurWeapon->BulletVelocity));
        weaponDescText.addString("\t\t\tScatter: " + floatToString(player.CurWeapon->Scatter) + " deg");

        if (player.CurWeapon == &pistol)
            weaponImg.setTexture(Textures::PH_Pistol, true);
        if (player.CurWeapon == &shotgun)
            weaponImg.setTexture(Textures::PH_Shotgun, true);
        if (player.CurWeapon == &rifle)
            weaponImg.setTexture(Textures::PH_Rifle, true);
    }
}

void openUpgradeShop() {
    {
        using namespace upgradeInterface;

        updateUpgradeShopStats();

        int btnTextSize = 24;
        sf::Vector2f costOffset = sf::Vector2f(300, -25);
        sf::Vector2f countOffset = sf::Vector2f(0, -24);
        sf::Vector2f statOffset = sf::Vector2f(0, 0);

        for (int i = 0; i < 4; i++) {
            clearVectorOfPointer(compUpgBtns[i]);
            clearVectorOfPointer(compUpgCosts[i]);
            clearVectorOfPointer(compUpgStats[i]);
            clearVectorOfPointer(compUpgCount[i]);
        }

        compUpgBtns[0].push_back(new RectButton(
                FontString("Mana storage", btnTextSize),
                []() {
                    upgradeStat(50,
                                &player.CurWeapon->MaxManaStorage,
                                compUpgCosts[0][0],
                                compUpgStats[0][0],
                                compUpgCount[0][0]);

                    if (!pistol.MaxManaStorage.maxed()) pistol.ManaStorage.top = pistol.MaxManaStorage;
                },
                Textures::BlueButton, Textures::BlueButtonPushed
            )
        );
        compUpgBtns[0][0]->setRect(listBG.getPosition().x + 50, listBG.getPosition().y + 50, 250, 100);

        compUpgCosts[0].push_back(new PlacedText());
        compUpgStats[0].push_back(new PlacedText());
        compUpgCount[0].push_back(new PlacedText());
        compUpgCosts[0][0]->setPosition(compUpgBtns[0][0]->getCenter() + costOffset);
        compUpgStats[0][0]->setPosition(compUpgBtns[0][0]->getRightTop() + statOffset);
        compUpgCount[0][0]->setPosition(compUpgBtns[0][0]->getRightBottom() + countOffset);
        updateCostsText(compUpgCosts[0][0], &player.CurWeapon->MaxManaStorage, 50);
        updateStatsText(compUpgStats[0][0], &player.CurWeapon->MaxManaStorage);
        updateCountText(compUpgCount[0][0], &player.CurWeapon->MaxManaStorage);

        compUpgBtns[0].push_back(new RectButton(
                FontString("Reload Speed", btnTextSize),
                []() {
                    upgradeStat(70,
                                &player.CurWeapon->ReloadSpeed,
                                compUpgCosts[0][1],
                                compUpgStats[0][1],
                                compUpgCount[0][1]);
                },
                Textures::GreenButton, Textures::GreenButtonPushed
            )
        );
        compUpgBtns[0][1]->setRect(compUpgBtns[0][0]->getPosition() + sf::Vector2f(0, 150), sf::Vector2f(250, 100));

        compUpgCosts[0].push_back(new PlacedText());
        compUpgStats[0].push_back(new PlacedText());
        compUpgCount[0].push_back(new PlacedText());
        compUpgCosts[0][1]->setPosition( compUpgBtns[0][1]->getCenter() + costOffset);
        compUpgStats[0][1]->setPosition( compUpgBtns[0][1]->getRightTop() + statOffset);
        compUpgCount[0][1]->setPosition( compUpgBtns[0][1]->getRightBottom() + countOffset);
        updateCostsText(compUpgCosts[0][1], &player.CurWeapon->ReloadSpeed, 70);
        updateStatsText(compUpgStats[0][1], &player.CurWeapon->ReloadSpeed);
        updateCountText(compUpgCount[0][1], &player.CurWeapon->ReloadSpeed);

        compUpgBtns[1].push_back(new RectButton(
                FontString("Time To\nHolster", btnTextSize),
                []() {
                    upgradeStat(25,
                                &player.CurWeapon->TimeToHolster,
                                compUpgCosts[1][0],
                                compUpgStats[1][0],
                                compUpgCount[1][0]);
                },
                Textures::BlueButton, Textures::BlueButtonPushed
            )
        );
        compUpgBtns[1][0]->setRect(listBG.getPosition().x + 50, listBG.getPosition().y + 50, 250, 100);

        compUpgCosts[1].push_back(new PlacedText());
        compUpgStats[1].push_back(new PlacedText());
        compUpgCount[1].push_back(new PlacedText());
        compUpgCosts[1][0]->setPosition(compUpgBtns[1][0]->getCenter() + costOffset);
        compUpgStats[1][0]->setPosition(compUpgBtns[1][0]->getRightTop() + statOffset);
        compUpgCount[1][0]->setPosition(compUpgBtns[1][0]->getRightBottom() + countOffset);
        updateCostsText(compUpgCosts[1][0], &player.CurWeapon->TimeToHolster, 25);
        updateStatsText(compUpgStats[1][0], &player.CurWeapon->TimeToHolster);
        updateCountText(compUpgCount[1][0], &player.CurWeapon->TimeToHolster);

        compUpgBtns[1].push_back(new RectButton(
                FontString("Time To\nDispatch", btnTextSize),
                []() {
                    upgradeStat(25,
                                &player.CurWeapon->TimeToDispatch,
                                compUpgCosts[1][1],
                                compUpgStats[1][1],
                                compUpgCount[1][1]);
                },
                Textures::GreenButton, Textures::GreenButtonPushed
            )
        );
        compUpgBtns[1][1]->setRect(compUpgBtns[1][0]->getPosition() + sf::Vector2f(0, 150), sf::Vector2f(250, 100));

        compUpgCosts[1].push_back(new PlacedText());
        compUpgStats[1].push_back(new PlacedText());
        compUpgCount[1].push_back(new PlacedText());
        compUpgCosts[1][1]->setPosition(compUpgBtns[1][1]->getCenter() + costOffset);
        compUpgStats[1][1]->setPosition(compUpgBtns[1][1]->getRightTop() + statOffset);
        compUpgCount[1][1]->setPosition(compUpgBtns[1][1]->getRightBottom() + countOffset);
        updateCostsText(compUpgCosts[1][1], &player.CurWeapon->TimeToDispatch, 25);
        updateStatsText(compUpgStats[1][1], &player.CurWeapon->TimeToDispatch);
        updateCountText(compUpgCount[1][1], &player.CurWeapon->TimeToDispatch);

        compUpgBtns[2].push_back(new RectButton(
                FontString("Fire Rate", btnTextSize),
                []() {
                    upgradeStat(35,
                                &player.CurWeapon->FireRate,
                                compUpgCosts[2][0],
                                compUpgStats[2][0],
                                compUpgCount[2][0]);
                },
                Textures::BlueButton, Textures::BlueButtonPushed
            )
        );
        compUpgBtns[2][0]->setRect(listBG.getPosition().x + 50, listBG.getPosition().y + 50, 250, 100);

        compUpgCosts[2].push_back(new PlacedText());
        compUpgStats[2].push_back(new PlacedText());
        compUpgCount[2].push_back(new PlacedText());
        compUpgCosts[2][0]->setPosition(compUpgBtns[2][0]->getCenter() + costOffset);
        compUpgStats[2][0]->setPosition(compUpgBtns[2][0]->getRightTop() + statOffset);
        compUpgCount[2][0]->setPosition(compUpgBtns[2][0]->getRightBottom() + countOffset);
        updateCostsText(compUpgCosts[2][0], &player.CurWeapon->FireRate, 35);
        updateStatsText(compUpgStats[2][0], &player.CurWeapon->FireRate);
        updateCountText(compUpgCount[2][0], &player.CurWeapon->FireRate);

        compUpgBtns[2].push_back(new RectButton(
                FontString("Mana Cost", btnTextSize),
                []() {
                    upgradeStat(80,
                                &player.CurWeapon->ManaCostOfBullet,
                                compUpgCosts[2][1],
                                compUpgStats[2][1],
                                compUpgCount[2][1]);
                },
                Textures::GreenButton, Textures::GreenButtonPushed
            )
        );
        compUpgBtns[2][1]->setRect(compUpgBtns[2][0]->getPosition() + sf::Vector2f(0, 150), sf::Vector2f(250, 100));

        compUpgCosts[2].push_back(new PlacedText());
        compUpgStats[2].push_back(new PlacedText());
        compUpgCount[2].push_back(new PlacedText());
        compUpgCosts[2][1]->setPosition(compUpgBtns[2][1]->getCenter() + costOffset);
        compUpgStats[2][1]->setPosition(compUpgBtns[2][1]->getRightTop() + statOffset);
        compUpgCount[2][1]->setPosition(compUpgBtns[2][1]->getRightBottom() + countOffset);
        updateCostsText(compUpgCosts[2][1], &player.CurWeapon->ManaCostOfBullet, 80);
        updateStatsText(compUpgStats[2][1], &player.CurWeapon->ManaCostOfBullet);
        updateCountText(compUpgCount[2][1], &player.CurWeapon->ManaCostOfBullet);

        compUpgBtns[2].push_back(new RectButton(
                FontString("Multishot", btnTextSize),
                []() {
                    upgradeStat(80,
                                &player.CurWeapon->Multishot,
                                compUpgCosts[2][2],
                                compUpgStats[2][2],
                                compUpgCount[2][2]);
                },
                Textures::RedButton, Textures::RedButtonPushed
            )
        );
        compUpgBtns[2][2]->setRect(compUpgBtns[2][1]->getPosition() + sf::Vector2f(0, 150), sf::Vector2f(250, 100));

        compUpgCosts[2].push_back(new PlacedText());
        compUpgStats[2].push_back(new PlacedText());
        compUpgCount[2].push_back(new PlacedText());
        compUpgCosts[2][2]->setPosition(compUpgBtns[2][2]->getCenter() + costOffset);
        compUpgStats[2][2]->setPosition(compUpgBtns[2][2]->getRightTop() + statOffset);
        compUpgCount[2][2]->setPosition(compUpgBtns[2][2]->getRightBottom() + countOffset);
        updateCostsText(compUpgCosts[2][2], &player.CurWeapon->Multishot, 80);
        updateStatsText(compUpgStats[2][2], &player.CurWeapon->Multishot);
        updateCountText(compUpgCount[2][2], &player.CurWeapon->Multishot);

        compUpgBtns[3].push_back(new RectButton(
                FontString("Bullet Velocity", btnTextSize),
                []() {
                    upgradeStat(65,
                                &player.CurWeapon->BulletVelocity,
                                compUpgCosts[3][0],
                                compUpgStats[3][0],
                                compUpgCount[3][0]);
                },
                Textures::BlueButton, Textures::BlueButtonPushed
            )
        );
        compUpgBtns[3][0]->setRect(listBG.getPosition().x + 50, listBG.getPosition().y + 50, 250, 100);

        compUpgCosts[3].push_back(new PlacedText());
        compUpgStats[3].push_back(new PlacedText());
        compUpgCount[3].push_back(new PlacedText());
        compUpgCosts[3][0]->setPosition(compUpgBtns[3][0]->getCenter() + costOffset);
        compUpgStats[3][0]->setPosition(compUpgBtns[3][0]->getRightTop() + statOffset);
        compUpgCount[3][0]->setPosition(compUpgBtns[3][0]->getRightBottom() + countOffset);
        updateCostsText(compUpgCosts[3][0], &player.CurWeapon->BulletVelocity, 65);
        updateStatsText(compUpgStats[3][0], &player.CurWeapon->BulletVelocity);
        updateCountText(compUpgCount[3][0], &player.CurWeapon->BulletVelocity);

        compUpgBtns[3].push_back(new RectButton(
                FontString("Scatter", btnTextSize),
                []() {
                    upgradeStat(65,
                                &player.CurWeapon->Scatter,
                                compUpgCosts[3][1],
                                compUpgStats[3][1],
                                compUpgCount[3][1]);
                },
                Textures::GreenButton, Textures::GreenButtonPushed
            )
        );
        compUpgBtns[3][1]->setRect(compUpgBtns[3][0]->getPosition() + sf::Vector2f(0, 150), sf::Vector2f(250, 100));

        compUpgCosts[3].push_back(new PlacedText());
        compUpgStats[3].push_back(new PlacedText());
        compUpgCount[3].push_back(new PlacedText());
        compUpgCosts[3][1]->setPosition(compUpgBtns[3][1]->getCenter() + costOffset);
        compUpgStats[3][1]->setPosition(compUpgBtns[3][1]->getRightTop() + statOffset);
        compUpgCount[3][1]->setPosition(compUpgBtns[3][1]->getRightBottom() + countOffset);
        updateCostsText(compUpgCosts[3][1], &player.CurWeapon->Scatter, 65);
        updateStatsText(compUpgStats[3][1], &player.CurWeapon->Scatter);
        updateCountText(compUpgCount[3][1], &player.CurWeapon->Scatter);
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
        }
        else {
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
                    creature->getDamage(effectVec[i]->parameters[0]);
                    creature->HealthRecoveryActive = false;
                    effectVec[i]->active = true;
                }
                if (effectVec[i]->customTickClock->getElapsedTime() >= effectVec[i]->customTick) {
                    creature->getDamage(effectVec[i]->parameters[0]);
                    creature->HealthRecoveryActive = false;
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
    case Effects::Burn:
        owner.HealthRecoveryActive = true;
        owner.effectStacks[effect->type] -= 1;
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
    default:
        return false;
    }
}
//==============================================================================================



//============================================================================================== GAME GRAPHICS FUNCTIONS
void updateShaders() {
    sf::Vector2f uMouse(sf::Mouse::getPosition());
    float uTime = GameClock->getElapsedTime().asSeconds();
    sf::Vector2f uPlayerPosition(player.hitbox.getCenter() - GameView.getCenter() + GameView.getSize() / 2.f);

    Shaders::Flashlight.setUniform("uMouse", uMouse);
    Shaders::Flashlight.setUniform("uPlayerPosition", uPlayerPosition);

    Shaders::Portal.setUniform("uTime", uTime);

    Shaders::Architect.setUniform("uTime", uTime);

    Shaders::Distortion1.setUniform("uTime", uTime);
    Shaders::Distortion2.setUniform("uTime", uTime);

    Shaders::Bullet.setUniform("uTime", uTime);

    Shaders::Fire.setUniform("uTime", uTime);

    Shaders::WaveMix.setUniform("uTime", uTime);
}
//==============================================================================================



//============================================================================================== HELPER FUNCTIONS
bool CanSomethingBeActivated() {
    CurInteractable = nullptr;
    for (Interactable*& x : InteractibeStuff) {
        if (x->CanBeActivated(player.hitbox)) {
            CurInteractable = x;
            return true;
        }
    }
    return false;
}

template <class T>
void updateCostsText(PlacedText* costText, Upgradable<T>* stat, int cost) {
    if (stat->maxed()) costText->setString(FontString("Maxed out", 50, sf::Color::Green));
    else costText->setString(FontString("Cost of upgrade: " + std::to_string(cost * (1 + stat->curLevel)), 50));
}

template <class T>
void updateCountText(PlacedText* counterText, Upgradable<T>* stat) {
    if (stat->maxed()) counterText->setString(FontString(std::to_string(stat->maxLevel) + " / " +
                                                         std::to_string(stat->maxLevel), 24, sf::Color::Green));
    else counterText->setString(FontString(std::to_string(stat->curLevel + 1) + " / " +
                                           std::to_string(stat->maxLevel), 24));
}

template <class T>
void updateStatsText(PlacedText* statText, Upgradable<T>* stat) {
    if constexpr (std::is_same_v<T, sf::Time>) {
        if (stat->maxed()) statText->setString(FontString(floatToString(stat->getStat().asSeconds()), 24, sf::Color::Green));
        else statText->setString(FontString(floatToString(stat->getStat().asSeconds()) + " -> " +
                                            floatToString(stat->stats[stat->curLevel + 1].asSeconds()), 24));
    }
    else if constexpr (std::is_same_v<T, float>) {
        if (stat->maxed()) statText->setString(FontString(floatToString(stat->getStat()), 24, sf::Color::Green));
        else statText->setString(FontString(floatToString(stat->getStat()) + " -> " +
                                            floatToString(stat->stats[stat->curLevel + 1]), 24));
    }
    else {
        if (stat->maxed()) statText->setString(FontString(std::to_string(stat->getStat()), 24, sf::Color::Green));
        else statText->setString(FontString(std::to_string(stat->getStat()) + " -> " +
                                            std::to_string(stat->stats[stat->curLevel + 1]), 24));
    }
}

template <class T>
void upgradeStat(int cost, Upgradable<T>* stat, PlacedText* costText, PlacedText* statText, PlacedText* counterText) {
    if (!stat->maxed() && player.inventory.money >= cost * (1 + stat->curLevel)) {
        player.inventory.money -= cost * (1 + stat->curLevel);
        stat->upgrade(1);
    }
    if (costText != nullptr) updateCostsText(costText, stat, cost);
    if (statText != nullptr) updateStatsText(statText, stat);
    if (counterText != nullptr) updateCountText(counterText, stat);
}

void saveGame() {
    std::ofstream fileToSave("sources/saves/save.json");
    json j;
    j["Player"] = player;
    j["Pistol"] = pistol;
    j["Shotgun"] = shotgun;
    j["Rifle"] = rifle;
    fileToSave << j.dump(4);

    fileToSave.close();
}

void loadSave() {
    std::ifstream saveFile("sources/saves/save.json");
    if (!saveFile.is_open()) {
        std::rand();
        player.Name.setString("Employee " + std::to_string(1 + (size_t(std::rand()) * 8645) % 999));
    }
    else {
        nlohmann::json j = nlohmann::json::parse(saveFile);
        j.at("Player").get_to<Player>(player);
        j.at("Pistol").get_to<Pistol>(pistol);
        j.at("Shotgun").get_to<Shotgun>(shotgun);
        j.at("Rifle").get_to<Rifle>(rifle);
    }
    saveFile.close();
}
//==============================================================================================



//============================================================================================== MAIN LOOP
void MainLoop() {
    while (window.isOpen()) {

        if (player.Health.toBottom() == 0) {
            EscapeButton.buttonFunction();
        }

        updateEnemies();
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
                player.CurWeapon->Shoot(player.hitbox, window.mapPixelToCoords(sf::Mouse::getPosition()), player.faction);
            }
            updateBullets();

            sf::Event event;
            while (window.pollEvent(event)) {}

            if (HostFuncRun) {
                mutex.lock();
                SendPacket << pacetStates::PlayerPos;
                for (Player& x : ConnectedPlayers) {
                    SendPacket << x;
                }
                SendToClients(SendPacket);
                SendPacket.clear();
                mutex.unlock();
            }
        }
        else {
            if (!chat.inputted && !isDrawInventory && !isDrawShop) {
                player.move(CurLocation);
                GameView.setCenter(player.hitbox.getCenter() + static_cast<sf::Vector2f>((sf::Mouse::getPosition() - sf::Vector2i(scw, sch) / 2) / 8));
                FindAllWaysTo(CurLocation, player.hitbox.getCenter(), TheWayToPlayer);
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::R)) {
                    player.CurWeapon->HolsterAction();
                }
            }
            int wasBulletsSize = Bullets.size();

            if (player.CurWeapon != nullptr) {
                player.CurWeapon->Shoot(player.hitbox, window.mapPixelToCoords(sf::Mouse::getPosition()), player.faction);
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
                }
                else if (ClientFuncRun) {
                    MySocket.send(SendPacket);
                }
                SendPacket.clear();
                mutex.unlock();
            }

            updateBullets();

            if (HostFuncRun || ClientFuncRun) {
                ConnectedPlayers[ComputerID].hitbox.setPosition(player.hitbox.getCenter());
                mutex.lock();
                SendPacket << pacetStates::PlayerPos;
                if (HostFuncRun) {
                    for (Player& x : ConnectedPlayers) {
                        SendPacket << x;
                    }
                    SendToClients(SendPacket);
                }
                else if (ClientFuncRun) {
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

        if (puddle.hitbox.intersect(player.hitbox))
            applyEffect(player, new Effect(Effects::Heal, std::vector<float>{30.f}, sf::seconds(1.5f)));

        for (int i = 0; i < listOfFire.size(); i++) {
            if (listOfFire[i]->hitbox.intersect(player.hitbox))
                applyEffect(player, new Effect(Effects::Burn, std::vector<float>{5.f}, sf::seconds(5.f), sf::seconds(1.f)));
        }
        processEffects();

        if (CanSomethingBeActivated()) {
            if (!in(InterfaceStuff, static_cast<sf::Drawable*>(&XButtonSprite))) {
                InterfaceStuff.push_back(&XButtonSprite);
            }
        }
        else {
            DeleteFromVector(InterfaceStuff, static_cast<sf::Drawable*>(&XButtonSprite));
        }

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
        ConnectedPlayers[ConnectedPlayers.size() - 1].hitbox.setPosition(float(CurLocation->m) * size / 2, float(CurLocation->n) * size / 2);
        SendPacket << pacetStates::PlayersAmount << (sf::Int32)ConnectedPlayers.size() - 1;

        DrawableStuff.push_back(&(ConnectedPlayers[ConnectedPlayers.size() - 1]));

        for (int i = 0; i < ListOfPlayers.getLineCount(); i++) {
            SendPacket << pacetStates::PlayerConnect << ListOfPlayers[i];
        }

        SendPacket << pacetStates::Shooting << (sf::Int32)Bullets.size();
        std::cout << "bullets: " << Bullets.size() << "\n";
        for (int i = 0; i < Bullets.size(); i++) {
            SendPacket << *Bullets[i];
        }

        std::cout << "amount players = " << ConnectedPlayers.size() - 1 << '\n';
        SendPacket << pacetStates::SetPos;
        for (Player& x : ConnectedPlayers) {
            SendPacket << x;
        }

        if (client->send(SendPacket) == sf::Socket::Done) {
            std::cout << "SendPacket was sended\n";
        }
        else {
            std::cout << "SendPacket didn't sended\n";
        }

        SendPacket.clear();
        SendPacket << LabyrinthLocation;
        if (client->send(SendPacket) == sf::Socket::Done) {
            std::cout << "Labyrinth walls sended\n";
        }
        else {
            std::cout << "Labyrinth walls didn't sended\n";
        }
        SendPacket.clear();

        mutex.unlock();
    }
    else delete client;
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
            }
            else {
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
                        for (Player& x : ConnectedPlayers) {
                            ReceivePacket >> x;
                        }
                        player.hitbox.setCenter(ConnectedPlayers[ComputerID].hitbox.getPosition());
                        GameView.setCenter(player.hitbox.getCenter());
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
