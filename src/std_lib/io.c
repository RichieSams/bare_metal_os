#include "arch/uart.h"


void _putchar(char character) {
    uart_send(character);
}
