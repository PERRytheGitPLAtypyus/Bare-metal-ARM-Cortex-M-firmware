#include <stdint.h>

/* Symbols from linker */
extern uint32_t _estack;
extern int main(void);

/* Handlers */
void Reset_Handler(void);
void Default_Handler(void);
void PendSV_Handler(void);
void SysTick_Handler(void);

/* Vector table */
__attribute__((section(".isr_vector")))
void (*const vector_table[])(void) = {
    (void (*)(void))(&_estack), // 0  Initial stack
    Reset_Handler,              // 1  Reset
    Default_Handler,            // 2  NMI
    Default_Handler,            // 3  HardFault
    Default_Handler,            // 4  MemManage
    Default_Handler,            // 5  BusFault
    Default_Handler,            // 6  UsageFault
    0, 0, 0, 0,                 // 7–10 Reserved
    Default_Handler,            // 11 SVCall
    Default_Handler,            // 12 DebugMonitor
    0,                          // 13 Reserved
    PendSV_Handler,             // 14 PendSV  ✅
    SysTick_Handler             // 15 SysTick ✅
};

void Reset_Handler(void)
{
    main();
    while (1) {}
}

void Default_Handler(void)
{
    while (1) {}
}
