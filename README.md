Bare-Metal ARM Cortex-M Firmware: Boot, Interrupts, Timing, and RTOS Foundations (Simulation)
🧭 Overview

This project implements a bare-metal firmware execution model for ARM Cortex-M processors, built from reset vector to interrupt-driven execution, without using vendor HALs, Arduino frameworks, or RTOS abstractions.

The goal is to understand and validate the hardware–software interface underlying real-time systems, with a focus on startup behavior, interrupt handling, timing, and preemption.
All development and debugging is performed using QEMU and GDB, enabling deterministic inspection of low-level behavior.

This project serves as a foundation for RTOS integration (FreeRTOS) and real-time system design.

🧱 Architecture & Platform

CPU Architecture: ARM Cortex-M (Cortex-M3, QEMU emulation)

Execution Model: Bare-metal (no OS, no HAL)

Toolchain: arm-none-eabi-gcc, gdb-multiarch, qemu-system-arm

Environment: Linux (WSL2 + Ubuntu)

Language: C (freestanding, no standard library)

🔁 System Bring-Up Flow

The firmware explicitly implements the Cortex-M boot process:

Vector Table Initialization

Initial stack pointer

Reset handler

Core exception handlers (SysTick, PendSV, faults)

Startup Code (Reset_Handler)

Copies .data section from FLASH to RAM

Zeros .bss section

Transfers control to main()

Traps execution if main() returns

Memory Model Validation

Verified correct initialization of global/static variables

Confirmed via ELF inspection and GDB memory analysis

⏱ Interrupts & Timing
SysTick Timer

Configured via memory-mapped registers

Generates periodic core exceptions

Acts as a deterministic system tick

PendSV Exception

Explicitly triggered to simulate deferred work

Used to demonstrate interrupt preemption

Models RTOS context switch behavior

Interrupt Priority

Priorities configured via SCB registers

Demonstrated how higher-priority exceptions delay SysTick

Observed interrupt latency and jitter under contention

🔍 Debugging & Verification

Full system debugging using QEMU + GDB

Verified:

Reset flow

Vector table correctness

Interrupt entry/exit

Stack integrity

Persistent state across ISR invocations

Learned limitations of breakpoints for real-time analysis and used counter-based observation instead

🎯 Key Concepts Demonstrated

Cortex-M startup sequence

Linker scripts and memory layout

Interrupt vector tables

Core exceptions (SysTick, PendSV)

Interrupt priority and preemption

Real-time timing behavior and jitter

Foundations of RTOS tick and context switching

🚧 Current Status

✔ Bare-metal startup complete

✔ Interrupt handling validated

✔ Timing and jitter analyzed

⏳ Next step: Minimal FreeRTOS kernel integration

📈 Why This Project Matters

This project focuses on execution correctness and determinism, not application features.
It demonstrates understanding of the same low-level mechanisms used in:

Defense systems

Avionics

Automotive ECUs

Safety-critical real-time software
