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
	int iCatched;
};
typedef struct _NODE NODE;


struct _QITEM{
	int iNode;
	int iDist;
	int iPrev;
	struct _QITEM *qNext;
};
typedef struct _QITEM QITEM;


struct _UVERTEX{
	int iPID;
	int iNID;
	int iDist;
};
typedef struct _UVERTEX UVERTEX;

QITEM *qHead = NULL;
                  
int AdjMatrix[NUM_NODES][NUM_NODES];

int g_qCount = 0;
int u=-1;
int chStart,chEnd;
NODE rgnNodes[NUM_NODES];
int ch;
UVERTEX uVertex[PROCESSORS];
int times[PROCESSORS]; 
int globalMiniCost=NONE;
int iPrev, iNode;

/* Forward declaring functions*/
void print_path(NODE*,int);
void enqueue(int,int,int);
void dequeue(int*,int*,int*);
int qcount (void);
void startBarrier(BARRIER*);
void actuateBarrier(BARRIER*);
void* startWorking(void*);
void startThreads(void);
int dijkstra(int);
void printResult();


void print_path(NODE *rgnNodes, int chNode){
	if (rgnNodes[chNode].iPrev != NONE){
		print_path(rgnNodes, rgnNodes[chNode].iPrev);
	}
	printf (" %d", chNode);
	fflush(stdout);
}


void enqueue (int iNode, int iDist, int iPrev){
	QITEM *qNew = (QITEM *) malloc(sizeof(QITEM));
	QITEM *qLast = qHead;  
	if (!qNew) {
      	fprintf(stderr, "Out of memory.\n");
      	exit(1);
	}
	qNew->iNode = iNode;
	qNew->iDist = iDist;
	qNew->iPrev = iPrev;
	qNew->qNext = NULL;  
	if (!qLast) {
		qHead = qNew;
	}else{
		while (qLast->qNext){ 
			qLast = qLast->qNext;
		}
		qLast->qNext = qNew;
	}
	//g_qCount++;
}


void dequeue (int *piNode, int *piDist, int *piPrev){
	QITEM *qKill = qHead;
	if (qHead){
      	*piNode = qHead->iNode;
      	*piDist = qHead->iDist;
      	*piPrev = qHead->iPrev;
      	qHead = qHead->qNext;
     		free(qKill);
		//g_qCount--;
	}
}


int qcount (void){
	return(g_qCount);
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
	uVertex[myID].iDist=NONE;
	uVertex[myID].iPID=myID;
	uVertex[myID].iNID=NONE;								
	/* Step 2: */
	for (i=myID*(NUM_NODES/PROCESSORS); (i<(myID+1)*(NUM_NODES/PROCESSORS)+(myID+1==PROCESSORS && NUM_NODES%PROCESSORS!=0 ? NUM_NODES%PROCESSORS:0)); i++) {
		rgnNodes[i].iDist =  AdjMatrix[chStart][i];
		rgnNodes[i].iPrev = NONE;
	}
	actuateBarrier(&myBarrier);				/* Actuate the barrier */
	/* Step 3: */
	dijkstra(myID);
	return NULL;
}


int dijkstra(int myID) {
	int i,v, iDist;
     	while (qcount() < NUM_NODES-1){	
		for (i=myID*(NUM_NODES/PROCESSORS); (i<(myID+1)*(NUM_NODES/PROCESSORS)+(myID+1==PROCESSORS && NUM_NODES%PROCESSORS!=0 ? NUM_NODES%PROCESSORS:0)); i++) {
			if(rgnNodes[i].iCatched==0 && rgnNodes[i].iDist<uVertex[myID].iDist && rgnNodes[i].iDist!=0){
				uVertex[myID].iDist=rgnNodes[i].iDist;
				uVertex[myID].iNID=i;
			}		
		}
		actuateBarrier(&myBarrier);				/* Actuate the barrier */
		if(myID==0){ 
			globalMiniCost=NONE;
			for(i=0;i<PROCESSORS;i++){			/* Get the global minimum cost */
				if(globalMiniCost>uVertex[i].iDist){
					globalMiniCost = uVertex[i].iDist;
					u=uVertex[i].iNID;
				}
			}
			g_qCount++;
		}
		actuateBarrier(&myBarrier);				/* Actuate the barrier */
		for (v=myID*(NUM_NODES/PROCESSORS); (v<(myID+1)*(NUM_NODES/PROCESSORS)+(myID+1==PROCESSORS && NUM_NODES%PROCESSORS!=0 ? NUM_NODES%PROCESSORS:0)); v++) {
			if(v==u){
				rgnNodes[v].iCatched = 1;
				continue;
			}
			if((rgnNodes[v].iCatched==0 && rgnNodes[v].iDist>(rgnNodes[u].iDist+AdjMatrix[u][v]))){			/* dist(v)>dist(u)+w(u,v) */
				rgnNodes[v].iDist=rgnNodes[u].iDist+AdjMatrix[u][v];
				rgnNodes[v].iPrev = u; 		
			}
		}
		uVertex[myID].iDist = NONE;	//Reset 
	}    
}



void printResult(){
	printf("Shortest path is %d in cost. ", rgnNodes[chEnd].iDist);
      printf("Path is: %d",chStart);
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
		startBarrier(&myBarrier);	/* Start barrier 	*/
		startThreads();			/* Start pthreads */	
	}
	printResult();
//}
	exit(0);
}
