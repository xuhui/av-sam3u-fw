#ifndef IRQS_H
#define IRQS_H

void NMI_Handler(void);
void HardFault_Handler(void);
void MemManage_Handler(void);
void BusFault_Handler(void);
void UsageFault_Handler(void);
void SVC_Handler(void);
void DebugMon_Handler(void);
void PendSV_Handler(void);
void SysTick_Handler(void);

void SUPC_IrqHandler(void);
void RSTC_IrqHandler(void);
void RTC_IrqHandler(void);
void RTT_IrqHandler(void);
void WDT_IrqHandler(void);
void PMC_IrqHandler(void);
void EFC0_IrqHandler(void);
void EFC1_IrqHandler(void);
void DBGU_IrqHandler(void);
void HSMC4_IrqHandler(void);
void PIOA_IrqHandler(void);
void PIOB_IrqHandler(void);
void PIOC_IrqHandler(void);
void USART0_IrqHandler(void);
void USART1_IrqHandler(void);
void USART2_IrqHandler(void);
void USART3_IrqHandler(void);
void MCI0_IrqHandler(void);
void TWI0_IrqHandler(void);
void TWI1_IrqHandler(void);
void SPI0_IrqHandler(void);
void SSC0_IrqHandler(void);
void TC0_IrqHandler(void);
void TC1_IrqHandler(void);
void TC2_IrqHandler(void);
void PWM_IrqHandler(void);
void ADCC0_IrqHandler(void);
void ADCC1_IrqHandler(void);
void HDMA_IrqHandler(void);
void USBD_IrqHandler(void);
void IrqHandlerNotUsed(void);

#endif
