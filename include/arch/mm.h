#pragma once

#define PAGE_SIZE    (1 << 12)
#define SECTION_SIZE (1 << 21)

#define STACK_START (2 * SECTION_SIZE)

#define CPU1_RESUME_ADDR (0xe0)
#define CPU2_RESUME_ADDR (0xe8)
#define CPU3_RESUME_ADDR (0xf0)
