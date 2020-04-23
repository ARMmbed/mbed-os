/***************************************************************************//**
* \file cyhal_gpio.c
*
* Description:
* Provides a high level interface for interacting with the Cypress GPIO. This is
* a wrapper around the lower level PDL API.
*
********************************************************************************
* \copyright
* Copyright 2018-2020 Cypress Semiconductor Corporation
* SPDX-License-Identifier: Apache-2.0
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
*******************************************************************************/

#include "cyhal_gpio_impl.h"
#include "cyhal_system_impl.h"
#include "cyhal_hwmgr.h"

#ifdef CY_IP_MXS40IOSS

#if defined(__cplusplus)
extern "C" {
#endif /* __cplusplus */


/*******************************************************************************
*       Internal
*******************************************************************************/
#define CYHAL_GPIO_DIRECTION_OUTPUT_MASK      (0x07UL)    /**< Mask to disable the input buffer */

/* Callback array for GPIO interrupts */
static cyhal_gpio_event_callback_t hal_gpio_callbacks[IOSS_GPIO_GPIO_PORT_NR][CY_GPIO_PINS_MAX];
static void *hal_gpio_callback_args[IOSS_GPIO_GPIO_PORT_NR][CY_GPIO_PINS_MAX];

/*******************************************************************************
*       Internal Interrrupt Service Routine
*******************************************************************************/

static void ioss_irq_handler()
{
    IRQn_Type irqn = CYHAL_GET_CURRENT_IRQN();
    uint32_t port = irqn - ioss_interrupts_gpio_0_IRQn;
    GPIO_PRT_Type *portAddr = Cy_GPIO_PortToAddr(port);

    for (uint8_t cnt = 0u; cnt < CY_GPIO_PINS_MAX; cnt++)
    {
        if (Cy_GPIO_GetInterruptStatusMasked(portAddr, cnt))
        {
            if (hal_gpio_callbacks[port][cnt] != NULL)
            {
                /* Call registered callbacks here */
                cyhal_gpio_event_t event, edge = (cyhal_gpio_event_t)Cy_GPIO_GetInterruptEdge(portAddr, cnt);
                switch (edge)
                {
                    case CYHAL_GPIO_IRQ_RISE:
                    case CYHAL_GPIO_IRQ_FALL:
                        event = edge;
                        break;
                    default:
                        event = Cy_GPIO_Read(portAddr, cnt) != 0 ? CYHAL_GPIO_IRQ_RISE : CYHAL_GPIO_IRQ_FALL;
                        break;
                }
                (void)(hal_gpio_callbacks[port][cnt])(hal_gpio_callback_args[port][cnt], event);
            }
            Cy_GPIO_ClearInterrupt(portAddr, cnt);
        }
    }
}

static uint32_t cyhal_gpio_convert_drive_mode(cyhal_gpio_drive_mode_t drive_mode, cyhal_gpio_direction_t direction)
{
    uint32_t drvMode;
    switch (drive_mode)
    {
        /* For DRIVE_NONE and DRIVE_ANALOG: Return immediately so drvMode is
         * not modified after switch statement based on direction as direction
         * does not make sense for input only drive modes */
        case CYHAL_GPIO_DRIVE_NONE:
            drvMode = CY_GPIO_DM_HIGHZ;
            return drvMode;
        case CYHAL_GPIO_DRIVE_ANALOG:
            drvMode = CY_GPIO_DM_ANALOG;
            return drvMode;
        case CYHAL_GPIO_DRIVE_PULLUP:
            drvMode = CY_GPIO_DM_PULLUP;
            break;
        case CYHAL_GPIO_DRIVE_PULLDOWN:
            drvMode = CY_GPIO_DM_PULLDOWN;
            break;
        case CYHAL_GPIO_DRIVE_OPENDRAINDRIVESLOW:
            drvMode = CY_GPIO_DM_OD_DRIVESLOW;
            break;
        case CYHAL_GPIO_DRIVE_OPENDRAINDRIVESHIGH:
            drvMode = CY_GPIO_DM_OD_DRIVESHIGH;
            break;
        case CYHAL_GPIO_DRIVE_STRONG:
            drvMode = CY_GPIO_DM_STRONG;
            break;
        case CYHAL_GPIO_DRIVE_PULLUPDOWN:
            drvMode = CY_GPIO_DM_PULLUP_DOWN;
            break;
        case CYHAL_GPIO_DRIVE_PULL_NONE:
            if (direction == CYHAL_GPIO_DIR_OUTPUT || direction == CYHAL_GPIO_DIR_BIDIRECTIONAL)
            {
                drvMode = CY_GPIO_DM_STRONG;
            }
            else
            {
                drvMode = CY_GPIO_DM_HIGHZ;
            }
            break;
        default:
            CY_ASSERT(false);
            drvMode = CY_GPIO_DM_HIGHZ;
    }

    if (direction == CYHAL_GPIO_DIR_OUTPUT)
    {
        drvMode &= CYHAL_GPIO_DIRECTION_OUTPUT_MASK;
    }
    return drvMode;
}

/*******************************************************************************
*       HAL Implementation
*******************************************************************************/

cy_rslt_t cyhal_gpio_init(cyhal_gpio_t pin, cyhal_gpio_direction_t direction, cyhal_gpio_drive_mode_t drvMode, bool initVal)
{
    /* Mbed creates GPIOs for pins that are dedicated to other peripherals in some cases. */
#ifndef __MBED__
    cyhal_resource_inst_t pinRsc = cyhal_utils_get_gpio_resource(pin);
    cy_rslt_t status = cyhal_hwmgr_reserve(&pinRsc);
#else
    cy_rslt_t status = CY_RSLT_SUCCESS;
#endif

    if (status == CY_RSLT_SUCCESS)
    {
        uint32_t pdlDrvMode = cyhal_gpio_convert_drive_mode(drvMode, direction);
        Cy_GPIO_Pin_FastInit(CYHAL_GET_PORTADDR(pin), CYHAL_GET_PIN(pin), pdlDrvMode, initVal, HSIOM_SEL_GPIO);
    }

    return status;
}

void cyhal_gpio_free(cyhal_gpio_t pin)
{
    if (pin != CYHAL_NC_PIN_VALUE)
    {
        Cy_GPIO_SetInterruptMask(CYHAL_GET_PORTADDR(pin), CYHAL_GET_PIN(pin), 0);
        hal_gpio_callbacks[CYHAL_GET_PORT(pin)][CYHAL_GET_PIN(pin)] = NULL;
        hal_gpio_callback_args[CYHAL_GET_PORT(pin)][CYHAL_GET_PIN(pin)] = NULL;

        Cy_GPIO_Pin_FastInit(CYHAL_GET_PORTADDR(pin), CYHAL_GET_PIN(pin), CY_GPIO_DM_ANALOG, 0UL, HSIOM_SEL_GPIO);
        /* Do not attempt to free the resource we don't reserve in mbed. */
#ifndef __MBED__
        cyhal_resource_inst_t pinRsc = cyhal_utils_get_gpio_resource(pin);
        cyhal_hwmgr_free(&pinRsc);
#endif
    }
}

cy_rslt_t cyhal_gpio_configure(cyhal_gpio_t pin, cyhal_gpio_direction_t direction, cyhal_gpio_drive_mode_t drvMode)
{
    uint32_t pdlDrvMode = cyhal_gpio_convert_drive_mode(drvMode, direction);
    Cy_GPIO_SetDrivemode(CYHAL_GET_PORTADDR(pin), CYHAL_GET_PIN(pin), pdlDrvMode);

    return CY_RSLT_SUCCESS;
}

void cyhal_gpio_register_callback(cyhal_gpio_t pin, cyhal_gpio_event_callback_t callback, void *callback_arg)
{
    uint32_t savedIntrStatus = cyhal_system_critical_section_enter();
    hal_gpio_callbacks[CYHAL_GET_PORT(pin)][CYHAL_GET_PIN(pin)] = callback;
    hal_gpio_callback_args[CYHAL_GET_PORT(pin)][CYHAL_GET_PIN(pin)] = callback_arg;
    cyhal_system_critical_section_exit(savedIntrStatus);
}

void cyhal_gpio_enable_event(cyhal_gpio_t pin, cyhal_gpio_event_t event, uint8_t intrPriority, bool enable)
{
    Cy_GPIO_ClearInterrupt(CYHAL_GET_PORTADDR(pin), CYHAL_GET_PIN(pin));
    Cy_GPIO_SetInterruptEdge(CYHAL_GET_PORTADDR(pin), CYHAL_GET_PIN(pin), (uint32_t)event);
    Cy_GPIO_SetInterruptMask(CYHAL_GET_PORTADDR(pin), CYHAL_GET_PIN(pin), (uint32_t)enable);

    /* Only enable if it's not already enabled */
    IRQn_Type irqn = (IRQn_Type)(ioss_interrupts_gpio_0_IRQn + CYHAL_GET_PORT(pin));
    if (NVIC_GetEnableIRQ(irqn) == 0)
    {
        cy_stc_sysint_t irqCfg = {irqn, intrPriority};

        Cy_SysInt_Init(&irqCfg, ioss_irq_handler);
        NVIC_EnableIRQ(irqn);
    }
    else
    {
        NVIC_SetPriority(irqn, intrPriority);
    }
}

#if defined(__cplusplus)
}
#endif /* __cplusplus */

#endif /* CY_IP_MXS40IOSS */
