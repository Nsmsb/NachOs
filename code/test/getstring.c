/* getstring.c
 *	Test program for basic I/O system calls.
 *
 *	Expected results : Echoes the ten first characters
 *	from the inputed string surrounded by <>
 */



#include "syscall.h"

int
main()
{
	char str[10];
	GetString(str, 10);
	PutChar('<');
	PutString(str);
	PutChar('>');
	PutChar('\n');

	//Halt();
	return 0;
}
