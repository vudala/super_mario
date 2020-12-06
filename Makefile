
CC = gcc
CFLAGS = -Wall
OBJECTS = main.o game.o level.o entity.o utils.o animation.o entity_list.o
HEADERS = game.h level.h entity.h utils.h animation.h entity_list.h
LIBS = -lallegro_font -lallegro_image -lallegro_primitives -lallegro -lallegro_audio -lallegro_acodec -lm
OUTPUT = game

all: $(OBJECTS)
	$(CC) $(OBJECTS) $(HEADERS) -o $(OUTPUT) $(LIBS) $(CFLAGS)

main.o: main.c
	$(CC) -g -c main.c $(CFLAGS)

game.o: game.c
	$(CC) -g -c game.c $(CFLAGS)

level.o: level.c
	$(CC) -g -c level.c $(CFLAGS)

entity.o: entity.c
	$(CC) -g -c entity.c $(CFLAGS)

entity_list.o: entity_list.c
	$(CC) -g -c entity_list.c $(CFLAGS)

utils.o: utils.c
	$(CC) -g -c utils.c $(CFLAGS)

animation.o: animation.c
	$(CC) -g -c animation.c $(CFLAGS)

clean:
	rm -rf $(OBJECTS)

purge: clean
	rm -rf $(OUTPUT)