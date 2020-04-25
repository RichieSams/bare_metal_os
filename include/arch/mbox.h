#pragma once

extern volatile unsigned int mbox[36];

#define MBOX_REQUEST    0

// Channels
#define MBOX_CH_POWER   0
#define MBOX_CH_FB      1
#define MBOX_CH_VUART   2
#define MBOX_CH_VCHIQ   3
#define MBOX_CH_LEDS    4
#define MBOX_CH_BTNS    5
#define MBOX_CH_TOUCH   6
#define MBOX_CH_COUNT   7
#define MBOX_CH_PROP    8

// Tags
#define MBOX_TAG_GET_MODEL      0x10001
#define MBOX_TAG_GET_REV        0x10002
#define MBOX_TAG_GET_MAC        0x10003
#define MBOX_TAG_GET_SERIAL     0x10004
#define MBOX_TAG_SET_CLK_RATE   0x38002

#define MBOX_TAG_LAST           0

int mbox_call(unsigned char ch);
