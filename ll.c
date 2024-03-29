#include "ll.h"
#include "at91sam3u1.h"

void SetDefaultMaster(unsigned char enable) {
  AT91PS_HMATRIX2 pMatrix = AT91C_BASE_MATRIX;

  // Set default master
  if (enable == 1) {
    // Set default master: SRAM0 -> Cortex-M3 System
    pMatrix->HMATRIX2_SCFG0 |= AT91C_MATRIX_FIXED_DEFMSTR_SCFG0_ARMS | AT91C_MATRIX_DEFMSTR_TYPE_FIXED_DEFMSTR;

    // Set default master: SRAM1 -> Cortex-M3 System
    pMatrix->HMATRIX2_SCFG1 |= AT91C_MATRIX_FIXED_DEFMSTR_SCFG1_ARMS | AT91C_MATRIX_DEFMSTR_TYPE_FIXED_DEFMSTR;

    // Set default master: Internal flash0 -> Cortex-M3 Instruction/Data
    pMatrix->HMATRIX2_SCFG3 |= AT91C_MATRIX_FIXED_DEFMSTR_SCFG3_ARMC | AT91C_MATRIX_DEFMSTR_TYPE_FIXED_DEFMSTR;
  } else {
    // Clear default master: SRAM0 -> Cortex-M3 System
    pMatrix->HMATRIX2_SCFG0 &= (~AT91C_MATRIX_DEFMSTR_TYPE);

    // Clear default master: SRAM1 -> Cortex-M3 System
    pMatrix->HMATRIX2_SCFG1 &= (~AT91C_MATRIX_DEFMSTR_TYPE);

    // Clear default master: Internal flash0 -> Cortex-M3 Instruction/Data
    pMatrix->HMATRIX2_SCFG3 &= (~AT91C_MATRIX_DEFMSTR_TYPE);
  }
}

void llinit(void) {
  unsigned int timeout = 0;

  /* Enable NRST reset*/
  AT91C_BASE_RSTC->RSTC_RMR |= (0xA5 << 24) | AT91C_RSTC_URSTEN;

  /* Set 3 WS for Embedded Flash Access */
  AT91C_BASE_EFC0->EFC_FMR = AT91C_EFC_FWS_3WS;

  /* Watchdog initialization */
  AT91C_BASE_WDTC->WDTC_WDMR = AT91C_WDTC_WDDIS;
#if 0 /* We have no 32K ext crystal soldered */
  /* Select external slow clock */
  if((AT91C_BASE_SUPC->SUPC_SR & AT91C_SUPC_SR_OSCSEL_CRYST) != AT91C_SUPC_SR_OSCSEL_CRYST) {
    AT91C_BASE_SUPC->SUPC_CR = AT91C_SUPC_CR_XTALSEL_CRYSTAL_SEL | (0xA5 << 24);
    timeout = 0;
    while (!(AT91C_BASE_SUPC->SUPC_SR & AT91C_SUPC_SR_OSCSEL_CRYST) && (timeout++ < CLOCK_TIMEOUT));
  }
#endif
  /* Initialize main oscillator */
  if(!(AT91C_BASE_PMC->PMC_MOR & AT91C_CKGR_MOSCSEL)) {
    AT91C_BASE_PMC->PMC_MOR = (0x37 << 16) | BOARD_OSCOUNT | AT91C_CKGR_MOSCRCEN | AT91C_CKGR_MOSCXTEN;
    timeout = 0;
    while (!(AT91C_BASE_PMC->PMC_SR & AT91C_PMC_MOSCXTS) && (timeout++ < CLOCK_TIMEOUT));
  }

  /* Switch to 3-20MHz Xtal oscillator */
  AT91C_BASE_PMC->PMC_MOR = (0x37 << 16) | BOARD_OSCOUNT | AT91C_CKGR_MOSCRCEN | AT91C_CKGR_MOSCXTEN | AT91C_CKGR_MOSCSEL;
  timeout = 0;
  while (!(AT91C_BASE_PMC->PMC_SR & AT91C_PMC_MOSCSELS) && (timeout++ < CLOCK_TIMEOUT));
  AT91C_BASE_PMC->PMC_MCKR = (AT91C_BASE_PMC->PMC_MCKR & ~AT91C_PMC_CSS) | AT91C_PMC_CSS_MAIN_CLK;
  timeout = 0;
  while (!(AT91C_BASE_PMC->PMC_SR & AT91C_PMC_MCKRDY) && (timeout++ < CLOCK_TIMEOUT));

  /* Initialize PLLA */
  AT91C_BASE_PMC->PMC_PLLAR = BOARD_PLLR;
  timeout = 0;
  while (!(AT91C_BASE_PMC->PMC_SR & AT91C_PMC_LOCKA) && (timeout++ < CLOCK_TIMEOUT));

  /* Initialize UTMI for USB usage, can be disabled if not using USB for the sake of saving power*/
  AT91C_BASE_CKGR->CKGR_UCKR |= (AT91C_CKGR_UPLLCOUNT & (3 << 20)) | AT91C_CKGR_UPLLEN;
  timeout = 0;
  while (!(AT91C_BASE_PMC->PMC_SR & AT91C_PMC_LOCKU) && (timeout++ < CLOCK_TIMEOUT));

  /* Switch to fast clock */
  AT91C_BASE_PMC->PMC_MCKR = (BOARD_MCKR & ~AT91C_PMC_CSS) | AT91C_PMC_CSS_MAIN_CLK;
  timeout = 0;
  while (!(AT91C_BASE_PMC->PMC_SR & AT91C_PMC_MCKRDY) && (timeout++ < CLOCK_TIMEOUT));

  AT91C_BASE_PMC->PMC_MCKR = BOARD_MCKR;
  timeout = 0;
  while (!(AT91C_BASE_PMC->PMC_SR & AT91C_PMC_MCKRDY) && (timeout++ < CLOCK_TIMEOUT));

  /* Enable clock for UART */
  AT91C_BASE_PMC->PMC_PCER = (1 << AT91C_ID_DBGU);

  /* Optimize CPU setting for speed, for engineering samples only */
  SetDefaultMaster(0);
}
