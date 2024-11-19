## Blockchain Peer-to-Peer Network

Este projeto implementa uma rede de blockchain distribuída simples utilizando **C++**, **Boost.Asio**, e **Boost.JSON**. Ele possui um servidor blockchain centralizado e peers que podem realizar transações, propagar blocos e sincronizar o estado do blockchain. O objetivo principal é aprender conceitos fundamentais de redes distribuídas e blockchain, incluindo a propagação de blocos entre peers.

---

### Estrutura do Projeto

O projeto está dividido em arquivos `.hpp` e `.cpp`, separados por responsabilidade:

- **`block.hpp`/`block.cpp`**: Contém a estrutura de um bloco, incluindo transações, hash e funções para validação do bloco.
- **`blockchain.hpp`/`blockchain.cpp`**: Implementa a lógica do blockchain, como criação de blocos, validação e mineração.
- **`controller.hpp`/`controller.cpp`**: Define um servidor que gerencia as requisições HTTP relacionadas ao blockchain (e.g., adicionar transações, consultar o blockchain).
- **`peer.hpp`/`peer.cpp`**: Define os peers da rede, permitindo que eles:
  - Enviem transações ao servidor.
  - Propaguem novos blocos pela rede.
  - Escutem a chegada de blocos propagados por outros peers.
- **`main.cpp`**: Ponto de entrada do programa que executa o peer para interagir com o servidor e com outros peers.

---

### Funcionalidades Implementadas

1. **Servidor Blockchain:**
   - Mantém um blockchain local que pode ser acessado via requisições HTTP.
   - Suporta:
     - **GET** `/blockchain`: Retorna o estado atual do blockchain em JSON.
     - **POST** `/add_transaction`: Adiciona uma nova transação ao blockchain.
     - Propagação de blocos minerados para os peers conectados.

2. **Peer:**
   - Envia transações para o servidor.
   - Solicita o blockchain atualizado do servidor.
   - Escuta novos blocos propagados por outros peers e sincroniza sua cópia do blockchain.
   - Propaga novos blocos criados localmente para outros peers conectados.

3. **Transações:**
   - Incluem informações sobre o remetente, destinatário e o valor transferido.
   - São adicionadas ao pool de transações pendentes até que sejam mineradas.

4. **Mineração de Blocos:**
   - Executada automaticamente ao adicionar uma nova transação.
   - Gera o hash do bloco utilizando uma função de proof-of-work simplificada.

5. **Rede Distribuída:**
   - Comunicação entre peers via sockets TCP utilizando **Boost.Asio**.
   - Propagação de novos blocos entre peers para manter o blockchain sincronizado.

---

### Como Compilar e Executar

#### Pré-requisitos

- **g++** com suporte a **C++17**.
- Biblioteca **Boost** instalada (com suporte para `Boost.Asio` e `Boost.JSON`).
  ```bash
  sudo apt update && sudo apt install libboost-all-dev
  ```
- OpenSSL para funções de hash.
  ```bash
  sudo apt update && sudo apt install libssl-dev
  ```
#### Compilação

Use o comando abaixo para compilar o projeto:

Peer

```bash
g++ -Wall -Wextra -std=c++17 -Iheaders -o "peer" peer.cpp main.cpp -lssl -lcrypto -lboost_system -lboost_json
```

Server

```bash
g++ -Wall -Wextra -std=c++17 -Iheaders -o "server" blockchain.cpp bloco.cpp controller.cpp transacao.cpp main.cpp -lssl -lcrypto -lboost_system -lboost_json
```

Ou simplesmente make, pois já existem os Makefiles correspondentes.

#### Execução

1. **Inicie o servidor blockchain:**

   Certifique-se de que o servidor blockchain está rodando antes de executar os peers.

   ```bash
   ./server
   ```

2. **Execute os peers:**

   Inicie um ou mais peers para interagir com o servidor e entre si.

   ```bash
   ./peer
   ```

---

### Próximos Passos

1. **Validação de Blocos:**
   - Verificar a validade dos blocos recebidos por outros peers antes de adicioná-los ao blockchain.

2. **Persistência de Dados:**
   - Armazenar o blockchain em um arquivo local para manter os dados entre execuções.

3. **Criação Dinâmica de Peers:**
   - Implementar um mecanismo para adicionar peers dinamicamente à rede.

4. **Sincronização Completa do Blockchain:**
   - Garantir que todos os peers na rede compartilhem uma versão única e válida do blockchain.

5. **Segurança:**
   - Implementar assinaturas digitais para autenticar transações.

6. **Interface Gráfica ou CLI Melhorada:**
   - Criar uma interface mais amigável para visualização do blockchain e envio de transações.

---

### Aprendizados e Conceitos Aplicados

- **Blockchain:**
  - Estrutura básica de blocos e transações.
  - Algoritmo de proof-of-work para validação de blocos.

- **Redes Distribuídas:**
  - Comunicação peer-to-peer com propagação de mensagens.
  - Sincronização de estados entre nós distribuídos.

- **Boost Libraries:**
  - Uso de **Boost.Asio** para comunicação de rede.
  - Manipulação de JSON com **Boost.JSON**.

- **C++ Avançado:**
  - Gerenciamento de threads e sockets.
  - Programação orientada a objetos aplicada ao desenvolvimento distribuído.

---

### Contribuições

Contribuições para este projeto são bem-vindas! Abra um issue ou envie um pull request no repositório.

---
