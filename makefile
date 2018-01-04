all:
	gcc -o blemshoga src/blemshoga.c src/map.c src/block.c -m32 -lallegro -lallegro_image -Wall
