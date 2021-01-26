



#include "syscall.h"



void testeopen(void* name){

	Open((char*)name);

}


int main(){


	char a='a';
	int tid;
	tid=UserThreadCreate(testeopen,(void *)&a,(int)UserThreadExit);
	return 0;
}
