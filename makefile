all:
	gcc -o blemshoga src/blemshoga.c src/map.c src/tile.c src/ent.c -lallegro -lallegro_image -lallegro_font -lallegro_ttf -Wall
