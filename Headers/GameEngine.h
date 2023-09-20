#include "portal.h"
#include "player.h"
#include "client.h"
#include "chat.h"
#include "contextMenu.h"
#include "enemy.h"

//////////////////////////////////////////////////////////// Settings of the game
bool IsDrawMinimap   = true;
bool IsDrawInterface = true;
bool MiniMapHoldOnPlayer = true;

//////////////////////////////////////////////////////////// Stuff for work with system and screen
sf::ContextSettings settings;

sf::RenderWindow window(sf::VideoMode(scw, sch), "multigame", sf::Style::Fullscreen, settings);
float MiniMapZoom = 1.f;
bool MiniMapActivated, EscapeMenuActivated;
screens::screens screen = screens::MainRoom;
std::vector<sf::Drawable*> DrawableStuff, InterfaceStuff;
std::vector<Interactible*> InteractibeStuff;

//////////////////////////////////////////////////////////// DrawableStuff
sf::Sprite WallRect;

//////////////////////////////////////////////////////////// MiniMapStuff
sf::CircleShape MMPlayerCircle; // MM - MiniMap prefix
sf::RectangleShape MMPortalRect, MMBoxRect;

//////////////////////////////////////////////////////////// InterfaceStuff
Bar<float> ManaBar, HpBar;
Bar<int> AmmoBar;
PlacedText WeaponNameText;
sf::Sprite XButtonSprite;

//////////////////////////////////////////////////////////// Music
sf::Music MainMenuMusic;
sf::Music FightMusic1, FightMusic2;

//////////////////////////////////////////////////////////// Animation
Animation animation("sources/textures/scottpilgrim.png", 8, {108, 140}, sf::seconds(1));

//////////////////////////////////////////////////////////// Online tools
sf::TcpListener listener;
sf::Packet ReceivePacket, SendPacket;
std::vector<Client*> clients;
sf::SocketSelector selector;
str ClientState, IPOfHost, MyIP, PacetData;
Client MySocket; // this computer socket
sf::Int32 packetState, ComputerID;
sf::Mutex mutex;
bool ClientFuncRun, HostFuncRun;

//////////////////////////////////////////////////////////// Portal
Portal portal;

//////////////////////////////////////////////////////////// Box
sf::Sprite BoxRect;

//////////////////////////////////////////////////////////// Locations
Location* CurLocation = nullptr;
Location LabyrinthLocation, WaitingRoomLoaction, MainMenuLocation;

//////////////////////////////////////////////////////////// Players
Player player;
std::vector<Player> ConnectedPlayers;

sf::Shader MapShader;
sf::RenderStates MapStates(&MapShader);

//////////////////////////////////////////////////////////// Chat
Chat chat;

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

//////////////////////////////////////////////////////////// functions
void draw();
void drawWalls();
void drawMiniMap();
void drawIterface();
void LevelGenerate(int, int);
void LoadMainMenu();
void init();
void EventHandler();
void MainLoop();
bool IsSomeOneCanBeActivated();

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

//////////////////////////////////////////////////////////// Pannels
Panel  IPPanel          ("sources/textures/YellowPanel", "IP:");
Panel  ListOfPlayers    ("sources/textures/SteelFrame"        );

//////////////////////////////////////////////////////////// Buttons
Button HostButton("sources/textures/GreenPanel", "Host", [](){
    listener.listen(53000);
    selector.add(listener);
    ListOfPlayers.setWord(MyIP);
    Bullets.clear();
    LevelGenerate(START_N, START_M);
    ComputerID = 0;
    ConnectedPlayers.push_back(*(new Player()));
    HostFuncRun = true;
    HostTread.launch();
});

Button EscapeButton("sources/textures/RedPanel", "Exit", [](){
    if (HostFuncRun) {
        mutex.lock();
        SendPacket << sf::Int32(pacetStates::disconnect);
        SendToClients(SendPacket);
        SendPacket.clear();
        mutex.unlock();
        clients.clear();
        selector.clear();
        listener.close();
        HostFuncRun = false;
        ConnectedPlayers.clear();
    } else if (ClientFuncRun)
        SelfDisconnect();
    screen = screens::MainRoom;
    EscapeMenuActivated = false;
    ListOfPlayers.clear();
    Bullets.clear();
    for (int i = 0; i < Enemies.size(); i++)
        delete Enemies[i];
    Enemies.clear();
    LoadMainMenu();
});

//////////////////////////////////////////////////////////// functions
void draw() {
    window.clear(sf::Color::Black);
    drawWalls();

    for (int i = 0; i < CurLocation->objects.size(); i++) {
        if (CurLocation->objects[i].id == Tiles::portal) {
            portal.setPosition(CurLocation->objects[i].pos);
            window.draw(portal, MapStates);
        }
        if (CurLocation->objects[i].id == Tiles::box) {
            BoxRect.setPosition(CurLocation->objects[i].pos);
            window.draw(BoxRect, MapStates);
        }
    }

    for (sf::Drawable* d: DrawableStuff) {
        if (d == static_cast<sf::Drawable*>(&player) ){
            window.draw(*d, MapStates);
        } else {
            window.draw(*d, MapStates);
        }
    }

    for (int i = 0; i < Bullets.size(); i++) window.draw(Bullets[i]);

    if (IsDrawMinimap)      drawMiniMap();
    if (IsDrawInterface)    drawIterface();
    window.display();
}

void drawWalls() {
    sf::Vector2f CameraPos = GameView.getCenter() - GameView.getSize() / 2.f;
    for (int i = std::max(0, 2 * int(CameraPos.y / size) - 1);
            i <= std::min(int(CurLocation->walls.size() - 1), 2 * int((CameraPos.y + sch + WallMinSize) / size) + 1); i++)
        for (int j = std::max(0, int(CameraPos.x / size) - 1);
                j <= std::min(int(CurLocation->walls[i].size() - 1), int((CameraPos.x + scw + WallMinSize) / size) + 1); j++) {
            if (CurLocation->wallsRect[i][j].intersect(CameraPos.x, CameraPos.y, static_cast<float>(scw), static_cast<float>(sch)))
                CurLocation->SeenWalls[i][j] = true;
            if (CurLocation->walls[i][j]) {
                WallRect.setPosition(CurLocation->wallsRect[i][j].getPosition());
                WallRect.setTextureRect({sf::Vector2i{0, 0}, static_cast<sf::Vector2i>(CurLocation->wallsRect[i][j].getSize())});
                window.draw(WallRect, MapStates);
            }
        }
}

void drawMiniMap() {
    if (MiniMapHoldOnPlayer)
        MiniMapView.setCenter(player.getPosition() * ScaleParam);

    // draw walls
    window.setView(MiniMapView);
    sf::VertexArray line(sf::Lines, 2);
    for (int i = 0; i < CurLocation->walls.size(); i++)
        for (int j = 0; j < CurLocation->walls[i].size(); j++)
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
void drawIterface() {
    window.setView(InterfaceView);
    for (sf::Drawable* d: InterfaceStuff) window.draw(*d);

    Bar<int> AmmoBar1(AmmoBar);
    AmmoBar1.setWidth(AmmoBar.getSize().x - 50);
    PlacedText WeaponNameText1(WeaponNameText);
    for (int i = 0; i < weapons.size(); i++) {
        AmmoBar1.setValue(weapons[i]->AmountOfAmmunition);
        WeaponNameText1.setString(weapons[i]->Name);
        AmmoBar1.setPosition(20, sch - 20 - (weapons.size() - i) * (AmmoBar1.getSize().y + 10));
        WeaponNameText1.setPosition(35 + AmmoBar1.getSize().x, AmmoBar1.getPosition().y + WeaponNameText1.Height / 4);
        window.draw(AmmoBar1);
        window.draw(WeaponNameText1);
    }

    if (EscapeMenuActivated) {
        window.draw(ListOfPlayers);
        window.draw(EscapeButton);
    }

    FPSCounter++;
    if (ClockFPS.getElapsedTime() >= sf::seconds(1)) {
        TextFPS.setString(std::to_string(FPSCounter));
        FPSCounter = 0;
        ClockFPS.restart();
    }
    window.draw(TextFPS);

    window.setView(GameView);
}

void LevelGenerate(int n, int m) {
    Bullets.clear();

    MiniMapView.zoom(1 / MiniMapZoom);
    MiniMapZoom = 1;

    LabyrinthLocation.GenerateLocation(n, m, player.getPosition());

    CurLocation->objects.clear();
    CurLocation->AddObject({Tiles::portal, player.getPosition() - portal.getSize() / 2.f});

    for (int i = 0; i < Enemies.size(); i++) {
        delete Enemies[i];
    }
    Enemies.clear();

    for (int i = 0; i < 5; i++) {
        Enemies.push_back(new DistortedScientist());
        Enemies.push_back(new ScottPiligrim());
        Enemies.push_back(new RamonaFlowers());
    }

    for (int i = 0; i < Enemies.size(); i++) {
        do {
            Enemies[i]->setPosition(sf::Vector2f((rand() % m) + 0.5f, (rand() % n) + 0.5f) * (float)size);
        } while (!CurLocation->EnableTiles[(int)Enemies[i]->PosY / size][(int)Enemies[i]->PosX / size] ||
                 distance(Enemies[i]->getPosition(), player.getPosition()) < size * 2);
    }
}

void LoadMainMenu() {
    CurLocation = &MainMenuLocation;
    
    player.setPosition(3.5f * size, 2.5f * size);
    FindAllWaysTo(*CurLocation, player.getPosition(), TheWayToPlayer);
    player.CurWeapon = nullptr;

    portal.setFunction([](){
        player.setPosition(sf::Vector2f{(START_M / 2 + 0.5f) * size, (START_N / 2 + 0.5f) * size});
        player.ChangeWeapon(weapons[CurWeapon.cur]);
        AmmoBar.setValue(player.CurWeapon->AmountOfAmmunition);
        WeaponNameText.setString(player.CurWeapon->Name);

        screen = screens::Dungeon;

        Bullets.clear();

        for (int i = 0; i < Enemies.size(); i++)
            delete Enemies[i];
        Enemies.clear();

        MiniMapActivated = false;
        EscapeMenuActivated = false;

        MiniMapView.setViewport(sf::FloatRect(0.f, 0.f, 0.25f, 0.25f));
        MiniMapView.setCenter(player.getPosition() * ScaleParam);

        MainMenuMusic.pause();
        if (FightMusic1.getStatus() != sf::Music::Playing && FightMusic2.getStatus() != sf::Music::Playing) {
            FightMusic1.play();
        }

        CurLocation = &LabyrinthLocation;
        LevelGenerate(START_N, START_M);
        FindAllWaysTo(*CurLocation, player.getPosition(), TheWayToPlayer);

        DrawableStuff.clear();
        DrawableStuff.push_back(&player);
        for (Enemy* &enemy: Enemies)
            DrawableStuff.push_back(enemy);

        InterfaceStuff.clear();
        InterfaceStuff.push_back(&ManaBar);
        InterfaceStuff.push_back(&HpBar);
        InterfaceStuff.push_back(&AmmoBar);
        InterfaceStuff.push_back(&WeaponNameText);
        InterfaceStuff.push_back(&chat);
        
        InteractibeStuff.clear();
    });

    // Set cameras
    GameView.setCenter(player.getPosition());
    MiniMapView.setCenter(player.getPosition() * ScaleParam);
    InterfaceView.setCenter({scw / 2.f, sch / 2.f});

    FightMusic1.stop();
    FightMusic2.stop();
    MainMenuMusic.play();

    DrawableStuff.clear();
    DrawableStuff.push_back(&player);
    DrawableStuff.push_back(&animation);
    
    InterfaceStuff.clear();
    InterfaceStuff.push_back(&ManaBar);
    InterfaceStuff.push_back(&HpBar);
    InterfaceStuff.push_back(&chat);
    InteractibeStuff.push_back(&portal);
}

void init() {
    setlocale(LC_ALL, "rus");

    window.setFramerateLimit(60);
    window.setVerticalSyncEnabled(true);
    settings.antialiasingLevel = 8;
    window.setView(GameView);
    
    MiniMapView.setViewport(sf::FloatRect(0.f, 0.f, 0.25f, 0.25f));
    GameClock = new sf::Clock;

    // Load music
    MainMenuMusic.openFromFile("sources/music/MainMenuMusic.wav");
    MainMenuMusic.setLoop(true);
    MainMenuMusic.setVolume(20);

    FightMusic1.openFromFile("sources/music/FightMusic1.wav");
    FightMusic1.setVolume(20);

    FightMusic2.openFromFile("sources/music/FightMusic2.wav");
    FightMusic2.setVolume(20);

    // Load locations
    WaitingRoomLoaction.LoadFromFile("sources/locations/WaitingRoom.txt");
    // MainMenuLocation.LoadFromFile("sources/locations/debugLocation.txt");
    MainMenuLocation.LoadFromFile("sources/locations/MainMenu.txt");

    MapShader.loadFromFile("sources/shaders/terrain.vert", "sources/shaders/terrain.frag");
    MapShader.setUniform("current", sf::Shader::CurrentTexture);
    MapShader.setUniform("u_resolution", sf::Vector2f{static_cast<float>(scw), static_cast<float>(sch)});
    MapShader.setUniform("u_playerRadius", player.Radius);

    sf::Texture* WallTexture = new sf::Texture; WallTexture->loadFromFile("sources/textures/wall.png");
    WallRect.setTexture(*WallTexture);

    animation.setPosition({800, 800});
    animation.play();

    CurWeapon.looped = true;

    listener.setBlocking(false);
    MyIP = MySocket.getRemoteAddress().getLocalAddress().toString();
    std::cout << "IP: " << MyIP << '\n';
    
    EscapeButton.setCharacterSize(110);
    IPPanel.setTextSize(80);
    ListOfPlayers.setTextSize(60);

    HostButton.setPosition      (scw / 6 -       HostButton.Width / 4,     sch * 5 / 8);
    EscapeButton.setPosition    (scw / 2 -       EscapeButton.Width / 2,   sch * 5 / 8);

    IPPanel.setPosition         (scw / 2 -       IPPanel.Width / 2,        scw / 8    );
    ListOfPlayers.setPosition   (scw / 2 -       ListOfPlayers.Width / 2,  scw / 16   );

    MMPlayerCircle.setRadius(9);
    MMPlayerCircle.setFillColor(sf::Color(0, 180, 0));
    MMPlayerCircle.setOrigin(MMPlayerCircle.getRadius(), MMPlayerCircle.getRadius());

    player.FirstWeapon  = &pistol;
    player.SecondWeapon = &shotgun;

    HpBar.setWidth(360);
    HpBar.setPosition(scw - HpBar.getSize().x - 10, 20);
    HpBar.setValue(player.Health);
    HpBar.setColors(sf::Color(255, 255, 255, 160), sf::Color(192, 0, 0, 160), sf::Color(32, 32, 32, 160));
    
    ManaBar.setWidth(240);
    ManaBar.setPosition(scw - ManaBar.getSize().x - 10, HpBar.getPosition().y + HpBar.getSize().y);
    ManaBar.setValue(player.Mana);
    ManaBar.setColors(sf::Color(255, 255, 255, 160), sf::Color(0, 0, 192, 160), sf::Color(32, 32, 32, 160));

    AmmoBar.setWidth(160);
    AmmoBar.setPosition(scw - AmmoBar.getSize().x - 10, 120);
    AmmoBar.setColors(sf::Color(255, 255, 255, 160), sf::Color(128, 128, 128, 160), sf::Color(32, 32, 32, 160));

    WeaponNameText.setPosition(AmmoBar.getPosition().x, AmmoBar.getPosition().y + AmmoBar.getSize().y);
    WeaponNameText.text.setFillColor(sf::Color(25, 192, 25, 160));

    sf::Texture *XButtonTexture = new sf::Texture; XButtonTexture->loadFromFile("sources/textures/XButton.png");
    XButtonSprite.setTexture(*XButtonTexture);
    XButtonSprite.setPosition(scw / 2.f - XButtonSprite.getGlobalBounds().width / 2.f, sch * 3.f / 4.f - XButtonSprite.getGlobalBounds().height / 2.f);

    sf::Texture *BoxTexture = new sf::Texture; BoxTexture->loadFromFile("sources/textures/Box.png");
    BoxRect.setTexture(*BoxTexture);
    BoxRect.setPosition(30, 30);
    
    MMPortalRect.setSize(portal.getSize() * ScaleParam);
    MMPortalRect.setFillColor(sf::Color(200, 0, 200, 200));
    
    MMBoxRect.setSize(sf::Vector2f{BoxRect.getGlobalBounds().width, BoxRect.getGlobalBounds().height} * ScaleParam);
    MMBoxRect.setFillColor(sf::Color(252, 108, 24, 200));
    
    LoadMainMenu();

    chat.SetCommand("play", []{ animation.play(); });
    chat.SetCommand("stop", []{ animation.stop(); });
    chat.SetCommand("pause", []{ animation.pause(); });
}

void EventHandler() {
    sf::Event event;
    while (window.pollEvent(event)) {
        if (chat.InputText(event)) {
            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Enter) {
                mutex.lock();
                SendPacket << sf::Int32(pacetStates::ChatEvent) << chat.Last();
                if (HostFuncRun)        SendToClients(SendPacket);
                else if (ClientFuncRun) MySocket.send(SendPacket);
                SendPacket.clear();
                mutex.unlock();
            }
        } else if (EscapeMenuActivated) {
            EscapeButton.isActivated(event);
            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape)
                EscapeMenuActivated = !EscapeMenuActivated;
        } else {
            if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::Escape) {
                    if (MiniMapActivated) {
                        MiniMapActivated = false;
                        MiniMapView.setViewport(sf::FloatRect(0.f, 0.f, 0.25f, 0.25f));
                    }
                }
                if (event.key.code == sf::Keyboard::Tab) {
                    MiniMapActivated = !MiniMapActivated;
                    if (MiniMapActivated) MiniMapView.setViewport(sf::FloatRect(0.f, 0.f, 1.f, 1.f));
                    else                  MiniMapView.setViewport(sf::FloatRect(0.f, 0.f, 0.25f, 0.25f));
                }
                if (event.key.code == sf::Keyboard::Space) {
                    if (MiniMapActivated) MiniMapHoldOnPlayer = !MiniMapHoldOnPlayer;
                }
            }
            if (event.type == sf::Event::MouseWheelMoved) {
                if (MiniMapActivated) {
                    if (event.mouseWheel.delta < 0) { MiniMapView.zoom(1.1f); MiniMapZoom *= 1.1f; }
                    else { MiniMapView.zoom(1.f / 1.1f); MiniMapZoom /= 1.1f; }
                }
            }

            if (IsSomeOneCanBeActivated()) {
                if (!in(InterfaceStuff, static_cast<sf::Drawable*>(&XButtonSprite)))
                    InterfaceStuff.push_back(&XButtonSprite);
            } else
                DeleteFromVector(InterfaceStuff, static_cast<sf::Drawable*>(&XButtonSprite));

            for (Interactible*& x: InteractibeStuff)
                if (x->CanBeActivated(player))
                    if (x->isActivated(player, event, InterfaceStuff))
                        break;

            switch (screen) {
                case screens::MainRoom:
                    if (event.type == sf::Event::KeyPressed)
                        if (event.key.code == sf::Keyboard::Escape) {
                            MainMenuMusic.pause();
                            window.close();
                        }
                    break;

                case screens::Dungeon:
                    if (player.CurWeapon != nullptr && !MiniMapActivated) {
                        player.CurWeapon->Update(event);
                        if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::R)
                            player.CurWeapon->Reload(player.Mana);
                    }
                    if (event.type == sf::Event::KeyPressed) {
                        if (event.key.code == sf::Keyboard::Escape)
                            EscapeMenuActivated = true;
                        if (event.key.code == sf::Keyboard::H) {
                            player.setPosition(size, size);
                            CurLocation = &WaitingRoomLoaction;
                        }
                    }
                    if (event.type == sf::Event::MouseWheelScrolled) {
                        if (!MiniMapActivated) {
                            CurWeapon -= (int)event.mouseWheelScroll.delta;
                            player.ChangeWeapon(weapons[CurWeapon.cur]);
                            AmmoBar.setValue(player.CurWeapon->AmountOfAmmunition);
                            WeaponNameText.setString(player.CurWeapon->Name);
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
                    if (event.type == sf::Event::KeyPressed)
                        if (event.key.code == sf::Keyboard::Enter) {
                            IPPanel.setWord("Connecting...");
                            draw();
                            if (MySocket.connect(IPOfHost, 53000, sf::milliseconds(300)) == sf::Socket::Done) {
                                selector.add(MySocket);
                                
                                if (selector.wait(sf::seconds(1)) && selector.isReady(MySocket) &&
                                MySocket.receive(ReceivePacket) == sf::Socket::Done)
                                    while (!ReceivePacket.endOfPacket()) {
                                        ReceivePacket >> packetState;
                                        switch (packetState) {
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
                                                for (Player& x: ConnectedPlayers) ReceivePacket >> x;
                                                player.setPosition(ConnectedPlayers[ComputerID].getPosition());
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
                    break;
                }
        }
    }
}

void updateBullets() {
    for (int i = 0; i < Bullets.size(); i++)
        if (Bullets[i].penetration < 0 || Bullets[i].todel)
            Bullets.erase(Bullets.begin() + i--);
        else {
            Bullets[i].move(*CurLocation);
            if (Fraction::Friendship[Bullets[i].fromWho].count(player.fraction) == 0 && player.intersect(Bullets[i])) {
                player.getDamage(Bullets[i].damage);
                Bullets[i].penetration--;
            }
            for (Enemy* &enemy: Enemies) {
                if (Fraction::Friendship[Bullets[i].fromWho].count(enemy->fraction) == 0 && enemy->intersect(Bullets[i])) {
                    enemy->getDamage(Bullets[i].damage);
                    Bullets[i].penetration--;
                }
            }
        }
}

void MainLoop() {
    while (window.isOpen()) {
        MapShader.setUniform("u_mouse", static_cast<sf::Vector2f>(sf::Mouse::getPosition()));
        MapShader.setUniform("u_time", GameClock->getElapsedTime().asSeconds());
        MapShader.setUniform("u_playerPosition", static_cast<sf::Vector2f>(window.mapCoordsToPixel(player.getPosition())));
        if (player.Health.toBottom() == 0) {
            EscapeButton.buttonFunction();
        }
        for (int i = 0; i < Enemies.size(); i++) {
            if (Enemies[i]->Health.toBottom() == 0) {
                DeleteFromVector(DrawableStuff, static_cast<sf::Drawable*>(Enemies[i]));
                delete Enemies[i];
                Enemies.erase(Enemies.begin() + i);
            } else {
                Enemies[i]->setTarget(player.getPosition());
                Enemies[i]->move(*CurLocation);
                Enemies[i]->UpdateState();
                Enemies[i]->CurWeapon->lock = false;
                Enemies[i]->CurWeapon->Shoot(*Enemies[i], player.getPosition(), Enemies[i]->fraction);
                Enemies[i]->CurWeapon->Reload(Enemies[i]->Mana);
            }
        }
        player.UpdateState();
        if (screen == screens::Dungeon) {
            if (FightMusic1.getDuration() - FightMusic1.getPlayingOffset() < sf::seconds(0.3f)) {
                FightMusic2.play();
            }
            if (FightMusic2.getDuration() - FightMusic2.getPlayingOffset() < sf::seconds(0.3f)) {
                FightMusic1.play();
            }
        }
        if (!window.hasFocus()) {
            if (player.CurWeapon != nullptr)
                player.CurWeapon->Shoot(player, window.mapPixelToCoords(sf::Mouse::getPosition()), player.fraction);
            updateBullets();

            sf::Event event;
            while (window.pollEvent(event)) {}

            if (HostFuncRun) {
                mutex.lock();
                SendPacket << sf::Int32(pacetStates::PlayerPos);
                for (Player& x: ConnectedPlayers) SendPacket << x;
                SendToClients(SendPacket);
                SendPacket.clear();
                mutex.unlock();
            }
        } else {
            if (!chat.inputted) {
                player.move(*CurLocation);
                GameView.setCenter(player.getPosition() + static_cast<sf::Vector2f>((sf::Mouse::getPosition() - sf::Vector2i{scw, sch} / 2) / 8));
                FindAllWaysTo(*CurLocation, player.getPosition(), TheWayToPlayer);
            }
            int wasBulletsSize = Bullets.size();
            if (player.CurWeapon != nullptr)
                player.CurWeapon->Shoot(player, window.mapPixelToCoords(sf::Mouse::getPosition()), player.fraction);

            if (wasBulletsSize < Bullets.size() && (HostFuncRun || ClientFuncRun)) {
                mutex.lock();
                SendPacket << sf::Int32(pacetStates::Shooting) << (int)Bullets.size() - wasBulletsSize;
                for (; wasBulletsSize < Bullets.size(); wasBulletsSize++)
                    SendPacket << Bullets[wasBulletsSize];
                if (HostFuncRun)        SendToClients(SendPacket);
                else if (ClientFuncRun) MySocket.send(SendPacket);
                SendPacket.clear();
                mutex.unlock();
            }

            updateBullets();

            if (HostFuncRun || ClientFuncRun) {
                ConnectedPlayers[ComputerID].setPosition(player.getPosition());
                mutex.lock();
                SendPacket << sf::Int32(pacetStates::PlayerPos);
                if (HostFuncRun) {
                    for (Player& x: ConnectedPlayers) SendPacket << x;
                    SendToClients(SendPacket);
                } else if (ClientFuncRun) {
                    SendPacket << player;
                    MySocket.send(SendPacket);
                }
                SendPacket.clear();
                mutex.unlock();
            }
            
            if (MiniMapActivated) {
                if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
                    MiniMapView.move(-sf::Vector2f(sf::Mouse::getPosition() - MouseBuffer) * MiniMapZoom);
            }
            MouseBuffer = sf::Mouse::getPosition();

            if (screen == screens::Dungeon) {
                if (Enemies.size() == 0 && !in(InteractibeStuff, (Interactible*)&portal))
                    InteractibeStuff.push_back(&portal);
            }
        }

        draw();

        EventHandler();
    }
}

bool IsSomeOneCanBeActivated() {
    for (Interactible*& x: InteractibeStuff)
        if (x->CanBeActivated(player)) return true;
    return false;
}

//////////////////////////////////////////////////////////// Server-Client functions
void ClientConnect() {
    Client* client = new Client;
    if (listener.accept(*client) == sf::Socket::Done) {
        mutex.lock();

        str ConnectedClientIP = (*client).getRemoteAddress().toString();
        SendPacket << sf::Int32(pacetStates::PlayerConnect) << ConnectedClientIP;
        SendToClients(SendPacket);
        SendPacket.clear();

        std::cout << ConnectedClientIP << " connected\n";
        ListOfPlayers.addWord(ConnectedClientIP);
        std::cout << "list of players:\n" << ListOfPlayers.getWord() << '\n';

        clients.push_back(client);
        selector.add(*client);

        ConnectedPlayers.push_back(*(new Player()));
        ConnectedPlayers[ConnectedPlayers.size() - 1].setPosition(float(CurLocation->m) * size / 2, float(CurLocation->n) * size / 2);
        SendPacket << sf::Int32(pacetStates::PlayersAmount) << (sf::Int32)ConnectedPlayers.size() - 1;

        DrawableStuff.push_back(&(ConnectedPlayers[ConnectedPlayers.size() - 1]));

        for (int i = 0; i < ListOfPlayers.size(); i++)
            SendPacket << sf::Int32(pacetStates::PlayerConnect) << ListOfPlayers[i];

        SendPacket << sf::Int32(pacetStates::Shooting) << (sf::Int32)Bullets.size();
        std::cout << "bullets: " << Bullets.size() << "\n";
        for (int i = 0; i < Bullets.size(); i++) SendPacket << Bullets[i];

        std::cout << "amount players = " << ConnectedPlayers.size() - 1 << '\n';
        SendPacket << sf::Int32(pacetStates::SetPos);
        for (Player& x: ConnectedPlayers) SendPacket << x;

        if (client->send(SendPacket) == sf::Socket::Done) std::cout << "SendPacket was sended\n";
        else std::cout << "SendPacket didn't sended\n";

        SendPacket.clear();
        SendPacket << LabyrinthLocation;
        if (client->send(SendPacket) == sf::Socket::Done) std::cout << "Labyrinth walls sended\n";
        else std::cout << "Labyrinth walls didn't sended\n";
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
    SendPacket << sf::Int32(pacetStates::PlayerDisconnect) << i;
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
    SendPacket << sf::Int32(pacetStates::disconnect);
    MySocket.send(SendPacket);
    SendPacket.clear();
    mutex.unlock();
    MySocket.disconnect();
    selector.clear();
    ConnectedPlayers.clear();
}

void SendToClients(sf::Packet& pac, int ExceptOf) {
    for (int i = 0; i < clients.size(); i++)
        if (i != ExceptOf && clients[i]->send(pac) != sf::Socket::Done) {}
}

void funcOfHost() {
    std::cout << "Server turn on\n";
    while (HostFuncRun) {
        if (selector.wait(sf::seconds(1))) {
            if (selector.isReady(listener)) ClientConnect();
            else
                for (int i = 0; i < clients.size(); i++)
                    if (selector.isReady(*clients[i]) && clients[i]->receive(ReceivePacket) == sf::Socket::Done)
                        while (!ReceivePacket.endOfPacket()) {
                            ReceivePacket >> packetState;
                            switch (packetState) {
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
                                    SendPacket << sf::Int32(pacetStates::ChatEvent) << PacetData;
                                    SendToClients(SendPacket, i);
                                    SendPacket.clear();
                                    mutex.unlock();
                                    break;
                                case pacetStates::Shooting: {
                                    mutex.lock();
                                    int i; ReceivePacket >> i;
                                    SendPacket << sf::Int32(pacetStates::Shooting) << i;
                                    for (; i > 0; i--) {
                                        ReceivePacket >> tempBullet;
                                        Bullets.push_back(tempBullet);
                                        SendPacket << tempBullet;
                                    }
                                    SendToClients(SendPacket, i);
                                    SendPacket.clear();
                                    mutex.unlock();
                                    }
                                    break;
                            }
                        }
        }
    }
    std::cout << "Server turn off\n";
}

void funcOfClient() {
    while (ClientFuncRun) {
        if (selector.wait(sf::seconds(1)))
            if (selector.isReady(MySocket) && MySocket.receive(ReceivePacket) == sf::Socket::Done)
                while (!ReceivePacket.endOfPacket()) {
                    ReceivePacket >> packetState;
                    switch (packetState) {
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
                            std::cout << ListOfPlayers[index] << " disconnected\n";
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
                            for (int i = 0; i < ConnectedPlayers.size(); i++) 
                                if (i != ComputerID)
                                    ReceivePacket >> ConnectedPlayers[i];
                                else {
                                    Point tempPoint;
                                    ReceivePacket >> tempPoint;
                                }
                            break;
                        case pacetStates::SetPos:
                            for (Player& x: ConnectedPlayers) ReceivePacket >> x;
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
