    .syntax unified
    .cpu    cortex-m3
    .thumb

    .global PendSV_Handler
    .type   PendSV_Handler, %function

    .extern current_tcb
    .extern next_tcb

PendSV_Handler:

    /* STEP 1: disable interrupts during context switch */
    cpsid   i

    /* STEP 2: r0 = current task's stack pointer */
    mrs     r0, psp

    /* STEP 3: push R4-R11 onto process stack, r0 updated */
    stmdb   r0!, {r4-r11}

    /* STEP 4: save updated SP into current_tcb->sp */
    ldr     r1, =current_tcb    /* r1 = address of current_tcb pointer */
    ldr     r2, [r1]            /* r2 = current_tcb (pointer value)    */
    str     r0, [r2, #0]        /* current_tcb->sp = r0 (sp is first field, offset 0) */

    /* STEP 5: symmetric pointer swap — current_tcb = next_tcb, next_tcb = old current_tcb */
    ldr     r3, =next_tcb       /* r3 = address of next_tcb pointer    */
    ldr     r4, [r3]            /* r4 = next_tcb value                 */
    str     r4, [r1]            /* current_tcb = next_tcb              */
    str     r2, [r3]            /* next_tcb = old current_tcb          */

    /* STEP 6: load new task stack pointer */
    ldr     r0, [r4, #0]        /* r0 = next_tcb->sp (sp at offset 0)  */

    /* STEP 7: pop R4-R11 from new task's stack */
    ldmia   r0!, {r4-r11}

    /* STEP 8: update PSP to new task's stack pointer */
    msr     psp, r0

    /* STEP 9: re-enable interrupts */
    cpsie   i

    /* STEP 10: EXC_RETURN — hardware restores R0-R3,R12,LR,PC,xPSR */
    bx      lr

    .size PendSV_Handler, .-PendSV_Handler
