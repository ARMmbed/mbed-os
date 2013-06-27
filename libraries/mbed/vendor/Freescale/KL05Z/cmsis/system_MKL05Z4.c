#include <stdint.h>
#include "MKL05Z4.h"

#define DISABLE_WDOG    1

#define CLOCK_SETUP     0
/* Predefined clock setups
   0 ... Multipurpose Clock Generator (MCG) in FLL Engaged Internal (FEI) mode
         Reference clock source for MCG module is the slow internal clock source 32.768kHz
         Core clock = 47.97MHz, BusClock = 23.48MHz
*/

/*----------------------------------------------------------------------------
  Define clock source values
 *----------------------------------------------------------------------------*/
#if (CLOCK_SETUP == 0)
    #define CPU_XTAL_CLK_HZ                 32768u    /* Value of the external crystal or oscillator clock frequency in Hz */
    #define CPU_INT_SLOW_CLK_HZ             32768u    /* Value of the slow internal oscillator clock frequency in Hz  */
    #define CPU_INT_FAST_CLK_HZ             4000000u  /* Value of the fast internal oscillator clock frequency in Hz  */
    #define DEFAULT_SYSTEM_CLOCK            47972352u /* Default System clock value */
#endif /* (CLOCK_SETUP == 0) */


/* ----------------------------------------------------------------------------
   -- Core clock
   ---------------------------------------------------------------------------- */

uint32_t SystemCoreClock = DEFAULT_SYSTEM_CLOCK;

/* ----------------------------------------------------------------------------
   -- SystemInit()
   ---------------------------------------------------------------------------- */

void SystemInit(void) {
#if (DISABLE_WDOG)
    /* Disable the WDOG module */
    /* SIM_COPC: COPT=0,COPCLKS=0,COPW=0 */
    SIM->COPC = (uint32_t)0x00u;
#endif /* (DISABLE_WDOG) */

    SIM->SCGC5 |= (SIM_SCGC5_PORTB_MASK | SIM_SCGC5_PORTA_MASK); /* Enable clock gate for ports to enable pin routing */
    /* SIM_SCGC5: LPTMR=1 */
    SIM->SCGC5 |= SIM_SCGC5_LPTMR_MASK;
    /* SIM_CLKDIV1: OUTDIV1=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,OUTDIV4=1,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0 */
    SIM->CLKDIV1 = (SIM_CLKDIV1_OUTDIV1(0x00) | SIM_CLKDIV1_OUTDIV4(0x01)); /* Update system prescalers */
    /* SIM_SOPT1: OSC32KSEL=0 */
    SIM->SOPT1 &= (uint32_t)~(uint32_t)(SIM_SOPT1_OSC32KSEL(0x03)); /* System oscillator drives 32 kHz clock for various peripherals */
    /* SIM_SOPT2: TPMSRC=2 */
    SIM->SOPT2 = (uint32_t)((SIM->SOPT2 & (uint32_t)~(uint32_t)(
                 SIM_SOPT2_TPMSRC(0x01)
                )) | (uint32_t)(
                 SIM_SOPT2_TPMSRC(0x02)
                ));                      /* Set the TPM clock */
    /* PORTA_PCR3: ISF=0,MUX=0 */
    PORTA->PCR[3] &= (uint32_t)~(uint32_t)((PORT_PCR_ISF_MASK | PORT_PCR_MUX(0x07)));
    /* MCG_SC: FCRDIV=1 */
    MCG->SC = (uint8_t)((MCG->SC & (uint8_t)~(uint8_t)(
              MCG_SC_FCRDIV(0x06)
             )) | (uint8_t)(
              MCG_SC_FCRDIV(0x01)
             ));
    /* Switch to FEI Mode */
    /* MCG_C1: CLKS=0,FRDIV=0,IREFS=1,IRCLKEN=1,IREFSTEN=0 */
    MCG->C1 = MCG_C1_CLKS(0x00) |
             MCG_C1_FRDIV(0x00) |
             MCG_C1_IREFS_MASK |
             MCG_C1_IRCLKEN_MASK;
    /* MCG_C2: LOCRE0=0,??=0,RANGE0=0,HGO0=0,EREFS0=0,LP=0,IRCS=1 */
    MCG->C2 = (MCG_C2_RANGE0(0x00) | MCG_C2_IRCS_MASK);
    /* MCG_C4: DMX32=1,DRST_DRS=1 */
    MCG->C4 = (uint8_t)((MCG->C4 & (uint8_t)~(uint8_t)(
              MCG_C4_DRST_DRS(0x02)
             )) | (uint8_t)(
              MCG_C4_DMX32_MASK |
              MCG_C4_DRST_DRS(0x01)
             ));
    /* OSC0_CR: ERCLKEN=1,??=0,EREFSTEN=0,??=0,SC2P=0,SC4P=0,SC8P=0,SC16P=0 */
    OSC0->CR = OSC_CR_ERCLKEN_MASK;
    while((MCG->S & MCG_S_IREFST_MASK) == 0x00U) { /* Check that the source of the FLL reference clock is the internal reference clock. */
    }
    while((MCG->S & 0x0CU) != 0x00U) {    /* Wait until output of the FLL is selected */
    }
}

// Make sure we are pulling in the retargeting module at link time
extern int stdio_retargeting_module;

/* ----------------------------------------------------------------------------
   -- SystemCoreClockUpdate()
   ---------------------------------------------------------------------------- */

void SystemCoreClockUpdate(void) {
/* TODO */
}
