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

/* Called before main - implement here if board needs it.
 * Otherwise, let the application override this if necessary */
void mbed_sdk_init()
{
    CHIP_Init();

#if defined(_SILICON_LABS_32B_PLATFORM_2)
    EMU_DCDCInit_TypeDef dcdcInit = EMU_DCDCINIT_DEFAULT;
    EMU_DCDCInit(&dcdcInit);
#endif

    /* Set up the clock sources for this chip */
#if( CORE_CLOCK_SOURCE == HFXO)
    CMU_ClockSelectSet(cmuClock_HF, cmuSelect_HFXO);
    SystemHFXOClockSet(HFXO_FREQUENCY);
#elif( CORE_CLOCK_SOURCE == HFRCO)
    CMU_ClockSelectSet(cmuClock_HF, cmuSelect_HFRCO);
# if defined _CMU_HFRCOCTRL_BAND_MASK
    CMU_HFRCOBandSet(HFRCO_FREQUENCY);
# elif defined _CMU_HFRCOCTRL_FREQRANGE_MASK
    CMU_HFRCOFreqSet(HFRCO_FREQUENCY_ENUM);
# else
#  error "Can't set HFRCO frequency"
# endif
#else
# error "Core clock selection not valid (mbed_overrides.c)"
#endif

    CMU_ClockEnable(cmuClock_CORELE, true);

#if( LOW_ENERGY_CLOCK_SOURCE == LFXO )
# ifdef _CMU_LFACLKEN0_MASK
    CMU_ClockSelectSet(cmuClock_LFA, cmuSelect_LFXO);
# endif
# ifdef _CMU_LFBCLKEN0_MASK
    /* cmuClock_LFB (to date) only has LEUART peripherals.
    *  This gets set automatically whenever you create serial objects using LEUART
    */
# endif
# ifdef _CMU_LFECLKEN0_MASK
    CMU_ClockSelectSet(cmuClock_LFE, cmuSelect_LFXO);
# endif
    SystemLFXOClockSet(LFXO_FREQUENCY);

#elif( LOW_ENERGY_CLOCK_SOURCE == LFRCO )
# ifdef _CMU_LFACLKEN0_MASK
    CMU_ClockSelectSet(cmuClock_LFA, cmuSelect_LFRCO);
# endif
# ifdef _CMU_LFBCLKEN0_MASK
    //CMU_ClockSelectSet(cmuClock_LFB, cmuSelect_LFRCO);
# endif
# ifdef _CMU_LFECLKEN0_MASK
    CMU_ClockSelectSet(cmuClock_LFE, cmuSelect_LFRCO);
# endif

#elif( LOW_ENERGY_CLOCK_SOURCE == ULFRCO)
# ifdef _CMU_LFACLKEN0_MASK
    CMU_ClockSelectSet(cmuClock_LFA, cmuSelect_ULFRCO);
# endif
# ifdef _CMU_LFBCLKEN0_MASK
    CMU_ClockSelectSet(cmuClock_LFB, cmuSelect_ULFRCO);
# endif
# ifdef _CMU_LFECLKEN0_MASK
    CMU_ClockSelectSet(cmuClock_LFE, cmuSelect_ULFRCO);
# endif
#else
# error "Low energy clock selection not valid"
#endif

    /* Enable BC line driver to avoid garbage on CDC port */
    gpio_init_out_ex(&bc_enable, EFM_BC_EN, 1);
}
