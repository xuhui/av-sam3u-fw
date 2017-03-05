#include <stdlib.h>
#include "at91sam3u1.h"
#include "hw.h"
#include "printf.h"

void dummy_delay(volatile unsigned int delay) {
  while(delay--);
}

int main(void) {
  unsigned int mask = (1 << 23);

  /* Enable clk PIOA */
  AT91C_BASE_PMC->PMC_PCER = (1 << AT91C_ID_PIOA);

  // Disable interrupts on the pin(s)
  AT91C_BASE_PIOA->PIO_IDR = mask;

  // Disable pullup(s)
  AT91C_BASE_PIOA->PIO_PPUDR = mask;

  // Set default value
  AT91C_BASE_PIOA->PIO_CODR = mask;

  // Configure pin(s) as output(s)
  AT91C_BASE_PIOA->PIO_OER = mask;
  AT91C_BASE_PIOA->PIO_PER = mask;

  dbgu_init(AT91C_US_PAR_NONE, 115200, BOARD_MCK);
  init_printf(NULL, dbgu_tiny_putc);

  tfp_printf("\r\nMCU started.\r\n");

  for(;;) {
    AT91C_BASE_PIOA->PIO_SODR = mask;
    dummy_delay(100000);
    AT91C_BASE_PIOA->PIO_CODR = mask;
    dummy_delay(100000);
  }

  return 0;
}
