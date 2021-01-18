#include "userprocess.h"
#include "thread.h"
#include "copyright.h"
#include "system.h"
#include "addrspace.h"

static void StartUserProcess(int arg){
	char *filename = (char *) arg;
	DEBUG('c', "Filename after cast = %s\n", filename);
	OpenFile *executable = fileSystem->Open (filename);
	AddrSpace *space;

	if (executable == NULL)
	{
		printf ("Unable to open file %s\n", filename);
		return;
	}
	space = new AddrSpace (executable);
	currentThread->space = space;

	delete executable;		// close file

	space->InitRegisters ();	// set the initial register values
	space->RestoreState ();	// load page table register
	
	delete filename;

	machine->Run ();		// jump to the user progam
	ASSERT (FALSE);		// machine->Run never returns

}

extern int do_ForkExec(char *filename){
	int pid = 1;

	DEBUG('c', "Filename = %s\n", filename);

	int arg = (int) filename;
	Thread *trd = new Thread("UserProcessus");
	trd->Fork(StartUserProcess, arg);

	return pid;
}

