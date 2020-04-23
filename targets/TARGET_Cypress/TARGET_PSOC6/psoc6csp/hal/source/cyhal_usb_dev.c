/*******************************************************************************
* File Name: cyhal_usb_dev.c
*
* \brief
* Provides a high level interface for interacting with the Cypress USB Device.
* This interface abstracts out the chip specific details.
* If any chip specific functionality is necessary, or performance is critical
* the low level functions can be used directly.
*
********************************************************************************
* \copyright
* Copyright 2019-2020 Cypress Semiconductor Corporation
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

#include "cyhal_usb_dev.h"
#include "cyhal_gpio.h"
#include "cyhal_hwmgr.h"
#include "cyhal_interconnect.h"
#include "cyhal_utils.h"

#if defined(CY_IP_MXUSBFS)

#if defined(__cplusplus)
extern "C"
{
#endif

/* Interrupt configuration and access */
#define CYHAL_USB_DEV_DEFAULT_IRQ_PRIORITY    (3U)

/* Assign all interrupt sources to Low interrupt */
#define CYHAL_USB_DEV_IRQ_LVL                   (CY_USBFS_DEV_DRV_LVL_LOW)
#define CYHAL_USB_DEV_IRQ_LVL_DEFAULT           (CY_USBFS_DEV_DRV_SET_SOF_LVL(CYHAL_USB_DEV_IRQ_LVL)      | \
                                                CY_USBFS_DEV_DRV_SET_BUS_RESET_LVL(CYHAL_USB_DEV_IRQ_LVL) | \
                                                CY_USBFS_DEV_DRV_SET_EP0_LVL(CYHAL_USB_DEV_IRQ_LVL)       | \
                                                CY_USBFS_DEV_DRV_SET_LPM_LVL(CYHAL_USB_DEV_IRQ_LVL)       | \
                                                CY_USBFS_DEV_DRV_SET_ARB_EP_LVL(CYHAL_USB_DEV_IRQ_LVL)    | \
                                                CY_USBFS_DEV_DRV_SET_EP1_LVL(CYHAL_USB_DEV_IRQ_LVL)       | \
                                                CY_USBFS_DEV_DRV_SET_EP2_LVL(CYHAL_USB_DEV_IRQ_LVL)       | \
                                                CY_USBFS_DEV_DRV_SET_EP3_LVL(CYHAL_USB_DEV_IRQ_LVL)       | \
                                                CY_USBFS_DEV_DRV_SET_EP4_LVL(CYHAL_USB_DEV_IRQ_LVL)       | \
                                                CY_USBFS_DEV_DRV_SET_EP5_LVL(CYHAL_USB_DEV_IRQ_LVL)       | \
                                                CY_USBFS_DEV_DRV_SET_EP6_LVL(CYHAL_USB_DEV_IRQ_LVL)       | \
                                                CY_USBFS_DEV_DRV_SET_EP7_LVL(CYHAL_USB_DEV_IRQ_LVL)       | \
                                                CY_USBFS_DEV_DRV_SET_EP8_LVL(CYHAL_USB_DEV_IRQ_LVL))

#define CYHAL_USB_DEV_IS_EVENT_VALID(event)     ( ((event) == CYHAL_USB_DEV_EVENT_BUS_RESET) || \
                                                  ((event) == CYHAL_USB_DEV_EVENT_EP0_SETUP) || \
                                                  ((event) == CYHAL_USB_DEV_EVENT_EP0_IN)    || \
                                                  ((event) == CYHAL_USB_DEV_EVENT_EP0_OUT) )

#define CYHAL_USB_DEV_IS_EP_NUM_VALID(endpoint)     CY_USBFS_DEV_DRV_IS_EP_VALID(endpoint)
#define CYHAL_USB_DEV_IS_EP0_SIZE_VALID(ep0_size)   (ep0_size >= CY_USBFS_DEV_DRV_EP0_BUFFER_SIZE)

#define CYHAL_USB_DEV_EVENT_NUM          (4)
#define CYHAL_USB_DEV_EP_EVENT_NUM       (CY_USBFS_DEV_DRV_NUM_EPS_MAX)

/* Clock configuration constants */
#define CYHAL_USB_DEV_USB_CLK_HF                (3U)        /* USBFS block dedicated path clock: CLK_HF3 */
#define CYHAL_USB_DEV_USB_CLK_HF_FREQ           (48000000U) /* CLK_HF3 required frequency, Hz */
#define CYHAL_USB_DEV_BUS_RESET_CLOCK_HZ        (100000U)   /* Bus Reset require frequency, Hz */

typedef enum
{
    CYHAL_USB_DEV_EP1_IDX = 0,     /* Callback number for Endpoint 1 completion interrupt */
    CYHAL_USB_DEV_EP2_IDX = 1,     /* Callback number for Endpoint 2 completion interrupt */
    CYHAL_USB_DEV_EP3_IDX = 2,     /* Callback number for Endpoint 3 completion interrupt */
    CYHAL_USB_DEV_EP4_IDX = 3,     /* Callback number for Endpoint 4 completion interrupt */
    CYHAL_USB_DEV_EP5_IDX = 4,     /* Callback number for Endpoint 5 completion interrupt */
    CYHAL_USB_DEV_EP6_IDX = 5,     /* Callback number for Endpoint 6 completion interrupt */
    CYHAL_USB_DEV_EP7_IDX = 6,     /* Callback number for Endpoint 7 completion interrupt */
    CYHAL_USB_DEV_EP8_IDX = 7,     /* Callback number for Endpoint 8 completion interrupt */
} cyhal_usb_dev_ep_cb_num_t;

static USBFS_Type* const CYHAL_USB_DEV_BASE_ADDRESSES[CY_IP_MXUSBFS_INSTANCES] =
{
#ifdef USBFS0
    USBFS0,
#endif
};

static const IRQn_Type CYHAL_USBDEV_IRQ_N[CY_IP_MXUSBFS_INSTANCES] =
{
#ifdef USBFS0
    usb_interrupt_lo_IRQn,
#endif
};


static bool      cyhal_usb_dev_set_hf_divider(uint32_t clock, uint32_t input_freq, uint32_t target_freq);
static cy_rslt_t cyhal_usb_dev_reseve_pll(cyhal_resource_inst_t *rsc);
static cy_rslt_t cyhal_usb_dev_init_pll(uint32_t clock, uint32_t pll, uint32_t target_freq);
static uint32_t cyhal_usb_dev_get_pll_freq(uint32_t path);
static cy_rslt_t cyhal_usb_dev_hf_clock_setup(cyhal_usb_dev_t *obj);
static cy_rslt_t cyhal_usb_dev_peri_clock_setup(cyhal_usb_dev_t *obj, const cyhal_clock_divider_t *clk);

static cy_rslt_t cyhal_usb_dev_pin_setup(cyhal_usb_dev_t *obj, cyhal_gpio_t dp, cyhal_gpio_t dm);
static void cyhal_usb_dev_free_resources(cyhal_usb_dev_t *obj);

static void cyhal_usb_0_dev_bus_reset_callback(USBFS_Type *base, struct cy_stc_usbfs_dev_drv_context *drvContext);
static void cyhal_usb_0_dev_ep0_setup_callback(USBFS_Type *base, struct cy_stc_usbfs_dev_drv_context *drvContext);
static void cyhal_usb_0_dev_ep0_in_callback(USBFS_Type *base, struct cy_stc_usbfs_dev_drv_context *drvContext);
static void cyhal_usb_0_dev_ep0_out_callback(USBFS_Type *base, struct cy_stc_usbfs_dev_drv_context *drvContext);
static cyhal_usb_dev_event_callback_t cyhal_usb_dev_event_callback_table[CY_IP_MXUSBFS_INSTANCES][CYHAL_USB_DEV_EVENT_NUM];
static const cy_cb_usbfs_dev_drv_callback_t cyhal_usb_dev_drv_event_cb_table[CY_IP_MXUSBFS_INSTANCES][CYHAL_USB_DEV_EVENT_NUM] =
{
    /* USBFS0 */
    {
        &cyhal_usb_0_dev_bus_reset_callback,    /* CY_USB_DEV_BUS_RESET = 0U */
        &cyhal_usb_0_dev_ep0_setup_callback,    /* CY_USB_DEV_EP0_SETUP = 1U */
        &cyhal_usb_0_dev_ep0_in_callback,       /* CY_USB_DEV_EP0_IN    = 2U */
        &cyhal_usb_0_dev_ep0_out_callback       /* CY_USB_DEV_EP0_OUT   = 3U */
    },
};

static void cyhal_usb_0_dev_sof_callback(USBFS_Type *base, struct cy_stc_usbfs_dev_drv_context *drvContext);
static cyhal_usb_dev_sof_callback_t cyhal_usb_dev_sof_user_callback[CY_IP_MXUSBFS_INSTANCES];
static const cy_cb_usbfs_dev_drv_callback_t cyhal_usb_dev_drv_sof_cb_table[CY_IP_MXUSBFS_INSTANCES]=
{
    /* USBFS0 */
    &cyhal_usb_0_dev_sof_callback,
};

static void cyhal_usb_0_dev_ep_callback(USBFS_Type *base, uint32_t ep_addr, uint32_t errorType, cy_stc_usbfs_dev_drv_context_t *drvContext);
static cyhal_usb_dev_endpoint_callback_t cyhal_usb_dev_ep_handler_table[CY_IP_MXUSBFS_INSTANCES][CYHAL_USB_DEV_EP_EVENT_NUM];
static const cy_cb_usbfs_dev_drv_ep_callback_t cyhal_usb_dev_drv_ep_cb_table[CY_IP_MXUSBFS_INSTANCES] =
{
    /* USBFS0 */
    &cyhal_usb_0_dev_ep_callback,
};


static void cyhal_usb_0_dev_bus_reset_callback(USBFS_Type *base, struct cy_stc_usbfs_dev_drv_context *drvContext)
{
    (void) base; (void) drvContext;

    cyhal_usb_dev_event_callback_table[0][CYHAL_USB_DEV_EVENT_BUS_RESET]();
}


static void cyhal_usb_0_dev_ep0_setup_callback(USBFS_Type *base, struct cy_stc_usbfs_dev_drv_context *drvContext)
{
    (void) base; (void) drvContext;

    cyhal_usb_dev_event_callback_table[0][CYHAL_USB_DEV_EVENT_EP0_SETUP]();
}


static void cyhal_usb_0_dev_ep0_in_callback(USBFS_Type *base, struct cy_stc_usbfs_dev_drv_context *drvContext)
{
    (void) base; (void) drvContext;

    cyhal_usb_dev_event_callback_table[0][CYHAL_USB_DEV_EVENT_EP0_IN]();
}


static void cyhal_usb_0_dev_ep0_out_callback(USBFS_Type *base, struct cy_stc_usbfs_dev_drv_context *drvContext)
{
    (void) base; (void) drvContext;

    cyhal_usb_dev_event_callback_table[0][CYHAL_USB_DEV_EVENT_EP0_OUT]();
}


static void cyhal_usb_0_dev_sof_callback(USBFS_Type *base, struct cy_stc_usbfs_dev_drv_context *drvContext)
{
    (void) drvContext;

    if (NULL != cyhal_usb_dev_sof_user_callback[0])
    {
        uint32_t frame_number = Cy_USBFS_Dev_Drv_GetSofNubmer(base);
        cyhal_usb_dev_sof_user_callback[0](frame_number);
    }
}


static void cyhal_usb_0_dev_ep_callback(USBFS_Type *base, uint32_t ep_addr, uint32_t errorType, cy_stc_usbfs_dev_drv_context_t *drvContext)
{
    (void) base; (void) errorType; (void) drvContext;
    uint32_t endpoint_idx = CYHAL_USB_DEV_GET_EP_IDX(ep_addr);
    cyhal_usb_dev_endpoint_callback_t ep_handler = cyhal_usb_dev_ep_handler_table[0U][endpoint_idx];

    if (NULL != ep_handler)
    {
        ep_handler(ep_addr);
    }
}


static bool cyhal_usb_dev_set_hf_divider(uint32_t clock, uint32_t input_freq, uint32_t target_freq)
{
    bool divider_found;
    cy_en_clkhf_dividers_t divider;

    if (target_freq == input_freq)
    {
        divider_found = true;
        divider = CY_SYSCLK_CLKHF_NO_DIVIDE;
    }
    else if ((target_freq * 2U) == input_freq)
    {
        divider_found = true;
        divider = CY_SYSCLK_CLKHF_DIVIDE_BY_2;
    }
    else if ((target_freq * 4U) == input_freq)
    {
        divider_found = true;
        divider = CY_SYSCLK_CLKHF_DIVIDE_BY_4;
    }
    else if ((target_freq * 8U) == input_freq)
    {
        divider_found = true;
        divider = CY_SYSCLK_CLKHF_DIVIDE_BY_8;
    }
    else
    {
        divider_found = false;
    }

    if (divider_found)
    {
        Cy_SysClk_ClkHfSetDivider(clock, divider);
    }

    return divider_found;
}


static cy_rslt_t cyhal_usb_dev_reseve_pll(cyhal_resource_inst_t *rsc)
{
    cy_rslt_t result = CYHAL_USB_DEV_RSLT_ERR_CLK_CFG;
    uint32_t i;

    rsc->type = CYHAL_RSC_CLKPATH;
    rsc->channel_num = 0U;

    for (i = 1U; i <= SRSS_NUM_PLL; ++i)
    {
        /* Set block number to path number that belongs to PLL */
        rsc->block_num = i;

        if (CY_RSLT_SUCCESS == cyhal_hwmgr_reserve(rsc))
        {
            result = CY_RSLT_SUCCESS;
            break;
        }
    }

    if (CY_RSLT_SUCCESS != result)
    {
        /* Reservation failed mark type invalid */
        rsc->type = CYHAL_RSC_INVALID;
    }

    return result;
}

static cy_rslt_t cyhal_usb_dev_init_pll(uint32_t clock, uint32_t pll, uint32_t target_freq)
{
    cy_stc_pll_config_t cfg;

    Cy_SysClk_PllDisable(pll);
    Cy_SysClk_ClkHfSetSource(clock, (cy_en_clkhf_in_sources_t)(pll));

    cfg.inputFreq  = CY_SYSCLK_IMO_FREQ;
    cfg.outputFreq = target_freq;
    cfg.lfMode     = false;
    cfg.outputMode = CY_SYSCLK_FLLPLL_OUTPUT_AUTO;

    Cy_SysClk_ClkPathSetSource(pll, CY_SYSCLK_CLKPATH_IN_IMO);
    cy_rslt_t result = Cy_SysClk_PllConfigure(pll, &cfg);

    if (result == CY_RSLT_SUCCESS)
    {
        /* Wait up to 1 seconds for PLL to lock */
        result = Cy_SysClk_PllEnable(pll, 1000000);
    }

    if (result == CY_RSLT_SUCCESS)
    {
        Cy_SysClk_ClkHfSetDivider(clock, CY_SYSCLK_CLKHF_NO_DIVIDE);
    }

    return result;
}

static uint32_t cyhal_usb_dev_get_pll_freq(uint32_t path)
{
    /* PLL sourced from the IMO */
    uint32_t freq = CY_SYSCLK_IMO_FREQ;

    cy_stc_pll_manual_config_t pll_config;
    Cy_SysClk_PllGetConfiguration(path, &pll_config);

    if (pll_config.outputMode != CY_SYSCLK_FLLPLL_OUTPUT_INPUT)
    {
        freq = (uint32_t)CY_SYSLIB_DIV_ROUND(((uint64_t)freq * (uint64_t)pll_config.feedbackDiv),
                                             ((uint64_t)pll_config.referenceDiv * (uint64_t)pll_config.outputDiv));
    }

    return freq;
}

static cy_rslt_t cyhal_usb_dev_hf_clock_setup(cyhal_usb_dev_t *obj)
{
    cy_rslt_t result = CYHAL_USB_DEV_RSLT_ERR_CLK_CFG;
    uint32_t  clock  = CYHAL_USB_DEV_USB_CLK_HF;
    uint32_t path;

    /* Start CLK_HF3 configuration */
    Cy_SysClk_ClkHfDisable(clock);

    /* Loop through all enabled PLLs and find one that matches requirements:
    * sourced from IMO and frequency in multiple of 48MHz.
    */
    for (path = 1U; path <= SRSS_NUM_PLL; ++path)
    {
        if ( Cy_SysClk_PllIsEnabled(path) &&
            (CY_SYSCLK_CLKPATH_IN_IMO == Cy_SysClk_ClkPathGetSource(path)) )
        {
            /* Get PLL frequency */
            uint32_t clk_pll_freq = cyhal_usb_dev_get_pll_freq(path);

            /* Try to adjust CLK_HF3 divider to meet frequency requirements (48 MHz) */
            if (cyhal_usb_dev_set_hf_divider(clock, clk_pll_freq, CYHAL_USB_DEV_USB_CLK_HF_FREQ))
            {
                /* Change path if it does not match after successful divider update */
                if (Cy_SysClk_ClkHfGetSource(clock) != path)
                {
                    Cy_SysClk_ClkHfSetSource(clock, (cy_en_clkhf_in_sources_t) path);
                }

                /* The existing PLL is used, mark to not try clear it */
                result = CY_RSLT_SUCCESS;
                break;
            }
        }
    }

    /* None of existing PLLs do meet USB requirements, try to allocate free */
    if (CY_RSLT_SUCCESS != result)
    {
        result = cyhal_usb_dev_reseve_pll(&(obj->pll_resource));

        if (CY_RSLT_SUCCESS == result)
        {
            /* Sets PLL source IMO and clear CLK_HF3 divider */
            result = cyhal_usb_dev_init_pll(clock, obj->pll_resource.block_num, CYHAL_USB_DEV_USB_CLK_HF_FREQ);
        }
    }

    /* End CLK_HF3 configuration */
    Cy_SysClk_ClkHfEnable(clock);

    if (result == CY_RSLT_SUCCESS)
    {
        SystemCoreClockUpdate();
    }

    return result;
}

static cy_rslt_t cyhal_usb_dev_peri_clock_setup(cyhal_usb_dev_t *obj, const cyhal_clock_divider_t *clk)
{
    cy_rslt_t result;
    cy_en_sysclk_status_t status = CY_SYSCLK_BAD_PARAM;

    if (NULL == clk)
    {
        obj->shared_clock = false;

        /* USB bus reset clock must be 100KHz. Usual peri clock frequency is > 26 MHz, which requires 16-bit divider */
        result = cyhal_hwmgr_allocate_clock(&(obj->clock), CY_SYSCLK_DIV_16_BIT, true);

        if (CY_RSLT_SUCCESS == result)
        {
            /* Get divider to provide 100kHz clock or less */
            uint32_t div_value = (Cy_SysClk_ClkPeriGetFrequency() / CYHAL_USB_DEV_BUS_RESET_CLOCK_HZ) - 1U;

            (void) Cy_SysClk_PeriphDisableDivider(obj->clock.div_type, obj->clock.div_num);
            status = Cy_SysClk_PeriphSetDivider(obj->clock.div_type, obj->clock.div_num, div_value);
            (void) Cy_SysClk_PeriphEnableDivider(obj->clock.div_type, obj->clock.div_num);
        }
    }
    else
    {
        obj->clock = *clk;
        obj->shared_clock = true;
        status = CY_SYSCLK_SUCCESS;
    }

    if (CY_SYSCLK_SUCCESS == status)
    {
        status = Cy_SysClk_PeriphAssignDivider(PCLK_USB_CLOCK_DEV_BRS, obj->clock.div_type, obj->clock.div_num);
    }

    result = (CY_SYSCLK_SUCCESS == status) ? CY_RSLT_SUCCESS : CYHAL_USB_DEV_RSLT_ERR_CLK_CFG;

    return result;
}

static cy_rslt_t cyhal_usb_dev_pin_setup(cyhal_usb_dev_t *obj, cyhal_gpio_t dp, cyhal_gpio_t dm)
{
    cy_rslt_t result = CYHAL_USB_DEV_RSLT_ERR;

    const cyhal_resource_pin_mapping_t *dp_map = CY_UTILS_GET_RESOURCE(dp, cyhal_pin_map_usb_usb_dp_pad);
    const cyhal_resource_pin_mapping_t *dm_map = CY_UTILS_GET_RESOURCE(dm, cyhal_pin_map_usb_usb_dm_pad);

    if((NULL != dp_map) && (NULL != dm_map) &&
       (dp_map->inst->block_num == dm_map->inst->block_num))
    {
        cyhal_resource_inst_t pin_rsc;

        obj->resource = *dp_map->inst;

        /* reserve DM and DP pins */
        pin_rsc = cyhal_utils_get_gpio_resource(dp);
        result = cyhal_hwmgr_reserve(&pin_rsc);
        if (CY_RSLT_SUCCESS == result)
        {
            obj->pin_dp = dp;
            pin_rsc = cyhal_utils_get_gpio_resource(dm);
            result = cyhal_hwmgr_reserve(&pin_rsc);
        }

        if (CY_RSLT_SUCCESS == result)
        {
            obj->pin_dm = dm;
            result = cyhal_connect_pin(dp_map);
        }

        if (CY_RSLT_SUCCESS == result)
        {
            result = cyhal_connect_pin(dm_map);
        }
    }

    return result;
}

static void cyhal_usb_dev_free_resources(cyhal_usb_dev_t *obj)
{
    /* The object set into the default state in the cyhal_usb_dev_init() */

    if (CYHAL_RSC_INVALID != obj->resource.type)
    {
        cyhal_hwmgr_free(&(obj->resource));
    }

    if (CYHAL_RSC_INVALID != obj->pll_resource.type)
    {
        cyhal_hwmgr_free(&(obj->pll_resource));
    }

    if (!obj->shared_clock)
    {
        cyhal_hwmgr_free_clock(&(obj->clock));
    }

    cyhal_utils_release_if_used(&(obj->pin_dp));
    cyhal_utils_release_if_used(&(obj->pin_dm));
}

cy_rslt_t cyhal_usb_dev_init(cyhal_usb_dev_t *obj, cyhal_gpio_t dp, cyhal_gpio_t dm, const cyhal_clock_divider_t *clk)
{
    cy_rslt_t result;

    CY_ASSERT(NULL != obj);

    /* Reset object into the default state to handle resource free */
    obj->base = NULL;
    obj->resource.type     = CYHAL_RSC_INVALID;
    obj->pll_resource.type = CYHAL_RSC_INVALID;
    obj->shared_clock     = true;
    obj->pin_dp = CYHAL_NC_PIN_VALUE;
    obj->pin_dm = CYHAL_NC_PIN_VALUE;

    result = cyhal_usb_dev_pin_setup(obj, dp, dm);

    if (CY_RSLT_SUCCESS == result)
    {
        /* Configure CLK_HF3 frequency and proper path */
        result = cyhal_usb_dev_hf_clock_setup(obj);

        if (CY_RSLT_SUCCESS == result)
        {
            /* Configure CLK_PERI divider */
            result = cyhal_usb_dev_peri_clock_setup(obj, clk);
        }
    }

    if (CY_RSLT_SUCCESS == result)
    {
        result = cyhal_hwmgr_reserve(&(obj->resource));

        obj->base = CYHAL_USB_DEV_BASE_ADDRESSES[obj->resource.block_num];
    }

    if (CY_RSLT_SUCCESS == result)
    {
        /* Configure driver */
        static cy_stc_usbfs_dev_drv_config_t default_cfg =
        {
            .mode         = CY_USBFS_DEV_DRV_EP_MANAGEMENT_CPU,
            .dmaConfig[0] = NULL,
            .dmaConfig[1] = NULL,
            .dmaConfig[2] = NULL,
            .dmaConfig[3] = NULL,
            .dmaConfig[4] = NULL,
            .dmaConfig[5] = NULL,
            .dmaConfig[6] = NULL,
            .dmaConfig[7] = NULL,
            .epBuffer     = NULL,
            .epBufferSize = 0U,
            .intrLevelSel = CYHAL_USB_DEV_IRQ_LVL_DEFAULT,
            .enableLpm    = false,
            .epAccess     = CY_USBFS_DEV_DRV_USE_8_BITS_DR
        };

        if (CY_USBFS_DEV_DRV_SUCCESS != Cy_USBFS_Dev_Drv_Init(obj->base,
                                                                &default_cfg,
                                                                &(obj->context)))
        {
            result = CYHAL_USB_DEV_RSLT_ERR;
        }
    }

    if (CY_RSLT_SUCCESS == result)
    {
        uint32_t instance = obj->resource.block_num;
        uint32_t cb_num;

        /* Register service callbacks */
        Cy_USBFS_Dev_Drv_RegisterServiceCallback(obj->base, CY_USB_DEV_BUS_RESET,
                                                 cyhal_usb_dev_drv_event_cb_table[instance][CY_USB_DEV_BUS_RESET],
                                                 &(obj->context));
        Cy_USBFS_Dev_Drv_RegisterServiceCallback(obj->base, CY_USB_DEV_EP0_SETUP,
                                                 cyhal_usb_dev_drv_event_cb_table[instance][CY_USB_DEV_EP0_SETUP],
                                                 &(obj->context));
        Cy_USBFS_Dev_Drv_RegisterServiceCallback(obj->base, CY_USB_DEV_EP0_IN,
                                                 cyhal_usb_dev_drv_event_cb_table[instance][CY_USB_DEV_EP0_IN],
                                                 &(obj->context));
        Cy_USBFS_Dev_Drv_RegisterServiceCallback(obj->base, CY_USB_DEV_EP0_OUT,
                                                 cyhal_usb_dev_drv_event_cb_table[instance][CY_USB_DEV_EP0_OUT],
                                                 &(obj->context));

        /* Register sof callback (it enables sof interrupt, so disable it after registration) */
        Cy_USBFS_Dev_Drv_RegisterSofCallback(obj->base, cyhal_usb_dev_drv_sof_cb_table[instance],
                                             &(obj->context));
        cyhal_usb_dev_sof_enable(obj, false);

        /* Register data endpoint handlers */
        for (cb_num = 0; cb_num < CYHAL_USB_DEV_EP_EVENT_NUM; cb_num++)
        {
            Cy_USBFS_Dev_Drv_RegisterEndpointCallback(obj->base,
                                                      (cb_num + 1U),
                                                      cyhal_usb_dev_drv_ep_cb_table[instance],
                                                      &(obj->context));
        }
    }

    if (CY_RSLT_SUCCESS != result)
    {
        cyhal_usb_dev_free_resources(obj);
    }

    return result;
}


void cyhal_usb_dev_free(cyhal_usb_dev_t *obj)
{
    cyhal_usb_dev_irq_enable(obj, false);
    cyhal_usb_dev_disconnect(obj);
    cyhal_usb_dev_free_resources(obj);
}


void cyhal_usb_dev_connect(cyhal_usb_dev_t *obj)
{
    Cy_USBFS_Dev_Drv_Enable(obj->base, &(obj->context));
}


void cyhal_usb_dev_disconnect(cyhal_usb_dev_t *obj)
{
    Cy_USBFS_Dev_Drv_Disable(obj->base, &(obj->context));
}


void cyhal_usb_dev_set_configured(cyhal_usb_dev_t *obj)
{
    Cy_USBFS_Dev_Drv_ConfigDevice(obj->base, &(obj->context));
}


void cyhal_usb_dev_set_unconfigured(cyhal_usb_dev_t *obj)
{
    Cy_USBFS_Dev_Drv_UnConfigureDevice(obj->base, &(obj->context));
}


void cyhal_usb_dev_sof_enable(cyhal_usb_dev_t *obj, bool enable)
{
    uint32_t mask = Cy_USBFS_Dev_Drv_GetSieInterruptMask(obj->base);

    if (enable)
    {
        mask |= CY_USBFS_DEV_DRV_INTR_SIE_SOF;
        Cy_USBFS_Dev_Drv_ClearSieInterrupt(obj->base, CY_USBFS_DEV_DRV_INTR_SIE_SOF);
    }
    else
    {
        mask &= ~CY_USBFS_DEV_DRV_INTR_SIE_SOF;
    }

    Cy_USBFS_Dev_Drv_SetSieInterruptMask(obj->base, mask);
}


void cyhal_usb_dev_set_address(cyhal_usb_dev_t *obj, uint8_t address)
{
    Cy_USBFS_Dev_Drv_SetAddress(obj->base, address, &(obj->context));
}


uint32_t cyhal_usb_dev_ep0_get_max_packet(cyhal_usb_dev_t *obj)
{
    return Cy_USBFS_Dev_Drv_GetEp0MaxPacket(obj->base);
}


void cyhal_usb_dev_ep0_setup_read_result(cyhal_usb_dev_t *obj, uint8_t *buffer, uint32_t size)
{
    CY_ASSERT_L1(CYHAL_USB_DEV_IS_EP0_SIZE_VALID(size));

    Cy_USBFS_Dev_Drv_Ep0GetSetup(obj->base, buffer, &(obj->context));
}


void cyhal_usb_dev_ep0_read(cyhal_usb_dev_t *obj, uint8_t *buffer, uint32_t size)
{
    Cy_USBFS_Dev_Drv_Ep0Read(obj->base, buffer, size, &(obj->context));
}


uint32_t cyhal_usb_dev_ep0_read_result(cyhal_usb_dev_t *obj)
{
    return Cy_USBFS_Dev_Drv_Ep0ReadResult(obj->base, &(obj->context));
}


uint32_t cyhal_usb_dev_ep0_write(cyhal_usb_dev_t *obj, uint8_t *buffer, uint32_t size)
{
    return Cy_USBFS_Dev_Drv_Ep0Write(obj->base, buffer, size, &(obj->context));
}


void cyhal_usb_dev_ep0_stall(cyhal_usb_dev_t *obj)
{
    Cy_USBFS_Dev_Drv_Ep0Stall(obj->base);
}


cy_rslt_t cyhal_usb_dev_endpoint_add(cyhal_usb_dev_t *obj, bool alloc, bool enable,
                                     cyhal_usb_dev_ep_t endpoint, uint32_t max_packet, cyhal_usb_dev_ep_type_t type)
{
    cy_rslt_t result = CYHAL_USB_DEV_RSLT_ERR;
    cy_stc_usb_dev_ep_config_t ep_config;

    /* Set parameters to allocate endpoint buffer */
    ep_config.allocBuffer    = alloc;
    ep_config.bufferSize     = (uint16_t) max_packet;

    /* Set parameters to enable endpoint operation */
    ep_config.enableEndpoint = enable;
    ep_config.endpointAddr   = (uint8_t) endpoint;
    ep_config.attributes     = (uint8_t) type;
    ep_config.maxPacketSize  = (uint16_t) max_packet;

    if (CY_USBFS_DEV_DRV_SUCCESS ==  Cy_USBFS_Dev_Drv_AddEndpoint(obj->base,
                                                                  &ep_config,
                                                                  &(obj->context)))
    {
        result = CY_RSLT_SUCCESS;
    }

    return result;
}


cy_rslt_t cyhal_usb_dev_endpoint_remove(cyhal_usb_dev_t *obj, cyhal_usb_dev_ep_t endpoint)
{
    cy_rslt_t result = CYHAL_USB_DEV_RSLT_ERR;

    if (CY_USBFS_DEV_DRV_SUCCESS == Cy_USBFS_Dev_Drv_RemoveEndpoint(obj->base,
                                                                    endpoint,
                                                                    &(obj->context)))
    {
        result = CY_RSLT_SUCCESS;
    }

    return result;
}


cy_rslt_t cyhal_usb_dev_endpoint_stall(cyhal_usb_dev_t *obj, cyhal_usb_dev_ep_t endpoint)
{
    cy_rslt_t result = CYHAL_USB_DEV_RSLT_ERR;

    if (CY_USBFS_DEV_DRV_SUCCESS == Cy_USBFS_Dev_Drv_StallEndpoint(obj->base,
                                                                   CYHAL_USB_DEV_GET_EP_NUM(endpoint),
                                                                   &(obj->context)))
    {
        result = CY_RSLT_SUCCESS;
    }

    return result;
}


cy_rslt_t cyhal_usb_dev_endpoint_unstall(cyhal_usb_dev_t *obj, cyhal_usb_dev_ep_t endpoint)
{
    cy_rslt_t result = CYHAL_USB_DEV_RSLT_ERR;

    if (CY_USBFS_DEV_DRV_SUCCESS == Cy_USBFS_Dev_Drv_UnStallEndpoint(obj->base,
                                                                     CYHAL_USB_DEV_GET_EP_NUM(endpoint),
                                                                     &(obj->context)))
    {
        result = CY_RSLT_SUCCESS;
    }

    return result;
}


bool cyhal_usb_dev_endpoint_is_stalled(cyhal_usb_dev_t *obj, cyhal_usb_dev_ep_t endpoint)
{
    bool is_stalled = (CY_USB_DEV_EP_STALLED == Cy_USBFS_Dev_Drv_GetEndpointState(obj->base,
                                                                                  CYHAL_USB_DEV_GET_EP_NUM(endpoint),
                                                                                  &(obj->context)));

    return is_stalled;
}


cy_rslt_t cyhal_usb_dev_endpoint_read(cyhal_usb_dev_t *obj, cyhal_usb_dev_ep_t endpoint, uint8_t *data, uint32_t size)
{
    cy_rslt_t result = CYHAL_USB_DEV_RSLT_ERR;
    uint32_t  ep_num = CYHAL_USB_DEV_GET_EP_NUM(endpoint);

    /* Check if endpoint is read for a read operation */
    cy_en_usb_dev_ep_state_t ep_state;
    ep_state = Cy_USBFS_Dev_Drv_GetEndpointState(obj->base, ep_num, &(obj->context));
    if ((CY_USB_DEV_EP_IDLE == ep_state) || (CY_USB_DEV_EP_COMPLETED == ep_state))
    {
        /* Save pointer and size to use in cyhal_usb_dev_endpoint_read_result */
        obj->rd_data[CYHAL_USB_DEV_GET_EP_IDX(endpoint)] = data;
        obj->rd_size[CYHAL_USB_DEV_GET_EP_IDX(endpoint)] = size;

        Cy_USBFS_Dev_Drv_EnableOutEndpoint(obj->base, ep_num, &obj->context);

        result = CY_RSLT_SUCCESS;
    }

    return result;
}


cy_rslt_t cyhal_usb_dev_endpoint_read_result(cyhal_usb_dev_t *obj, cyhal_usb_dev_ep_t endpoint, uint32_t *actSize)
{
    cy_rslt_t result = CYHAL_USB_DEV_RSLT_ERR;
    uint32_t  ep_num = CYHAL_USB_DEV_GET_EP_NUM(endpoint);

    /* Check if endpoint is read for a read result operation */
    if (CY_USB_DEV_EP_COMPLETED == Cy_USBFS_Dev_Drv_GetEndpointState(obj->base, ep_num, &(obj->context)))
    {
        cy_en_usbfs_dev_drv_status_t drvStatus;
        drvStatus = Cy_USBFS_Dev_Drv_ReadOutEndpoint(obj->base,
                                                     ep_num,
                                                     obj->rd_data[CYHAL_USB_DEV_GET_EP_IDX(endpoint)],
                                                     obj->rd_size[CYHAL_USB_DEV_GET_EP_IDX(endpoint)],
                                                     actSize,
                                                     &(obj->context));

        if (drvStatus == CY_USBFS_DEV_DRV_SUCCESS)
        {
            result = CY_RSLT_SUCCESS;
        }
    }

    return result;
}


cy_rslt_t cyhal_usb_dev_endpoint_write(cyhal_usb_dev_t *obj, cyhal_usb_dev_ep_t endpoint, uint8_t const *data, uint32_t size)
{
    cy_rslt_t result = CYHAL_USB_DEV_RSLT_ERR;
    uint32_t  ep_num = CYHAL_USB_DEV_GET_EP_NUM(endpoint);

    /* Check if endpoint is read for a write operation */
    cy_en_usb_dev_ep_state_t ep_state;
    ep_state = Cy_USBFS_Dev_Drv_GetEndpointState(obj->base, ep_num, &(obj->context));
    if ((CY_USB_DEV_EP_IDLE == ep_state) || (CY_USB_DEV_EP_COMPLETED == ep_state))
    {
        cy_en_usbfs_dev_drv_status_t drvStatus;
        drvStatus = Cy_USBFS_Dev_Drv_LoadInEndpoint(obj->base,
                                                    ep_num,
                                                    data,
                                                    size,
                                                    &(obj->context));

        if (drvStatus == CY_USBFS_DEV_DRV_SUCCESS)
        {
            result = CY_RSLT_SUCCESS;
        }
    }

    return result;
}


cy_rslt_t cyhal_usb_dev_endpoint_abort(cyhal_usb_dev_t *obj, cyhal_usb_dev_ep_t endpoint)
{
    cy_rslt_t result = CYHAL_USB_DEV_RSLT_ERR;
    uint32_t  ep_num = CYHAL_USB_DEV_GET_EP_NUM(endpoint);

    if (CY_USBFS_DEV_DRV_SUCCESS == Cy_USBFS_Dev_Drv_Abort(obj->base, ep_num, &(obj->context)))
    {
        result = CY_RSLT_SUCCESS;
    }

    return result;
}


void cyhal_usb_dev_irq_enable(cyhal_usb_dev_t *obj, bool enable)
{
    uint32_t instance = (uint32_t)obj->resource.block_num;
    if (enable)
    {
        NVIC_ClearPendingIRQ(CYHAL_USBDEV_IRQ_N[instance]);
        NVIC_EnableIRQ(CYHAL_USBDEV_IRQ_N[instance]);
    }
    else
    {
        NVIC_DisableIRQ(CYHAL_USBDEV_IRQ_N[instance]);
    }
}


void cyhal_usb_dev_process_irq(cyhal_usb_dev_t *obj)
{
    Cy_USBFS_Dev_Drv_Interrupt(obj->base,
                               Cy_USBFS_Dev_Drv_GetInterruptCauseLo(obj->base),
                               &(obj->context));
}


cy_rslt_t cyhal_usb_dev_register_irq_callback(cyhal_usb_dev_t *obj, cyhal_usb_dev_irq_callback_t callback)
{
    cy_rslt_t result = CYHAL_USB_DEV_RSLT_ERR;
    cy_stc_sysint_t config;
    uint32_t instance = (uint32_t)obj->resource.block_num;

    /* Configure NVIC parameters */
    config.intrPriority = CYHAL_USB_DEV_DEFAULT_IRQ_PRIORITY;
    config.intrSrc = CYHAL_USBDEV_IRQ_N[instance];

    /* Setup interrupt in NVIC to trigger the callback */
    if (CY_SYSINT_SUCCESS == Cy_SysInt_Init(&config, (cy_israddress)callback))
    {
        result = CY_RSLT_SUCCESS;
    }

    return result;
}


void cyhal_usb_dev_register_event_callback(cyhal_usb_dev_t *obj, cyhal_usb_dev_event_t event, cyhal_usb_dev_event_callback_t callback)
{
    CY_ASSERT_L1(CYHAL_USB_DEV_IS_EVENT_VALID(event));
    uint32_t instance = (uint32_t)obj->resource.block_num;

    cyhal_usb_dev_event_callback_table[instance][event] = callback;
}


void cyhal_usb_dev_register_sof_callback( cyhal_usb_dev_t *obj, cyhal_usb_dev_sof_callback_t callback)
{
    uint32_t instance = (uint32_t)obj->resource.block_num;
    cyhal_usb_dev_sof_user_callback[instance] = callback;
}


void cyhal_usb_dev_register_endpoint_callback(cyhal_usb_dev_t *obj, cyhal_usb_dev_ep_t endpoint, cyhal_usb_dev_endpoint_callback_t callback)
{
    uint32_t endpoint_num = CYHAL_USB_DEV_GET_EP_NUM(endpoint);
    uint32_t instance = (uint32_t)obj->resource.block_num;

    CY_ASSERT_L1(CYHAL_USB_DEV_IS_EP_NUM_VALID(endpoint_num));

    cyhal_usb_dev_ep_handler_table[instance][CYHAL_USB_DEV_GET_EP_IDX(endpoint_num)]= callback;
}

#if defined(__cplusplus)
}
#endif

#endif /* CY_IP_MXUSBFS) */
