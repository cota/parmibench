/*
 * $Id: demo.c,v 1.4 2005/12/07 20:55:52 dplonka Exp $
 *
 * This is based on "demo.c" provided with MRT-2.2.2a.
 */

#include "patricia.h"
#include <stdio.h> /* printf */
#include <stdlib.h> /* exit */

void func(prefix_t *prefix) {
	printf("node: %s/%d\n", prefix_toa(prefix), prefix->bitlen);
}

int main(int argc, char **argv) {
	prefix_t *prefix;
    	patricia_tree_t *tree;
   	patricia_node_t *node;
    	tree = New_Patricia(32);
	FILE *fp;
	char *line[STACK];
	long i,j=0;
	for(i=0;i<STACK;i++){
		line[i] = (char*)malloc(sizeof(char)*50);
	}
	if ((fp = fopen(argv[1], "r")) == NULL) {
		printf("File %s doesn't seem to exist\n",argv[1]);
		exit(0);
	}
	i=0;
	while (fgets(line[i], 128, fp)) {
		make_and_lookup(tree,line[i]);
		i++;
	}
	int x=0;
	for(x=0;x<100;x++){
		for(j=0;j<i;j++){
			try_search_best(tree, line[j]);
		}
	}
    	Destroy_Patricia(tree, (void *)0);
    	exit(0);
}
