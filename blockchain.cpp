#include "blockchain.hpp"
#include <boost/json.hpp>
#include <iostream>

Blockchain::Blockchain() : dificuldade(4) {
    cadeia.push_back(criarBlocoGenesis());
}

Bloco Blockchain::criarBlocoGenesis() {
    std::vector<Transacao> transacoes;
    return Bloco(0, transacoes, "0");
}

void Blockchain::adicionarTransacao(const Transacao& transacao) {
    transacoesPendentes.push_back(transacao);
}

void Blockchain::minerarTransacoesPendentes() {
    Bloco novoBloco(cadeia.size(), transacoesPendentes, getUltimoBloco().getHash());
    novoBloco.minerarBloco(dificuldade);
    cadeia.push_back(novoBloco);
    transacoesPendentes.clear();
}

bool Blockchain::isValid() const {
    for(size_t i = 1; i < cadeia.size(); i++) {
        const Bloco& blocoAtual = cadeia[i];
        const Bloco& blocoAnterior = cadeia[i-1];

        if(blocoAtual.getHash() != blocoAtual.calcularHash()) return false;
        if(blocoAtual.getHashAnterior() != blocoAnterior.getHash()) return false;
    }
    return true;
}

Bloco Blockchain::getUltimoBloco() const {
    return cadeia.back();
}

std::string Blockchain::toJson() const {
    boost::json::array blockchainJson;
    for (const auto& bloco : cadeia) {
        boost::json::object blocoJson;
        blocoJson["index"] = bloco.getIndex();
        blocoJson["nonce"] = bloco.getNonce();
        blocoJson["hash"] = bloco.getHash();
        blocoJson["hashAnterior"] = bloco.getHashAnterior();
        blocoJson["transacoes[]"] = bloco.transactionsToString();
        blockchainJson.push_back(blocoJson);
    }
    return boost::json::serialize(blockchainJson);
}
