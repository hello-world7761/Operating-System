//这个文件有参考网上的代码
#include<stdarg.h>
int row=0,col=0;
void    printch（char c,int color)  
{  
    unsigned char *ptr = (unsigned char *)0xb8000;
	int i=0,j=0;
	if (c == 0xa)
	{
		i=row; j=col;
		
		for(;j<80;j++)
		{
			ptr[(i*80+j)*2] = 0;
			ptr[(i*80+j)*2+1] = 0;
		}
		col = 0;
		row++;

	}
	else
	{
		ptr[(row*80+col)*2] = c;
		ptr[(row*80+col)*2+1] = color;
	}
	
	if(c!=0xa) col++;
	
	if(col == 80)
	{
		col=0;
		row++;
	}	
	if(row == 25)
	{
		row = 24;
		roll_up();
	}
	
	return;
}  
void    printdec(int dec,int color)  
{  
    if(dec==0)  
    {  
        return;  
    }  
    printdec(dec/10,color);  
    printch( (char)(dec%10 + '0'),color);  
}  
  
void    printflt(double flt,int color)  
{  
    int icnt = 0;  
    int tmpint = 0;  
      
    tmpint = (int)flt;  
    printdec(tmpint,color);  
    printch('.',color);  
    flt = flt - tmpint;  
    tmpint = (int)(flt * 1000000);  
    printdec(tmpint,color);  
}  
  
void    printstr(char* str,int color)  
{  
    while(*str)  
    {  
        printch(*str++,color);  
    }  
}  
  
void    printbin(int bin,int color)  
{  
    if(bin == 0)  
    {  
        printstr("0b",color);  
        return;  
    }  
    printbin(bin/2,color);  
    printch( (char)(bin%2 + '0'),color);  
}  
  
void    printhex(int hex,int color)  
{  
    if(hex==0)  
    {  
        printstr("0x",color);  
        return;  
    }  
    printhex(hex/16,color);  
    if(hex%16 < 10)  
    {  
        printch((char)(hex%16 + '0'),color);  
    }  
    else  
    {  
        printch((char)(hex%16 - 10 + 'a' ),color);  
    }  
}  
void roll_up(void)
{
	unsigned char *p = (unsigned char *)0xb8000;
	int i=0,j=0;
	
	for(i=1;i<25;i++)
	{
		for(j=0;j<80;j++)
		{
			p[((i-1)*80+j)*2]=p[(i*80+j)*2];
			p[((i-1)*80+j)*2+1]=p[(i*80+j)*2+1];
		}
	}
	for(j=0;j<80;j++)
	{
		p[(i*80+j)*2] = 0;
		p[(i*80+j)*2+1] = 0;
	}	
}
void    myprintf(int color,const char* fmt, ...)  
{ 
    double vargflt = 0;  
    int  vargint = 0;  
    char* vargpch;  
    char vargch = 0;  
    const char* pfmt;  
    va_list vp;  
  
    va_start(vp, fmt);  
    pfmt = fmt;  
  
    while(*pfmt)  
    {  
        if(*pfmt == '%')  
        {  
            switch(*(++pfmt))  
            {  
                  
                case 'c':  
                    vargch = va_arg(vp, int);   
                    
                    printch(vargch,color);  
                    break;  
                case 'd':  
                case 'i':  
                    vargint = va_arg(vp, int);
                    if(vargint == 0) printch('0',color);  
                    else printdec(vargint,color);  
                    break;  
                case 'f':  
                    vargflt = va_arg(vp, double);  
                    printflt(vargflt,color);  
                    break;  
                case 's':  
                    vargpch = va_arg(vp, char*);  
                    printstr(vargpch,color);  
                    break;  
                case 'b':  
                case 'B':  
                    vargint = va_arg(vp, int);  
                    printbin(vargint,color);  
                    break;  
                case 'x':  
                case 'X':  
                    vargint = va_arg(vp, int); 
                    if(vargint == 0) printch('0',color);   
                    else printhex(vargint,color);  
                    break;  
                case '%':  
                    printch('%',color);  
                    break;  
                default:  
                    break;  
            }  
            pfmt++;  
        }  
        else  
        {  
            printch(*pfmt++,color);  
        }  
    }  
    va_end(vp);  
}  
  

