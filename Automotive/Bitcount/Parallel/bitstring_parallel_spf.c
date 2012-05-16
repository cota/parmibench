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

char* strs[PROCESSORS];
int tasks[PROCESSORS][2];
//When the amount of group is more than 4, change: char sx[GROUPS][180000000], otherwise char sx[GROUPS][780000000]
char sx[GROUPS][180000000];
long lengths[GROUPS];
long _lengths[PROCESSORS];


void bitstring_parallel(int iteration, int strwid,int csize){
	FUNCARGS* funcArgs = (FUNCARGS*)malloc(sizeof(FUNCARGS));
	funcArgs->iteration = iteration;
	funcArgs->strwid = strwid;
	//funcArgs->str = str;
	funcArgs->csize = csize;
	startBarrier_group(myBarrier_group);
	startThreads(2,funcArgs);
	free(funcArgs);
}

void* bitstring_thread_parallel(void* threadArgs){
	int i,j=0,x,y;
	int length=0;
	THREADARGS* args = (THREADARGS*)threadArgs;
	int myID = args->id;
	FUNCARGS* funcArgs = args->funcArgs;
	int strwid = funcArgs->strwid;
	long iteration = funcArgs->iteration;
	int csize = funcArgs->csize;
	int biz;
	long byze;
	for (x = (long)tasks[myID][0]+1; x <= (long)tasks[myID][1]; x++){
		actuateBarrier_group(myBarrier_group,myID);
		_lengths[myID] = 0;
		char* tmp_str = (char*)malloc(sizeof(char)*(csize));
		strs[myID] = tmp_str;
		biz = (int)x;
		byze = x;
		j = strwid - (biz + (biz >> 2)- (biz % 4 ? 0 : 1));		
		if(j>0){
			int start = j/members[groups[myID]]*(myID%members[groups[myID]]);
			int end = j/members[groups[myID]]*(myID%members[groups[myID]]+1)+(myID%members[groups[myID]]+1==members[groups[myID]]&&j%members[groups[myID]]!=0?j%members[groups[myID]]:0);
			for(i=start; i<end; i++){
				sx[groups[myID]][lengths[groups[myID]]+i] = ' ';
			}
			_lengths[myID] = end-start;
		}else{
			j=0;
		}
		for(i=biz/members[groups[myID]]*(myID%members[groups[myID]]+1)+(myID%members[groups[myID]]+1==members[groups[myID]]&&biz%members[groups[myID]]!=0?biz%members[groups[myID]]:0)-1; i>=biz/members[groups[myID]]*(myID%members[groups[myID]]); i--){
			*tmp_str++ = ((byze >> i) & 1) + '0';			
			if (!(i % 4) && i){
				*tmp_str++ = ' ';
			}
		}
		*tmp_str = '\0';
		actuateBarrier_group(myBarrier_group,myID);
		if(myID%members[groups[myID]] == 0){
			int y =0;
			long size = 0;
			for(y=myID+members[groups[myID]]-1;y>=myID;y--){
				size+=_lengths[y];
			}
			lengths[groups[myID]] = lengths[groups[myID]] + size;
			for(y=myID+members[groups[myID]]-1;y>=myID;y--){
				memcpy(sx[groups[myID]] + lengths[groups[myID]], strs[y], strlen(strs[y]));
				lengths[groups[myID]] += strlen(strs[y]);
			}
			sx[groups[myID]][lengths[groups[myID]]] = '\n';
			lengths[groups[myID]]++;
		}
		actuateBarrier_group(myBarrier_group,myID);
		free(strs[myID]);
	}	
}

void divide_task_group(int task){ // 2m-1,2g, 4m-1,2,4g, 6m-1,2,3,6g, 8m-1,2,4,8g
	int i=0,j=0;
	if(GROUPS==1){
		for(i=0;i<PROCESSORS;i++){
			tasks[i][0] = 0;
			tasks[i][1] = task;
			groups[i] = 0;
		}
		members[0] = PROCESSORS;
		return;
	}
	if(GROUPS == PROCESSORS){
		for(i=0;i<PROCESSORS;i++){
			tasks[i][0] = task/PROCESSORS* (i);
			tasks[i][1] = task/PROCESSORS* (i+1) + (i+1==PROCESSORS&task%PROCESSORS!=0?task%PROCESSORS:0);
			groups[i] = i;
			members[i] = 1;
		}
		return;
	}
	if(PROCESSORS==2 || PROCESSORS==4){
		for(i=0;i<PROCESSORS;i++){
			j = i<GROUPS-PROCESSORS%GROUPS?0:1;
			tasks[i][0] = task/GROUPS * (j);
			tasks[i][1] = task/GROUPS * (j+1) + (j+1==GROUPS&task%GROUPS!=0?task%GROUPS:0);
			groups[i] = j;
			members[j] ++;
		}
	}else if(PROCESSORS==6){
		for(i=0;i<PROCESSORS;i++){
			if(GROUPS==2){
				j = i<3?0:1;
			}else if(GROUPS == 3){
				if(i<2){
					j=0;
				}else if(i>1 && i<4){
					j=1;
				}else if(i>3){
					j=2;
				}
			}
			tasks[i][0] = task/GROUPS * (j);
			tasks[i][1] = task/GROUPS * (j+1) + (j+1==GROUPS&task%GROUPS!=0?task%GROUPS:0);
			groups[i] = j;
			members[j] ++;
		}
	}else if(PROCESSORS==8){
		for(i=0;i<PROCESSORS;i++){
			if(GROUPS==2){
				j = i<4?0:1;
			}else if(GROUPS == 4){
				if(i<2){
					j=0;
				}else if(i>1 && i<4){
					j=1;
				}else if(i>3 && i<6){
					j=2;
				}else if(i>5 && i<8){
					j=3;
				}
			}
			tasks[i][0] = task/GROUPS * (j);
			tasks[i][1] = task/GROUPS * (j+1) + (j+1==GROUPS&task%GROUPS!=0?task%GROUPS:0);
			groups[i] = j;
			members[j] ++;
		}
	}
}

#ifdef TEST

#include <stdlib.h>

int main(void){
	int i;
	long j;
	int strwid = 9200;
	int csize = 18000;
	int iteration = 9200; //tasks
	divide_task_group(iteration);
	bitstring_parallel(iteration, strwid,csize);
	for(i=0;i<GROUPS;i++){
		printf("%s",sx[i]);
	}
	return EXIT_SUCCESS;
}

#endif /* TEST */
