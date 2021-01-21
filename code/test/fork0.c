#include "syscall.h"
#define THIS "aaa"
#define THAT "fff"

const int N = 10;

void puts(char *s){
	char *p;
	for (p = s ; *p != '\0' ; p++)
		PutChar(*p);
}

void f(void *s){
	int i ;
	for(i = 0 ; i < N ; i++)
		puts((char *)s);

	//UserThreadExit();
}

int main()
{
	int tid = UserThreadCreate(f, (void *) THIS,(int)UserThreadExit);

	f((void *) THAT);
	UserThreadJoin(tid);

	return 0;
}
