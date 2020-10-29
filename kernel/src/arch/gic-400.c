#include "arch/gic-400.h"

#include "arch/mmio.h"

void gic_400_init() {
	// Initialize the distributor

	// Start by disabling it, acknowledge and deactivate all interrupts
	mmio_store(GICD_CTLR, GICD_CTLR_DISABLE);
	for (unsigned n = 0; n < IRQ_LINES / 32; n++) {
		mmio_store(GICD_ICENABLER0 + (4 * n), 0xFFFFFFFF);
		mmio_store(GICD_ICPENDR0 + (4 * n), 0xFFFFFFFF);
		mmio_store(GICD_ICACTIVER0 + (4 * n), 0xFFFFFFFF);
	}

	// direct all interrupts to core 0 with default priority
	for (unsigned n = 0; n < IRQ_LINES / 4; n++) {
		const uint32_t priority = GICD_IPRIORITYR_DEFAULT |
		                          GICD_IPRIORITYR_DEFAULT << 8 |
		                          GICD_IPRIORITYR_DEFAULT << 16 |
		                          GICD_IPRIORITYR_DEFAULT << 24;
		mmio_store(GICD_IPRIORITYR0 + (4 * n), priority);

		const uint32_t routing = GICD_ITARGETSR_CORE0 |
		                         GICD_ITARGETSR_CORE0 << 8 |
		                         GICD_ITARGETSR_CORE0 << 16 |
		                         GICD_ITARGETSR_CORE0 << 24;
		mmio_store(GICD_ITARGETSR0 + (4 * n), routing);
	}

	// set all interrupts to level triggered
	for (unsigned n = 0; n < IRQ_LINES / 16; n++) {
		mmio_store(GICD_ICFGR0 + (4 * n), 0);
	}

	// Now we can re-enable the distributor
	mmio_store(GICD_CTLR, GICD_CTLR_ENABLE);

	// initialize core 0 CPU interface
	mmio_store(GICC_PMR, GICC_PMR_PRIORITY);
	mmio_store(GICC_CTLR, GICC_CTLR_ENABLE);
}