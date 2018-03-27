/**
***************************************************************************
* @file ncs36510_init.c
* @brief Initialization of Orion SoC
* @internal
* @author ON Semiconductor
* $Rev:
* $Date: $
******************************************************************************
 * Copyright 2016 Semiconductor Components Industries LLC (d/b/a “ON Semiconductor”).
 * All rights reserved.  This software and/or documentation is licensed by ON Semiconductor
 * under limited terms and conditions.  The terms and conditions pertaining to the software
 * and/or documentation are available at http://www.onsemi.com/site/pdf/ONSEMI_T&C.pdf
 * (“ON Semiconductor Standard Terms and Conditions of Sale, Section 8 Software”) and
 * if applicable the software license agreement.  Do not use this software and/or
 * documentation unless you have carefully read and you agree to the limited terms and
 * conditions.  By using this software and/or documentation, you agree to the limited
 * terms and conditions.
*
* THIS SOFTWARE IS PROVIDED "AS IS".  NO WARRANTIES, WHETHER EXPRESS, IMPLIED
* OR STATUTORY, INCLUDING, BUT NOT LIMITED TO, IMPLIED WARRANTIES OF
* MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE APPLY TO THIS SOFTWARE.
* ON SEMICONDUCTOR SHALL NOT, IN ANY CIRCUMSTANCES, BE LIABLE FOR SPECIAL,
* INCIDENTAL, OR CONSEQUENTIAL DAMAGES, FOR ANY REASON WHATSOEVER.
* @endinternal
*
* @ingroup main
*
* @details
*/

/*************************************************************************************************
*                                                                                                *
*  Header files                                                                                  *
*                                                                                                *
*************************************************************************************************/
#include "ncs36510Init.h"

void fPmuInit(void);

/**
 * @brief
 * Hardware trimming function
 * This function copies trim codes from specific flash location
 * where they are stored to proper hw registers.
 */
boolean fTrim()
{
    boolean status = False;

    /**- Check if trim values are present */
    /**- If Trim data is present.  Only trim if valid trim values are present. */
    /**- Copy trims in registers */
    if (TRIMREG->REVISION_CODE != 0xFFFFFFFF) {

        if ( TRIMREG->MAC_ADDR_LOW != 0xFFFFFFFF ) {
            MACHWREG->LONG_ADDRESS_LOW = TRIMREG->MAC_ADDR_LOW;
        }

        if ( TRIMREG->MAC_ADDR_HIGH != 0xFFFFFFFF ) {
            MACHWREG->LONG_ADDRESS_HIGH = TRIMREG->MAC_ADDR_HIGH;
        }

        /**- board specific clock trims may only be done when present, writing all 1's is not good */
        if ((TRIMREG->TRIM_32K_EXT & 0xFFFF0000) != 0xFFFF0000) {
            CLOCKREG->TRIM_32K_EXT.WORD = TRIMREG->TRIM_32K_EXT;
        }

        if ((TRIMREG->TRIM_32M_EXT & 0xFFFF0000) != 0xFFFF0000) {
            CLOCKREG->TRIM_32M_EXT.WORD = TRIMREG->TRIM_32M_EXT;
        }

        MACHWREG->TX_LENGTH.BITS.TX_PRE_CHIPS = TRIMREG->TX_PRE_CHIPS;

        if ((TRIMREG->TX_TRIM & 0xFFFF0000) != 0xFFFF0000) {
            RFANATRIMREG->TX_TRIM.WORD = TRIMREG->TX_TRIM;
        }
        RFANATRIMREG->PLL_VCO_TAP_LOCATION = TRIMREG->PLL_VCO_TAP_LOCATION;
        RFANATRIMREG->PLL_TRIM.WORD = TRIMREG->PLL_TRIM;

        /**- board specific RSSI trims may only be done when present, writing all 1's is not good */
        if ((TRIMREG->RSSI_OFFSET & 0xFFFF0000) != 0xFFFF0000) {
            DMDREG->DMD_CONTROL2.BITS.RSSI_OFFSET = TRIMREG->RSSI_OFFSET;
        }

        RFANATRIMREG->RX_CHAIN_TRIM = TRIMREG->RX_CHAIN_TRIM;
        RFANATRIMREG->PMU_TRIM = TRIMREG->PMU_TRIM;
        RANDREG->WR_SEED_RD_RAND = TRIMREG->WR_SEED_RD_RAND;

        /* High side injection settings */
        RFANATRIMREG->RX_VCO_TRIM_LUT1 = TRIMREG->RX_VCO_LUT1.WORD;;
        RFANATRIMREG->RX_VCO_TRIM_LUT2 = TRIMREG->RX_VCO_LUT2.WORD;;

        RFANATRIMREG->TX_VCO_TRIM_LUT1 = TRIMREG->TX_VCO_LUT1.WORD;;
        RFANATRIMREG->TX_VCO_TRIM_LUT2 = TRIMREG->TX_VCO_LUT2.WORD;;

        status = True;

    } else {

        return(False);
    }

    /** Read in user trim values programmed in the flash memory
    The user trim values take precedence over factory trim for MAC address
    */
    if (( USERTRIMREG->MAC_ADDRESS_LOW != 0xFFFFFFFF ) &&
            (USERTRIMREG->MAC_ADDRESS_HIGH != 0xFFFFFFFF)) {

        MACHWREG->LONG_ADDRESS_LOW = USERTRIMREG->MAC_ADDRESS_LOW;
        MACHWREG->LONG_ADDRESS_HIGH = USERTRIMREG->MAC_ADDRESS_HIGH;
    }

    if (USERTRIMREG->TRIM_32K_EXT != 0xFFFFFFFF) {
        CLOCKREG->TRIM_32K_EXT.WORD = (USERTRIMREG->TRIM_32K_EXT & 0x00000FFF);
    }

    if (USERTRIMREG->TRIM_32K_EXT != 0xFFFFFFFF) {
        CLOCKREG->TRIM_32K_EXT.WORD = (USERTRIMREG->TRIM_32K_EXT & 0x00000FFF);
    }

    if (USERTRIMREG->RSSI_OFFSET != 0xFFFFFFFF) {
        DMDREG->DMD_CONTROL2.BITS.RSSI_OFFSET = (USERTRIMREG->RSSI_OFFSET & 0x0000003F);
    }

    if (USERTRIMREG->TX_TRIM != 0xFFFFFFFF) {
        RFANATRIMREG->TX_TRIM.BITS.TX_TUNE = (USERTRIMREG->TX_TRIM & 0x0000000F);
    }
    return(status);
}

/* See clock.h for documentation. */
void fClockInit()
{

    /** Enable external 32MHz oscillator */
    CLOCKREG->CCR.BITS.OSC_SEL = 1;

    /** - Wait external 32MHz oscillator to be ready */
    while(CLOCKREG->CSR.BITS.XTAL32M != 1) {} /* If you get stuck here, something is wrong with board or trim values */

    /** Internal 32MHz calibration \n *//** - Enable internal 32MHz clock */
    PMUREG->CONTROL.BITS.INT32M = 0;

    /** - Wait 5 uSec for clock to stabilize */
    volatile uint8_t Timer;
    for(Timer = 0; Timer < 10; Timer++);

    /** - Enable calibration */
    CLOCKREG->CCR.BITS.CAL32M = True;

    /** - Wait calibration to be completed */
    while(CLOCKREG->CSR.BITS.CAL32MDONE == False); /* If you stuck here, issue with internal 32M calibration */

    /** - Check calibration status */
    while(CLOCKREG->CSR.BITS.CAL32MFAIL == True); /* If you stuck here, issue with internal 32M calibration */

    /** - Power down internal 32MHz osc */
    PMUREG->CONTROL.BITS.INT32M = 1;

    /** Internal 32KHz calibration \n */ /** - Enable internal 32KHz clock */
    PMUREG->CONTROL.BITS.INT32K = 0;

    /** - Wait 5 uSec for clock to stabilize */
    for(Timer = 0; Timer < 10; Timer++);

    /** - Enable calibration */
    CLOCKREG->CCR.BITS.CAL32K = True;

    /** - Wait calibration to be completed */
    while(CLOCKREG->CSR.BITS.DONE32K == False); /* If you stuck here, issue with internal 32K calibration */

    /** - Check calibration status */
    while(CLOCKREG->CSR.BITS.CAL32K == True); /* If you stuck here, issue with internal 32M calibration */

    /** - Power down external 32KHz osc */
    PMUREG->CONTROL.BITS.EXT32K = 1;

    /** Disable all peripheral clocks by default */
    CLOCKREG->PDIS.WORD = 0xFFFFFFFF;

    /** Set core frequency */
    CLOCKREG->FDIV = CPU_CLOCK_DIV - 1;
}

/* Initializes PMU module */
void fPmuInit()
{
    /** Enable the clock for PMU peripheral device */
    CLOCK_ENABLE(CLOCK_PMU);

    /** Unset wakeup on pending (only enabled irq can wakeup) */
    SCB->SCR &= ~SCB_SCR_SEVONPEND_Msk;

    /** Unset auto sleep when returning from wakeup irq */
    SCB->SCR &= ~SCB_SCR_SLEEPONEXIT_Msk;

    /** Set regulator timings */
    PMUREG->FVDD_TSETTLE    = 160;
    PMUREG->FVDD_TSTARTUP   = 400;


    /** Keep SRAMA & SRAMB powered in coma mode */
    PMUREG->CONTROL.BITS.SRAMA = False;
    PMUREG->CONTROL.BITS.SRAMB = False;

    PMUREG->CONTROL.BITS.N1V1 = True;   /* Enable ACTIVE mode switching regulator */
    PMUREG->CONTROL.BITS.C1V1 = True;   /* Enable COMA mode switching regulator */

    /** Disable the clock for PMU peripheral device, all settings are done */
    CLOCK_DISABLE(CLOCK_PMU);
}

/* See clock.h for documentation. */
uint32_t fClockGetPeriphClockfrequency()
{
    return (CPU_CLOCK_ROOT_HZ / CPU_CLOCK_DIV);
}


/**
* @brief
* Hardware initialization function
* This function initializes hardware at application start up prior
* to other initializations or OS operations.
*/
static void fHwInit(void)
{

    /* Trim register settings */
    fTrim();

    /* Clock setting */
    /** - Initialize clock */
    fClockInit();

    /** - Initialize pmu */
    fPmuInit();

    /** Orion has 4 interrupt bits in interrupt priority register
    * The lowest 4 bits are not used.
    *
    @verbatim
    +-----+-----+-----+-----+-----+-----+-----+-----+
    |bit 7|bit 6|bit 5|bit 4|bit 3|bit 2|bit 1|bit 0|
    |     |     |     |     |  0  |  0  |  0  |  0  |
    +-----+-----+-----+-----+-----+-----+-----+-----+
                            |
       INTERRUPT PRIORITY   |   NOT IMPLEMENTED,
                            |    read as 0
    Valid priorities are 0x00, 0x10, 0x20, 0x30
                         0x40, 0x50, 0x60, 0x70
                         0x80, 0x90, 0xA0, 0xB0
                         0xC0, 0xD0, 0xE0, 0xF0
    @endverbatim
    * Lowest number is highest priority
    *
    *
    * This range is defined by
    * configKERNEL_INTERRUPT_PRIORITY (lowest)
    * and configMAX_SYSCALL_INTERRUPT_PRIORITY (highest).  All interrupt
    * priorities need to fall in that range.
    *
    * To be future safe, the LSbits of the priority are set to 0xF.
    * This wil lmake sure that if more interrupt bits are used, the
    * priority is maintained.
    */

    /** - Set IRQs priorities */
    NVIC_SetPriority(Tim0_IRQn, 14);
    NVIC_SetPriority(Tim1_IRQn, 14);
    NVIC_SetPriority(Tim2_IRQn, 14);
    NVIC_SetPriority(Uart1_IRQn,14);
    NVIC_SetPriority(Spi_IRQn, 14);
    NVIC_SetPriority(I2C_IRQn, 14);
    NVIC_SetPriority(Gpio_IRQn, 14);
    NVIC_SetPriority(Rtc_IRQn, 14);
    NVIC_SetPriority(MacHw_IRQn, 13);
    NVIC_SetPriority(Aes_IRQn, 13);
    NVIC_SetPriority(Adc_IRQn, 14);
    NVIC_SetPriority(ClockCal_IRQn, 14);
    NVIC_SetPriority(Uart2_IRQn, 14);
    NVIC_SetPriority(Dma_IRQn, 14);
    NVIC_SetPriority(Uvi_IRQn, 14);
    NVIC_SetPriority(DbgPwrUp_IRQn, 14);
    NVIC_SetPriority(Spi2_IRQn, 14);
    NVIC_SetPriority(I2C2_IRQn, 14);
}

extern void __Vectors;

void fNcs36510Init(void)
{
    /** Setting this register is helping to debug imprecise bus access faults
    * making them precise bus access faults. It has an impact on application
    * performance. */
    // SCnSCB->ACTLR |= SCnSCB_ACTLR_DISDEFWBUF_Msk;

    /** This main function implements: */
    /**- Disable all interrupts */
    NVIC->ICER[0] = 0x1F;

    /**- Clear all Pending interrupts */
    NVIC->ICPR[0] = 0x1F;

    /**- Clear all pending SV and systick */
    SCB->ICSR = (uint32_t)0x0A000000;
    SCB->VTOR = (uint32_t) (&__Vectors);

    /**- Initialize hardware */
    fHwInit();
}
