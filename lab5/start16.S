.code16
.section ".bstext","a"
.globl bootsect_start
bootsect_start:
    cli                                 
    movw %cs,%ax
	movw %ax,%ds
	movw %ax,%es
	movw %ax,%ss                        
	cld                                
    movw	$0xB800, %ax					
	movw	%ax, %es
	xorw	%di, %di					
	movw	$0000, %ax                 
	movw	$4000, %cx		         
	rep	stosb                           
    movw $0xB800,%bx                    
	movw %bx,%es                        
	movl $160,%edx                     
	movw $msg,%si                      
	movb $0x7,%ah                       #白色
print:                                
    lodsb                           
	andb %al,%al                       
	jz  print_end                            
	movw %ax,%es:0(,%edx,2)            
	incl %edx                         
	jmp print                           
	                                   
print_end:
    movl $320,%edx
	movw $msg0,%si
print_second:
    lodsb                               
	andb %al,%al                        
	jz  print_second_end               
	movw %ax,%es:0(,%edx,2)             #写VGA
	incl %edx                          
	jmp print_second                  
print_second_end:
    xorw %ax,%ax
	int $0x16                           #等待输入   
    lgdt lgdt_opcode              
	inb $0x92,%al
	orb $0x02,%al
	outb %al,$0x92                      #打开A20端口
	#reset disk drive
	movb $0,%ah
	movb $0,%dl
	int $0x13
	#get  the buffer address pointer
	movw $0,%ax
	movw %ax,%es
	movw $0x7e00,%bx
	#load floopy
	movb	$2, %ah
	movb	$7, %al
	movb	$0, %dh
	movb	$0, %dl
	movb	$0, %ch
	movb	$2, %cl
	int		$0x13
	#set the PE bit
	movl %cr0,%eax
	orl $1,%eax
	movl %eax,%cr0                      
	ljmp $8,$0x7e00                  #至保护模式
	
.section ".bsdata","a"
msg:
  .asciz "helloworld PB15051087"	
msg0:
  .asciz "Press any key to get into protect mode..."
msg1:
  .asciz "O.K.!"
msg2:
  .asciz "We are now in PROTECT MODE!"
gdt:                                        
  .short 0,0,0,0                             #空白
  #global code section
  .long 0x0000ffff
  .long 0x00c09a00
  #global VGA section
  .long 0x80000001
  .long 0x00c0920b
gdt_end:

lgdt_opcode:
  .short gdt_end-gdt-1
  .long gdt
.section ".signature","a"
	.globl	sig
sig:	.word 0xAA55                    
    
