# Compiler and it's settings
CC = gcc
CFLAGS = -Wall -std=c99 -g
LIBS = -lallegro_font -lallegro_ttf -lallegro_image -lallegro -lallegro_audio -lallegro_acodec -lm

# Inputs and outputs for the compiler
OBJECTS = main.o game.o level.o entity.o utils.o animation.o entity_list.o character.o sound.o
OUTPUT = game

all: $(OBJECTS)
	$(CC) $(CFLAGS) $(OBJECTS) -o $(OUTPUT) $(LIBS)

main.o: main.c
	$(CC) $(CFLAGS) -c main.c 

game.o: game.c
	$(CC) $(CFLAGS) -c game.c 

level.o: level.c
	$(CC) $(CFLAGS) -c level.c 

entity.o: entity.c
	$(CC) $(CFLAGS) -c entity.c

entity_list.o: entity_list.c
	$(CC) $(CFLAGS) -c entity_list.c

utils.o: utils.c
	$(CC) $(CFLAGS) -c utils.c

animation.o: animation.c
	$(CC) $(CFLAGS) -c animation.c

character.o: character.c
	$(CC) $(CFLAGS) -c character.c

sound.o: sound.c
	$(CC) $(CFLAGS) -c sound.c


clean:
	rm -rf $(OBJECTS)

purge: clean
	rm -rf $(OUTPUT)