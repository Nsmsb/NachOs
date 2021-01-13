/* putint.c
 *	Test program for basic I/O system calls.
 *
 *	Echoes numbers from 0..5.
 *	
 */



#include "syscall.h"

void putint()
{
	for (int i = 0; i < 5; i++)
	{
		PutInt(i);
		PutChar(' ');
	}
	PutChar('\n');
}

int
main()
{
	putint();
	//Halt();
	return 0;
}
