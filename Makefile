all: rodcut

OBJS = rodcut.o piece_values.o vec.o cut_list.o lru.o lfu.o
CC = gcc
CFLAGS = -Wall -g

rodcut: $(OBJS)
	$(CC) -o rodcut $(CFLAGS) $(OBJS)

rodcut.o: rodcut.c piece_values.h cut_list.h vec.h lru.h lfu.h

piece_values.o: piece_values.c piece_values.h

cut_list.o: cut_list.c cut_list.h vec.h piece_values.h

vec.o: vec.c vec.h

lru.o: lru.c lru.h

lfu.o: lfu.c lfu.h

clean:
	rm -f rodcut $(OBJS)