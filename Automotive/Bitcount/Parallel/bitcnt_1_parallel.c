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
**  Bit counter by Ratko Tomic
*/

#include "bitops.h"
#include "pt_head.h"
#include "string.h"

#define LENGTH 2000000

/*
** The loop will execute once for each bit of x set, this is in average
** twice as fast as the shift/test method.
*/
int ns[PROCESSORS];
long x;

int CDECL bit_count(long* inputs){	
	int i,n= 0;
	FUNCARGS* funcArgs = (FUNCARGS*)malloc(sizeof(FUNCARGS));
	funcArgs->inputs = inputs;
	startThreads(1,funcArgs);
	free(funcArgs);
	for(i=0;i<PROCESSORS;i++){
		//n+=ns[j][i];
		n+=ns[i];
	}
	return (n);
}


void* bit_count_thread(void* threadArgs){
	int i,j,n = 0; 
	THREADARGS* args = (THREADARGS*)threadArgs;
	int myID = args->id;
	/* the max hexadecimal length of LONG type is 16 in 64-bite, coverring 8 in 32-bit */
	char* str = malloc(17);
	sprintf(str,"%X",x);
	int bit_length = strlen(str);
	int bit_start = bit_length/PROCESSORS*myID;
	int bit_end = bit_length/PROCESSORS*(1+myID) + (myID+1==PROCESSORS && bit_length%PROCESSORS!=0? bit_length%PROCESSORS : 0);
	char* local_str = (char*) malloc(bit_length+1);
	strncpy(local_str,str+bit_start,bit_end-bit_start);
	local_str[bit_length] = '\0';
	unsigned long global_x = strtol(local_str, NULL, 16);
	long local_x;	
	for(j=0;j<LENGTH;j++){
		/* different cpu processes a certain range of bits. i.e, cpu0 processes 1~16 bits, cpu1 processes 17~32 bits */
		local_x = global_x;
		if(local_x){
			do{
				ns[myID]++;
        		}while (0 != (local_x = local_x&(local_x-1))) ;
		}
	}
}




#include <stdlib.h>
#include "snip_str.h"               /* For plural_text() macro    */


//#ifdef TEST
main(int argc, char *argv[])
{				
/*
      long n;
      while(--argc){
            int i;
            n = atol(*++argv);
            i = bit_count(n);
            printf("%ld contains %d bit%s set\n",n, i, plural_text(i));
      	}
*/
	unsigned long inputs[LENGTH];
	int i=0;
	unsigned long n = atol(*++argv);
	x= n;
	for(i=0;i<LENGTH;i++){
		inputs[i] = n;
	}
	int sum = bit_count(inputs);
	printf("%d times %ld contains %d bit%s set\n", LENGTH, n, sum, plural_text(sum));
      return 0;
}

//#endif /* TEST */
