
CC = gcc
CFLAGS = -Wall -std=c99
OBJECTS = main.o game.o level.o entity.o utils.o animation.o entity_list.o character.o
HEADERS = game.h level.h entity.h utils.h animation.h entity_list.h character.h
LIBS = -lallegro_font -lallegro_image -lallegro_primitives -lallegro -lallegro_audio -lallegro_acodec -lm
OUTPUT = game

all: $(OBJECTS)
	$(CC) $(CFLAGS) $(OBJECTS) -o $(OUTPUT) $(LIBS)

main.o: main.c
	$(CC) $(CFLAGS) -g -c main.c 

game.o: game.c
	$(CC) $(CFLAGS) -g -c game.c 

level.o: level.c
	$(CC) $(CFLAGS) -g -c level.c 

entity.o: entity.c
	$(CC) $(CFLAGS) -g -c entity.c

entity_list.o: entity_list.c
	$(CC) $(CFLAGS) -g -c entity_list.c

utils.o: utils.c
	$(CC) $(CFLAGS) -g -c utils.c

animation.o: animation.c
	$(CC) $(CFLAGS) -g -c animation.c

character.o: character.c
	$(CC) $(CFLAGS) -g -c character.c

clean:
	rm -rf $(OBJECTS)

purge: clean
	rm -rf $(OUTPUT)