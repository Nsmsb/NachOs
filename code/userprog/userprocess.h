
#ifndef USERPROCESS_H
#define USERPROCESS_H

// userprocess.h
//
//	Functions to create and handle user processus.
//

extern int do_ForkExec(char *filename);

extern void do_UserProcessJoin(int pid);

#endif
