#include "hw.h"

void dbgu_init(unsigned int mode, unsigned int bd, unsigned int mck) {
  /* DBGU pins (DTXD and DRXD) definitions, PA11,12 */
  unsigned int pio_mask = (1U << 11U) | (1U << 12U);
  unsigned int abmr;

  AT91C_BASE_PMC->PMC_PCER = (1 << AT91C_ID_PIOA);
  AT91C_BASE_PIOA->PIO_IDR = pio_mask;
  AT91C_BASE_PIOA->PIO_PPUDR = pio_mask; // Pull-up Dis. or PIO_PPUDR to Enable

  abmr = AT91C_BASE_PIOA->PIO_ABSR;
  AT91C_BASE_PIOA->PIO_ABSR &= (~pio_mask & abmr);
  AT91C_BASE_PIOA->PIO_PDR = pio_mask;

  AT91C_BASE_PMC->PMC_PCER = (1 << AT91C_ID_DBGU);

  /* Reset & disable receiver and transmitter, disable interrupts */
  AT91C_BASE_DBGU->DBGU_CR = AT91C_US_RSTRX | AT91C_US_RSTTX;
  AT91C_BASE_DBGU->DBGU_IDR = 0xFFFFFFFF;

  AT91C_BASE_DBGU->DBGU_BRGR = mck / (bd * 16);

  AT91C_BASE_DBGU->DBGU_MR = mode;

  /* Disable DMA channel */
  AT91C_BASE_DBGU->DBGU_PTCR = AT91C_PDC_RXTDIS | AT91C_PDC_TXTDIS;

  /* Enable receiver and transmitter */
  AT91C_BASE_DBGU->DBGU_CR = AT91C_US_RXEN | AT91C_US_TXEN;
}

void dbgu_putchar(unsigned char c) {
  /* Wait for the transmitter to be ready */
  while ((AT91C_BASE_DBGU->DBGU_CSR & AT91C_US_TXEMPTY) == 0);
  AT91C_BASE_DBGU->DBGU_THR = c;
  /* Wait for the transfer to complete */
  while ((AT91C_BASE_DBGU->DBGU_CSR & AT91C_US_TXEMPTY) == 0);
}

unsigned int dbgu_rx_ready(void) {
  return (AT91C_BASE_DBGU->DBGU_CSR & AT91C_US_RXRDY);
}

unsigned int dbgu_getchar(void) {
  while ((AT91C_BASE_DBGU->DBGU_CSR & AT91C_US_RXRDY) == 0);
  return AT91C_BASE_DBGU->DBGU_RHR;
}
