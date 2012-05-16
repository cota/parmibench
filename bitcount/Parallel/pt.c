#include "pt_head.h"
#include "bitops.h"

/* Intilize barrier */
void startBarrier(BARRIER* myBarrier){
	pthread_mutex_init(&(myBarrier->barrier_mutex), NULL);
	pthread_cond_init(&(myBarrier->barrier_cond), NULL);
	myBarrier->count = 0;
}

/* Intilize barrier for group*/
void startBarrier_group(BARRIER* myBarrier_group){
	int i=0;	
	for(i=0;i<GROUPS;i++){
		pthread_mutex_init(&(myBarrier_group[i].barrier_mutex), NULL);
		pthread_cond_init(&(myBarrier_group[i].barrier_cond), NULL);
		myBarrier_group[i].count = 0;
	}
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

/* Actuate barrier for group*/
void actuateBarrier_group(BARRIER* myBarrier_group, int myID){
	int group = groups[myID];
	pthread_mutex_lock(&(myBarrier_group[group].barrier_mutex));
	myBarrier_group[group].count++;
	if (myBarrier_group[group].count!=members[group]) {
		pthread_cond_wait(&(myBarrier_group[group].barrier_cond), &(myBarrier_group[group].barrier_mutex));
	}
	else{
		myBarrier_group[group].count=0;
		pthread_cond_broadcast(&(myBarrier_group[group].barrier_cond));
	}
	pthread_mutex_unlock(&(myBarrier_group[group].barrier_mutex));
}

void startThreads(int func,FUNCARGS* funcArgs){
	int i,pc;
	for(i=0;i<PROCESSORS;i++){
		pthread_t thread;
		THREADARGS* threadArgs = (THREADARGS*)malloc(sizeof(THREADARGS));
		threadArgs->id = i;
		threadArgs->funcArgs=funcArgs;
		switch(func){
			case 1:	//bitcnt_1_parallel.c
				pc = pthread_create(&thread,NULL,bit_count_thread,(void*)threadArgs);
				break;
			case 2:	//bitstring_parallel_spf.c
				pc = pthread_create(&thread,NULL,bitstring_thread_parallel,(void*)threadArgs);
				break;
			case 3:	//bitstring_partition_spf.c
				pc = pthread_create(&thread,NULL,bitstring_thread_partition,(void*)threadArgs);
				break;
			case 4:	//bitcnts_parallel.c
				pc = pthread_create(&thread,NULL,func_thread,(void*)threadArgs);
				break;
		}
		if(pc != 0){
			printf("Fail to create threads!");
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
