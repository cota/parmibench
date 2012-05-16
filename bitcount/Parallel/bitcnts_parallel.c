/*********************************************************************************************************
*						      			       				   
* 				Bitcount			                                
*		Parallel 1.0 Version with Pthread
*															
* Authors:           	Yuchen Liang and Syed Muhammad Zeeshan Iqbal
* Supervisor:		Hakan Grahn	 						
* Section:          	Computer Science of BTH,Sweden
* GCC Version:		4.2.4								
* Environment: 		Kraken(Ubuntu4.2.4) with Most 8 Processors 				
* Start Date:       	19th October 2009					
* End Date:         	29th October 2009			
*		
************************************************************************************************************/


/* +++Date last modified: 05-Jul-1997 */

/*
**  BITCNTS.C - Test program for bit counting functions
**
**  public domain by Bob Stout & Auke Reitsma
*/

#include <stdio.h>
#include <stdlib.h>
#include "conio.h"
#include <limits.h>
#include <time.h>
#include <float.h>
#include "bitops.h"
#include "pt_head.h"

#define FUNCS  6

long ns[PROCESSORS];

int cminix, cmaxix;
	double ct, cmin = DBL_MAX, cmax = 0;

static int CDECL bit_shifter(long int x);

static int (* CDECL pBitCntFunc[FUNCS])(long) = {
	//bit_count,
	bitcount,
	ntbl_bitcnt,
	ntbl_bitcount,
	/*            btbl_bitcnt, DOESNT WORK*/
	BW_btbl_bitcount,
	AR_btbl_bitcount,
	bit_shifter
};

static char *text[FUNCS] = {
	//"Optimized 1 bit/loop counter",
	"Ratko's mystery algorithm",
	"Recursive bit count by nybbles",
	"Non-recursive bit count by nybbles",
	/*            "Recursive bit count by bytes",*/
	"Non-recursive bit count by bytes (BW)",
	"Non-recursive bit count by bytes (AR)",
	"Shift and count bits"
};

static int CDECL bit_shifter(long int x){
	int i, n;
	for (i = n = 0; x && (i < (sizeof(long) * CHAR_BIT)); ++i, x >>= 1){
		n += (int)(x & 1L);
	}
	return n;
}

void* func_thread(void* threadArgs){
	int i;
	clock_t start, stop;
	long n=0;
	long j, seed;
	THREADARGS* args = (THREADARGS*)threadArgs;
	int myID = args->id;
	FUNCARGS* funcArgs = args->funcArgs;
	long randSeed = funcArgs->randSeed;
	int funcI = funcArgs->funcI;
	long iterations = funcArgs->iterations;
	ns[myID] = 0;
	for (i = n = 0; i < FUNCS; i++) {
		actuateBarrier(&myBarrier);	
		if(myID==0){
    			start = clock();
		}
		for (j = iterations/PROCESSORS*myID, seed = randSeed+(iterations/PROCESSORS*myID*13); j < iterations/PROCESSORS*(myID+1)+(myID+1==PROCESSORS&&iterations%PROCESSORS!=0?iterations%PROCESSORS:0); j++, seed += 13){
			ns[myID] += pBitCntFunc[funcI](seed);
    		}
		actuateBarrier(&myBarrier);	
		if(myID==0){
			stop = clock();
    			ct = (stop - start) / (double)CLOCKS_PER_SEC;
    			if (ct < cmin) {
	 			cmin = ct;
	 			cminix = i;
    			}
    			if (ct > cmax) {
	 			cmax = ct;
	 			cmaxix = i;
    			}  
			for(j=n=0;j<PROCESSORS;j++){
				n+=ns[j];
				ns[j] = 0;
			} 
    			printf("%-38s> Time: %7.3f sec.; Bits: %ld\n", text[i], ct, n);
		}
	}
}


#ifdef TEST
int main(int argc, char *argv[]){
	clock_t start, stop;
	double ct, cmin = DBL_MAX, cmax = 0;
	int i, j;
	int iterations;
	long n;
	if (argc<2) {
    		fprintf(stderr,"Usage: bitcnts <iterations>\n");
    		exit(-1);
	}
  	iterations=atoi(argv[1]);
  	puts("Bit counter algorithm benchmark\n");
	FUNCARGS* funcArgs = (FUNCARGS*)malloc(sizeof(FUNCARGS));
	//funcArgs->randSeed = rand();
	funcArgs->randSeed = 112500;
	funcArgs->iterations = iterations;
	startBarrier(&myBarrier);
    	startThreads(4,funcArgs);
	free(funcArgs);
	/*
	FUNCARGS* funcArgs = (FUNCARGS*)malloc(sizeof(FUNCARGS));
  	for (i = n = 0; i < FUNCS; i++) {
    		start = clock();
		funcArgs->funcI = i;
		//funcArgs->randSeed = rand();
		funcArgs->randSeed = 112500;
		funcArgs->iterations = iterations;
		startBarrier(&myBarrier);
    		startThreads(3,funcArgs);
		for(j=n=0;j<PROCESSORS;j++){
			n+=ns[j];
			ns[j] = 0;
		}
    		stop = clock();
    		ct = (stop - start) / (double)CLOCKS_PER_SEC;
    		if (ct < cmin) {
	 		cmin = ct;
	 		cminix = i;
    		}
    		if (ct > cmax) {
	 		cmax = ct;
	 		cmaxix = i;
    		}   
    		printf("%-38s> Time: %7.3f sec.; Bits: %ld\n", text[i], ct, n);
  	}
	free(funcArgs);
	*/
	printf("\nBest  > %s\n", text[cminix]);
  	printf("Worst > %s\n", text[cmaxix]);
  	return 0;
}
#endif /* TEST */
