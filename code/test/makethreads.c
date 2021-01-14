
#include "syscall.h"

void print(void *val){
	PutChar(*((char *)val));
	UserThreadExit();
}


int main(){
	char val = 'y';
	UserThreadCreate(print, ((void *) &val));
	while(1) {
		GetChar();
	}

	return 0;
}
