#ifndef BLOCO_HPP
#define BLOCO_HPP

#include <vector>
#include <string>
#include "transacao.hpp"

class Bloco {
private:
    int index;
    std::vector<Transacao> transacoes;
    std::string timestamp;
    std::string hashAnterior;
    std::string hash;
    int nonce;

public:
    std::string calcularHash() const;
    Bloco(int idx, const std::vector<Transacao>& trans, const std::string& hashAnt);
    void minerarBloco(int dificuldade);
    std::string getHash() const;
    std::string getHashAnterior() const;
    int getIndex() const;
    std::vector<Transacao> getTransacoes();
    std::string transactionsToString() const;
    int getNonce() const;
};

#endif
