/* getchar.c
 *	Test program for basic I/O system calls.
 *
 *	Echoes each character written, sourrounded by <>.
 *	Reads 10 characters.
 *	
 */



#include "syscall.h"

void read(int n)
{
	int i;
	char c;
	for(i = 0 ; i < n ; i++){
		c = (char) GetChar();
		PutChar('<');
		PutChar(c);
		PutChar('>');
	}
	PutChar('\n');
}

int
main()
{
	read(10);
	//Halt();
	return 0;
}
