
#include "userprocess.h"

extern int do_ForkExec(char *filename){
	int pid = 1;

	Thread *trd = new Thread("UserProcessus");
	trd->Fork(StartProcess, filename);



	return pid;
}

