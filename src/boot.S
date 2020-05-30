#include "arch/mm.h"

.section ".text.boot"

.globl _start
_start:
/******************************************************************
 *  Core specific initialization
 *****************************************************************/
    mrs     x0, mpidr_el1
    and     x0, x0, #0x3    // Get the core id

    cbz     x0, core_0_init
	cmp     x0, #1
	beq     core_1_init
	cmp     x0, #2
	beq     core_2_init
	cmp     x0, #3
	beq     core_3_init
	b       _hang   // We should never get here


core_0_init:
    // Set the stack pointers
    ldr	    x1, =__EL0_stack_core0
	ldr     x2, =__EL1_stack_core0
	ldr	    x3, =__EL2_stack_core0
	msr	    sp_el0, x1
	msr     sp_el1, x2
	mov	    sp, x3
    b       el1_ret

core_1_init:
    // Set the stack pointers
    ldr	    x1, =__EL0_stack_core1
	ldr     x2, =__EL1_stack_core1
	ldr	    x3, =__EL2_stack_core1
	msr	    sp_el0, x1
	msr     sp_el1, x2
	mov	    sp, x3
    b       el1_ret

core_2_init:
    // Set the stack pointers
    ldr	    x1, =__EL0_stack_core2
	ldr     x2, =__EL1_stack_core2
	ldr	    x3, =__EL2_stack_core2
	msr	    sp_el0, x1
	msr     sp_el1, x2
	mov	    sp, x3
    b       el1_ret

core_3_init:
    // Set the stack pointers
    ldr	    x1, =__EL0_stack_core3
	ldr     x2, =__EL1_stack_core3
	ldr	    x3, =__EL2_stack_core3
	msr	    sp_el0, x1
	msr     sp_el1, x2
	mov	    sp, x3
    b       el1_ret



el1_ret:

/******************************************************************
 *  Start the kernel code
 *****************************************************************/
    mrs     x0, mpidr_el1
    and     x0, x0, #0x3    // Get the core id

    cbz     x0, __main_core_start
	cmp     x0, #1
	beq     __worker_core_start
	cmp     x0, #2
	beq     __worker_core_start
	cmp     x0, #3
	beq     __worker_core_start
	b       _hang   // We should never get here


__main_core_start:
    // Clear bss section to zero
	adr     x0, __bss_start
    adr     x1, __bss_end
    // Check for empty bss
    cmp     x0, x1
    beq     1f
__bss_zero_loop:
    str     xzr, [x0], #8
    cmp     x1, x0
    bhi     __bss_zero_loop
1:
    bl      system_init

    // Wake up the worker cores
    // ldr     x0, _start
    // ldr     x1, #CPU1_RESUME_ADDR
    // str     x0, [x1]
    // ldr     x1, #CPU2_RESUME_ADDR
    // str     x0, [x1]
    // ldr     x1, #CPU2_RESUME_ADDR
    // str     x0, [x1]
    // sev

    // Start it ourselves
    bl      kernel_main
    b       _hang


__worker_core_start:
    bl      kernel_main
    b       _hang


_hang:
    b       _hang
