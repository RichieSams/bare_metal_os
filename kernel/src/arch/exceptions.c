#include "arch/exceptions.h"

#include "arch/mmio.h"
#include "arch/timer.h"

#include "printf/printf.h"

#include <stddef.h>

static IRQHandler s_irqHandlerTable[IRQ_LINES];
static void *s_irqHandlerTableParams[IRQ_LINES];

void exception_handler_table_init();

const char *entry_error_messages[] = {
    "SYNC_INVALID_EL1t",
    "IRQ_INVALID_EL1t",
    "FIQ_INVALID_EL1t",
    "ERROR_INVALID_EL1T",

    "SYNC_INVALID_EL1h",
    "IRQ_INVALID_EL1h",
    "FIQ_INVALID_EL1h",
    "ERROR_INVALID_EL1h",

    "SYNC_INVALID_EL0_64",
    "IRQ_INVALID_EL0_64",
    "FIQ_INVALID_EL0_64",
    "ERROR_INVALID_EL0_64",

    "SYNC_INVALID_EL0_32",
    "IRQ_INVALID_EL0_32",
    "FIQ_INVALID_EL0_32",
    "ERROR_INVALID_EL0_32"};

void show_invalid_entry_message(int type, size_t esr, size_t address) {
	printf("%s, ESR: %x, address: %x\n", entry_error_messages[type], esr, address);
}

void exception_handler_init() {
	exception_handler_table_init();

	for (size_t i = 0; i < IRQ_LINES; ++i) {
		s_irqHandlerTable[i] = NULL;
	}
}

void handle_irq() {
	printf("IRQ handled\n");

	uint32_t nIAR = mmio_load(GICC_IAR);

	uint32_t nIRQ = nIAR & GICC_IAR_INTERRUPT_ID__MASK;
	// assert(nIRQ < IRQ_LINES);
	if (nIRQ < IRQ_LINES) {
		if (nIRQ > 15) {
			// peripheral interrupts (PPI and SPI)
			IRQHandler handler = s_irqHandlerTable[nIRQ];

			if (handler != NULL) {
				void *param = s_irqHandlerTableParams[nIRQ];
				handler(param);
			} else {
				disable_irq(nIRQ);
			}
		}
#ifdef ARM_ALLOW_MULTI_CORE
		else {
			// software generated interrupts (SGI)
			unsigned nFromCore = (nIAR & GICC_IAR_CPUID__MASK) >> GICC_IAR_CPUID__SHIFT;
			CMultiCoreSupport::LocalInterruptHandler(nFromCore, nIRQ);
		}
#endif

		// Arm recommends that software preserves the entire register value read from GICC_IAR,
		// and writes that value back to GICC_EOIR on completion of interrupt processing.
		mmio_store(GICC_EOIR, nIAR);
	} else {
		// spurious interrupts
		// assert(nIRQ >= 1020);
	}
}

void connect_irq(unsigned nIRQ, IRQHandler handler, void *param) {
	// assert(nIRQ < IRQ_LINES);

	s_irqHandlerTable[nIRQ] = handler;
	s_irqHandlerTableParams[nIRQ] = param;
	mmio_store(GICD_ISENABLER0 + 4 * (nIRQ / 32), 1 << (nIRQ % 32));
}

void disconnect_irq(unsigned nIRQ) {
	// assert(nIRQ < IRQ_LINES);

	mmio_store(GICD_ICENABLER0 + 4 * (nIRQ / 32), 1 << (nIRQ % 32));
	s_irqHandlerTable[nIRQ] = NULL;
	s_irqHandlerTableParams[nIRQ] = NULL;
}
