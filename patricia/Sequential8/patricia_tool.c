#include <stdlib.h>
#include <stdio.h>
#include "patricia.h"

void insertStack(patricia_node_t *n){
	nstack[stack_size] = n;
	stack_size++;
}

void process816(){
	long i;
	long counter;
	patricia_node_t* tn;
	for(i=0; i<stack_size; i++){
		counter=0;
		tn = nstack[i];
		while(tn->parent){
			tn = tn->parent;
			counter++;
		}
		printf("counter=%d\n",counter);
		if(counter==8){
			nstack8[stack8_size] = nstack[i];
			stack8_size++;
		}else if(counter==17){
			nstack16[stack16_size] = nstack[i];
			stack16_size++;
		}
	}
}


void outputInformation816(){
	printf("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
	long i;
	patricia_node_t* n;
	for(i=0; i<stack8_size; i++){
		n = nstack8[i];
		if(n->prefix){
			printf("8node=%s\n",prefix_toa (n->prefix));
		}else{
			printf("8node=glue/%d\n",n->bit);
		}
	}
	for(i=0; i<stack16_size; i++){
		n = nstack16[i];
		if(n->prefix){
			printf("16node=%s\n",prefix_toa (n->prefix));
		}else{
			printf("16node=glue/%d\n",n->bit);
		}
	}
}
