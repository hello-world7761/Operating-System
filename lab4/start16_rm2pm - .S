.code16 #16位代码
.section ".bstext","a"
.globl bootsect_start
bootsect_start:
    cli                                 #关中断
    movw %cs,%ax
	movw %ax,%ds
	movw %ax,%es
	movw %ax,%ss                        #初始化基本段寄存器
	cld                                 #清除方向标志
    movw	$0xB800, %ax					
	movw	%ax, %es
	xorw	%di, %di					
	movw	$0000, %ax                  #the zero
	movw	$4000, %cx		            #4000为屏幕尺寸
	rep	stosb
    movw $0xB800,%bx                    
	movw %bx,%es                        #VGA的首地址
	movl $160,%edx                      #偏移量，第四行开始
	movw $msg,%si                      
	movb $0x7,%ah     	#白色
	
print:                              
    lodsb                                   # LODSB的功能是将DS:[si]中的内容取到AL中，然后SI<=SI+/-1
	andb %al,%al                            #用来判断是否加载到字符串末尾
	jz  print_end                
	movw %ax,%es:0(,%edx,2)             #把字符写入VGA
	incl %edx                           #偏移量自增运算
	jmp print                           #循环输出
	
print_end:
    movl $320,%edx
	movw $msg0,%si
	
print2:
    lodsb                              
	andb %al,%al                        
	jz  print2_end               
	movw %ax,%es:0(,%edx,2)           
	incl %edx                          
	jmp print2 
	
print2_end:

    xorw %ax,%ax
	int $0x16
	xorl %eax,%eax                      #eax清零
	movw %cs,%ax                        #段基地址存入ax
	sall $4,%eax
	movl %eax,%ebx
	addl $gdt,%eax
	movl %eax,lgdt_opcode+2
	lgdt lgdt_opcode
	addl $prot_mode,%ebx
	movw %bx,gdt+10
	shrl $16,%ebx
	movb %bl,gdt+12
	movb %bh,gdt+15
	cli
	inb $0x92,%al
	orb $0x02,%al
	outb %al,$0x92                      #打开A20地址线
	movl %cr0,%eax
	orl $1,%eax
	movl %eax,%cr0         #保护模式            
	ljmp $8,$0
	
.code32  #32位代码

prot_mode:	
    movw $16,%ax
	movw %ax,%es
	movl $202,%edi
	movw $msg1,%si
	movb $0x7,%ah
	
print3:
	lodsb                               
	andb %al,%al                        
	jz  print3_end                    
	movw %ax,%es:0(,%edx,2)             
	incl %edx                           
	jmp print3                          
	                                    
print3_end:
    movw $msg2,%si
	movl $480,%edx
	
print4:

    lodsb                               
	andb %al,%al                        
	jz  loop               #跳到死循环
	movw %ax,%es:0(,%edx,2)             
	incl %edx                           
	jmp print4                   
	                                    
    
loop:                                      
    jmp loop                            #死循环
.section ".bsdata","a"

gdt:                                         #GDT
  .short 0,0,0,0#空白

  .short 0x01ff
  .short 0x0000
  .short 0x9a00
  .short 0x00c0

  .short 0x0001
  .short 0x8000
  .short 0x920b
  .short 0x00c0
gdt_end:

msg:

  .asciz "helloworld PB15051087"	

  msg0:


  .asciz "Press any key to get into protect mode..."
  
msg1:
  .asciz "O.K.!"
  
msg2:
  .asciz "We are now in PROTECT MODE!"

lgdt_opcode:
  .short gdt_end-gdt-1
  .long gdt
  
.section ".signature","a"

	.globl	sig
sig:	.word 0xAA55                        #最后2字节

