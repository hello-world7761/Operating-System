#include "myOS.h"
void osStart()
{
    int i;
    for(i=0;i<TASKNUM;i++)
	{
		TCB[i].tcbIndex=i;
		if(i!=0&&i!=STACKSIZE-1) TCB[i].next=TCB+i+1;
		else TCB[i].next=(void *)0;
	}
	FREE=TCB;
	idleTsk=TCB;
	readyqueueInit(idleTsk);
	stack_init(&(FREE->stktop),idle);
	initTskBody();
}
int createTsk(void (*tskBody) (void))
{
	myTCB *allocated=FREE;
	if(FREE==(void *)0) return -1;
	FREE=allocated->next;
	stack_init(&(allocated->stktop),tskBody);
	tskStart(allocated);
	return allocated->tcbIndex;
}

void tskEnquene(myTCB *tsk)
{
	readyqueue.tail->next=tsk;
	readyqueue.tail=tsk;
}
void tskDequeue (myTCB *tsk)
{
	tsk=readyqueue.head;
	if(readyqueue.head!=(void *)0)      
	tsk->next=(void *)0;
	schedule();
}
void tskStart(myTCB *tsk)
{
	tsk->state =TSK_RDY;
	tskEnquene(tsk);
}
void destroyTsk(int index)
{
	if(index>=0&&index<STACKSIZE)
	TCB[index].state= -1;
}
void tskEnd()
{
	tskDequeue(currentTsk);
	destroyTsk(currentTsk->tcbIndex);
}
void stack_init(unsigned long **stk, void (*task)(void))
{
	
	*(*stk)-- = (unsigned long) 0x80;   //cs
	*(*stk)-- = (unsigned long) task;   //eip
	*(*stk)-- = (unsigned long) 0xAAAAAAAA;  //EAX
	*(*stk)-- = (unsigned long) 0xCCCCCCCC;  //ECX
	*(*stk)-- = (unsigned long) 0xDDDDDDDD;  //EDX
	*(*stk)-- = (unsigned long) 0xBBBBBBBB;  //EBX
	*(*stk)-- = (unsigned long) 0x44444444;  //ESP
	*(*stk)-- = (unsigned long) 0x55555555;  //EBP
	*(*stk)-- = (unsigned long) 0x66666666;  //ESI
	*(*stk)= (unsigned long) 0x77777777;     //EDI
}
void schedule()
{
	if(readyqueue.head==(void *)0)
		nextTSK_StackPtr=readyqueue.idle->stktop;
	else
	{
	prevTSK_StackPtr=currentTsk->stktop;
	nextTSK_StackPtr=readyqueue.head->stktop;
	}
	CTX_SW(prevTSK_StackPtr,nextTSK_StackPtr);
}

void idle()
{
	int row,col;
	char *message1 = "****************************\0";
	char *message2 = "*         idle...idle...       *\0";
	row=20; col=4; put_chars(message1,WHITE,&row,&col);
	row++; col=4; put_chars(message2,WHITE,&row,&col);
	row++; col=4; put_chars(message1,WHITE,&row,&col);	
	while(1);
}
void readyqueueInit(myTCB *idleTsk)
{
	myTCB virtual_TCB;
	readyqueue.idle=idleTsk;
	readyqueue.head=readyqueue.tail=&virtual_TCB;
	readyqueue.head->next=(void *)0;
}

















