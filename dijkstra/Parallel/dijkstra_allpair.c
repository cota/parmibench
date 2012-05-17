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

#define NUM_NODES                          160		//16 for small input; 160 for large input
#define NONE                               9999		//Maximum

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

int nodes_tasks[NUM_NODES*(NUM_NODES-1)/2][2];
int tasks[PROCESSORS][2];

QITEM *qHead = NULL;
                  
int AdjMatrix[NUM_NODES][NUM_NODES];

int g_qCount[PROCESSORS];
int chStart,chEnd;
NODE rgnNodes[PROCESSORS][NUM_NODES];
UVERTEX uVertex[PROCESSORS];
int globalMiniCost[PROCESSORS];
int us[PROCESSORS];
int iPrev, iNode;

/* Forward declaring functions*/
void print_path(NODE*,int);
void enqueue(int,int,int);
void dequeue(int*,int*,int*);
int qcount (int);
void* startWorking(void*);
void startThreads(void);
int dijkstra(int);
void printResult(int,int,int);


void print_path(NODE *rgnNodes, int chNode){
	if ((rgnNodes+chNode)->iPrev != NONE){
		print_path(rgnNodes, (rgnNodes+chNode)->iPrev);
	}
	printf (" %d", chNode+1);
	fflush(stdout);
}

void printResult(int myID,int chStart, int chEnd){
	printf("From %d to %d, shortest path is %d in cost. ", chStart+1,chEnd+1, rgnNodes[myID][chEnd].iDist);
      printf("Path is:  %d",chStart+1);
      print_path(rgnNodes[myID], chEnd);
      printf("\n");
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


int qcount (int myID){
	return(g_qCount[myID]);
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


void* startWorking(void* threadArgs){ //
 	int i,j;
 	THREADARGS* args = (THREADARGS*)threadArgs;
	int myID = args->id;	
	dijkstra(myID);
	return NULL;
}


int dijkstra(int myID) {
	int x,i,v,iDist;
	int chStart, chEnd;
	int u =-1;
	for(x=tasks[myID][0]; x<tasks[myID][1]; x++){
		chStart = nodes_tasks[x][0];	//Start node
		chEnd = nodes_tasks[x][1];	//End node
		u=-1;
		//Initialize and clear	
		uVertex[myID].iDist=NONE;
		uVertex[myID].iPID=myID;
		uVertex[myID].iNID=NONE;
		g_qCount[myID] = 0;
		u=-1;
		for (v=0; v<NUM_NODES; v++) {
			rgnNodes[myID][v].iDist =  AdjMatrix[chStart][v];
			rgnNodes[myID][v].iPrev = NONE;
			rgnNodes[myID][v].iCatched = 0;
		}	
		//Start working
     		while (qcount(myID) < NUM_NODES-1){
			for (i=0; i<NUM_NODES; i++) {
				if(rgnNodes[myID][i].iCatched==0 && rgnNodes[myID][i].iDist<uVertex[myID].iDist && rgnNodes[myID][i].iDist!=0){
					uVertex[myID].iDist=rgnNodes[myID][i].iDist;
					uVertex[myID].iNID=i;
				}		
			}				
			globalMiniCost[myID]=NONE;
			if(globalMiniCost[myID]>uVertex[myID].iDist){
				globalMiniCost[myID] = uVertex[myID].iDist;
				u=uVertex[myID].iNID;
				g_qCount[myID]++;
			}	
			for (v=0; v<NUM_NODES; v++) {
				if(v==u){
					rgnNodes[myID][v].iCatched = 1;
					continue;
				}
				if((rgnNodes[myID][v].iCatched==0 && rgnNodes[myID][v].iDist>(rgnNodes[myID][u].iDist+AdjMatrix[u][v]))){	
					rgnNodes[myID][v].iDist=rgnNodes[myID][u].iDist+AdjMatrix[u][v];	
					rgnNodes[myID][v].iPrev = u; 		
				}
			}
			uVertex[myID].iDist = NONE;	//Reset 
		}
		printResult(myID,chStart,chEnd);
	}  
}


void divide_task_group(int task){ // 2m-1,2g, 4m-1,2,4g, 6m-1,2,3,6g, 8m-1,2,4,8g
	int i;
	for(i=0;i<PROCESSORS;i++){
			tasks[i][0] = task/PROCESSORS* (i);
			tasks[i][1] = task/PROCESSORS* (i+1) + (i+1==PROCESSORS&task%PROCESSORS!=0?task%PROCESSORS:0);
	}
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
	int tasks= NUM_NODES*(NUM_NODES-1)/2;
	int x=0;
	for(i=0;i<159;i++){ //small:15; large:159
		for(j=i+1;j<160;j++){	//small:16; large:160
			nodes_tasks[x][0] = i;
			nodes_tasks[x][1] = j;
			x++;
		}
	}
	divide_task_group(tasks);
	startThreads();			/* Start pthreads */	
	exit(0);
}
