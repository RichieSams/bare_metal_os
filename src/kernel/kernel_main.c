#include "arch/uart.h"
#include "printf.h"


void kernel_main() {
    uart_init();
    
    printf("Hello World!\n");

    while (1) {
        uart_send(uart_recv());
    }
}
