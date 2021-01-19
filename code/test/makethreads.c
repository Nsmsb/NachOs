/* makethreads.c
 *	User test file. Create a thread that displays the ten first letters
 *	of the alphabet. The Main thread waits for an input.
 */

#include "syscall.h"

void print(void *val){
	char arg = *((char *) val);
	for(int i = 0 ; i < 10 ; i++){
		PutChar(arg);
		arg++;
	}
	UserThreadExit();
}


int main(){
	char val = 'a';
	UserThreadCreate(print, ((void *) &val),(int)UserThreadExit);
	GetChar();

	return 0;
}
