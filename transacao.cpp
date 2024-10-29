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

    const EVP_MD* md = EVP_sha256(); 
    EVP_MD_CTX* ctx = EVP_MD_CTX_new();

    if (ctx == nullptr || md == nullptr) {
        return ""; 
    }

    if (!EVP_DigestInit_ex(ctx, md, nullptr)) {
       EVP_MD_CTX_free(ctx);
       return "";
    }

    if (!EVP_DigestUpdate(ctx, input.c_str(), input.size())) {
        EVP_MD_CTX_free(ctx);
        return "";
    }


    unsigned int len = SHA256_DIGEST_LENGTH;
    if (!EVP_DigestFinal_ex(ctx, hash, &len)) {
        EVP_MD_CTX_free(ctx);
        return "";
    }

    EVP_MD_CTX_free(ctx);

    std::stringstream hashHex;
    for (unsigned int i = 0; i < len; i++) {
        hashHex << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(hash[i]);
    }
    return hashHex.str();
}

std::string Transacao::getHash() const {
    return hash;
}

std::string Transacao::toString() const {
    boost::json::object blocoJson;
    blocoJson["remetente"] = remetente;
    blocoJson["destinatario"] = destinatario;
    blocoJson["valor"] = valor;
    blocoJson["timestamp"] = timestamp;
    return boost::json::serialize(blocoJson);
}
