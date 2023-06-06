#include "Tools.h"
// -lsfml-graphics -lsfml-window -lsfml-system -lsfml-network
sf::TcpListener listener;
sf::TcpSocket socket;
std::vector<sf::TcpSocket*> clients(0);
sf::SocketSelector selector;
sf::Packet pacet;
str ClientState, AllClients;
sf::Mutex mutex;

void ClientConnect();
void ClientDisconnect(int);
void SendToClients(str);

int main() {
    listener.setBlocking(false);
    if (listener.listen(53000) != sf::Socket::Done)
        std::cout << "bruh\n";
    selector.add(listener);
    std::cout << "Waiting for clients...\n";
    AllClients = socket.getRemoteAddress().getLocalAddress().toString();
    while (true) {
        if (selector.wait(sf::seconds(1))) {
            if (selector.isReady(listener)) {
                mutex.lock();
                ClientConnect();
                mutex.unlock();
            } else
                for (int i = 0; i < clients.size(); i++)
                    if (selector.isReady(*clients[i]) && clients[i]->receive(pacet) == sf::Socket::Done) {
                        pacet >> ClientState;
                        if (ClientState == "disconnect") {
                            mutex.lock();
                            ClientDisconnect(i--);
                            mutex.unlock();
                        }
                        pacet.clear();
                    }
        }
    }
    return 0;
}

void ClientConnect() {
    sf::TcpSocket* client = new sf::TcpSocket;
    if (listener.accept(*client) == sf::Socket::Done) {
        clients.push_back(client);
        selector.add(*client);
        str ConnectedClientIP = (*client).getRemoteAddress().toString();
        std::cout << ConnectedClientIP << " connected\n";
        mutex.lock();
        AllClients = AllClients + "\n" + ConnectedClientIP;
        SendToClients(AllClients);
        mutex.unlock();
    } else delete client;
}

void ClientDisconnect(int i) {
    selector.remove(*clients[i]);
    str DisconnectedClientIP = (*clients[i]).getRemoteAddress().toString();
    std::cout << DisconnectedClientIP << " disconnected\n";
    for (; i < clients.size(); i++)
        std::swap(clients[i], clients[i + 1]);
    clients.pop_back();
    
    int first = AllClients.rfind(DisconnectedClientIP);
    AllClients.erase(first - 1, DisconnectedClientIP.size() + 1);
    SendToClients(AllClients);
}

void SendToClients(str data) {
    pacet << data;
    std::cout << "datasize = " << pacet.getDataSize() << '\n';
    for (int i = 0; i < clients.size(); i++) {
        if (clients[i]->send(pacet) != sf::Socket::Done) {
            ClientDisconnect(i--);
            break;
        }
    }
}
