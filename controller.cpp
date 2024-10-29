#include "controller.hpp"
#include <iostream>
#include <boost/json.hpp>

Controller::Controller(Blockchain& blockchain) : blockchain(blockchain) {}

void Controller::handleRequest(http::request<http::string_body> req, http::response<http::string_body>& res) {
    if (req.method() == http::verb::get && req.target() == "/blockchain") {
        res.result(http::status::ok);
        res.set(http::field::content_type, "application/json");
        res.body() = blockchain.toJson();
    }
    else if(req.method() == http::verb::post && req.target() == "/add_transaction"){
        auto const &jsonBody = req.body();
        boost::json::value json = boost::json::parse(jsonBody);
        if (!json.is_object()) { 
            throw std::runtime_error("Invalid JSON: Expected an object.");
        }
        boost::json::object jsonObject = json.as_object();

        std::string from = jsonObject["from"].as_string().c_str();
        std::string to = jsonObject["to"].as_string().c_str();
        float amount = jsonObject["amount"].as_double();
        blockchain.adicionarTransacao(Transacao(from, to, amount));
        blockchain.minerarTransacoesPendentes();
        res.result(http::status::ok);
        res.body() = "Transacao adicionada com sucesso\n";
    }
    else {
        res.result(http::status::not_found);
        res.body() = "Endpoint not found";
    }
    res.prepare_payload();
}

void Controller::runServer() {
    net::io_context ioc;
    tcp::acceptor acceptor(ioc, {tcp::v4(), 8080});

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
