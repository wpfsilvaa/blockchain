#ifndef PEER_HPP
#define PEER_HPP

#include <string>

class Peer {
public:
    Peer(const std::string& serverHost, int serverPort);
    void connectToServer();
    void sendTransaction(const std::string& from, const std::string& to, double amount);
    void requestBlockchain();

private:
    std::string serverHost;
    int serverPort;
};

#endif // PEER_HPP
