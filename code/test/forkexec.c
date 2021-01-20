#include "syscall.h"

int main(){
	int a = 0;
	ForkExec("../build/fork0");
	ForkExec("../build/fork1");
	
	while(1){
		a++;
	}
	
	return 0;
}
