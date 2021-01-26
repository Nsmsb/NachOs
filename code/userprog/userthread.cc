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

	int p,*a=(int*)f,pospile=a[2];
	int *arg=(int*)a[1];

	currentThread->tid=currentThread->space->pile[pospile];
	currentThread->space->InitRegisters();

	currentThread->space->RestoreState();


	p=machine->ReadRegister(StackReg);
	
	machine->WriteRegister (4,arg[0]);
	
	machine->WriteRegister (StackReg,p-2*(pospile+1)*PageSize);

	machine->WriteRegister (PCReg,a[0]);
	machine->WriteRegister (NextPCReg,a[0]+4);
	machine->WriteRegister (RetAddrReg,arg[1]);

	delete a;


	machine->Run();

}

//le thread utilisateur a terminer vient se supprimer et indiquer aux thread qui l'attendais 
//qu'il a termeiner

void do_UserThreadExit(){
	int pPile=cherchepospile(currentThread->tid);
	currentThread->space->lockthreadp();
	while(currentThread->space->tid[pPile]!=0){
		((Semaphore *)currentThread->space->semthread[pPile])->V();
		currentThread->space->tid[pPile]--;
	}
	currentThread->space->pile[pPile]=-1;
	currentThread->space->lockthreadv();
	currentThread->space->userthread--;
	
	currentThread->space->haltv();
	currentThread->Finish();
	
}

int cherchepospile(int tid){

	int i=0;
	while(currentThread->space->pile[i]!=tid && i<nbthread){
		i++;
	}
	return i;
}


//le thread appelant attend que le thread tid est fini pour continuer son éxécution

void do_UserThreadJoin(int tid){

	if(tid>=currentThread->space->tidMax){
		synchconsole->SynchPutString("le thread a attendre n'éxiste pas");
	}
	else{
		currentThread->space->lockthreadp();
		int pospile=cherchepospile(tid);
		if(pospile>=nbthread){
			currentThread->space->lockthreadv();
			return;
		}
		currentThread->space->tid[pospile]++;
		currentThread->space->lockthreadv();
		((Semaphore *)currentThread->space->semthread[pospile])->P();
	}

}


//prépare la création des thread utilisateur

int do_UserThreadCreate(int f, int arg){


	int tiduse;
	int *i=new int[3];
	int j=0;
	i[0]=f;
	i[1]=arg;
	if((VoidFunctionPtr)f==NULL){
		return -1;
	}
	
	currentThread->space->lockthreadp();
	while(j<nbthread && currentThread->space->pile[j]!=-1){
		j++;
	}
	if(j>=nbthread){
		currentThread->space->lockthreadv();
		return -1;
	}
	currentThread->space->userthread++;
	tiduse=currentThread->space->tidMax;
	currentThread->space->pile[j]=tiduse;
	currentThread->space->tidMax++;
	currentThread->space->lockthreadv();

	i[2]=j;
	char *name = new char [MAX_STRING_SIZE];
	snprintf(name, MAX_STRING_SIZE, "%s%d", "UserThread", tiduse);
	Thread *t = new Thread(name);
	t->Fork (StartUserThread,(int)i);

	return  tiduse;
}


int do_open(char *name){

	int i=0;
	semfilep();
	while(threadproprietaire[i]==-1 &&i<10){
		i++;
	}
	if(i>=10){
		printf("trops de fichier ouvert pour ouvrire %s\n",name);
	}
	printf("overture de %s\n",name);
	//si pas encore ouvert
	threadproprietaire[i]=(int)currentThread;
	

	semfilev();
	return 0;

}


int do_close(int id){

	semfilep();
	if((int)currentThread==threadproprietaire[id]{
		printf("fermeture de %d\n",id);
		//close file[id]
		threadproprietaire[id]=-1;
	}
	else{
		printf("vous ne pouver pas fermer se fichier%d\n",id);
	}

	semfilev();
	return 0;

}




