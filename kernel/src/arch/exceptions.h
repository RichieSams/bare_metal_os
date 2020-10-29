#pragma once

void exception_handler_init();

void enable_debug_exceptions();
void disable_debug_exceptions();

void enable_serror();
void disable_serror();

void enable_irq();
void disable_irq();

void enable_fiq();
void disable_fiq();

typedef void (*IRQHandler)(void *param);
void connect_irq(unsigned nIRQ, IRQHandler handler, void *param);
void disconnect_irq(unsigned nIRQ);
