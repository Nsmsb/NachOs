/* userthread.cc
 *	Creates, starts and destroys user threads.
 *	
 *
 */


#include "userthread.h"
#include "system.h"
#include "thread.h"

static void StartUserThread(int f);


int do_UserThreadCreate(int f, int arg){
	int tid = 1;
	
	args_t *str = new args_t;
	str->fonction = f;
	str->arg = arg;
	str->SP = machine->ReadRegister(StackReg);

	Thread *trd = new Thread("user_thread");
	trd->space = currentThread->space;
	trd->Fork(StartUserThread, (int) str);

	return tid;
}


static void StartUserThread(int f){
	// Get back the arguments.
	args_t *str = (args_t *) f;

	for(int i = 0 ; i < NumTotalRegs ; i++){
		machine->WriteRegister(i, 0);	
	}

	machine->WriteRegister(PCReg, str->fonction);
	machine->WriteRegister(NextPCReg, str->fonction+4);
	machine->WriteRegister(4, str->arg);

	machine->WriteRegister(StackReg, str->SP - 3*PageSize);
	machine->Run();
}

void do_UserThreadExit(){
	currentThread->Finish();
}
