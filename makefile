TARGET = firmware

CC = arm-none-eabi-gcc
AS = arm-none-eabi-as
LD = arm-none-eabi-ld
BIN = arm-none-eabi-objcopy
STL = st-flash

# define for CMSIS
DEFINES = STM32F103xB 
CFLAGS = -mthumb -mcpu=cortex-m3 -D $(DEFINES)


# get list .c files in working dir
SRC = $(wildcard *.c)  

# var store renamed list .c -> .o from $(SRC)
OBJ = $(patsubst %.c, %.o, $(SRC))

#adding startup.s/.o
#OBJ += startup.o


all: $(TARGET).bin

startup.o: startup.s
	$(AS) -o startup.o startup.s

%.o : %.c
	$(CC) $(CFLAGS)  -c $< -o $@

#main.o: main.c
#	$(CC) $(CFLAGS) -c  SRC)




$(TARGET).elf: linker.ld $(OBJ) startup.o
	$(LD) -T linker.ld -o $(TARGET).elf $(OBJ) startup.o

$(TARGET).bin: $(TARGET).elf
	$(BIN) -O binary $(TARGET).elf $(TARGET).bin

clean:
	rm -f *.o *.elf *.bin

rebuild: clean
	make all

flash: $(TARGET).bin
	$(STL) write $(TARGET).bin 0x8000000

erase:
	$(STL) erase
