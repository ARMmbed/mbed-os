/***************************************************************************//**
 * @file mbed_overrides.c
 *******************************************************************************
 * @section License
 * <b>(C) Copyright 2015 Silicon Labs, http://www.silabs.com</b>
 *******************************************************************************
 *
 * Permission is granted to anyone to use this software for any purpose,
 * including commercial applications, and to alter it and redistribute it
 * freely, subject to the following restrictions:
 *
 * 1. The origin of this software must not be misrepresented; you must not
 *    claim that you wrote the original software.
 * 2. Altered source versions must be plainly marked as such, and must not be
 *    misrepresented as being the original software.
 * 3. This notice may not be removed or altered from any source distribution.
 *
 * DISCLAIMER OF WARRANTY/LIMITATION OF REMEDIES: Silicon Labs has no
 * obligation to support this Software. Silicon Labs is providing the
 * Software "AS IS", with no express or implied warranties of any kind,
 * including, but not limited to, any implied warranties of merchantability
 * or fitness for any particular purpose or warranties against infringement
 * of any proprietary rights of a third party.
 *
 * Silicon Labs will not be liable for any consequential, incidental, or
 * special damages, or any other relief, or for any claim by any third party,
 * arising from your use of this Software.
 *
 ******************************************************************************/

#include "em_chip.h"
#include "em_device.h"
#include "em_cmu.h"
#include "em_emu.h"
#include "device_peripherals.h"
#include "device.h"
#include "em_usart.h"
#include "gpio_api.h"

gpio_t bc_enable;

void check_usart_clock(USART_TypeDef* usart, uint32_t clockmask);

/* Called before main - implement here if board needs it.
 * Otherwise, let the application override this if necessary */
void mbed_sdk_init()
{
    CHIP_Init();

    /* Set up the clock sources for this chip */
#if( CORE_CLOCK_SOURCE == HFXO)
    CMU_ClockSelectSet(cmuClock_HF, cmuSelect_HFXO);
    SystemHFXOClockSet(HFXO_FREQUENCY);
#elif( CORE_CLOCK_SOURCE == HFRCO)
    CMU_ClockSelectSet(cmuClock_HF, cmuSelect_HFRCO);
    CMU_HFRCOBandSet(HFRCO_FREQUENCY);
#else
#error "Core clock selection not valid (mbed_overrides.c)"
#endif

    CMU_ClockEnable(cmuClock_CORELE, true);

#if( LOW_ENERGY_CLOCK_SOURCE == LFXO )
#ifdef CMU_LFACLKSEL_REG
    CMU_ClockSelectSet(cmuClock_LFA, cmuSelect_LFXO);
#endif
#ifdef CMU_LFBCLKSEL_REG
    /* cmuClock_LFB (to date) only has LEUART peripherals.
    *  This gets set automatically whenever you create serial objects using LEUART
    */
#endif
#ifdef CMU_LFECLKSEL_REG
    CMU_ClockSelectSet(cmuClock_LFE, cmuSelect_LFXO);
#endif
    SystemLFXOClockSet(LFXO_FREQUENCY);

#elif( LOW_ENERGY_CLOCK_SOURCE == LFRCO )
#ifdef CMU_LFACLKSEL_REG
    CMU_ClockSelectSet(cmuClock_LFA, cmuSelect_LFRCO);
#endif
#ifdef CMU_LFBCLKSEL_REG
    //CMU_ClockSelectSet(cmuClock_LFB, cmuSelect_LFRCO);
#endif
#ifdef CMU_LFECLKSEL_REG
    CMU_ClockSelectSet(cmuClock_LFE, cmuSelect_LFRCO);
#endif
    CMU_HFRCOBandSet(HFRCO_FREQUENCY);

#elif( LOW_ENERGY_CLOCK_SOURCE == ULFRCO)
#ifdef CMU_LFACLKSEL_REG
    CMU_ClockSelectSet(cmuClock_LFA, cmuSelect_ULFRCO);
#endif
#ifdef CMU_LFBCLKSEL_REG
    CMU_ClockSelectSet(cmuClock_LFB, cmuSelect_ULFRCO);
#endif
#ifdef CMU_LFECLKSEL_REG
    CMU_ClockSelectSet(cmuClock_LFE, cmuSelect_ULFRCO);
#endif
#else
#error "Low energy clock selection not valid"
#endif

    /* Enable BC line driver to avoid garbage on CDC port */
    gpio_init_out_ex(&bc_enable, EFM_BC_EN, 1);
}

void check_usart_clock(USART_TypeDef* usart, uint32_t clockmask)
{
    uint32_t freq = 14000000, baudrate;
    USART_OVS_TypeDef ovs;

    if(CMU->HFPERCLKEN0 & clockmask) {
        /* Different methods for sync vs async */
        if(usart->CTRL & USART_CTRL_SYNC) {
            ovs  = (USART_OVS_TypeDef) (usart->CTRL & _USART_CTRL_OVS_MASK);
            baudrate = USART_BaudrateCalc(freq, usart->CLKDIV, true, ovs);
            USART_BaudrateSyncSet(usart, 0, baudrate);
        } else {
            ovs  = (USART_OVS_TypeDef) (usart->CTRL & _USART_CTRL_OVS_MASK);
            baudrate = USART_BaudrateCalc(freq, usart->CLKDIV, false, ovs);
            USART_BaudrateAsyncSet(usart, 0, baudrate, ovs);
        }
    }
}
