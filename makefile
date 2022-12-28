TARGET = firmware

CC = arm-none-eabi-gcc
AS = arm-none-eabi-as
LD = arm-none-eabi-gcc 
BIN = arm-none-eabi-objcopy
STL = st-flash

# define for CMSIS
DEFINES = STM32F103xB 
CFLAGS = -mthumb -mcpu=cortex-m3 -mfloat-abi=soft -D $(DEFINES) 
LDFLAGS = -mthumb -mcpu=cortex-m3 -mfloat-abi=soft --specs=nano.specs -T linker.ld -Wl,-Map=firmware.map 

# get list .c files in working dir
SRC = $(wildcard *.c)  

# var store renamed list .c -> .o from $(SRC)
OBJ = $(patsubst %.c, %.o, $(SRC))


all: $(TARGET).bin

%.o : %.c
	$(CC) $(CFLAGS) -Wall  -c $< -o $@

$(TARGET).elf: linker.ld $(OBJ) 
	$(LD) $(LDFLAGS) -o $(TARGET).elf $(OBJ) 

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
