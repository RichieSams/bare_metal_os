#pragma once

#include <stdint.h>

void wait_cycles(unsigned int n);

void sys_timer3_init();
void sys_timer3_term();

/**
 * @brief Set the timer to fire every *interval_ms* milliseconds
 * 
 * @param interval_ms    The interval to fire an IRQ in milliseconds
 */
void sys_timer3_set(uint32_t interval_ms);
