/* putchar.c
 *	Test program for basic I/O system calls.
 *
 *	Expected results : displays "abcd\n", then stops.
 */



#include "syscall.h"

void print(char c, int n)
{
	int i;
	for (i=0 ; i<n ; i++){
		PutChar(c+i);
	}
	PutChar('\n');
}

int
main()
{
	print('a', 4);
	Halt();
}
