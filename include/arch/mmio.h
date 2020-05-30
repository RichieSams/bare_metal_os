#pragma once

#include <stddef.h>
#include <stdint.h>

#define MMIO_BASE 0xFE000000

#define GPFSEL0   (MMIO_BASE + 0x00200000)
#define GPFSEL1   (MMIO_BASE + 0x00200004)
#define GPFSEL2   (MMIO_BASE + 0x00200008)
#define GPFSEL3   (MMIO_BASE + 0x0020000C)
#define GPFSEL4   (MMIO_BASE + 0x00200010)
#define GPFSEL5   (MMIO_BASE + 0x00200014)
#define GPSET0    (MMIO_BASE + 0x0020001C)
#define GPSET1    (MMIO_BASE + 0x00200020)
#define GPCLR0    (MMIO_BASE + 0x00200028)
#define GPLEV0    (MMIO_BASE + 0x00200034)
#define GPLEV1    (MMIO_BASE + 0x00200038)
#define GPEDS0    (MMIO_BASE + 0x00200040)
#define GPEDS1    (MMIO_BASE + 0x00200044)
#define GPHEN0    (MMIO_BASE + 0x00200064)
#define GPHEN1    (MMIO_BASE + 0x00200068)
#define GPPUD     (MMIO_BASE + 0x00200094)
#define GPPUDCLK0 (MMIO_BASE + 0x00200098)
#define GPPUDCLK1 (MMIO_BASE + 0x0020009C)

#define GPIO_INPUT  (0 << 0) // GPIO is input      (000)
#define GPIO_OUTPUT (1 << 0) // GPIO is output     (001)
#define GPIO_ALT0   (4)      // GPIO is Alternate0 (100)
#define GPIO_ALT1   (5)      // GPIO is Alternate1 (101)
#define GPIO_ALT2   (6)      // GPIO is Alternate2 (110)
#define GPIO_ALT3   (7)      // GPIO is Alternate3 (111)
#define GPIO_ALT4   (3)      // GPIO is Alternate4 (011)
#define GPIO_ALT5   (2)      // GPIO is Alternate5 (010)

// GPIO Pull Up and Pull Down registers
#define GPPUD           (MMIO_BASE + 0x00200094)
#define GPPUD_OFF       (0 << 0)
#define GPPUD_PULL_DOWN (1 << 0)
#define GPPUD_PULL_UP   (1 << 1)
#define GPPUDCLK0       (MMIO_BASE + 0x00200098)
#define GPPUDCLK1       (MMIO_BASE + 0x0020009C)

// GPIO Pull Up and Down registers (RPI 4)
#define GPIO_PUP_PDN_CNTRL_REG0 (MMIO_BASE + 0x002000E4)
#define GPIO_PUP_PDN_CNTRL_REG1 (MMIO_BASE + 0x002000E8)
#define GPIO_PUP_PDN_CNTRL_REG2 (MMIO_BASE + 0x002000EC)
#define GPIO_PUP_PDN_CNTRL_REG3 (MMIO_BASE + 0x002000F0)
#define GPPUP_OFF               (0 << 0)
#define GPPUP_PULL_UP           (1 << 0)
#define GPPUP_PULL_DOWN         (1 << 1)

#define CLOCK_RESERVED 0x000000000
#define CLOCK_EMMC     0x000000001
#define CLOCK_UART     0x000000002
#define CLOCK_ARM      0x000000003
#define CLOCK_CORE     0x000000004
#define CLOCK_V3D      0x000000005
#define CLOCK_H264     0x000000006
#define CLOCK_ISP      0x000000007
#define CLOCK_SDRAM    0x000000008
#define CLOCK_PIXEL    0x000000009
#define CLOCK_PWM      0x00000000a

#define IRQ_PENDING_BASIC (MMIO_BASE + 0x0000B200)
#define IRQ_PENDING_1     (MMIO_BASE + 0x0000B204)
#define IRQ_PENDING_2     (MMIO_BASE + 0x0000B208)
#define FIQ_CONTROL       (MMIO_BASE + 0x0000B20C)
#define IRQ_ENABLE_1      (MMIO_BASE + 0x0000B210)
#define IRQ_ENABLE_2      (MMIO_BASE + 0x0000B214)
#define IRQ_ENABLE_BASIC  (MMIO_BASE + 0x0000B218)
#define IRQ_DISABLE_1     (MMIO_BASE + 0x0000B21C)
#define IRQ_DISABLE_2     (MMIO_BASE + 0x0000B220)
#define IRQ_DISABLE_BASIC (MMIO_BASE + 0x0000B224)

// SYSTEM_TIMER_IRQ_0 and SYSTEM_TIMER_IRQ_2 are used by the GPU and are inaccessible from the ARM core
#define SYSTEM_TIMER_IRQ_1 (1 << 1)
#define SYSTEM_TIMER_IRQ_3 (1 << 3)

#define SYSTEM_TIMER_CONTROL_STATUS (MMIO_BASE + 0x00003000)
#define SYSTEM_TIMER_COUNTER_LO     (MMIO_BASE + 0x00003004)
#define SYSTEM_TIMER_COUNTER_HI     (MMIO_BASE + 0x00003008)
#define SYSTEM_TIMER_COMPARE_1      (MMIO_BASE + 0x00003010)
#define SYSTEM_TIMER_COMPARE_3      (MMIO_BASE + 0x00003018)
// Same as above, SYSTEM_TIMER_COMPARE_0 and SYSTEM_TIMER_COMPARE_2 are used by the GPU and are inaccessible from the ARM core

#define SYSTEM_TIMER_CS_M1 (1 << 1)
#define SYSTEM_TIMER_CS_M3 (1 << 3)

static inline void mmio_store(size_t address, uint32_t val) {
	asm volatile(
	    "str %w[val], [ %[addr] ]"
	    :
	    : [ addr ] "r"(address), [ val ] "r"(val)
	    : "memory");
}

static inline uint32_t mmio_load(size_t address) {
	uint32_t val;

	asm volatile(
	    "ldr %w[val], [ %[addr] ]"
	    : [ val ] "=r"(val)
	    : [ addr ] "r"(address)
	    : "memory");

	return val;
}
