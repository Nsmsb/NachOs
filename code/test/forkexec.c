#include "syscall.h"

int main(){
	int pid1,pid2;
	pid1=ForkExec("../build/fork0");
	pid2=ForkExec("../build/fork1");
	UserProcessJoin(pid1);
	UserProcessJoin(pid2);
	return 0;
}
