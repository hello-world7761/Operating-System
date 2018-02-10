.code32
.section .text
.global _start
_start:
	#initialize the pointer
	movl $__bss_start,%eax
    movl %eax,%esp
	movl %eax,%ebp
	#clear bss
	movl $__bss_start,%eax
loop:
	movl $__bss_end,%ebx
	movl $0,(%eax)
	addl $1,%eax
	subl %eax,%ebx
	jnz loop
    call main
	#dead loop
loop1:
    jmp loop1
.section .bss
.global bss
bss:
__bss_start:
    .space 4000
__bss_end:
    .word 0