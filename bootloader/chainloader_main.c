#include "arch/uart.h"

#include "stddef.h"

void chainloader_main() {
	uart_init();

	size_t size;
	volatile char *kernel = (char *)0x80000;

	while (1) {
		// Notify raspbootcom to send the kernel
		uart_send(3);
		uart_send(3);
		uart_send(3);

		/* read the kernel's size */
		size = uart_recv();
		size |= uart_recv() << 8;
		size |= uart_recv() << 16;
		size |= uart_recv() << 24;

		// send negative or positive acknowledge
		if (size < 32 || size >= 4 * 1024 * 1024) {
			// Size error
			uart_send('S');
			uart_send('E');
		} else {
			uart_send('O');
			uart_send('K');
			break;
		}
	}

	// Read in the image
	while (size--) {
		*kernel++ = uart_recv();
	}

	// restore arguments and jump to the new kernel.
	asm volatile(
	    "mov x0, x10;"
	    "mov x1, x11;"
	    "mov x2, x12;"
	    "mov x3, x13;"
	    // we must force an absolute address to branch to
	    "mov x30, 0x80000; ret");
}
