DEBUG =
CDEBUG = -g -fno-builtin
CFLAGS = -Wall
CC = gcc

ifneq ($(DEBUG),)
CFLAGS += $(CDEBUG)
endif

LIBS = -lallegro -lallegro_image -lallegro_font -lallegro_ttf -lallegro_audio -lallegro_acodec -lm
SRC ?= src/*.c
all:
	$(CC) -o blemshoga $(SRC) $(LIBS) $(CFLAGS)
