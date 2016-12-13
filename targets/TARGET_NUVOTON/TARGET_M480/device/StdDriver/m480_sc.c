/**************************************************************************//**
 * @file     sc.c
 * @version  V3.00
 * @brief    M480 Smartcard(SC) driver source file
 *
 * @copyright (C) 2016 Nuvoton Technology Corp. All rights reserved.
 *****************************************************************************/
#include "M480.h"

// Below are variables used locally by SC driver and does not want to parse by doxygen unless HIDDEN_SYMBOLS is defined
/// @cond HIDDEN_SYMBOLS
static uint32_t u32CardStateIgnore[SC_INTERFACE_NUM] = {0, 0, 0};

/// @endcond HIDDEN_SYMBOLS

/** @addtogroup M480_Device_Driver M480 Device Driver
  @{
*/

/** @addtogroup M480_SC_Driver SC Driver
  @{
*/

/** @addtogroup M480_SC_EXPORTED_FUNCTIONS SC Exported Functions
  @{
*/

/**
  * @brief This function indicates specified smartcard slot status
  * @param[in] sc Base address of smartcard module
  * @return Card insert status
  * @retval TRUE Card insert
  * @retval FALSE Card remove
  */
uint32_t SC_IsCardInserted(SC_T *sc)
{
    // put conditions into two variable to remove IAR compilation warning
    uint32_t cond1 = ((sc->STATUS & SC_STATUS_CDPINSTS_Msk) >> SC_STATUS_CDPINSTS_Pos);
    uint32_t cond2 = ((sc->CTL & SC_CTL_CDLV_Msk) >> SC_CTL_CDLV_Pos);

    if(sc == SC0 && u32CardStateIgnore[0] == 1)
        return TRUE;
    else if(sc == SC1 && u32CardStateIgnore[1] == 1)
        return TRUE;
    else if(sc == SC2 && u32CardStateIgnore[2] == 1)
        return TRUE;
    else if(cond1 != cond2)
        return FALSE;
    else
        return TRUE;
}

/**
  * @brief This function reset both transmit and receive FIFO of specified smartcard module
  * @param[in] sc Base address of smartcard module
  * @return None
  */
void SC_ClearFIFO(SC_T *sc)
{
    sc->ALTCTL |= (SC_ALTCTL_TXRST_Msk | SC_ALTCTL_RXRST_Msk);
}

/**
  * @brief This function disable specified smartcard module
  * @param[in] sc Base address of smartcard module
  * @return None
  */
void SC_Close(SC_T *sc)
{
    sc->INTEN = 0;
    while(sc->PINCTL & SC_PINCTL_SYNC_Msk);
    sc->PINCTL = 0;
    sc->ALTCTL = 0;
    while(sc->CTL & SC_CTL_SYNC_Msk);
    sc->CTL = 0;
}

/**
  * @brief This function initialized smartcard module
  * @param[in] sc Base address of smartcard module
  * @param[in] u32CD Card detect polarity, select the CD pin state which indicates card insert. Could be
  *                 -\ref SC_PIN_STATE_HIGH
  *                 -\ref SC_PIN_STATE_LOW
  *                 -\ref SC_PIN_STATE_IGNORE, no card detect pin, always assumes card present
  * @param[in] u32PWR Power on polarity, select the PWR pin state which could set smartcard VCC to high level. Could be
  *                 -\ref SC_PIN_STATE_HIGH
  *                 -\ref SC_PIN_STATE_LOW
  * @return None
  */
void SC_Open(SC_T *sc, uint32_t u32CD, uint32_t u32PWR)
{
    uint32_t u32Reg = 0, u32Intf;

    if(sc == SC0)
        u32Intf = 0;
    else if(sc == SC1)
        u32Intf = 1;
    else
        u32Intf = 2;

    if(u32CD != SC_PIN_STATE_IGNORE) {
        u32Reg = u32CD ? 0: SC_CTL_CDLV_Msk;
        u32CardStateIgnore[u32Intf] = 0;
    } else {
        u32CardStateIgnore[u32Intf] = 1;
    }
    sc->PINCTL = u32PWR ? 0 : SC_PINCTL_PWRINV_Msk;
    while(sc->CTL & SC_CTL_SYNC_Msk);
    sc->CTL = SC_CTL_SCEN_Msk | u32Reg;
}

/**
  * @brief This function reset specified smartcard module to its default state for activate smartcard
  * @param[in] sc Base address of smartcard module
  * @return None
  */
void SC_ResetReader(SC_T *sc)
{
    uint32_t u32Intf;

    if(sc == SC0)
        u32Intf = 0;
    else if(sc == SC1)
        u32Intf = 1;
    else
        u32Intf = 2;


    // Reset FIFO, enable auto de-activation while card removal
    sc->ALTCTL |= (SC_ALTCTL_TXRST_Msk | SC_ALTCTL_RXRST_Msk | SC_ALTCTL_ADACEN_Msk);
    // Set Rx trigger level to 1 character, longest card detect debounce period, disable error retry (EMV ATR does not use error retry)
    while(sc->CTL & SC_CTL_SYNC_Msk);
    sc->CTL &= ~(SC_CTL_RXTRGLV_Msk | SC_CTL_CDDBSEL_Msk | SC_CTL_TXRTY_Msk | SC_CTL_RXRTY_Msk);
    // Enable auto convention, and all three smartcard internal timers
    sc->CTL |= SC_CTL_AUTOCEN_Msk | SC_CTL_TMRSEL_Msk;
    // Disable Rx timeout
    sc->RXTOUT = 0;
    // 372 clocks per ETU by default
    sc->ETUCTL= 371;


    /* Enable necessary interrupt for smartcard operation */
    if(u32CardStateIgnore[u32Intf]) // Do not enable card detect interrupt if card present state ignore
        sc->INTEN = (SC_INTEN_RDAIEN_Msk |
                     SC_INTEN_TERRIEN_Msk |
                     SC_INTEN_TMR0IEN_Msk |
                     SC_INTEN_TMR1IEN_Msk |
                     SC_INTEN_TMR2IEN_Msk |
                     SC_INTEN_BGTIEN_Msk |
                     SC_INTEN_ACERRIEN_Msk);
    else
        sc->INTEN = (SC_INTEN_RDAIEN_Msk |
                     SC_INTEN_TERRIEN_Msk |
                     SC_INTEN_TMR0IEN_Msk |
                     SC_INTEN_TMR1IEN_Msk |
                     SC_INTEN_TMR2IEN_Msk |
                     SC_INTEN_BGTIEN_Msk |
                     SC_INTEN_CDIEN_Msk |
                     SC_INTEN_ACERRIEN_Msk);

    return;
}

/**
  * @brief This function block guard time (BGT) of specified smartcard module
  * @param[in] sc Base address of smartcard module
  * @param[in] u32BGT Block guard time using ETU as unit, valid range are between 1 ~ 32
  * @return None
  */
void SC_SetBlockGuardTime(SC_T *sc, uint32_t u32BGT)
{
    sc->CTL = (sc->CTL & ~SC_CTL_BGT_Msk) | ((u32BGT - 1) << SC_CTL_BGT_Pos);
}

/**
  * @brief This function character guard time (CGT) of specified smartcard module
  * @param[in] sc Base address of smartcard module
  * @param[in] u32CGT Character guard time using ETU as unit, valid range are between 11 ~ 267
  * @return None
  */
void SC_SetCharGuardTime(SC_T *sc, uint32_t u32CGT)
{
    u32CGT -= sc->CTL & SC_CTL_NSB_Msk ? 11: 12;
    sc->EGT = u32CGT;
}

/**
  * @brief This function stop all smartcard timer of specified smartcard module
  * @param[in] sc Base address of smartcard module
  * @return None
  * @note This function stop the timers within smartcard module, \b not timer module
  */
void SC_StopAllTimer(SC_T *sc)
{
    sc->ALTCTL &= ~(SC_ALTCTL_CNTEN0_Msk | SC_ALTCTL_CNTEN1_Msk | SC_ALTCTL_CNTEN2_Msk);
}

/**
  * @brief This function configure and start a smartcard timer of specified smartcard module
  * @param[in] sc Base address of smartcard module
  * @param[in] u32TimerNum Timer(s) to start. Valid values are 0, 1, 2.
  * @param[in] u32Mode Timer operating mode, valid values are:
  *             - \ref SC_TMR_MODE_0
  *             - \ref SC_TMR_MODE_1
  *             - \ref SC_TMR_MODE_2
  *             - \ref SC_TMR_MODE_3
  *             - \ref SC_TMR_MODE_4
  *             - \ref SC_TMR_MODE_5
  *             - \ref SC_TMR_MODE_6
  *             - \ref SC_TMR_MODE_7
  *             - \ref SC_TMR_MODE_8
  *             - \ref SC_TMR_MODE_F
  * @param[in] u32ETUCount Timer timeout duration, ETU based. For timer 0, valid  range are between 1~0x1000000ETUs.
  *                        For timer 1 and timer 2, valid range are between 1 ~ 0x100 ETUs
  * @return None
  * @note This function start the timer within smartcard module, \b not timer module
  * @note Depend on the timer operating mode, timer may not start counting immediately
  */
void SC_StartTimer(SC_T *sc, uint32_t u32TimerNum, uint32_t u32Mode, uint32_t u32ETUCount)
{
    uint32_t reg = u32Mode | (SC_TMRCTL0_CNT_Msk & (u32ETUCount - 1));

    if(u32TimerNum == 0) {
        sc->TMRCTL0 = reg;
        sc->ALTCTL |= SC_ALTCTL_CNTEN0_Msk;
    } else if(u32TimerNum == 1) {
        sc->TMRCTL1 = reg;
        sc->ALTCTL |= SC_ALTCTL_CNTEN1_Msk;
    } else {   // timer 2
        sc->TMRCTL2 = reg;
        sc->ALTCTL |= SC_ALTCTL_CNTEN2_Msk;
    }
}

/**
  * @brief This function stop a smartcard timer of specified smartcard module
  * @param[in] sc Base address of smartcard module
  * @param[in] u32TimerNum Timer(s) to stop. Valid values are 0, 1, 2.
  * @return None
  * @note This function stop the timer within smartcard module, \b not timer module
  */
void SC_StopTimer(SC_T *sc, uint32_t u32TimerNum)
{
    if(u32TimerNum == 0)
        sc->ALTCTL &= ~SC_ALTCTL_CNTEN0_Msk;
    else if(u32TimerNum == 1)
        sc->ALTCTL &= ~SC_ALTCTL_CNTEN1_Msk;
    else    // timer 2
        sc->ALTCTL &= ~SC_ALTCTL_CNTEN2_Msk;
}

/**
  * @brief  This function gets smartcard clock frequency.
  * @param[in] sc Base address of smartcard module
  * @return Smartcard frequency in kHZ
  */
uint32_t SC_GetInterfaceClock(SC_T *sc)
{
    uint32_t u32ClkSrc, u32Num, u32Clk;

    if(sc == SC0)
        u32Num = 0;
    else if(sc == SC1)
        u32Num = 1;
    else
        u32Num = 2;


    u32ClkSrc = (CLK->CLKSEL3 >> (2 * u32Num)) & CLK_CLKSEL3_SC0SEL_Msk;

    // Get smartcard module clock
    if(u32ClkSrc == 0)
        u32Clk = __HXT;
    else if(u32ClkSrc == 1)
        u32Clk = CLK_GetPLLClockFreq();
    else if(u32ClkSrc == 2) {
        if(u32Num == 1)
            u32Clk = CLK_GetPCLK1Freq();
        else
            u32Clk = CLK_GetPCLK0Freq();
    } else
        u32Clk = __HIRC;


    u32Clk /= (((CLK->CLKDIV1 >> (8 * u32Num)) & CLK_CLKDIV1_SC0DIV_Msk) + 1) * 1000;
    return u32Clk;
}

/*@}*/ /* end of group M480_SC_EXPORTED_FUNCTIONS */

/*@}*/ /* end of group M480_SC_Driver */

/*@}*/ /* end of group M480_Device_Driver */

/*** (C) COPYRIGHT 2016 Nuvoton Technology Corp. ***/
