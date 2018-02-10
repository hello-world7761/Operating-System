#ifndef __MYOS_H__
#define __MYOS_H__

#include "../userApp/userApp.h"

#define TASKNUM (2 + USER_TASKNUM)   // at least: 0-idle, 1-init

#define STACKSIZE  512

typedef struct myTCB
{
	unsigned long state;
	int tcbIndex;
	struct myTCB *next;
	unsigned long *stktop;
	unsigned long stack[STACKSIZE];
}myTCB;
myTCB TCB[TASKNUM];
myTCB *idleTsk;
myTCB *currentTsk;
myTCB *FREE;

//========ready queue===============================================
#define TSK_RDY 0
typedef struct readyQueue{
	myTCB *head;
	myTCB *tail;
	myTCB *idle;
}readyQueue;
readyQueue readyqueue;
void readyqueueInit(myTCB *idleTsk);
myTCB *nextTsk();
//=============context switch======================================
unsigned long *prevTSK_StackPtr;
unsigned long *nextTSK_StackPtr; 
void CTX_SW(void *prev_stkTop,void *next_stkTop);
//=============task manager========================================
void schedule();
//=======OS API to user ============================================
void osStart(void);
int createTsk(void (*tskBody)(void));
void tskEnd(void);

//=======APP, user should define this==================================
void initTskBody(void);

#endif

