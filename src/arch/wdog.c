#include "arch/mmio.h"

#define SECS_TO_WDOG_TICKS(x) ((x) << 16)
#define WDOG_TICKS_TO_SECS(x) ((x) >> 16)

#define PM_WDOG_TIME_SET 0x000fffff

void reboot() {
	mmio_store(PM_WDOG, PM_PASSWORD | 1); // timeout = 1/16th of a second? (whatever)
	mmio_store(PM_RSTC, PM_PASSWORD | PM_RSTC_WRCFG_FULL_RESET);
	while (1) {
		// Wait for timeout to expire and we will reboot
	}
}
