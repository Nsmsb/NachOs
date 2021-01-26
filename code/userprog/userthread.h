

#ifndef USERTHREAD_H
#define USERTHREAD_H

// userthread.h
//
//	Functions to create and handle user threads.
//

void do_UserThreadExit();

void do_UserThreadJoin(int tid);

int cherchepospile(int tid);

extern int do_UserThreadCreate(int f, int arg);

int do_open(char *name);


#endif
