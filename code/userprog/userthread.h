

#ifndef USERTHREAD_H
#define USERTHREAD_H

// userthread.h
//
//	Functions to create and handle user threads.
//

void do_UserThreadExit();

void UserThreadJoin(int tid);

extern int do_UserThreadCreate(int f, int arg);


#endif
