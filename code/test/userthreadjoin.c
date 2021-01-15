



#include "syscall.h"




void teste(void *a){

	PutChar(*(char*)a);
	PutChar('\n');
	UserThreadExit();

}



int main(){


	char a='a';
	int tid;
	for (int i=0;i<24;i++){
		tid=UserThreadCreate(teste,(void *)&a);
		UserThreadJoin(tid);
		a++;
	}	
	return 0;
}
