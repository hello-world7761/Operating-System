#include "myOS.h"
#define dead -1
void schedule()
{
	if(rqFCFS.head==(void *)0)
		nextTSK_StackPtr=rqFCFS.idle->stktop;
	else
	{
	prevTSK_StackPtr=currentTsk->stktop;
	nextTSK_StackPtr=rqFCFS.head->stktop;
	}
	CTX_SW(prevTSK_StackPtr,nextTSK_StackPtr);
}
void tskEnqueneFCFS(myTCB *tsk)
{
	rqFCFS.tail->next=tsk;
	rqFCFS.tail=tsk;
}
void tskDequeueFCFS(myTCB *tsk)
{
	tsk=rqFCFS.head;
	if(rqFCFS.head!=(void *)0)
        rqFCFS.head=rqFCFS.head->next;
	tsk->next=(void *)0;
	schedule();
}
void tskStart(myTCB *tsk)
{
	tsk->state =TSK_RDY;
	tskEnqueneFCFS(tsk);
}
void destroyTsk(int index)
{
	if(index>=0&&index<STACK_SIZE)
	tcbpool[index].state=dead;
}
void tskEnd()
{
	tskDequeueFCFS(currentTsk);
	destroyTsk(currentTsk->tcbIndex);
}
void stack_init(unsigned long **stk, void (*task)(void))
{
	*(*stk)-- = (unsigned long) 0x80;   //cs 高地址
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
int createTsk(void (*tskBody) (void))
{
	myTCB *allocated=firstFreeTsk;
	if(firstFreeTsk==(void *)0) return -1;
	firstFreeTsk=allocated->next;
	allocated->next =(void *)0;
	stack_init(&(allocated->stktop),tskBody);
	tskStart(allocated);
	return allocated->tcbIndex;
}
void idle()
{
	int row,col;
	char *message1 = "****************************\0";
	char *message2 = "*     idle...idle...       *\0";
	row=20; col=4; put_chars(message1,WHITE,&row,&col);
	row++; col=4; put_chars(message2,WHITE,&row,&col);
	row++; col=4; put_chars(message1,WHITE,&row,&col);	
	while(1);
}
void rqFCFSInit(myTCB *idleTsk)
{
	myTCB virtual_TCB;
	rqFCFS.idle=idleTsk;
	rqFCFS.head=rqFCFS.tail=&virtual_TCB;
	rqFCFS.head->next=(void *)0;
}
void osStart()
{
    int i;
    for(i=0;i<TASK_NUM;i++)
	{
		tcbpool[i].tcbIndex=i;
		tcbpool[i].stktop=tcbpool[i].stack+STACK_SIZE-1;
		if(i!=0&&i!=STACK_SIZE-1) tcbpool[i].next=tcbpool+i+1;
		else tcbpool[i].next=(void *)0;
	}
	firstFreeTsk=tcbpool;
	idleTsk=tcbpool;
	rqFCFSInit(idleTsk);
	stack_init(&(firstFreeTsk->stktop),idle);
	firstFreeTsk=tcbpool+2;
	initTskBody();
}

















