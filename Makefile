CFLAGS=-lsfml-graphics -lsfml-window -lsfml-system -lsfml-network -lsfml-audio -O3 -std=c++20

all: multigame

multigame: multigame.cpp
	g++ multigame.cpp -o multigame $(CFLAGS) -L ./SFML-2.5.1/lib/

hide: multigame.cpp
	g++ multigame.cpp -o multigame $(CFLAGS) -mwindows -L ./SFML-2.5.1/lib/

editor: level_Editor

level_editor: LevelEditor/level_editor.cpp
	g++ LevelEditor/level_editor.cpp -o level_editor $(CFLAGS) -L ./SFML-2.5.1/lib/