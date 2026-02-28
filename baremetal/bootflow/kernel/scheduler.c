#include "scheduler.h"

/* TCBs defined in main.c for now */
extern tcb_t tcb1;
extern tcb_t tcb2;

tcb_t *current_tcb;
tcb_t *next_tcb;

void scheduler_init(void)
{
    current_tcb = &tcb1;
    next_tcb    = &tcb2;
}

void scheduler_tick(void)
{
    /* Simple round-robin */
    tcb_t *tmp = current_tcb;
    current_tcb = next_tcb;
    next_tcb = tmp;
}
