STACK_SEG	= 0x4000

ProtectMode_CS	= 0x08
ProtectMode_DS	= 0x10

VGA_START	= 0xb8000
VGA_SEG		= 0xb800
SCREEN_SIZE	= 4000 		# 25*80*2

WHITE	= 0x7			# 白色，

OS_SEG		= 0x07E0
OS_START	= 0x7E00
OS_SECTS	= 0x7

/* ===== code16 for bootsector =========================== */
	.code16
	.section ".bstext", "ax"

	.global bootsect_start
bootsect_start:
	
	jmp	start16

dead16:	jmp dead16				

reset_floppy:
	xorw	%ax, %ax		  
	xorb	%dl, %dl
	int	$0x13
	ret

read_OS_sectors:	
	movw	$OS_SEG, %dx
	movw	%dx, %es		
	xorl	%edi, %edi
	xorl	%ebx, %ebx
	
	#	从第二个扇区开始读若干个扇区
	movb	$OS_SECTS, %al	# %al: 要读写的扇区数
	movw	$0x0000, %dx 	  # %dh: 磁头号; 
	                      # 驱动器号）		
	movw	$0x0002, %cx	
                  		  # 开始扇区
	movb	$0x02, %ah	    # 读扇区
	int	$0x13
	ret	

get_key:    #读取下一个字符
	movb $0x00, %ah
	int $0x16  
	ret


rm_print_string:
	movw	$VGA_SEG, %bx
	movw	%bx, %es
rm_print_string_loop:	
	lodsb
	andb	%al, %al
	jz	rm_print_string_end

	movw	%ax, %es:0(,%edx,2)
	incl	%edx
	jmp	rm_print_string_loop
rm_print_string_end:
	ret

	.p2align 4
start16:

	cli  # 关中断

	movw	%cs, %ax
	movw	%ax, %ds  # set the rest of real mode registers
	movw	%ax, %es

	movw	$STACK_SEG, %ax
	movw	%ax, %ss

	xorw	%sp, %sp
	cld

clear_screen16:	
	movw	$VGA_SEG, %ax	      # 使es指向显示内存	
	movw	%ax, %es
	xorw	%di, %di	          # 开始位置
	movw	$0000, %ax
	movw	$SCREEN_SIZE, %cx	  # 大小
	rep	stosb

print_hello1:
	movw	$hello_message, %si 
	movb	$WHITE, %ah
	movl	$(1*80), %edx
	call	rm_print_string

get_any_key1:
	movw	$get_key_message, %si
	movb	$WHITE, %ah
	movl	$(3*80), %edx
	call	rm_print_string

	call	get_key

#在进入保护模式前，先加载OS
load_img0:
	movw	$loading_OS_message, %si
	movb	$WHITE, %ah
	movl	$(5*80), %edx
	call	rm_print_string

# 加载OS
load_img:
	call reset_floppy

	call read_OS_sectors

load_ok:
	movw	$ok_message, %si
	movb	$WHITE, %ah
	movl	$(5*80+13), %edx
	call	rm_print_string


to_protect_mode:
	movw	$to_protect_mode_message, %si
	movb	$WHITE, %ah
	movl	$(7*80), %edx
	call	rm_print_string

load_gdt:
	lgdt	gdtptr	


	movw	$1, %ax
	lmsw	%ax      

	jmp	flush_instr	
flush_instr:

	movl	$ProtectMode_DS, %eax
	movw	%ax, %ds
	movw	%ax, %es
	movw	%ax, %fs
	movw	%ax, %gs

	movw	%ax, %ss
	movl	$0x4000, %ebp
	movl	$0x4000, %esp

	ljmp	$ProtectMode_CS, $start32

.code32


pm_print_string:
	movl	$VGA_START, %ebx	
pm_print_string_loop:	
	lodsb
	andb	%al, %al
	jz	pm_print_string_end
	movw	%ax, %es:0(%ebx,%edx,2)
	incl	%edx
	jmp	pm_print_string_loop
pm_print_string_end:
	ret
	
start32:
protect_mode_ok:
	movl	$ok_message_in_PM, %esi
	movb	$WHITE, %ah

	call	pm_print_string

	jmp	to_start32

dead:	jmp dead			

to_start32:
	movl	$to_start32_message, %esi
	movb	$WHITE, %ah
	movl	$(9*80), %edx 	# %edx is set above
	call	pm_print_string

go:	
	ljmp	$ProtectMode_CS, $OS_START


bad:	jmp	bad

/* ===== data for bootsect ============================= */
	.section ".bsdata", "a"
hello_message:
	.ascii	"Hello, OS world!"
	.byte	0

get_key_message:
	.ascii	"Press any key to get into protect mode..."
	.byte	0

loading_OS_message:
	.ascii	"Loading OS..."
	.byte	0

to_protect_mode_message:
	.ascii	"Switch to PROTECT MODE..."
	.byte	0

to_start32_message:
	.ascii	"GOING TO START32..."
	.byte	0

ok_message_in_PM:
	.ascii	"O.K.! We are now in PROTECT MODE!"
	.byte	0

ok_message:
	.ascii	"O.K.!"
	.byte	0
	
	.p2align 4
gdtptr:
	.word	0x18-1
	.long	gdt

# GDT
	.p2align 4
gdt:
	.word	0,0,0,0
	
	/* code segment */
	.word	0xffff, 0
	.byte	0, 0x9a, 0xcf, 0

        /* data segment */
	.word	0xffff, 0
	.byte	0, 0x92, 0xcf, 0

	.word	0,0,0,0
	
/* ===== signature ======================================= */	

	.section ".header", "a"
	.globl	hdr
hdr:
os_sectors:	.word 0x6
boot_flag:	.word 0xAA55

	# offset 512
