/***************************************************************************//**
* \file cyhal_gpio.c
*
* Description:
* Provides a high level interface for interacting with the Cypress GPIO. This is
* a wrapper around the lower level PDL API.
*
********************************************************************************
* \copyright
* Copyright 2018-2019 Cypress Semiconductor Corporation
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
#include "cyhal_hwmgr.h"

#ifdef CY_IP_MXS40IOSS

#if defined(__cplusplus)
extern "C" {
#endif /* __cplusplus */


/*******************************************************************************
*       Internal
*******************************************************************************/
#define CYHAL_DIRECTION_INPUT_MASK      (0x08UL)    /**< Mask to enable the input buffer */
#define CYHAL_DIRECTION_OUTPUT_MASK     (0x07UL)    /**< Mask to disable the input buffer */

/* Callback array for GPIO interrupts */
static cyhal_gpio_irq_handler_t hal_gpio_callbacks[IOSS_GPIO_GPIO_PORT_NR][CY_GPIO_PINS_MAX];
static void *hal_gpio_callback_args[IOSS_GPIO_GPIO_PORT_NR][CY_GPIO_PINS_MAX];


/*******************************************************************************
*       Dispatcher Interrrupt Service Routine
*******************************************************************************/

static void ioss_interrupts_dispatcher_IRQHandler(uint32_t port)
{
    GPIO_PRT_Type *portAddr = Cy_GPIO_PortToAddr(port);

    for (uint8_t cnt = 0u; cnt < CY_GPIO_PINS_MAX; cnt++)
    {
        if (Cy_GPIO_GetInterruptStatusMasked(portAddr, cnt))
        {
            if (hal_gpio_callbacks[port][cnt] != NULL)
            {
                /* Call registered callbacks here */
                cyhal_gpio_irq_event_t event, edge = (cyhal_gpio_irq_event_t)Cy_GPIO_GetInterruptEdge(portAddr, cnt);
                switch (edge)
                {
                    case CYHAL_GPIO_IRQ_NONE:
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


/*******************************************************************************
*       (Internal) Interrrupt Service Routines
*******************************************************************************/
static void ioss_interrupts_gpio_0_IRQHandler (void) __attribute__((unused));
static void ioss_interrupts_gpio_1_IRQHandler (void) __attribute__((unused));
static void ioss_interrupts_gpio_2_IRQHandler (void) __attribute__((unused));
static void ioss_interrupts_gpio_3_IRQHandler (void) __attribute__((unused));
static void ioss_interrupts_gpio_4_IRQHandler (void) __attribute__((unused));
static void ioss_interrupts_gpio_5_IRQHandler (void) __attribute__((unused));
static void ioss_interrupts_gpio_6_IRQHandler (void) __attribute__((unused));
static void ioss_interrupts_gpio_7_IRQHandler (void) __attribute__((unused));
static void ioss_interrupts_gpio_8_IRQHandler (void) __attribute__((unused));
static void ioss_interrupts_gpio_9_IRQHandler (void) __attribute__((unused));
static void ioss_interrupts_gpio_10_IRQHandler (void) __attribute__((unused));
static void ioss_interrupts_gpio_11_IRQHandler (void) __attribute__((unused));
static void ioss_interrupts_gpio_12_IRQHandler (void) __attribute__((unused));
static void ioss_interrupts_gpio_13_IRQHandler (void) __attribute__((unused));
static void ioss_interrupts_gpio_14_IRQHandler (void) __attribute__((unused));

static void ioss_interrupts_gpio_0_IRQHandler (void)
{
    ioss_interrupts_dispatcher_IRQHandler(CYHAL_PORT_0);
}

static void ioss_interrupts_gpio_1_IRQHandler (void)
{
    ioss_interrupts_dispatcher_IRQHandler(CYHAL_PORT_1);
}

static void ioss_interrupts_gpio_2_IRQHandler (void)
{
    ioss_interrupts_dispatcher_IRQHandler(CYHAL_PORT_2);
}

static void ioss_interrupts_gpio_3_IRQHandler (void)
{
    ioss_interrupts_dispatcher_IRQHandler(CYHAL_PORT_3);
}

static void ioss_interrupts_gpio_4_IRQHandler (void)
{
    ioss_interrupts_dispatcher_IRQHandler(CYHAL_PORT_4);
}

static void ioss_interrupts_gpio_5_IRQHandler (void)
{
    ioss_interrupts_dispatcher_IRQHandler(CYHAL_PORT_5);
}

static void ioss_interrupts_gpio_6_IRQHandler (void)
{
    ioss_interrupts_dispatcher_IRQHandler(CYHAL_PORT_6);
}

static void ioss_interrupts_gpio_7_IRQHandler (void)
{
    ioss_interrupts_dispatcher_IRQHandler(CYHAL_PORT_7);
}

static void ioss_interrupts_gpio_8_IRQHandler (void)
{
    ioss_interrupts_dispatcher_IRQHandler(CYHAL_PORT_8);
}

static void ioss_interrupts_gpio_9_IRQHandler (void)
{
    ioss_interrupts_dispatcher_IRQHandler(CYHAL_PORT_9);
}

static void ioss_interrupts_gpio_10_IRQHandler (void)
{
    ioss_interrupts_dispatcher_IRQHandler(CYHAL_PORT_10);
}

static void ioss_interrupts_gpio_11_IRQHandler (void)
{
    ioss_interrupts_dispatcher_IRQHandler(CYHAL_PORT_11);
}

static void ioss_interrupts_gpio_12_IRQHandler (void)
{
    ioss_interrupts_dispatcher_IRQHandler(CYHAL_PORT_12);
}

static void ioss_interrupts_gpio_13_IRQHandler (void)
{
    ioss_interrupts_dispatcher_IRQHandler(CYHAL_PORT_13);
}

static void ioss_interrupts_gpio_14_IRQHandler (void)
{
    ioss_interrupts_dispatcher_IRQHandler(CYHAL_PORT_14);
}

static void (*ioss_interrupts_dispatcher_table[IOSS_GPIO_GPIO_PORT_NR])(void) =
{
#if (IOSS_GPIO_GPIO_PORT_NR > 0)
    ioss_interrupts_gpio_0_IRQHandler,
#endif
#if (IOSS_GPIO_GPIO_PORT_NR > 1)
    ioss_interrupts_gpio_1_IRQHandler,
#endif
#if (IOSS_GPIO_GPIO_PORT_NR > 2)
    ioss_interrupts_gpio_2_IRQHandler,
#endif
#if (IOSS_GPIO_GPIO_PORT_NR > 3)
    ioss_interrupts_gpio_3_IRQHandler,
#endif
#if (IOSS_GPIO_GPIO_PORT_NR > 4)
    ioss_interrupts_gpio_4_IRQHandler,
#endif
#if (IOSS_GPIO_GPIO_PORT_NR > 5)
    ioss_interrupts_gpio_5_IRQHandler,
#endif
#if (IOSS_GPIO_GPIO_PORT_NR > 6)
    ioss_interrupts_gpio_6_IRQHandler,
#endif
#if (IOSS_GPIO_GPIO_PORT_NR > 7)
    ioss_interrupts_gpio_7_IRQHandler,
#endif
#if (IOSS_GPIO_GPIO_PORT_NR > 8)
    ioss_interrupts_gpio_8_IRQHandler,
#endif
#if (IOSS_GPIO_GPIO_PORT_NR > 9)
    ioss_interrupts_gpio_9_IRQHandler,
#endif
#if (IOSS_GPIO_GPIO_PORT_NR > 10)
    ioss_interrupts_gpio_10_IRQHandler,
#endif
#if (IOSS_GPIO_GPIO_PORT_NR > 11)
    ioss_interrupts_gpio_11_IRQHandler,
#endif
#if (IOSS_GPIO_GPIO_PORT_NR > 12)
    ioss_interrupts_gpio_12_IRQHandler,
#endif
#if (IOSS_GPIO_GPIO_PORT_NR > 13)
    ioss_interrupts_gpio_13_IRQHandler,
#endif
#if (IOSS_GPIO_GPIO_PORT_NR > 14)
    ioss_interrupts_gpio_14_IRQHandler,
#endif
#if (IOSS_GPIO_GPIO_PORT_NR > 15)
    #error "Unhandled port count"
#endif
};


/*******************************************************************************
*       Functions
*******************************************************************************/

cy_rslt_t cyhal_gpio_init(cyhal_gpio_t pin, cyhal_gpio_direction_t direction, cyhal_gpio_drive_mode_t drvMode, bool initVal)
{
    cyhal_resource_inst_t pinRsc = cyhal_utils_get_gpio_resource(pin);
    /* Mbed creates GPIOs for pins that are dedicated to other peripherals in some cases. */
#ifndef __MBED__
    cy_rslt_t status = cyhal_hwmgr_reserve(&pinRsc);
#else
    cy_rslt_t status = CY_RSLT_SUCCESS;
#endif

    if (status == CY_RSLT_SUCCESS)
    {
        /* Do not check for configured for resources we do not reserve. */
#ifndef __MBED__
        bool configured = cyhal_hwmgr_is_configured(pinRsc.type, pinRsc.block_num, pinRsc.channel_num);
        if (!configured)
#endif
        {
            Cy_GPIO_Pin_FastInit(CYHAL_GET_PORTADDR(pin), CYHAL_GET_PIN(pin), drvMode, initVal, HSIOM_SEL_GPIO);
            cyhal_gpio_direction(pin, direction); //always returns success
            status = cyhal_hwmgr_set_configured(pinRsc.type, pinRsc.block_num, pinRsc.channel_num);
        }
    }

    return status;
}

void cyhal_gpio_free(cyhal_gpio_t pin)
{
    if (pin != CYHAL_NC_PIN_VALUE)
    {
        cyhal_resource_inst_t pinRsc = cyhal_utils_get_gpio_resource(pin);
        Cy_GPIO_Pin_FastInit(CYHAL_GET_PORTADDR(pin), CYHAL_GET_PIN(pin), CYHAL_GPIO_DRIVE_ANALOG, 0UL, HSIOM_SEL_GPIO);
        cyhal_hwmgr_set_unconfigured(pinRsc.type, pinRsc.block_num, pinRsc.channel_num);
        /* Do not attempt to free the resource we don't reserve in mbed. */
#ifndef __MBED__
        cyhal_hwmgr_free(&pinRsc);
#endif
    }
}

cy_rslt_t cyhal_gpio_direction(cyhal_gpio_t pin, cyhal_gpio_direction_t direction)
{
    uint32_t drvMode = Cy_GPIO_GetDrivemode(CYHAL_GET_PORTADDR(pin), CYHAL_GET_PIN(pin));
    if ((direction == CYHAL_GPIO_DIR_INPUT) | (direction == CYHAL_GPIO_DIR_BIDIRECTIONAL))
    {
        Cy_GPIO_SetDrivemode(CYHAL_GET_PORTADDR(pin), CYHAL_GET_PIN(pin), (drvMode | CYHAL_DIRECTION_INPUT_MASK));
    }
    else
    {
        Cy_GPIO_SetDrivemode(CYHAL_GET_PORTADDR(pin), CYHAL_GET_PIN(pin), (drvMode & CYHAL_DIRECTION_OUTPUT_MASK));
    }

    return CY_RSLT_SUCCESS;
}

cy_rslt_t cyhal_gpio_drivemode(cyhal_gpio_t pin, cyhal_gpio_drive_mode_t drvMode)
{
    uint32_t inputBuf = Cy_GPIO_GetDrivemode(CYHAL_GET_PORTADDR(pin), CYHAL_GET_PIN(pin)) & CYHAL_DIRECTION_INPUT_MASK;
    Cy_GPIO_SetDrivemode(CYHAL_GET_PORTADDR(pin), CYHAL_GET_PIN(pin), drvMode | inputBuf);

    return CY_RSLT_SUCCESS;
}

void cyhal_gpio_register_irq(cyhal_gpio_t pin, uint8_t intrPriority, cyhal_gpio_irq_handler_t handler, void *handler_arg)
{
    IRQn_Type irqn = (IRQn_Type)(ioss_interrupts_gpio_0_IRQn + CYHAL_GET_PORT(pin));

    hal_gpio_callbacks[CYHAL_GET_PORT(pin)][CYHAL_GET_PIN(pin)] = handler;
    hal_gpio_callback_args[CYHAL_GET_PORT(pin)][CYHAL_GET_PIN(pin)] = handler_arg;

    /* Only enable if it's not already enabled */
    if (NVIC_GetEnableIRQ(irqn) == 0)
    {
        cy_stc_sysint_t irqCfg = {irqn, intrPriority};

        Cy_SysInt_Init(&irqCfg, ioss_interrupts_dispatcher_table[CYHAL_GET_PORT(pin)]);
        NVIC_EnableIRQ(irqn);
    }
    else
    {
        NVIC_SetPriority(irqn, intrPriority);
    }
}

void cyhal_gpio_irq_enable(cyhal_gpio_t pin, cyhal_gpio_irq_event_t event, bool enable)
{
    Cy_GPIO_SetInterruptEdge(CYHAL_GET_PORTADDR(pin), CYHAL_GET_PIN(pin), (uint32_t)event);
    Cy_GPIO_SetInterruptMask(CYHAL_GET_PORTADDR(pin), CYHAL_GET_PIN(pin), (uint32_t)enable);
}

#if defined(__cplusplus)
}
#endif /* __cplusplus */

#endif /* CY_IP_MXS40IOSS */
