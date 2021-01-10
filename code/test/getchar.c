/* getchar.c
 *	Test program for basic I/O system calls.
 *
 *	Echoes each character written, sourrounded by <>.
 *	Reads 10 characters.
 *	
 */



#include "syscall.h"

void read()
{
	int c;
	while((c = GetChar() ) > 0){
		PutChar('<');
		PutChar((char) c);
		PutChar('>');
	}
	PutChar('\n');
}

int
main()
{
	read();
	//Halt();
	return 0;
}
