#include "peer.hpp"
#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/json.hpp>
#include <iostream>

namespace http = boost::beast::http;
namespace net = boost::asio;
using tcp = boost::asio::ip::tcp;

Peer::Peer(const std::string& serverHost, int serverPort) 
    : serverHost(serverHost), serverPort(serverPort) {}

void Peer::connectToServer() {
    try {
        net::io_context ioc;
        tcp::resolver resolver(ioc);
        auto const results = resolver.resolve(serverHost, std::to_string(serverPort));
        tcp::socket socket(ioc);
        net::connect(socket, results.begin(), results.end());

        http::request<http::string_body> req{http::verb::post, "/add_peer", 11};
        req.set(http::field::host, serverHost);
        req.set(http::field::content_type, "text/plain");
        req.body() = "localhost";
        req.prepare_payload();

        http::write(socket, req);

        boost::beast::flat_buffer buffer;
        http::response<http::string_body> res;
        http::read(socket, buffer, res);

        std::cout << "Connected to server: " << res.body() << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "Failed to connect to server: " << e.what() << std::endl;
    }
}

void Peer::sendTransaction(const std::string& from, const std::string& to, double amount) {
    try {
        net::io_context ioc;
        tcp::resolver resolver(ioc);
        auto const results = resolver.resolve(serverHost, std::to_string(serverPort));
        tcp::socket socket(ioc);
        net::connect(socket, results.begin(), results.end());

        boost::json::object transaction = {
            {"from", from},
            {"to", to},
            {"amount", amount}
        };

        http::request<http::string_body> req{http::verb::post, "/add_transaction", 11};
        req.set(http::field::host, serverHost);
        req.set(http::field::content_type, "application/json");
        req.body() = boost::json::serialize(transaction);
        req.prepare_payload();

        http::write(socket, req);

        boost::beast::flat_buffer buffer;
        http::response<http::string_body> res;
        http::read(socket, buffer, res);

        std::cout << "Transaction response: " << res.body() << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "Failed to send transaction: " << e.what() << std::endl;
    }
}

void Peer::requestBlockchain() {
    try {
        net::io_context ioc;
        tcp::resolver resolver(ioc);
        auto const results = resolver.resolve(serverHost, std::to_string(serverPort));
        tcp::socket socket(ioc);
        net::connect(socket, results.begin(), results.end());

        http::request<http::string_body> req{http::verb::get, "/blockchain", 11};
        req.set(http::field::host, serverHost);
        req.prepare_payload();

        http::write(socket, req);

        boost::beast::flat_buffer buffer;
        http::response<http::string_body> res;
        http::read(socket, buffer, res);

        std::cout << "Blockchain data: " << res.body() << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "Failed to request blockchain: " << e.what() << std::endl;
    }
}
