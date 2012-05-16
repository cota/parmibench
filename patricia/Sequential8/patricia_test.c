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
	process816();
	for(j=0;j<i;j++){
		try_search_best(tree, line[j]);
	}
    	//make_and_lookup(tree, "127.0.0.0/8");
	//make_and_lookup(tree, "10.42.42.0/24");
    	//make_and_lookup(tree, "10.42.69.0/24");
	//make_and_lookup(tree, "0.0.69.0/24");
    	//try_search_best(tree, "127.0.0.1");
    	//try_search_best(tree, "10.0.0.1");
    	//make_and_lookup(tree, "10.0.0.0/8");
    	//make_and_lookup(tree, "10.0.0.0/9");
    	//try_search_best(tree, "10.42.42.0/24");
    	//try_search_best(tree, "10.10.10.10");
    	//try_search_best(tree, "10.10.10.1");
    	//try_search_exact(tree, "10.0.0.0");
    	//try_search_exact(tree, "10.0.0.0/8");
	#if 0
    	PATRICIA_WALK(tree->head, node) {
       printf("node: %s/%d,left=%s,right=%s\n", prefix_toa(node->prefix), node->prefix->bitlen,prefix_toa(node->l->prefix),prefix_toa(node->r->prefix));
    	} PATRICIA_WALK_END;
	#else
    	//printf("%u total nodes.\n", patricia_walk_inorder(tree->head, func));
	#endif
    	//lookup_then_remove(tree, "42.0.0.0/8");
    	//lookup_then_remove(tree, "10.0.0.0/8");
    	//try_search_exact(tree, "10.0.0.0");
	#if 0
    	PATRICIA_WALK(tree->head, node) {
        printf("node: %s/%d,left=%s,right=%s\n", prefix_toa(node->prefix), node->prefix->bitlen,prefix_toa(node->l->prefix),prefix_toa(node->r->prefix));
    	} PATRICIA_WALK_END;
	#else
    	//printf("%u total nodes.\n", patricia_walk_inorder(tree->head, func));
	#endif
	//outputInformation816();
	//printf("@@%d\n",comp_with_mask (prefix_tochar (nstack[2]->prefix), prefix_tochar (nstack[4]->prefix),nstack[2]->prefix->bitlen));
    	Destroy_Patricia(tree, (void *)0);
    	exit(0);
}
