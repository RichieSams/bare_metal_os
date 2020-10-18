#pragma once

static inline unsigned int get_core_id() {
	unsigned int coreId;

	asm volatile(
	    "mrs	%[coreId], mpidr_el1\n"
	    "and	%[coreId], %[coreId], #0x3"
	    : [ coreId ] "=r"(coreId));

	return coreId;
}

static inline unsigned int get_el() {
	unsigned int el;

	asm volatile(
	    "mrs	%[el], CurrentEL\n"
	    "lsr	%[el], %[el], #2" // Shift off the first two bits. They're always zero
	    : [ el ] "=r"(el));

	return el;
}

void reboot();
