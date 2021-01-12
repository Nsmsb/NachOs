// exception.cc 
//      Entry point into the Nachos kernel from user programs.
//      There are two kinds of things that can cause control to
//      transfer back to here from user code:
//
//      syscall -- The user code explicitly requests to call a procedure
//      in the Nachos kernel.  Right now, the only function we support is
//      "Halt".
//
//      exceptions -- The user code does something that the CPU can't handle.
//      For instance, accessing memory that doesn't exist, arithmetic errors,
//      etc.  
//
//      Interrupts (which can also cause control to transfer from user
//      code into the Nachos kernel) are handled elsewhere.
//
// For now, this only handles the Halt() system call.
// Everything else core dumps.
//
// Copyright (c) 1992-1993 The Regents of the University of California.
// All rights reserved.  See copyright.h for copyright notice and limitation 
// of liability and disclaimer of warranty provisions.

#include "copyright.h"
#include "system.h"
#include "syscall.h"

//----------------------------------------------------------------------
// UpdatePC : Increments the Program Counter register in order to resume
// the user program immediately after the "syscall" instruction.
//----------------------------------------------------------------------
static void
UpdatePC ()
{
    int pc = machine->ReadRegister (PCReg);
    machine->WriteRegister (PrevPCReg, pc);
    pc = machine->ReadRegister (NextPCReg);
    machine->WriteRegister (PCReg, pc);
    pc += 4;
    machine->WriteRegister (NextPCReg, pc);
}


//----------------------------------------------------------------------
// ExceptionHandler
//      Entry point into the Nachos kernel.  Called when a user program
//      is executing, and either does a syscall, or generates an addressing
//      or arithmetic exception.
//
//      For system calls, the following is the calling convention:
//
//      system call code -- r2
//              arg1 -- r4
//              arg2 -- r5
//              arg3 -- r6
//              arg4 -- r7
//
//      The result of the system call, if any, must be put back into r2. 
//
// And don't forget to increment the pc before returning. (Or else you'll
// loop making the same system call forever!
//
//      "which" is the kind of exception.  The list of possible exceptions 
//      are in machine.h.
//----------------------------------------------------------------------

void
ExceptionHandler (ExceptionType which)
{
    int type = machine->ReadRegister (2);

		if (which == SyscallException){
			switch(type){
				case SC_Halt: {
					DEBUG('a', "Shutdown, initated by user program.\n");
					interrupt->Halt();
					break;
				}

				case SC_PutChar: {
					DEBUG('a', "PutChar, called by user.\n");
					synchconsole->SynchPutChar((char) machine->ReadRegister(4));
					break;
				}

				case SC_GetChar: {
					DEBUG('a', "GetChar, called by user.\n");
					machine->WriteRegister(2, synchconsole->SynchGetChar());
					break;
				}

				case SC_PutString: {
					DEBUG('a', "PutString, called by user.\n");
					char buffer[MAX_STRING_SIZE];	
					synchconsole->copyStringFromMachine(machine->ReadRegister(4), buffer, MAX_STRING_SIZE);
					synchconsole->SynchPutString(buffer);
					break;
				}

				case SC_GetString: {
					DEBUG('a', "GetString, called by user.\n");
					char buffer[MAX_STRING_SIZE];
					int adr = machine->ReadRegister(4);
					int taille = machine->ReadRegister(5);
					if(taille > MAX_STRING_SIZE)
						taille = MAX_STRING_SIZE;

					synchconsole->SynchGetString(buffer, taille);
					synchconsole->copyStringToMachine(adr, buffer, taille);
					break;
				}

				case SC_PutInt: {
					DEBUG('a', "PutInt(), called by user.\n");
					int n = machine->ReadRegister(4);
					synchconsole->SynchPutInt(n);
					break;
				}

				case SC_GetInt: {
					DEBUG('a', "GetInt(), called by user.\n");
					int n = machine->ReadRegister(4);
					synchconsole->SynchGetInt(&n);
					break;
				}

				case SC_Exit: {
					DEBUG('a', "Exit, called by user.\n");
					interrupt->Exit(machine->ReadRegister(4));
					break;
				}

				default: {
					printf("Unexpected user mode exception %d %d\n", which, type);
				}
			}
		}
    else // Exception raised isn't a SyscallException.
		{
			printf ("Unexpected user mode exception %d %d\n", which, type);
			ASSERT (FALSE);
    }

    // LB: Do not forget to increment the pc before returning!
    UpdatePC ();
    // End of addition
}
