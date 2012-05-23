/********************************************************************************
*						      			       				   
													SHA Secure Hash Algorithm				                                
*															Parallel 1.0 Version															
* Authors:           	Yuchen Liang and Syed Muhammad Zeeshan Iqbal
* Supervisor:					Hakan Grahn	 						
* Section:          	Computer Science of BTH,Sweden
* GCC Version:				4.2.4								
* Environment: 				Kraken(Ubuntu4.2.4) with Most 8 Processors 				
* Start Date:       	27th October 2009					
* End Date:         	23th November 2009			
*		
*********************************************************************************/


/* NIST Secure Hash Algorithm */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include <pthread.h>
#include "sha.h"

#include "params.h"

#define MAX_NO_FILES 16
#define PRINT 0


typedef struct Params {
	int s_index;
	int e_index;
	int no;
	char *name;
} parameters;

struct fileData{
	  BYTE* data;
	  int size;
}Tdata[MAX_NO_FILES];

char* in_file_list[]={"in_file1.asc","in_file2.asc","in_file3.asc","in_file4.asc","in_file5.asc","in_file6.asc","in_file7.asc","in_file8.asc","in_file9.asc","in_file10.asc","in_file11.asc","in_file12.asc","in_file13.asc","in_file14.asc","in_file15.asc","in_file16.asc",NULL};
char* out_file_list[]={"out_file1.txt","out_file2.txt","out_file3.txt","out_file4.txt","out_file5.txt","out_file6.txt","out_file7.txt","out_file8.txt",NULL};


int no_files=16;
int no_workers=PROCESSORS;

pthread_attr_t string_attr;
pthread_mutex_t string_mutex;
pthread_t workers[PROCESSORS];

static int partition_size;
static parameters paramsArr[PROCESSORS];


void readFilesData(){
	FILE *fin;
	int index;
	 
	 for ( index=0; index<no_files; index++){
	      if ((fin = fopen(in_file_list[index], "rb")) == NULL) {
					printf("Error opening %s for reading\n", in_file_list[index]);
					}		
	 			// Calculate File Size
    		fseek(fin, 0, SEEK_END);
				Tdata[index].size = ftell(fin);
				fseek(fin, 0, SEEK_SET);
								
		    Tdata[index].data = (BYTE *) calloc(sizeof(BYTE),Tdata[index].size+5);
		    
   			if (Tdata[index].size != fread(Tdata[index].data, sizeof(BYTE), Tdata[index].size, fin))			{
						free(Tdata[index].data);
			  		printf("Error: Could not read file!\n");
	   		}
		}
printf("File reading completed!\n");
}


void compute_digest(int index){
	
	   SHA_INFO sha_info;
    
	    	sha_stream(&sha_info, Tdata[index].data,Tdata[index].size);
				
				if (PRINT)			{
				   sha_print(&sha_info);	
				}
				 		 
}

void *parallel_SHA_Dig_Calc(void *params){
	
			 int index;
			 parameters *param=(parameters *)params;
       
       if (PRINT)
       printf("Worker number %d: started calculating digest.\n",param->no);
       for (index=param->s_index; index<param->e_index; index++){
       	     compute_digest(index);
        }
        
        if (PRINT)
        printf("Worker number %d: finished calculating digest.\n",param->no);
}

void init_workers(){
	
	pthread_attr_init(&string_attr);
	pthread_mutex_init(&string_mutex,NULL);
	pthread_attr_setdetachstate(&string_attr,PTHREAD_CREATE_JOINABLE);
	
}

void createWorkers(){
	long index;
	for ( index= 0 ; index<no_workers; index++){
		    pthread_create(&workers[index],&string_attr,parallel_SHA_Dig_Calc,(void *)&paramsArr[index]);
	}
	
}
void waitForWorkersToFinish(){
	int index;
	    for ( index= 0 ; index<no_workers; index++)
	    	   pthread_join(workers[index],NULL);
	    	   
 }                                               
 
void divide_into_sub_tasks(){

	  int i;
	  int s_index=0;
	  
	  
	  
	  for (i=0;i<no_workers;i++) {
			   paramsArr[i].s_index=s_index;
			   paramsArr[i].e_index=s_index+partition_size;
			   paramsArr[i].name="Worker";
			   paramsArr[i].no=i+1;
			   s_index+=partition_size;
			      
	  }
 
	  // It handles even not of jobs and odd no of workers vice versa.
	   paramsArr[i-1].e_index=paramsArr[i-1].e_index+(no_files%no_workers);
	   	  
}

  void show_sub_tasks() {

		int i;
			for (i=0;i<no_workers;i++) {
				   
				   printf("%d   ",paramsArr[i].s_index);
				   printf("%d   ",paramsArr[i].e_index);
				   printf("%s ",paramsArr[i].name);
				   printf("%d \n ",paramsArr[i].no);
		  }
}

void init(){
	  partition_size= MAX_NO_FILES/no_workers;
	  divide_into_sub_tasks();
	  if (PRINT)
	  		show_sub_tasks();
}   

void parallel_process(){
	 		printf("*******  Digest Calucation:Parallel Process  *******\n");
       
	    init();
	    init_workers();
	    createWorkers();
	    waitForWorkersToFinish();
	
}

void sequential_process(){
	
	      int index;
       
       printf("*******  Digest Calucation:Sequential Process  *******\n");
       
       for (index=0; index<no_files; index++){
       	if (PRINT){
       	    printf("-------*******************************-------------------\n");
       	    printf("Computing Digest for file: %s\n",in_file_list[index]);
       	  }
       	    
       	       compute_digest(index);
       	   
       	  if (PRINT){     
	       	    printf("Digest written to  file: %s\n",out_file_list[index]);
	       	    printf("-------*******************************-------------------\n");
       	  }
      }
	
}


int main(int argc,char *argv[])
{
	char *token;
      if (argc<2 ||argc>3){
      	  printf("|-----------------------------------------------------------------------|\n");
      	  printf("	Error: Insufficient Parameters.                             \n");
	  printf("	Maximum Workers are %d. Number of workers should be even!\n", PROCESSORS);
      	  printf("	Commands to run!                             \n");
      	  printf("	Command Format: OjbectFileName -Sequential(S)/Parallel(P) -Workers!\n");
      	  printf("	Example: sequential SHA: ' ./sha -S   '!                             \n");
      	  printf("	Example: parallel SHA  : ' ./sha -P -2 '!\n");
      	  printf("|---------------------------------------------------------------------- |\n");
      	  exit(0);
      }else {
      	    
      	    token=argv[1];
      	    if (*token=='-') {
      	    	   token++;
      	    	  if (strcmp(token,"S")==0){
      	    	  	    readFilesData();
			      	    	   sequential_process();
			      	    }else if (strcmp(token, "P")==0){
			      	    	     token= argv[2];
			      	    	     token++;
			      	    	     if (isdigit(*token)!=0){
			      	    	         no_workers= atoi(token);
						 /*if (no_workers%2 != 0 || no_workers >PROCESSORS)
			      	    	    		     printf("ERROR: Number of worker should be even and no more than 8\n");
			      	    	    		 else {*/
			      	    	    		 	   readFilesData();
			      	                   parallel_process();	
			      	               //}
			      	          }else{
			      	          	printf("ERROR: Invalid number of workers\n");
			      	          }
			      	  	}else {
			      	  		  printf("ERROR: Unknown Parameters!\n");
			      	  	}//end-if	  	
      	  }else {
      	  	 printf("ERROR: Unknown Parameters!\n");
      	  }
      	  	
      }
    return(0);
}
