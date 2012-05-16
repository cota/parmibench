CC := $(CROSS_COMPILE)gcc
CFLAGS := -static -O2

OBJS := dijkstra_large dijkstra_small dijkstra_allpair

all: $(OBJS)

dijkstra_large: dijkstra_large.c Makefile
	$(CC) $(CFLAGS) $< -o $@

dijkstra_small: dijkstra_small.c Makefile
	$(CC) $(CFLAGS) $< -o $@

dijkstra_allpair: dijkstra_allpair.c Makefile
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm -rf $(OBJS) output*

.PHONY: clean all
