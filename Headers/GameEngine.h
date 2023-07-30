#include "portal.h"
#include "player.h"
#include "client.h"
#include "chat.h"
#include "contextMenu.h"
#include "location.h"

//////////////////////////////////////////////////////////// Settings of the game
bool IsDrawMinimap   = true;
bool IsDrawInterface = true;

//////////////////////////////////////////////////////////// Stuff for work with system and screen
sf::ContextSettings settings;

sf::RenderWindow window(sf::VideoMode(scw, sch), "multigame", sf::Style::Fullscreen, settings);
sf::View GameView = window.getDefaultView();
sf::View InterfaceView = window.getDefaultView();
sf::View MiniMapView = window.getDefaultView();
float MiniMapZoom = 1.f;
bool MiniMapActivated, EscapeMenuActivated;
screens::screens screen = screens::MainRoom;
std::vector<sf::Drawable*> DrawableStuff;

//////////////////////////////////////////////////////////// Music
sf::Music MainMenuMusic;

//////////////////////////////////////////////////////////// Online tools
sf::TcpListener listener;
sf::Packet ReceivePacket, SendPacket, LabyrinthData;
std::vector<Client*> clients(0);
sf::SocketSelector selector;
str ClientState, IPOfHost, MyIP, PacetData;
Client MySocket; // this computer socket
sf::Int32 packetState, ComputerID;
sf::Mutex mutex;

//////////////////////////////////////////////////////////// Portal
Portal portal;

//////////////////////////////////////////////////////////// Locations
Location* CurLocation = nullptr;
Location LabyrinthLocation, WaitingRoomLoaction, MainMenuLocation;

//////////////////////////////////////////////////////////// System stuff
bool ClientFuncRun, HostFuncRun;
vvr wallsRect(0);
vvb SeenWalls;
std::vector<sf::Sprite> Sprites(0);
sf::RectangleShape WallRect;

//////////////////////////////////////////////////////////// Players and chat
Player player;
std::vector<Player> ConnectedPlayers(0);
Chat chat;
bool multiplayer;
sf::CircleShape CircleOfSmallPlayer;  // for drawing players on minimap

//////////////////////////////////////////////////////////// other stuff
// PlaccedText TextFPS;
sf::Vector2i MouseBuffer;
Bullet tempBullet;

//////////////////////////////////////////////////////////// the weapons
Pistol pistol;
Shotgun shotgun;
Revolver revolver;
Rifle rifle;
Bubblegun bubblegun;
Armagedon armagedon;
Chaotic chaotic;

//////////////////////////////////////////////////////////// functions
void draw();
void drawWalls();
void drawMiniMap();
void drawIterface();
void ClientConnect();
void ClientDisconnect(int);
void SelfDisconnect();
void SendToClients(sf::Packet&);
void SendToOtherClients(sf::Packet&, int);
void funcOfHost();
void funcOfClient();
void LevelGenerate(int, int);
void LoadMainMenu();
void init();
void EventHandler();
void MainLoop();

//////////////////////////////////////////////////////////// Threads
sf::Thread HostTread(funcOfHost);
sf::Thread ClientTread(funcOfClient);

//////////////////////////////////////////////////////////// Pannels
Panel  IPPanel          ("sources/textures/YellowPanel", "IP:");
Panel  ListOfPlayers    ("sources/textures/SteelFrame"        );

//////////////////////////////////////////////////////////// Buttons
Button CoopButton("sources/textures/RedPanel", "Online", [](){
    multiplayer = true;
    MiniMapActivated = false;
    MiniMapView.setViewport(sf::FloatRect(0.f, 0.f, 0.25f, 0.25f));
    MiniMapView.setCenter({CurLocation->m * miniSize / 2.f, CurLocation->n * miniSize / 2.f});
    MainMenuMusic.pause();
});

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

Button EscapeToMainMenuButton("sources/textures/RedPanel", "Exit", [](){
    if (multiplayer) {
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
        } else
            SelfDisconnect();
    }
    screen = screens::MainRoom;
    EscapeMenuActivated = false;
    ListOfPlayers.clear();
    Bullets.clear();
    LoadMainMenu();
});

void draw() {
    window.clear(sf::Color::Black);
    drawWalls();

    for (sf::Drawable* d: DrawableStuff) window.draw(*d);

    for (int i = 0; i < Bullets.size(); i++) window.draw(Bullets[i]);

    if (IsDrawMinimap)      drawMiniMap();
    if (IsDrawInterface)    drawIterface();
    window.display();
}

void drawWalls() {
    CameraPos = GameView.getCenter() - GameView.getSize() / 2.f;
    for (int i = std::max(0, 2 * int(CameraPos.y / size) - 1);
            i <= std::min(int(CurLocation->walls.size() - 1), 2 * int((CameraPos.y + sch + WallMinSize) / size) + 1); i++)
        for (int j = std::max(0, int(CameraPos.x / size) - 1);
                j <= std::min(int(CurLocation->walls[i].size() - 1), int((CameraPos.x + scw + WallMinSize) / size) + 1); j++) {
            if (wallsRect[i][j].intersect(CameraPos.x, CameraPos.y, (float)scw, (float)sch))
                SeenWalls[i][j] = true;
            if ((*CurLocation)[i][j] == Tiles::wall) {
                WallRect.setPosition(wallsRect[i][j].getPosition());
                WallRect.setSize(wallsRect[i][j].getSize());
                window.draw(WallRect);
            }
        }
}

void drawMiniMap() {
    // draw walls
    float ScaleParam = float(miniSize) / float(size);
    window.setView(MiniMapView);
    sf::VertexArray line(sf::Lines, 2);
    for (int i = 0; i < CurLocation->walls.size(); i++)
        for (int j = 0; j < CurLocation->walls[i].size(); j++)
            if ((*CurLocation)[i][j] == Tiles::wall && SeenWalls[i][j]) {
                if (i % 2 == 1) { // |
                    line[0] = sf::Vertex(sf::Vector2f(miniSize * j, miniSize * (i - 1) / 2), sf::Color::White);
                    line[1] = sf::Vertex(sf::Vector2f(miniSize * j, miniSize * (i + 1) / 2), sf::Color::White);
                } else { // -
                    line[0] = sf::Vertex(sf::Vector2f(miniSize * j, miniSize * i / 2));
                    line[1] = sf::Vertex(sf::Vector2f(miniSize * (j + 1), miniSize * i / 2));
                }
                window.draw(line);
            }
    
    // draw location objects minimap
    for (int i = 0; i < CurLocation->objects.size(); i++) {
        if (CurLocation->objects[i].id == Tiles::portal) {
            sf::RectangleShape portalRect(portal.getSize() * ScaleParam);
            portalRect.setPosition(portal.getPosition() * ScaleParam);
            portalRect.setFillColor(sf::Color(200, 0, 200, 200));
            window.draw(portalRect);
            break;
        }
    }

    // draw players
    if (multiplayer) {
        for (Player& p: ConnectedPlayers) {
            CircleOfSmallPlayer.setPosition(p.getPosition() * ScaleParam);
            window.draw(CircleOfSmallPlayer);
        }
    } else {
        CircleOfSmallPlayer.setPosition(player.getPosition() * ScaleParam);
        window.draw(CircleOfSmallPlayer);
    }
    window.setView(GameView);
}

void drawIterface() {
    window.setView(InterfaceView);
    switch (screen) {
        case screens::MainRoom:
            portal.interface(window);
            chat.draw(window);
            break;
        case screens::Dungeon:
            player.interface(window);
            portal.interface(window);
            chat.draw(window);
            break;
        case screens::SetIP:
            IPPanel.draw(window);
            break;

    }
    if (EscapeMenuActivated) {
        window.draw(ListOfPlayers);
        window.draw(EscapeToMainMenuButton);
    }
    window.setView(GameView);
}

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
        if (client->send(LabyrinthData) == sf::Socket::Done) std::cout << "Labyrinth walls sended\n";
        else std::cout << "Labyrinth walls didn't sended\n";

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

void SendToClients(sf::Packet& pac) {
    if (clients.size() < 1) return;
    // int index = -1;
    for (int i = 0; i < clients.size(); i++)
        if (clients[i]->send(pac) != sf::Socket::Done) {}
            // index = i;
    // std::cout << "index = " << index << '\n';
    // if (index != -1) ClientDisconnect(index);
}

void SendToOtherClients(sf::Packet& pac, int num) {
    // int index = -1;
    for (int i = 0; i < clients.size(); i++)
        if (i != num && clients[i]->send(pac) != sf::Socket::Done) {}
    //         index = i;
    // if (index != -1) ClientDisconnect(index);
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
                                    SendToOtherClients(SendPacket, i);
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
                                    SendToOtherClients(SendPacket, i);
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
                            ReceivePacket >> LabyrinthLocation.n >> LabyrinthLocation.m;
                            std::cout << "n = " << LabyrinthLocation.n << " m = " << LabyrinthLocation.m << '\n';
                            LabyrinthLocation.SetSize(LabyrinthLocation.n, LabyrinthLocation.m);
                            for (int i = 0; i < LabyrinthLocation.walls.size(); i++)
                                for (int j = 0; j < LabyrinthLocation[i].size(); j++)
                                    ReceivePacket >> LabyrinthLocation[i][j];
                            CreateMapRectByLocation(LabyrinthLocation, wallsRect, Sprites);
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
                            GameView.setCenter(player.getCenter());
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

void LevelGenerate(int n, int m) {
    LabyrinthLocation.SetSize(n, m);
    LabyrinthData.clear();
    LabyrinthData << sf::Int32(pacetStates::Labyrinth) << n << m;

    player.setPosition(sf::Vector2f{(m / 2 + 0.5f) * size, (n / 2 + 0.5f) * size} - player.getSize() / 2.f);
    LabyrinthLocation.objects.clear();
    Bullets.clear();
    
    int CounterOfGenerations = 0, CountOfEnableTilesOnMap;
    do {
        LabyrinthLocation.WallGenerator(0.48);
        CountOfEnableTilesOnMap = LabyrinthLocation.BuildWayFrom(player.getPosition() / (float)size);
        CounterOfGenerations++;
    } while (CountOfEnableTilesOnMap < float(n * m) / 3 || CountOfEnableTilesOnMap > float(n * m) / 1.5);
    std::cout << "total count of generations = " << CounterOfGenerations
              << " Count Of Enable Tiles = " << CountOfEnableTilesOnMap << '\n';
    
    for (int i = 0; i < LabyrinthLocation.walls.size(); i++)
        for (int j = 0; j < LabyrinthLocation[i].size(); j++)
            LabyrinthData << LabyrinthLocation[i][j];
    CreateMapRectByLocation(LabyrinthLocation, wallsRect, Sprites);
    
    SeenWalls.assign(LabyrinthLocation.walls.size(), vb(0));
    for (int i = 0; i < LabyrinthLocation.walls.size(); i++)
        SeenWalls[i].assign(LabyrinthLocation[i].size(), false);
    
    MiniMapView.zoom(1 / MiniMapZoom);
    MiniMapZoom = 1;

    CurLocation->objects.push_back({Tiles::portal, portal.getPosition()});
}

void LoadMainMenu() {
    CurLocation = &MainMenuLocation;
    CreateMapRectByLocation(*CurLocation, wallsRect, Sprites);
    SeenWalls.assign(CurLocation->walls.size(), vb(0));
    for (int i = 0; i < CurLocation->walls.size(); i++)
        SeenWalls[i].assign(CurLocation->walls[i].size(), false);
    
    player.setCenter(1.f * size, 1.f * size);
    player.CurWeapon = nullptr;

    portal.setCenter(size * 3, size);
    portal.setFunction([](){
        screen = screens::Dungeon;
        Bullets.clear();
        MiniMapActivated = false;
        EscapeMenuActivated = false;
        MiniMapView.setViewport(sf::FloatRect(0.f, 0.f, 0.25f, 0.25f));
        MiniMapView.setCenter({CurLocation->m * miniSize / 2.f, CurLocation->n * miniSize / 2.f});
        MainMenuMusic.pause();
        CurLocation = &LabyrinthLocation;
        LevelGenerate(START_N, START_M);

        portal.setCenter(player.getCenter());
        portal.setFunction([](){
            LevelGenerate(START_N, START_M);
            portal.setCenter(player.getCenter());
        });
    });

    // Set cameras
    GameView.setCenter(player.getCenter());
    MiniMapView.setCenter({CurLocation->m * miniSize / 2.f, CurLocation->n * miniSize / 2.f});
    InterfaceView.setCenter({scw / 2.f, sch / 2.f});

    MainMenuMusic.play();

    DrawableStuff.push_back(&portal);
    DrawableStuff.push_back(&player);
}

void init() {
    setlocale(LC_ALL, "rus");
    GlobalClock.restart();

    window.setFramerateLimit(60);
    window.setVerticalSyncEnabled(true);
    settings.antialiasingLevel = 8;
    window.setView(GameView);
    
    MiniMapView.setViewport(sf::FloatRect(0.f, 0.f, 0.25f, 0.25f));

    // Load music
    MainMenuMusic.openFromFile("sources/music/RestAreaMusic.wav");
    MainMenuMusic.setLoop(true);
    MainMenuMusic.setVolume(20);

    // Load locations
    WaitingRoomLoaction.LoadFromFile("sources/locations/WaitingRoom.txt");
    MainMenuLocation.LoadFromFile("sources/locations/MainMenu.txt");
    
    LoadMainMenu();

    WallRect.setFillColor(sf::Color(120, 120, 120));

    // TextFPS.text.setPosition(20, 20);
    // TextFPS.text.setFillColor(sf::Color(255, 255, 255));

    listener.setBlocking(false);
    MyIP = MySocket.getRemoteAddress().getLocalAddress().toString();
    std::cout << "IP: " << MyIP << '\n';
    
    EscapeToMainMenuButton.setCharacterSize(110);
    IPPanel.setTextSize(80);
    ListOfPlayers.setTextSize(60);

    CoopButton.setPosition      (scw - scw / 6 - CoopButton.Width * 3 / 4,      sch * 5 / 8);
    HostButton.setPosition      (scw / 6 -       HostButton.Width / 4,          sch * 5 / 8);
    EscapeToMainMenuButton.setPosition  (scw / 2 -       EscapeToMainMenuButton.Width / 2,      sch * 5 / 8);
    
    IPPanel.setPosition         (scw / 2 -       IPPanel.Width / 2,             scw / 8    );
    ListOfPlayers.setPosition   (scw / 2 -       ListOfPlayers.Width / 2,       scw / 16   );

    // sf::Time time = GlobalClock.getElapsedTime();

    CircleOfSmallPlayer.setRadius(9);
    CircleOfSmallPlayer.setFillColor(sf::Color(0, 180, 0));

    player.FirstWeapon  = &pistol;
    player.SecondWeapon = &shotgun;
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
            EscapeToMainMenuButton.isActivated(event);
            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape)
                EscapeMenuActivated = !EscapeMenuActivated;
        } else {
            if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::Escape) {
                    if (MiniMapActivated) {
                        MiniMapActivated = false;
                        MiniMapView.setViewport(sf::FloatRect(0.f, 0.f, 0.25f, 0.25f));
                    } else if (portal.isInterfaceDrawn) portal.isInterfaceDrawn = false;
                    else {
                        if (screen == screens::MainRoom) {
                            MainMenuMusic.pause();
                            window.close();
                        } else EscapeMenuActivated = true;
                    }
                } else if (event.key.code == sf::Keyboard::Tab) {
                    MiniMapActivated = !MiniMapActivated;
                    if (MiniMapActivated) MiniMapView.setViewport(sf::FloatRect(0.f, 0.f, 1.f, 1.f));
                    else                  MiniMapView.setViewport(sf::FloatRect(0.f, 0.f, 0.25f, 0.25f));
                }
            } else if (event.type == sf::Event::MouseWheelMoved) {
                if (MiniMapActivated) {
                    if (event.mouseWheel.delta < 0) { MiniMapView.zoom(1.1f); MiniMapZoom *= 1.1f; }
                    else { MiniMapView.zoom(1.f / 1.1f); MiniMapZoom /= 1.1f; }
                }
            }
        
            switch (screen) {
                case screens::MainRoom:
                    portal.isActivated(player, event);
                    player.update(event, MiniMapActivated);
                    break;

                case screens::Dungeon:
                    portal.isActivated(player, event);
                    player.update(event, MiniMapActivated);
                    if (event.type == sf::Event::KeyPressed) {
                        if (event.key.code == sf::Keyboard::H) {
                            player.setPosition(size, size);
                            CurLocation = &WaitingRoomLoaction;
                            CreateMapRectByLocation(WaitingRoomLoaction, wallsRect, Sprites);
                        } else if (event.key.code == sf::Keyboard::Num1) player.ChangeWeapon(&pistol);
                        else if (event.key.code == sf::Keyboard::Num2) player.ChangeWeapon(&shotgun);
                        else if (event.key.code == sf::Keyboard::Num3) player.ChangeWeapon(&revolver);
                        else if (event.key.code == sf::Keyboard::Num4) player.ChangeWeapon(&rifle);
                        else if (event.key.code == sf::Keyboard::Num5) player.ChangeWeapon(&bubblegun);
                        else if (event.key.code == sf::Keyboard::Num6) player.ChangeWeapon(&armagedon);
                        else if (event.key.code == sf::Keyboard::Num7) player.ChangeWeapon(&chaotic);
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

void MainLoop() {
    while (window.isOpen()) {
        if (!window.hasFocus()) {
            player.update();
            for (int i = 0; i < Bullets.size();) {
                if (Bullets[i].penetration < 0 || Bullets[i].todel)
                    Bullets.erase(Bullets.begin() + i);
                else
                    Bullets[i++].move(wallsRect);
            }
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
                player.move(wallsRect);
                GameView.setCenter(player.getCenter());
            }
            int wasBulletsSize = Bullets.size();
            player.update();

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

            for (int i = 0; i < Bullets.size();) {
                if (Bullets[i].penetration < 0 || Bullets[i].todel)
                    Bullets.erase(Bullets.begin() + i);
                else
                    Bullets[i++].move(wallsRect);
            }

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
        }
        draw();

        EventHandler();
    }
}