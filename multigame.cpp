#include "Headers/portal.h"
#include "Headers/player.h"
#include "Headers/client.h"
#include "Headers/chat.h"
#include "Headers/contextMenu.h"

//////////////////////////////////////////////////////////// Settings of the game
bool IsDrawMinimap   = true;
bool IsDrawInterface = true;

//////////////////////////////////////////////////////////// Stuff for work with sistem and screen
sf::ContextSettings settings;

sf::RenderWindow window(sf::VideoMode(scw, sch), "main", sf::Style::Fullscreen, settings);
sf::View GameView = window.getDefaultView();
sf::View InterfaceView = window.getDefaultView();
sf::View MiniMapView = window.getDefaultView();
float MiniMapZoom = 1.f;
bool MiniMapActivated;
sf::Event event;
sf::Mouse Mouse;
screens::screens screen = screens::MainRoom;

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
sf::Sprite tempSprite;
int CountOfEnableTilesOnMap;
sf::RectangleShape WallRectG(sf::Vector2f(WallMaxSize, WallMinSize));
sf::RectangleShape WallRectV(sf::Vector2f(WallMinSize, WallMaxSize));

//////////////////////////////////////////////////////////// Players and chat
Player player;
std::vector<Player> ConnectedPlayers(0);
Chat chat;
bool multiplayer;
sf::CircleShape CircleOfSmallPlayer;  // for drawing players on minimap

//////////////////////////////////////////////////////////// other stuff
// PlaccedText TextFPS;
Point tempPoint;
sf::Vector2i MouseBuffer;
sf::CircleShape BulletShape;
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
void ClientConnect();
void ClientDisconnect(int);
void SelfDisconnect();
void SendToClients(sf::Packet&);
void SendToOtherClients(sf::Packet&, int);
void funcOfHost();
void funcOfClient();
void LevelGenerate(int, int);
void drawWalls();
void drawMiniMap();
void drawIterface();
void LoadMainMenu();
void init();
void EventHandler();

//////////////////////////////////////////////////////////// Threads
sf::Thread HostTread(funcOfHost);
sf::Thread ClientTread(funcOfClient);

//////////////////////////////////////////////////////////// Pannels
Panel  IPPanel          ("sources/textures/YellowPanel", "IP:");
Panel  ListOfPlayers    ("sources/textures/SteelFrame"        );

//////////////////////////////////////////////////////////// Buttons
Button SoloButton("sources/textures/RedPanel", "Play" , [](){
    screen = screens::Solo;
    Bullets.clear();
    LevelGenerate(START_N, START_M);
    multiplayer = false;
    MiniMapActivated = false;
    MiniMapView.setViewport(sf::FloatRect(0.f, 0.f, 0.25f, 0.25f));
    MiniMapView.setCenter({CurLocation->m * miniSize / 2.f, CurLocation->n * miniSize / 2.f});
    MainMenuMusic.pause();
});

Button NewGameButton("sources/textures/GreenPanel", "New Game", [](){});

Button ContinueButton("sources/textures/BluePanel", "Continue", [](){});

Button CoopButton("sources/textures/RedPanel", "Online", [](){
    screen = screens::Coop;
    multiplayer = true;
    MiniMapActivated = false;
    MiniMapView.setViewport(sf::FloatRect(0.f, 0.f, 0.25f, 0.25f));
    MiniMapView.setCenter({CurLocation->m * miniSize / 2.f, CurLocation->n * miniSize / 2.f});
    MainMenuMusic.pause();
});

Button HostButton("sources/textures/GreenPanel", "Host", [](){
    screen = screens::Host;
    listener.listen(53000);
    selector.add(listener);
    ListOfPlayers.setWord(MyIP);
    Bullets.clear();
    LevelGenerate(START_N, START_M);
    ComputerID = 0;
    Player* NewPlayer = new Player();
    ConnectedPlayers.push_back(*NewPlayer);
    HostFuncRun = true;
    HostTread.launch();
});

Button ConnectButton("sources/textures/BluePanel", "Connect", [](){
    screen = screens::SetIP;
});

Button MainMenuButton("sources/textures/RedPanel", "Exit", [](){
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
    ListOfPlayers.clear();
    Bullets.clear();
    LoadMainMenu();
});

//////////////////////////////////////////////////////////// the program
int main() {
    init();

    while (window.isOpen()) {
        if (!window.hasFocus()) {
            if (HostFuncRun) {
                mutex.lock();
                SendPacket << sf::Int32(pacetStates::PlayerPos);
                for (Player& x: ConnectedPlayers) SendPacket << x;
                SendToClients(SendPacket);
                SendPacket.clear();
                mutex.unlock();
            }
            player.update(Bullets);
            for (int i = 0; i < Bullets.size();) {
                // int y = (int(Bullets[i].PosY) / size), x = (int(Bullets[i].PosX) / size);
                // if (!(0 <= x && x < BigM && 0 <= y && y < BigN) || Bullets[i].penetration < 0 || Bullets[i].todel) {
                if (Bullets[i].penetration < 0 || Bullets[i].todel)
                    Bullets.erase(Bullets.begin() + i--);
                else
                    Bullets[i++].move(wallsRect);
            }
            while (window.pollEvent(event)) {}
            draw();
            // TextFPS.text.setString(std::to_string((int)std::round(1.f / (GlobalClock.getElapsedTime() - time).asSeconds())) + " fps");
            // time = GlobalClock.getElapsedTime();
        } else {
            if (screen == screens::MainRoom) {
                player.move(wallsRect);
                GameView.setCenter(player.getCenter());
            } else if (screen == screens::Solo || screen == screens::Host || screen == screens::Connect) {
                if (!chat.inputted) {
                    player.move(wallsRect);
                    GameView.setCenter(player.getCenter());
                }
                int wasBulletsCount = Bullets.size();
                player.update(Bullets);
                if (wasBulletsCount < Bullets.size() && (screen == screens::Host || screen == screens::Connect)) {
                    mutex.lock();
                    SendPacket << sf::Int32(pacetStates::Shooting) << (int)Bullets.size() - wasBulletsCount;
                    for (; wasBulletsCount < Bullets.size(); wasBulletsCount++)
                        SendPacket << Bullets[wasBulletsCount];
                    if (HostFuncRun)        SendToClients(SendPacket);
                    else if (ClientFuncRun) MySocket.send(SendPacket);
                    SendPacket.clear();
                    mutex.unlock();
                }
                int CountOfBulletsOtOfScreen = 0;
                for (int i = 0; i < Bullets.size() - CountOfBulletsOtOfScreen;) {
                    if (Bullets[i].penetration < 0 || Bullets[i].todel)
                        std::swap(Bullets[i], Bullets[Bullets.size() - 1 - CountOfBulletsOtOfScreen++]);
                    else
                        Bullets[i++].move(wallsRect);
                }
                Bullets.resize(Bullets.size() - CountOfBulletsOtOfScreen);
            }
            if (HostFuncRun || ClientFuncRun) {
                ConnectedPlayers[ComputerID].setPosition(player.getPosition());
                mutex.lock();
                SendPacket << sf::Int32(pacetStates::PlayerPos);
                if (HostFuncRun) {
                    for (Player& x: ConnectedPlayers)
                        SendPacket << x;
                    SendToClients(SendPacket);
                } else if (ClientFuncRun) {
                    SendPacket << player;
                    MySocket.send(SendPacket);
                }
                SendPacket.clear();
                mutex.unlock();
            }
            if (MiniMapActivated) {
                if (Mouse.isButtonPressed(Mouse.Left))
                    MiniMapView.move(-sf::Vector2f(Mouse.getPosition() - MouseBuffer) * MiniMapZoom);
            }
            MouseBuffer = Mouse.getPosition();
            draw();
            // TextFPS.text.setString(std::to_string((int)std::round(1.f / (GlobalClock.getElapsedTime() - time).asSeconds())) + " fps");
            // time = GlobalClock.getElapsedTime();
        }

        EventHandler();
    }
    return 0;
}

void draw() {
    window.clear(sf::Color::Black);
    drawWalls();

    for (sf::Sprite& s: Sprites) window.draw(s);

    switch (screen) {
        case screens::MainRoom:
        case screens::Solo:
            window.draw(portal);
            window.draw(player);
            break;
        case screens::Coop:
            window.draw(HostButton);
            window.draw(ConnectButton);
            break;
        case screens::Connect:
        case screens::Host:
        case screens::EscOfCoop:
            for (Player& p: ConnectedPlayers)
                window.draw(p);
            window.draw(player);
            break;
        case screens::SetIP:
            break;
    }

    for (int i = 0; i < Bullets.size(); i++)
        window.draw(Bullets[i]);

    if (IsDrawMinimap)      drawMiniMap();
    if (IsDrawInterface)    drawIterface();
    // window.draw(TextFPS.text);
    window.display();
}

void drawWalls() {
    CameraPos = GameView.getCenter() - GameView.getSize() / 2.f;
    for (int i = std::max(0, 2 * int(CameraPos.y / size) - 1);
            i <= std::min(int(CurLocation->data.size() - 1), 2 * int((CameraPos.y + sch + WallMinSize) / size) + 1); i++)
        for (int j = std::max(0, int(CameraPos.x / size) - 1);
                j <= std::min(int(CurLocation->data[i].size() - 1), int((CameraPos.x + scw + WallMinSize) / size) + 1); j++) {
            if (wallsRect[i][j].intersect(CameraPos.x, CameraPos.y, (float)scw, (float)sch))
                SeenWalls[i][j] = true;
            if ((*CurLocation)[i][j] == Tiles::wall) {
                if (i % 2 == 1) { // |
                    WallRectV.setPosition(sf::Vector2f(wallsRect[i][j].PosX, wallsRect[i][j].PosY));
                    window.draw(WallRectV);
                } else { // -
                    WallRectG.setPosition(sf::Vector2f(wallsRect[i][j].PosX, wallsRect[i][j].PosY));
                    window.draw(WallRectG);
                }
            }
        }
}

void drawMiniMap() {
    // draw minimap wall
    window.setView(MiniMapView);
    sf::VertexArray line(sf::Lines, 2);
    for (int i = 0; i < CurLocation->data.size(); i++)
        for (int j = 0; j < CurLocation->data[i].size(); j++)
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
    // draw minimap players
    if (multiplayer){
        for (Player& p: ConnectedPlayers) {
            CircleOfSmallPlayer.setPosition(p.getPosition() / float(size) * float(miniSize));
            window.draw(CircleOfSmallPlayer);
        }
    } else {
        CircleOfSmallPlayer.setPosition(player.getPosition() / float(size) * float(miniSize));
        window.draw(CircleOfSmallPlayer);
    }
    window.setView(GameView);
}

void drawIterface() {
    window.setView(InterfaceView);
    switch (screen) {
        case screens::MainRoom:
            portal.interface(window);
            break;
        case screens::Solo:
            player.interface(window);
            portal.interface(window);
            chat.draw(window);
            break;
        case screens::Coop:
            window.draw(HostButton);
            window.draw(ConnectButton);
            break;
        case screens::Connect:
            player.interface(window);
            portal.interface(window);
            chat.draw(window);
            break;
        case screens::Host:
            player.interface(window);
            portal.interface(window);
            chat.draw(window);
            break;
        case screens::EscOfCoop:
            player.interface(window);
            portal.interface(window);
            chat.draw(window);
            window.draw(ListOfPlayers);
            window.draw(MainMenuButton);
            break;
        case screens::SetIP:
            IPPanel.draw(window);
            break;
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

        Player* NewPlayer = new Player();
        NewPlayer->setPosition({float(CurLocation->m) * size / 2, float(CurLocation->n) * size / 2});
        ConnectedPlayers.push_back(*NewPlayer);
        SendPacket << sf::Int32(pacetStates::PlayersAmount) << (int)ConnectedPlayers.size() - 1;

        for (int i = 0; i < ListOfPlayers.size(); i++)
            SendPacket << sf::Int32(pacetStates::PlayerConnect) << ListOfPlayers[i];

        SendPacket << sf::Int32(pacetStates::Shooting) << (int)Bullets.size();
        std::cout << "bullets: " << Bullets.size() << "\n";
        for (int i = 0; i < Bullets.size(); i++) SendPacket << Bullets[i];

        std::cout << "amount players = " << ConnectedPlayers.size() - 1 << '\n';
        SendPacket << sf::Int32(pacetStates::SetPos);
        for (Player& x: ConnectedPlayers) SendPacket << x;

        if (client->send(SendPacket) == sf::Socket::Done) std::cout << "SendPacket was sended\n";
        else std::cout << "SendPacket didn't sended\n";

        SendPacket.clear();
        if (client->send(LabyrinthData) == sf::Socket::Done) std::cout << "Labyrinth data sended\n";
        else std::cout << "Labyrinth data didn't sended\n";

        mutex.unlock();
    } else delete client;
}

void ClientDisconnect(int i) {
    selector.remove(*clients[i]);
    std::cout << (*clients[i]).getRemoteAddress().toString() << " disconnected; number = " << i << "\n";
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
                                        tempBullet.UpdateCircleShape();
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
                            std::cout << "bruh!\n";
                            SelfDisconnect();
                            break;
                        case pacetStates::PlayerConnect:
                            ReceivePacket >> PacetData;
                            ListOfPlayers.addWord(PacetData);
                            { Player* NewPlayer = new Player();
                            ConnectedPlayers.push_back(*NewPlayer);
                            }
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
                            for (int i = 0; i < LabyrinthLocation.data.size(); i++)
                                for (int j = 0; j < LabyrinthLocation[i].size(); j++)
                                    ReceivePacket >> LabyrinthLocation[i][j];
                            CreateMapRectByLocation(LabyrinthLocation, wallsRect, Sprites);
                            std::cout << "Labyrinth receive\n";
                            break;
                        case pacetStates::PlayerPos:
                            for (int i = 0; i < ConnectedPlayers.size(); i++) 
                                if (i != ComputerID)
                                    ReceivePacket >> ConnectedPlayers[i];
                                else
                                    ReceivePacket >> tempPoint;
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
                                tempBullet.UpdateCircleShape();
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
    int CounterOfGenerations = 0;
    do {
        LabyrinthLocation.WallGenerator(0.48);
        CountOfEnableTilesOnMap = LabyrinthLocation.BuildWayFrom(int(player.PosX / size), int(player.PosY / size));
        CounterOfGenerations++;
    } while (CountOfEnableTilesOnMap < float(n * m) / 3 || CountOfEnableTilesOnMap > float(n * m) / 1.5);
    std::cout << "total count of generations = " << CounterOfGenerations
              << " CountOfEnableTilesOnMap = " << CountOfEnableTilesOnMap << '\n';
    for (int i = 0; i < LabyrinthLocation.data.size(); i++)
        for (int j = 0; j < LabyrinthLocation[i].size(); j++)
            LabyrinthData << LabyrinthLocation[i][j];
    CreateMapRectByLocation(LabyrinthLocation, wallsRect, Sprites);
    SeenWalls.assign(LabyrinthLocation.data.size(), vb(0));
    for (int i = 0; i < LabyrinthLocation.data.size(); i++)
        SeenWalls[i].assign(LabyrinthLocation[i].size(), false);
    
    MiniMapView.zoom(1 / MiniMapZoom);
    MiniMapZoom = 1;
}

void LoadMainMenu() {
    CurLocation = &MainMenuLocation;
    CreateMapRectByLocation(*CurLocation, wallsRect, Sprites);
    SeenWalls.assign(CurLocation->data.size(), vb(0));
    for (int i = 0; i < CurLocation->data.size(); i++)
        SeenWalls[i].assign(CurLocation->data[i].size(), false);
    
    player.setCenter(1.f * size, 1.f * size);
    player.CurWeapon = nullptr;

    portal.setCenter(size * 3, size);
    portal.setFunction([](){
        screen = screens::Solo;
        Bullets.clear();
        multiplayer = false;
        MiniMapActivated = false;
        MiniMapView.setViewport(sf::FloatRect(0.f, 0.f, 0.25f, 0.25f));
        MiniMapView.setCenter({CurLocation->m * miniSize / 2.f, CurLocation->n * miniSize / 2.f});
        MainMenuMusic.pause();
        CurLocation = &LabyrinthLocation;
        LevelGenerate(START_N, START_M);

        portal.setPosition(float(-200) * size, float(-200) * size);
        portal.setFunction([](){
            LevelGenerate(START_N, START_M);
            portal.setPosition(-10 * size, -10 * size);
        });
    });

    // Set cameras
    GameView.setCenter(player.getCenter());
    MiniMapView.setCenter({CurLocation->m * miniSize / 2.f, CurLocation->n * miniSize / 2.f});
    InterfaceView.setCenter({scw / 2.f, sch / 2.f});

    MainMenuMusic.play();
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
    WaitingRoomLoaction.LoadLocationFromFile("sources/locations/WaitingRoom.txt");
    MainMenuLocation.LoadLocationFromFile("sources/locations/MainMenu.txt");
    
    LoadMainMenu();

    WallRectG.setFillColor(sf::Color(120, 120, 120));
    WallRectV.setFillColor(sf::Color(120, 120, 120));

    // TextFPS.text.setPosition(20, 20);
    // TextFPS.text.setFillColor(sf::Color(255, 255, 255));

    listener.setBlocking(false);
    MyIP = MySocket.getRemoteAddress().getLocalAddress().toString();
    std::cout << "IP: " << MyIP << '\n';
    
    MainMenuButton.setTextSize(110);
    IPPanel.setTextSize(80);
    ListOfPlayers.setTextSize(60);

    SoloButton.setPosition      (scw / 6 -       SoloButton.Width / 4,          sch * 5 / 8);
    CoopButton.setPosition      (scw - scw / 6 - CoopButton.Width * 3 / 4,      sch * 5 / 8);
    HostButton.setPosition      (scw / 6 -       HostButton.Width / 4,          sch * 5 / 8);
    ConnectButton.setPosition   (scw - scw / 6 - ConnectButton.Width * 3 / 4,   sch * 5 / 8);
    MainMenuButton.setPosition  (scw / 2 -       MainMenuButton.Width / 2,      sch * 5 / 8);
    
    IPPanel.setPosition         (scw / 2 -       IPPanel.Width / 2,             scw / 8    );
    ListOfPlayers.setPosition   (scw / 2 -       ListOfPlayers.Width / 2,       scw / 16   );

    // sf::Time time = GlobalClock.getElapsedTime();

    CircleOfSmallPlayer.setRadius(9);
    CircleOfSmallPlayer.setFillColor(sf::Color(0, 180, 0));

    player.FirstWeapon  = &pistol;
    player.SecondWeapon = &shotgun;
}

void EventHandler() {
    while (window.pollEvent(event))
        switch (screen) {
            case screens::MainRoom:
                player.update(event, MiniMapActivated);
                portal.isActivated(player, event);

                if (event.type == sf::Event::KeyPressed) {
                    if (event.key.code == sf::Keyboard::Escape) {
                        if (MiniMapActivated) {
                            MiniMapActivated = false;
                            MiniMapView.setViewport(sf::FloatRect(0.f, 0.f, 0.25f, 0.25f));
                        } else if (portal.isInterfaceDrawn) portal.isInterfaceDrawn = false;
                        else {
                            MainMenuMusic.pause();
                            window.close();
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
                break;

            case screens::Coop:
                if (event.type == sf::Event::KeyPressed)
                    if (event.key.code == sf::Keyboard::Escape) screen = screens::MainRoom;
                HostButton.isActivated(event);
                ConnectButton.isActivated(event);
                break;

            case screens::Host:
                if (chat.inputted) {
                    if (event.type == sf::Event::KeyPressed)
                        if (event.key.code == sf::Keyboard::Escape)
                            chat.inputted = false;
                        else if (event.key.code == sf::Keyboard::Enter)
                            if (chat.Entered()) {
                                mutex.lock();
                                SendPacket << sf::Int32(pacetStates::ChatEvent) << chat.Last();
                                SendToClients(SendPacket);
                                SendPacket.clear();
                                mutex.unlock();
                            }
                    chat.InputText(event);
                } else {
                    player.update(event, MiniMapActivated);
                    if (event.type == sf::Event::KeyPressed) {
                        if (event.key.code == sf::Keyboard::Escape)
                            if (MiniMapActivated) {
                                MiniMapActivated = false;
                                MiniMapView.setViewport(sf::FloatRect(0.f, 0.f, 0.25f, 0.25f));
                            } else screen = screens::EscOfCoop;
                        else if (event.key.code == sf::Keyboard::Enter)
                            chat.Entered();
                        else if (event.key.code == sf::Keyboard::Tab) {
                            MiniMapActivated = !MiniMapActivated;
                            if (MiniMapActivated) MiniMapView.setViewport(sf::FloatRect(0.f, 0.f, 1.f, 1.f));
                            else                  MiniMapView.setViewport(sf::FloatRect(0.f, 0.f, 0.25f, 0.25f));
                        } else if (event.key.code == sf::Keyboard::Num1) player.ChangeWeapon(&pistol);
                        else if (event.key.code == sf::Keyboard::Num2) player.ChangeWeapon(&shotgun);
                        else if (event.key.code == sf::Keyboard::Num3) player.ChangeWeapon(&revolver);
                        else if (event.key.code == sf::Keyboard::Num4) player.ChangeWeapon(&rifle);
                        else if (event.key.code == sf::Keyboard::Num5) player.ChangeWeapon(&bubblegun);
                        else if (event.key.code == sf::Keyboard::Num6) player.ChangeWeapon(&armagedon);
                        else if (event.key.code == sf::Keyboard::Num7) player.ChangeWeapon(&chaotic);
                    } else if (event.type == sf::Event::MouseWheelMoved) {
                        if (MiniMapActivated) {
                            if (event.mouseWheel.delta < 0) { MiniMapView.zoom(1.1f); MiniMapZoom *= 1.1f; }
                            else { MiniMapView.zoom(1.f / 1.1f); MiniMapZoom /= 1.1f; }
                        } else {
                            if (CurLocation->n > 2 || event.mouseWheel.delta > 0) {
                                CurLocation->n += std::max(event.mouseWheel.delta + CurLocation->n, 2);
                                CurLocation->m += std::max(event.mouseWheel.delta + CurLocation->m, 2);
                            }
                            MiniMapView.setCenter({CurLocation->m * miniSize / 2.f, CurLocation->n * miniSize / 2.f});
                            Bullets.clear();
                            LevelGenerate(CurLocation->n, CurLocation->m);
                            mutex.lock();
                            SendToClients(LabyrinthData);
                            for (Player& p: ConnectedPlayers)
                                p.setPosition(player.getPosition());
                            SendPacket << sf::Int32(pacetStates::SetPos);
                            for (Player& x: ConnectedPlayers) SendPacket << x;
                            SendToClients(SendPacket);
                            SendPacket.clear();
                            mutex.unlock();
                        }
                    }
                }
                break;

            case screens::EscOfCoop:
                MainMenuButton.isActivated(event);
                if ((event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape)) {
                    if (multiplayer)
                        screen = HostFuncRun ? screens::Host : screens::Connect;
                    else screen = screens::Solo;
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
                    if (event.key.code == sf::Keyboard::Escape) screen = screens::Coop;
                    else if (event.key.code == sf::Keyboard::Enter) {
                        IPPanel.setWord("Connecting...");
                        draw();
                        if (MySocket.connect(IPOfHost, 53000, sf::milliseconds(300)) == sf::Socket::Done) {
                            screen = screens::Connect;
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
                                        { Player* NewPlayer = new Player();
                                        ConnectedPlayers.push_back(*NewPlayer);
                                        }
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

            case screens::Connect:
                if (chat.inputted) {
                    if (event.type == sf::Event::KeyPressed)
                        if (event.key.code == sf::Keyboard::Escape)
                            chat.inputted = false;
                        else if (event.key.code == sf::Keyboard::Enter)
                            if (chat.Entered()) {
                                mutex.lock();
                                SendPacket << sf::Int32(pacetStates::ChatEvent) << chat.Last();
                                MySocket.send(SendPacket);
                                SendPacket.clear();
                                mutex.unlock();
                            }
                    chat.InputText(event);
                } else {
                    player.update(event, MiniMapActivated);
                    if (event.type == sf::Event::KeyPressed) {
                        if (event.key.code == sf::Keyboard::Escape)
                            if (MiniMapActivated) {
                                MiniMapActivated = false;
                                MiniMapView.setViewport(sf::FloatRect(0.f, 0.f, 0.25f, 0.25f)); 
                            } else screen = screens::EscOfCoop;
                        else if (event.key.code == sf::Keyboard::Enter) chat.Entered();
                        else if (event.key.code == sf::Keyboard::Tab) {
                            MiniMapActivated = !MiniMapActivated;
                            if (MiniMapActivated) MiniMapView.setViewport(sf::FloatRect(0.f, 0.f, 1.f, 1.f));
                            else                  MiniMapView.setViewport(sf::FloatRect(0.f, 0.f, 0.25f, 0.25f));
                        } else if (event.key.code == sf::Keyboard::Num1) player.ChangeWeapon(&pistol);
                        else if (event.key.code == sf::Keyboard::Num2) player.ChangeWeapon(&shotgun);
                        else if (event.key.code == sf::Keyboard::Num3) player.ChangeWeapon(&revolver);
                        else if (event.key.code == sf::Keyboard::Num4) player.ChangeWeapon(&rifle);
                        else if (event.key.code == sf::Keyboard::Num5) player.ChangeWeapon(&bubblegun);
                        else if (event.key.code == sf::Keyboard::Num6) player.ChangeWeapon(&armagedon);
                        else if (event.key.code == sf::Keyboard::Num7) player.ChangeWeapon(&chaotic);
                    } else if (event.type == sf::Event::MouseWheelMoved) {
                        if (MiniMapActivated) {
                            if (event.mouseWheel.delta < 0) { MiniMapView.zoom(1.1f); MiniMapZoom *= 1.1f; }
                            else { MiniMapView.zoom(1.f / 1.1f); MiniMapZoom /= 1.1f; }
                        }
                    }
                }
                break;

            case screens::Solo:
                portal.isActivated(player, event);
                if (chat.inputted) {
                    if (event.type == sf::Event::KeyPressed)
                        if (event.key.code == sf::Keyboard::Escape)
                            chat.inputted = false;
                        else if (event.key.code == sf::Keyboard::Enter)
                            chat.Entered();
                    chat.InputText(event);
                } else {
                    player.update(event, MiniMapActivated);
                    if (event.type == sf::Event::KeyPressed) {
                        if (event.key.code == sf::Keyboard::Escape)
                            if (MiniMapActivated) {
                                MiniMapActivated = false;
                                MiniMapView.setViewport(sf::FloatRect(0.f, 0.f, 0.25f, 0.25f));
                            } else screen = screens::EscOfCoop;
                        else if (event.key.code == sf::Keyboard::Enter)
                            chat.Entered();
                        else if (event.key.code == sf::Keyboard::H) {
                            player.setPosition(size, size);
                            CurLocation = &WaitingRoomLoaction;
                            CreateMapRectByLocation(WaitingRoomLoaction, wallsRect, Sprites);
                        }
                        else if (event.key.code == sf::Keyboard::Tab) {
                            MiniMapActivated = !MiniMapActivated;
                            if (MiniMapActivated) MiniMapView.setViewport(sf::FloatRect(0.f, 0.f, 1.f, 1.f));
                            else                  MiniMapView.setViewport(sf::FloatRect(0.f, 0.f, 0.25f, 0.25f));
                        } else if (event.key.code == sf::Keyboard::Num1) player.ChangeWeapon(&pistol);
                        else if (event.key.code == sf::Keyboard::Num2) player.ChangeWeapon(&shotgun);
                        else if (event.key.code == sf::Keyboard::Num3) player.ChangeWeapon(&revolver);
                        else if (event.key.code == sf::Keyboard::Num4) player.ChangeWeapon(&rifle);
                        else if (event.key.code == sf::Keyboard::Num5) player.ChangeWeapon(&bubblegun);
                        else if (event.key.code == sf::Keyboard::Num6) player.ChangeWeapon(&armagedon);
                        else if (event.key.code == sf::Keyboard::Num7) player.ChangeWeapon(&chaotic);
                    } else if (event.type == sf::Event::MouseWheelMoved) {
                        if (MiniMapActivated) {
                            if (event.mouseWheel.delta < 0) { MiniMapView.zoom(1.1f); MiniMapZoom *= 1.1f; }
                            else { MiniMapView.zoom(1.f / 1.1f); MiniMapZoom /= 1.1f; }
                        } else {
                            CurLocation->n = std::max(event.mouseWheel.delta + CurLocation->n, 2);
                            CurLocation->m = std::max(event.mouseWheel.delta + CurLocation->m, 2);
                            MiniMapView.setCenter({CurLocation->m * miniSize / 2.f, CurLocation->n * miniSize / 2.f});
                            Bullets.clear();
                            LevelGenerate(CurLocation->n, CurLocation->m);
                        }
                    }
                }
                break;
            }
}