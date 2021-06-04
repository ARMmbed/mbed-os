/***************************************************************************//**
* \file cyhal_gpio.c
*
* Description:
* Provides a high level interface for interacting with the Cypress GPIO. This is
* a wrapper around the lower level PDL API.
*
********************************************************************************
* \copyright
* Copyright 2018-2021 Cypress Semiconductor Corporation
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
#include "cyhal_interconnect.h"
#include "cyhal_system.h"
#include "cyhal_hwmgr.h"

#if defined(CY_IP_MXS40IOSS) || defined(CY_IP_M0S8IOSS)

#if defined(__cplusplus)
extern "C" {
#endif /* __cplusplus */


/*******************************************************************************
*       Internal
*******************************************************************************/
#if defined(COMPONENT_CAT1A) || defined(COMPONENT_CAT1B)
#define _CYHAL_GPIO_DIRECTION_OUTPUT_MASK        (0x07UL)   /**< Mask to disable the input buffer */
#elif defined(COMPONENT_CAT2)
#define _CYHAL_GPIO_DIRECTION_OUTPUT_MASK        (0x08UL)   /**< Mask to disable the input buffer */
#endif

/* Callback array for GPIO interrupts */
static cyhal_gpio_event_callback_t _cyhal_gpio_callbacks[IOSS_GPIO_GPIO_PORT_NR][CY_GPIO_PINS_MAX];
static void *_cyhal_gpio_callback_args[IOSS_GPIO_GPIO_PORT_NR][CY_GPIO_PINS_MAX];
#if defined(CY_IP_MXS40IOSS)
static cyhal_source_t _cyhal_gpio_source_signals[sizeof(cyhal_pin_map_peri_tr_io_output)/sizeof(cyhal_resource_pin_mapping_t)] = { CYHAL_TRIGGER_CPUSS_ZERO };
#endif

#if defined(CY_DEVICE_PMG1S3)
/* PMG1-S3: Work-around as device does not support any GPIO port specific interrupts. */
#define ioss_interrupts_gpio_0_IRQn     (ioss_interrupt_gpio_IRQn)
#endif

/*******************************************************************************
*       Internal Interrupt Service Routine
*******************************************************************************/

static void _cyhal_gpio_irq_handler(void)
{
    IRQn_Type irqn = _CYHAL_UTILS_GET_CURRENT_IRQN();
    uint32_t port = (uint32_t)(irqn - ioss_interrupts_gpio_0_IRQn);
    uint32_t intr_cause = 1 << port;
#if defined(COMPONENT_CAT2)
    if (irqn == ioss_interrupt_gpio_IRQn)
    {
        intr_cause = Cy_GPIO_GetInterruptCause();
    }
#endif

    while(intr_cause != 0)
    {
        uint32_t curr_port = 31U - __CLZ(intr_cause);
        GPIO_PRT_Type *portAddr = Cy_GPIO_PortToAddr(curr_port);
        for (uint8_t cnt = 0u; cnt < CY_GPIO_PINS_MAX; cnt++)
        {
            /* Each supported architecture much have a way to check the interrupt status of a pin on a port. */
#if defined(COMPONENT_CAT1A) || defined(COMPONENT_CAT1B)
            if (Cy_GPIO_GetInterruptStatusMasked(portAddr, cnt))
            {
#elif defined(COMPONENT_CAT2)
            if (Cy_GPIO_GetInterruptStatus(portAddr, cnt))
            {
#else
    #error "Unsupported architecture"
#endif
                if (_cyhal_gpio_callbacks[curr_port][cnt] != NULL)
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
                    (void)(_cyhal_gpio_callbacks[curr_port][cnt])(_cyhal_gpio_callback_args[curr_port][cnt], event);
                }
                Cy_GPIO_ClearInterrupt(portAddr, cnt);
            }
        }
        intr_cause &= ~(1 << curr_port);
    }
}

static uint32_t _cyhal_gpio_convert_drive_mode(cyhal_gpio_drive_mode_t drive_mode, cyhal_gpio_direction_t direction)
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
#if defined(COMPONENT_CAT1A) || defined(COMPONENT_CAT1B)
        drvMode &= _CYHAL_GPIO_DIRECTION_OUTPUT_MASK;
#elif defined(COMPONENT_CAT2)
        drvMode |= _CYHAL_GPIO_DIRECTION_OUTPUT_MASK;
#endif
    }
    return drvMode;
}

/*******************************************************************************
*       HAL Implementation
*******************************************************************************/

cy_rslt_t cyhal_gpio_init(cyhal_gpio_t pin, cyhal_gpio_direction_t direction, cyhal_gpio_drive_mode_t drive_mode, bool init_val)
{
    /* Mbed creates GPIOs for pins that are dedicated to other peripherals in some cases. */
#ifndef __MBED__
    cyhal_resource_inst_t pinRsc = _cyhal_utils_get_gpio_resource(pin);
    cy_rslt_t status = cyhal_hwmgr_reserve(&pinRsc);
#else
    cy_rslt_t status = CY_RSLT_SUCCESS;
#endif

    if (status == CY_RSLT_SUCCESS)
    {
        uint32_t pdl_drive_mode = _cyhal_gpio_convert_drive_mode(drive_mode, direction);
        Cy_GPIO_Pin_FastInit(CYHAL_GET_PORTADDR(pin), CYHAL_GET_PIN(pin), pdl_drive_mode, init_val, HSIOM_SEL_GPIO);
    }

    return status;
}

void cyhal_gpio_free(cyhal_gpio_t pin)
{
    if (pin != CYHAL_NC_PIN_VALUE)
    {
#if defined(COMPONENT_CAT1A) || defined(COMPONENT_CAT1B)
        Cy_GPIO_SetInterruptMask(CYHAL_GET_PORTADDR(pin), CYHAL_GET_PIN(pin), 0);
#elif defined(COMPONENT_CAT2)
        Cy_GPIO_SetInterruptEdge(CYHAL_GET_PORTADDR(pin), CYHAL_GET_PIN(pin), CY_GPIO_INTR_DISABLE);
#endif
        _cyhal_gpio_callbacks[CYHAL_GET_PORT(pin)][CYHAL_GET_PIN(pin)] = NULL;
        _cyhal_gpio_callback_args[CYHAL_GET_PORT(pin)][CYHAL_GET_PIN(pin)] = NULL;

        (void)cyhal_gpio_disable_output(pin);
        #if defined(CY_IP_MXS40IOSS)
        for(uint8_t i = 0; i < (uint8_t)(sizeof(cyhal_pin_map_peri_tr_io_output)/sizeof(cyhal_resource_pin_mapping_t)); i++)
        {
            cyhal_resource_pin_mapping_t mapping = cyhal_pin_map_peri_tr_io_output[i];
            if(mapping.pin == pin)
            {
                if (CYHAL_TRIGGER_CPUSS_ZERO != _cyhal_gpio_source_signals[i])
                {
                    (void)cyhal_gpio_disconnect_digital(pin, _cyhal_gpio_source_signals[i]);
                }
            }
        }
        #endif

        Cy_GPIO_Pin_FastInit(CYHAL_GET_PORTADDR(pin), CYHAL_GET_PIN(pin), CY_GPIO_DM_ANALOG, 0UL, HSIOM_SEL_GPIO);
        /* Do not attempt to free the resource we don't reserve in mbed. */
#ifndef __MBED__
        cyhal_resource_inst_t pinRsc = _cyhal_utils_get_gpio_resource(pin);
        cyhal_hwmgr_free(&pinRsc);
#endif
    }
}

cy_rslt_t cyhal_gpio_configure(cyhal_gpio_t pin, cyhal_gpio_direction_t direction, cyhal_gpio_drive_mode_t drive_mode)
{
    uint32_t pdldrive_mode = _cyhal_gpio_convert_drive_mode(drive_mode, direction);
    Cy_GPIO_SetDrivemode(CYHAL_GET_PORTADDR(pin), CYHAL_GET_PIN(pin), pdldrive_mode);

    return CY_RSLT_SUCCESS;
}

void cyhal_gpio_register_callback(cyhal_gpio_t pin, cyhal_gpio_event_callback_t callback, void *callback_arg)
{
    uint32_t savedIntrStatus = cyhal_system_critical_section_enter();
    _cyhal_gpio_callbacks[CYHAL_GET_PORT(pin)][CYHAL_GET_PIN(pin)] = callback;
    _cyhal_gpio_callback_args[CYHAL_GET_PORT(pin)][CYHAL_GET_PIN(pin)] = callback_arg;
    cyhal_system_critical_section_exit(savedIntrStatus);
}

void cyhal_gpio_enable_event(cyhal_gpio_t pin, cyhal_gpio_event_t event, uint8_t intr_priority, bool enable)
{
    Cy_GPIO_ClearInterrupt(CYHAL_GET_PORTADDR(pin), CYHAL_GET_PIN(pin));
#if defined(COMPONENT_CAT1A) || defined(COMPONENT_CAT1B)
    Cy_GPIO_SetInterruptEdge(CYHAL_GET_PORTADDR(pin), CYHAL_GET_PIN(pin), (uint32_t)event);
    Cy_GPIO_SetInterruptMask(CYHAL_GET_PORTADDR(pin), CYHAL_GET_PIN(pin), (uint32_t)enable);
    IRQn_Type irqn = (IRQn_Type)(ioss_interrupts_gpio_0_IRQn + CYHAL_GET_PORT(pin));
#elif defined(COMPONENT_CAT2)
    uint32_t intr_val = enable ? (uint32_t)event : CY_GPIO_INTR_DISABLE;
    Cy_GPIO_SetInterruptEdge(CYHAL_GET_PORTADDR(pin), CYHAL_GET_PIN(pin), intr_val);
    IRQn_Type irqn = (ioss_interrupts_gpio_0_IRQn + CYHAL_GET_PORT(pin) < ioss_interrupt_gpio_IRQn)
                    ? (IRQn_Type)(ioss_interrupts_gpio_0_IRQn + CYHAL_GET_PORT(pin))
                    : (IRQn_Type)(ioss_interrupt_gpio_IRQn);
#endif
    /* Only enable if it's not already enabled */
    if (NVIC_GetEnableIRQ(irqn) == 0)
    {
        cy_stc_sysint_t irqCfg = {irqn, intr_priority};

        Cy_SysInt_Init(&irqCfg, _cyhal_gpio_irq_handler);
        NVIC_EnableIRQ(irqn);
    }
    else
    {
        NVIC_SetPriority(irqn, intr_priority);
    }
}

#if defined(CY_IP_MXS40IOSS)
cy_rslt_t cyhal_gpio_connect_digital(cyhal_gpio_t pin, cyhal_source_t source, cyhal_signal_type_t type)
{
    // Search through cyhal_pin_map_peri_tr_io_output to determine if a trigger
    // can be routed to it. (Note: tr_io_output refers to trigger mux output,
    // not peripheral output. A trigger mux output is routed to a peripheral
    // input.)
    for(uint8_t i = 0; i < (uint8_t)(sizeof(cyhal_pin_map_peri_tr_io_output)/sizeof(cyhal_resource_pin_mapping_t)); i++)
    {
        cyhal_resource_pin_mapping_t mapping = cyhal_pin_map_peri_tr_io_output[i];
        if(mapping.pin == pin)
        {
            Cy_GPIO_SetHSIOM(CYHAL_GET_PORTADDR(pin), CYHAL_GET_PIN(pin), mapping.hsiom);

            cyhal_dest_t dest = (cyhal_dest_t)(CYHAL_TRIGGER_PERI_TR_IO_OUTPUT0 + (uint32_t)(mapping.inst));

            cy_rslt_t rslt = _cyhal_connect_signal(source, dest, type);
            if (CY_RSLT_SUCCESS == rslt)
            {
                _cyhal_gpio_source_signals[i] = source;
            }
            return rslt;
        }
    }

    return CYHAL_GPIO_RSLT_ERR_NO_INPUT_SIGNAL;
}

cy_rslt_t cyhal_gpio_enable_output(cyhal_gpio_t pin, cyhal_source_t *source)
{
    // Search through cyhal_pin_map_peri_tr_io_input to determine if pin can be
    // used to drive a trigger line. (Note: tr_io_input refers to trigger mux
    // input, not peripheral input. A peripheral output is routed to a trigger
    // mux input.)
    for(uint8_t i = 0; i < (uint8_t)(sizeof(cyhal_pin_map_peri_tr_io_input)/sizeof(cyhal_resource_pin_mapping_t)); i++)
    {
        cyhal_resource_pin_mapping_t mapping = cyhal_pin_map_peri_tr_io_input[i];
        if(mapping.pin == pin)
        {
            Cy_GPIO_SetHSIOM(CYHAL_GET_PORTADDR(pin), CYHAL_GET_PIN(pin), mapping.hsiom);

            *source = (cyhal_source_t)(CYHAL_TRIGGER_PERI_TR_IO_INPUT0 + (uint32_t)(mapping.inst));

            return CY_RSLT_SUCCESS;
        }
    }

    return CYHAL_GPIO_RSLT_ERR_NO_OUTPUT_SIGNAL;
}

cy_rslt_t cyhal_gpio_disconnect_digital(cyhal_gpio_t pin, cyhal_source_t source)
{
    for(uint8_t i = 0; i < (uint8_t)(sizeof(cyhal_pin_map_peri_tr_io_output)/sizeof(cyhal_resource_pin_mapping_t)); i++)
    {
        cyhal_resource_pin_mapping_t mapping = cyhal_pin_map_peri_tr_io_output[i];
        if(mapping.pin == pin)
        {
            Cy_GPIO_SetHSIOM(CYHAL_GET_PORTADDR(pin), CYHAL_GET_PIN(pin), HSIOM_SEL_GPIO);

            cyhal_dest_t dest = (cyhal_dest_t)(CYHAL_TRIGGER_PERI_TR_IO_OUTPUT0 + (uint32_t)(mapping.inst));

            cy_rslt_t rslt = _cyhal_disconnect_signal(source, dest);
            if (CY_RSLT_SUCCESS == rslt)
            {
                _cyhal_gpio_source_signals[i] = CYHAL_TRIGGER_CPUSS_ZERO;
            }
            return rslt;
        }
    }

    return CYHAL_GPIO_RSLT_ERR_NO_INPUT_SIGNAL;
}

cy_rslt_t cyhal_gpio_disable_output(cyhal_gpio_t pin)
{
    for(uint8_t i = 0; i < (uint8_t)(sizeof(cyhal_pin_map_peri_tr_io_input)/sizeof(cyhal_resource_pin_mapping_t)); i++)
    {
        cyhal_resource_pin_mapping_t mapping = cyhal_pin_map_peri_tr_io_input[i];
        if(mapping.pin == pin)
        {
            Cy_GPIO_SetHSIOM(CYHAL_GET_PORTADDR(pin), CYHAL_GET_PIN(pin), HSIOM_SEL_GPIO);

            return CY_RSLT_SUCCESS;
        }
    }

    return CYHAL_GPIO_RSLT_ERR_NO_OUTPUT_SIGNAL;
}
#elif defined(CY_IP_M0S8IOSS)
// M0S8 devices do not have gpio triggers
cy_rslt_t cyhal_gpio_connect_digital(cyhal_gpio_t pin, cyhal_source_t source, cyhal_signal_type_t type)
{
    CY_UNUSED_PARAMETER(pin);
    CY_UNUSED_PARAMETER(source);
    CY_UNUSED_PARAMETER(type);
    return CYHAL_INTERCONNECT_RSLT_INVALID_CONNECTION;
}

cy_rslt_t cyhal_gpio_enable_output(cyhal_gpio_t pin, cyhal_source_t *source)
{
    CY_UNUSED_PARAMETER(pin);
    CY_UNUSED_PARAMETER(source);
    return CYHAL_INTERCONNECT_RSLT_INVALID_CONNECTION;
}

cy_rslt_t cyhal_gpio_disconnect_digital(cyhal_gpio_t pin, cyhal_source_t source)
{
    CY_UNUSED_PARAMETER(pin);
    CY_UNUSED_PARAMETER(source);
    return CYHAL_INTERCONNECT_RSLT_INVALID_CONNECTION;
}

cy_rslt_t cyhal_gpio_disable_output(cyhal_gpio_t pin)
{
    CY_UNUSED_PARAMETER(pin);
    return CYHAL_INTERCONNECT_RSLT_INVALID_CONNECTION;
}
#endif

#if defined(__cplusplus)
}
#endif /* __cplusplus */

#endif /* defined(CY_IP_MXS40IOSS) || defined(CY_IP_M0S8IOSS) */
