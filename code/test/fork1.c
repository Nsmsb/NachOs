#include "syscall.h"
#define THIS "ppp"
#define THAT "lll"


const int N = 10;

void f(void *s){
	int i ;
	for(i = 0 ; i < N ; i++)
		PutInt(i);

	//UserThreadExit();
}

int main()
{
	int tid = UserThreadCreate(f, (void *) THIS,(int)UserThreadExit);
	int val = -20;
	f((void *) val);
	UserThreadJoin(tid);


	return 0;
}
