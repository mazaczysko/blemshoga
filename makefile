LIBS = -lallegro -lallegro_image -lallegro_font -lallegro_ttf
SRC ?= src/*.c
all:
	gcc -o blemshoga $(SRC) $(LIBS) -Wall
