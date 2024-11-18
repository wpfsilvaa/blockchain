#include "controller.hpp"
#include <iostream>
#include <boost/json.hpp>
#include <boost/asio/connect.hpp>
#include <boost/asio/write.hpp>

Controller::Controller(Blockchain& blockchain) : blockchain(blockchain) {}

void Controller::handleRequest(http::request<http::string_body> req, http::response<http::string_body>& res) {
    if (req.method() == http::verb::get && req.target() == "/blockchain") {
        res.result(http::status::ok);
        res.set(http::field::content_type, "application/json");
        res.body() = blockchain.toJson();
    } else if (req.method() == http::verb::post && req.target() == "/add_transaction") {
        auto const& jsonBody = req.body();
        boost::json::value json = boost::json::parse(jsonBody);
        if (!json.is_object()) {
            res.result(http::status::bad_request);
            res.body() = "Invalid JSON: Expected an object.";
            res.prepare_payload();
            return;
        }
        boost::json::object jsonObject = json.as_object();
        try {
            std::string from = jsonObject["from"].as_string().c_str();
            std::string to = jsonObject["to"].as_string().c_str();
            double amount = jsonObject["amount"].as_double();

            Transacao newTransacao(from, to, amount);
            blockchain.adicionarTransacao(newTransacao);
            blockchain.minerarTransacoesPendentes();
            propagateTransaction(newTransacao);

            res.result(http::status::ok);
            res.body() = "Transaction added and propagated successfully\n";
        } catch (...) {
            res.result(http::status::bad_request);
            res.body() = "Invalid transaction data.";
        }
    } else if (req.method() == http::verb::post && req.target() == "/add_peer") {
        handleAddPeer(req, res);
    } else if (req.method() == http::verb::get && req.target() == "/sync") {
        handleSyncRequest(req, res);
    } else {
        res.result(http::status::not_found);
        res.body() = "Endpoint not found.";
    }
    res.prepare_payload();
}

void Controller::runServer() {
    net::io_context ioc;
    tcp::acceptor acceptor(ioc, {tcp::v4(), 8080});

    std::cout << "Blockchain server running on port 8080..." << std::endl;

    while (true) {
        tcp::socket socket(ioc);
        acceptor.accept(socket);

        boost::beast::flat_buffer buffer;
        http::request<http::string_body> req;
        http::read(socket, buffer, req);

        http::response<http::string_body> res;
        handleRequest(req, res);
        http::write(socket, res);
    }
}

void Controller::handleSyncRequest(http::request<http::string_body> req, http::response<http::string_body>& res) {
    res.result(http::status::ok);
    res.set(http::field::content_type, "application/json");
    res.body() = blockchain.toJson();
}

void Controller::handleAddPeer(http::request<http::string_body> req, http::response<http::string_body>& res) {
    auto const& peerAddress = req.body();
    if (peerAddress.empty()) {
        res.result(http::status::bad_request);
        res.body() = "Peer address is missing.";
        return;
    }
    peers.insert(peerAddress);
    res.result(http::status::ok);
    res.body() = "Peer added successfully: " + peerAddress + "\n";
}

void Controller::connectToPeer(const std::string& peerAddress) {
    peers.insert(peerAddress);
    std::cout << "Connected to peer: " << peerAddress << std::endl;
}

void Controller::propagateTransaction(const Transacao& transaction) {
    boost::json::object transactionJson = {
        {"from", transaction.getHash()},
        {"to", transaction.toString()},
        {"amount", transaction.isValid()}
    };
    std::string jsonStr = boost::json::serialize(transactionJson);
    broadcastToPeers("/add_transaction", jsonStr);
}

void Controller::broadcastToPeers(const std::string& endpoint, const std::string& data) {
    for (const auto& peer : peers) {
        try {
            net::io_context ioc;
            tcp::resolver resolver(ioc);
            auto const results = resolver.resolve(peer, "8080");

            tcp::socket socket(ioc);
            net::connect(socket, results.begin(), results.end());

            http::request<http::string_body> req{http::verb::post, endpoint, 11};
            req.set(http::field::host, peer);
            req.set(http::field::content_type, "application/json");
            req.body() = data;
            req.prepare_payload();

            http::write(socket, req);
        } catch (const std::exception& e) {
            std::cerr << "Error broadcasting to peer " << peer << ": " << e.what() << std::endl;
        }
    }
}
