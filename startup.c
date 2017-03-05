#include "at91sam3u1.h"
#include "ll.h"
#include "irqs.h"

typedef void( *IntFunc )( void );

extern unsigned int _estack;

extern unsigned int _vect_start;

extern unsigned int _sfixed;
extern unsigned int _sfixed;
extern unsigned int _efixed;
extern unsigned int _srelocate;
extern unsigned int _erelocate;
extern unsigned int _szero;
extern unsigned int _ezero;

extern int main(void);
void ResetException(void);

__attribute__((section(".vectors")))
IntFunc exception_table[] = {

    // Configure Initial Stack Pointer, using linker-generated symbols
    (IntFunc)&_estack,
    ResetException,

    NMI_Handler,
    HardFault_Handler,
    MemManage_Handler,
    BusFault_Handler,
    UsageFault_Handler,
    0, 0, 0, 0,             // Reserved
    SVC_Handler,
    DebugMon_Handler,
    0,                      // Reserved
    PendSV_Handler,
    SysTick_Handler,

    // Configurable interrupts
    SUPC_IrqHandler,    // 0  SUPPLY CONTROLLER
    RSTC_IrqHandler,    // 1  RESET CONTROLLER
    RTC_IrqHandler,     // 2  REAL TIME CLOCK
    RTT_IrqHandler,     // 3  REAL TIME TIMER
    WDT_IrqHandler,     // 4  WATCHDOG TIMER
    PMC_IrqHandler,     // 5  PMC
    EFC0_IrqHandler,    // 6  EFC0
    EFC1_IrqHandler,    // 7  EFC1
    DBGU_IrqHandler,    // 8  DBGU
    HSMC4_IrqHandler,   // 9  HSMC4
    PIOA_IrqHandler,    // 10 Parallel IO Controller A
    PIOB_IrqHandler,    // 11 Parallel IO Controller B
    PIOC_IrqHandler,    // 12 Parallel IO Controller C
    USART0_IrqHandler,  // 13 USART 0
    USART1_IrqHandler,  // 14 USART 1
    USART2_IrqHandler,  // 15 USART 2
    USART3_IrqHandler,  // 16 USART 3
    MCI0_IrqHandler,    // 17 Multimedia Card Interface
    TWI0_IrqHandler,    // 18 TWI 0
    TWI1_IrqHandler,    // 19 TWI 1
    SPI0_IrqHandler,    // 20 Serial Peripheral Interface
    SSC0_IrqHandler,    // 21 Serial Synchronous Controller 0
    TC0_IrqHandler,     // 22 Timer Counter 0
    TC1_IrqHandler,     // 23 Timer Counter 1
    TC2_IrqHandler,     // 24 Timer Counter 2
    PWM_IrqHandler,     // 25 Pulse Width Modulation Controller
    ADCC0_IrqHandler,   // 26 ADC controller0
    ADCC1_IrqHandler,   // 27 ADC controller1
    HDMA_IrqHandler,    // 28 HDMA
    USBD_IrqHandler,    // 29 USB Device High Speed UDP_HS
    IrqHandlerNotUsed   // 30 not used
};

void ResetException(void) {
    unsigned int *pSrc, *pDest;

    LowLevelInit();

    // Initialize data
    pSrc = &_efixed;
    pDest = &_srelocate;
    if (pSrc != pDest) {
        for(; pDest < &_erelocate;) {
            *pDest++ = *pSrc++;
        }
    }

    // Zero fill bss
    for(pDest = &_szero; pDest < &_ezero;) {
        *pDest++ = 0;
    }

    pSrc = (unsigned int *)&_sfixed;

    AT91C_BASE_NVIC->NVIC_VTOFFR = ((unsigned int)(pSrc)) | (0x0 << 7);

    main();
}
