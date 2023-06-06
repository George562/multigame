#include "Tools.h"
// -lsfml-graphics -lsfml-window -lsfml-system -lsfml-network
sf::Mouse Mouse;
sf::SocketSelector selector;
sf::Vector2i mouse(1, 1);
sf::TcpSocket socket;
std::string serverIP;
sf::Packet pacet;
str ListOfPlayers;
int main() {
    srand(time(0)); rand();
    std::cout << "input server IP:";
    while (true) {
        std::cin >> serverIP;
        std::cout << "Connecting...\n";
        if (socket.connect(serverIP, 53000, sf::seconds(2)) == sf::Socket::Done)
            break;
        else std::cout << "server not found, try again:";
    }
    selector.add(socket);
    std::cout << "Success\n";
    while (mouse.x * mouse.y != 0) {
        if (selector.wait(sf::seconds(1)))
            if (selector.isReady(socket) && socket.receive(pacet) == sf::Socket::Done) {
                std::cout << "datasize = " << pacet.getDataSize() << '\n';
                ListOfPlayers.clear();
                while (!pacet.endOfPacket())
                    pacet >> ListOfPlayers;
                if (ListOfPlayers == "disconnect")
                    break;
                else std::cout << "list of players:\n" << ListOfPlayers << '\n';
                pacet.clear();
            }
    }
    pacet << "disconnect";
    socket.send(pacet);
    pacet.clear();
    socket.disconnect();
    return 0;
}