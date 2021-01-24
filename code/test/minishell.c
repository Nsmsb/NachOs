#include "syscall.h"

int
main ()
{
    
    char prompt[3], buffer[60];
    int i,newProc;

    prompt[0] = '-';
    prompt[1] = '-';
    prompt[2] = '\0';

    while (1)
      {
	  PutString(prompt);

	  i = 0;

	  do
	    {

		buffer[i]=GetChar();

	    }
	  while (buffer[i++] != '\n');

	  buffer[--i] = '\0';

	  if (i > 0)
	    {
		newProc=ForkExec(buffer);
		if(newProc!=-1)
			UserProcessJoin(newProc);
	    }
      }
}
