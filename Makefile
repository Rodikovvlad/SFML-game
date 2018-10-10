all:
	g++ test.cpp -o test.exe -DSFML_STATIC -I D:\SfmlSources\SFML\include -L D:\SfmlSources\SFML\lib -lsfml-graphics-s -lsfml-window-s -lsfml-system-s -lopengl32 -lwinmm -lgdi32 -lfreetype -ljpeg
