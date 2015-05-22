/* mbed Microcontroller Library
 * Copyright (c) 2006-2013 ARM Limited
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
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
    CMU_ClockSelectSet(cmuClock_HF, HFXO);
    SystemHFXOClockSet(HFXO_FREQUENCY);
#elif( CORE_CLOCK_SOURCE == HFRCO)
    CMU_ClockSelectSet(cmuClock_HF, HFRCO);
    CMU_HFRCOBandSet(HFRCO_FREQUENCY);
#else
#error "Core clock selection not valid (mbed_overrides.c)"
#endif

    CMU_ClockEnable(cmuClock_CORELE, true);

#if( LOW_ENERGY_CLOCK_SOURCE == LFXO )
#ifdef CMU_LFACLKSEL_REG
    CMU_ClockSelectSet(cmuClock_LFA, LFXO);
#endif
#ifdef CMU_LFBCLKSEL_REG
	  /* cmuClock_LFB (to date) only has LEUART peripherals.
		*  Do NOT set it up here, as LEUARTs might have been initialized
		*	 before this code is called. (Limitation of the override mechanism of ARMCC) 
		*/
		//TODO: Look for a more elegant fix.
    //CMU_ClockSelectSet(cmuClock_LFB, LFXO);
#endif
#ifdef CMU_LFECLKSEL_REG
    CMU_ClockSelectSet(cmuClock_LFE, LFXO);
#endif
    SystemLFXOClockSet(LFXO_FREQUENCY);

#elif( LOW_ENERGY_CLOCK_SOURCE == LFRCO )
#ifdef CMU_LFACLKSEL_REG
    CMU_ClockSelectSet(cmuClock_LFA, LFRCO);
#endif
#ifdef CMU_LFBCLKSEL_REG
    //CMU_ClockSelectSet(cmuClock_LFB, LFRCO);
#endif
#ifdef CMU_LFECLKSEL_REG
    CMU_ClockSelectSet(cmuClock_LFE, LFRCO);
#endif
    CMU_HFRCOBandSet(HFRCO_FREQUENCY);

#elif( LOW_ENERGY_CLOCK_SOURCE == ULFRCO)
#ifdef CMU_LFACLKSEL_REG
    CMU_ClockSelectSet(cmuClock_LFA, ULFRCO);
#endif
#ifdef CMU_LFBCLKSEL_REG
    CMU_ClockSelectSet(cmuClock_LFB, ULFRCO);
#endif
#ifdef CMU_LFECLKSEL_REG
    CMU_ClockSelectSet(cmuClock_LFE, ULFRCO);
#endif
#else
#error "Low energy clock selection not valid"
#endif

    /* Enable BC line driver to avoid garbage on CDC port */
    gpio_init_out_ex(&bc_enable, EFM_BC_EN, 1);
}

void check_usart_clock(USART_TypeDef* usart, uint32_t clockmask) {
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
