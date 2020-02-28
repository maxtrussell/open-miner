CPPC=g++
FLAGS=-Wall -I . -I include -lglfw3 -lGL -lX11 -lpthread -lXrandr -lXi -ldl \
	     -lsfml-system -lsfml-network
DEPS=lib/*.c src/shader.cpp src/client/client.cpp src/server/server.cpp \
	     src/client/engine.cpp src/common/game/entity.cpp
BIN=bin/main

build:
	$(CPPC) src/main.cpp $(DEPS) -o $(BIN) $(FLAGS)

client_engine:
	$(CPPC) src/client/client_engine.cpp $(DEPS) -o $(BIN) $(FLAGS)

run:
	./$(BIN)

clean:
	rm bin/*
