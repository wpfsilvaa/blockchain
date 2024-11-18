#ifndef CONTROLLER_HPP
#define CONTROLLER_HPP

#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <set>
#include <string>
#include "blockchain.hpp"

namespace http = boost::beast::http;
namespace net = boost::asio;
using tcp = boost::asio::ip::tcp;

class Controller {
public:
    Controller(Blockchain& blockchain);
    void handleRequest(http::request<http::string_body> req, http::response<http::string_body>& res);
    void runServer();
    void connectToPeer(const std::string& peerAddress);
    void propagateTransaction(const Transacao& transaction);

private:
    Blockchain& blockchain;
    std::set<std::string> peers; // Lista de peers conectados
    void handleSyncRequest(http::request<http::string_body> req, http::response<http::string_body>& res);
    void handleAddPeer(http::request<http::string_body> req, http::response<http::string_body>& res);
    void broadcastToPeers(const std::string& endpoint, const std::string& data);
};

#endif // CONTROLLER_HPP
