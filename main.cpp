#include "blockchain.hpp"
#include "controller.hpp"
#include <iostream>

int main() {
    Blockchain blockchain;
    /*
    std::cout << "Minerando bloco 1..." << std::endl;
    blockchain.adicionarTransacao(Transacao("Alice", "Bob", 50));
    blockchain.minerarTransacoesPendentes("minerador1");

    std::cout << "Minerando bloco 2..." << std::endl;
    blockchain.adicionarTransacao(Transacao("Bob", "Charlie", 30));
    blockchain.minerarTransacoesPendentes("minerador1");

    std::cout << "A blockchain é válida? " << std::boolalpha << blockchain.isValid() << std::endl;
    */
    std::cout << "Servidor iniciado em http://localhost:8080" << std::endl;
    Controller controller(blockchain);
    controller.runServer();

    return 0;
}
