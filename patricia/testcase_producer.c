#include <stdlib.h>
#include <time.h>

#define PROCESSORS 4

void getABusyMachine(){	//Random polling
	int machineID;
	srand( (unsigned)time(NULL));
	int i=0;
	for(i=0;i<10;i++){
		machineID = rand()%PROCESSORS;
		printf("machine id=%d\n",machineID);
	}
}

void create_test_file(){
	int i=0;
	int j=0;
	srand(100);
	for(i=0;i<160;i++){
		for(j=0;j<160;j++){
			int tmp= rand()%100;
			if(tmp==0){
				tmp = 50;
			}
			printf("%d ",tmp);
		}
		printf("\n");
	}
}

void create_test_ip(){
	int i=0;
	int y=32;
	srand((unsigned)time(NULL));
	for(i=0;i<5000;i++){
		y=32;
		int x0 = rand()%255;
		if(x0<20){
			x0=0;
			y=y-8;
		}
		int x1 = rand()%255;
		if(x0==0 && x1<20){
			x1=0;
			y=y-8;
		}
		int x2 = rand()%255;
		if(x2<20 && x1==0 && x0==0){
			x2=0;
			y=y-8;
		}
		int x3 = rand()%255;
		if(x3==0 && x2==0 && x1==0 && x0==0){
			y=y-8;
		}
		printf("%d.%d.%d.%d/%d\n",x0,x1,x2,x3,y);
	}
}

int main(int argc, char *argv[]){
	create_test_ip();	
}
