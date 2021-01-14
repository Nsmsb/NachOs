#include "copyright.h"
#include "system.h"
#include "console.h"
#include "synchconsole.h"
#include "addrspace.h"
#include "synch.h"
#include "userthread.h"


//prépare un thread utilisateur ,puis le lance sur la fonction voulu
//f est un pointeure sur int* qui contien les argument de la fonction


static void StartUserThread(int f){

	int p,*a=(int*)f,tid=a[2];

	currentThread->tid=a[2];
	currentThread->space->InitRegisters();

	currentThread->space->RestoreState();


	p=machine->ReadRegister(StackReg);
	
	machine->WriteRegister (4,a[1]);
	
	machine->WriteRegister (StackReg,p-2*(tid+1)*PageSize);

	machine->WriteRegister (PCReg,a[0]);
	machine->WriteRegister (NextPCReg,a[0]+4);


	delete a;


	machine->Run();

}

//le thread utilisateur a terminer vient se supprimer et indiquer aux thread qui l'attendais 
//qu'il a termeiner

void do_UserThreadExit(){
	
	currentThread->space->lockthreadp();
	while(currentThread->space->tid[currentThread->tid]!=0){
		((Semaphore *)currentThread->space->semthread[currentThread->tid])->V();
		currentThread->space->tid[currentThread->tid]--;
	}
	currentThread->space->tid[currentThread->tid]=-1;
	currentThread->space->lockthreadv();
	currentThread->space->userthread--;
	currentThread->space->haltv();
	currentThread->Finish();
	
}

//le thread appelant attend que le thread tid est fini pour continuer son éxécution

void UserThreadJoin(int tid){

	currentThread->space->tid[tid]++;
	((Semaphore *)currentThread->space->semthread[tid])->P();

}


//prépare la création des thread utilisateur

int do_UserThreadCreate(int f, int arg){

	int *i=new int[3];
	int j=0;
	i[0]=f;
	i[1]=arg;
	if((VoidFunctionPtr)f==NULL){
		return -1;
	}
	
	currentThread->space->lockthreadp();
	while(j<nbthread && currentThread->space->tid[j]!=-1){
		j++;
	}
	if(j>=nbthread){
		currentThread->space->lockthreadv();
		return -1;
	}
	currentThread->space->userthread++;	
	currentThread->space->tid[j]=0;
	currentThread->space->lockthreadv();
	i[2]=j;
	Thread *t = new Thread("UserThread");
	t->Fork (StartUserThread,(int)i);

	return  0;
}

