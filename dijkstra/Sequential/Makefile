all:  dijkstra_large dijkstra_small dijkstra_allpair

dijkstra_large: dijkstra_large.c Makefile
	gcc -static dijkstra_large.c -O3 -o dijkstra_large

dijkstra_small: dijkstra_small.c Makefile
	gcc -static dijkstra_small.c -O3 -o dijkstra_small

dijkstra_allpair: dijkstra_allpair.c Makefile
	gcc -static dijkstra_allpair.c -O3 -o dijkstra_allpair

clean:
	rm -rf dijkstra_large dijkstra_small dijkstra_allpair output*
