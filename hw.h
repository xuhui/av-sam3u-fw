#ifndef HW_H
#define HW_H

#include "at91sam3u1.h"

void dbgu_init(unsigned int mode, unsigned int bd, unsigned int mck);
void dbgu_putchar(unsigned char c);
unsigned int dbgu_rx_ready(void);
unsigned int dbgu_getchar(void);
void dbgu_tiny_putc(void* p, char c);

#endif
