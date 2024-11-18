#include "peer.hpp"
#include "iostream"
int main() {
    const std::string serverHost = "127.0.0.1";
    std::string remetente, destinatario;
    double qtd;
    const int serverPort = 8080;

    Peer peer(serverHost, serverPort);

    peer.connectToServer();
    std::cout << "Remetente:";
    std::cin >> remetente;
    std::cout << "Destinatario:";
    std::cin >> destinatario;
    std::cout << "Valor:";
    std::cin >> qtd;
    peer.sendTransaction(remetente, destinatario, qtd);
    peer.requestBlockchain();

    return 0;
}
