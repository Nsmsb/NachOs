#include "syscall.h"

int main(){
	int tmp=0;
	tmp=ForkExec("../build/fork0");
	//ForkExec("../build/fork1");
	return 0;
}
