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
		nbProcess--;
		varprocessp();
		int i=0;
		while(process[i]!=currentThread->tid){
			i++;
		}
		while(attenteprocess[i] > 0){
		((Semaphore*)pointeursem[i])->V();
			attenteprocess[i]--;
		}
		varprocessv();
		return;
	}

	space = new AddrSpace (executable);
	currentThread->space = space;

	delete executable;		// close file

	space->InitRegisters ();	// set the initial register values
	space->RestoreState ();	// load page table register
	
	delete filename;

	DEBUG('t', "%s has address space at %d\n", currentThread->getName(), currentThread->space);
	DEBUG('t', "%s has physical page address %d\n", currentThread->getName(), currentThread->space);

	machine->Run ();		// jump to the user progam
	ASSERT (FALSE);		// machine->Run never returns

}

extern int do_ForkExec(char *filename){

	int arg = (int) filename;
	DEBUG('c', "Filename = %s\n", filename);
	char *name = new char [MAX_STRING_SIZE];

	varprocessp();

	int pid = pidMax;
	pidMax++;

	int i=0;
	while (process[i]!=-1 && i<NbProcess){
		i++;
	}
	if(i<NbProcess){
	snprintf(name, MAX_STRING_SIZE, "%s%d", "UserProcessus", pid);
	Thread *trd = new Thread(name);
	process[i]=pid;
	trd->tid=pid;
	trd->Fork(StartUserProcess, arg);
	}
	else{
		pid=999999999;
	}
	varprocessv();
	
	return pid;
}


extern void do_UserProcessJoin(int pid){

	int i=0;
	if(pid>pidMax){
		printf("le processus a attendre n'éxiste pas");
		return;
	}
	varprocessp();
	while(process[i]!=pid && i<NbProcess){
		i++;
	}
	if(i<NbProcess){
		attenteprocess[i]++;
		varprocessv();
		((Semaphore*)pointeursem[i])->P();
	}
	return;
}





