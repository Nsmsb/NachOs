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

SynchConsole::SynchConsole(char *readFile, char *writeFile)
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
	for(int i=0 ; i < MAX_STRING_SIZE && s[i] != '\0' ; i++){
		SynchPutChar(s[i]);
	}
}

void SynchConsole::SynchGetString(char *s, int size)
{
	int i = 0;
	char c;

	// This feels *very* convoluted.
	// We read "At most one less than size charaters" (see fgets specs')
	if(size > 1){
		do{
			c = SynchGetChar();
			s[i] = c;
			i++;
		}while((i < size-1) && (c >= 0) && (c != '\n'));
	}

	// Adding the terminating byte after the last character.
	s[i] = '\0';
}

/* copyStringFromMachine(int from, char *to, unsigned size)
 *	Copies a string at MIPS adress "from"
 *	into a kernel pointer string "to".
 *	Up to size character are copied. The string is terminated
 *	by a '\0' even if none were copied.
 */
void SynchConsole::copyStringFromMachine(int from, char *to, unsigned size)
{
	unsigned i = 0;
	int val = 0;
	// We stop at i = size-1 so we can put the '\0' character.
	for(i = 0 ; i < size-1 ; i++){
		machine->ReadMem(from, 1, &val);
		to[i] = (char) val;
		if(val == '\0')
			return;
		from++;
	}
	to[i] = '\0';
}

/* copyStringToMachine
 *	Copies a kernel string "from" to a user string
 *	at MIPS adress "to".
 *	size characters are copied
 *
 *	IMPORTANT : We assume the String comes from SynchGetString, so
 *	there is a '\0' and it's after the first \n, or somewhere if there
 *	are none.
 */
// Note : this function could be simplified, assuming the string comes from SynchGetChar.
void SynchConsole::copyStringToMachine(int to, char *from, unsigned size)
{
	unsigned i = 0;
	// We stop at size-1, since the last char is \0 ; and we don't want to overflow
	while(i < size-1 && from[i] > 0){
		machine->WriteMem(to, 1, from[i]);
		to++;
		i++;
	}
	machine->WriteMem(to, 1, from[i]);
}

void SynchConsole::SynchPutInt(int n)
{
	char* n_string =  new char(MAX_STRING_SIZE);
	snprintf(n_string,MAX_STRING_SIZE,"%d",n);
	synchconsole->SynchPutString(n_string);
	delete n_string
}

void SynchConsole::SynchGetInt(int n)
{

}