#include <stdint.h>

/* SCB + SysTick */
#define SCB_ICSR   (*(volatile uint32_t *)0xE000ED04)
#define SCB_SHPR3  (*(volatile uint32_t *)0xE000ED20)
#define SYST_CSR   (*(volatile uint32_t *)0xE000E010)
#define SYST_RVR   (*(volatile uint32_t *)0xE000E014)
#define SYST_CVR   (*(volatile uint32_t *)0xE000E018)

/* Task Control Block */
typedef struct {
    uint32_t *sp;
    uint32_t id;
} tcb_t;

/* Forward declarations */
void task1(void);
void task2(void);

/* Stacks */
#define STACK_WORDS 128
uint32_t task1_stack[STACK_WORDS];
uint32_t task2_stack[STACK_WORDS];

/* TCBs */
tcb_t tcb1, tcb2;
tcb_t *current_tcb;
tcb_t *next_tcb;

/* Initialize a task stack */
void init_task(tcb_t *tcb, void (*entry)(void), uint32_t *stack, uint32_t id)
{
    uint32_t *sp = stack + STACK_WORDS;

    /* Hardware-stacked frame */
    *(--sp) = 0x01000000;        // xPSR
    *(--sp) = (uint32_t)entry;  // PC
    *(--sp) = 0xFFFFFFFD;       // LR (return to thread, PSP)
    *(--sp) = 0;                // R12
    *(--sp) = 0;                // R3
    *(--sp) = 0;                // R2
    *(--sp) = 0;                // R1
    *(--sp) = 0;                // R0

    /* Software-saved registers */
    for (int i = 0; i < 8; i++)
        *(--sp) = 0xDEADBEEF;

    tcb->sp = sp;
    tcb->id = id;
}

/* Tasks */
void task1(void)
{
    __asm volatile (
        "ldr r4, =0x11111111\n"
        "ldr r5, =0x11112222\n"
        "ldr r6, =0x11113333\n"
        "ldr r7, =0x11114444\n"
        "ldr r8, =0x11115555\n"
        "ldr r9, =0x11116666\n"
        "ldr r10,=0x11117777\n"
        "ldr r11,=0x11118888\n"
    );

    while (1) {
        __asm volatile ("nop");
    }
}

void task2(void)
{
    __asm volatile (
        "ldr r4, =0x22221111\n"
        "ldr r5, =0x22222222\n"
        "ldr r6, =0x22223333\n"
        "ldr r7, =0x22224444\n"
        "ldr r8, =0x22225555\n"
        "ldr r9, =0x22226666\n"
        "ldr r10,=0x22227777\n"
        "ldr r11,=0x22228888\n"
    );

    while (1) {
        __asm volatile ("nop");
    }
}

/* SysTick */
void SysTick_Handler(void)
{
    SCB_ICSR = (1 << 28);
}

int main(void)
{
    init_task(&tcb1, task1, task1_stack, 1);
    init_task(&tcb2, task2, task2_stack, 2);

    current_tcb = &tcb1;
    next_tcb    = &tcb2;

    /* Set PSP */
    __asm volatile ("msr psp, %0" :: "r"(current_tcb->sp));

    /* Switch to PSP in thread mode */
    __asm volatile (
        "mov r0, #2\n"
        "msr control, r0\n"
        "isb\n"
    );

    /* Priorities: PendSV lowest */
    SCB_SHPR3 = (0xFF << 16);

    /* SysTick */
    SYST_RVR = 100000;
    SYST_CVR = 0;
    SYST_CSR = 7;

    __asm volatile ("cpsie i");

    /* Start FIRST task via PendSV */
    SCB_ICSR = (1 << 28);
    __asm volatile ("wfi");

    while (1) {}
}
