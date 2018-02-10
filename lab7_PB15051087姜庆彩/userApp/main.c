#include "../myOS/myOS.h"
#include "userApp.h"
//=========================my Main===========================================
#define VGA_BASE 0xb8000
unsigned short VGA_rows = 25;
unsigned short VGA_cols = 80;

void clear_screen(void) {
	int row, col;
	unsigned short *ptr = (unsigned short *)VGA_BASE;
	for(row=0; row< VGA_rows; row++) {
	    for (col=0; col< VGA_cols; col++) {
		(*ptr++) = 0;
	    }
	}
}

void put_char(char c, char color, int row, int col) {
	unsigned char *ptr = (unsigned char *)VGA_BASE;
	unsigned short value;

	ptr[(row*80+col)*2] = c;
	ptr[(row*80+col)*2+1] = color;	
}

void put_chars(char *msg, char color, int *_row, int *_col){
	char *ptr=msg;
	char c;
	int row, col;
	
	row = *_row;	col = *_col;
	c = *ptr;
	while (c!='\0'){
	    if ( col==80 ) {	col = 0;	row ++;	}
	    if ( row==25 ) row = 0;

	    put_char(c, color, row, col++);	    
	    c = *(++ptr);
	}
	
}


int myMain(void){

	clear_screen();

	myprintf(0x7,"myMain:HELLO WORLD!,%d\n",10);
	osStart();

	//never here    	
	return 0;
}


