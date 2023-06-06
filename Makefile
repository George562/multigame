CC=g++
CFLAGS=-lsfml-graphics -lsfml-window -lsfml-system -lsfml-network -O3

all: multigame

multigame: multigame.o
	$(CC) multigame.o -o multigame $(CFLAGS)

hide: multigame.o
	$(CC) multigame.o -o multigame $(CFLAGS) -mwindows

multigame.o: multigame.cpp chat.h.gch client.h.gch button.h.gch player.h.gch
	$(CC) multigame.cpp -c -O3

button.h.gch: button.h panel.h.gch
	$(CC) button.h -O3

panel.h.gch: panel.h text.h.gch
	$(CC) panel.h -O3

player.h.gch: player.h creature.h.gch
	$(CC) player.h -O3

creature.h.gch: creature.h weapon.h.gch
	$(CC) creature.h -O3

weapon.h.gch: weapon.h bullet.h.gch Bar.h.gch
	$(CC) weapon.h -O3

Bar.h.gch: Bar.h
	$(CC) Bar.h -O3

bullet.h.gch: bullet.h text.h.gch
	$(CC) bullet.h -O3

client.h.gch: client.h tools.h.gch
	$(CC) client.h -O3

chat.h.gch: chat.h text.h.gch
	$(CC) chat.h -O3

text.h.gch: text.h tools.h.gch
	$(CC) text.h -O3

tools.h.gch: tools.h scale.h.gch textures.h.gch
	$(CC) tools.h -O3

scale.h.gch: scale.h
	$(CC) scale.h -O3

textures.h.gch: textures.h
	$(CC) textures.h -O3

server: server.o tools.h.gch
	$(CC) server.o $(CFLAGS) -o server

server.o: server.cpp
	$(CC) server.cpp -c -O3

client: client.o tools.h.gch
	$(CC) client.o $(CFLAGS) -o client

client.o: client.cpp
	$(CC) client.cpp -c -O3

clean:
	del -rf *.o *.gch