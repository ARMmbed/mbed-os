/**
 *******************************************************************************
 * @file    sleep.c
 * @brief Implementation of an sleep functionality
 * @internal
 * @author  ON Semiconductor
 * $Rev:  0.1 $
 * $Date: 01-21-2016 $
 ******************************************************************************
 * @copyright (c) 2015 ON Semiconductor. All rights reserved.
 * ON Semiconductor is supplying this software for use with ON Semiconductor
 * processor based microcontrollers only.
 *
 * THIS SOFTWARE IS PROVIDED "AS IS".  NO WARRANTIES, WHETHER EXPRESS, IMPLIED
 * OR STATUTORY, INCLUDING, BUT NOT LIMITED TO, IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE APPLY TO THIS SOFTWARE.
 * ON SEMICONDUCTOR SHALL NOT, IN ANY CIRCUMSTANCES, BE LIABLE FOR SPECIAL,
 * INCIDENTAL, OR CONSEQUENTIAL DAMAGES, FOR ANY REASON WHATSOEVER.
 * @endinternal
 *
 * @ingroup sleep
 *
 * @details
 * Sleep implementation
 *
 */
#if DEVICE_SLEEP
#include "sleep.h"
#include "sleep_api.h"
#include "cmsis_nvic.h"

#define ENABLE			(uint8_t)0x01
#define DISABLE 		(uint8_t)0x00
#define MAC_LUT_SIZE	(uint8_t)96

#define portNVIC_SYSTICK_CTRL_REG			( * ( ( volatile unsigned long * ) 0xe000e010 ) )
#define portNVIC_SYSTICK_CLK_BIT			( 1UL << 2UL )
#define portNVIC_SYSTICK_INT_BIT			( 1UL << 1UL )
#define portNVIC_SYSTICK_ENABLE_BIT			( 1UL << 0UL )

void sleep(void)
{
	/** Unset SLEEPDEEP (SCR) and COMA to select sleep mode */
	SCB->SCR &= ~SCB_SCR_SLEEPDEEP_Msk;
	PMUREG->CONTROL.BITS.ENCOMA = DISABLE;

	/* Enter into sleep mode */
	__ISB();
	__WFI();
}

void deepsleep(void)
{
	/** Set SLEEPDEEP (SCR) and unset COMA to select deep sleep mode */
	SCB->SCR |= SCB_SCR_SLEEPDEEP_Msk;
	PMUREG->CONTROL.BITS.ENCOMA = DISABLE;

	/** Enter into deep sleep mode */
	__ISB();
	__WFI();

	/** Wait for the external 32MHz to be power-ed up & running
	 * Re-power down the 32MHz internal osc
	 */
	while (!CLOCKREG->CSR.BITS.XTAL32M);
	PMUREG->CONTROL.BITS.INT32M = 1;
}

void coma(void)
{
	/** Set SLEEPDEEP (SCR) and set COMA to select coma mode */
	SCB->SCR |= SCB_SCR_SLEEPDEEP_Msk;
	PMUREG->CONTROL.BITS.ENCOMA = ENABLE;

	/* TODO Wait till MAC is idle */
	//		while((MACHWREG->SEQUENCER == MACHW_SEQ_TX) || (MACHWREG->SEQUENCER == MACHW_SEQ_ED) || (MACHWREG->SEQUENCER == MACHW_SEQ_CCA));

	/* TODO Back up MAC_LUT *
	uint8_t MAC_LUT_BackUp[MAC_LUT_SIZE];
	fMacBackupFrameStoreLUT(MAC_LUT_BackUp); */

	/* Disable UART 1 & 2 FIFO during coma*/
	UART1REG->FCR.WORD &= ~(FCR_FIFO_ENABLE);
	UART2REG->FCR.WORD &= ~(FCR_FIFO_ENABLE);

	/** Enter into coma mode */
	__ISB();
	__WFI();

	/** Wait for the external 32MHz to be power-ed up & running
	 * Re-power down the 32MHz internal osc
	 */
	while (!CLOCKREG->CSR.BITS.XTAL32M);
	PMUREG->CONTROL.BITS.INT32M = 1;

	/** Trim the oscillators */
	if ((TRIMREG->TRIM_32K_EXT & 0xFFFF0000) != 0xFFFF0000) {
		CLOCKREG->TRIM_32K_EXT = TRIMREG->TRIM_32K_EXT;
	}
	if ((TRIMREG->TRIM_32M_EXT & 0xFFFF0000) != 0xFFFF0000) {
		CLOCKREG->TRIM_32M_EXT = TRIMREG->TRIM_32M_EXT;
	}

	/* Enable UART 1 & 2 FIFO */
	UART1REG->FCR.WORD |= FCR_FIFO_ENABLE;
	UART2REG->FCR.WORD |= FCR_FIFO_ENABLE;

	/* TODO Restore MAC_LUT *
	fMacRestoreFrameStoreLUT(MAC_LUT_BackUp);  */
}

#endif /* DEVICE_SLEEP */