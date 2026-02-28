#include "scheduler.h"

/* ─── Raw register macros ────────────────────────────────────── */
#define SYST_CSR  (*(volatile uint32_t *)0xE000E010)
#define SYST_RVR  (*(volatile uint32_t *)0xE000E014)
#define SYST_CVR  (*(volatile uint32_t *)0xE000E018)
#define SCB_SHPR3 (*(volatile uint32_t *)0xE000ED20)

/* ─── systick_init ───────────────────────────────────────────── */
void systick_init(uint32_t reload_value)
{
    SCB_SHPR3 |= (0xFF << 16);   /* PendSV  priority = lowest (0xFF) */
    SCB_SHPR3 |= (0xFF << 24);   /* SysTick priority = lowest (0xFF) */

    SYST_RVR = reload_value - 1; /* reload register                  */
    SYST_CVR = 0;                /* clear current value              */
    SYST_CSR = 7;                /* ENABLE | TICKINT | CLKSOURCE     */
}

/* ─── SysTick_Handler ─────────────────────────────────────────
   Not declared weak — this overrides the stub in startup.c.   */
void SysTick_Handler(void)
{
    scheduler_tick();
}
