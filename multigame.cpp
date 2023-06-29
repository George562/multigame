#include "Headers/button.h"
#include "Headers/player.h"
#include "Headers/client.h"
#include "Headers/chat.h"

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
screens::screens screen = screens::Main;

//////////////////////////////////////////////////////////// Online tools
sf::TcpListener listener;
sf::Packet ReceivePacket, SendPacket, LabirintData;
std::vector<Client*> clients(0);
sf::SocketSelector selector;
str ClientState, IPOfHost, MyIP, PacetData;
Client MySocket; // this computer socket
int packetState, ComputerID;
sf::Mutex mutex;

//////////////////////////////////////////////////////////// Buttons
Button SoloButton       ("sources/RedPanel",    "Play"      );
Button NewGameButton    ("sources/GreenPanel",  "New Game"  );
Button ContinueButton   ("sources/BluePanel",   "Continue"  );
Button CoopButton       ("sources/RedPanel",    "Online"    );
Button HostButton       ("sources/GreenPanel",  "Host"      );
Button ConnectButton    ("sources/BluePanel",   "Connect"   );
Button MainMenuButton   ("sources/RedPanel",    "Exit"      );
Button EnterButton      ("sources/RedPanel",    "Enter"      );

//////////////////////////////////////////////////////////// Pannels
Panel  IPPanel          ("sources/YellowPanel", "IP:"       );
Panel  ListOfPlayers    ("sources/SteelFrame"               );

//////////////////////////////////////////////////////////// Locations
location* CurLocation = nullptr;
location LabirintWalls(0), WaitingRoomWalls(0);

//////////////////////////////////////////////////////////// System stuff
bool ClientFuncRun, HostFuncRun;
sf::Vector2f CameraPos(0, 0), miniCameraPos((scw - m * miniSize) / 2, (sch - n * miniSize) / 2);
float WallMinSize = size / 8, WallMaxSize = size;
vvr wallsRect(0);
std::vector<sf::Sprite> Sprites(0);
sf::Sprite tempSprite;
vp ways(0);
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

//////////////////////////////////////////////////////////// functions
void draw();
void ClientConnect();
void ClientDisconnect(int);
void SelfDisconnect();
void SendToClients(sf::Packet&);
void SendToOtherClients(sf::Packet&, int);
void funcOfHost();
void funcOfClient();
void LevelGenerate();
void drawWalls();

//////////////////////////////////////////////////////////// the program
int main() {
    setlocale(LC_ALL, "rus");

    init();

    window.setFramerateLimit(60);
    window.setVerticalSyncEnabled(true);
    settings.antialiasingLevel = 8;
    window.setView(GameView);


    // Load locations
    LoadLocationFromFile(WaitingRoomWalls, "sources/locations/WaitingRoom.txt");
    CreateWallRectByLocation(WaitingRoomWalls, wallsRect, Sprites, size, WallMinSize);

    CurLocation = &LabirintWalls;

    WallRectG.setFillColor(sf::Color(120, 120, 120));
    WallRectV.setFillColor(sf::Color(120, 120, 120));

    // TextFPS.text.setPosition(20, 20);
    // TextFPS.text.setFillColor(sf::Color(255, 255, 255));

    setlocale(LC_ALL, "rus");
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

    sf::Clock clock;
    sf::Time time = clock.getElapsedTime();

    player.Clock  = &clock;
    player.Camera  = &CameraPos;

    chat.Clock    = &clock;

    CircleOfSmallPlayer.setRadius(9);
    CircleOfSmallPlayer.setFillColor(sf::Color(0, 180, 0));

    sf::Thread HostTread(funcOfHost);
    sf::Thread ClientTread(funcOfClient);

    if (!LoadLocationFromFile(LabirintWalls, "save/LabirintWalls.dat"))
        LevelGenerate();
    else
        CreateWallRectByLocation(LabirintWalls, wallsRect, Sprites, size, WallMinSize);

    Pistol pistol;
    Shotgun shotgun;
    Revolver revolver;
    Rifle rifle;
    Bubblegun bubblegun;
    Armagedon armagedon;
    Chaotic chaotic;
    player.FirstWeapon  = &pistol;
    player.SecondWeapon = &shotgun;
    player.CurWeapon = player.FirstWeapon;

    while (window.isOpen()) {
        if (!window.hasFocus()) {
            if (HostFuncRun) {
                mutex.lock();
                SendPacket << pacetStates::PlayerPos;
                for (Player& x: ConnectedPlayers) SendPacket << x;
                SendToClients(SendPacket);
                SendPacket.clear();
                mutex.unlock();
            }
            player.update(Bullets);
            for (int i = 0; i < Bullets.size();) {
                int y = (int(Bullets[i].PosY) / size) * 2, x = (int(Bullets[i].PosX) / size) * 2;
                if (!(0 <= x && x < BigM && 0 <= y && y < BigN) || Bullets[i].penetration < 0 || Bullets[i].todel) {
                    Bullets.erase(Bullets.begin() + i--); continue;
                }
                Bullets[i++].move(wallsRect, size, clock);
            }
            while (window.pollEvent(event)) {}
            draw();
            // TextFPS.text.setString(std::to_string((int)std::round(1.f / (clock.getElapsedTime() - time).asSeconds())) + " fps");
            time = clock.getElapsedTime();
        } else {
            if (screen == screens::Solo || screen == screens::Host || screen == screens::Connect) {
                if (!chat.inputted) {
                    player.move(wallsRect, size);
                    CameraPos = {player.PosX - scw / 2 + player.radius, player.PosY - sch / 2 + player.radius};
                }
                int wasBulletsCount = Bullets.size();
                player.update(Bullets);
                if (wasBulletsCount < Bullets.size() && (screen == screens::Host || screen == screens::Connect)) {
                    mutex.lock();
                    SendPacket << pacetStates::Shooting << (int)Bullets.size() - wasBulletsCount;
                    for (; wasBulletsCount < Bullets.size(); wasBulletsCount++) SendPacket << Bullets[wasBulletsCount];
                    if (HostFuncRun)        SendToClients(SendPacket);
                    else if (ClientFuncRun) MySocket.send(SendPacket);
                    SendPacket.clear();
                    mutex.unlock();
                }
                size_t CountOfBulletsOtOfScreen = 0;
                for (size_t i = 0; i < Bullets.size() - CountOfBulletsOtOfScreen;) {
                    if (Bullets[i].penetration < 0 || Bullets[i].todel) {
                        std::swap(Bullets[i], Bullets[Bullets.size() - 1 - CountOfBulletsOtOfScreen]);
                        CountOfBulletsOtOfScreen++;
                        continue;
                    } else
                        Bullets[i++].move(wallsRect, size, clock);
                }
                Bullets.resize(Bullets.size() - CountOfBulletsOtOfScreen);
            }
            if (HostFuncRun || ClientFuncRun) {
                ConnectedPlayers[ComputerID].setPosition(player.getPosition());
                mutex.lock();
                SendPacket << pacetStates::PlayerPos;
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
                if (Mouse.isButtonPressed(Mouse.Left))
                    miniCameraPos += sf::Vector2f(Mouse.getPosition() - MouseBuffer) * MiniMapZoom;
            }
            MouseBuffer = Mouse.getPosition();
            draw();
            // TextFPS.text.setString(std::to_string((int)std::round(1.f / (clock.getElapsedTime() - time).asSeconds())) + " fps");
            time = clock.getElapsedTime();
        }

        while (window.pollEvent(event))
            switch (screen) {
            case screens::Main:
                if (event.type == sf::Event::KeyPressed)
                    if (event.key.code == sf::Keyboard::Escape) window.close();
                if (CoopButton.isActivated(event)) {
                    screen = screens::Coop;
                    multiplayer = true;
                    MiniMapActivated = false;
                    MiniMapView.setViewport(sf::FloatRect(0.f, 0.f, 0.25f, 0.25f));
                    miniCameraPos = {float(scw - m * miniSize) / 2, float(sch - n * miniSize) / 2};
                } else if (SoloButton.isActivated(event)) {
                    screen = screens::Solo;
                    Bullets.clear();
                    LevelGenerate();
                    multiplayer = false;
                    MiniMapActivated = false;
                    MiniMapView.setViewport(sf::FloatRect(0.f, 0.f, 0.25f, 0.25f));
                    miniCameraPos = {float(scw - m * miniSize) / 2, float(sch - n * miniSize) / 2};
                }
                break;

            case screens::Coop:
                if (event.type == sf::Event::KeyPressed)
                    if (event.key.code == sf::Keyboard::Escape) screen = screens::Main;
                if (HostButton.isActivated(event)) { 
                    screen = screens::Host;
                    listener.listen(53000);
                    selector.add(listener);
                    ListOfPlayers.setWord(MyIP);
                    Bullets.clear();
                    LevelGenerate();
                    ComputerID = 0;
                    { Player* NewPlayer = new Player();
                      NewPlayer->Camera = &CameraPos;
                      ConnectedPlayers.push_back(*NewPlayer);
                      }
                    HostFuncRun = true;
                    HostTread.launch();
                }
                if (ConnectButton.isActivated(event)) screen = screens::SetIP;
                break;

            case screens::Host:
                if (chat.inputted) {
                    if (event.type == sf::Event::KeyPressed)
                        if (event.key.code == sf::Keyboard::Escape)
                            chat.inputted = false;
                        else if (event.key.code == sf::Keyboard::Enter)
                            if (chat.Enterred()) {
                                mutex.lock();
                                SendPacket << pacetStates::ChatEvent << chat.Last();
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
                            chat.Enterred();
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
                            if (n > 2 || event.mouseWheel.delta > 0) {
                                n += event.mouseWheel.delta;
                                m += event.mouseWheel.delta;
                            }
                            miniCameraPos = {float(scw - m * miniSize) / 2, float(sch - n * miniSize) / 2};
                            Bullets.clear();
                            LevelGenerate();
                            mutex.lock();
                            SendToClients(LabirintData);
                            for (Player& p: ConnectedPlayers)
                                p.setPosition(player.getPosition());
                            SendPacket << pacetStates::SetPos;
                            for (Player& x: ConnectedPlayers) SendPacket << x;
                            SendToClients(SendPacket);
                            SendPacket.clear();
                            mutex.unlock();
                        }
                    }
                }
                break;

            case screens::EscOfCoop:
                if (MainMenuButton.isActivated(event)) {
                    if (multiplayer) {
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
                        } else
                            SelfDisconnect();
                    }
                    screen = screens::Main;
                    ListOfPlayers.clear();
                    Bullets.clear();
                } else if ((event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape)) {
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
                            ClientFuncRun = true;
                            
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
                                          NewPlayer->Camera = &CameraPos;
                                          ConnectedPlayers.push_back(*NewPlayer);
                                          }
                                        std::cout << PacetData << " connected\n";
                                        break;
                                    case pacetStates::SetPos:
                                        for (Player& x: ConnectedPlayers) ReceivePacket >> x;
                                        player.setPosition(ConnectedPlayers[ComputerID].getPosition());
                                    }
                                }
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
                            if (chat.Enterred()) {
                                mutex.lock();
                                SendPacket << pacetStates::ChatEvent << chat.Last();
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
                        else if (event.key.code == sf::Keyboard::Enter) chat.Enterred();
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
                if (chat.inputted) {
                    if (event.type == sf::Event::KeyPressed)
                        if (event.key.code == sf::Keyboard::Escape)
                            chat.inputted = false;
                        else if (event.key.code == sf::Keyboard::Enter)
                            chat.Enterred();
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
                            chat.Enterred();
                        else if (event.key.code == sf::Keyboard::H) {
                            n = WaitingRoomWalls.size() / 2;
                            m = WaitingRoomWalls[0].size() / 2;
                            player.setPosition(size, size);
                            CurLocation = &WaitingRoomWalls;
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
                            n = std::max(event.mouseWheel.delta + n, 2);
                            m = std::max(event.mouseWheel.delta + m, 2);
                            miniCameraPos = {float(scw - m * miniSize) / 2, float(sch - n * miniSize) / 2};
                            Bullets.clear();
                            LevelGenerate();
                        }
                    }
                }
                break;
            }
    }
    return 0;
}

void draw() {
    window.clear(sf::Color::White);
    switch (screen) {
    case screens::Main:
        SoloButton.draw(window);
        CoopButton.draw(window);
        break;
    case screens::Solo:
        window.clear(sf::Color::Black);
        for (sf::Sprite& s: Sprites) {
            sf::FloatRect fr = s.getGlobalBounds();
            if (fr.intersects({CameraPos.x, CameraPos.x, float(scw), float(sch)})) {
                s.setPosition(s.getPosition() - CameraPos);
                window.draw(s);
                s.setPosition(s.getPosition() + CameraPos);
            }
        }
        player.draw(window);
        drawWalls();
        for (int i = 0; i < Bullets.size(); i++)
            Bullets[i].draw(window, CameraPos);
        // draw minimap
        window.setView(MiniMapView);
        CircleOfSmallPlayer.setPosition(player.getPosition() / float(size) * float(miniSize) + miniCameraPos);
        window.draw(CircleOfSmallPlayer);
        window.setView(GameView);
        
        player.interface(window);
        chat.draw(window);
        break;
    case screens::Coop:
        HostButton.draw(window);
        ConnectButton.draw(window);
        break;
    case screens::Connect:
        window.clear(sf::Color::Black);
        for (Player& p: ConnectedPlayers) {
            for (sf::Sprite& s: Sprites) {
                sf::FloatRect fr = s.getGlobalBounds();
                if (fr.intersects({CameraPos.x, CameraPos.x, float(scw), float(sch)})) {
                    s.setPosition(s.getPosition() - CameraPos);
                    window.draw(s);
                    s.setPosition(s.getPosition() + CameraPos);
                }
            }
            p.draw(window);
            for (int i = 0; i < Bullets.size(); i++)
                Bullets[i].draw(window, CameraPos);
            // draw minimap
            window.setView(MiniMapView);
            CircleOfSmallPlayer.setPosition(p.getPosition() / float(size) * float(miniSize) + miniCameraPos);
            window.draw(CircleOfSmallPlayer);
            window.setView(GameView);
        }
        drawWalls();
        player.interface(window);
        chat.draw(window);
        break;
    case screens::Host:
        window.clear(sf::Color::Black);
        for (Player& p: ConnectedPlayers) {
            for (sf::Sprite& s: Sprites) {
                sf::FloatRect fr = s.getGlobalBounds();
                if (fr.intersects({CameraPos.x, CameraPos.x, float(scw), float(sch)})) {
                    s.setPosition(s.getPosition() - CameraPos);
                    window.draw(s);
                    s.setPosition(s.getPosition() + CameraPos);
                }
            }
            p.draw(window);
            for (int i = 0; i < Bullets.size(); i++)
                Bullets[i].draw(window, CameraPos);
            // draw minimap
            window.setView(MiniMapView);
            CircleOfSmallPlayer.setPosition(p.getPosition() / float(size) * float(miniSize) + miniCameraPos);
            window.draw(CircleOfSmallPlayer);
            window.setView(GameView);
        }
        drawWalls();
        player.interface(window);
        chat.draw(window);
        break;
    case screens::EscOfCoop:
        window.clear(sf::Color::Black);
        for (Player& p: ConnectedPlayers)
            p.draw(window);
        for (sf::Sprite& s: Sprites) {
            sf::FloatRect fr = s.getGlobalBounds();
            if (fr.intersects({CameraPos.x, CameraPos.x, float(scw), float(sch)})) {
                s.setPosition(s.getPosition() - CameraPos);
                window.draw(s);
                s.setPosition(s.getPosition() + CameraPos);
            }
        }
        drawWalls();
        chat.draw(window);
        ListOfPlayers.draw(window);
        MainMenuButton.draw(window);
        break;
    case screens::SetIP:
        IPPanel.draw(window);
        break;
    }
    // window.draw(TextFPS.text);
    window.display();
}

void ClientConnect() {
    Client* client = new Client;
    if (listener.accept(*client) == sf::Socket::Done) {
        mutex.lock();

        str ConnectedClientIP = (*client).getRemoteAddress().toString();
        SendPacket << pacetStates::PlayerConnect << ConnectedClientIP;
        SendToClients(SendPacket);
        SendPacket.clear();

        std::cout << ConnectedClientIP << " connected\n";
        ListOfPlayers.addWord(ConnectedClientIP);
        std::cout << "list of players:\n" << ListOfPlayers.getWord() << '\n';

        clients.push_back(client);
        selector.add(*client);

        Player* NewPlayer = new Player();
        NewPlayer->Camera = &CameraPos;
        NewPlayer->setPosition({float(m - m % 2 + 1) * size / 2, float(n - n % 2 + 1) * size / 2});
        ConnectedPlayers.push_back(*NewPlayer);
        SendPacket << pacetStates::PlayersAmount << (int)ConnectedPlayers.size() - 1;

        for (int i = 0; i < ListOfPlayers.size(); i++)
            SendPacket << pacetStates::PlayerConnect << ListOfPlayers[i];

        SendPacket << pacetStates::Shooting << (int)Bullets.size();
        std::cout << "bullets: " << Bullets.size() << "\n";
        for (int i = 0; i < Bullets.size(); i++) SendPacket << Bullets[i];

        std::cout << "amount players = " << ConnectedPlayers.size() - 1 << '\n';
        SendPacket << pacetStates::SetPos;
        for (Player& x: ConnectedPlayers) SendPacket << x;

        if (client->send(SendPacket) == sf::Socket::Done) std::cout << "SendPacket was sended\n";
        else std::cout << "SendPacket didn't sended\n";

        SendPacket.clear();
        if (client->send(LabirintData) == sf::Socket::Done) std::cout << "Labirint data sended\n";
        else std::cout << "Labirint data didn't sended\n";

        mutex.unlock();
    } else delete client;
}

void ClientDisconnect(int i) {
    selector.remove(*clients[i]);
    std::cout << (*clients[i]).getRemoteAddress().toString() << " disconnected; numver = " << i << "\n";
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
    screen = screens::Main;
    mutex.lock();
    SendPacket << pacetStates::disconnect;
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
                                    SendToOtherClients(SendPacket, i);
                                    SendPacket.clear();
                                    mutex.unlock();
                                    break;
                                case pacetStates::Shooting: {
                                    mutex.lock();
                                    int i; ReceivePacket >> i;
                                    SendPacket << pacetStates::Shooting << i;
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
                            SelfDisconnect();
                            break;
                        case pacetStates::PlayerConnect:
                            ReceivePacket >> PacetData;
                            ListOfPlayers.addWord(PacetData);
                            { Player* NewPlayer = new Player();
                            NewPlayer->Camera = &CameraPos;
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
                        case pacetStates::Labirint:
                            std::cout << "Labirint receiving\n";
                            ReceivePacket >> n >> m;
                            std::cout << "n = " << n << " m = " << m << '\n';
                            LabirintWalls.assign(BigN, vu(BigM, 0));
                            wallsRect.assign(BigN, vr(BigM, Rect{-1, -1, -1, -1}));
                            bool tempBool;
                            for (int i = 0; i <= n * 2; i++)
                                for (int j = 0; j <= m * 2; j++) {
                                    ReceivePacket >> tempBool; LabirintWalls[i][j] = tempBool;
                                    if (LabirintWalls[i][j]) {
                                        if (i % 2 == 1) // |
                                            wallsRect[i][j].setRect((size * j - WallMinSize) / 2, size * (i - 1) / 2.f, WallMinSize, float(size));
                                        else // -
                                            wallsRect[i][j].setRect(size * (j - 1) / 2.f, (size * i - WallMinSize) / 2, float(size), WallMinSize);
                                    }
                            }
                            std::cout << "Labirint receive\n";
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
                            CameraPos = {player.PosX - scw / 2 + player.radius, player.PosY - sch / 2 + player.radius};
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

void LevelGenerate() {
    LabirintData.clear();
    LabirintData << pacetStates::Labirint << n << m;
    player.setPosition({float(m - m % 2 + 1) * size / 2, float(n - n % 2 + 1) * size / 2});
    size_t CounterOfGenerations = 0;
    do {
        generation(LabirintWalls, n, m, 0.48);
        find_ways(ways, LabirintWalls, m - m % 2 + 1, n - n % 2 + 1, n, m);
        CounterOfGenerations++;
    } while (ways.size() < float(n * m) / 3 || ways.size() > float(n * m) / 1.5);
    std::cout << "total count of generations = " << CounterOfGenerations <<'\n';
    wallsRect.assign(BigN, vr(BigM));
    for (int i = 0; i <= n * 2; i++)
        for (int j = 0; j <= m * 2; j++) {
            if (LabirintWalls[i][j] == LocationIndex::wall) {
                if (i % 2 == 1) // |
                    wallsRect[i][j].setRect((size * j - WallMinSize) / 2, size * (i - 1) / 2.f, WallMinSize, float(size));
                else // -
                    wallsRect[i][j].setRect(size * (j - 1) / 2.f, (size * i - WallMinSize) / 2, float(size), WallMinSize);
            } else  {
                wallsRect[i][j].setRect(-1, -1, -1, -1);
                if (LabirintWalls[i][j] == LocationIndex::box) {
                    tempSprite.setTexture(Box);
                    tempSprite.setPosition(size * j / 2, size * i / 2);
                    Sprites.push_back(tempSprite);
                }
            }
            LabirintData << LabirintWalls[i][j];
        }
}

void drawWalls() {
    // draw main wall
    for (int i = std::max(0, 2 * int(CameraPos.y) / size - 1);
            i <= std::min(2 * n, 2 * int(CameraPos.y + sch + WallMinSize) / size + 1); i++)
        for (int j = std::max(0, 2 * int(CameraPos.x) / size - 1);
                j <= std::min(2 * m, 2 * int(CameraPos.x + scw + WallMinSize) / size + 1); j++)
            if ((*CurLocation)[i][j] == LocationIndex::wall)
                if (i % 2 == 1) { // |
                    WallRectV.setPosition(sf::Vector2f(wallsRect[i][j].PosX, wallsRect[i][j].PosY) - CameraPos);
                    window.draw(WallRectV);
                } else { // -
                    WallRectG.setPosition(sf::Vector2f(wallsRect[i][j].PosX, wallsRect[i][j].PosY) - CameraPos);
                    window.draw(WallRectG);
                }
    // draw minimap wall
    window.setView(MiniMapView);
    for (int i = 0; i <= n * 2; i++)
        for (int j = 0; j <= m * 2; j++)
            if ((*CurLocation)[i][j] == LocationIndex::wall)
                if (i % 2 == 1) { // |
                sf::Vertex line[2] = {
                    sf::Vertex(sf::Vector2f((miniSize * j) / 2, miniSize * (i - 1) / 2) + miniCameraPos),
                    sf::Vertex(sf::Vector2f((miniSize * j) / 2, miniSize * (i + 1) / 2) + miniCameraPos)
                };
                window.draw(line, 2, sf::Lines);
                } else { // -
                sf::Vertex line[2] = {
                    sf::Vertex(sf::Vector2f(miniSize * (j - 1) / 2, (miniSize * i) / 2) + miniCameraPos),
                    sf::Vertex(sf::Vector2f(miniSize * (j + 1) / 2, (miniSize * i) / 2) + miniCameraPos)
                };
                window.draw(line, 2, sf::Lines);
                }
    window.setView(GameView);
}
