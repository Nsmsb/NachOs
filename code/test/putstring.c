/* putstring.c
 *	Test program for basic I/O system calls.
 *
 *	Expected results : displays "abcd\n", then stops.
 */



#include "syscall.h"

void print(char *c)
{
	PutString(c);
	PutChar('\n');
}

int
main()
{
	char chaine[20] = "abcdefghijklmnopqr";
	print(chaine);
	//Halt();
	return 0;
}
