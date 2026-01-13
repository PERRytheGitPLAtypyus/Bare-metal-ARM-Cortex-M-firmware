/* SysTick registers */
#define SYST_CSR   (*(volatile unsigned int *)0xE000E010)
#define SYST_RVR   (*(volatile unsigned int *)0xE000E014)
#define SYST_CVR   (*(volatile unsigned int *)0xE000E018)

/* System Control Block registers */
#define SCB_ICSR   (*(volatile unsigned int *)0xE000ED04)
#define SCB_SHPR3  (*(volatile unsigned int *)0xE000ED20)

/* From startup.c */
extern volatile unsigned int systick_count;

int main(void) {
    /* -------------------------------
     * Interrupt priority configuration
     * -------------------------------
     * SysTick = medium priority (0x80)
     * PendSV  = higher priority (0x40)
     * Lower value = higher priority
     */
    SCB_SHPR3 = (0x80 << 24) | (0x40 << 16);

    /* -------------------------------
     * SysTick configuration
     * ------------------------------- */
    SYST_CSR = 0;          // Disable SysTick
    SYST_RVR = 100000;     // Reload value
    SYST_CVR = 0;          // Clear current value

    /* Enable SysTick: ENABLE | TICKINT | CLKSOURCE */
    SYST_CSR = (1 << 0) | (1 << 1) | (1 << 2);

    while (1) {
        /* Continuously trigger PendSV */
        SCB_ICSR = (1 << 28);
    }
}

