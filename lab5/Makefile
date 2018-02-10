os : myOS.elf start16.elf
	objcopy -O binary myOS.elf myOS.bin  
	objcopy -O binary start16.elf start16.bin
start16.o : start16.S
	gcc -c  --pipe -Wall -fasm -g -O1  start16.S -o start16.o -m32

start32.o : start16.S
	gcc -c  --pipe -Wall -fasm -g -O1  start32.S -o start32.o -m32

main.o: main.c
	gcc -c main.c -o main.o -m32

start16.elf : start16.ld start16.o 
	ld -T start16.ld start16.o -o start16.elf


myOS.elf : myOS.ld start32.o main.o
	ld -T myOS.ld start32.o main.o -o myOS.elf

clean : 
	rm *.bin *.o *.elf
	

