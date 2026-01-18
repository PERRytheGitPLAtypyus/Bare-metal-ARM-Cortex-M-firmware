.syntax unified
.cpu cortex-m3
.thumb

.global PendSV_Handler
.type PendSV_Handler, %function

.extern current_tcb
.extern next_tcb

PendSV_Handler:
    mrs r0, psp
    stmdb r0!, {r4-r11}

    ldr r1, =current_tcb
    ldr r2, [r1]
    str r0, [r2]

    ldr r3, =next_tcb
    ldr r4, [r3]
    str r4, [r1]

    ldr r0, [r4]
    ldmia r0!, {r4-r11}
    msr psp, r0

    bx lr
