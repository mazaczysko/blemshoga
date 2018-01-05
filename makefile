all:
	gcc -o blemshoga src/blemshoga.c src/map.c src/tile.c -lallegro -lallegro_image -Wall
