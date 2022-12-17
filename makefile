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




all: $(TARGET).bin

#startup.o: startup.s
#	$(AS) -o startup.o startup.s 

%.o : %.c
	$(CC) $(CFLAGS) -Wall  -c $< -o $@


$(TARGET).elf: linker.ld $(OBJ) 
	$(LD) -T linker.ld -o $(TARGET).elf $(OBJ) -Map=firmware.map

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
