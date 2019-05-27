/*******************************************************************************
* File Name: cyhal_udb_sdio.c
*
* Description:
* Provides a high level interface for interacting with the Cypress UDB SDIO.
* This is a wrapper around the lower level UDB SDIO API.
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

#include "cyhal_hwmgr.h"

#if defined(CYHAL_UDB_SDIO)

#include <stdlib.h>
#include "SDIO_HOST_cfg.h"
#include "cyhal_utils.h"
#include "cyhal_sdio.h"
#include "cyhal_gpio.h"
#include "cyhal_interconnect.h"
#include "cyhal_implementation.h"

/* Not connected pin define */
#define SDIO_PINS_NC    ((cyhal_gpio_t) CYHAL_NC_PIN_VALUE)

#define CY_HAL_SDIO_CLK_DIV_VALUE   ((uint8_t) 0xFF)

/* Not configured clock divider define*/
#define CY_HAL_SDIO_CLK_DIV_NC      ((cy_en_divider_types_t) CY_HAL_SDIO_CLK_DIV_VALUE)

/* Define for default SDIO frequency */
#define  CY_HAL_SDIO_DEF_FREQ       (400000U)

/* The 64b block transition mode define */
#define CY_HAL_SDIO_64B       (64u)

/* The 1 byte transition mode define */
#define CY_HAL_SDIO_1B        (1u)

/*******************************************************************************
*       (Internal) Configuration structures for SDIO pins
*******************************************************************************/
const cy_stc_gpio_pin_config_t pin_cmd_config =
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

const cy_stc_gpio_pin_config_t pin_data_config =
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

const cy_stc_gpio_pin_config_t pin_clk_config =
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

/* Callback pointers */
static cyhal_sdio_irq_handler_t cyhal_sdio_callback = NULL;
static cyhal_sdio_t *cyhal_sdio_config_struct = NULL;
static void *cyhal_sdio_callback_args = NULL;


/*******************************************************************************
*       Dispatcher Interrupt Callbacks Service Routine
*******************************************************************************/
static void cyhal_sdio_interrupts_dispatcher_IRQHandler(void)
{
    if ((cyhal_sdio_callback != NULL) && (cyhal_sdio_config_struct->irq_cause != 0U))
    {
        (void)(cyhal_sdio_callback)(cyhal_sdio_callback_args, (cyhal_sdio_irq_event_t) cyhal_sdio_config_struct->irq_cause);
    }
}


/*******************************************************************************
*       Internal functions
*******************************************************************************/
static void cyhal_free_pins(cyhal_sdio_t *obj);

static void cyhal_free_pins(cyhal_sdio_t *obj)
{
    cyhal_gpio_free(obj->pin_clk);
    obj->pin_clk = SDIO_PINS_NC;
    cyhal_gpio_free(obj->pin_cmd);
    obj->pin_cmd = SDIO_PINS_NC;
    cyhal_gpio_free(obj->pin_data0);
    obj->pin_data0 = SDIO_PINS_NC;
    cyhal_gpio_free(obj->pin_data1);
    obj->pin_data1 = SDIO_PINS_NC;
    cyhal_gpio_free(obj->pin_data2);
    obj->pin_data2 = SDIO_PINS_NC;
    cyhal_gpio_free(obj->pin_data3);
    obj->pin_data3 = SDIO_PINS_NC;
}

static void cyhal_free_clocks(cyhal_sdio_t *obj)
{
    cyhal_resource_inst_t udbClkRsc;
    udbClkRsc.type = CYHAL_RSC_CLOCK;
    udbClkRsc.block_num = obj->clock.div_num;
    udbClkRsc.channel_num = 0;
    cyhal_hwmgr_free(&udbClkRsc);
}

static void cyhal_free_dmas(cyhal_sdio_t *obj)
{
    cyhal_resource_inst_t dmaRsc;
    dmaRsc.type = CYHAL_RSC_DMA;

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

cy_rslt_t cyhal_sdio_init(cyhal_sdio_t *obj, cyhal_gpio_t cmd, cyhal_gpio_t clk, cyhal_gpio_t data0, cyhal_gpio_t data1, cyhal_gpio_t data2, cyhal_gpio_t data3)
{
    CY_ASSERT(NULL != obj);

    /* If something go wrong, any resource not marked as invalid will be freed.
    *  Explicitly marked not allocated resources as invalid to prevent freeing
    *  them.
    *
    *  Before we reserve UDB at all we need try to reserve clock divider, then
    *  DMAs and pins. If all these resources are reserved only then reserve UDB
    *  SDIO.
    */
    obj->resource.type = CYHAL_RSC_INVALID;
    obj->pin_cmd = SDIO_PINS_NC;
    obj->pin_clk = SDIO_PINS_NC;
    obj->pin_data0 = SDIO_PINS_NC;
    obj->pin_data1 = SDIO_PINS_NC;
    obj->pin_data2 = SDIO_PINS_NC;
    obj->pin_data3 = SDIO_PINS_NC;
    obj->dma0Ch0.resource.type = CYHAL_RSC_INVALID;
    obj->dma0Ch1.resource.type = CYHAL_RSC_INVALID;
    obj->dma1Ch1.resource.type = CYHAL_RSC_INVALID;
    obj->dma1Ch3.resource.type = CYHAL_RSC_INVALID;

    cy_rslt_t retVal = CY_RSLT_SUCCESS;

    obj->clock.div_type = CY_HAL_SDIO_CLK_DIV_NC;

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
        cyhal_resource_inst_t dmaRsc = { CYHAL_RSC_DMA, 0, 0 };
        retVal = cyhal_hwmgr_reserve(&dmaRsc);
    }

    if (retVal == CY_RSLT_SUCCESS)
    {
        /* Reserve DMA0 CH1 */
        cyhal_resource_inst_t dmaRsc = { CYHAL_RSC_DMA, 0, 1 };
        retVal = cyhal_hwmgr_reserve(&dmaRsc);
    }

    if (retVal == CY_RSLT_SUCCESS)
    {
        /* Reserve DMA1 CH1 */
        cyhal_resource_inst_t dmaRsc = { CYHAL_RSC_DMA, 1, 1 };
        retVal = cyhal_hwmgr_reserve(&dmaRsc);
    }

    if (retVal == CY_RSLT_SUCCESS)
    {
        /* Reserve DMA1 CH3 */
        cyhal_resource_inst_t dmaRsc = { CYHAL_RSC_DMA, 1, 3 };
        retVal = cyhal_hwmgr_reserve(&dmaRsc);
    }

    /* Reserve the clk pin */
    if (retVal == CY_RSLT_SUCCESS)
    {
        cyhal_resource_inst_t pinRsc = cyhal_utils_get_gpio_resource(clk);
        retVal = cyhal_hwmgr_reserve(&pinRsc);
        if (retVal == CY_RSLT_SUCCESS)
        {
            obj->pin_clk = clk;
            Cy_GPIO_Pin_Init(CYHAL_GET_PORTADDR(clk), CYHAL_GET_PIN(clk), &pin_clk_config);
        }
    }

    /* Reserve the cmd pin */
    if (retVal == CY_RSLT_SUCCESS)
    {
        cyhal_resource_inst_t pinRsc = cyhal_utils_get_gpio_resource(cmd);
        retVal = cyhal_hwmgr_reserve(&pinRsc);
        if (retVal == CY_RSLT_SUCCESS)
        {
            obj->pin_cmd = cmd;
            Cy_GPIO_Pin_Init(CYHAL_GET_PORTADDR(cmd), CYHAL_GET_PIN(cmd), &pin_cmd_config);
        }
    }

    /* Reserve the data0 pin */
    if (retVal == CY_RSLT_SUCCESS)
    {
        cyhal_resource_inst_t pinRsc = cyhal_utils_get_gpio_resource(data0);
        retVal = cyhal_hwmgr_reserve(&pinRsc);
        if (retVal == CY_RSLT_SUCCESS)
        {
            obj->pin_data0 = data0;
            Cy_GPIO_Pin_Init(CYHAL_GET_PORTADDR(data0), CYHAL_GET_PIN(data0), &pin_data_config);
        }
    }

    /* Reserve the data1 pin */
    if (retVal == CY_RSLT_SUCCESS)
    {
        cyhal_resource_inst_t pinRsc = cyhal_utils_get_gpio_resource(data1);
        retVal = cyhal_hwmgr_reserve(&pinRsc);
        if (retVal == CY_RSLT_SUCCESS)
        {
            obj->pin_data1 = data1;
            Cy_GPIO_Pin_Init(CYHAL_GET_PORTADDR(data1), CYHAL_GET_PIN(data1), &pin_data_config);
        }
    }

    /* Reserve the data2 pin */
    if (retVal == CY_RSLT_SUCCESS)
    {
        cyhal_resource_inst_t pinRsc = cyhal_utils_get_gpio_resource(data2);
        retVal = cyhal_hwmgr_reserve(&pinRsc);
        if (retVal == CY_RSLT_SUCCESS)
        {
            obj->pin_data2 = data2;
            Cy_GPIO_Pin_Init(CYHAL_GET_PORTADDR(data2), CYHAL_GET_PIN(data2), &pin_data_config);
        }
    }

    /* Reserve the data3 pin */
    if (retVal == CY_RSLT_SUCCESS)
    {
        cyhal_resource_inst_t pinRsc = cyhal_utils_get_gpio_resource(data3);
        retVal = cyhal_hwmgr_reserve(&pinRsc);
        if (retVal == CY_RSLT_SUCCESS)
        {
            obj->pin_data3 = data3;
            Cy_GPIO_Pin_Init(CYHAL_GET_PORTADDR(data3), CYHAL_GET_PIN(data3), &pin_data_config);
        }
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

            /* Set default interrupt priority of 7 (lowest possible priority) */
            cy_stc_sysint_t irqCfg = {udb_interrupts_0_IRQn, 7};
            Cy_SysInt_Init(&irqCfg, &SDIO_IRQ);
            NVIC_EnableIRQ(udb_interrupts_0_IRQn);

            /* Setup write DMA interrupt handler */
            cy_stc_sysint_t irqDma1_1 = {cpuss_interrupts_dw1_1_IRQn, 7};
            Cy_SysInt_Init(&irqDma1_1, &SDIO_WRITE_DMA_IRQ);
            NVIC_EnableIRQ(cpuss_interrupts_dw1_1_IRQn);

            /* Setup read DMA interrupt handler */
            cy_stc_sysint_t irqDma1_3 = {cpuss_interrupts_dw1_3_IRQn, 7};
            Cy_SysInt_Init(&irqDma1_3, &SDIO_READ_DMA_IRQ);
            NVIC_EnableIRQ(cpuss_interrupts_dw1_3_IRQn);

            /* Connect UDB to DMA */
            Cy_TrigMux_Connect(TRIG0_IN_TR_GROUP14_OUTPUT1, TRIG0_OUT_CPUSS_DW0_TR_IN1, false, TRIGGER_TYPE_LEVEL);
            Cy_TrigMux_Connect(TRIG0_IN_TR_GROUP14_OUTPUT4, TRIG0_OUT_CPUSS_DW0_TR_IN0, false, TRIGGER_TYPE_LEVEL);
            Cy_TrigMux_Connect(TRIG1_IN_TR_GROUP14_OUTPUT0, TRIG1_OUT_CPUSS_DW1_TR_IN1, false, TRIGGER_TYPE_LEVEL);
            Cy_TrigMux_Connect(TRIG1_IN_TR_GROUP14_OUTPUT5, TRIG1_OUT_CPUSS_DW1_TR_IN3, false, TRIGGER_TYPE_LEVEL);

            Cy_TrigMux_Connect(TRIG14_IN_UDB_TR_UDB0, TRIG14_OUT_TR_GROUP1_INPUT43, false, TRIGGER_TYPE_LEVEL);
            Cy_TrigMux_Connect(TRIG14_IN_UDB_TR_UDB1, TRIG14_OUT_TR_GROUP0_INPUT44, false, TRIGGER_TYPE_LEVEL);
            Cy_TrigMux_Connect(TRIG14_IN_UDB_TR_UDB3, TRIG14_OUT_TR_GROUP0_INPUT47, false, TRIGGER_TYPE_LEVEL);
            Cy_TrigMux_Connect(TRIG14_IN_UDB_TR_UDB7, TRIG14_OUT_TR_GROUP1_INPUT48, false, TRIGGER_TYPE_LEVEL);

            stc_sdio_irq_cb_t   irq_cbs;
            irq_cbs.pfnCardIntCb = cyhal_sdio_interrupts_dispatcher_IRQHandler;

            SDIO_Init(&irq_cbs);

            /* SDIO_Init() configures the SDIO to 40 kHz */
            obj->frequencyhal_hz = CY_HAL_SDIO_DEF_FREQ;
            obj->block_size   = CY_HAL_SDIO_64B;

            /* Initialize interrupt cause */
            obj->irq_cause = 0u;

            retVal = cyhal_hwmgr_set_configured(udbRsc.type, udbRsc.block_num, udbRsc.channel_num);
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

    cyhal_free_pins(obj);
    cyhal_free_clocks(obj);
    cyhal_free_dmas(obj);
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
        SDIO_SetBlockSize(config->block_size);
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
    
    uint32_t cmdResponse;
    stc_sdio_cmd_t cmd;
    en_sdio_result_t status;
    cy_rslt_t retVal = CY_RSLT_SUCCESS;

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

    status = SDIO_SendCommandAndWait(&cmd);

    if (Ok != status)
    {
        retVal = CYHAL_SDIO_RSLT_ERR_FUNC_RET(status);
    }

    if (response != NULL)
    {
        *response = cmdResponse;
    }

    return retVal;
}

cy_rslt_t cyhal_sdio_bulk_transfer(cyhal_sdio_t *obj, cyhal_transfer_t direction, uint32_t argument, const uint32_t* data, uint16_t length, uint32_t* response)
{
    CY_ASSERT(NULL != obj);

    stc_sdio_cmd_t cmd;
    en_sdio_result_t status;
    uint32_t cmdResponse;
    cy_rslt_t retVal = CY_RSLT_SUCCESS;

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
        cmd.u16BlockCnt = CY_HAL_SDIO_1B;
        cmd.u16BlockSize = length;
    }

    status = SDIO_SendCommandAndWait(&cmd);

    if (Ok != status)
    {
        retVal = CYHAL_SDIO_RSLT_ERR_FUNC_RET(status);
    }

    if (response != NULL)
    {
        *response = cmdResponse;
    }
    return retVal;
}

cy_rslt_t cyhal_sdio_transfer_async(cyhal_sdio_t *obj, cyhal_transfer_t direction, uint32_t argument, const uint32_t* data, uint16_t length)
{
    /* UDB SDIO implementation does not support async transfers */
    
    /* Just add check to suppress warning about unused arguments */
    if ((NULL == obj) && (data == NULL) && (length == 0) && (argument == 0) && (direction == ((cyhal_transfer_t) 0x3)))
    {
        return CYHAL_SDIO_RSLT_ERR_BAD_PARAM;
    }

    return CYHAL_SDIO_FUNC_NOT_SUPPORTED;
}

bool cyhal_sdio_is_busy(const cyhal_sdio_t *obj)
{
    /* UDB SDIO does not support async transfers */
    return true;
}

cy_rslt_t cyhal_sdio_abort_async(const cyhal_sdio_t *obj)
{
    /* Reset UDB SDIO */
    SDIO_Reset();
    return CY_RSLT_SUCCESS;
}

void cyhal_sdio_register_irq(cyhal_sdio_t *obj, cyhal_sdio_irq_handler_t handler, void *handler_arg)
{
    cyhal_sdio_config_struct = obj;
    cyhal_sdio_callback = handler;
    cyhal_sdio_callback_args = handler_arg;
}

void cyhal_sdio_irq_enable(cyhal_sdio_t *obj, cyhal_sdio_irq_event_t event, bool enable)
{
    /* Only CYHAL_SDIO_CARD_INTERRUPT event can be registered */
    if (event == CYHAL_SDIO_CARD_INTERRUPT)
    {
        if (enable)
        {
            obj->irq_cause = event;

            if (cyhal_sdio_config_struct != NULL)
            {
                cyhal_sdio_config_struct->irq_cause = event;
            }
            
            SDIO_EnableChipInt();
        }
        else
        {
            obj->irq_cause = 0U;

            if (cyhal_sdio_config_struct != NULL)
            {
                cyhal_sdio_config_struct->irq_cause = 0U;
            }

            SDIO_DisableChipInt();
        }
    }
}

#endif /* defined(CYHAL_UDB_SDIO) */
