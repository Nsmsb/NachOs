// addrspace.h 
//      Data structures to keep track of executing user programs 
//      (address spaces).
//
//      For now, we don't keep any information about address spaces.
//      The user level CPU state is saved and restored in the thread
//      executing the user program (see thread.h).
//
// Copyright (c) 1992-1993 The Regents of the University of California.
// All rights reserved.  See copyright.h for copyright notice and limitation 
// of liability and disclaimer of warranty provisions.

#ifndef ADDRSPACE_H
#define ADDRSPACE_H

#include "copyright.h"
#include "filesys.h"
#include "translate.h"

#define UserStackSize		3072	// increase this as necessary!

class Semaphore;
#define nbthread (UserStackSize)/(2*(PageSize))

class AddrSpace
{
  public:

    AddrSpace (OpenFile * executable);	// Create an address space,
    // initializing it with the program
    // stored in the file "executable"
    ~AddrSpace ();		// De-allocate an address space

    void haltv();
    void haltp();
    void lockthreadp();
    void lockthreadv();
    int allocthread(int tid);

    void InitRegisters ();	// Initialize user-level CPU registers,
    // before jumping to user code

    void SaveState ();		// Save/restore address space-specific
    void RestoreState ();	// info on a context switch 
    int userthread;		// nombre de thread du pocessus
    int tidMax;			//le prochain numéros de tid a donner
    int *tid;			//tableaux indiquant que le thread a l'indice i du tabeaux est  
				//attendu par tid[i] thread 
    int *pile;			//tableaux indiquant quelle espace de la pile pouvant acueillire un
				//thread est libre valeur -1 et le tid du thread occupant la zone
				//de la pile correspondant
    int *semthread;		//tableaux de Semaphore * convertie en int,chaque Semaphore est
 				//relier a un tid
    int pid;			//pid du processus
    int lockthread;
    int halt;			//Semaphore pour que le thread main attende la fin de tout les thread
    int *pointeurThread;	//liste de pointeur des thread du processus
    int reussite;


	Semaphore *nbThreads;

  private:
      TranslationEntry * pageTable;	// Assume linear page table translation
    // for now!
    unsigned int numPages;	// Number of pages in the virtual 
    // address space
};

#endif // ADDRSPACE_H
