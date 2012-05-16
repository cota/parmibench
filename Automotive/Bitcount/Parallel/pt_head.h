#include <pthread.h>
#include <stdlib.h>
#define PROCESSORS				8	//The amount of processors
#define GROUPS					3	//The amount of groups

int groups[PROCESSORS]; 	//a member is in which group
int members[GROUPS];		//how many members in a group

struct _FUNCARGS{
	/* bitcnt_1_parallel.c */
	long* inputs;			
	int* ns;
	/* bitstring_parallel_spf.c and bitstring_partition_spf.c*/
	int strwid;
	long iteration;
	int csize;
	/* bitcnts_parallel */
	int funcI;
	long randSeed;
	long iterations;
};
typedef struct _FUNCARGS FUNCARGS;

/* define a thread pool */
struct _THREADPOOL{
	int id;			/* thread id */
	pthread_t thread;		/* thread itself */
};
typedef struct _THREADPOOL THREADPOOL;
THREADPOOL threadPool[PROCESSORS];

struct _THREADARGS{
	int id;			/* thread id */
	FUNCARGS* funcArgs;	/* function arguments */	
};
typedef struct _THREADARGS THREADARGS;

struct _BARRIER{
	int count;
	pthread_mutex_t barrier_mutex;
	pthread_cond_t barrier_cond;
};
typedef struct _BARRIER BARRIER;
BARRIER myBarrier;

BARRIER myBarrier_group[GROUPS];

/* Forward declaring functions*/
extern void startBarrier(BARRIER*);
extern void actuateBarrier(BARRIER*);
extern void startThreads(int,FUNCARGS*);
extern void* bit_count_thread(void*);
extern void* bitstring_thread_parallel(void*);
extern void* bitstring_thread_partition(void*);
extern void* func_thread(void*);

extern void startBarrier_group(BARRIER*);
extern void actuateBarrier_group(BARRIER*,int);
