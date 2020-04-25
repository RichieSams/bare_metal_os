#include "arch/mbox.h"
#include "arch/gpio.h"

volatile unsigned int  __attribute__((aligned(16))) mbox[36];

#define VIDEOCORE_MBOX  (MMIO_BASE+0x0000B880)
#define MBOX_READ       ((volatile unsigned int*)(VIDEOCORE_MBOX+0x0))
#define MBOX_POLL       ((volatile unsigned int*)(VIDEOCORE_MBOX+0x10))
#define MBOX_SENDER     ((volatile unsigned int*)(VIDEOCORE_MBOX+0x14))
#define MBOX_STATUS     ((volatile unsigned int*)(VIDEOCORE_MBOX+0x18))
#define MBOX_CONFIG     ((volatile unsigned int*)(VIDEOCORE_MBOX+0x1C))
#define MBOX_WRITE      ((volatile unsigned int*)(VIDEOCORE_MBOX+0x20))
#define MBOX_RESPONSE   0x80000000
#define MBOX_FULL       0x80000000
#define MBOX_EMPTY      0x40000000


// Reference: https://github.com/raspberrypi/documentation/blob/JamesH65-mailbox_docs/configuration/mailboxes/accessing.md


/**
 * Make a mailbox call
 * 
 * @return    0 on failure. Non-zero on success
 */
int mbox_call(unsigned char ch) {
    // Mailbox expects the address of the data to be in the upper 28 bits
    // and the channel id to be in the lower 4 bits

    // We truncate address to 32 bits, by assuming bss will be in a 32 bit addressable section
    // We can truncate the trailing 4 bits because it requires 16 bit alignment
    const unsigned int addr = (unsigned long)&mbox & ~0xF;
    unsigned int data = (unsigned int)addr | (ch & 0xF);
    
    // Wait until we can write to the mailbox
    do {
        asm volatile("nop");
    } while (*MBOX_STATUS & MBOX_FULL);

    // Write to the register
    *MBOX_WRITE = data;

    // Wait for a response
    while(1) {
        do {
            asm volatile("nop");
        } while (*MBOX_STATUS & MBOX_EMPTY);

        // Check if the response for our message
        if (*MBOX_READ == data) {
            // Check for success
            return mbox[1] == MBOX_RESPONSE;
        }
    }
}
