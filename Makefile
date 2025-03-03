all: rodcut

OBJS = rodcut.o piece_values.o vec.o cut_list.o
CC = gcc
CFLAGS = -Wall -g

rodcut: $(OBJS)
	gcc -o rodcut $(CFLAGS) $(OBJS)

rodcut.o: rodcut.c piece_values.h cut_list.h vec.h

piece_values.o: piece_values.c piece_values.h

cut_list.o: cut_list.c cut_list.h vec.h piece_values.h

clean:
	rm -f rodcut $(OBJS)
