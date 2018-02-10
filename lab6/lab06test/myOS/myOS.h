#ifndef __MYOS_H__
#define __MYOS_H__

#include "../userApp/userApp.h"

#define TASK_NUM (2 + USER_TASK_NUM)   // at least: 0-idle, 1-init

#define STACK_SIZE  512

typedef struct myTCB
{
	unsigned long state;
	int tcbIndex;
	struct myTCB *next;
	unsigned long *stktop;
	unsigned long stack[STACK_SIZE];
}myTCB;
myTCB tcbpool[TASK_NUM];
myTCB *idleTsk;
myTCB *currentTsk;
myTCB *firstFreeTsk;

//========ready queue===============================================
#define TSK_RDY 0
typedef struct rdyQueueFCFS{
	myTCB *head;
	myTCB *tail;
	myTCB *idle;
}rdyQueueFCFS;
rdyQueueFCFS rqFCFS;
void rqFCFSInit(myTCB *idleTsk);
myTCB *nextFCFSTsk();
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

