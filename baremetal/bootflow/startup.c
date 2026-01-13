extern int main(void);

/* Linker symbols */
extern unsigned int _sidata;
extern unsigned int _sdata;
extern unsigned int _edata;
extern unsigned int _sbss;
extern unsigned int _ebss;

/* Global SysTick counter */
volatile unsigned int systick_count = 0;

/* Default handler */
void Default_Handler(void) {
    while (1);
}

/* SysTick handler */
void SysTick_Handler(void) {
    systick_count++;
}
void PendSV_Handler(void) {
    volatile int i;
    for (i = 0; i < 100000; i++) {
        /* intentional delay */
    }
}

/* Reset handler */
void Reset_Handler(void) {
    unsigned int *src;
    unsigned int *dst;

    /* Copy .data from FLASH to RAM */
    src = (unsigned int *)&_sidata;
    dst = (unsigned int *)&_sdata;
    while (dst < (unsigned int *)&_edata) {
        *dst++ = *src++;
    }

    /* Zero .bss */
    dst = (unsigned int *)&_sbss;
    while (dst < (unsigned int *)&_ebss) {
        *dst++ = 0;
    }

    /* Enter main */
    main();

    /* Trap if main returns */
    while (1);
}

/* Vector table */
__attribute__((section(".isr_vector")))
void (*vector_table[])(void) = {
    (void (*)(void))0x20001000, // Initial SP
    Reset_Handler,             // Reset
    Default_Handler,           // NMI
    Default_Handler,           // HardFault
    Default_Handler,           // MemManage
    Default_Handler,           // BusFault
    Default_Handler,           // UsageFault
    0, 0, 0, 0,                // Reserved
    Default_Handler,           // SVCall
    Default_Handler,           // Debug Monitor
    0,                         // Reserved
    PendSV_Handler,           // PendSV
    SysTick_Handler            // SysTick
};
