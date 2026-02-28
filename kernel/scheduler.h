#ifndef SCHEDULER_H
#define SCHEDULER_H

#include <stdint.h>
#include "tcb.h"

extern tcb_t     task_table[MAX_TASKS];
extern tcb_t    *current_tcb;
extern tcb_t    *next_tcb;
extern uint32_t  global_tick_count;

void scheduler_init(void);
int  scheduler_add_task(void (*entry)(void), uint32_t *stack, uint32_t stack_size, uint8_t priority, const char *name);
void scheduler_tick(void);
void scheduler_select_next(void);
void task_delay(uint32_t ticks);
void task_alive(void);

#endif
