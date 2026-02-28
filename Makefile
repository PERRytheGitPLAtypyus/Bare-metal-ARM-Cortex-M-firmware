TARGET = firmware.elf
CC     = arm-none-eabi-gcc
AS     = arm-none-eabi-gcc
CFLAGS = -mcpu=cortex-m3 -mthumb -O0 -g -Wall -ffreestanding -nostdlib
LDFLAGS = -T linker/lm3s6965.ld -nostdlib

SRCS = startup/startup.c \
       kernel/scheduler.c \
       kernel/systick.c \
       hal/uart.c \
       tasks/task_led.c \
       tasks/task_sensor.c \
       tasks/task_watchdog.c \


ASM_SRCS = kernel/context_switch.S

OBJS = $(SRCS:.c=.o) $(ASM_SRCS:.S=.o)

all: $(TARGET)
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) $(LDFLAGS) -o $(TARGET)
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@
%.o: %.S
	$(AS) $(CFLAGS) -c $< -o $@
clean:
	find . -name "*.o" -delete && rm -f $(TARGET)
run:
	qemu-system-arm -M lm3s6965evb -nographic -kernel $(TARGET)
debug:
	qemu-system-arm -M lm3s6965evb -nographic -kernel $(TARGET) -S -gdb tcp::1234
