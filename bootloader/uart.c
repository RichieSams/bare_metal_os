#include "arch/uart.h"

#include "arch/mmio.h"

/*
 * Primary UART(16C650) register map
 */
#define UART0_BASE (MMIO_BASE | 0x201000)

#define UART0_DATA         (UART0_BASE + 0x00)
#define RX_DATA            (0xFF << 0)
#define DR_FRAME_ERROR     (1 << 8)
#define DR_PARITY_ERROR    (1 << 9)
#define DR_BREAK_ERROR     (1 << 10)
#define DR_OVERRUN_ERROR   (1 << 11)
#define DATA_ERROR         (DR_FRAME_ERROR | DR_PARITY_ERROR | DR_BREAK_ERROR | DR_OVERRUN_ERROR)
#define UART0_RX_STATUS    (UART0_BASE + 0x04)
#define RX_FRAME_ERROR     (1 << 0)
#define RX_PARITY_ERROR    (1 << 1)
#define RX_BREAK_ERROR     (1 << 2)
#define RX_OVERRUN_ERROR   (1 << 3)
#define RX_ERROR           (RX_FRAME_ERROR | RX_PARITY_ERROR | RX_BREAK_ERROR | RX_OVERRUN_ERROR)
#define UART0_STATUS       (UART0_BASE + 0x18)
#define CLEAR_TO_SEND      (1 << 0)
#define BUSY               (1 << 3)
#define RX_FIFO_EMPTY      (1 << 4)
#define TX_FIFO_FULL       (1 << 5)
#define RX_FIFO_FULL       (1 << 6)
#define TX_FIFO_EMPTY      (1 << 7)
#define UART0_ILPR         (UART0_BASE + 0x20)
#define UART0_IBRD         (UART0_BASE + 0x24)
#define BAUD_DIVISOR       (0xFFFF << 0)
#define UART0_FBRD         (UART0_BASE + 0x28)
#define FRACTIONAL_DIVISOR (0x1F << 0)
#define UART0_LINE_CTRL    (UART0_BASE + 0x2C)
#define BREAK              (1 << 0)
#define PARITY             (1 << 1)
#define EVEN_PARITY        (1 << 2)
#define TWO_STOP_BITS      (1 << 3)
#define ENABLE_FIFO        (1 << 4)
#define BYTE_WORD_LENGTH   (3 << 5)
#define STICK_PARITY       (1 << 7)
#define UART0_CONTROL      (UART0_BASE + 0x30)
#define ENABLE             (1 << 0)
#define LOOPBACK           (1 << 7)
#define TX_ENABLE          (1 << 8)
#define RX_ENABLE          (1 << 9)
#define RTS                (1 << 11)
#define RTS_FLOW_CONTROL   (1 << 14)
#define CTS_FLOW_CONTROL   (1 << 15)
#define UART0_IFLS         (UART0_BASE + 0x34)
#define UART0_IMSC         (UART0_BASE + 0x38)
#define UART0_RIS          (UART0_BASE + 0x3C)
#define UART0_MIS          (UART0_BASE + 0x40)
#define UART0_ICR          (UART0_BASE + 0x44)
#define UART0_DMACR        (UART0_BASE + 0x48)
#define UART0_ITCR         (UART0_BASE + 0x80)
#define UART0_ITIP         (UART0_BASE + 0x84)
#define UART_RTS           (1 << 0)
#define UART_CTS           (1 << 3)
#define UART0_ITOP         (UART0_BASE + 0x88)
#define UART0_TDR          (UART0_BASE + 0x8C)

#define RX_BUFFER_MASK 0xFFF

void wait_cycles(unsigned int n) {
	for (unsigned int i = 0; i < n; ++i) {
		asm volatile("nop");
	}
}

void uart_send(char c) {
	// Convert newline to carriage return + newline
	if (c == '\n') {
		uart_send('\r');
	}

	// Loop until UART transmission line has room to send
	while (mmio_load(UART0_STATUS) & TX_FIFO_FULL) {
		// Nop
	}

	// Send the character
	mmio_store(UART0_DATA, c);
}

char uart_recv(void) {
	// Loop until UART Rx FIFO is no longer empty
	while (mmio_load(UART0_STATUS) & RX_FIFO_EMPTY) {
		// Nop
	}
	// Read the character
	char c = mmio_load(UART0_DATA);

	// Read Rx status to acknowledge character and check for error
	uint32_t status = mmio_load(UART0_RX_STATUS);

	// Check for and clear any read status errors
	if (status & RX_ERROR)
		mmio_store(UART0_RX_STATUS, status);

	// Return the character read
	return (RX_DATA & c);
}

void uart_init(void) {
	// Disable the UART before configuring
	mmio_store(UART0_LINE_CTRL, 0);
	mmio_store(UART0_CONTROL, 0);

	// Select Alternate 0 for UART over GPIO pins 14 Tx and 15 Rx.*/
	uint32_t gpio = mmio_load(GPFSEL1);
	gpio &= ~(7 << 12);      // clear GPIO 14
	gpio |= GPIO_ALT0 << 12; // set GPIO 14 to Alt 0
	gpio &= ~(7 << 15);      // clear GPIO 15
	gpio |= GPIO_ALT0 << 15; // set GPIO 15 to Alt 0
	mmio_store(GPFSEL1, gpio);

	// UART is GPIO 14 and 15 with 2 bits per PUP GPIO
	gpio = mmio_load(GPIO_PUP_PDN_CNTRL_REG0);
	gpio &= ~(3 << (14 * 2));                  // Clear GPIO 14 value
	gpio &= ~(3 << (15 * 2));                  // Clear GPIO 15 value
	mmio_store(GPIO_PUP_PDN_CNTRL_REG0, gpio); // commit changes

	// Set the baud rate
	// RPI 3/4 has a 48MHz clock as it was intended to work with BT
	// (48000000 / (16 * 115200) = 26.042
	// (0.042*64)+0.5 = 3
	// 115200 baud is int 26 frac 3
	mmio_store(UART0_IBRD, 26); // Integer baud rate divisor
	mmio_store(UART0_FBRD, 3);  // Fractional baud rate divisor

	// Enable the UART and clear the received error count
	mmio_store(UART0_LINE_CTRL, BYTE_WORD_LENGTH | ENABLE_FIFO);
	mmio_store(UART0_CONTROL, ENABLE | TX_ENABLE | RX_ENABLE);
	return;
}
