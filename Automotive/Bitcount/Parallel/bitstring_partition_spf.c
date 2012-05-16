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
** bitstring(): print bit pattern of bytes formatted to string.
**
** By J. Blauth, Sept. 1992. Hereby placed into the public domain.
**
** byze:    value to transform to bitstring.
** biz:     count of bits to be shown (counted from lowest bit, can be any
**          even or odd number).
** strwid:  total width the string shall have. Since between every 4 bits a
**          blank (0x20) is inserted (not added after lowest bit), width of
**          bitformat only is (biz+(biz/4-1)). Bits are printed right aligned,
**          positions from highest bit to start of string filled with blanks.
**          If value of strwid smaller than space needed to print all bits,
**          strwid is ignored (e.g.:
**                bitstr(s,b,16,5) results in 19 chars +'\0').
**
**   EXAMPLE:
**   for (j = 1; j <= 16; j++) { bitstring(s, j, j, 16); puts(s); }
**       1:                1
**       2:               10
**       3:              011
**       d: 0 0000 0000 1101
**       e: 00 0000 0000 1110
**       f: 000 0000 0000 1111
*/

#include "bitops.h"
#include "pt_head.h"
#include <string.h>

char strs[PROCESSORS][78000000];


long bitstring_original(char *str, long byze, int biz, int strwid, int csize, long length){
      int i, j; 
	long x = 0L;
      j = strwid - (biz + (biz >> 2)- (biz % 4 ? 0 : 1));
      for (i = 0; i < j; i++){
		*(str+length+i) = ' ';
	}
	x = i;
      while (--biz >= 0){
		*(str+length+x) = ((byze >> biz) & 1) + '0';
		x++;
            if (!(biz % 4) && biz){
			*(str+length+x)=' ';
			x++;
		}
	}
	*(str+length+x)='\n';
	length = length + x + 1;
	return length;
}

void bitstring_partition(int iteration, int strwid, int csize){
	FUNCARGS* funcArgs = (FUNCARGS*)malloc(sizeof(FUNCARGS));
	funcArgs->iteration = iteration;
	funcArgs->strwid = strwid;
	funcArgs->csize = csize;
	startThreads(3,funcArgs);
	free(funcArgs);
}

void* bitstring_thread_partition(void* threadArgs){
	long x=0L;
	long length=0L;
	THREADARGS* args = (THREADARGS*)threadArgs;
	int myID = args->id;
	FUNCARGS* funcArgs = args->funcArgs;
	int strwid = funcArgs->strwid;
	long iteration = funcArgs->iteration;
	int csize = funcArgs->csize;
	long start = 1L+(iteration-1L)/PROCESSORS*myID;
	long end = 1L+(iteration-1L)/PROCESSORS*(myID+1) + (myID+1==PROCESSORS && (iteration-1L)%PROCESSORS!=0 ? (iteration-1L)%PROCESSORS : 0) + (myID+1 ==PROCESSORS?1:0);
	for (x=start; x<end; x++){
		//bitstring_original(strs[myID]+length,x,x,iteration, csize, &length);
		length = bitstring_original(strs[myID],x,x,iteration, csize, length);	
	}
	strs[myID][length+1] = '\0';
}


#ifdef TEST

#include <stdlib.h>

int main(void){
	int i;
	long j;
	int strwid = 9200;
	int csize = 10000;
	int iteration = 9200; //tasks
	bitstring_partition(iteration, strwid,csize);
	for(i=0;i<PROCESSORS;i++){
		printf("%s",strs[i]);
	}
	return EXIT_SUCCESS;
}

#endif /* TEST */
