#include "peer.hpp"

int main() {
    const std::string serverHost = "127.0.0.1";
    const int serverPort = 8080;

    Peer peer(serverHost, serverPort);

    peer.connectToServer();
    peer.sendTransaction("Alice", "Bob", 50.0);
    peer.requestBlockchain();

    return 0;
}
