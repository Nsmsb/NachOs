#include "syscall.h"

void user_pages_thread(int *arg){
	char c = *(char*)arg;
	for (int i = 0; i < 15; i++)
	{
		PutChar(c);
	}
	PutChar('\n');
	UserThreadExit();
}

int main(){
	int tid1, tid2;
	char c1 = 'a', c2 = 'b';

	tid1 = UserThreadCreate((void (*)(void *))user_pages_thread,(void *)(&c1));
	tid2 = UserThreadCreate((void (*)(void *))user_pages_thread,(void *)(&c2));
	UserThreadJoin(tid1);
	UserThreadJoin(tid2);
	PutString("TERMINATED.\n");
	Exit(0);
}