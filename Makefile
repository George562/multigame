CFLAGS=-lsfml-graphics -lsfml-window -lsfml-system -lsfml-network -lsfml-audio -O3 -std=c++20
IncludeDir = Headers
IncludeDirLevelEditor = LevelEditor/Headers

all: multigame

multigame: multigame.cpp $(IncludeDir)/*
	g++ multigame.cpp -o multigame $(CFLAGS) -I./$(IncludeDir) -I./SFML-2.5.1/include/ -L ./SFML-2.5.1/lib/

hide: multigame.cpp $(IncludeDir)/*
	g++ multigame.cpp -o multigame $(CFLAGS) -mwindows -I./$(IncludeDir) -I./SFML-2.5.1/include/ -L ./SFML-2.5.1/lib/

editor: level_Editor

level_editor: LevelEditor/level_editor.cpp $(IncludeDirLevelEditor)/*
	g++ LevelEditor/level_editor.cpp -o level_editor $(CFLAGS) -I./$(IncludeDirLevelEditor) -I./SFML-2.5.1/include/ -L ./SFML-2.5.1/lib/