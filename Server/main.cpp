#include "blockchain.hpp"
#include "controller.hpp"
#include <iostream>

int main() {
    Blockchain blockchain;
    std::cout << "A blockchain é válida? " << std::boolalpha << blockchain.isValid() << std::endl;
    std::cout << "Servidor iniciado em http://localhost:8080" << std::endl;
    Controller controller(blockchain);
    controller.runServer();

    return 0;
}
