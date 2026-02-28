#include <stdint.h>

#define UART0_DR  (*((volatile uint32_t *)0x4000C000))
#define UART0_FR  (*((volatile uint32_t *)0x4000C018))

void uart_putc(char c)
{
    while (UART0_FR & (1 << 5));
    UART0_DR = c;
}

void uart_puts(const char *s)
{
    while (*s) uart_putc(*s++);
}

void uart_puthex(uint32_t val)
{
    const char hex[] = "0123456789ABCDEF";
    uart_putc('0');
    uart_putc('x');
    int i;
    for (i = 28; i >= 0; i -= 4)
    {
        uart_putc(hex[(val >> i) & 0xF]);
    }
}

void uart_putdec(uint32_t val)
{
    if (val == 0)
    {
        uart_putc('0');
        return;
    }
    char buf[10];
    int idx = 0;
    while (val > 0)
    {
        buf[idx++] = '0' + (val % 10);
        val /= 10;
    }
    /* reverse */
    int i;
    for (i = idx - 1; i >= 0; i--)
    {
        uart_putc(buf[i]);
    }
}
