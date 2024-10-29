#ifndef BLOCKCHAIN_HPP
#define BLOCKCHAIN_HPP

#include <vector>
#include <string>
#include "bloco.hpp"
#include "transacao.hpp"

class Blockchain {
private:
    std::vector<Bloco> cadeia;
    int dificuldade;
    std::vector<Transacao> transacoesPendentes;
    Bloco criarBlocoGenesis();

public:
    Blockchain();
    void adicionarTransacao(const Transacao& transacao);
    void minerarTransacoesPendentes();
    bool isValid() const;
    Bloco getUltimoBloco() const;
    std::string toJson() const;
};

#endif
