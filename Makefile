CC=g++
CFLAGS=-lsfml-graphics -lsfml-window -lsfml-system -lsfml-network -O3
dir = Headers

# include $(dir)

all: multigame

multigame: multigame.cpp $(dir)/*
	$(CC) multigame.cpp -o multigame $(CFLAGS) -I./$(dir) -I./SFML-2.5.1/include/ -L ./SFML-2.5.1/lib/

hide: multigame.cpp $(dir)/*
	$(CC) multigame.cpp -o multigame $(CFLAGS) -mwindows -I./$(dir) -I./SFML-2.5.1/include/ -L ./SFML-2.5.1/lib/

# multigame.o: multigame.cpp $(dir)/*
# 	$(CC) multigame.cpp -c -O3 -I ./$(dir) -L ./bin

# button: $(dir)\button.h panel
# 	$(CC) $(dir)\button.h -O3

# panel: $(dir)\panel.h text
# 	$(CC) $(dir)\panel.h -O3

# player: $(dir)\player.h creature
# 	$(CC) $(dir)\player.h -O3

# creature: $(dir)\creature.h weapon
# 	$(CC) $(dir)\creature.h -O3

# weapon: $(dir)\weapon.h bullet Bar
# 	$(CC) $(dir)\weapon.h -O3

# Bar: $(dir)\Bar.h
# 	$(CC) $(dir)\Bar.h -O3

# bullet: $(dir)\bullet.h text
# 	$(CC) $(dir)\bullet.h -O3

# client: $(dir)\client.h tools
# 	$(CC) $(dir)\client.h -O3

# chat: $(dir)\chat.h text
# 	$(CC) $(dir)\chat.h -O3

# text: $(dir)\text.h tools
# 	$(CC) $(dir)\text.h -O3

# tools: $(dir)\tools.h scale textures
# 	$(CC) $(dir)\tools.h -O3

# scale: $(dir)\scale.h
# 	$(CC) $(dir)\scale.h -O3

# textures: $(dir)\textures.h
# 	$(CC) $(dir)\textures.h -O3

# server: server.o tools
# 	$(CC) server.o $(CFLAGS) -o server

# server.o: server.cpp
# 	$(CC) server.cpp -c -O3

# client: client.o tools
# 	$(CC) client.o $(CFLAGS) -o client

# client.o: client.cpp
# 	$(CC) client.cpp -c -O3

clear:
	del -rf *.o "Headers\*.gch"