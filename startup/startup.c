#include <stdint.h>

/* ─── Linker symbols ─────────────────────────────────────── */
extern uint32_t _estack;
extern uint32_t _sidata;
extern uint32_t _sdata;
extern uint32_t _edata;
extern uint32_t _sbss;
extern uint32_t _ebss;

/* ─── External application entry ───────────────────────────── */
extern int main(void);

/* ─── External UART helpers (defined in hal/uart.c) ────────── */
extern void uart_puts(const char *s);
extern void uart_puthex(uint32_t val);

/* ─── Handler forward declarations ──────────────────────────── */
void Reset_Handler(void);
void Default_Handler(void);
void HardFault_Handler(void);
void PendSV_Handler(void);
void SysTick_Handler(void);

/* ─── Vector table ───────────────────────────────────────────
   Placed in .isr_vector — exactly 16 system exception slots.  */
__attribute__((section(".isr_vector"), used))
static const void (*vector_table[])(void) =
{
    (void (*)(void))(&_estack), /* 0  : initial stack pointer        */
    Reset_Handler,              /* 1  : Reset                        */
    Default_Handler,            /* 2  : NMI                          */
    HardFault_Handler,          /* 3  : HardFault                    */
    Default_Handler,            /* 4  : MemManage                    */
    Default_Handler,            /* 5  : BusFault                     */
    Default_Handler,            /* 6  : UsageFault                   */
    0,                          /* 7  : reserved                     */
    0,                          /* 8  : reserved                     */
    0,                          /* 9  : reserved                     */
    0,                          /* 10 : reserved                     */
    Default_Handler,            /* 11 : SVCall                       */
    Default_Handler,            /* 12 : DebugMonitor                 */
    0,                          /* 13 : reserved                     */
    PendSV_Handler,             /* 14 : PendSV                       */
    SysTick_Handler,            /* 15 : SysTick                      */
};

/* ─── Reset_Handler ─────────────────────────────────────────── */
void Reset_Handler(void)
{
    /* 1. Copy .data section from Flash to RAM */
    uint32_t *src = &_sidata;
    uint32_t *dst = &_sdata;
    while (dst < &_edata)
    {
        *dst++ = *src++;
    }

    /* 2. Zero .bss section */
    dst = &_sbss;
    while (dst < &_ebss)
    {
        *dst++ = 0;
    }

    /* 3. Call application main */
    main();

    /* 4. Infinite loop if main() returns */
    while (1);
}

/* ─── Default_Handler ───────────────────────────────────────── */
void Default_Handler(void)
{
    while (1);
}

/* ─── HardFault C handler (called from naked asm wrapper) ───── */
void HardFault_Handler_C(uint32_t *stack_frame)
{
    uart_puts("[HF] R0  = 0x"); uart_puthex(stack_frame[0]); uart_puts("\r\n");
    uart_puts("[HF] R1  = 0x"); uart_puthex(stack_frame[1]); uart_puts("\r\n");
    uart_puts("[HF] R2  = 0x"); uart_puthex(stack_frame[2]); uart_puts("\r\n");
    uart_puts("[HF] R3  = 0x"); uart_puthex(stack_frame[3]); uart_puts("\r\n");
    uart_puts("[HF] R12 = 0x"); uart_puthex(stack_frame[4]); uart_puts("\r\n");
    uart_puts("[HF] LR  = 0x"); uart_puthex(stack_frame[5]); uart_puts("\r\n");
    uart_puts("[HF] PC  = 0x"); uart_puthex(stack_frame[6]); uart_puts("\r\n");
    uart_puts("[HF] xPSR= 0x"); uart_puthex(stack_frame[7]); uart_puts("\r\n");
    while (1);
}

/* ─── HardFault_Handler (naked asm — MSP vs PSP detection) ──── */
__attribute__((naked))
void HardFault_Handler(void)
{
    __asm volatile (
        "tst    lr, #4          \n" /* test EXC_RETURN bit[2]         */
        "ite    eq              \n"
        "mrseq  r0, msp         \n" /* MSP was active — use MSP       */
        "mrsne  r0, psp         \n" /* PSP was active — use PSP       */
        "b      HardFault_Handler_C \n"
    );
}

/* ─── PendSV_Handler stub ────────────────────────────────────── */
__attribute__((weak))
void PendSV_Handler(void)
{
    while (1);
}

/* ─── SysTick_Handler stub ───────────────────────────────────── */
__attribute__((weak))
void SysTick_Handler(void)
{
    while (1);
}
