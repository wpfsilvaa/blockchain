#ifndef CONTROLLER_HPP
#define CONTROLLER_HPP

#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <boost/asio/ip/tcp.hpp>
#include "blockchain.hpp"

namespace http = boost::beast::http;
namespace net = boost::asio;
using tcp = boost::asio::ip::tcp;

class Controller {
public:
    Controller(Blockchain& blockchain);
    void handleRequest(http::request<http::string_body> req, http::response<http::string_body>& res);
    void runServer();

private:
    Blockchain& blockchain;
};

#endif // CONTROLLER_HPP
