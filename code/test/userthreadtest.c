



#include "syscall.h"


void teste(void *a){

	int i=0;
	char c='i';
	while(c!='\0'){
		c=((char*)a)[i];
		PutChar(c);
		i++;
	}
	PutChar('\n');

}

void testec(void *a){

	PutChar('\n');
	PutChar(*(char*)a);
	PutChar('\n');


}

void testei(void *a){

	PutChar('\n');
	PutInt(*(int*)a);
	PutChar('\n');


}

int main (){

	char a[3];
	a[0]='a';
	a[1]='b';
	a[2]='\0';
	UserThreadCreate(teste,(void *)a,(int)UserThreadExit);
	return 0;
}











