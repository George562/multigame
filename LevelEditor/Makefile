CFLAGS=-lsfml-graphics -lsfml-window -lsfml-system -lsfml-network -lsfml-audio -O3 -w

all: level_editor

level_editor: level_editor.cpp ../*
	g++ level_editor.cpp -o level_editor $(CFLAGS) -I./Headers/* -I../SFML-2.5.1/include/ -L../SFML-2.5.1/lib/