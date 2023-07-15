CC=g++
CFLAGS=-lsfml-graphics -lsfml-window -lsfml-system -lsfml-network -lsfml-audio -O3

all: level_editor

level_editor: level_editor.cpp ../*
	$(CC) level_editor.cpp -o level_editor $(CFLAGS) -I../SFML-2.5.1/include/ -L../SFML-2.5.1/lib/