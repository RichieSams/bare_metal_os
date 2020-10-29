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

// Power Management, Reset controller and Watchdog registers
#define PM_BASE                  (MMIO_BASE + 0x100000)
#define PM_RSTC                  (PM_BASE + 0x00001c)
#define PM_WDOG                  (PM_BASE + 0x000024)
#define PM_WDOG_RESET            0000000000
#define PM_PASSWORD              0x5a000000
#define PM_WDOG_TIME_SET         0x000fffff
#define PM_RSTC_WRCFG_CLR        0xffffffcf
#define PM_RSTC_WRCFG_SET        0x00000030
#define PM_RSTC_WRCFG_FULL_RESET 0x00000020
#define PM_RSTC_RESET            0x00000102

#define GIC_BASE  0xFF840000
#define GICD_BASE (GIC_BASE + 0x1000)
#define GICC_BASE (GIC_BASE + 0x2000)

#define IRQ_LINES 256

// GIC distributor registers
#define GICD_CTLR         (GICD_BASE + 0x000)
#define GICD_CTLR_DISABLE (0 << 0)
#define GICD_CTLR_ENABLE  (1 << 0)
#define GICD_TYPER        (GICD_BASE + 0x004)
// secure access
#define GICD_CTLR_ENABLE_GROUP0             (1 << 0)
#define GICD_CTLR_ENABLE_GROUP1             (1 << 1)
#define GICD_IGROUPR0                       (GICD_BASE + 0x080) // secure access for group 0
#define GICD_ISENABLER0                     (GICD_BASE + 0x100)
#define GICD_ICENABLER0                     (GICD_BASE + 0x180)
#define GICD_ISPENDR0                       (GICD_BASE + 0x200)
#define GICD_ICPENDR0                       (GICD_BASE + 0x280)
#define GICD_ISACTIVER0                     (GICD_BASE + 0x300)
#define GICD_ICACTIVER0                     (GICD_BASE + 0x380)
#define GICD_IPRIORITYR0                    (GICD_BASE + 0x400)
#define GICD_IPRIORITYR_DEFAULT             0xA0
#define GICD_IPRIORITYR_FIQ                 0x40
#define GICD_ITARGETSR0                     (GICD_BASE + 0x800)
#define GICD_ITARGETSR_CORE0                (1 << 0)
#define GICD_ICFGR0                         (GICD_BASE + 0xC00)
#define GICD_ICFGR_LEVEL_SENSITIVE          (0 << 1)
#define GICD_ICFGR_EDGE_TRIGGERED           (1 << 1)
#define GICD_SGIR                           (GICD_BASE + 0xF00)
#define GICD_SGIR_SGIINTID__MASK            0x0F
#define GICD_SGIR_CPU_TARGET_LIST__SHIFT    16
#define GICD_SGIR_TARGET_LIST_FILTER__SHIFT 24

// GIC CPU interface registers
#define GICC_CTLR         (GICC_BASE + 0x000)
#define GICC_CTLR_DISABLE (0 << 0)
#define GICC_CTLR_ENABLE  (1 << 0)
// secure access
#define GICC_CTLR_ENABLE_GROUP0     (1 << 0)
#define GICC_CTLR_ENABLE_GROUP1     (1 << 1)
#define GICC_CTLR_FIQ_ENABLE        (1 << 3)
#define GICC_PMR                    (GICC_BASE + 0x004)
#define GICC_PMR_PRIORITY           (0xF0 << 0)
#define GICC_IAR                    (GICC_BASE + 0x00C)
#define GICC_IAR_INTERRUPT_ID__MASK 0x3FF
#define GICC_IAR_CPUID__SHIFT       10
#define GICC_IAR_CPUID__MASK        (3 << 10)
#define GICC_EOIR                   (GICC_BASE + 0x010)
#define GICC_EOIR_EOIINTID__MASK    0x3FF
#define GICC_EOIR_CPUID__SHIFT      10
#define GICC_EOIR_CPUID__MASK       (3 << 10)

// Arm Local
#define ARM_LOCAL_BASE      0xFF800000
#define ARM_CONTROL         (ARM_LOCAL_BASE + 0x00)
#define CORE_IRQ_CONTROL    (ARM_LOCAL_BASE + 0x0c)
#define PMU_CONTROL_SET     (ARM_LOCAL_BASE + 0x10)
#define PMU_CONTROL_CLR     (ARM_LOCAL_BASE + 0x14)
#define PERI_IRQ_ROUTE0     (ARM_LOCAL_BASE + 0x24)
#define AXI_QUIET_TIME      (ARM_LOCAL_BASE + 0x30)
#define LOCAL_TIMER_CONTROL (ARM_LOCAL_BASE + 0x34)
#define LOCAL_TIMER_IRQ     (ARM_LOCAL_BASE + 0x38)
#define TIMER_CNTRL0        (ARM_LOCAL_BASE + 0x40)
#define TIMER_CNTRL1        (ARM_LOCAL_BASE + 0x44)
#define TIMER_CNTRL2        (ARM_LOCAL_BASE + 0x48)
#define TIMER_CNTRL3        (ARM_LOCAL_BASE + 0x4c)
#define MAILBOX_CNTRL0      (ARM_LOCAL_BASE + 0x50)
#define MAILBOX_CNTRL1      (ARM_LOCAL_BASE + 0x54)
#define MAILBOX_CNTRL2      (ARM_LOCAL_BASE + 0x58)
#define MAILBOX_CNTRL3      (ARM_LOCAL_BASE + 0x5c)
#define IRQ_SOURCE0         (ARM_LOCAL_BASE + 0x60)
#define IRQ_SOURCE1         (ARM_LOCAL_BASE + 0x64)
#define IRQ_SOURCE2         (ARM_LOCAL_BASE + 0x68)
#define IRQ_SOURCE3         (ARM_LOCAL_BASE + 0x6c)
#define FIQ_SOURCE0         (ARM_LOCAL_BASE + 0x70)
#define FIQ_SOURCE1         (ARM_LOCAL_BASE + 0x74)
#define FIQ_SOURCE2         (ARM_LOCAL_BASE + 0x78)
#define FIQ_SOURCE3         (ARM_LOCAL_BASE + 0x7c)

#define SYS_TIMER_BASE  (MMIO_BASE + 0x3000)
#define SYS_TIMER_CS    (SYS_TIMER_BASE + 0x00)
#define SYS_TIMER_CS_M0 (1 << 0)
#define SYS_TIMER_CS_M1 (1 << 1)
#define SYS_TIMER_CS_M2 (1 << 2)
#define SYS_TIMER_CS_M3 (1 << 3)
#define SYS_TIMER_CLO   (SYS_TIMER_BASE + 0x04)
#define SYS_TIMER_CHI   (SYS_TIMER_BASE + 0x08)
#define SYS_TIMER_C0    (SYS_TIMER_BASE + 0x0C)
#define SYS_TIMER_C1    (SYS_TIMER_BASE + 0x10)
#define SYS_TIMER_C2    (SYS_TIMER_BASE + 0x14)
#define SYS_TIMER_C3    (SYS_TIMER_BASE + 0x18)

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
