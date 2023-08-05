CFLAGS=-lsfml-graphics -lsfml-window -lsfml-system -lsfml-network -lsfml-audio -O3 -std=c++20
IncludeDir = Headers

all: multigame

multigame: multigame.cpp $(IncludeDir)/*
	g++ multigame.cpp -o multigame $(CFLAGS) -I./$(IncludeDir) -I./SFML-2.5.1/include/ -L ./SFML-2.5.1/lib/

hide: multigame.cpp $(IncludeDir)/*
	g++ multigame.cpp -o multigame $(CFLAGS) -mwindows -I./$(IncludeDir) -I./SFML-2.5.1/include/ -L ./SFML-2.5.1/lib/

# server: server.o tools
# 	g++ server.o $(CFLAGS) -o server

# server.o: server.cpp
# 	g++ server.cpp -c -O3

# client: client.o tools
# 	g++ client.o $(CFLAGS) -o client

# client.o: client.cpp
# 	g++ client.cpp -c -O3

clear:
	del -rf *.o "Headers\*.gch"