CC = arm-none-eabi-gcc
AS = arm-none-eabi-as
LD = arm-none-eabi-ld
BIN = arm-none-eabi-objcopy
STL = st-flash
CFLAGS = -mthumb -mcpu=cortex-m3

all: app.bin

startup.o: startup.s
	$(AS) -o startup.o startup.s

main.o: main.c
	$(CC) $(CFLAGS) -c -o main.o main.c

app.elf: linker.ld startup.o main.o
	$(LD) -T linker.ld -o app.elf startup.o main.o

app.bin: app.elf
	$(BIN) -O binary app.elf app.bin

clean:
	rm -f *.o *.elf *.bin

flash: app.bin
	$(STL) write app.bin 0x8000000

erase:
	$(STL) erase
