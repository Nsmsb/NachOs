// synchconsole.h 
//
//	Data structures to simulate a synchronous I/O device above the
//	Console layer. A terminal has two parts -- a keyboard input,
//	and a display output, each of which produces/accepts 
//	characters sequentially.
//
// Copyright (c) 1992-1993 The Regents of the University of California.
// All rights reserved.  See copyright.h for copyright notice and limitation 
// of liability and disclaimer of warranty provisions.

#ifndef SYNCHCONSOLE_H
#define SYNCHCONSOLE_H

#include "copyright.h"
#include "utility.h"
#include "console.h"

class SynchConsole {
  public:
    SynchConsole(char *readFile, char *writeFile);
				// initialize the hardware console device
    ~SynchConsole();			// clean up console emulation

    void SynchPutChar(const char ch);	// Unix putchar(3S)
	int SynchGetChar();	   			// Unix getchar(3S)

	void SynchPutString(const char *s); 	// Unix puts(3S)
	void SynchGetString(char *s, int n); 	// Unix fgets(3S)

	void copyStringFromMachine(int from, char *to, unsigned size);
	void copyStringToMachine(int to, char *from, unsigned size);

	void SynchPutInt(int n);
	void SynchGetInt(int* n);
  
	private:
	Console *console;
};

#endif // SYNCHCONSOLE_H
