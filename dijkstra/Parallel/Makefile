CC := $(CROSS_COMPILE)gcc
CFLAGS := -lpthread -O2

OBJS := dijkstra_parallel_mqueue dijkstra_parallel_squeue dijkstra_allpair

all:  $(OBJS)

dijkstra_parallel_mqueue: dijkstra_parallel_mqueue.c
	$(CC) $(CFLAGS) $< -o $@

dijkstra_parallel_squeue: dijkstra_parallel_squeue.c
	$(CC) $(CFLAGS) $< -o $@

dijkstra_allpair: dijkstra_allpair.c
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm -rf $(OBJS) output*

.PHONY: all clean
