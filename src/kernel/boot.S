#include "mm.h"

.section ".text.boot"

.globl _start
_start:
    mrs     x0, mpidr_el1
    and     x0, x0, #0xFF   // Get the processor id
    cbz     x0, bss_clear   // Clear bss only on core zero
    b       delay_start

delay_start:
    mov     x0, #200   // Wait 200 cycles
delay_loop:
    subs    x0, x0, #1
    bne     delay_loop
    b       master

bss_clear:
    adr     x0, bss_begin
    adr     x1, bss_end
    sub     x1, x1, x0
    bl      memzero
    b       master

master:
    mov     sp, #STACK_START
    mrs     x0, mpidr_el1  // Get the processor id again
    and     x0, x0, #0xFF
    bl      kernel_main
