#include "at91sam3u1.h"

int main(void) {
  unsigned int mask = (1 << 23);

  /* Enable clk PIOA */
  AT91C_BASE_PMC->PMC_PCER = (1 << AT91C_ID_PIOA);

  // Disable interrupts on the pin(s)
  AT91C_BASE_PIOA->PIO_IDR = mask;

  // Disable pullup(s)
  AT91C_BASE_PIOA->PIO_PPUDR = mask;

  // Set default value
  AT91C_BASE_PIOA->PIO_SODR = mask;

  // Configure pin(s) as output(s)
  AT91C_BASE_PIOA->PIO_OER = mask;
  AT91C_BASE_PIOA->PIO_PER = mask;

  return 0;
}
