TARGET = "blockchain"

INCLUDES = -Iheaders
SRCS = blockchain.cpp bloco.cpp controller.cpp transacao.cpp main.cpp 

CXX = g++

CXXFLAGS = -Wall -Wextra -std=c++17 $(INCLUDES)

LIBS = -lssl -lcrypto -lboost_system -lboost_json

all: $(TARGET)
	./$(TARGET)

$(TARGET): $(SRCS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(SRCS) $(LIBS)

clean:
	rm -f $(TARGET)