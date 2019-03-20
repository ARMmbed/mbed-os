/*
 * mbed Microcontroller Library
 * Copyright (c) 2017-2018 Future Electronics
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

#ifndef _PSOC6_UTILS_H_
#define _PSOC6_UTILS_H_

#if defined(__MBED__)

#include <stdint.h>
#include <device.h>
#include "cy_gpio.h"
#include "cy_sysclk.h"

#else

#include "project.h"

#endif

#include "PinNamesTypes.h"
#include "PinNames.h"

#if defined(__cplusplus)
extern "C" {
#endif /* __cplusplus */

#define CY_INVALID_DIVIDER      0xFF

/** \brief Allocates clock divider to be used for a new clock signal.
 *
 * \param div_type cy_en_divider_types_t Divider type.
 * \return uint32_t Divider number (id) or CY_CLK_INVALID_DIVIDER if unavailable.
 *
 */
uint32_t cy_clk_allocate_divider(cy_en_divider_types_t div_type);

/** \brief Reserves clock divider to be used for a new clock signal.
 *
 * \param div_type cy_en_divider_types_t Divider type.
 * \param div_num Divider number to be reserved.
 * \return uint32_t Divider number (id) or CY_CLK_INVALID_DIVIDER if unavailable.
 *
 */
uint32_t cy_clk_reserve_divider(cy_en_divider_types_t div_type, uint32_t div_num);

/** \brief Releases already reserved clock divider.
 *
 * \param div_type cy_en_divider_types_t Divider type.
 * \param div_num Divider number to be released.
 *
 */
void cy_clk_free_divider(cy_en_divider_types_t div_type, uint32_t div_num);

#ifdef TARGET_MCU_PSOC6_M0

#include "gpio_irq_api.h"

/** \brief  On PSoC6 M0 core interrupts are routed into NVIC via additional multiplexer.
 *          This function allocates free NVIC channel to be used by particular interrupt.
 *
 * \param   allocation_id Unique identifier (for debug purposes).
 * \return  IRQ channel allocated or (-1) if no free channel is available.
 *
 */
IRQn_Type cy_m0_nvic_allocate_channel(uint32_t allocation_id);

/** \brief Reserves particular NVIC channel if it is available.
 *
 * \param channel IRQn_Type     Channel to be reserved.
 * \param allocation_id uint32_t    Identifier.
 * \return  IRQ channel allocated or (-1) if no free channel is available.
 *
 */
IRQn_Type cy_m0_nvic_reserve_channel(IRQn_Type channel, uint32_t allocation_id);

/** \brief Releases NVIC channel.
 *
 * \param channel IRQn_Type     Channel to be released.
 * \param allocation_id uint32_t    Id used during allocation (for cross check).
 * \return void
 *
 */
void cy_m0_nvic_release_channel(IRQn_Type channel, uint32_t allocation_id);

#endif /* M0+ core */


/** \brief Request allocation of SCB block.
 *
 * \param scb_num uint32_t     Id of the SCB block.
 * \return (0) when OK, (-1) when reservation conflict occurs.
 *
 */
int cy_reserve_scb(uint32_t scb_num);

/** \brief Releases SCB block.
 *
 * \param scb_num uint32_t     Id of the SCB block.
 * \return void
 *
 */
void cy_free_scb(uint32_t scb_num);

/** \brief Request allocation of TCPWM block.
 *
 * \param tcpwm_num uint32_t     Id of the TCPWM block.
 * \return (0) when OK, (-1) when reservation conflict occurs.
 *
 */
int cy_reserve_tcpwm(uint32_t tcpwm_num);

/** \brief Releases TCPWM block.
 *
 * \param tcpwm_num uint32_t     Id of the TCPWM block.
 * \return void
 *
 */
void cy_free_tcpwm(uint32_t tcpwm_num);

/** \brief Request allocation of i/o pin.
 *
 * \param pin PinName     Id of the pin to allocate.
 * \return (0) when OK, (-1) when reservation conflict occurs.
 *
 */
int cy_reserve_io_pin(PinName pin);


/** \brief Releases i/o pin.
 *
 * \param pin PinName     Id of the pin.
 * \return void
 *
 */
void cy_free_io_pin(PinName pin);

/** \brief Initializes shared resource manager.
 *
 * \param none.
 * \return void
 *
 */
void cy_srm_initialize(void);


/** \brief Returns board-specific hardware MAC address.
 *
 * \param uint8_t *buffer   Buffer where address will be returned.
 * \return void.
 *
 */
void cy_get_bd_mac_address(uint8_t* buffer);


/** \brief Determines proper PSoC6 pin drive mode settings.
 *
 * \param dir PinDirection      Pin direction, in or out.
 * \param mode PinMode      Mbed pin mode.
 * \return PSoC6 pin drive mode.
 *
 */
static inline uint32_t gpio_get_cy_drive_mode(PinDirection dir, PinMode mode)
{
    uint32_t cymode = 0;

    switch (mode) {
        case PullNone:
            switch (dir) {
                case PIN_INPUT:
                    cymode = CY_GPIO_DM_HIGHZ;
                    break;
                case PIN_OUTPUT:
                    cymode = CY_GPIO_DM_STRONG;
                    break;
            }
            break;

        case PushPull:
            cymode = CY_GPIO_DM_STRONG;
            break;

        case PullUp:
            cymode = CY_GPIO_DM_PULLUP;
            break;
        case PullDown:
            cymode = CY_GPIO_DM_PULLDOWN;
            break;
        case OpenDrainDriveLow:
            cymode = CY_GPIO_DM_OD_DRIVESLOW;
            break;
        case OpenDrainDriveHigh:
            cymode = CY_GPIO_DM_OD_DRIVESHIGH;
            break;
        case AnalogMode:
            cymode = CY_GPIO_DM_ANALOG;
            break;
    }

    return cymode;
}



#if defined(__cplusplus)
}
#endif /* __cplusplus */


#endif // _PSOC6_UTILS_H_
