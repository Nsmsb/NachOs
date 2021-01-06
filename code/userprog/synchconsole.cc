// synchconsole.cc
//
//	Routines that simulate a synchronous I/O console.
//

#include "copyright.h"
#include "system.h"
#include "synchconsole.h"
#include "synch.h"

static Semaphore *readAvail;
static Semaphore *writeDone;

static void ReadAvail(int arg)
{
	readAvail->V();
}

static void WriteDone(int arg)
{
	writeDone->V();
}

SynchConsole::SynchConsole(char *readFile, chat *writeFile)
{
	readAvail = new Semaphore("read avail", 0);
	writeDone = new Semaphore("write done", 0);
	console = new Console (readFile, writeFile, ReadAvail, WriteDone, 0);
}

SynchConsole::~SynchConsole()
{
	delete console;
	delete writeDone;
	delete readAvail;
}

void SynchConsole::SynchPutChar(const char ch)
{
	console->PutChar(ch);
	writeDone->P();
}

char SynchConsole::SynchGetChar()
{
	readAvail->P();
	return console->GetChar();
}

void SynchConsole::SynchPutString(const char s[])
{
	// A completer.
}

void SynchConsole::SynchGetString(char *s, int n)
{
	// A completer.
}
