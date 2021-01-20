#include "syscall.h"
#define THIS "ppp"
#define THAT "lll"


const int N = 10;

void f(void *s){
	int i ;
	for(i = 0 ; i < N ; i++)
		PutInt(i);

	UserThreadExit();
}

int main()
{
	int tid = UserThreadCreate(f, (void *) THIS,(int)UserThreadExit);

	//for(int i = 100 ; i < N+100 ; i++)
	//	PutInt(i);

	UserThreadJoin(tid);

	while(1){
		tid++;
	}

	return 0;
}
