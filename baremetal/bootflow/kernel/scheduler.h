#ifndef SCHEDULER_H
#define SCHEDULER_H

#include <stdint.h>

typedef struct {
    uint32_t *sp;
    uint32_t id;
} tcb_t;

/* Current and next task pointers (used by PendSV) */
extern tcb_t *current_tcb;
extern tcb_t *next_tcb;

/* Scheduler interface */
void scheduler_init(void);
void scheduler_tick(void);

#endif
