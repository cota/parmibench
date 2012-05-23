CC := $(CROSS_COMPILE)gcc
CFLAGS := -O2

OBJS := dijkstra_large dijkstra_small dijkstra_allpair

all: $(OBJS)

dijkstra_large: dijkstra_large.c
	$(CC) $(CFLAGS) $^ -o $@

dijkstra_small: dijkstra_small.c
	$(CC) $(CFLAGS) $^ -o $@

dijkstra_allpair: dijkstra_allpair.c
	$(CC) $(CFLAGS) $^ -o $@

clean:
	rm -rf $(OBJS) output*

.PHONY: clean all
