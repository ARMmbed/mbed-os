/*******************************************************************************
* File Name: cyhal_sdhc.c
*
* Description:
* Provides a high level interface for interacting with the Cypress SDHC. This
* is a wrapper around the lower level PDL API.
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

#include <string.h> /* For memcpy */
#include <stdlib.h>
#include "cy_pdl.h"
#include "cy_utils.h"
#include "cy_result.h"
#include "cyhal_sdhc.h"
#include "cyhal_sdio.h"
#include "cyhal_gpio.h"
#include "cyhal_clock.h"
#include "cyhal_hwmgr.h"
#include "cyhal_utils.h"
#include "cyhal_system.h"
#include "cyhal_syspm.h"

#ifdef CY_IP_MXSDHC

#if defined(__cplusplus)
extern "C"
{
#endif

#define _CYHAL_SDHC_IRQ_PRIORITY                        (3UL)
#define _CYHAL_SDHC_RW_RETRY_CYCLES                     (1000u)   /* Number of cycles for read/write operation complete */
#define _CYHAL_SDHC_RETRY_TIMES                         (1000UL)  /* The number loops to make the timeout in msec */
#define _CYHAL_SDHC_FUJE_TIMEOUT_MS                     (1000U)   /* The Fuje timeout for one block */
#define _CYHAL_SDHC_RW_TIMEOUT_US                       (500U)    /* The SDHC Read/Write timeout for one block */
#define _CYHAL_SDHC_TRANSFER_TIMEOUT                    (0xCUL)   /* The transfer timeout */
#define _CYHAL_SDHC_EMMC_TRIM_DELAY_MS                  (100U)    /* The EMMC TRIM timeout */
#define _CYHAL_SDIO_ENUMERATION_TIMEOUT_MS              (500U)

#define _CYHAL_SDIO_RW_TIMEOUT_US                       (5U)      /* The SDIO Read/Write timeout for one block */
#define _CYHAL_SDIO_CMD_CMPLT_DELAY_US                  (5U)      /* The Command complete delay */
#define _CYHAL_SDIO_HOST_CLK_400K                       (400UL * 1000UL)    /* 400 kHz clock frequency */
#define _CYHAL_SDIO_64B_BLOCK                           (64U)


#define _CYHAL_SDIO_SET_ALL_INTERRUPTS_MASK             (0x61FFUL)
#define _CYHAL_SDIO_ALL_INTERRUPTS_ENABLE_MASK          (0x61FFUL)
#define _CYHAL_SDIO_CLEAR_ALL_INTERRUPTS_MASK           (0x0UL)
#define _CYHAL_SDIO_CLEAR_ALL_INTERRUPTS_ENABLE_MASK    (0x0UL)
#define _CYHAL_SDIO_TRANSFER_TRIES                      (50U)


/* Clock frequency which is connected to SDHC divider */
#define _CYHAL_SDIO_CLK_HF_HZ                           (100000000UL)

/* Macro-function to calculate pin mapping number */
#define _CYHAL_SDHC_ELEM_COUNT(pin_mapping)    (sizeof(pin_mapping)/sizeof(cyhal_resource_pin_mapping_t))

#if (defined(SDHC_CHIP_TOP_DATA8_PRESENT) && (SDHC_CHIP_TOP_DATA8_PRESENT))   || \
    (defined(SDHC0_CHIP_TOP_DATA8_PRESENT) && (SDHC0_CHIP_TOP_DATA8_PRESENT)) || \
    (defined(SDHC1_CHIP_TOP_DATA8_PRESENT) && (SDHC1_CHIP_TOP_DATA8_PRESENT))
    #define _CYHAL_SDHC_DATA8_PRESENT   1
#else
    #define _CYHAL_SDHC_DATA8_PRESENT   0
#endif
#if (defined(SDHC_CHIP_TOP_CARD_DETECT_PRESENT) && (SDHC_CHIP_TOP_CARD_DETECT_PRESENT))   || \
    (defined(SDHC0_CHIP_TOP_CARD_DETECT_PRESENT) && (SDHC0_CHIP_TOP_CARD_DETECT_PRESENT)) || \
    (defined(SDHC1_CHIP_TOP_CARD_DETECT_PRESENT) && (SDHC1_CHIP_TOP_CARD_DETECT_PRESENT))
    #define _CYHAL_SDHC_CARD_DETECT_PRESENT   1
#else
    #define _CYHAL_SDHC_CARD_DETECT_PRESENT   0
#endif
#if (defined(SDHC_CHIP_TOP_CARD_WRITE_PROT_PRESENT) && (SDHC_CHIP_TOP_CARD_WRITE_PROT_PRESENT))   || \
    (defined(SDHC0_CHIP_TOP_CARD_WRITE_PROT_PRESENT) && (SDHC0_CHIP_TOP_CARD_WRITE_PROT_PRESENT)) || \
    (defined(SDHC1_CHIP_TOP_CARD_WRITE_PROT_PRESENT) && (SDHC1_CHIP_TOP_CARD_WRITE_PROT_PRESENT))
    #define _CYHAL_SDHC_CARD_WRITE_PROT_PRESENT   1
#else
    #define _CYHAL_SDHC_CARD_WRITE_PROT_PRESENT   0
#endif
#if (defined(SDHC_CHIP_TOP_LED_CTRL_PRESENT) && (SDHC_CHIP_TOP_LED_CTRL_PRESENT))   || \
    (defined(SDHC0_CHIP_TOP_LED_CTRL_PRESENT) && (SDHC0_CHIP_TOP_LED_CTRL_PRESENT)) || \
    (defined(SDHC1_CHIP_TOP_LED_CTRL_PRESENT) && (SDHC1_CHIP_TOP_LED_CTRL_PRESENT))
    #define _CYHAL_SDHC_LED_CTRL_PRESENT   1
#else
    #define _CYHAL_SDHC_LED_CTRL_PRESENT   0
#endif
#if (defined(SDHC_CHIP_TOP_IO_VOLT_SEL_PRESENT) && (SDHC_CHIP_TOP_IO_VOLT_SEL_PRESENT))   || \
    (defined(SDHC0_CHIP_TOP_IO_VOLT_SEL_PRESENT) && (SDHC0_CHIP_TOP_IO_VOLT_SEL_PRESENT)) || \
    (defined(SDHC1_CHIP_TOP_IO_VOLT_SEL_PRESENT) && (SDHC1_CHIP_TOP_IO_VOLT_SEL_PRESENT))
    #define _CYHAL_SDHC_IO_VOLT_SEL_PRESENT   1
#else
    #define _CYHAL_SDHC_IO_VOLT_SEL_PRESENT   0
#endif
#if (defined(SDHC_CHIP_TOP_CARD_IF_PWR_EN_PRESENT) && (SDHC_CHIP_TOP_CARD_IF_PWR_EN_PRESENT))   || \
    (defined(SDHC0_CHIP_TOP_CARD_IF_PWR_EN_PRESENT) && (SDHC0_CHIP_TOP_CARD_IF_PWR_EN_PRESENT)) || \
    (defined(SDHC1_CHIP_TOP_CARD_IF_PWR_EN_PRESENT) && (SDHC1_CHIP_TOP_CARD_IF_PWR_EN_PRESENT))
    #define _CYHAL_SDHC_CARD_IF_PWR_EN_PRESENT   1
#else
    #define _CYHAL_SDHC_CARD_IF_PWR_EN_PRESENT   0
#endif
#if (defined(SDHC_CHIP_TOP_CARD_EMMC_RESET_PRESENT) && (SDHC_CHIP_TOP_CARD_EMMC_RESET_PRESENT))   || \
    (defined(SDHC0_CHIP_TOP_CARD_EMMC_RESET_PRESENT) && (SDHC0_CHIP_TOP_CARD_EMMC_RESET_PRESENT)) || \
    (defined(SDHC1_CHIP_TOP_CARD_EMMC_RESET_PRESENT) && (SDHC1_CHIP_TOP_CARD_EMMC_RESET_PRESENT))
    #define _CYHAL_SDHC_CARD_EMMC_RESET_PRESENT   1
#else
    #define _CYHAL_SDHC_CARD_EMMC_RESET_PRESENT   0
#endif

/* Mask which indicates interface change */
#define _CYHAL_SDIO_INTERFACE_CHANGE_MASK    ((uint32_t) ((uint32_t) CYHAL_SDIO_GOING_DOWN) | ((uint32_t) CYHAL_SDIO_COMING_UP))
const uint32_t MAX_FREQUENCY = 50000000;

/* List of available SDHC instances */
static SDHC_Type * const _CYHAL_SDHC_BASE_ADDRESSES[CY_IP_MXSDHC_INSTANCES] =
{
#ifdef SDHC0
    SDHC0,
#endif /* ifdef SDHC0 */

#ifdef SDHC1
    SDHC1,
#endif /* ifdef SDHC1 */
};


/* List of available SDHC interrupt sources */
static const IRQn_Type _CYHAL_SDHC_IRQ_N[CY_IP_MXSDHC_INSTANCES] =
{
#ifdef SDHC0
    sdhc_0_interrupt_general_IRQn,
#endif /* ifdef SDHC0 */

#ifdef SDHC1
    sdhc_1_interrupt_general_IRQn,
#endif /* ifdef SDHC1 */
};

static uint8_t _cyhal_sdhc_get_block_from_irqn(IRQn_Type irqn)
{
    switch (irqn)
    {
#if (CY_CPU_CORTEX_M4)
    #if (CY_IP_MXSDHC_INSTANCES > 0)
        case sdhc_0_interrupt_general_IRQn: return 0;
    #endif

    #if (CY_IP_MXSDHC_INSTANCES > 1)
        case sdhc_1_interrupt_general_IRQn: return 1;
    #endif

    #if (CY_IP_MXSDHC_INSTANCES > 2)
        #error "Unhandled SDHC count"
    #endif
#endif /* (CY_CPU_CORTEX_M4) */
    default:
        CY_ASSERT(false); /* Should never be called with a non-SDHC IRQn */
        return 0;
    }
}

static void *_cyhal_sdhc_config_structs[CY_IP_MXSDHC_INSTANCES];

/* Structure to map SDHC events on SDHC interrupts */
static const uint32_t _cyhal_sdhc_event_map[] =
{
    (uint32_t)CYHAL_SDHC_ERR_INTERRUPT,   // Default error if nothing is recognized
    (uint32_t)CYHAL_SDHC_CMD_COMPLETE,    // CY_SD_HOST_CMD_COMPLETE
    (uint32_t)CYHAL_SDHC_XFER_COMPLETE,   // CY_SD_HOST_XFER_COMPLETE
    (uint32_t)CYHAL_SDHC_BGAP_EVENT,      // CY_SD_HOST_BGAP
    (uint32_t)CYHAL_SDHC_DMA_INTERRUPT,   // CY_SD_HOST_DMA_INTERRUPT
    (uint32_t)CYHAL_SDHC_BUF_WR_READY,    // CY_SD_HOST_BUF_WR_READY
    (uint32_t)CYHAL_SDHC_BUF_RD_READY,    // CY_SD_HOST_BUF_RD_READY
    (uint32_t)CYHAL_SDHC_CARD_INSERTION,  // CY_SD_HOST_CARD_INSERTION
    (uint32_t)CYHAL_SDHC_CARD_REMOVAL,    // CY_SD_HOST_CARD_REMOVAL
    (uint32_t)CYHAL_SDHC_CARD_INTERRUPT,  // CY_SD_HOST_CARD_INTERRUPT
    (uint32_t)CYHAL_SDHC_FX_EVENT,        // CY_SD_HOST_FX_EVENT
    (uint32_t)CYHAL_SDHC_CQE_EVENT,       // CY_SD_HOST_CQE_EVENT
    (uint32_t)CYHAL_SDHC_ERR_INTERRUPT,   // CY_SD_HOST_ERR_INTERRUPT
};

static cy_rslt_t _cyhal_sdhc_setup_pin(cyhal_gpio_t pin, const cyhal_resource_pin_mapping_t *pinmap, size_t count, cyhal_gpio_t *objRef)
{
    cy_rslt_t result;
    const cyhal_resource_pin_mapping_t *map = _cyhal_utils_get_resource(pin, pinmap, count, NULL);

    if (map == NULL)
    {
        result = CYHAL_SDHC_RSLT_ERR_PIN;
    }
    else
    {
        result = _cyhal_utils_reserve_and_connect(pin, map);
        if (result == CY_RSLT_SUCCESS)
        {
            *objRef = pin;
        }
    }

    return result;
}

/* Internal global variable to indicate pending sdio async status */
static bool _cyhal_sdhc_transer_in_progress = false;

/*******************************************************************************
*       Deep Sleep Callback Service Routine
*******************************************************************************/
static bool _cyhal_sdio_syspm_callback(cyhal_syspm_callback_state_t state, cyhal_syspm_callback_mode_t mode, void* callback_arg)
{
    bool allow = true;
    cyhal_sdio_t *obj = (cyhal_sdio_t *)callback_arg;
    CY_ASSERT(obj != NULL);

    cy_stc_syspm_callback_params_t pdl_params =
    {
        .base = obj->base,
        .context = &(obj->context)
    };

    /* Check if hardware is ready to go sleep using lower level callback. */
    if (state == CYHAL_SYSPM_CB_CPU_DEEPSLEEP)
    {
        allow = (Cy_SD_Host_DeepSleepCallback(&pdl_params, _cyhal_utils_convert_haltopdl_pm_mode(mode)) == CY_SYSPM_SUCCESS);
    }

    if (allow)
    {
        switch (mode)
        {
            case CYHAL_SYSPM_CHECK_READY:
            {
                allow = !cyhal_sdio_is_busy(obj);
                if (allow)
                {
                    /* Call the event only if we are ready to go to sleep */
                    cyhal_sdio_event_callback_t callback = (cyhal_sdio_event_callback_t) obj->callback_data.callback;
                    if ((callback != NULL) && (0U != (obj->events & (uint32_t) CYHAL_SDIO_GOING_DOWN)))
                    {
                        (callback)(obj->callback_data.callback_arg, CYHAL_SDIO_GOING_DOWN);
                    }
                    /* Set transition flag to prevent any further transaction */
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
                    cyhal_sdio_event_callback_t callback = (cyhal_sdio_event_callback_t) obj->callback_data.callback;
                    if ((callback != NULL) && (0U != (obj->events & (uint32_t) CYHAL_SDIO_COMING_UP)))
                    {
                        (callback)(obj->callback_data.callback_arg, CYHAL_SDIO_COMING_UP);
                    }

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

static bool _cyhal_sdhc_syspm_callback(cyhal_syspm_callback_state_t state, cyhal_syspm_callback_mode_t mode, void* callback_arg)
{
    bool allow = true;
    cyhal_sdhc_t *obj = (cyhal_sdhc_t *)callback_arg;
    CY_ASSERT(obj != NULL);

    cy_stc_syspm_callback_params_t pdl_params =
    {
        .base = obj->base,
        .context = &(obj->context)
    };

    /* Check if hardware is ready to go sleep using lower level callback. */
    if (state == CYHAL_SYSPM_CB_CPU_DEEPSLEEP)
    {
        allow = (Cy_SD_Host_DeepSleepCallback(&pdl_params, _cyhal_utils_convert_haltopdl_pm_mode(mode)) == CY_SYSPM_SUCCESS);
    }

    if (allow)
    {
        switch (mode)
        {
            case CYHAL_SYSPM_CHECK_READY:
            {
                allow = !cyhal_sdhc_is_busy(obj);
                if (allow)
                {
                    /* Set transition flag to prevent any further transaction */
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
                obj->pm_transition_pending = false;
                break;
            }

            default:
                CY_ASSERT(false);
                break;
        }
    }
    return allow;
}

static cy_en_sd_host_bus_width_t _cyhal_sdhc_convert_buswidth(uint8_t stopbits)
{
    switch (stopbits)
    {
        case 1:
            return CY_SD_HOST_BUS_WIDTH_1_BIT;
        case 4:
            return CY_SD_HOST_BUS_WIDTH_4_BIT;
        case 8:
            return CY_SD_HOST_BUS_WIDTH_8_BIT;
        default:
            CY_ASSERT(false);
            return CY_SD_HOST_BUS_WIDTH_1_BIT;
    }
}


/******************************************************************************
*
* Changes the Host controller SD clock.
*
* clock         - The HFCLK clock instance driving the SD host.
* base          - The SD host registers structure pointer.
* frequency     - The frequency in Hz.
*
*******************************************************************************/
static cy_rslt_t _cyhal_sdhc_sdcardchangeclock(cyhal_clock_t *clock, SDHC_Type *base, uint32_t frequency)
{
    CY_ASSERT(NULL != clock);
    CY_ASSERT(NULL != base);

    /* Update SD Host clock divider */
    uint32_t clockInput = cyhal_clock_get_frequency(clock);
    uint16_t clkDiv = (uint16_t) ((clockInput / frequency) >> 1UL);

    Cy_SD_Host_DisableSdClk(base);
    cy_rslt_t ret = Cy_SD_Host_SetSdClkDiv(base, clkDiv);
    Cy_SD_Host_EnableSdClk(base);

    /* Update bus speed mode based on new frequency */
    if (CY_RET_SUCCESS == ret)
    {
        cy_en_sd_host_bus_speed_mode_t busSpeed;

        if (frequency == CY_SD_HOST_CLK_50M)
        {
            busSpeed = CY_SD_HOST_BUS_SPEED_SDR50;
        }
        else if (frequency == CY_SD_HOST_CLK_25M)
        {
            busSpeed = CY_SD_HOST_BUS_SPEED_SDR25;
        }
        else
        {
            busSpeed = CY_SD_HOST_BUS_SPEED_SDR12_5;
        }

        ret = Cy_SD_Host_SetBusSpeedMode(base, busSpeed, NULL);
    }

    return ret;
}


/*******************************************************************************
*
* Waits for the command complete event.
*
* base - The SD host registers structure pointer.
*
*******************************************************************************/
static cy_en_sd_host_status_t _cyhal_sdhc_pollcmdcomplete(SDHC_Type *base)
{
    cy_en_sd_host_status_t ret = CY_SD_HOST_ERROR_TIMEOUT;
    uint32_t               retry = _CYHAL_SDHC_RETRY_TIMES;

    while (retry > 0UL)
    {
        /* Command complete */
        if (CY_SD_HOST_CMD_COMPLETE == (CY_SD_HOST_CMD_COMPLETE & Cy_SD_Host_GetNormalInterruptStatus(base)))
        {
            /* Clear interrupt flag */
            Cy_SD_Host_ClearNormalInterruptStatus(base, CY_SD_HOST_CMD_COMPLETE);

            ret = CY_SD_HOST_SUCCESS;
            break;
        }

        Cy_SysLib_DelayUs(_CYHAL_SDIO_CMD_CMPLT_DELAY_US);
        retry--;
    }

    return ret;
}


/*******************************************************************************
*
* Waits for the command complete event.
*
* base  - The SD host registers structure pointer.
* delay - The delay timeout for one block transfer.
*
* return If the base pointer is NULL, returns error.
*
*******************************************************************************/
static cy_en_sd_host_status_t _cyhal_sdhc_polltransfercomplete(SDHC_Type *base, const uint16_t delay)
{
    cy_en_sd_host_status_t ret = CY_SD_HOST_ERROR_TIMEOUT;
    uint32_t               retry = _CYHAL_SDHC_RW_RETRY_CYCLES;
    uint32_t               status = 0UL;

    while ((CY_SD_HOST_ERROR_TIMEOUT == ret) && (retry-- > 0U))
    {
        status = Cy_SD_Host_GetNormalInterruptStatus(base);

        /* Transfer complete */
        if (CY_SD_HOST_XFER_COMPLETE == (CY_SD_HOST_XFER_COMPLETE & status))
        {
            /* Clear interrupt flag */
            Cy_SD_Host_ClearNormalInterruptStatus(base, CY_SD_HOST_XFER_COMPLETE);

            ret = CY_SD_HOST_SUCCESS;
            break;
        }

        Cy_SysLib_DelayUs(delay);
    }

    return ret;
}

static void _cyhal_sdhc_irq_handler(void)
{
    IRQn_Type irqn = _CYHAL_UTILS_GET_CURRENT_IRQN();
    uint8_t block = _cyhal_sdhc_get_block_from_irqn(irqn);
    SDHC_Type *blockAddr = _CYHAL_SDHC_BASE_ADDRESSES[block];
    cyhal_sdhc_t *obj = (cyhal_sdhc_t*) _cyhal_sdhc_config_structs[block];

    uint32_t interruptStatus = Cy_SD_Host_GetNormalInterruptStatus(blockAddr);
    uint32_t userInterruptStatus = interruptStatus & obj->irq_cause;
    cyhal_sdhc_event_t user_events = (cyhal_sdhc_event_t)_cyhal_utils_convert_flags(
        _cyhal_sdhc_event_map, sizeof(_cyhal_sdhc_event_map) / sizeof(uint32_t), userInterruptStatus);

    if (obj->callback_data.callback != NULL)
    {
        cyhal_sdhc_event_callback_t callback = (cyhal_sdhc_event_callback_t) obj->callback_data.callback;

        /* Call registered callbacks here */
        (callback) (obj->callback_data.callback_arg, user_events);
    }

    /* Clear only handled events */
    Cy_SD_Host_ClearNormalInterruptStatus(blockAddr, interruptStatus);

    /* To clear SD Card interrupt need to disable SD Card Interrupt Enable bit */
    if (0U != (interruptStatus & CY_SD_HOST_CARD_INTERRUPT))
    {
        uint32_t interruptMask = Cy_SD_Host_GetNormalInterruptEnable(blockAddr);
        interruptMask &= (uint32_t) ~CY_SD_HOST_CARD_INTERRUPT;

        /* Disable SD Card interrupt */
        Cy_SD_Host_SetNormalInterruptEnable(blockAddr, interruptMask);
    }
}

static void _cyhal_sdio_irq_handler(void)
{
    IRQn_Type irqn = _CYHAL_UTILS_GET_CURRENT_IRQN();
    uint8_t block = _cyhal_sdhc_get_block_from_irqn(irqn);
    SDHC_Type *blockAddr = _CYHAL_SDHC_BASE_ADDRESSES[block];
    cyhal_sdio_t *obj = (cyhal_sdio_t*) _cyhal_sdhc_config_structs[block];

    uint32_t interruptStatus = Cy_SD_Host_GetNormalInterruptStatus(blockAddr);
    uint32_t userInterruptStatus = interruptStatus & obj->irq_cause;

    if (obj->callback_data.callback != NULL && userInterruptStatus > 0)
    {
        cyhal_sdio_event_callback_t callback = (cyhal_sdio_event_callback_t) obj->callback_data.callback;
        /* Call registered callbacks here */
        (callback)(obj->callback_data.callback_arg, (cyhal_sdio_irq_event_t) userInterruptStatus);
    }

    /* Clear only handled events */
    Cy_SD_Host_ClearNormalInterruptStatus(blockAddr, userInterruptStatus);

    /* To clear SD Card interrupt need to disable SD Card Interrupt Enable bit.
    *  The SD Card interrupt is enabled in the bulk transfer function later on
    *  the next data transfer.
    */
    if (0U != (obj->irq_cause & CY_SD_HOST_CARD_INTERRUPT))
    {
        if (0U != (interruptStatus & CY_SD_HOST_CARD_INTERRUPT))
        {
            uint32_t interruptMask = Cy_SD_Host_GetNormalInterruptEnable(blockAddr);
            interruptMask &= (uint32_t) ~CY_SD_HOST_CARD_INTERRUPT;

            /* Disable SD Card interrupt */
            Cy_SD_Host_SetNormalInterruptEnable(blockAddr, interruptMask);
        }
    }
}

cy_rslt_t cyhal_sdhc_init(cyhal_sdhc_t *obj,
                          const cyhal_sdhc_config_t *config,
                          cyhal_gpio_t cmd,
                          cyhal_gpio_t clk,
                          cyhal_gpio_t data0,
                          cyhal_gpio_t data1,
                          cyhal_gpio_t data2,
                          cyhal_gpio_t data3,
                          cyhal_gpio_t data4,
                          cyhal_gpio_t data5,
                          cyhal_gpio_t data6,
                          cyhal_gpio_t data7,
                          cyhal_gpio_t cardDetect,
                          cyhal_gpio_t ioVoltSel,
                          cyhal_gpio_t cardIfPwrEn,
                          cyhal_gpio_t cardMechWriteProt,
                          cyhal_gpio_t ledCtrl,
                          cyhal_gpio_t cardEmmcReset)
{
    CY_ASSERT(NULL != obj);

    cy_rslt_t result = CY_RSLT_SUCCESS;

    obj->base = NULL;
    obj->resource.type = CYHAL_RSC_INVALID;

    obj->pin_clk = CYHAL_NC_PIN_VALUE;
    obj->pin_cmd = CYHAL_NC_PIN_VALUE;
    obj->pin_data[0] = CYHAL_NC_PIN_VALUE;
    obj->pin_data[1] = CYHAL_NC_PIN_VALUE;
    obj->pin_data[2] = CYHAL_NC_PIN_VALUE;
    obj->pin_data[3] = CYHAL_NC_PIN_VALUE;
    obj->pin_data[4] = CYHAL_NC_PIN_VALUE;
    obj->pin_data[5] = CYHAL_NC_PIN_VALUE;
    obj->pin_data[6] = CYHAL_NC_PIN_VALUE;
    obj->pin_data[7] = CYHAL_NC_PIN_VALUE;
    obj->pin_cardDetect = CYHAL_NC_PIN_VALUE;
    obj->pin_ioVoltSel = CYHAL_NC_PIN_VALUE;
    obj->pin_cardIfPwrEn = CYHAL_NC_PIN_VALUE;
    obj->pin_cardMechWriteProt = CYHAL_NC_PIN_VALUE;
    obj->pin_ledCtrl = CYHAL_NC_PIN_VALUE;
    obj->pin_cardEmmcReset = CYHAL_NC_PIN_VALUE;

    cyhal_gpio_t data[8];
    data[0] = data0;
    data[1] = data1;
    data[2] = data2;
    data[3] = data3;
    data[4] = data4;
    data[5] = data5;
    data[6] = data6;
    data[7] = data7;

    /* Reserve SDHC */
    const cyhal_resource_pin_mapping_t *map = _CYHAL_UTILS_GET_RESOURCE(cmd, cyhal_pin_map_sdhc_card_cmd);

    if (NULL == map || NC == data0)
    {
        result = CYHAL_SDHC_RSLT_ERR_PIN;
    }

    /* Reserve and configure GPIO pins */
    if (result == CY_RSLT_SUCCESS)
    {
        result = _cyhal_sdhc_setup_pin(
            cmd, cyhal_pin_map_sdhc_card_cmd, _CYHAL_SDHC_ELEM_COUNT(cyhal_pin_map_sdhc_card_cmd), &(obj->pin_cmd));
    }

    if (result == CY_RSLT_SUCCESS)
    {
        result = _cyhal_sdhc_setup_pin(
            clk, cyhal_pin_map_sdhc_clk_card, _CYHAL_SDHC_ELEM_COUNT(cyhal_pin_map_sdhc_clk_card), &(obj->pin_clk));
    }

    for (uint8_t i = 0; (i < 4) && (CY_RSLT_SUCCESS == result); i++)
    {
        if (NC != data[i])
        {
            result = _cyhal_sdhc_setup_pin(data[i], cyhal_pin_map_sdhc_card_dat_3to0, _CYHAL_SDHC_ELEM_COUNT(cyhal_pin_map_sdhc_card_dat_3to0), &(obj->pin_data[i]));
        }
    }

#if _CYHAL_SDHC_DATA8_PRESENT
    for (uint8_t i = 4; (i < 8) && (CY_RSLT_SUCCESS == result); i++)
    {
        if (NC != data[i])
        {
            result = _cyhal_sdhc_setup_pin(data[i], cyhal_pin_map_sdhc_card_dat_7to4, _CYHAL_SDHC_ELEM_COUNT(cyhal_pin_map_sdhc_card_dat_7to4), &(obj->pin_data[i]));
        }
    }
#else
    if ((NC != data4 || NC != data5 || NC != data6 || NC != data7) && (CY_RSLT_SUCCESS == result))
    {
        result = CYHAL_SDHC_RSLT_ERR_PIN;
    }
#endif

    if ((NC != cardDetect) && (CY_RSLT_SUCCESS == result))
    {
#if _CYHAL_SDHC_CARD_DETECT_PRESENT
        result = _cyhal_sdhc_setup_pin(cardDetect, cyhal_pin_map_sdhc_card_detect_n,
            _CYHAL_SDHC_ELEM_COUNT(cyhal_pin_map_sdhc_card_detect_n), &(obj->pin_cardDetect));
#else
        result = CYHAL_SDHC_RSLT_ERR_PIN;
#endif
    }

    if ((NC != ioVoltSel) && (CY_RSLT_SUCCESS == result))
    {
#if _CYHAL_SDHC_IO_VOLT_SEL_PRESENT
        result = _cyhal_sdhc_setup_pin(ioVoltSel, cyhal_pin_map_sdhc_io_volt_sel,
            _CYHAL_SDHC_ELEM_COUNT(cyhal_pin_map_sdhc_io_volt_sel), &(obj->pin_ioVoltSel));
#else
        result = CYHAL_SDHC_RSLT_ERR_PIN;
#endif
    }

    if ((NC != cardIfPwrEn) && (CY_RSLT_SUCCESS == result))
    {
#if _CYHAL_SDHC_CARD_IF_PWR_EN_PRESENT
        result = _cyhal_sdhc_setup_pin(cardIfPwrEn, cyhal_pin_map_sdhc_card_if_pwr_en,
            _CYHAL_SDHC_ELEM_COUNT(cyhal_pin_map_sdhc_card_if_pwr_en), &(obj->pin_cardIfPwrEn));
#else
        result = CYHAL_SDHC_RSLT_ERR_PIN;
#endif
    }

    if ((NC != cardMechWriteProt) && (CY_RSLT_SUCCESS == result))
    {
#if _CYHAL_SDHC_CARD_WRITE_PROT_PRESENT
        result = _cyhal_sdhc_setup_pin(cardMechWriteProt, cyhal_pin_map_sdhc_card_mech_write_prot,
            _CYHAL_SDHC_ELEM_COUNT(cyhal_pin_map_sdhc_card_mech_write_prot), &(obj->pin_cardMechWriteProt));
#else
        result = CYHAL_SDHC_RSLT_ERR_PIN;
#endif
    }

    if ((NC != ledCtrl) && (CY_RSLT_SUCCESS == result))
    {
#if _CYHAL_SDHC_LED_CTRL_PRESENT
        result = _cyhal_sdhc_setup_pin(ledCtrl, cyhal_pin_map_sdhc_led_ctrl,
            _CYHAL_SDHC_ELEM_COUNT(cyhal_pin_map_sdhc_led_ctrl), &(obj->pin_ledCtrl));
#else
        result = CYHAL_SDHC_RSLT_ERR_PIN;
#endif
    }

    if ((NC != cardEmmcReset) && (CY_RSLT_SUCCESS == result))
    {
#if _CYHAL_SDHC_CARD_EMMC_RESET_PRESENT
        result = _cyhal_sdhc_setup_pin(cardEmmcReset, cyhal_pin_map_sdhc_card_emmc_reset_n,
            _CYHAL_SDHC_ELEM_COUNT(cyhal_pin_map_sdhc_card_emmc_reset_n), &(obj->pin_cardEmmcReset));
#else
        result = CYHAL_SDHC_RSLT_ERR_PIN;
#endif
    }

    if (CY_RSLT_SUCCESS == result)
    {
        result = _cyhal_utils_allocate_clock(&(obj->clock), map->inst, CYHAL_CLOCK_BLOCK_PERIPHERAL_8BIT, true);

        if (CY_RSLT_SUCCESS == result)
        {
            result = _cyhal_utils_set_clock_frequency2(&(obj->clock), MAX_FREQUENCY, &CYHAL_CLOCK_TOLERANCE_5_P);
        }

        if (CY_RSLT_SUCCESS == result && !cyhal_clock_is_enabled(&(obj->clock)))
        {
            result = cyhal_clock_set_enabled(&(obj->clock), true, true);
        }
    }

    if (CY_RSLT_SUCCESS == result)
    {
        obj->resource = *map->inst;
        result = cyhal_hwmgr_reserve(&obj->resource);

        if (CY_RSLT_SUCCESS == result)
        {
            obj->base = _CYHAL_SDHC_BASE_ADDRESSES[obj->resource.block_num];

            /* Enable the SDHC block */
            Cy_SD_Host_Enable(obj->base);

            /* Do not support eMMC card */
            obj->emmc = config->isEmmc;
            obj->dmaType = CY_SD_HOST_DMA_ADMA2;

            /* Configure SD Host to operate */
            cy_stc_sd_host_init_config_t hostConfig;
            hostConfig.dmaType = obj->dmaType;
            hostConfig.emmc = obj->emmc;

        #if _CYHAL_SDHC_LED_CTRL_PRESENT
            hostConfig.enableLedControl = (NC != ledCtrl);
        #else
            hostConfig.enableLedControl = false;
        #endif

            obj->context.cardType = CY_SD_HOST_NOT_EMMC;

            obj->irq_cause = 0UL;
            obj->callback_data.callback = NULL;
            obj->callback_data.callback_arg = NULL;
            _cyhal_sdhc_config_structs[obj->resource.block_num] = obj;

            IRQn_Type irqn = _CYHAL_SDHC_IRQ_N[obj->resource.block_num];
            cy_stc_sysint_t irqCfg = { irqn, CYHAL_ISR_PRIORITY_DEFAULT };
            Cy_SysInt_Init(&irqCfg, _cyhal_sdhc_irq_handler);
            NVIC_EnableIRQ(irqn);

            result = (cy_rslt_t) Cy_SD_Host_Init(obj->base, &hostConfig, &obj->context);
        }

        /* Initialize card */
        if (result == CY_RSLT_SUCCESS)
        {
            cy_stc_sd_host_sd_card_config_t stcSdcardCfg;
            cy_en_sd_host_card_capacity_t cardCapacity;
            uint32_t rca = 0UL;
            cy_en_sd_host_card_type_t cardType = CY_SD_HOST_UNUSABLE;

            /* Initialize the SDIO/SD/eMMC card configuration structure */
            stcSdcardCfg.lowVoltageSignaling = config->lowVoltageSignaling;
            stcSdcardCfg.busWidth = _cyhal_sdhc_convert_buswidth(config->busWidth);
            stcSdcardCfg.cardType = &cardType;
            stcSdcardCfg.rca = &rca;
            stcSdcardCfg.cardCapacity = &cardCapacity;

            /* Initialize the card */
            result = (cy_rslt_t)Cy_SD_Host_InitCard(obj->base, &stcSdcardCfg, &obj->context);

            if (result == CY_RSLT_SUCCESS)
            {
                /* Update SD Card frequency to be 25 Mhz */
                result = _cyhal_sdhc_sdcardchangeclock(&(obj->clock), obj->base, CY_SD_HOST_CLK_25M);
            }
        }

        if(result == CY_RSLT_SUCCESS)
        {
            obj->pm_transition_pending = false;
            obj->pm_callback_data.callback = &_cyhal_sdhc_syspm_callback,
            obj->pm_callback_data.states = (cyhal_syspm_callback_state_t)(CYHAL_SYSPM_CB_CPU_DEEPSLEEP | CYHAL_SYSPM_CB_SYSTEM_HIBERNATE);
            obj->pm_callback_data.next = NULL;
            obj->pm_callback_data.args = obj;
            /* The CYHAL_SYSPM_BEFORE_TRANSITION mode cannot be ignored because the PM handler
             * calls the PDL deep-sleep callback that disables the block in this mode before transitioning.
             */
            obj->pm_callback_data.ignore_modes = (cyhal_syspm_callback_mode_t)0,

            _cyhal_syspm_register_peripheral_callback(&obj->pm_callback_data);
        }
    }

    if (result != CY_RSLT_SUCCESS)
    {
        cyhal_sdhc_free(obj);
    }

    return result;
}

void cyhal_sdhc_free(cyhal_sdhc_t *obj)
{
    CY_ASSERT(NULL != obj);

    if (NULL != obj->base)
    {
        IRQn_Type irqn = _CYHAL_SDHC_IRQ_N[obj->resource.block_num];
        NVIC_DisableIRQ(irqn);

        Cy_SD_Host_DeInit(obj->base);

        cyhal_hwmgr_free(&(obj->resource));
        obj->base = NULL;

        _cyhal_syspm_unregister_peripheral_callback(&obj->pm_callback_data);
    }

    if (obj->resource.type != CYHAL_RSC_INVALID)
    {
        cyhal_clock_free(&(obj->clock));
        obj->resource.type = CYHAL_RSC_INVALID;
    }

    /* Free pins */
    _cyhal_utils_release_if_used(&(obj->pin_cmd));
    _cyhal_utils_release_if_used(&(obj->pin_clk));

#if _CYHAL_SDHC_DATA8_PRESENT
    const uint8_t max_idx = 8;
#else
    const uint8_t max_idx = 4;
#endif
    for (uint8_t i = 0; i < max_idx; i++)
    {
        _cyhal_utils_release_if_used(&(obj->pin_data[i]));
    }

#if _CYHAL_SDHC_CARD_DETECT_PRESENT
    _cyhal_utils_release_if_used(&(obj->pin_cardDetect));
#endif

#if _CYHAL_SDHC_IO_VOLT_SEL_PRESENT
    _cyhal_utils_release_if_used(&(obj->pin_ioVoltSel));
#endif

#if _CYHAL_SDHC_CARD_IF_PWR_EN_PRESENT
    _cyhal_utils_release_if_used(&(obj->pin_cardIfPwrEn));
#endif

#if _CYHAL_SDHC_CARD_WRITE_PROT_PRESENT
    _cyhal_utils_release_if_used(&(obj->pin_cardMechWriteProt));
#endif

#if _CYHAL_SDHC_LED_CTRL_PRESENT
    _cyhal_utils_release_if_used(&(obj->pin_ledCtrl));
#endif

#if _CYHAL_SDHC_CARD_EMMC_RESET_PRESENT
    _cyhal_utils_release_if_used(&(obj->pin_cardEmmcReset));
#endif
}

cy_rslt_t cyhal_sdhc_read(const cyhal_sdhc_t *obj, uint32_t address, uint8_t *data, size_t *length)
{
    if (obj->pm_transition_pending)
    {
        return CYHAL_SYSPM_RSLT_ERR_PM_PENDING;
    }

    cy_rslt_t ret = CY_RSLT_SUCCESS;
    cy_en_sd_host_status_t driverRet;
    cy_stc_sd_host_write_read_config_t dataConfig;
    uint32_t regIntrSts = Cy_SD_Host_GetNormalInterruptMask(obj->base);

    dataConfig.data = (uint32_t*)data;   /* The pointer to data. */
    dataConfig.address = address;        /* The address to write/read data on the card or eMMC. */
    dataConfig.numberOfBlocks = (uint32_t)*length;  /* The number of blocks to write/read. */
    dataConfig.autoCommand = (1UL == (uint32_t)*length)
        ? CY_SD_HOST_AUTO_CMD_NONE
        : CY_SD_HOST_AUTO_CMD_AUTO;
    dataConfig.dataTimeout = _CYHAL_SDHC_TRANSFER_TIMEOUT;      /* The timeout value for the transfer. */
    dataConfig.enReliableWrite = false; /* For EMMC cards enable reliable write. */
    dataConfig.enableDma = true;

    /* First clear out the transfer and command complete statuses */
    Cy_SD_Host_ClearNormalInterruptStatus(obj->base, (CY_SD_HOST_XFER_COMPLETE | CY_SD_HOST_CMD_COMPLETE));

    /* Disable CMD Done interrupt, will be enabled after transition is complete */
    Cy_SD_Host_SetNormalInterruptMask(obj->base, (regIntrSts & (uint32_t)~(CY_SD_HOST_XFER_COMPLETE | CY_SD_HOST_CMD_COMPLETE)));

    driverRet = Cy_SD_Host_Read(obj->base, &dataConfig, &obj->context);

    if (CY_SD_HOST_SUCCESS != driverRet)
    {
        ret = CY_RSLT_TYPE_ERROR;
    }
    else
    {
        driverRet = _cyhal_sdhc_polltransfercomplete(obj->base, _CYHAL_SDHC_RW_TIMEOUT_US);

        if (CY_SD_HOST_SUCCESS != driverRet)
        {
            ret = CY_RSLT_TYPE_ERROR;
        }
    }

    /* Restore interrupts after transition */
    Cy_SD_Host_SetNormalInterruptMask(obj->base, regIntrSts);

    /* Enable SD Card interrupt because it was disabled in interrupt handler */
    if (0U != (obj->irq_cause & CY_SD_HOST_CARD_INTERRUPT))
    {
        uint32_t intrStsEn = Cy_SD_Host_GetNormalInterruptEnable(obj->base);

        /* This interrupt is disabled in interrupt handler so need to enable it here */
        Cy_SD_Host_SetNormalInterruptEnable(obj->base, (intrStsEn | CY_SD_HOST_CARD_INTERRUPT));
    }

    return ret;
}

cy_rslt_t cyhal_sdhc_write(const cyhal_sdhc_t *obj, uint32_t address, const uint8_t *data, size_t *length)
{
    if (obj->pm_transition_pending)
    {
        return CYHAL_SYSPM_RSLT_ERR_PM_PENDING;
    }

    cy_rslt_t ret = CY_RSLT_SUCCESS;
    cy_en_sd_host_status_t driverRet;
    cy_stc_sd_host_write_read_config_t dataConfig;
    uint32_t regIntrSts = Cy_SD_Host_GetNormalInterruptMask(obj->base);

    dataConfig.data = (uint32_t*)data;   /* The pointer to data. */
    dataConfig.address = address;        /* The address to write/read data on the card or eMMC. */
    dataConfig.numberOfBlocks = (uint32_t)*length;  /* The number of blocks to write/read. */
    dataConfig.autoCommand = (1UL == (uint32_t)*length)
        ? CY_SD_HOST_AUTO_CMD_NONE
        : CY_SD_HOST_AUTO_CMD_AUTO;
    dataConfig.dataTimeout = _CYHAL_SDHC_TRANSFER_TIMEOUT;  /* The timeout value for the transfer. */
    dataConfig.enReliableWrite = false; /* For EMMC cards enable reliable write. */
    dataConfig.enableDma = true;

    /* First clear out the transfer and command complete statuses */
    Cy_SD_Host_ClearNormalInterruptStatus(obj->base, (CY_SD_HOST_XFER_COMPLETE | CY_SD_HOST_CMD_COMPLETE));

    /* Disable CMD Done interrupt, will be enabled after transition is complete */
    Cy_SD_Host_SetNormalInterruptMask(obj->base, (regIntrSts & (uint32_t)~(CY_SD_HOST_XFER_COMPLETE | CY_SD_HOST_CMD_COMPLETE)));

    driverRet = Cy_SD_Host_Write(obj->base, &dataConfig, &obj->context);

    if (CY_SD_HOST_SUCCESS != driverRet)
    {
        ret = CY_RSLT_TYPE_ERROR;
    }
    else
    {
        driverRet = _cyhal_sdhc_polltransfercomplete(obj->base, _CYHAL_SDHC_RW_TIMEOUT_US);

        if (CY_SD_HOST_SUCCESS != driverRet)
        {
            ret = CY_RSLT_TYPE_ERROR;
        }
    }

    /* Restore interrupts after transition */
    Cy_SD_Host_SetNormalInterruptMask(obj->base, regIntrSts);

    /* Enable SD Card interrupt because it was disabled in interrupt handler */
    if (0U != (obj->irq_cause & CY_SD_HOST_CARD_INTERRUPT))
    {
        uint32_t intrStsEn = Cy_SD_Host_GetNormalInterruptEnable(obj->base);

        /* This interrupt is disabled in interrupt handler so need to enable it here */
        Cy_SD_Host_SetNormalInterruptEnable(obj->base, (intrStsEn | CY_SD_HOST_CARD_INTERRUPT));
    }

    return ret;
}

cy_rslt_t cyhal_sdhc_erase(const cyhal_sdhc_t *obj, uint32_t start_addr, size_t length)
{
    if (obj->pm_transition_pending)
    {
        return CYHAL_SYSPM_RSLT_ERR_PM_PENDING;
    }

    cy_rslt_t ret = CY_RSLT_SUCCESS;
    cy_en_sd_host_status_t driverRet;
    cy_en_sd_host_erase_type_t eraseType = CY_SD_HOST_ERASE_ERASE;
    uint32_t i = _CYHAL_SDHC_RETRY_TIMES;
    uint32_t cardStatus;
    uint32_t regIntrSts = Cy_SD_Host_GetNormalInterruptMask(obj->base);

    if (obj->emmc)
    {
         eraseType = CY_SD_HOST_ERASE_TRIM;
    }

    /* First clear out the transfer and command complete statuses */
    Cy_SD_Host_ClearNormalInterruptStatus(obj->base, (CY_SD_HOST_XFER_COMPLETE | CY_SD_HOST_CMD_COMPLETE));

    /* Disable CMD Done interrupt, will be enabled after transition is complete */
    Cy_SD_Host_SetNormalInterruptMask(obj->base, (regIntrSts & (uint32_t)~(CY_SD_HOST_XFER_COMPLETE | CY_SD_HOST_CMD_COMPLETE)));

    driverRet = Cy_SD_Host_Erase(obj->base, start_addr, (start_addr + length), eraseType, &obj->context);

    if (CY_SD_HOST_SUCCESS != driverRet)
    {
        ret = CY_RSLT_TYPE_ERROR;
    }
    else
    {
        driverRet = _cyhal_sdhc_pollcmdcomplete(obj->base);
    }

    if (CY_SD_HOST_SUCCESS != driverRet)
    {
        ret = CY_RSLT_TYPE_ERROR;
    }

    if (false == obj->emmc)
    {
        /* polling result */
        driverRet = CY_SD_HOST_ERROR;

        while (i-- > 0UL)
        {
            cardStatus = Cy_SD_Host_GetCardStatus(obj->base, &obj->context);
            if (((CY_SD_HOST_CARD_TRAN << CY_SD_HOST_CMD13_CURRENT_STATE) |
                (1UL << CY_SD_HOST_CMD13_READY_FOR_DATA)) == cardStatus)
            {
                driverRet = CY_SD_HOST_SUCCESS;
                break;
            }

            Cy_SysLib_DelayUs(_CYHAL_SDHC_FUJE_TIMEOUT_MS); /* The Fuje timeout for one block. */
        }

        if (CY_SD_HOST_SUCCESS != driverRet)
        {
            ret = CY_RSLT_TYPE_ERROR;
        }
    }
    else
    {
        Cy_SysLib_Delay(_CYHAL_SDHC_EMMC_TRIM_DELAY_MS);
    }

    /* Restore interrupts after transition */
    Cy_SD_Host_SetNormalInterruptMask(obj->base, regIntrSts);

    /* Enable SD Card interrupt because it was disabled in interrupt handler */
    if (0U != (obj->irq_cause & CY_SD_HOST_CARD_INTERRUPT))
    {
        uint32_t intrStsEn = Cy_SD_Host_GetNormalInterruptEnable(obj->base);

        /* This interrupt is disabled in interrupt handler so need to enable it here */
        Cy_SD_Host_SetNormalInterruptEnable(obj->base, (intrStsEn | CY_SD_HOST_CARD_INTERRUPT));
    }

    return ret;
}

cy_rslt_t cyhal_sdhc_read_async(const cyhal_sdhc_t *obj, uint32_t address, uint8_t *data, size_t *length)
{
    /* Not yet implemented for this device. */
    CY_UNUSED_PARAMETER(obj);
    CY_UNUSED_PARAMETER(address);
    CY_UNUSED_PARAMETER(data);
    CY_UNUSED_PARAMETER(length);
    cy_rslt_t ret = CYHAL_SDHC_RSLT_ERR_UNSUPPORTED;
    return ret;
}

cy_rslt_t cyhal_sdhc_write_async(const cyhal_sdhc_t *obj, uint32_t address, const uint8_t *data, size_t *length)
{
    /* Not yet implemented for this device. */
    CY_UNUSED_PARAMETER(obj);
    CY_UNUSED_PARAMETER(address);
    CY_UNUSED_PARAMETER(data);
    CY_UNUSED_PARAMETER(length);
    cy_rslt_t ret = CYHAL_SDHC_RSLT_ERR_UNSUPPORTED;
    return ret;
}

bool cyhal_sdhc_is_busy(const cyhal_sdhc_t *obj)
{
    /* Check DAT Line Active */
    uint32_t pState = Cy_SD_Host_GetPresentState(obj->base);
    if ((CY_SD_HOST_DAT_LINE_ACTIVE != (pState & CY_SD_HOST_DAT_LINE_ACTIVE)) &&
        (CY_SD_HOST_CMD_CMD_INHIBIT_DAT != (pState & CY_SD_HOST_CMD_CMD_INHIBIT_DAT)))
    {
        return false;
    }

    return true;
}

cy_rslt_t cyhal_sdhc_abort_async(const cyhal_sdhc_t *obj)
{
    cy_rslt_t ret = CY_RSLT_SUCCESS;
    cy_en_sd_host_status_t driverRet;

    driverRet = Cy_SD_Host_AbortTransfer(obj->base, &obj->context);

    if (CY_SD_HOST_SUCCESS != driverRet)
    {
        ret = CY_RSLT_TYPE_ERROR;
    }

    return ret;
}

void cyhal_sdhc_register_callback(cyhal_sdhc_t *obj, cyhal_sdhc_event_callback_t callback, void *callback_arg)
{
    uint32_t savedIntrStatus = cyhal_system_critical_section_enter();
    obj->callback_data.callback = (cy_israddress) callback;
    obj->callback_data.callback_arg = callback_arg;
    cyhal_system_critical_section_exit(savedIntrStatus);
}

void cyhal_sdhc_enable_event(cyhal_sdhc_t *obj, cyhal_sdhc_event_t event, uint8_t intr_priority, bool enable)
{
    uint32_t interruptMask = Cy_SD_Host_GetNormalInterruptMask(obj->base);

    IRQn_Type irqn = _CYHAL_SDHC_IRQ_N[obj->resource.block_num];
    NVIC_SetPriority(irqn, intr_priority);
    uint32_t map_count = sizeof(_cyhal_sdhc_event_map) / sizeof(uint32_t);

    /* Specific interrupt */
    if ((uint32_t) event < (uint32_t) CYHAL_SDHC_ALL_INTERRUPTS)
    {
        for (uint8_t i = 1; i < map_count; i++)
        {
            if ((_cyhal_sdhc_event_map[i] & (uint32_t) event) != 0)
            {
                if (enable)
                    interruptMask |= (1 << (i - 1));
                else
                    interruptMask &= ~(1 << (i - 1));
            }
        }
    }
    /* All interrupts */
    else
    {
        interruptMask = (enable)
            ? _CYHAL_SDIO_SET_ALL_INTERRUPTS_MASK
            : _CYHAL_SDIO_CLEAR_ALL_INTERRUPTS_MASK;
    }

    obj->irq_cause = interruptMask;
    Cy_SD_Host_SetNormalInterruptMask(obj->base, interruptMask);
}

cy_rslt_t cyhal_sdio_init(cyhal_sdio_t *obj, cyhal_gpio_t cmd, cyhal_gpio_t clk, cyhal_gpio_t data0, cyhal_gpio_t data1, cyhal_gpio_t data2, cyhal_gpio_t data3)
{
    cy_stc_sd_host_init_config_t hostConfig;
    cy_rslt_t result;
    cy_stc_sd_host_context_t context;

    CY_ASSERT(NULL != obj);

    obj->resource.type = CYHAL_RSC_INVALID;
    obj->base = NULL;
    obj->pin_cmd = CYHAL_NC_PIN_VALUE;
    obj->pin_clk = CYHAL_NC_PIN_VALUE;
    obj->pin_data0 = CYHAL_NC_PIN_VALUE;
    obj->pin_data1 = CYHAL_NC_PIN_VALUE;
    obj->pin_data2 = CYHAL_NC_PIN_VALUE;
    obj->pin_data3 = CYHAL_NC_PIN_VALUE;

    result = _cyhal_sdhc_setup_pin(cmd, cyhal_pin_map_sdhc_card_cmd,
        _CYHAL_SDHC_ELEM_COUNT(cyhal_pin_map_sdhc_card_cmd), &(obj->pin_cmd));

    if (CY_RSLT_SUCCESS == result)
    {
        result = _cyhal_sdhc_setup_pin(clk, cyhal_pin_map_sdhc_clk_card,
            _CYHAL_SDHC_ELEM_COUNT(cyhal_pin_map_sdhc_clk_card), &(obj->pin_clk));
    }

    if (CY_RSLT_SUCCESS == result)
    {
        result = _cyhal_sdhc_setup_pin(data0, cyhal_pin_map_sdhc_card_dat_3to0,
            _CYHAL_SDHC_ELEM_COUNT(cyhal_pin_map_sdhc_card_dat_3to0), &(obj->pin_data0));
    }

    if (CY_RSLT_SUCCESS == result)
    {
        result = _cyhal_sdhc_setup_pin(data1, cyhal_pin_map_sdhc_card_dat_3to0,
            _CYHAL_SDHC_ELEM_COUNT(cyhal_pin_map_sdhc_card_dat_3to0), &(obj->pin_data1));
    }

    if (CY_RSLT_SUCCESS == result)
    {
        result = _cyhal_sdhc_setup_pin(data2, cyhal_pin_map_sdhc_card_dat_3to0,
            _CYHAL_SDHC_ELEM_COUNT(cyhal_pin_map_sdhc_card_dat_3to0), &(obj->pin_data2));
    }

    if (CY_RSLT_SUCCESS == result)
    {
        result = _cyhal_sdhc_setup_pin(data3, cyhal_pin_map_sdhc_card_dat_3to0,
            _CYHAL_SDHC_ELEM_COUNT(cyhal_pin_map_sdhc_card_dat_3to0), &(obj->pin_data3));
    }

    if (result == CY_RSLT_SUCCESS)
    {
        const cyhal_resource_pin_mapping_t *cmd_map = _CYHAL_UTILS_GET_RESOURCE(cmd, cyhal_pin_map_sdhc_card_cmd);
        cyhal_resource_inst_t sdhc = *cmd_map->inst;

        result = _cyhal_utils_allocate_clock(&(obj->clock), &sdhc, CYHAL_CLOCK_BLOCK_PERIPHERAL_16BIT, true);
        if (CY_RSLT_SUCCESS == result)
        {
            result = _cyhal_utils_set_clock_frequency2(&(obj->clock), MAX_FREQUENCY, &CYHAL_CLOCK_TOLERANCE_5_P);
        }

        if (CY_RSLT_SUCCESS == result && !cyhal_clock_is_enabled(&(obj->clock)))
        {
            result = cyhal_clock_set_enabled(&(obj->clock), true, true);
        }

        if (result == CY_RSLT_SUCCESS)
        {
            obj->resource = sdhc;
            result = cyhal_hwmgr_reserve(&sdhc);
        }

        if (result == CY_RSLT_SUCCESS)
        {
            obj->base = _CYHAL_SDHC_BASE_ADDRESSES[obj->resource.block_num];

            /* Enable the SDHC block */
            Cy_SD_Host_Enable(obj->base);

            hostConfig.dmaType = CY_SD_HOST_DMA_SDMA;
            hostConfig.emmc = false;
            hostConfig.enableLedControl = false;
            context.cardType = CY_SD_HOST_SDIO;

            /* Configure SD Host to operate */
            result = Cy_SD_Host_Init(obj->base, &hostConfig, &context);

            /* Register SDIO Deep Sleep Callback */
            if (CY_RSLT_SUCCESS == result)
            {
                obj->pm_transition_pending = false;
                obj->pm_callback_data.callback = &_cyhal_sdio_syspm_callback,
                obj->pm_callback_data.states = (cyhal_syspm_callback_state_t)(CYHAL_SYSPM_CB_CPU_DEEPSLEEP | CYHAL_SYSPM_CB_SYSTEM_HIBERNATE);
                obj->pm_callback_data.next = NULL;
                obj->pm_callback_data.args = obj;
                /* The CYHAL_SYSPM_BEFORE_TRANSITION mode cannot be ignored because the PM handler
                    * calls the PDL deep-sleep callback that disables the block in this mode before transitioning.
                    */
                obj->pm_callback_data.ignore_modes = (cyhal_syspm_callback_mode_t)0;

                _cyhal_syspm_register_peripheral_callback(&obj->pm_callback_data);
            }

            if (result == CY_RSLT_SUCCESS)
            {
                /* Don't enable any error interrupts for now */
                Cy_SD_Host_SetErrorInterruptMask(obj->base, 0UL);

                /* Clear all interrupts */
                Cy_SD_Host_ClearErrorInterruptStatus(obj->base, _CYHAL_SDIO_SET_ALL_INTERRUPTS_MASK);
                Cy_SD_Host_ClearNormalInterruptStatus(obj->base, _CYHAL_SDIO_SET_ALL_INTERRUPTS_MASK);

                obj->irq_cause = 0UL;
                obj->events    = 0UL;

                obj->callback_data.callback = NULL;
                obj->callback_data.callback_arg = NULL;
                _cyhal_sdhc_config_structs[obj->resource.block_num] = obj;

                IRQn_Type irqn = _CYHAL_SDHC_IRQ_N[obj->resource.block_num];
                cy_stc_sysint_t irqCfg = { irqn, CYHAL_ISR_PRIORITY_DEFAULT };
                Cy_SysInt_Init(&irqCfg, _cyhal_sdio_irq_handler);
                NVIC_EnableIRQ(irqn);

                result = Cy_SD_Host_SetHostBusWidth(obj->base, CY_SD_HOST_BUS_WIDTH_4_BIT);

                /* Change the host SD clock to 400 kHz */
                if (result == CY_RSLT_SUCCESS)
                {
                    result = _cyhal_sdhc_sdcardchangeclock(&(obj->clock), obj->base, _CYHAL_SDIO_HOST_CLK_400K);

                    obj->frequencyhal_hz = _CYHAL_SDIO_HOST_CLK_400K;
                    obj->block_size = _CYHAL_SDIO_64B_BLOCK;
                }
            }
        }
    }

    if (result != CY_RSLT_SUCCESS)
    {
        cyhal_sdio_free(obj);
    }

    return result;
}

void cyhal_sdio_free(cyhal_sdio_t *obj)
{
    CY_ASSERT(NULL != obj);

    if (NULL != obj->base)
    {
        IRQn_Type irqn = _CYHAL_SDHC_IRQ_N[obj->resource.block_num];
        NVIC_DisableIRQ(irqn);

        Cy_SD_Host_DeInit(obj->base);

        cyhal_hwmgr_free(&(obj->resource));
        obj->base = NULL;

        _cyhal_sdhc_config_structs[obj->resource.block_num] = NULL;
        _cyhal_syspm_unregister_peripheral_callback(&obj->pm_callback_data);
    }

    if (obj->resource.type != CYHAL_RSC_INVALID)
    {
        obj->resource.type = CYHAL_RSC_INVALID;
        cyhal_clock_free(&(obj->clock));
    }

    /* Free pins */
    _cyhal_utils_release_if_used(&obj->pin_clk);
    _cyhal_utils_release_if_used(&obj->pin_cmd);
    _cyhal_utils_release_if_used(&obj->pin_data0);
    _cyhal_utils_release_if_used(&obj->pin_data1);
    _cyhal_utils_release_if_used(&obj->pin_data2);
    _cyhal_utils_release_if_used(&obj->pin_data3);
}

cy_rslt_t cyhal_sdio_configure(cyhal_sdio_t *obj, const cyhal_sdio_cfg_t *config)
{
    cy_rslt_t result = (cy_rslt_t)CY_SD_HOST_ERROR_TIMEOUT;

    if ((NULL == obj) || (config == NULL))
    {
        return CYHAL_SDIO_RSLT_ERR_BAD_PARAM;
    }

    if (config->frequencyhal_hz != 0U)
    {
        result = _cyhal_sdhc_sdcardchangeclock(&(obj->clock), obj->base, config->frequencyhal_hz);
        obj->frequencyhal_hz = config->frequencyhal_hz;
    }

    if (config->block_size != 0U)
    {
        /* No need to change anything in HW, because it will be overwritten
        *  in cyhal_sdio_bulk_transfer()/cyhal_sdio_transfer_async() functions.
        *  The HW block size will taken based on obj->block_size, which is
        *  updated here.
        */
        obj->block_size = config->block_size;
    }

    return result;
}

/*******************************************************************************
* Note that during this function execution the CY_SD_HOST_CMD_COMPLETE
* interrupt is disabled.
*******************************************************************************/
cy_rslt_t cyhal_sdio_send_cmd(const cyhal_sdio_t *obj, cyhal_transfer_t direction, \
                              cyhal_sdio_command_t command, uint32_t argument, uint32_t* response)
{
    CY_UNUSED_PARAMETER(direction);
    if (NULL == obj)
    {
        return CYHAL_SDIO_RSLT_ERR_BAD_PARAM;
    }

    if (obj->pm_transition_pending)
    {
        return CYHAL_SDIO_RSLT_ERR_PM_PENDING;
    }

    cy_rslt_t                   ret = CY_RSLT_SUCCESS;
    cy_en_sd_host_status_t      result = CY_SD_HOST_ERROR_TIMEOUT;
    cy_stc_sd_host_cmd_config_t cmd;
    uint32_t                    retry = _CYHAL_SDIO_TRANSFER_TRIES;
    uint32_t                    regIntrSts = Cy_SD_Host_GetNormalInterruptMask(obj->base);

    /* Clear out the response */
    if ( response != NULL )
    {
        *response = 0UL;
    }

    while ((CY_SD_HOST_SUCCESS != result) && (retry-- > 0UL))
    {
        /* First clear out the command complete and transfer complete statuses */
        Cy_SD_Host_ClearNormalInterruptStatus(obj->base, CY_SD_HOST_CMD_COMPLETE);

        /* Disable CMD Done interrupt, will be enabled after transition is complete */
        Cy_SD_Host_SetNormalInterruptMask(obj->base, (regIntrSts & (uint16_t)~CY_SD_HOST_CMD_COMPLETE));

        /* Check if an error occurred on any previous transactions */
        if ( Cy_SD_Host_GetNormalInterruptStatus(obj->base) & CY_SD_HOST_ERR_INTERRUPT )
        {
            /* Reset the block if there was an error. Note a full reset usually
             * requires more time, but this short version is working quite well and
             * successfully clears out the error state.
             */
            Cy_SD_Host_ClearErrorInterruptStatus(obj->base, _CYHAL_SDIO_SET_ALL_INTERRUPTS_MASK);
            Cy_SD_Host_SoftwareReset(obj->base, CY_SD_HOST_RESET_CMD_LINE);
            Cy_SD_Host_SoftwareReset(obj->base, CY_SD_HOST_RESET_DATALINE);
            Cy_SysLib_DelayUs(1);
            obj->base->CORE.SW_RST_R = 0U;
        }

        cmd.commandIndex                 = (uint32_t)command;
        cmd.commandArgument              = argument;
        cmd.enableCrcCheck               = true;
        cmd.enableAutoResponseErrorCheck = false;
        cmd.respType                     = CY_SD_HOST_RESPONSE_LEN_48;
        cmd.enableIdxCheck               = true;
        cmd.dataPresent                  = false;
        cmd.cmdType                      = CY_SD_HOST_CMD_NORMAL;

        result = Cy_SD_Host_SendCommand(obj->base, &cmd);

        if (CY_SD_HOST_SUCCESS == result)
        {
            result = _cyhal_sdhc_pollcmdcomplete(obj->base);
        }
    }

    result = Cy_SD_Host_GetResponse(obj->base, response, false);

    if (CY_SD_HOST_SUCCESS != result)
    {
        ret = CY_RSLT_TYPE_ERROR;
    }

    /* Restore interrupts after transition */
    Cy_SD_Host_SetNormalInterruptMask(obj->base, regIntrSts);

    return ret;
}


/*******************************************************************************
* \note
* During this function execution the CYHAL_SDIO_CMD_COMPLETE and
* CYHAL_SDIO_XFER_COMPLETE events are disabled.
* The CYHAL_SDIO_CMD_COMPLETE and CYHAL_SDIO_XFER_COMPLETE events are enabled
* after the asynchronous transfer is complete and in the condition they were
* enabled in before the transfer operation started.
*******************************************************************************/
cy_rslt_t cyhal_sdio_bulk_transfer(cyhal_sdio_t *obj, cyhal_transfer_t direction,
                                   uint32_t argument, const uint32_t* data,
                                   uint16_t length, uint32_t* response)
{
    if (NULL == obj)
    {
        return CYHAL_SDIO_RSLT_ERR_BAD_PARAM;
    }

    if (obj->pm_transition_pending)
    {
        return CYHAL_SDIO_RSLT_ERR_PM_PENDING;
    }

    cy_rslt_t                    ret = CY_RSLT_SUCCESS;
    uint32_t                     retry = _CYHAL_SDIO_TRANSFER_TRIES;
    cy_stc_sd_host_cmd_config_t  cmd;
    cy_stc_sd_host_data_config_t dat;
    cy_en_sd_host_status_t       result = CY_SD_HOST_ERROR_TIMEOUT;
    uint32_t                     regIntrSts = Cy_SD_Host_GetNormalInterruptMask(obj->base);

    /* Initialize data constants*/
    dat.autoCommand         = CY_SD_HOST_AUTO_CMD_NONE;
    dat.dataTimeout         = 0x0dUL;
    dat.enableIntAtBlockGap = false;
    dat.enReliableWrite     = false;
    dat.enableDma           = true;

    dat.read = ( direction == CYHAL_WRITE ) ? false : true;

    /* Clear out the response */
    if ( response != NULL )
    {
        *response = 0UL;
    }

    while ((CY_SD_HOST_SUCCESS != result) && (retry > 0UL))
    {
        /* Add SDIO Error Handling
         * SDIO write timeout is expected when doing first write to register
         * after KSO bit disable (as it goes to AOS core).
         * This timeout, however, triggers an error state in the hardware.
         * So, check for the error and then recover from it
         * as needed via reset issuance. This is the only time known that
         * a write timeout occurs.
         */

        /* First clear out the command complete and transfer complete statuses */
        Cy_SD_Host_ClearNormalInterruptStatus(obj->base, (CY_SD_HOST_XFER_COMPLETE | CY_SD_HOST_CMD_COMPLETE));

        /* Check if an error occurred on any previous transactions or reset
        *  after the first unsuccessful bulk transfer try
        */
        if ( (Cy_SD_Host_GetNormalInterruptStatus(obj->base) & CY_SD_HOST_ERR_INTERRUPT) ||
            (retry < _CYHAL_SDIO_TRANSFER_TRIES))
        {
            /* Reset the block if there was an error. Note a full reset usually
             * requires more time, but this short version is working quite well
             * and successfully clears out the error state.
             */
            Cy_SD_Host_ClearErrorInterruptStatus(obj->base, _CYHAL_SDIO_SET_ALL_INTERRUPTS_MASK);
            Cy_SD_Host_SoftwareReset(obj->base, CY_SD_HOST_RESET_CMD_LINE);
            Cy_SD_Host_SoftwareReset(obj->base, CY_SD_HOST_RESET_DATALINE);
            Cy_SysLib_DelayUs(1);
            obj->base->CORE.SW_RST_R = 0U;
        }

        /* Prepare the data transfer register */
        cmd.commandIndex                 = (uint32_t) CYHAL_SDIO_CMD_IO_RW_EXTENDED;
        cmd.commandArgument              = argument;
        cmd.enableCrcCheck               = true;
        cmd.enableAutoResponseErrorCheck = false;
        cmd.respType                     = CY_SD_HOST_RESPONSE_LEN_48;
        cmd.enableIdxCheck               = true;
        cmd.dataPresent                  = true;
        cmd.cmdType                      = CY_SD_HOST_CMD_NORMAL;
        dat.data                         = (uint32_t*)data;

        /* Block mode */
        if (length >= obj->block_size)
        {
            dat.blockSize     = obj->block_size;
            dat.numberOfBlock = ( length + obj->block_size - 1 ) / obj->block_size;
        }
        /* Byte mode */
        else
        {
            dat.blockSize     = length;
            dat.numberOfBlock = 1UL;
        }

        /* Disable XFER Done interrupt */
        Cy_SD_Host_SetNormalInterruptMask(obj->base, (regIntrSts &
                                                     (uint16_t)~(CY_SD_HOST_XFER_COMPLETE | CY_SD_HOST_CMD_COMPLETE)));

        result = Cy_SD_Host_InitDataTransfer(obj->base, &dat);

        if ( CY_SD_HOST_SUCCESS == result )
        {
            result = Cy_SD_Host_SendCommand(obj->base, &cmd);
        }

        if ( CY_SD_HOST_SUCCESS == result )
        {
            result = _cyhal_sdhc_pollcmdcomplete(obj->base);

            if ( CY_SD_HOST_SUCCESS == result )
            {
                result = _cyhal_sdhc_polltransfercomplete(obj->base, _CYHAL_SDIO_RW_TIMEOUT_US);
            }
        }
        retry--;
    }

    if (response != NULL && CY_SD_HOST_SUCCESS == result)
    {
        result = Cy_SD_Host_GetResponse(obj->base, response, false);
    }

    /* Restore interrupts after transition */
    Cy_SD_Host_SetNormalInterruptMask(obj->base, regIntrSts);

    if (CY_SD_HOST_SUCCESS != result)
    {
        ret = CY_RSLT_TYPE_ERROR;
    }

    if (0u != (CY_SD_HOST_CARD_INTERRUPT & obj->irq_cause))
    {
        uint32_t intrStsEn = Cy_SD_Host_GetNormalInterruptEnable(obj->base);

        /* This interrupt is disabled in interrupt handler so need to enable it here */
        Cy_SD_Host_SetNormalInterruptEnable(obj->base, (intrStsEn | CY_SD_HOST_CARD_INTERRUPT));
    }

    return ret;
}


/*******************************************************************************
*
* The asynchronous transfer is implemented on the CY_SD_HOST_CMD_COMPLETE and
* CY_SD_HOST_CMD_COMPLETE interrupts.
* The function sets up data and disables the CY_SD_HOST_CMD_COMPLETE and
* CY_SD_HOST_CMD_COMPLETE interrupts.
* After exiting this function the CYHAL_SDIO_CMD_COMPLETE and
* CYHAL_SDIO_XFER_COMPLETE events are not asserted.
*
* To complete the asynchronous transfer, call  cyhal_sdio_is_busy()
* until it returns false.
* The CYHAL_SDIO_CMD_COMPLETE and CYHAL_SDIO_XFER_COMPLETE events are enabled
* after the asynchronous transfer is complete and in the condition they were
* enabled in before the transfer operation started.
*
* \warning
* It is prohibited to enable CYHAL_SDIO_CMD_COMPLETE and
* CYHAL_SDIO_XFER_COMPLETE events during the asynchronous transfer operation.
*
*******************************************************************************/
cy_rslt_t cyhal_sdio_transfer_async(cyhal_sdio_t *obj, cyhal_transfer_t direction,
                                    uint32_t argument, const uint32_t* data, uint16_t length)
{
    if (NULL == obj)
    {
        return CYHAL_SDIO_RSLT_ERR_BAD_PARAM;
    }

    if (obj->pm_transition_pending)
    {
        return CYHAL_SDIO_RSLT_ERR_PM_PENDING;
    }

    cy_rslt_t                    ret = CY_RSLT_SUCCESS;
    uint32_t                     retry = _CYHAL_SDIO_TRANSFER_TRIES;
    cy_stc_sd_host_cmd_config_t  cmd;
    cy_stc_sd_host_data_config_t dat;
    cy_en_sd_host_status_t       result = CY_SD_HOST_ERROR_TIMEOUT;
    uint32_t                     intMaskReg = Cy_SD_Host_GetNormalInterruptMask(obj->base);
    uint32_t                     intEnableReg = Cy_SD_Host_GetNormalInterruptEnable(obj->base);

    /* Initialize data constants*/
    dat.autoCommand         = CY_SD_HOST_AUTO_CMD_NONE;
    dat.dataTimeout         = 0x0dUL;
    dat.enableIntAtBlockGap = false;
    dat.enReliableWrite     = false;
    dat.enableDma           = true;

    while ((CY_SD_HOST_SUCCESS != result) && (retry > 0UL))
    {
        /* Check if an error occurred on any previous transactions or reset after the first unsuccessful bulk transfer try */
        if ((Cy_SD_Host_GetNormalInterruptStatus(obj->base) & CY_SD_HOST_ERR_INTERRUPT) ||
            (retry < _CYHAL_SDIO_TRANSFER_TRIES))
        {
            /* Reset the block if there was an error. Note a full reset usually
             * requires more time, but this short version is working quite well and
             * successfully clears out the error state.
             */
            Cy_SD_Host_ClearErrorInterruptStatus(obj->base, _CYHAL_SDIO_SET_ALL_INTERRUPTS_MASK);
            Cy_SD_Host_SoftwareReset(obj->base, CY_SD_HOST_RESET_CMD_LINE);
            Cy_SD_Host_SoftwareReset(obj->base, CY_SD_HOST_RESET_DATALINE);
            Cy_SysLib_DelayUs(1U);
            obj->base->CORE.SW_RST_R = 0U;
        }

        /* Prepare the data transfer register */
        cmd.commandIndex                 = (uint32_t) CYHAL_SDIO_CMD_IO_RW_EXTENDED;
        cmd.commandArgument              = argument;
        cmd.enableCrcCheck               = true;
        cmd.enableAutoResponseErrorCheck = false;
        cmd.respType                     = CY_SD_HOST_RESPONSE_LEN_48;
        cmd.enableIdxCheck               = true;
        cmd.dataPresent                  = true;
        cmd.cmdType                      = CY_SD_HOST_CMD_NORMAL;
        dat.data                         = (uint32_t*)data;

        dat.read = ( direction == CYHAL_WRITE ) ? false : true;

        /* Block mode */
        if (length >= obj->block_size)
        {
            dat.blockSize     = obj->block_size;
            dat.numberOfBlock = ( length + obj->block_size - 1 ) / obj->block_size;
        }
        /* Byte mode */
        else
        {
            dat.blockSize     = length;
            dat.numberOfBlock = 1UL;
        }

        /* Disable XFER Done interrupt */
        Cy_SD_Host_SetNormalInterruptMask(obj->base, (intMaskReg &
                                                     (uint16_t)~(CY_SD_HOST_XFER_COMPLETE | CY_SD_HOST_CMD_COMPLETE)));

        result = Cy_SD_Host_InitDataTransfer(obj->base, &dat);

        if (CY_SD_HOST_SUCCESS == result)
        {
            result = Cy_SD_Host_SendCommand(obj->base, &cmd);
        }
        retry--;
    }

    if (CY_SD_HOST_SUCCESS != result)
    {
        ret = CY_RSLT_TYPE_ERROR;
    }
    else
    {
        /* Indicate that async transfer in progress */
        _cyhal_sdhc_transer_in_progress = true;
    }

    if (0U != (CY_SD_HOST_CARD_INTERRUPT & obj->irq_cause))
    {
        /* This interrupt is disabled in interrupt handler so need to enable it here */
        Cy_SD_Host_SetNormalInterruptEnable(obj->base, (intEnableReg | CY_SD_HOST_CARD_INTERRUPT));
    }

    return ret;
}

bool cyhal_sdio_is_busy(const cyhal_sdio_t *obj)
{
    static bool isCmdComplete = false;

    if (_cyhal_sdhc_transer_in_progress)
    {
        if (!isCmdComplete)
        {
            if (CY_SD_HOST_CMD_COMPLETE == (CY_SD_HOST_CMD_COMPLETE & Cy_SD_Host_GetNormalInterruptStatus(obj->base)))
            {
                Cy_SD_Host_ClearNormalInterruptStatus(obj->base, CY_SD_HOST_CMD_COMPLETE);
                isCmdComplete = true;
            }
        }

        if (isCmdComplete)
        {
            if (CY_SD_HOST_XFER_COMPLETE == (CY_SD_HOST_XFER_COMPLETE & Cy_SD_Host_GetNormalInterruptStatus(obj->base)))
            {
                Cy_SD_Host_ClearNormalInterruptStatus(obj->base, CY_SD_HOST_XFER_COMPLETE);

                _cyhal_sdhc_transer_in_progress = false;
                isCmdComplete = false;

                /* Restore XFER Done interrupt if it was previously enabled */
                if (0U != ((CY_SD_HOST_XFER_COMPLETE | CY_SD_HOST_CMD_COMPLETE) & obj->irq_cause))
                {
                    uint32_t intrStatus = Cy_SD_Host_GetNormalInterruptMask(obj->base);

                    if (0U != (CY_SD_HOST_XFER_COMPLETE & obj->irq_cause))
                    {
                        Cy_SD_Host_SetNormalInterruptMask(obj->base, (intrStatus | CY_SD_HOST_XFER_COMPLETE));
                    }

                    if (0U != (CY_SD_HOST_CMD_COMPLETE & obj->irq_cause))
                    {
                        Cy_SD_Host_SetNormalInterruptMask(obj->base, (intrStatus | CY_SD_HOST_CMD_COMPLETE));
                    }
                }
            }
        }
    }

    return _cyhal_sdhc_transer_in_progress;
}

cy_rslt_t cyhal_sdio_abort_async(const cyhal_sdio_t *obj)
{
    cy_rslt_t ret = CY_RSLT_SUCCESS;

    /* To abort transition reset dat and cmd lines (software reset) */
    Cy_SD_Host_SoftwareReset(obj->base, CY_SD_HOST_RESET_CMD_LINE);
    Cy_SD_Host_SoftwareReset(obj->base, CY_SD_HOST_RESET_DATALINE);
    Cy_SysLib_DelayUs(1U);
    obj->base->CORE.SW_RST_R = 0U;

    _cyhal_sdhc_transer_in_progress = false;

    return ret;
}

void cyhal_sdio_register_callback(cyhal_sdio_t *obj, cyhal_sdio_event_callback_t callback, void *callback_arg)
{
    uint32_t savedIntrStatus = cyhal_system_critical_section_enter();
    obj->callback_data.callback = (cy_israddress) callback;
    obj->callback_data.callback_arg = callback_arg;
    cyhal_system_critical_section_exit(savedIntrStatus);
}

void cyhal_sdio_enable_event(cyhal_sdio_t *obj, cyhal_sdio_irq_event_t event, uint8_t intr_priority, bool enable)
{
    /* Configure interrupt-based event(s) */
    if (0U != ((uint32_t) event & (uint32_t) CYHAL_SDIO_ALL_INTERRUPTS))
    {
        uint32_t interruptMask   = Cy_SD_Host_GetNormalInterruptMask(obj->base);

        IRQn_Type irqn = _CYHAL_SDHC_IRQ_N[obj->resource.block_num];
        NVIC_SetPriority(irqn, intr_priority);

        if (enable)
        {
            interruptMask   |= event;
            obj->irq_cause  |= event;
        }
        else
        {
            interruptMask   &= ~(event);
            obj->irq_cause  &= ~event;
        }

        Cy_SD_Host_ClearNormalInterruptStatus(obj->base, interruptMask);
        Cy_SD_Host_SetNormalInterruptMask(obj->base, interruptMask);
    }

    /* Configure non-interrupt based event(s) */
    if (0U != ((uint32_t) event & _CYHAL_SDIO_INTERFACE_CHANGE_MASK))
    {
        if (enable)
        {
            obj->events |= (uint32_t) event;
        }
        else
        {
            obj->events &= (uint32_t) ~((uint32_t) event);
        }
    }
}

#if defined(__cplusplus)
}
#endif

#endif /* CY_IP_MXSDHC */
