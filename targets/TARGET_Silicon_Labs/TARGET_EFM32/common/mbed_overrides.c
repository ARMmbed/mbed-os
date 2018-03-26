/***************************************************************************//**
 * @file mbed_overrides.c
 *******************************************************************************
 * @section License
 * <b>(C) Copyright 2015 Silicon Labs, http://www.silabs.com</b>
 *******************************************************************************
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the "License"); you may
 * not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
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

#if defined(_SILICON_LABS_32B_SERIES_1)
#if defined(EMU_NO_DCDC)
    EMU_DCDCPowerOff();
#else
    EMU_DCDCInit_TypeDef dcdcInit = EMU_DCDCINIT_STK_DEFAULT;
    EMU_DCDCInit(&dcdcInit);
#endif

#if (CORE_CLOCK_SOURCE == HFXO)
    // Only init HFXO if not already done (e.g. by bootloader)
    if (CMU_ClockSelectGet(cmuClock_HF) != cmuSelect_HFXO) {
#if defined(_EFR_DEVICE)
        CMU_HFXOInit_TypeDef hfxoInit = CMU_HFXOINIT_WSTK_DEFAULT;
#else
        CMU_HFXOInit_TypeDef hfxoInit = CMU_HFXOINIT_STK_DEFAULT;
#endif
        CMU_HFXOInit(&hfxoInit);
    }
#endif
#endif

    /* Set up the clock sources for this chip */
#if( CORE_CLOCK_SOURCE == HFXO)
    CMU_ClockSelectSet(cmuClock_HF, cmuSelect_HFXO);
    SystemHFXOClockSet(HFXO_FREQUENCY);
#elif( CORE_CLOCK_SOURCE == HFRCO)
    CMU_ClockSelectSet(cmuClock_HF, cmuSelect_HFRCO);
# if defined _CMU_HFRCOCTRL_BAND_MASK
    CMU_HFRCOBandSet(HFRCO_FREQUENCY_ENUM);
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

#if defined(RTCC_PRESENT)
    /* Turn RTCC clock gate back on to keep RTC time correct */
    CMU_ClockEnable(cmuClock_RTCC, true);
#endif

#if defined(EFM_BC_EN)
    /* Enable BC line driver to avoid garbage on CDC port */
    gpio_init_out_ex(&bc_enable, EFM_BC_EN, 1);
#endif
}
