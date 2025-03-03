// system.h 
//      All global variables used in Nachos are defined here.
//
// Copyright (c) 1992-1993 The Regents of the University of California.
// All rights reserved.  See copyright.h for copyright notice and limitation 
// of liability and disclaimer of warranty provisions.

#ifndef SYSTEM_H
#define SYSTEM_H

#include "copyright.h"
#include "utility.h"
#include "thread.h"
#include "scheduler.h"
#include "interrupt.h"
#include "stats.h"
#include "timer.h"

#define NbProcess 25

// Initialization and cleanup routines
extern void Initialize (int argc, char **argv);	// Initialization,
						// called before anything else
extern void Cleanup ();		// Cleanup, called when
						// Nachos is done.
void varprocessv();
void varprocessp();


extern Thread *currentThread;	// the thread holding the CPU
extern Thread *threadToBeDestroyed;	// the thread that just finished
extern Scheduler *scheduler;	// the ready list
extern Interrupt *interrupt;	// interrupt status
extern Statistics *stats;	// performance metrics
extern Timer *timer;		// the hardware alarm clock
extern int nbProcess;		// nombre de processus en cours d'éxécution 


#ifdef USER_PROGRAM
#include "machine.h"
#include "synchconsole.h"
#define MAX_STRING_SIZE 100
extern Machine *machine;	// user program memory and registers
extern SynchConsole *synchconsole;
extern int pidMax;
extern int *process;		// le processus qui sera en index process[i] avec son pid,
extern int *pointeursem;	// pourra être attendu grace a (semaphore*)pointeursem[i]
extern int *attenteprocess;	// il saura combien de processus l'attende grace a attenteprocess[i]
extern int *pointeurProcess;	// et sera accesible race aux pointeur (thrad*)pointeurProcess[i]
#endif

#ifdef FILESYS_NEEDED		// FILESYS or FILESYS_STUB
#include "filesys.h"
extern FileSystem *fileSystem;
#endif

#ifdef FILESYS
#include "synchdisk.h"
extern SynchDisk *synchDisk;
#endif

#ifdef NETWORK
#include "post.h"
extern PostOffice *postOffice;
#endif

#endif // SYSTEM_H
