/*******************************************************************************
* File Name: cyhal_udb_sdio.c
*
* Description:
* Provides a high level interface for interacting with the Cypress UDB SDIO.
* This is a wrapper around the lower level UDB SDIO API.
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

/**
 * \addtogroup group_hal_impl_udb_sdio UDB SDIO (Secure Digital Input Output)
 * \ingroup group_hal_impl
 * \{
 * The UDB based SDIO interface allows for communicating between a PSoC 6 and a
 * Cypress wireless device such as the CYW4343W, CYW43438, or CYW43012. This
 * library allows PSoC 6 devices that do not have a dedicated SDHC hardware block,
 * but do have UDBs, to work with the
 * <a href="https://github.com/cypresssemiconductorco/wifi-host-driver">Wi-Fi
 * Host Driver (WHD)</a> library.
 *
 * \warning This library does not provide a complete SDIO implementation. It is
 * only intended for use with a Cypress wireless device. Additionally, using this
 * driver imposes a few system wide requirements, described below, that must be
 * met to work properly.
 *
 * \section section_psoc6_udb_sdio_restrictions Restrictions
 * The optimal configuration is to have ClkSlow & ClkPeri running at 100 MHz
 * and for the SDIO to run at 25 MHz. For Cypress provided Board Support Packages
 * (BSPs) that use this driver the necessary configuration is done automatically.
 *
 * To use this library, the following must be true:
 * 1. ClkSlow & ClkPeri must both run at the same speed
 * 2. ClkSlow & ClkPeri must run at 4x the desired SDIO speed
 * 3. The first 8-bit peripheral clock divider must be reserved for use by this driver
 * 4. The following DMA channels must be reserved for use by this driver
 *     * DataWire 0 channel 0
 *     * DataWire 0 channel 1
 *     * DataWire 1 channel 1
 *     * DataWire 1 channel 3
 *
 * \} group_hal_impl_udb_sdio
 */

#include "cyhal_hwmgr.h"
#include "cy_utils.h"

#if defined(CYHAL_UDB_SDIO)

#if defined(__cplusplus)
extern "C"
{
#endif

#include <stdlib.h>
#include "SDIO_HOST_cfg.h"
#include "cyhal_utils.h"
#include "cyhal_sdio.h"
#include "cyhal_gpio.h"
#include "cyhal_interconnect.h"
#include "cyhal_syspm.h"

#define _CY_HAL_SDIO_CLK_DIV_VALUE   ((uint8_t) 0xFF)

/* Not configured clock divider define*/
#define _CY_HAL_SDIO_CLK_DIV_NC      ((cy_en_divider_types_t) _CY_HAL_SDIO_CLK_DIV_VALUE)

/* Define for default SDIO frequency */
#define  _CY_HAL_SDIO_DEF_FREQ       (400000U)

/* The 64b block transition mode define */
#define _CY_HAL_SDIO_64B       (64u)

/* The 1 byte transition mode define */
#define _CY_HAL_SDIO_1B        (1u)

/* Mask which indicates interface change */
#define _CYHAL_SDIO_INTERFACE_CHANGE_MASK    ((uint32_t) ((uint32_t) CYHAL_SDIO_GOING_DOWN) | ((uint32_t) CYHAL_SDIO_COMING_UP))

#if !defined(CYHAL_SDIO_DS_CB_ORDER)
    /* The order value for SDIO Deep Sleep callback */
    #define CYHAL_SDIO_DS_CB_ORDER        (0u)
 #endif  /* !defined(CYHAL_SDIO_DS_CB_ORDER) */

/* Callback pointers */
static cyhal_sdio_event_callback_t _cyhal_sdio_callback = NULL;
static cyhal_sdio_t *_cyhal_sdio_config_struct = NULL;
static void *_cyhal_sdio_callback_args = NULL;
static bool _cyhal_sdio_op_pending = false;

/*******************************************************************************
*       (Internal) Configuration structures for SDIO pins
*******************************************************************************/
static const cy_stc_gpio_pin_config_t _cyhal_sdio_pin_config =
{
    .outVal = 1,
    .driveMode = CY_GPIO_DM_STRONG,
    .hsiom = HSIOM_SEL_DSI_DSI, /* DSI controls 'out' and 'output enable' */
    .intEdge = CY_GPIO_INTR_DISABLE,
    .intMask = 0UL,
    .vtrip = CY_GPIO_VTRIP_CMOS,
    .slewRate = CY_GPIO_SLEW_FAST,
    .driveSel = CY_GPIO_DRIVE_1_2,
    .vregEn = 0UL,
    .ibufMode = 0UL,
    .vtripSel = 0UL,
    .vrefSel = 0UL,
    .vohSel = 0UL,
};

static const cy_stc_gpio_pin_config_t _cyhal_sdio_pin_clk_config =
{
    .outVal = 1,
    .driveMode = CY_GPIO_DM_STRONG_IN_OFF,
    .hsiom = HSIOM_SEL_DSI_GPIO, /* DSI controls 'out', GPIO controls 'output enable' */
    .intEdge = CY_GPIO_INTR_DISABLE,
    .intMask = 0UL,
    .vtrip = CY_GPIO_VTRIP_CMOS,
    .slewRate = CY_GPIO_SLEW_FAST,
    .driveSel = CY_GPIO_DRIVE_1_2,
    .vregEn = 0UL,
    .ibufMode = 0UL,
    .vtripSel = 0UL,
    .vrefSel = 0UL,
    .vohSel = 0UL,
};

/*******************************************************************************
*       Internal functions
*******************************************************************************/

/* Internal deep sleep callback, which does following:
*  1. Save/restore not retained configuration registers in the Deep Sleep
*  2. Execute registered callback with CYHAL_SDIO_GOING_DOWN event, before
*     entering into Deep Sleep
*  3. Execute registered callback with CYHAL_SDIO_COMING_UP event, after
*     exit from Deep Sleep
* */
static bool _cyhal_sdio_ds_callback(cyhal_syspm_callback_state_t state, cyhal_syspm_callback_mode_t mode, void* callback_arg)
{
    static cy_stc_syspm_callback_params_t cyhal_sdio_pm_callback_params = { NULL, NULL };
    bool allow = true;
    cyhal_sdio_t *obj = (cyhal_sdio_t *)callback_arg;
    CY_ASSERT(obj != NULL);

    if(state == CYHAL_SYSPM_CB_CPU_DEEPSLEEP)
    {
        allow = (SDIO_DeepSleepCallback(&cyhal_sdio_pm_callback_params, _cyhal_utils_convert_haltopdl_pm_mode(mode)) == CY_SYSPM_SUCCESS);
    }

    if (allow)
    {
        switch (mode)
        {
            case CYHAL_SYSPM_CHECK_READY:
            {
                /* Check if transfer is pending */
                allow = !_cyhal_sdio_op_pending;
                if (allow)
                {
                    /* Execute callback to indicate that interface is going down */
                    if ((_cyhal_sdio_callback != NULL) && (0U != (_cyhal_sdio_config_struct->events & (uint32_t) CYHAL_SDIO_GOING_DOWN)))
                    {
                        (void)(_cyhal_sdio_callback)(_cyhal_sdio_callback_args, CYHAL_SDIO_GOING_DOWN);
                    }

                    /* Indicate Deep Sleep entering */
                    obj->pm_transition_pending = true;
                }

                break;
            }

            case CYHAL_SYSPM_BEFORE_TRANSITION:
            {
                /* Nothing to do */
                break;
            }

            case CYHAL_SYSPM_AFTER_TRANSITION:
            case CYHAL_SYSPM_CHECK_FAIL:
            {
                /* Execute this only if check ready case was executed */
                if (obj->pm_transition_pending)
                {
                    /* Execute callback to indicate that interface is coming up */
                    if ((_cyhal_sdio_callback != NULL) && (0U != (_cyhal_sdio_config_struct->events & (uint32_t) CYHAL_SDIO_COMING_UP)))
                    {
                        (void)(_cyhal_sdio_callback)(_cyhal_sdio_callback_args, CYHAL_SDIO_COMING_UP);
                    }

                    /* Indicate PM mode transition exit */
                    obj->pm_transition_pending = false;
                }
                break;
            }

            default:
                CY_ASSERT(false);
                break;
        }
    }
    return allow;
}


/*******************************************************************************
*       Dispatcher Interrupt Callbacks Service Routine
*******************************************************************************/
static void _cyhal_sdio_interrupts_dispatcher_IRQHandler(void)
{
    /* Only CYHAL_SDIO_CARD_INTERRUPT event can be registered and executed */
    if ((_cyhal_sdio_callback != NULL) && (_cyhal_sdio_config_struct->irq_cause != 0U))
    {
        (void)(_cyhal_sdio_callback)(_cyhal_sdio_callback_args, CYHAL_SDIO_CARD_INTERRUPT);
    }
}

static void _cyhal_sdio_free_clocks(cyhal_sdio_t *obj)
{
    cyhal_resource_inst_t udbClkRsc;
    udbClkRsc.type = CYHAL_RSC_CLOCK;
    udbClkRsc.block_num = obj->clock.div_num;
    udbClkRsc.channel_num = 0;
    cyhal_hwmgr_free(&udbClkRsc);
}

static void _cyhal_sdio_free_dmas()
{
    cyhal_resource_inst_t dmaRsc;
    dmaRsc.type = CYHAL_RSC_DW;

    dmaRsc.block_num = 0;
    dmaRsc.channel_num = 0;
    cyhal_hwmgr_free(&dmaRsc);

    dmaRsc.block_num = 0;
    dmaRsc.channel_num = 1;
    cyhal_hwmgr_free(&dmaRsc);

    dmaRsc.block_num = 1;
    dmaRsc.channel_num = 1;
    cyhal_hwmgr_free(&dmaRsc);

    dmaRsc.block_num = 1;
    dmaRsc.channel_num = 3;
    cyhal_hwmgr_free(&dmaRsc);
}

static cy_rslt_t _cyhal_sdio_configure_pin(
    cyhal_gpio_t pin, cyhal_gpio_t *pin_ref, const cy_stc_gpio_pin_config_t* cfg)
{
    cyhal_resource_inst_t pinRsc = _cyhal_utils_get_gpio_resource(pin);
    cy_rslt_t result = cyhal_hwmgr_reserve(&pinRsc);
    if (result == CY_RSLT_SUCCESS)
    {
        *pin_ref = pin;
        Cy_GPIO_Pin_Init(CYHAL_GET_PORTADDR(pin), CYHAL_GET_PIN(pin), cfg);
    }
    return result;
}

cy_rslt_t cyhal_sdio_init(cyhal_sdio_t *obj, cyhal_gpio_t cmd, cyhal_gpio_t clk, cyhal_gpio_t data0, cyhal_gpio_t data1, cyhal_gpio_t data2, cyhal_gpio_t data3)
{
    CY_ASSERT(NULL != obj);

    /* If something goes wrong, any resource not marked as invalid will be freed.
    *  We explicitly mark unallocated resources as invalid to prevent freeing
    *  them.
    *
    *  Before we reserve UDB at all we need try to reserve clock divider, then
    *  DMAs and pins. If all these resources are reserved only then reserve UDB
    *  SDIO.
    */
    obj->resource.type = CYHAL_RSC_INVALID;
    obj->pin_cmd = CYHAL_NC_PIN_VALUE;
    obj->pin_clk = CYHAL_NC_PIN_VALUE;
    obj->pin_data0 = CYHAL_NC_PIN_VALUE;
    obj->pin_data1 = CYHAL_NC_PIN_VALUE;
    obj->pin_data2 = CYHAL_NC_PIN_VALUE;
    obj->pin_data3 = CYHAL_NC_PIN_VALUE;
    obj->dma0Ch0.resource.type = CYHAL_RSC_INVALID;
    obj->dma0Ch1.resource.type = CYHAL_RSC_INVALID;
    obj->dma1Ch1.resource.type = CYHAL_RSC_INVALID;
    obj->dma1Ch3.resource.type = CYHAL_RSC_INVALID;

    cy_rslt_t retVal = CY_RSLT_SUCCESS;

    obj->clock.div_type = _CY_HAL_SDIO_CLK_DIV_NC;

    /* Reserve clock */
    cyhal_resource_inst_t udbClkRsc = { CYHAL_RSC_CLOCK, 0, 0 };
    retVal = cyhal_hwmgr_reserve(&udbClkRsc);
    if (retVal == CY_RSLT_SUCCESS)
    {
        /* Assign clock divider */
        obj->clock.div_type = CY_SYSCLK_DIV_8_BIT;
        obj->clock.div_num = 0;

        retVal = Cy_SysClk_PeriphSetDivider(obj->clock.div_type, obj->clock.div_num, 0U);

        if (CY_SYSCLK_SUCCESS == retVal)
        {
            retVal = Cy_SysClk_PeriphEnableDivider(obj->clock.div_type, obj->clock.div_num);
        }

        if (CY_SYSCLK_SUCCESS == retVal)
        {
            retVal = Cy_SysClk_PeriphAssignDivider(PCLK_UDB_CLOCKS0, obj->clock.div_type, obj->clock.div_num);
        }
    }

    /* The DMAs are initialized in SDIO_Init() function, so only reserve */
    if (retVal == CY_RSLT_SUCCESS)
    {
        /* Reserve DMA0 CH0 */
        cyhal_resource_inst_t dmaRsc = { CYHAL_RSC_DW, 0, 0 };
        retVal = cyhal_hwmgr_reserve(&dmaRsc);
    }

    if (retVal == CY_RSLT_SUCCESS)
    {
        /* Reserve DMA0 CH1 */
        cyhal_resource_inst_t dmaRsc = { CYHAL_RSC_DW, 0, 1 };
        retVal = cyhal_hwmgr_reserve(&dmaRsc);
    }

    if (retVal == CY_RSLT_SUCCESS)
    {
        /* Reserve DMA1 CH1 */
        cyhal_resource_inst_t dmaRsc = { CYHAL_RSC_DW, 1, 1 };
        retVal = cyhal_hwmgr_reserve(&dmaRsc);
    }

    if (retVal == CY_RSLT_SUCCESS)
    {
        /* Reserve DMA1 CH3 */
        cyhal_resource_inst_t dmaRsc = { CYHAL_RSC_DW, 1, 3 };
        retVal = cyhal_hwmgr_reserve(&dmaRsc);
    }

    /* Reserve the clk, cmd & 4 data pins */
    if (retVal == CY_RSLT_SUCCESS)
    {
        retVal = _cyhal_sdio_configure_pin(clk, &obj->pin_clk, &_cyhal_sdio_pin_clk_config);
    }
    if (retVal == CY_RSLT_SUCCESS)
    {
        retVal = _cyhal_sdio_configure_pin(cmd, &obj->pin_cmd, &_cyhal_sdio_pin_config);
    }
    if (retVal == CY_RSLT_SUCCESS)
    {
        retVal = _cyhal_sdio_configure_pin(data0, &obj->pin_data0, &_cyhal_sdio_pin_config);
    }
    if (retVal == CY_RSLT_SUCCESS)
    {
        retVal = _cyhal_sdio_configure_pin(data1, &obj->pin_data1, &_cyhal_sdio_pin_config);
    }
    if (retVal == CY_RSLT_SUCCESS)
    {
        retVal = _cyhal_sdio_configure_pin(data2, &obj->pin_data2, &_cyhal_sdio_pin_config);
    }
    if (retVal == CY_RSLT_SUCCESS)
    {
        retVal = _cyhal_sdio_configure_pin(data3, &obj->pin_data3, &_cyhal_sdio_pin_config);
    }

    /* Reserve UDB SDIO */
    if (retVal == CY_RSLT_SUCCESS)
    {
        /* UDB SDIO resource */
        cyhal_resource_inst_t udbRsc = { CYHAL_RSC_UDB, 0, 0 };

        retVal = cyhal_hwmgr_reserve(&udbRsc);
        if (retVal == CY_RSLT_SUCCESS)
        {
            /* Update SDIO object */
            obj->resource.type = udbRsc.type;
            obj->resource.block_num = udbRsc.block_num;
            obj->resource.channel_num = udbRsc.channel_num;

            /* Set default interrupt priority to default value */
            cy_stc_sysint_t irqCfg = { udb_interrupts_0_IRQn, CYHAL_ISR_PRIORITY_DEFAULT };
            Cy_SysInt_Init(&irqCfg, &SDIO_IRQ);
            NVIC_EnableIRQ(udb_interrupts_0_IRQn);

            /* Setup write DMA interrupt handler */
            cy_stc_sysint_t irqDma1_1 = { cpuss_interrupts_dw1_1_IRQn, CYHAL_ISR_PRIORITY_DEFAULT };
            Cy_SysInt_Init(&irqDma1_1, &SDIO_WRITE_DMA_IRQ);
            NVIC_EnableIRQ(cpuss_interrupts_dw1_1_IRQn);

            /* Setup read DMA interrupt handler */
            cy_stc_sysint_t irqDma1_3 = { cpuss_interrupts_dw1_3_IRQn, CYHAL_ISR_PRIORITY_DEFAULT };
            Cy_SysInt_Init(&irqDma1_3, &SDIO_READ_DMA_IRQ);
            NVIC_EnableIRQ(cpuss_interrupts_dw1_3_IRQn);

            stc_sdio_irq_cb_t   irq_cbs;
            irq_cbs.pfnCardIntCb = _cyhal_sdio_interrupts_dispatcher_IRQHandler;

            SDIO_Init(&irq_cbs);

            /* SDIO_Init() configures the SDIO to 40 kHz */
            obj->frequencyhal_hz = _CY_HAL_SDIO_DEF_FREQ;
            obj->block_size   = _CY_HAL_SDIO_64B;

            /* Initialize interrupt cause and events */
            obj->irq_cause = 0u;
            obj->events = 0u;

            /* Register SDIO Deep Sleep Callback */
            if (CY_RSLT_SUCCESS == retVal)
            {
                obj->pm_callback_data.callback = &_cyhal_sdio_ds_callback,
                obj->pm_callback_data.states = (cyhal_syspm_callback_state_t)(CYHAL_SYSPM_CB_CPU_DEEPSLEEP | CYHAL_SYSPM_CB_SYSTEM_HIBERNATE);
                obj->pm_callback_data.next = NULL;
                obj->pm_callback_data.args = obj;
                /* The CYHAL_SYSPM_BEFORE_TRANSITION mode cannot be ignored because the PM handler
                 * calls the SDIO host driver callback that saves UDB state in this mode before transitioning.
                 */
                obj->pm_callback_data.ignore_modes = (cyhal_syspm_callback_mode_t)0;

                _cyhal_syspm_register_peripheral_callback(&obj->pm_callback_data);
            }
        }
    }

    if (retVal != CY_RSLT_SUCCESS)
    {
        cyhal_sdio_free(obj);
    }

    return retVal;
}

void cyhal_sdio_free(cyhal_sdio_t *obj)
{
    CY_ASSERT(NULL != obj);

    NVIC_DisableIRQ(udb_interrupts_0_IRQn);
    NVIC_DisableIRQ(cpuss_interrupts_dw1_1_IRQn);
    NVIC_DisableIRQ(cpuss_interrupts_dw1_3_IRQn);

    _cyhal_utils_release_if_used(&(obj->pin_clk));
    _cyhal_utils_release_if_used(&(obj->pin_cmd));
    _cyhal_utils_release_if_used(&(obj->pin_data0));
    _cyhal_utils_release_if_used(&(obj->pin_data1));
    _cyhal_utils_release_if_used(&(obj->pin_data2));
    _cyhal_utils_release_if_used(&(obj->pin_data3));

    _cyhal_sdio_free_clocks(obj);
    _cyhal_sdio_free_dmas();
    cyhal_hwmgr_free(&(obj->resource));
    SDIO_Free();

    _cyhal_syspm_unregister_peripheral_callback(&obj->pm_callback_data);
}

cy_rslt_t cyhal_sdio_configure(cyhal_sdio_t *obj, const cyhal_sdio_cfg_t *config)
{
    CY_ASSERT(NULL != obj);

    /* Do not change frequency if requested value is zero */
    if (config->frequencyhal_hz != 0)
    {
        SDIO_SetSdClkFrequency(config->frequencyhal_hz);
        obj->frequencyhal_hz = config->frequencyhal_hz;
    }

    /* Do not change block size if requested value is zero */
    if (config->block_size != 0)
    {
        SDIO_SetBlockSize((uint8_t)config->block_size);
        obj->block_size   = config->block_size;
    }

    return CY_RSLT_SUCCESS;
}

cy_rslt_t cyhal_sdio_send_cmd(const cyhal_sdio_t *obj, cyhal_transfer_t direction, cyhal_sdio_command_t command, uint32_t argument, uint32_t* response)
{
    CY_ASSERT(NULL != obj);
    if (command == CYHAL_SDIO_CMD_IO_RW_EXTENDED)
    {
        return CYHAL_SDIO_RSLT_ERR_BAD_PARAM;
    }

    if (obj->pm_transition_pending)
    {
        return CYHAL_SDIO_RSLT_ERR_PM_PENDING;
    }

    uint32_t cmdResponse;
    stc_sdio_cmd_t cmd;
    en_sdio_result_t status;
    cy_rslt_t retVal = CYHAL_SDIO_RSLT_CANCELED;

    /* Check other pending operations */
    if (!_cyhal_sdio_op_pending)
    {
        /* Indicate pending operation to prevent entering into Deep Sleep */
        _cyhal_sdio_op_pending = true;

        if (response != NULL)
        {
            *response = 0;
        }

        cmd.u32CmdIdx = (uint32_t) command;
        cmd.u32Arg = argument;
        cmd.pu32Response = &cmdResponse;
        cmd.pu8Data = NULL;              /* Not used */
        cmd.bRead = (direction != CYHAL_READ) ? false : true;
        cmd.u16BlockCnt = 0U;            /* Not used */
        cmd.u16BlockSize = 0U;           /* Not used */

        /* Send command only if there is no attempts to enter into Deep Sleep */

        status = SDIO_SendCommandAndWait(&cmd);

        if (Ok != status)
        {
            retVal = CYHAL_SDIO_RSLT_ERR_FUNC_RET(status);
        }
        else
        {
            retVal = CY_RSLT_SUCCESS;
        }

        if (response != NULL)
        {
            *response = cmdResponse;
        }


        /*  Indicate finished operation */
        _cyhal_sdio_op_pending = false;
    }

    return retVal;
}

cy_rslt_t cyhal_sdio_bulk_transfer(cyhal_sdio_t *obj, cyhal_transfer_t direction, uint32_t argument, const uint32_t* data, uint16_t length, uint32_t* response)
{
    CY_ASSERT(NULL != obj);
    if (obj->pm_transition_pending)
    {
        return CYHAL_SDIO_RSLT_ERR_PM_PENDING;
    }

    cy_rslt_t retVal = CYHAL_SDIO_RSLT_CANCELED;

    /* Check other pending operations */
    if (!_cyhal_sdio_op_pending)
    {
        /* Indicate pending operation to prevent entering into Deep Sleep */
        _cyhal_sdio_op_pending = true;
        stc_sdio_cmd_t cmd;
        en_sdio_result_t status;
        uint32_t cmdResponse;

        if (response != NULL)
        {
            *response = 0;
        }

        cmd.u32CmdIdx = (uint32_t) CYHAL_SDIO_CMD_IO_RW_EXTENDED;
        cmd.u32Arg = argument;
        cmd.pu32Response = &cmdResponse;

        /* Note that this implementation uses 8b address */
        cmd.pu8Data = (uint8_t *) data;
        cmd.bRead = (direction != CYHAL_READ) ? false : true;

        if (length >= obj->block_size)
        {
            cmd.u16BlockCnt = (uint16_t) ((length + obj->block_size - 1)/obj->block_size);
            cmd.u16BlockSize = obj->block_size;
        }
        else
        {
            /* Data will be sent as one packet */
            cmd.u16BlockCnt = _CY_HAL_SDIO_1B;
            cmd.u16BlockSize = length;
        }

        status = SDIO_SendCommandAndWait(&cmd);

        if (Ok != status)
        {
            retVal = CYHAL_SDIO_RSLT_ERR_FUNC_RET(status);
        }
        else
        {
            retVal = CY_RSLT_SUCCESS;
        }

        if (response != NULL)
        {
            *response = cmdResponse;
        }

        /*  Indicate finished transfer */
        _cyhal_sdio_op_pending = false;
    }

    return retVal;
}

cy_rslt_t cyhal_sdio_transfer_async(cyhal_sdio_t *obj, cyhal_transfer_t direction, uint32_t argument, const uint32_t* data, uint16_t length)
{
    /* UDB SDIO implementation does not support async transfers */

    CY_UNUSED_PARAMETER(obj);
    CY_ASSERT(NULL != obj);
    /* Just add check to suppress warning about unused arguments */
    if ((data == NULL) && (length == 0) && (argument == 0) && (direction == ((cyhal_transfer_t) 0x3)))
    {
        return CYHAL_SDIO_RSLT_ERR_BAD_PARAM;
    }

    return CYHAL_SDIO_FUNC_NOT_SUPPORTED;
}

bool cyhal_sdio_is_busy(const cyhal_sdio_t *obj)
{
    /* UDB SDIO does not support async transfers */
    CY_UNUSED_PARAMETER(obj);
    return false;
}

cy_rslt_t cyhal_sdio_abort_async(const cyhal_sdio_t *obj)
{
    /* Reset UDB SDIO */
    CY_UNUSED_PARAMETER(obj);
    SDIO_Reset();
    return CY_RSLT_SUCCESS;
}

void cyhal_sdio_register_callback(cyhal_sdio_t *obj, cyhal_sdio_event_callback_t callback, void *callback_arg)
{
    _cyhal_sdio_config_struct = obj;
    _cyhal_sdio_callback      = callback;
    _cyhal_sdio_callback_args = callback_arg;
}

void cyhal_sdio_enable_event(cyhal_sdio_t *obj, cyhal_sdio_event_t event, uint8_t intr_priority, bool enable)
{
    /* Only CYHAL_SDIO_CARD_INTERRUPT event can be registered */
    if (event == CYHAL_SDIO_CARD_INTERRUPT)
    {
        if (enable)
        {
            obj->irq_cause = event;

            if (_cyhal_sdio_config_struct != NULL)
            {
                _cyhal_sdio_config_struct->irq_cause = event;
            }

            SDIO_EnableChipInt();
        }
        else
        {
            obj->irq_cause = 0U;

            if (_cyhal_sdio_config_struct != NULL)
            {
                _cyhal_sdio_config_struct->irq_cause = 0U;
            }

            SDIO_DisableChipInt();
        }
    }

    /* Enable/disable non-interrupt based event */
    if (0u != ((uint32_t) event & _CYHAL_SDIO_INTERFACE_CHANGE_MASK))
    {
        if (enable)
        {
            obj->events |= (uint32_t) event;
        }
        else
        {
            obj->events &= (uint32_t) ~((uint32_t) event);
        }

        _cyhal_sdio_config_struct->events = obj->events;
    }

    NVIC_SetPriority(udb_interrupts_0_IRQn, intr_priority);
}

#if defined(__cplusplus)
}
#endif

#endif /* defined(CYHAL_UDB_SDIO) */
