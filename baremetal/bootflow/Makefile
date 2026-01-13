CC = arm-none-eabi-gcc

CFLAGS = -mcpu=cortex-m3 \
         -mthumb \
         -nostdlib \
         -O0 \
         -g

LDFLAGS = -T linker.ld

TARGET = firmware.elf
SRCS = startup.c main.c

all: $(TARGET)

$(TARGET): $(SRCS)
	$(CC) $(CFLAGS) $(LDFLAGS) $(SRCS) -o $(TARGET)

clean:
	rm -f $(TARGET)
