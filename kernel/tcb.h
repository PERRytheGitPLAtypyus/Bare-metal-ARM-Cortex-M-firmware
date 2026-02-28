#ifndef TCB_H
#define TCB_H
#include <stdint.h>

#define MAX_TASKS 4

typedef enum {
    TASK_READY,
    TASK_RUNNING,
    TASK_BLOCKED,
    TASK_DEAD
} task_state_t;

typedef struct {
    uint32_t      *sp;
    uint32_t       id;
    uint8_t        priority;
    task_state_t   state;
    uint32_t       ticks_remaining;
    uint32_t       last_alive_tick;
    const char    *name;
} tcb_t;

#endif
