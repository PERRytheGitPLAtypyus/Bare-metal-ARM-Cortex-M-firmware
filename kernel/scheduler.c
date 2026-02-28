#include "scheduler.h"
#include "tcb.h"

/* ─── Globals ────────────────────────────────────────────────── */
tcb_t     task_table[MAX_TASKS];
tcb_t    *current_tcb;
tcb_t    *next_tcb;
uint32_t  global_tick_count;

/* ─── Raw register macros ────────────────────────────────────── */
#define SCB_ICSR  (*(volatile uint32_t *)0xE000ED04)

/* ─── scheduler_init ─────────────────────────────────────────── */
void scheduler_init(void)
{
    uint32_t i;
    for (i = 0; i < MAX_TASKS; i++)
    {
        task_table[i].state            = TASK_DEAD;
        task_table[i].sp               = 0;
        task_table[i].id               = i;
        task_table[i].priority         = 255;
        task_table[i].ticks_remaining  = 0;
        task_table[i].last_alive_tick  = 0;
        task_table[i].name             = "idle";
    }
}

/* ─── scheduler_add_task ─────────────────────────────────────── */
int scheduler_add_task(void (*entry)(void), uint32_t *stack,
                       uint32_t stack_size, uint8_t priority,
                       const char *name)
{
    int slot = -1;
    uint32_t i;

    /* Find first dead slot */
    for (i = 0; i < MAX_TASKS; i++)
    {
        if (task_table[i].state == TASK_DEAD)
        {
            slot = (int)i;
            break;
        }
    }

    if (slot < 0)
        return -1;

    /* Build initial stack frame from top of stack downward */
    uint32_t *sp = stack + stack_size;

    /* Hardware exception frame (ARM pushes in this order on exception entry):
       xPSR, PC, LR, R12, R3, R2, R1, R0 — bottom to top in memory,
       so we push top-down: xPSR first (ends up at lowest address of frame) */
    *(--sp) = 0x01000000;          /* xPSR  — Thumb bit set             */
    *(--sp) = (uint32_t)entry;     /* PC    — task entry point          */
    *(--sp) = 0xFFFFFFFD;          /* LR    — EXC_RETURN: Thread, PSP   */
    *(--sp) = 0;                   /* R12                               */
    *(--sp) = 0;                   /* R3                                */
    *(--sp) = 0;                   /* R2                                */
    *(--sp) = 0;                   /* R1                                */
    *(--sp) = 0;                   /* R0                                */

    /* Software-saved frame: R11 down to R4 (8 regs) */
    for (i = 0; i < 8; i++)
    {
        *(--sp) = 0xDEADBEEF;
    }

    /* Fill in TCB */
    task_table[slot].sp               = sp;
    task_table[slot].state            = TASK_READY;
    task_table[slot].priority         = priority;
    task_table[slot].name             = name;
    task_table[slot].id               = (uint32_t)slot;
    task_table[slot].ticks_remaining  = 0;
    task_table[slot].last_alive_tick  = 0;

    return slot;
}

/* ─── scheduler_select_next ──────────────────────────────────── */
void scheduler_select_next(void)
{
    uint32_t    i;
    tcb_t      *best      = 0;
    uint8_t     best_prio = 255;

    for (i = 0; i < MAX_TASKS; i++)
    {
        if (task_table[i].state == TASK_READY &&
            task_table[i].priority <= best_prio)
        {
            best_prio = task_table[i].priority;
            best      = &task_table[i];
        }
    }

    if (best)
        next_tcb = best;
    else
        next_tcb = current_tcb;  /* stay on current if no READY task */
}

/* ─── scheduler_tick ─────────────────────────────────────────── */
void scheduler_tick(void)
{
    uint32_t i;

    global_tick_count++;

    /* Unblock tasks whose delay has expired */
    for (i = 0; i < MAX_TASKS; i++)
    {
        if (task_table[i].state == TASK_BLOCKED)
        {
            if (task_table[i].ticks_remaining > 0)
                task_table[i].ticks_remaining--;

            if (task_table[i].ticks_remaining == 0)
                task_table[i].state = TASK_READY;
        }
    }

    scheduler_select_next();

    /* Trigger PendSV to perform context switch */
    SCB_ICSR |= (1U << 28);
}

/* ─── task_delay ─────────────────────────────────────────────── */
void task_delay(uint32_t ticks)
{
    current_tcb->state           = TASK_BLOCKED;
    current_tcb->ticks_remaining = ticks;
    SCB_ICSR |= (1U << 28);
}

/* ─── task_alive ─────────────────────────────────────────────── */
void task_alive(void)
{
    current_tcb->last_alive_tick = global_tick_count;
}
