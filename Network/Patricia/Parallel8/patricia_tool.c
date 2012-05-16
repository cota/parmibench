#include <stdlib.h>
#include <stdio.h>
#include "patricia.h"
#include "parallel.h"

void insertStack(patricia_node_t *n){
	nstack[stack_size] = n;
	stack_size++;
}

void process(){
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
		nstack[i]->depth = counter;
		if(counter==CUTOFF_DEPTH){
			nstack[i] -> i8ndex = stack8_size;
			n8stack[stack8_size] = nstack[i];
			stack8_size++;
		}
	}
}


void outputInformation(){
	printf("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
	long i;
	patricia_node_t* n;
	for(i=0; i<stack8_size; i++){
		n = n8stack[i];
		if(n->prefix){
			printf("8node=%s\n",prefix_toa (n->prefix));
		}else{
			printf("8node=glue/%d\n",n->bit);
		}
	}
}



/* Intilize barrier */
void startBarrier(BARRIER* myBarrier){
	pthread_mutex_init(&(myBarrier->barrier_mutex), NULL);
	pthread_cond_init(&(myBarrier->barrier_cond), NULL);
	myBarrier->count = 0;
}


/* Actuate barrier */
void actuateBarrier(BARRIER* myBarrier){
	pthread_mutex_lock(&(myBarrier->barrier_mutex));
	myBarrier->count++;
	if (myBarrier->count!=PROCESSORS) {
		pthread_cond_wait(&(myBarrier->barrier_cond), &(myBarrier->barrier_mutex));
	}
	else{
		myBarrier->count=0;
		pthread_cond_broadcast(&(myBarrier->barrier_cond));
	}
	pthread_mutex_unlock(&(myBarrier->barrier_mutex));
}


/* Initialize a thread pool and create the threads 
   actionType: 0-search, 1-insert
*/
void startThreads(patricia_tree_t *tree){
	int i,pc;
	for(i=0;i<PROCESSORS;i++){
		pthread_t thread;
		THREADARGS* threadArgs = (THREADARGS*)malloc(sizeof(THREADARGS));
		threadArgs->id = i;
		threadArgs->tree = tree;
		pc = pthread_create(&thread,NULL,search_best_thread,(void*)threadArgs);
		if(pc != 0){
			printf("Fail to create threads!\n");
			exit(0);
		}else{
			threadPool[i].id = i;
			threadPool[i].thread = thread;
		}
  	}
  	for(i=0;i<PROCESSORS;i++){
      	pthread_join(threadPool[i].thread,NULL);
  	}
}



