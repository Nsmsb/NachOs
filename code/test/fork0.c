#include "syscall.h"
#define THIS "aaa"
#define THAT "fff\0"

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

/*
	char *p;
	char *s = (char *) THAT;

	for(int i = 0 ; i < N ; i++){
		for(p = s ; *p != '\0' ; p++){
			PutChar(*p);
		}
	}
*/
	UserThreadJoin(tid);
	/*while(1){
		tid++;
	}*/

	return 0;
}
