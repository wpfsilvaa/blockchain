#include "bloco.hpp"
#include <sstream>
#include <iomanip>
#include <openssl/sha.h>
#include <openssl/evp.h>
#include <iostream>
#include <ctime>

Bloco::Bloco(int idx, const std::vector<Transacao>& trans, const std::string& hashAnt)
    : index(idx), transacoes(trans), hashAnterior(hashAnt), nonce(0) {
    this->timestamp = Transacao::obterTimestampPublico();
    this->hash = calcularHash();
}

std::string Bloco::calcularHash() const {
    unsigned char hash[SHA256_DIGEST_LENGTH];
    EVP_MD_CTX *mdctx = EVP_MD_CTX_new();

    EVP_DigestInit_ex(mdctx, EVP_sha256(), NULL);

    std::stringstream ss;
    ss << index << timestamp << hashAnterior << nonce;
    for (const auto& transacao : transacoes) {
        ss << transacao.getHash();
    }

    const std::string& input = ss.str();
    EVP_DigestUpdate(mdctx, input.c_str(), input.size());
    EVP_DigestFinal_ex(mdctx, hash, NULL);
    EVP_MD_CTX_free(mdctx);

    std::stringstream hashHex;
    for (int i = 0; i < SHA256_DIGEST_LENGTH; i++) {
        hashHex << std::hex << std::setw(2) << std::setfill('0') << (int)hash[i];
    }

    return hashHex.str();
}


void Bloco::minerarBloco(int dificuldade) {
    std::string alvo(dificuldade, '0');
    while(hash.substr(0, dificuldade) != alvo) {
        nonce++;
        hash = calcularHash();
        if (nonce % 10000 == 0) {
            std::cout << "Tentando nonce " << nonce << " - Hash: " << hash << "\r";
            std::cout.flush();
        }
    }
    std::cout << "Bloco "<< this->index <<" minerado! Hash: " << hash << std::endl;
}

std::vector<Transacao> Bloco::getTransacoes(){
    return transacoes;
}

std::string Bloco::transactionsToString() const
{
    std::stringstream ss;
    for (const auto& trans : transacoes)
    {
        ss << trans.toString();
    }
    return ss.str();
}
int Bloco::getNonce() const{ return nonce;}
int Bloco::getIndex() const { return index; }
std::string Bloco::getHash() const { return hash; }
std::string Bloco::getHashAnterior() const { return hashAnterior; }


