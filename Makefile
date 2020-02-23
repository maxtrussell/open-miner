CPPC=g++
CPPFLAGS=-Wall -I include -lglfw3 -lGL -lX11 -lpthread -lXrandr -lXi -ldl lib/*.c src/*.cpp
BIN=bin/main

client_engine:
	$(CPPC) client_engine.cpp -o $(BIN) $(CPPFLAGS)

run:
	./$(BIN)

clean:
	rm bin/*
