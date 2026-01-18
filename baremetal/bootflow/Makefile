TARGET = firmware.elf

CC = arm-none-eabi-gcc
OBJDUMP = arm-none-eabi-objdump

CFLAGS = -mcpu=cortex-m3 -mthumb -O0 -g -Wall -ffreestanding -nostdlib
LDFLAGS = -T linker/linker.ld

SRCS = \
	startup/startup.c \
	startup/pendsv.S \
	kernel/main.c

all: $(TARGET)

$(TARGET): $(SRCS)
	$(CC) $(CFLAGS) $(SRCS) $(LDFLAGS) -o $(TARGET)

clean:
	rm -f $(TARGET)

run:
	qemu-system-arm -M lm3s6965evb -nographic -kernel $(TARGET)

debug:
	qemu-system-arm -M lm3s6965evb -nographic -kernel $(TARGET) -S -gdb tcp::1234
