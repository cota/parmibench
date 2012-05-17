/********************************************************************************
*						      			       				   
* 				Dijkstra				                                
*		Parallel 1.0 Version with Pthread
*															
* Authors:           	Yuchen Liang and Syed Muhammad Zeeshan Iqbal
* Supervisor:		Hakan Grahn	 						
* Section:          	Computer Science of BTH,Sweden
* GCC Version:		4.2.4								
* Environment: 		Kraken(Ubuntu4.2.4) with Most 8 Processors 				
* Start Date:       	15th October 2009					
* End Date:         	3rd November 2009		
*				
*********************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/types.h>
#include "params.h"

#define NUM_NODES                          2000	//16 for small input; 2000 for large input
#define NONE                               9999	//Maximum

/* define a thread pool */
struct _THREADPOOL{
	int id;			/* thread id */
	pthread_t thread;		/* thread itself */
};
typedef struct _THREADPOOL THREADPOOL;
THREADPOOL threadPool[PROCESSORS]; 

struct _BARRIER{
	int count;
	pthread_mutex_t barrier_mutex;
	pthread_cond_t barrier_cond;
};
typedef struct _BARRIER BARRIER;
BARRIER myBarrier;

struct _THREADARGS{
	int id;			/* thread id */	
};
typedef struct _THREADARGS THREADARGS;

struct _NODE{
	int iDist;
	int iPrev;
	int iQueue;//0-not in queue;1-in queue;
};
typedef struct _NODE NODE;

struct _QITEM{
  	int iNode;
  	int iDist;
  	int iPrev;
  	struct _QITEM *qNext;
};
typedef struct _QITEM QITEM;
QITEM *qHead;

struct _QHEADS{
	QITEM *head;
};
typedef struct _QHEADS QHEADS;
QHEADS Qheads[PROCESSORS];



int AdjMatrix[NUM_NODES][NUM_NODES];
int qCount[PROCESSORS];
int g_qCount;
NODE rgnNodes[NUM_NODES];
int iPrev[PROCESSORS];
int iNode[PROCESSORS];
int iDist[PROCESSORS];
int chStart, chEnd;
pthread_mutex_t pt_mutex;
int deadP[PROCESSORS]; //0:alive,1:dead

/* Forward declaring functions*/
void print_path(NODE*,int);
int qcount (void);
void startBarrier(BARRIER*);
void actuateBarrier(BARRIER*);
void* startWorking(void*);
void startThreads(void);
void dijkstra(int);
void printResult();
void enqueue (int, int, int, int);
void dequeue (int,int*, int*, int*);

void print_path(NODE *rgnNodes, int chNode){
	if (rgnNodes[chNode].iPrev != NONE){
		print_path(rgnNodes, rgnNodes[chNode].iPrev);
	}
	printf (" %d", chNode);
	fflush(stdout);
}


int qcount (){
	int i=0;
	g_qCount = 0;
	for(i=0;i<PROCESSORS;i++){
		g_qCount += qCount[i];
	}
	return(g_qCount);
}


void enqueue (int myID, int iNode, int iDist, int iPrev){
	QITEM *qNew = (QITEM *) malloc(sizeof(QITEM));
	QITEM *qLast = Qheads[myID].head;  
	if (!qNew) {
		fprintf(stderr, "Out of memory.\n");
		exit(1);
	}
  	qNew->iNode = iNode;
  	qNew->iDist = iDist;
  	qNew->iPrev = iPrev;
  	qNew->qNext = NULL;
  	if (!qLast) {
      	Qheads[myID].head = qNew;
    	}else{
      	while (qLast->qNext) 
			qLast = qLast->qNext;
      	qLast->qNext = qNew;
    	}
  	qCount[myID]++;
}


void dequeue (int myID,int *piNode, int *piDist, int *piPrev){
	int i=0;
	QITEM *qKill = Qheads[myID].head;	
	if(qKill){
		*piNode = qKill->iNode;
      	*piDist = qKill->iDist;
      	*piPrev = qKill->iPrev; 
		Qheads[myID].head = qKill -> qNext;
      	free(qKill);
      	qCount[myID]--;
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


/* Initialize a thread pool and create the threads */
void startThreads(void){
	int i,pc;
	for(i=0;i<PROCESSORS;i++){
		pthread_t thread;
		THREADARGS* threadArgs = (THREADARGS*)malloc(sizeof(THREADARGS));
		threadArgs->id = i;
		pc = pthread_create(&thread,NULL,startWorking,(void*)threadArgs);
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


void* startWorking(void* threadArgs){		
 	int k, i;
 	THREADARGS* args = (THREADARGS*)threadArgs;
	int myID = args->id;					/* Current processor No */							
	for (i=myID*(NUM_NODES/PROCESSORS); (i<(myID+1)*(NUM_NODES/PROCESSORS)+(myID+1==PROCESSORS && NUM_NODES%PROCESSORS!=0 ? NUM_NODES%PROCESSORS:0)); i++) {
		rgnNodes[i].iDist = NONE;
      	rgnNodes[i].iPrev = NONE;
		rgnNodes[i].iQueue = 0;
	}
	if(myID==0){
		rgnNodes[chStart].iDist = 0;
		rgnNodes[chStart].iQueue = 1;
	}
	enqueue (myID,chStart, 0, NONE);
	actuateBarrier(&myBarrier);
	dijkstra(myID);
	//actuateBarrier(&myBarrier);
	return NULL;
}


void dijkstra(int myID) {
	int iCost=0;
	int i=0;
	int y=0;
	int x=0;
	int target=0;
	while (qCount[myID] > 0){
		x++;
		y=0;
		//actuateBarrier(&myBarrier);	
		dequeue (myID,iNode+myID, iDist+myID, iPrev+myID);
		for (i = 0; i < NUM_NODES; i++){
			if ((iCost = AdjMatrix[iNode[myID]][i]) != NONE){
				if ((NONE == rgnNodes[i].iDist) || (rgnNodes[i].iDist > (iCost + iDist[myID]))){
					target = myID+y%PROCESSORS;	
					if(target>=PROCESSORS){
						target = 0;
					}
					if(deadP[target]==1){
						target = myID;	
					}						
					pthread_mutex_trylock(&pt_mutex);
		      		rgnNodes[i].iDist = iDist[myID] + iCost;
		      		rgnNodes[i].iPrev = iNode[myID];
					enqueue (target, i, iDist[myID] + iCost, iNode[myID]);
					pthread_mutex_unlock(&pt_mutex);
				}
		    	}
	    	}
		//actuateBarrier(&myBarrier);
	}
	deadP[myID] = 1;
}



void printResult(){
	printf("Shortest path is %d in cost. ", rgnNodes[chEnd].iDist);
      printf("Path is: ");
      print_path(rgnNodes, chEnd);
      printf("\n");
}


int main(int argc, char *argv[]) {
	int i,j,k;
	if (argc<2) {
		fprintf(stderr, "Usage: dijkstra <filename>\n");
		fprintf(stderr, "Only supports matrix size is #define'd.\n");
	}
	//Open the adjacency matrix file
	FILE *fp;
	fp = fopen (argv[1],"r");
	/*Step 1: geting the working vertexs and assigning values*/
  	for (i=0;i<NUM_NODES;i++) {
    		for (j=0;j<NUM_NODES;j++) {
     			fscanf(fp,"%d",&k);
			AdjMatrix[i][j]= k;
    		}
  	}
	fclose(fp);
	chStart=0,chEnd=1999; //15 for small input; 1999 for large input
	if (chStart == chEnd) {
		printf("Shortest path is 0 in cost. Just stay where you are.\n");
		
	}else{
		pthread_mutex_init(&pt_mutex, NULL);
		startBarrier(&myBarrier);	/* Start barrier 	*/
		startThreads();			/* Start pthreads */	
	}
	printResult();
	return 0;
}
