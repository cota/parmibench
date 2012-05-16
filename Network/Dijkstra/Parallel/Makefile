all:  dijkstra_parallel_mqueue dijkstra_parallel_squeue dijkstra_allpair

dijkstra_parallel_mqueue: dijkstra_parallel_mqueue.c
	gcc -lpthread dijkstra_parallel_mqueue.c -o dijkstra_parallel_mqueue

dijkstra_parallel_squeue: dijkstra_parallel_squeue.c
	gcc -lpthread dijkstra_parallel_squeue.c -o dijkstra_parallel_squeue

dijkstra_allpair: dijkstra_allpair.c
	gcc -lpthread dijkstra_allpair.c -o dijkstra_allpair

clean:
	rm -rf dijkstra_parallel_mqueue dijkstra_parallel_squeue dijkstra_allpair output*
