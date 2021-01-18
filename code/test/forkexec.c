#include "syscall.h"

int main(){
	ForkExec("../build/fork0");
	ForkExec("../build/fork1");
	GetChar();
	return 0;
}
