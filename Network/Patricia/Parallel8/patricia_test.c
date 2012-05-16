/*
 * $Id: demo.c,v 1.4 2005/12/07 20:55:52 dplonka Exp $
 *
 * This is based on "demo.c" provided with MRT-2.2.2a.
 */

#include "patricia.h"
#include "parallel.h"
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
	int i,j=0;
	for(i=0;i<STACK;i++){
		ips[i] = (char*)malloc(sizeof(char)*50);
	}
	if ((fp = fopen(argv[1], "r")) == NULL) {
		printf("File %s doesn't seem to exist\n",argv[1]);
		exit(0);
	}
	i=0;
	while (fgets(ips[i], 128, fp)) {
		make_and_lookup(tree,ips[i]);
		i++;
	}
	process();
	search_size = i;
	//divide_task_group(search_size);
	//pthread_mutex_init(&lock_mutex, NULL);
	startBarrier(&myBarrier);
	startThreads(tree);
    	//make_and_lookup(tree, "127.0.0.0/8");
	//make_and_lookup(tree, "10.42.42.0/24");
    	//make_and_lookup(tree, "10.42.69.0/24");
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
	//outputInformation();
	//comp_with_mask(prefix_tochar (node->prefix), prefix_tochar (prefix),node->prefix->bitlen);
    	Destroy_Patricia(tree, (void *)0);
    	exit(0);
}
