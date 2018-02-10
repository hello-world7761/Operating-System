#ifndef __MYOS_H__
#define __MYOS_H__

#include "../userApp/userApp.h"

#define TASK_NUM (2 + USER_TASK_NUM)   // at least: 0-idle, 1-init

#define STACK_SIZE	512  // 1024*4

typedef struct myTCB {
	unsigned long state;  // 0:rdy
	int tcbIndex;
	struct myTCB * next;
	unsigned long* stkTop;
	unsigned long stack[STACK_SIZE];  
} myTCB;

myTCB tcbPool[TASK_NUM];

myTCB * idleTsk;
myTCB * currentTsk;
myTCB * firstFreeTsk;

//========ready queue FCFS==================================
#define TSK_RDY 0
typedef struct rdyQueueFCFS{
	myTCB * head;
	myTCB * tail;
	myTCB * idleTsk;
} rdyQueueFCFS;

rdyQueueFCFS rqFCFS;

int rqFCFSIsEmpty(void);
void rqFCFSInit(myTCB *idleTsk);
myTCB * nextFCFSTsk(void);

//========context switch====================================
unsigned long *prevTSK_StackPtr;
unsigned long *nextTSK_StackPtr;
void CTX_SW(void*prev_stkTop, void*next_stkTop);

//=======task manager======================================
void schedule(void);
void destroyTsk(int takIndex);

//=======OS API to user ============================================
void osStart(void);
int createTsk(void (*tskBody)(void));
void tskEnd(void);

//=======APP, user should define this==================================
void initTskBody(void);

//=======memory manager======================================
unsigned long dPartitionInit(unsigned long start, unsigned long size);//初始化并返回dPartition句柄
unsigned long dPartitionAlloc(unsigned long dp, unsigned long size);  //0失败；其他：所分配到的内存块起始地址
unsigned long dPartitionFree( unsigned long dp, unsigned long start); //0失败；1成功

unsigned long eFPartitionInit(unsigned long start, unsigned long perSize, unsigned long n); //初始化并返回eFPartition句柄
unsigned long eFPartitionTotalSize(unsigned long perSize, unsigned long n); //根据单位大小和单位个数，计算出eFPartition所需内存大小
unsigned long eFPartitionAlloc(unsigned long EFPHandler); //0失败；其他：所分配到的内存块起始地址
unsigned long eFPartitionFree(unsigned long EFPHandler,unsigned long mbStart); //0失败；1成功

void pMemStart(void); //内存检测并采用dPartition初始化malloc机制

unsigned long pMemHandler; //动态内存的dPartition句柄
#define malloc(size)  dPartitionAlloc(pMemHandler,size)
#define free(start)		dPartitionFree(pMemHandler,start)

//========output===============================================
void myprintf(int color,const char *format, ...);


#endif

