/* getint.c
 *	Test program for basic I/O system calls.
 *
 *	Echoes numbers from 0..5.
 *	
 */



#include "syscall.h"

void getint()
{
	int n;
	for (int i = 0; i < 5; i++)
	{
		GetInt(&n);
		PutChar('<');
		PutInt(n);
		PutChar('>');
		PutChar('\n');
	}
	PutChar('\n');
}

int
main()
{
	getint();
	//Halt();
	return 0;
}
