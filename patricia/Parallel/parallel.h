#include <pthread.h>

#include "params.h"

/* define a thread pool */
struct _THREADPOOL{
	int id;			/* thread id */
	pthread_t thread;		/* thread itself */
};
typedef struct _THREADPOOL THREADPOOL;
THREADPOOL threadPool[PROCESSORS];

struct _THREADARGS{
	int id;			// thread id 
	patricia_tree_t *tree;
};
typedef struct _THREADARGS THREADARGS;

struct _BARRIER{
	int count;
	pthread_mutex_t barrier_mutex;
	pthread_cond_t barrier_cond;
};
typedef struct _BARRIER BARRIER;
BARRIER myBarrier;

/* Forward declaring functions*/
void startBarrier(BARRIER*);
void actuateBarrier(BARRIER*);
void startThreads(patricia_tree_t *);
void* search_best_thread(void*);

int search_size;
