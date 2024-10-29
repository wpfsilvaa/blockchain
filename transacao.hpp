#ifndef TRANSACAO_HPP
#define TRANSACAO_HPP

#include <string>

class Transacao {
private:
    std::string remetente;
    std::string destinatario;
    double valor;
    std::string timestamp;
    std::string hash;
    std::string calcularHash() const;
    static std::string obterTimestamp();

public:
    static std::string obterTimestampPublico() { return obterTimestamp(); }
    Transacao(const std::string& rem, const std::string& dest, double val);
    std::string getHash() const;
    std::string toString() const;
};

#endif
