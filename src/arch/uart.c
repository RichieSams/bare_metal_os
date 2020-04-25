#include "arch/uart.h"

#include "arch/gpio.h"
#include "arch/mbox.h"
#include "arch/timer.h"

// PL011 UART registers
#define UART0_DR   ((volatile unsigned int *)(MMIO_BASE + 0x00201000))
#define UART0_FR   ((volatile unsigned int *)(MMIO_BASE + 0x00201018))
#define UART0_IBRD ((volatile unsigned int *)(MMIO_BASE + 0x00201024))
#define UART0_FBRD ((volatile unsigned int *)(MMIO_BASE + 0x00201028))
#define UART0_LCRH ((volatile unsigned int *)(MMIO_BASE + 0x0020102C))
#define UART0_CR   ((volatile unsigned int *)(MMIO_BASE + 0x00201030))
#define UART0_IMSC ((volatile unsigned int *)(MMIO_BASE + 0x00201038))
#define UART0_ICR  ((volatile unsigned int *)(MMIO_BASE + 0x00201044))

void uart_init() {
	// Initialize UART
	*UART0_CR = 0; // Turn UART0 off

	// Set the clock rate of the UART
	// Ref: https://github.com/raspberrypi/documentation/blob/JamesH65-mailbox_docs/configuration/mailboxes/propertiesARM-VC.md#set-clock-rate
	mbox[0] = 9 * sizeof(*mbox); // Byte size of all the entries
	mbox[1] = MBOX_REQUEST;
	mbox[2] = MBOX_TAG_SET_CLK_RATE;
	mbox[3] = 3 * sizeof(*mbox); // Byte size of request value
	mbox[4] = 2 * sizeof(*mbox); // Byte size of response value
	mbox[5] = CLOCK_UART;
	mbox[6] = 4000000; // 4Mhz
	mbox[7] = 0;       // No turbo
	mbox[8] = MBOX_TAG_LAST;
	mbox[9] = 0; // Clear in preparation for the response
	mbox_call(MBOX_CH_PROP);

	// Map UART0 to GPIO pins
	register unsigned int r = *GPFSEL1;
	r &= ~((7 << 12) | (7 << 15)); // GPIO14 and GPIO15
	r |= (4 << 12) | (4 << 15);    // ALT0
	*GPFSEL1 = r;

	// Enable pins 14 and 15
	*GPPUD = 0;
	wait_cycles(150);
	*GPPUDCLK0 = (1 << 14) | (1 << 15);
	wait_cycles(150);
	*GPPUDCLK0 = 0; // Flush GPIO setup

	*UART0_ICR = 0x7FF; // Clear interrupts
	*UART0_IBRD = 2;    // 115200 baud
	*UART0_FBRD = 0xB;
	*UART0_LCRH = 0b11 << 5; // 8n1
	*UART0_CR = 0x301;       // enable Tx, Rx, FIFO
}

void uart_send(char c) {
	// Convert newline to carriage return + newline
	if (c == '\n') {
		uart_send('\r');
	}

	// Wait until we can send
	do {
		asm volatile("nop");
	} while (*UART0_FR & 0x20);

	// Write the chartacter to the buffer
	*UART0_DR = c;
}

char uart_recv() {
	// Wait until something is in the buffer
	do {
		asm volatile("nop");
	} while (*UART0_FR & 0x10);

	char c = (char)*UART0_DR;
	// Convert carriage return to newline
	return c == '\r' ? '\n' : c;
}
