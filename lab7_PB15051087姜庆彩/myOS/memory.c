#include"myOS.h"

unsigned long *dpartHead,*dpartTail;
unsigned long *efpartHead,*efpartTail;
unsigned long length,efSize;


void memStart(void)
{
	unsigned short *mem;
	int i=0;
	int row=0,col=0;
	char *msg1 = OK\n";
	mem[0] = 0x6666;
	mem = (unsigned short *)0x100000;

	while(mem[i] == 0x6666)
	{
		if((i+1)%0x100000==0)
		{
			if(row==25) row=0;
			myprintf(0x7,msg1);
			row++;
			}
			mem[i] = 0;
			i++;
			mem[i] = 0x6666;
	}
	myprintf(0x7,"Memory Size: %xBytes\n",i*2);
	memHandler = dPartitioninit_ef(0x100000,0x300000);
	return;
}

unsigned long dPartitioninit_ef(unsigned long start,unsigned long size)
{
	dpartHead = (unsigned long *)start;
	dpartTail = (unsigned long *)(start + size);
	return (unsigned long)dpartHead;
}


unsigned long dPartitionAlloc(unsigned long dp,unsigned long size)
{
	int j,k;
	unsigned long *loc;
	unsigned long i;

	if(size == 0) return(dp);


	if(size%4!=0) i=size/4+1;
	else i=size/4;
	i++;


	loc = (unsigned long *)dp;

	for(j=0;j<i;)
	{
		if((loc + i) > dpartTail)
		{
			myprintf(0x7,"   No Enough DPARTITION ROOM!\n");
			return 0;
		}
		else if(loc[j] == 0)
		{
			j++;
		}
		else
		{
			loc = loc + (j + loc[j]);
			j=0;
		}
	}

	loc[0] = i;

	return((unsigned long)(loc + 0x1));
}

unsigned long dPartitionFree( unsigned long dp, unsigned long start)
{
	unsigned long *loc;
	int count,i;

	loc = (unsigned long *)start;
	loc--;
	count = (int)loc[0];
	for(i=0;i<count;i++) loc[i] = 0;
	return 1;
}

unsigned long eFPartitionTotalSize(unsigned long perSize, unsigned long n)
{
	int k,j;
	unsigned long totalsize = 0;

	if(perSize == 0 || n == 0) return 0;

	if(perSize%4!=0) efSize=perSize/4+1;
	else efSize=perSize/4;
	efSize++;
	myprintf(0x7,"efBlock: %d\n",efSize);
	k = (int)n;
	for(j=0;j<k;j++) totalsize = totalsize+efSize;
	return(totalsize);
}

unsigned long eFPartitioninit_ef(unsigned long start, unsigned long perSize, unsigned long n)
{
	unsigned long *init_ef;
	length = eFPartitionTotalSize(perSize,n);

	efpartHead = (unsigned long *)start;
	efpartTail = (unsigned long *)(start + length*4);

	myprintf(0x7,"efpartHead: %x   efpartTail: %x\n",efpartHead,efpartTail);

	init_ef = efpartHead;

	for(;n>0;n--)
	{
		init_ef[0] = 0;
		init_ef  = init_ef + efSize;
	}

	return((unsigned long)efpartHead);
}
unsigned long eFPartitionAlloc(unsigned long EFPHandler)
{
	unsigned long *alloc_ef;
	alloc_ef = (unsigned long *)EFPHandler;
	while(alloc_ef[0])
	{
		alloc_ef = alloc_ef + efSize;
		if(alloc_ef >= efpartTail) return 0;
	}

	alloc_ef[0] = 1;

	return((unsigned long)(alloc_ef+1));
}

unsigned long eFPartitionFree(unsigned long EFPHandler,unsigned long mbStart)
{
	unsigned long *free_ef;

	free_ef = (unsigned long *)(mbStart - 4);
	free_ef[0] = 0;

	return 1;
}








