#include <SFML/Network.hpp>
#include <iostream>
#include "header.h"

int main() {


    std::string filePath = "maclist.xml";
    std::string mac = "11:22:33:44:55:66";
    std::string password="parola";

    std::string mac2 = "11:22:33:44:55:55";
    std::string password2="parola";

    try {
        addMacPassword(filePath, mac2, password2);
    } catch (const std::exception& e) {
        std::cerr << "Exceptie: " << e.what() << "\n";
    }
    

    sf::TcpListener listener;

    if (listener.listen(54000) != sf::Socket::Done) {
        std::cout << "Eroare la bind!\n";
        return -1;
    }

    std::cout << "Server pornit pe portul 54000...\n";

    sf::TcpSocket client;
    if (listener.accept(client) != sf::Socket::Done) {
        std::cout << "Eroare la accept!\n";
        return -1;
    }

    std::cout << "Client conectat!\n";

    while (true) {
        char data[1024];
        std::size_t received;
        if (client.receive(data, sizeof(data), received) == sf::Socket::Done) {
            std::cout << "Am primit: " << std::string(data, received) << "\n";

            std::string reply = "Server: Task primit!\n";
            client.send(reply.c_str(), reply.size());
        }
    }

    return 0;
}
