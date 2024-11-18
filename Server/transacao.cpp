#include "transacao.hpp"
#include <sstream>
#include <iomanip>
#include <openssl/sha.h>
#include <openssl/evp.h>
#include <iostream>
#include <ctime>
#include <boost/json.hpp>

Transacao::Transacao(const std::string& rem, const std::string& dest, double val) 
    : remetente(rem), destinatario(dest), valor(val) {
    if (rem.empty() || dest.empty() || val <= 0) {
        throw std::invalid_argument("Dados inválidos na transação!");
    }
    this->timestamp = obterTimestamp();
    this->hash = calcularHash();
}

std::string Transacao::obterTimestamp() {
    auto t = std::time(nullptr);
    auto tm = *std::localtime(&t);
    std::ostringstream oss;
    oss << std::put_time(&tm, "%Y-%m-%d %H:%M:%S");
    return oss.str();
}

std::string Transacao::calcularHash() const {
    std::stringstream ss;
    ss << remetente << destinatario << valor << timestamp;
    const std::string& input = ss.str();

    unsigned char hash[SHA256_DIGEST_LENGTH];
    EVP_MD_CTX* ctx = EVP_MD_CTX_new();

    if (!ctx) throw std::runtime_error("Erro ao inicializar contexto de hash!");

    EVP_DigestInit_ex(ctx, EVP_sha256(), NULL);
    EVP_DigestUpdate(ctx, input.c_str(), input.size());
    EVP_DigestFinal_ex(ctx, hash, NULL);
    EVP_MD_CTX_free(ctx);

    std::stringstream hashHex;
    for (unsigned int i = 0; i < SHA256_DIGEST_LENGTH; i++) {
        hashHex << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(hash[i]);
    }
    return hashHex.str();
}

std::string Transacao::getHash() const {
    return hash;
}

std::string Transacao::toString() const {
    boost::json::object transacaoJson;
    transacaoJson["remetente"] = remetente;
    transacaoJson["destinatario"] = destinatario;
    transacaoJson["valor"] = valor;
    transacaoJson["timestamp"] = timestamp;
    transacaoJson["hash"] = hash;
    return boost::json::serialize(transacaoJson);
}

bool Transacao::isValid() const {
    return calcularHash() == hash;
}
