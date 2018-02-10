all: output/myOS.bin output/start16.bin clean

.PHONY: all

output/start16.bin : start16.S
	gcc -c --pipe -Wall -fasm -g -O1  start16.S -o output/start16.o	
	ld -T start16.ld output/start16.o -o output/start16.elf
	objcopy -O binary output/start16.elf output/start16.bin

output/myOS.bin : output/myOS.elf
	objcopy -O binary output/myOS.elf output/myOS.bin

output/myOS.elf : myOS.ld output/start32.o userApp/main.o userApp/init.o myOS/memory.o myOS/printf.o myOS/task.o 
	ld -T myOS.ld output/start32.o userApp/main.o userApp/init.o myOS/memory.o myOS/printf.o myOS/task.o -o output/myOS.elf

output/start32.o : myOS/start32.S
	gcc -c --pipe -Wall -fasm -g -O1  myOS/start32.S -o output/start32.o

myOS/memory.o : myOS/memory.c
	gcc -fno-stack-protector -c myOS/memory.c -o myOS/memory.o
	
myOS/printf.o : myOS/printf.c
	gcc -fno-stack-protector -c myOS/printf.c -o myOS/printf.o
	
myOS/task.o : myOS/task.c
	gcc -fno-stack-protector -c myOS/task.c -o myOS/task.o

userApp/main.o : userApp/main.c
	gcc -fno-stack-protector -c userApp/main.c -o userApp/main.o
	
userApp/init.o : userApp/init.c
	gcc -fno-stack-protector -c userApp/init.c -o userApp/init.o

clean :
	rm userApp/main.o userApp/init.o myOS/memory.o myOS/printf.o myOS/task.o output/myOS.elf output/start32.o output/start16.o output/start16.elf

