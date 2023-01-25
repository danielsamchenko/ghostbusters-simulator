CFLAGS = -std=gnu99 -lpthread
CC = gcc
SOURCES = main.c building.c room.c hunter.c ghost.c
OBJECTS = main.o building.o room.o hunter.o ghost.o
HEADERS = defs.h

final_project: $(OBJECTS) $(HEADERS)
	$(CC) $(CFLAGS) -o final_project $(SOURCES) $(HEADERS)

main.o: main.c defs.h
	gcc -c main.c

room.o: room.c defs.h
	gcc -c room.c

building.o: building.c defs.h
	gcc -c building.c

hunter.o: hunter.c defs.h
	gcc -c hunter.c

ghost.o: ghost.c defs.h
	gcc -c ghost.c

clean:
	rm -f *.o final_project