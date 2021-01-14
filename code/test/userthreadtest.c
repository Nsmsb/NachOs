



#include "syscall.h"


void teste(void *a){

	PutChar(*(char*)a);
	PutChar('\n');
	/*PutChar('a');
	PutChar('\n');*/
	UserThreadExit();

}


int main (){

	char a='a';
	UserThreadCreate(teste,(void *)&a);
	char i=a+1;
	UserThreadCreate(teste,(void *)&i);
	char r=i+1;
	UserThreadCreate(teste,(void *)&r);
	char p=r+1;
	UserThreadCreate(teste,(void *)&p);
	char t=p+1;
	UserThreadCreate(teste,(void *)&t);
	char y=t+1;
	UserThreadCreate(teste,(void *)&y);
	/*char o=y+1;
	UserThreadCreate(teste,(void *)&o);*/
	return 0;
}
