CFLAGS=-lsfml-graphics -lsfml-window -lsfml-system -lsfml-network -lsfml-audio -O3 -std=c++20

all: Generation

Generation: Generation.cpp icon
	g++ Generation.cpp my.res -o Generation $(CFLAGS) -I./SFML-2.5.1/include/ -L ./SFML-2.5.1/lib/

hide: Generation.cpp icon
	g++ Generation.cpp my.res -o Generation $(CFLAGS) -mwindows -I./SFML-2.5.1/include/ -L ./SFML-2.5.1/lib/

editor: level_Editor

level_editor: LevelEditor/level_editor.cpp
	g++ LevelEditor/level_editor.cpp -o level_editor $(CFLAGS) -I./SFML-2.5.1/include/ -L ./SFML-2.5.1/lib/

icon: my.rc
	windres my.rc -O coff -o my.res