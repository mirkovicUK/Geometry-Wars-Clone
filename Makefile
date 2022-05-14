make:
	g++ src/*.cpp -Isrc -std=c++17 -o game -lsfml-graphics -lsfml-window -lsfml-system