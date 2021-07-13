/*******************************************************************************
* File Name: cyhal_sdhc.c
*
* Description:
* Provides a high level interface for interacting with the Cypress SDHC. This
* is a wrapper around the lower level PDL API.
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

/**
* \addtogroup group_hal_impl_sdhc SDHC (SD Host Controller)
* \ingroup group_hal_impl
* \{
*
* The SHDC HAL implemenation for PSoC 6 provides implementations for the following weak functions
* specified by the PDL to make their usage in SDHC HAL driver more flexible by providing user ability
* to use card detect, write protect, pwr en, and io select signals on custom pins instead of dedicated
* SDHC block pins
*   - Cy_SD_Host_IsCardConnected
*   - Cy_SD_Host_IsWpSet
*   - Cy_SD_Host_EnableCardVoltage
*   - Cy_SD_Host_DisableCardVoltage
*   - Cy_SD_Host_ChangeIoVoltage
* In order to disable these implementations, the define CYHAL_DISABLE_WEAK_FUNC_IMPL
* (DEFINES+=CYHAL_DISABLE_WEAK_FUNC_IMPL) must be set when building the application.
*
* \} group_hal_impl_sdhc
*/

#ifdef CY_IP_MXSDHC

#if defined(__cplusplus)
extern "C"
{
#endif

#define _CYHAL_SDHC_IRQ_PRIORITY                        (3UL)
#define _CYHAL_SDHC_RW_RETRY_CYCLES                     (1000u)     /* Number of cycles for read/write operation complete */
#define _CYHAL_SDHC_RETRY_TIMES                         (1000UL)    /* The number loops to make the timeout in msec */
#define _CYHAL_SDHC_FUJE_TIMEOUT_MS                     (1000U)     /* The Fuje timeout for one block */
#define _CYHAL_SDHC_RW_TIMEOUT_US                       (500U)      /* The SDHC Read/Write timeout for one block */
#define _CYHAL_SDHC_TRANSFER_TIMEOUT                    (0xCUL)     /* The transfer timeout */
#define _CYHAL_SDHC_EMMC_TRIM_DELAY_MS                  (100U)      /* The EMMC TRIM timeout */
#define _CYHAL_SDIO_ENUMERATION_TIMEOUT_MS              (500U)
#define _CYHAL_SDHC_READ_TIMEOUT_MS                     (100U)      /* The Read timeout for one block. */
#define _CYHAL_SDHC_ALL_ERR_INTERRUPTS                  (CYHAL_SDHC_CMD_TOUT_ERR | CYHAL_SDHC_CMD_CRC_ERR |\
                                                         CYHAL_SDHC_CMD_END_BIT_ERR | CYHAL_SDHC_CMD_IDX_ERR |\
                                                         CYHAL_SDHC_DATA_TOUT_ERR | CYHAL_SDHC_DATA_CRC_ERR |\
                                                         CYHAL_SDHC_DATA_END_BIT_ERR | CYHAL_SDHC_CUR_LMT_ERR |\
                                                         CYHAL_SDHC_AUTO_CMD_ERR | CYHAL_SDHC_ADMA_ERR |\
                                                         CYHAL_SDHC_TUNNING_ERR | CYHAL_SDHC_RESP_ERR |\
                                                         CYHAL_SDHC_BOOT_ACK_ERR)
#define _CYHAL_SDHC_RCA_SHIFT                           (16U)
#define _CYHAL_SDHC_1_8_REG_STABLE_TIME_MS              (30U)       /* The 1.8 voltage regulator stable time. */
#define _CYHAL_SDHC_PWR_RAMP_UP_TIME_MS                 (36U)       /* Time needed for card VDD to achieve operating
                                                                    * supply range level after power on (power on time,
                                                                    * 1 ms + pwr ramp up, max 35 ms) */
#define _CYHAL_SDHC_TOUT_TMCLK_POW_MIN                  (13U)        /* Minimal power of 2 for data timeout counter value */
#define _CYHAL_SDHC_TOUT_TMCLK_POW_MAX                  (27U)        /* Maximal power of 2 for data timeout counter value */
#define _CYHAL_SDHC_EXPECTED_BASE_CLK_FREQ_HZ           (100000000U) /* By default SDHC block is expected to be clocked by 100 MHz */

#define _CYHAL_SDIO_RW_TIMEOUT_US                       (5U)        /* The SDIO Read/Write timeout for one block */
#define _CYHAL_SDIO_CMD_CMPLT_DELAY_US                  (5U)        /* The Command complete delay */
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

static void * stored_objects[CY_IP_MXSDHC_INSTANCES];

typedef enum
{
    _CYHAL_SDHC_NOT_RUNNING = 0x0,
    _CYHAL_SDHC_WAIT_CMD_COMPLETE = 0x1,
    _CYHAL_SDHC_WAIT_XFER_COMPLETE = 0x2,
    _CYHAL_SDHC_WAIT_BOTH = _CYHAL_SDHC_WAIT_CMD_COMPLETE | _CYHAL_SDHC_WAIT_XFER_COMPLETE
} _cyhal_sdhc_transfer_status_t;

/*  cyhal_sdhc_t object is better place for storing this data,
*   but cyhal_sdhc_write and cyhal_sdhc_read takes
*   instance as pointer to const, so there is no way to
*   update object fields inside those functions. */
static _cyhal_sdhc_transfer_status_t _cyhal_sdhc_data_transfer_status[CY_IP_MXSDHC_INSTANCES];

#if defined(CY_RTOS_AWARE) || defined(COMPONENT_RTOS_AWARE)
#include "cyabs_rtos.h"

typedef enum
{
    /* Semaphore is not initialized */
    _CYHAL_SDXX_SEMA_NOT_INITED,
    /* Semaphore is initialized, but will not be used */
    _CYHAL_SDXX_SEMA_NOT_USED,
    /* Semaphore is initialized and used (expected to be set in IRQ handler) */
    _CYHAL_SDXX_SEMA_USED,
    /* Set in irq handler */
    _CYHAL_SDXX_SEMA_SET
} _cyhal_sdxx_semaphore_status_t;

/* cyhal_sdhc_t and cyhal_sdio_t are the better places for keeping
*  these items, but cyhal_sdhc_write and cyhal_sdhc_read takes
*  instance as pointer to const, so there is no way to use
*  semaphore and initizalization flag as instance object fields
*  until we don't change interface in scope of major version
*  update. */
static cy_semaphore_t _cyhal_sdhc_smphr_xfer_done[CY_IP_MXSDHC_INSTANCES];
static _cyhal_sdxx_semaphore_status_t _cyhal_sdxx_sema_status[CY_IP_MXSDHC_INSTANCES];

static inline bool _cyhal_sdxx_is_smfr_ready_for_set(uint8_t block)
{
    return (_CYHAL_SDXX_SEMA_USED == _cyhal_sdxx_sema_status[block]);
}

static inline bool _cyhal_sdxx_is_smfr_ready_for_get(uint8_t block)
{
    return (_CYHAL_SDXX_SEMA_USED == _cyhal_sdxx_sema_status[block]) ||
                (_CYHAL_SDXX_SEMA_SET == _cyhal_sdxx_sema_status[block]);
}

#endif /* CY_RTOS_AWARE or COMPONENT_RTOS_AWARE defined */

static cy_rslt_t _cyhal_sdxx_prepare_for_transfer(SDHC_Type *base, bool is_sdio)
{
    CY_ASSERT(base != NULL);
    uint8_t block_num = (base == SDHC0) ? 0 : 1;

    uint32_t activated_cmd_complete = 0;
    if (((is_sdio) && (((cyhal_sdio_t *)stored_objects[block_num])->irq_cause & CYHAL_SDIO_CMD_COMPLETE)) ||
        ((!is_sdio) && (((cyhal_sdhc_t *)stored_objects[block_num])->irq_cause & CYHAL_SDHC_CMD_COMPLETE)))
    {
        /* Activate CY_SD_HOST_CMD_COMPLETE interrupt mask only if user enabled callback for that event */
        activated_cmd_complete = CY_SD_HOST_CMD_COMPLETE;
    }

    /* Enabling transfer complete interrupt as it takes part in in write / read processes */
    Cy_SD_Host_SetNormalInterruptMask(base, Cy_SD_Host_GetNormalInterruptMask(base) | activated_cmd_complete |
            CY_SD_HOST_XFER_COMPLETE);

    return CY_RSLT_SUCCESS;
}

typedef enum
{
    _CYHAL_SDHC_CARD_VDD                    = 0,
    _CYHAL_SDHC_CARD_IO_VOLTAGE             = 1,
    _CYHAL_SDHC_CARD_DETECT                 = 2,
    _CYHAL_SDHC_CARD_MECH_WRITE_PROTECT     = 3,
    _CYHAL_SDHC_NOT_WEAK_FUNC               = 4
} _cyhal_sdhc_weak_func_type;

/* This callback is used when card detect pin is GPIO (not dedicated SDHC block signal) */
static void _cyhal_sdhc_gpio_card_detect_callback(void *callback_arg, cyhal_gpio_event_t event)
{
    CY_ASSERT(NULL != callback_arg);
    cyhal_sdhc_t * obj = (cyhal_sdhc_t *)callback_arg;

    if (obj->callback_data.callback != NULL)
    {
        cyhal_sdhc_event_callback_t callback = (cyhal_sdhc_event_callback_t) obj->callback_data.callback;

        /* Card is considered as inserted if card_detect pin in low state. */

        /* Card removal event */
        if ((event & CYHAL_GPIO_IRQ_RISE) && (obj->irq_cause & CYHAL_SDHC_CARD_REMOVAL))
        {
            /* Call registered callbacks here */
            (callback) (obj->callback_data.callback_arg, CYHAL_SDHC_CARD_REMOVAL);
        }
        /* Card insertion event */
        if ((event & CYHAL_GPIO_IRQ_FALL) && (obj->irq_cause & CYHAL_SDHC_CARD_INSERTION))
        {
            /* Call registered callbacks here */
            (callback) (obj->callback_data.callback_arg, CYHAL_SDHC_CARD_INSERTION);
        }
    }
}

#if !defined(CYHAL_DISABLE_WEAK_FUNC_IMPL)

/*  Function, that handles pins that stands behind SD Host PDL driver pin-related WEAK functions, such as:
*   - Cy_SD_Host_IsCardConnected
*   - Cy_SD_Host_IsWpSet
*   - Cy_SD_Host_EnableCardVoltage
*   - Cy_SD_Host_DisableCardVoltage
*   - Cy_SD_Host_ChangeIoVoltage
*   To make their usage in SDHC HAL driver more flexible by providing user ability to use card detect,
*   write protect, pwr en, and io select signals on pins they like instead of dedicated SDHC block pins */
static bool _cyhal_sdhc_handle_weak_func(const SDHC_Type *base, _cyhal_sdhc_weak_func_type weak_function, bool enable)
{
    uint8_t block_idx = (SDHC0 == base) ? 0 : 1;
    cyhal_sdhc_t * obj = (cyhal_sdhc_t *)stored_objects[block_idx];
    CY_ASSERT(obj != NULL);
    CY_ASSERT((weak_function == _CYHAL_SDHC_CARD_VDD) || (weak_function == _CYHAL_SDHC_CARD_IO_VOLTAGE) ||
            (weak_function == _CYHAL_SDHC_CARD_DETECT) || (weak_function == _CYHAL_SDHC_CARD_MECH_WRITE_PROTECT));
    /* function index */
    uint8_t f_idx = (uint8_t)weak_function;

    /* Indexes according to _cyhal_sdhc_weak_func_type */
    static const cyhal_resource_pin_mapping_t* pin_mappings[] = {
            cyhal_pin_map_sdhc_card_if_pwr_en, cyhal_pin_map_sdhc_io_volt_sel, cyhal_pin_map_sdhc_card_detect_n,
            cyhal_pin_map_sdhc_card_mech_write_prot
        };
    static const size_t pin_mapping_sizes[] = {
            _CYHAL_SDHC_ELEM_COUNT(cyhal_pin_map_sdhc_card_if_pwr_en), _CYHAL_SDHC_ELEM_COUNT(cyhal_pin_map_sdhc_io_volt_sel),
            _CYHAL_SDHC_ELEM_COUNT(cyhal_pin_map_sdhc_card_detect_n), _CYHAL_SDHC_ELEM_COUNT(cyhal_pin_map_sdhc_card_mech_write_prot)
        };
    cyhal_gpio_t pins[] = {
        obj->pin_card_pwr_en, obj->pin_io_vol_sel, obj->pin_card_detect, obj->pin_card_mech_write_prot
        };

    /*  Per sd_host PDL documentation (documentation for Cy_SD_Host_ChangeIoVoltage),
    *   SIGNALING_EN bit of the SDHC_CORE_HOST_CTRL2_R register must be set even if
    *   GPIO used for io voltage switching. */
    if (_CYHAL_SDHC_CARD_IO_VOLTAGE == weak_function)
    {
        SDHC_CORE_HOST_CTRL2_R(base) = _CLR_SET_FLD16U(SDHC_CORE_HOST_CTRL2_R(base),
                SDHC_CORE_HOST_CTRL2_R_SIGNALING_EN, enable);
        /* enable (true) stands for 1.8V while false for 3.3V */
        obj->low_voltage_io_set = enable;
    }

    /* Pin is not provided by user */
    if (NC == pins[f_idx])
    {
        /* Return true for card detect, false for write protect and false for other (dont care) */
        return (weak_function == _CYHAL_SDHC_CARD_DETECT) ? true : false;
    }
    /* Pin is GPIO */
    else if (NULL == _cyhal_utils_get_resource(pins[f_idx], pin_mappings[f_idx], pin_mapping_sizes[f_idx], NULL))
    {
        if (weak_function == _CYHAL_SDHC_CARD_DETECT)
        {
            /* Card is inserted if signal is low */
            return !cyhal_gpio_read(pins[f_idx]);
        }
        else if (weak_function == _CYHAL_SDHC_CARD_MECH_WRITE_PROTECT)
        {
            /* Card is mech. write protected if signal is high */
            return cyhal_gpio_read(pins[f_idx]);
        }
        /* _CYHAL_SDHC_CARD_VDD or _CYHAL_SDHC_CARD_IO_VOLTAGE */
        else
        {
            cyhal_gpio_write(pins[f_idx], enable);
            /* Don't care */
            return true;
        }
    }
    /* Pin is dedicated SDHC block signal */
    else
    {
        /* Actually copies of corresponding functions from cy_sd_host.c */
        switch(weak_function)
        {
            case _CYHAL_SDHC_CARD_VDD:
                SDHC_CORE_PWR_CTRL_R(base) =
                        _CLR_SET_FLD8U(SDHC_CORE_PWR_CTRL_R(base), SDHC_CORE_PWR_CTRL_R_SD_BUS_PWR_VDD1, enable);
                break;
            case _CYHAL_SDHC_CARD_DETECT:
                while(true != _FLD2BOOL(SDHC_CORE_PSTATE_REG_CARD_STABLE, SDHC_CORE_PSTATE_REG(base)))
                {
                    /* Wait until the card is stable. */
                }
                return _FLD2BOOL(SDHC_CORE_PSTATE_REG_CARD_INSERTED, SDHC_CORE_PSTATE_REG(base));
            case _CYHAL_SDHC_CARD_MECH_WRITE_PROTECT:
                return _FLD2BOOL(SDHC_CORE_PSTATE_REG_WR_PROTECT_SW_LVL, SDHC_CORE_PSTATE_REG(base));
            default:
            case _CYHAL_SDHC_CARD_IO_VOLTAGE:
                /* io voltage already handled above */
                (void)0;
                break;
        }
        /* dont care */
        return true;
    }
}

/* Overriden Cy_SD_Host_IsCardConnected (originally part of cy_sd_host.c)
*  This version can handle both - dedicated IP block sdhc_card_detect_n pin and
*  user-provided GPIO */
bool Cy_SD_Host_IsCardConnected(SDHC_Type const *base)
{
    return _cyhal_sdhc_handle_weak_func(base, _CYHAL_SDHC_CARD_DETECT, false);
}

/* Overriden Cy_SD_Host_IsWpSet function (originally part of cy_sd_host.c).
*  This version can handle both - dedicated IP block sdhc_card_mech_write_prot pin
*  and user-provided GPIO */
bool Cy_SD_Host_IsWpSet(SDHC_Type const *base)
{
    return _cyhal_sdhc_handle_weak_func(base, _CYHAL_SDHC_CARD_MECH_WRITE_PROTECT, false);
}

/* Overriden Cy_SD_Host_EnableCardVoltage and Cy_SD_Host_DisableCardVoltage
*  functions (originally part of in cy_sd_host.c).
*  This version can handle both - dedicated IP block sdhc_card_if_pwr_en pin and
*  user-provided GPIO */
void Cy_SD_Host_EnableCardVoltage(SDHC_Type *base)
{
    (void)_cyhal_sdhc_handle_weak_func(base, _CYHAL_SDHC_CARD_VDD, true);
}
void Cy_SD_Host_DisableCardVoltage(SDHC_Type *base)
{
    (void)_cyhal_sdhc_handle_weak_func(base, _CYHAL_SDHC_CARD_VDD, false);
}

/* Overriden Cy_SD_Host_ChangeIoVoltage function (originally part of cy_sd_host.c).
*  This version can handle both - dedicated IP block sdhc_io_volt_sel pin
*  and user-provided GPIO */
void Cy_SD_Host_ChangeIoVoltage(SDHC_Type *base, cy_en_sd_host_io_voltage_t ioVoltage)
{
    (void)_cyhal_sdhc_handle_weak_func(base, _CYHAL_SDHC_CARD_IO_VOLTAGE, (CY_SD_HOST_IO_VOLT_1_8V == ioVoltage));
}

#endif /* CYHAL_DISABLE_WEAK_FUNC_IMPL */

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
    /*  Placeholder of removed CYHAL_SDHC_CARD_INTERRUPT.
    *   It is needed for cyhal_sdhc_enable_event and _cyhal_utils_convert_flags
    *   functions correct work. */
    0,
    (uint32_t)CYHAL_SDHC_FX_EVENT,        // CY_SD_HOST_FX_EVENT
    (uint32_t)CYHAL_SDHC_CQE_EVENT,       // CY_SD_HOST_CQE_EVENT
    (uint32_t)CYHAL_SDHC_ERR_INTERRUPT,   // CY_SD_HOST_ERR_INTERRUPT
};

static cy_rslt_t _cyhal_sdxx_setup_pin(void *sdhc_instance, cyhal_gpio_t pin, const cyhal_resource_pin_mapping_t *pinmap, size_t count,
    cyhal_gpio_t *objRef, _cyhal_sdhc_weak_func_type weak_func_pin_type)
{
    cy_rslt_t result;
    const cyhal_resource_pin_mapping_t *map = _cyhal_utils_get_resource(pin, pinmap, count, NULL);

    if (map == NULL)
    {
        result = CYHAL_SDHC_RSLT_ERR_PIN;
        if (_CYHAL_SDHC_NOT_WEAK_FUNC != weak_func_pin_type)
        {
            /* pin, provided by user is probably not dedicated SDHC signal, but GPIO */
            switch(weak_func_pin_type)
            {
                case _CYHAL_SDHC_CARD_DETECT:
                case _CYHAL_SDHC_CARD_MECH_WRITE_PROTECT:
                    result = cyhal_gpio_init(pin, CYHAL_GPIO_DIR_INPUT, CYHAL_GPIO_DRIVE_NONE, true);
                    if ((_CYHAL_SDHC_CARD_DETECT == weak_func_pin_type) && (CY_RSLT_SUCCESS == result))
                    {
                        cyhal_gpio_register_callback(pin, _cyhal_sdhc_gpio_card_detect_callback, sdhc_instance);
                        cyhal_gpio_enable_event(pin, CYHAL_GPIO_IRQ_BOTH, CYHAL_ISR_PRIORITY_DEFAULT, true);
                        ((cyhal_sdhc_t *)sdhc_instance)->cd_gpio_cb_enabled = true;
                    }
                    break;
                case _CYHAL_SDHC_CARD_VDD:
                case _CYHAL_SDHC_CARD_IO_VOLTAGE:
                    result = cyhal_gpio_init(pin, CYHAL_GPIO_DIR_OUTPUT, CYHAL_GPIO_DRIVE_STRONG, false);
                    break;
                default:
                    CY_ASSERT(false); /* Should never get here. */
            }
        }
    }
    else
    {
        result = _cyhal_utils_reserve_and_connect(pin, map);
    }

    if (result == CY_RSLT_SUCCESS)
    {
        *objRef = pin;
    }

    return result;
}

/*******************************************************************************
*       Deep Sleep Callback Service Routine
*******************************************************************************/
static bool _cyhal_sdio_syspm_callback(cyhal_syspm_callback_state_t state, cyhal_syspm_callback_mode_t mode, void *callback_arg)
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

static bool _cyhal_sdhc_syspm_callback(cyhal_syspm_callback_state_t state, cyhal_syspm_callback_mode_t mode, void *callback_arg)
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

static cy_en_sd_host_bus_width_t _cyhal_sdhc_convert_buswidth(uint8_t sd_data_bits)
{
    switch (sd_data_bits)
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

/***********************************************************************************************************************
*
* Finds SDHC internal divider value according to source clock frequency and desired frequency.
*
* hz_src                - Source clock frequency, that needs to be divided
* desired_hz            - Desired clock frequency
* tolerance             - (Not used)
* only_below_desired    - (Not used)
* div                   - Calculated divider value will be placed by this pointer
*
***********************************************************************************************************************/
static cy_rslt_t _cyhal_sdxx_find_best_div(uint32_t hz_src, uint32_t desired_hz,
            const cyhal_clock_tolerance_t *tolerance, bool only_below_desired, uint32_t *div)
{
    CY_UNUSED_PARAMETER(tolerance);
    CY_UNUSED_PARAMETER(only_below_desired);

    /* Rounding up for correcting the error in integer division
    * to ensure the actual frequency is less than or equal to
    * the requested frequency.
    * Ensure computed divider is no more than 10-bit.
    */

    if (hz_src > desired_hz)
    {
        uint32_t freq = (desired_hz << 1);
        uint32_t calculated_divider = ((hz_src + freq - 1) / freq) & 0x3FF;
        /* Real divider is 2 x calculated_divider */
        *div = calculated_divider << 1;
    }
    else
    {
        *div = 1;
    }

    return CY_RSLT_SUCCESS;
}

/***********************************************************************************************************************
*
* Changes the Host controller SD clock.
*
* obj                   - Void pointer to cyhal_sdhc_t or cyhal_sdio_t instance
* is_sdio               - Function is called to switch SDIO clock if true, and SDHC if false
* frequency             - The frequency in Hz (pointer). This variable will be updated with actual frequency achieved
*   upon function return.
* lowVoltageSignaling   - Support 1.8V
* negotiate             - Whether new frequency value needs to be negotiated with the card
*
***********************************************************************************************************************/
static cy_rslt_t _cyhal_sdxx_sdcardchangeclock(void *obj, bool is_sdio, uint32_t *frequency, bool lowVoltageSignaling,
            bool negotiate)
{
    CY_ASSERT(NULL != obj);
    CY_ASSERT(NULL != frequency);

    SDHC_Type                       *base;
    cyhal_clock_t                   *clock;
    cy_stc_sd_host_context_t        *context;
    cy_en_sd_host_bus_speed_mode_t  busSpeed;

    if (is_sdio)
    {
        base = ((cyhal_sdio_t *)obj)->base;
        clock = &(((cyhal_sdio_t *)obj)->clock);
        context = &(((cyhal_sdio_t *)obj)->context);
    }
    else
    {
        base = ((cyhal_sdhc_t *)obj)->base;
        clock = &(((cyhal_sdhc_t *)obj)->clock);
        context = &(((cyhal_sdhc_t *)obj)->context);
    }

    CY_ASSERT(NULL != base);

    cyhal_clock_t most_suitable_hf_source;
    uint32_t most_suitable_div = 0;
    uint32_t actualFreq = 0;
    uint32_t hf_source_freq = 0;

    /*  Find most suitable HF clock source and divider for it to achieve closest frequency to desired.
    *   No clock settings are being changed here. */
    cy_rslt_t ret = _cyhal_utils_find_hf_source_n_divider(clock, *frequency, NULL, _cyhal_sdxx_find_best_div,
                                &most_suitable_hf_source, &most_suitable_div);
    if (CY_RSLT_SUCCESS == ret)
    {
        hf_source_freq = cyhal_clock_get_frequency(&most_suitable_hf_source);
        actualFreq = hf_source_freq / most_suitable_div;
        if (actualFreq <= CY_SD_HOST_CLK_25M)
        {
            busSpeed = (lowVoltageSignaling)
                ? CY_SD_HOST_BUS_SPEED_SDR12_5
                : CY_SD_HOST_BUS_SPEED_DEFAULT;
        }
        else if (actualFreq <= CY_SD_HOST_CLK_50M)
        {
            busSpeed = (lowVoltageSignaling)
                ? CY_SD_HOST_BUS_SPEED_SDR25
                : CY_SD_HOST_BUS_SPEED_HIGHSPEED;
        }
        else
        {
            busSpeed = (lowVoltageSignaling)
                ? CY_SD_HOST_BUS_SPEED_SDR50
                : CY_SD_HOST_BUS_SPEED_HIGHSPEED;
        }

        /*  Currently can only negotiate with SD Card. SDIO negotiation proccess is tracked in BSP-2643. */
        if (negotiate && !is_sdio)
        {
            ret = (cy_rslt_t)Cy_SD_Host_SetBusSpeedMode(base, busSpeed, context);

            /* Delay to be removed after DRIVERS-5140 resolvation */
            if (CY_RSLT_SUCCESS == ret)
            {
                cyhal_system_delay_ms(_CYHAL_SDHC_READ_TIMEOUT_MS);
            }
        }
        else
        {
            ret = (cy_rslt_t)Cy_SD_Host_SetHostSpeedMode(base, busSpeed);
        }
    }

    if (CY_RSLT_SUCCESS == ret)
    {
        /* Switch to most suitable HF clock source */
        ret = cyhal_clock_set_source(clock, &most_suitable_hf_source);
    }

    if (CY_RSLT_SUCCESS == ret)
    {
        if (!is_sdio)
        {
            ((cyhal_sdhc_t *)obj)->block_source_freq_hz = hf_source_freq;
        }
        /* SDHC internal divider will be configured instead. */
        ret = cyhal_clock_set_divider(clock, 1);
    }

    if (CY_RSLT_SUCCESS == ret)
    {
        Cy_SD_Host_DisableSdClk(base);
        ret = (cy_rslt_t)Cy_SD_Host_SetSdClkDiv(base, most_suitable_div >> 1);
        Cy_SD_Host_EnableSdClk(base);
    }

    if (CY_RSLT_SUCCESS == ret)
    {
        *frequency = actualFreq;
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
static cy_en_sd_host_status_t _cyhal_sdxx_pollcmdcomplete(SDHC_Type *base, uint8_t block)
{
    cy_en_sd_host_status_t ret = CY_SD_HOST_ERROR_TIMEOUT;
    uint32_t retry  = _CYHAL_SDHC_RETRY_TIMES;

    while (retry > 0UL)
    {
        /* Command complete */
        if (CY_SD_HOST_CMD_COMPLETE == (CY_SD_HOST_CMD_COMPLETE & Cy_SD_Host_GetNormalInterruptStatus(base)))
        {
            _cyhal_sdhc_data_transfer_status[block] &= ~_CYHAL_SDHC_WAIT_CMD_COMPLETE;

            /* Clear interrupt flag */
            Cy_SD_Host_ClearNormalInterruptStatus(base, CY_SD_HOST_CMD_COMPLETE);

            ret = CY_SD_HOST_SUCCESS;
            break;
        }

        cyhal_system_delay_us(_CYHAL_SDIO_CMD_CMPLT_DELAY_US);
        retry--;
    }

    return ret;
}


/*******************************************************************************
*
* Waits for the transfer complete event.
*
* base  - The SD host registers structure pointer.
* delay - The delay timeout for one block transfer.
*
* return If the base pointer is NULL, returns error.
*
*******************************************************************************/
static cy_en_sd_host_status_t _cyhal_sdxx_polltransfercomplete(SDHC_Type *base, const uint16_t delay, bool is_sdio)
{
    cy_en_sd_host_status_t ret = CY_SD_HOST_ERROR_TIMEOUT;
    uint32_t               retry = _CYHAL_SDHC_RW_RETRY_CYCLES;
    uint8_t                block_idx = (SDHC0 == base) ? 0 : 1;
    uint32_t               status = 0UL;

    while ((CY_SD_HOST_ERROR_TIMEOUT == ret) && (retry-- > 0U))
    {
        /* We check for either the interrupt register or the byte set in the _cyhal_sdxx_irq_handler
         * to avoid a deadlock in the case where if an API that is polling is called from an ISR
         * and its priority is higher than the priority of the _cyhal_sdxx_irq_handler thus not allowing
         * the signalling byte to be set.
         */
        status = Cy_SD_Host_GetNormalInterruptStatus(base);
        if (CY_SD_HOST_XFER_COMPLETE == (CY_SD_HOST_XFER_COMPLETE & status) ||
            ((is_sdio) && (!cyhal_sdio_is_busy((cyhal_sdio_t *)stored_objects[block_idx]))) ||
            ((!is_sdio) && (!cyhal_sdhc_is_busy((cyhal_sdhc_t *)stored_objects[block_idx]))))
        {
            /* Transfer complete */
            ret = CY_SD_HOST_SUCCESS;
            break;
        }
        cyhal_system_delay_us(delay);
    }

    return ret;
}

// Indicates that a sync transfer is in process
static void _cyhal_sdxx_setup_smphr(uint32_t block_num)
{
#if defined(CY_RTOS_AWARE) || defined(COMPONENT_RTOS_AWARE)
    bool in_isr = (SCB->ICSR & SCB_ICSR_VECTACTIVE_Msk) != 0;
    if (!in_isr)
    {
        cy_rslt_t ret = CY_RSLT_SUCCESS;
        if (_CYHAL_SDXX_SEMA_NOT_INITED == _cyhal_sdxx_sema_status[block_num])
        {
            /* Semaphore is used to make transfer complete event wait process be more RTOS-friendly.
            *  It cannot be initialized in ISR context (some of mbed layers are initializing sdio in ISR conext),
            *  so we have to do it in scope of transfer functions. */
            /* Assert that we are not in an ISR */
            CY_ASSERT((SCB->ICSR & SCB_ICSR_VECTACTIVE_Msk) == 0);
            ret = cy_rtos_init_semaphore(&_cyhal_sdhc_smphr_xfer_done[block_num], 1, 0);
        }
        else if (_CYHAL_SDXX_SEMA_SET == _cyhal_sdxx_sema_status[block_num])
        {
            /*  Situation, when semaphore was set, but _cyhal_sdxx_waitfor_transfer_complete function was not
            *   used for the transfer to get the semaphore (async transfer). Clearing the semaphore in order
            *   to prepare it for another transfer.  */
            ret = cy_rtos_get_semaphore(&_cyhal_sdhc_smphr_xfer_done[block_num], 500, in_isr);
        }
        if (CY_RSLT_SUCCESS == ret)
        {
            _cyhal_sdxx_sema_status[block_num] = _CYHAL_SDXX_SEMA_USED;
        }
    }
    else if (_CYHAL_SDXX_SEMA_NOT_INITED != _cyhal_sdxx_sema_status[block_num])
    {
        _cyhal_sdxx_sema_status[block_num] = _CYHAL_SDXX_SEMA_NOT_USED;
    }
#else
    // We don't need to do anything special if we're not in an RTOS context
    CY_UNUSED_PARAMETER(block_num);
#endif
}

static cy_rslt_t _cyhal_sdxx_waitfor_transfer_complete(uint32_t block_num, SDHC_Type *base, bool is_sdio)
{
    /* When using an RTOS if a sdxx api (read or write) is called from and ISR in
     * in certain RTOSes cy_rtos_get_semaphore returns immediately without blocking. So we can
     * either busy wait around the semaphore being set in the ISR or use the normal polling method
     * we use in the non-RTOS case. For simplicity and to avoid the calling ISR from depending on
     * the SDXX ISR priority we use the normal polling method.
     */
    cy_rslt_t ret = CY_RSLT_SUCCESS;
    #if defined(CY_RTOS_AWARE) || defined(COMPONENT_RTOS_AWARE)
    bool in_isr = (SCB->ICSR & SCB_ICSR_VECTACTIVE_Msk) != 0;
    if ((!in_isr) && _cyhal_sdxx_is_smfr_ready_for_get(block_num))
    {
        ret = cy_rtos_get_semaphore(&_cyhal_sdhc_smphr_xfer_done[block_num], 500, in_isr);
        if (CY_RSLT_SUCCESS == ret)
        {
            _cyhal_sdxx_sema_status[block_num] = _CYHAL_SDXX_SEMA_NOT_USED;
        }
    }
    else
    {
    #endif
        CY_UNUSED_PARAMETER(block_num);
        ret = (cy_rslt_t)_cyhal_sdxx_polltransfercomplete(base, _CYHAL_SDHC_RW_TIMEOUT_US, is_sdio);
    #if defined(CY_RTOS_AWARE) || defined(COMPONENT_RTOS_AWARE)
    }
    #endif

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

    /*  Some parts of SDHost PDL and SDHC HAL drivers are sending SD commands and polling interrupt status
    *   until CY_SD_HOST_CMD_COMPLETE occurs. Thats why we can't clear CY_SD_HOST_CMD_COMPLETE interrupt status
    *   and code below prevents _cyhal_sdhc_irq_handler from being continuosly called because of uncleared
    *   CY_SD_HOST_CMD_COMPLETE event. */

    if (interruptStatus & Cy_SD_Host_GetNormalInterruptMask(blockAddr) & CY_SD_HOST_CMD_COMPLETE)
    {
        /* Disabling command complete interrupt mask */
        Cy_SD_Host_SetNormalInterruptMask(obj->base,
                Cy_SD_Host_GetNormalInterruptMask(obj->base) & (uint32_t) ~CY_SD_HOST_CMD_COMPLETE);
        _cyhal_sdhc_data_transfer_status[block] &= ~_CYHAL_SDHC_WAIT_CMD_COMPLETE;
    }

    /*  During SDHost PDL driver operation, CY_SD_HOST_XFER_COMPLETE status can occur and driver
    *   is polling Cy_SD_Host_GetNormalInterruptStatus while waiting for it. Because of this
    *   it is critical to have CY_SD_HOST_XFER_COMPLETE event mask disabled and only enabled during
    *   transfers. Write / Read SDHC/SDIO HAL functions are taking care of enabling CY_SD_HOST_XFER_COMPLETE
    *   mask before transfer while code below disables it. */

    /* CY_SD_HOST_XFER_COMPLETE occured and appropriate bit in interrupt mask is enabled */
    if (interruptStatus & Cy_SD_Host_GetNormalInterruptMask(blockAddr) & CY_SD_HOST_XFER_COMPLETE)
    {
        /* Clearing transfer complete status */
        Cy_SD_Host_ClearNormalInterruptStatus(blockAddr, CY_SD_HOST_XFER_COMPLETE);

        _cyhal_sdhc_data_transfer_status[block] &= ~_CYHAL_SDHC_WAIT_XFER_COMPLETE;

        #if defined(CY_RTOS_AWARE) || defined(COMPONENT_RTOS_AWARE)
        if (_cyhal_sdxx_is_smfr_ready_for_set(block))
        {
            cy_rtos_set_semaphore(&_cyhal_sdhc_smphr_xfer_done[block], true);
            _cyhal_sdxx_sema_status[block] = _CYHAL_SDXX_SEMA_SET;
        }
        #endif /* CY_RTOS_AWARE or COMPONENT_RTOS_AWARE defined */

        /* Disabling transfer complete interrupt mask */
        Cy_SD_Host_SetNormalInterruptMask(obj->base,
                Cy_SD_Host_GetNormalInterruptMask(obj->base) & (uint32_t) ~CY_SD_HOST_XFER_COMPLETE);
    }

    if (obj->callback_data.callback != NULL)
    {
        cyhal_sdhc_event_callback_t callback = (cyhal_sdhc_event_callback_t) obj->callback_data.callback;

        /* Call registered callbacks here */
        (callback) (obj->callback_data.callback_arg, user_events);
    }

    /*  Cannot clear cmd complete interrupt, as it is being polling-waited by many SD Host functions.
    *   It is expected to be cleared by mentioned polling functions. */
    userInterruptStatus &= (uint32_t) ~CY_SD_HOST_CMD_COMPLETE;

    /* Clear only handled events */
    Cy_SD_Host_ClearNormalInterruptStatus(blockAddr, userInterruptStatus);
}

static void _cyhal_sdio_irq_handler(void)
{
    IRQn_Type irqn = _CYHAL_UTILS_GET_CURRENT_IRQN();
    uint8_t block = _cyhal_sdhc_get_block_from_irqn(irqn);
    SDHC_Type *blockAddr = _CYHAL_SDHC_BASE_ADDRESSES[block];
    cyhal_sdio_t *obj = (cyhal_sdio_t*) _cyhal_sdhc_config_structs[block];

    uint32_t interruptStatus = Cy_SD_Host_GetNormalInterruptStatus(blockAddr);
    uint32_t userInterruptStatus = interruptStatus & obj->irq_cause;
    uint32_t normalInterruptMask = Cy_SD_Host_GetNormalInterruptMask(blockAddr);

    /*  Some parts of SDHost PDL and SDIO HAL drivers are sending SD commands and polling interrupt status
    *   until CY_SD_HOST_CMD_COMPLETE occurs. Thats why we can't clear CY_SD_HOST_CMD_COMPLETE interrupt status
    *   and code below prevents _cyhal_sdhc_irq_handler from being continuosly called because of uncleared
    *   CY_SD_HOST_CMD_COMPLETE event. */

    if (interruptStatus & normalInterruptMask & CY_SD_HOST_CMD_COMPLETE)
    {
        /* Disabling command complete interrupt mask */
        Cy_SD_Host_SetNormalInterruptMask(obj->base,
                Cy_SD_Host_GetNormalInterruptMask(obj->base) & (uint32_t) ~CY_SD_HOST_CMD_COMPLETE);
        _cyhal_sdhc_data_transfer_status[block] &= ~_CYHAL_SDHC_WAIT_CMD_COMPLETE;
    }

    /*  During SDHost PDL driver operation, CY_SD_HOST_XFER_COMPLETE status can occur and driver
    *   is polling Cy_SD_Host_GetNormalInterruptStatus while waiting for it. Because of this
    *   it is critical to have CY_SD_HOST_XFER_COMPLETE event mask disabled and only enabled during
    *   transfers. Write / Read SDHC/SDIO HAL functions are taking care of enabling CY_SD_HOST_XFER_COMPLETE
    *   mask before transfer while code below disables it. */

    /*  CY_SD_HOST_XFER_COMPLETE occured and appropriate bit in interrupt mask is enabled */
    if (interruptStatus & normalInterruptMask & CY_SD_HOST_XFER_COMPLETE)
    {
        _cyhal_sdhc_data_transfer_status[block] &= ~_CYHAL_SDHC_WAIT_XFER_COMPLETE ;
        Cy_SD_Host_ClearNormalInterruptStatus(blockAddr, CY_SD_HOST_XFER_COMPLETE);
        #if defined(CY_RTOS_AWARE) || defined(COMPONENT_RTOS_AWARE)
        if (_cyhal_sdxx_is_smfr_ready_for_set(block))
        {
            cy_rtos_set_semaphore(&_cyhal_sdhc_smphr_xfer_done[block], true);
            _cyhal_sdxx_sema_status[block] = _CYHAL_SDXX_SEMA_SET;
        }
        #endif /* CY_RTOS_AWARE or COMPONENT_RTOS_AWARE defined */

        /* Disabling transfer complete interrupt mask */
        Cy_SD_Host_SetNormalInterruptMask(obj->base,
                Cy_SD_Host_GetNormalInterruptMask(obj->base) & (uint32_t) ~CY_SD_HOST_XFER_COMPLETE);

        /* Transfer is no more active. If card interrupt was not yet enabled after it was disabled in
        *   interrupt handler, enable it.
        */
        uint32_t interrupt_enable_status = Cy_SD_Host_GetNormalInterruptEnable(obj->base);
        if (((interrupt_enable_status & CY_SD_HOST_CARD_INTERRUPT) == 0) && (0U != (CY_SD_HOST_CARD_INTERRUPT & obj->irq_cause)))
        {
            Cy_SD_Host_SetNormalInterruptEnable(obj->base, (interrupt_enable_status | CY_SD_HOST_CARD_INTERRUPT));
        }
    }

    if (obj->callback_data.callback != NULL && userInterruptStatus > 0)
    {
        cyhal_sdio_event_callback_t callback = (cyhal_sdio_event_callback_t) obj->callback_data.callback;
        /* Call registered callbacks here */
        (callback)(obj->callback_data.callback_arg, (cyhal_sdio_irq_event_t) userInterruptStatus);
    }

    /*  Cannot clear cmd complete interrupt, as it is being polling-waited by many SD Host functions.
    *   It is expected to be cleared by mentioned polling functions. */
    userInterruptStatus &= (uint32_t) ~CY_SD_HOST_CMD_COMPLETE;

    /* Clear only handled events */
    Cy_SD_Host_ClearNormalInterruptStatus(blockAddr, userInterruptStatus);

    /* To clear Card Interrupt need to disable Card Interrupt Enable bit.
    *  The Card Interrupt is enabled after the current transfer is complete
    */
    if (0U != (obj->irq_cause & CY_SD_HOST_CARD_INTERRUPT))
    {
        if (0U != (interruptStatus & CY_SD_HOST_CARD_INTERRUPT))
        {
            uint32_t interruptMask = Cy_SD_Host_GetNormalInterruptEnable(blockAddr);
            interruptMask &= (uint32_t) ~CY_SD_HOST_CARD_INTERRUPT;

            /* Disable Card Interrupt */
            Cy_SD_Host_SetNormalInterruptEnable(blockAddr, interruptMask);
        }
    }
}

/* Software reset of SDHC block data and command circuits */
static void _cyhal_sdxx_reset(SDHC_Type *base)
{
    uint8_t block_num = (base == SDHC0) ? 0 : 1;
    _cyhal_sdhc_data_transfer_status[block_num] = _CYHAL_SDHC_NOT_RUNNING;;
    Cy_SD_Host_SoftwareReset(base, CY_SD_HOST_RESET_CMD_LINE);
    Cy_SD_Host_SoftwareReset(base, CY_SD_HOST_RESET_DATALINE);
    cyhal_system_delay_us(1);
    base->CORE.SW_RST_R = 0U;
}

cy_rslt_t cyhal_sdhc_init_hw(cyhal_sdhc_t *obj,
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
                        cyhal_gpio_t card_detect,
                        cyhal_gpio_t io_volt_sel,
                        cyhal_gpio_t card_pwr_en,
                        cyhal_gpio_t card_mech_write_prot,
                        cyhal_gpio_t led_ctrl,
                        cyhal_gpio_t emmc_reset)
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
    obj->pin_card_detect = CYHAL_NC_PIN_VALUE;
    obj->pin_io_vol_sel = CYHAL_NC_PIN_VALUE;
    obj->pin_card_pwr_en = CYHAL_NC_PIN_VALUE;
    obj->pin_card_mech_write_prot = CYHAL_NC_PIN_VALUE;
    obj->pin_led_ctrl = CYHAL_NC_PIN_VALUE;
    obj->pin_emmc_reset = CYHAL_NC_PIN_VALUE;

    obj->cd_gpio_cb_enabled = false;

    cyhal_gpio_t data[8];
    data[0] = data0;
    data[1] = data1;
    data[2] = data2;
    data[3] = data3;
    data[4] = data4;
    data[5] = data5;
    data[6] = data6;
    data[7] = data7;

    obj->data_timeout_tout = _CYHAL_SDHC_TRANSFER_TIMEOUT;
    obj->data_timeout_card_clocks_user = 0;
    obj->data_timeout_auto_reconfig = false;

    /* Reserve SDHC */
    const cyhal_resource_pin_mapping_t *map = _CYHAL_UTILS_GET_RESOURCE(cmd, cyhal_pin_map_sdhc_card_cmd);

    if (NULL == map || NC == data0)
    {
        result = CYHAL_SDHC_RSLT_ERR_PIN;
    }

    /* Reserve and configure GPIO pins */
    if (result == CY_RSLT_SUCCESS)
    {
        result = _cyhal_sdxx_setup_pin(
            obj, cmd, cyhal_pin_map_sdhc_card_cmd, _CYHAL_SDHC_ELEM_COUNT(cyhal_pin_map_sdhc_card_cmd),
            &(obj->pin_cmd), _CYHAL_SDHC_NOT_WEAK_FUNC);
    }

    if (result == CY_RSLT_SUCCESS)
    {
        result = _cyhal_sdxx_setup_pin(
            obj, clk, cyhal_pin_map_sdhc_clk_card, _CYHAL_SDHC_ELEM_COUNT(cyhal_pin_map_sdhc_clk_card),
            &(obj->pin_clk), _CYHAL_SDHC_NOT_WEAK_FUNC);
    }

    for (uint8_t i = 0; (i < 4) && (CY_RSLT_SUCCESS == result); i++)
    {
        if (NC != data[i])
        {
            result = _cyhal_sdxx_setup_pin(obj, data[i], cyhal_pin_map_sdhc_card_dat_3to0,
                    _CYHAL_SDHC_ELEM_COUNT(cyhal_pin_map_sdhc_card_dat_3to0), &(obj->pin_data[i]),
                    _CYHAL_SDHC_NOT_WEAK_FUNC);
        }
    }

#if _CYHAL_SDHC_DATA8_PRESENT
    for (uint8_t i = 4; (i < 8) && (CY_RSLT_SUCCESS == result); i++)
    {
        if (NC != data[i])
        {
            result = _cyhal_sdxx_setup_pin(obj, data[i], cyhal_pin_map_sdhc_card_dat_7to4,
                    _CYHAL_SDHC_ELEM_COUNT(cyhal_pin_map_sdhc_card_dat_7to4), &(obj->pin_data[i]),
                    _CYHAL_SDHC_NOT_WEAK_FUNC);
        }
    }
#else
    if ((NC != data4 || NC != data5 || NC != data6 || NC != data7) && (CY_RSLT_SUCCESS == result))
    {
        result = CYHAL_SDHC_RSLT_ERR_PIN;
    }
#endif

    if ((NC != card_detect) && (CY_RSLT_SUCCESS == result))
    {
#if _CYHAL_SDHC_CARD_DETECT_PRESENT
        result = _cyhal_sdxx_setup_pin(obj, card_detect, cyhal_pin_map_sdhc_card_detect_n,
                _CYHAL_SDHC_ELEM_COUNT(cyhal_pin_map_sdhc_card_detect_n), &(obj->pin_card_detect),
                _CYHAL_SDHC_CARD_DETECT);
#else
        result = CYHAL_SDHC_RSLT_ERR_PIN;
#endif
    }

    if ((NC != io_volt_sel) && (CY_RSLT_SUCCESS == result))
    {
#if _CYHAL_SDHC_IO_VOLT_SEL_PRESENT
        result = _cyhal_sdxx_setup_pin(obj, io_volt_sel, cyhal_pin_map_sdhc_io_volt_sel,
            _CYHAL_SDHC_ELEM_COUNT(cyhal_pin_map_sdhc_io_volt_sel), &(obj->pin_io_vol_sel),
            _CYHAL_SDHC_CARD_IO_VOLTAGE);
#else
        result = CYHAL_SDHC_RSLT_ERR_PIN;
#endif
    }

    if ((NC != card_pwr_en) && (CY_RSLT_SUCCESS == result))
    {
#if _CYHAL_SDHC_CARD_IF_PWR_EN_PRESENT
        result = _cyhal_sdxx_setup_pin(obj, card_pwr_en, cyhal_pin_map_sdhc_card_if_pwr_en,
            _CYHAL_SDHC_ELEM_COUNT(cyhal_pin_map_sdhc_card_if_pwr_en), &(obj->pin_card_pwr_en), _CYHAL_SDHC_CARD_VDD);
#else
        result = CYHAL_SDHC_RSLT_ERR_PIN;
#endif
    }

    if ((NC != card_mech_write_prot) && (CY_RSLT_SUCCESS == result))
    {
#if _CYHAL_SDHC_CARD_WRITE_PROT_PRESENT
        result = _cyhal_sdxx_setup_pin(obj, card_mech_write_prot, cyhal_pin_map_sdhc_card_mech_write_prot,
            _CYHAL_SDHC_ELEM_COUNT(cyhal_pin_map_sdhc_card_mech_write_prot), &(obj->pin_card_mech_write_prot),
            _CYHAL_SDHC_CARD_MECH_WRITE_PROTECT);
#else
        result = CYHAL_SDHC_RSLT_ERR_PIN;
#endif
    }

    if ((NC != led_ctrl) && (CY_RSLT_SUCCESS == result))
    {
#if _CYHAL_SDHC_LED_CTRL_PRESENT
        result = _cyhal_sdxx_setup_pin(obj, led_ctrl, cyhal_pin_map_sdhc_led_ctrl,
            _CYHAL_SDHC_ELEM_COUNT(cyhal_pin_map_sdhc_led_ctrl), &(obj->pin_led_ctrl), _CYHAL_SDHC_NOT_WEAK_FUNC);
#else
        result = CYHAL_SDHC_RSLT_ERR_PIN;
#endif
    }

    if ((NC != emmc_reset) && (CY_RSLT_SUCCESS == result))
    {
#if _CYHAL_SDHC_CARD_EMMC_RESET_PRESENT
        result = _cyhal_sdxx_setup_pin(obj, emmc_reset, cyhal_pin_map_sdhc_card_emmc_reset_n,
            _CYHAL_SDHC_ELEM_COUNT(cyhal_pin_map_sdhc_card_emmc_reset_n), &(obj->pin_emmc_reset),
            _CYHAL_SDHC_NOT_WEAK_FUNC);
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
    }

        if (CY_RSLT_SUCCESS == result)
        {
            obj->base = _CYHAL_SDHC_BASE_ADDRESSES[obj->resource.block_num];
            stored_objects[obj->resource.block_num] = (void *)obj;
            _cyhal_sdhc_data_transfer_status[obj->resource.block_num] = _CYHAL_SDHC_NOT_RUNNING;
            #if defined(CY_RTOS_AWARE) || defined(COMPONENT_RTOS_AWARE)
            _cyhal_sdxx_sema_status[obj->resource.block_num] = _CYHAL_SDXX_SEMA_NOT_INITED;
            #endif /* defined(CY_RTOS_AWARE) || defined(COMPONENT_RTOS_AWARE) */

        /* Enable the SDHC block */
        Cy_SD_Host_Enable(obj->base);

        /* Do not support eMMC card */
        obj->emmc = config->isEmmc;
        obj->dma_type = CY_SD_HOST_DMA_ADMA2;

        /* Configure SD Host to operate */
        cy_stc_sd_host_init_config_t hostConfig;
        hostConfig.dmaType = obj->dma_type;
        hostConfig.emmc = obj->emmc;

    #if _CYHAL_SDHC_LED_CTRL_PRESENT
        hostConfig.enableLedControl = (NC != led_ctrl);
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

        result = (cy_rslt_t)Cy_SD_Host_Init(obj->base, &hostConfig, &obj->context);
    }

    if(result == CY_RSLT_SUCCESS)
    {
        obj->pm_transition_pending = false;
        obj->pm_callback_data.callback = &_cyhal_sdhc_syspm_callback,
        obj->pm_callback_data.states =
                (cyhal_syspm_callback_state_t)(CYHAL_SYSPM_CB_CPU_DEEPSLEEP | CYHAL_SYSPM_CB_SYSTEM_HIBERNATE);
        obj->pm_callback_data.next = NULL;
        obj->pm_callback_data.args = obj;
        /* The CYHAL_SYSPM_BEFORE_TRANSITION mode cannot be ignored because the PM handler
            * calls the PDL deep-sleep callback that disables the block in this mode before transitioning.
            */
        obj->pm_callback_data.ignore_modes = (cyhal_syspm_callback_mode_t)0,

        _cyhal_syspm_register_peripheral_callback(&obj->pm_callback_data);

        obj->low_voltage_io_desired = config->lowVoltageSignaling;
        obj->bus_width = config->busWidth;
    }

    if (result != CY_RSLT_SUCCESS)
    {
        cyhal_sdhc_free(obj);
    }

    return result;
}

cy_rslt_t cyhal_sdhc_init_card(cyhal_sdhc_t *obj)
{
    CY_ASSERT(NULL != obj);
    CY_ASSERT(NULL != obj->base);

    cy_stc_sd_host_sd_card_config_t stcSdcardCfg;
    cy_en_sd_host_card_capacity_t cardCapacity;
    uint32_t rca = 0UL;
    cy_en_sd_host_card_type_t cardType = CY_SD_HOST_UNUSABLE;

    /* Initialize the SDIO/SD/eMMC card configuration structure */
    stcSdcardCfg.lowVoltageSignaling = obj->low_voltage_io_desired;
    stcSdcardCfg.busWidth = _cyhal_sdhc_convert_buswidth(obj->bus_width);
    stcSdcardCfg.cardType = &cardType;
    stcSdcardCfg.rca = &rca;
    stcSdcardCfg.cardCapacity = &cardCapacity;

    /* Initialize the card */
    cy_rslt_t result = (cy_rslt_t)Cy_SD_Host_InitCard(obj->base, &stcSdcardCfg, &obj->context);

    /* Update SD Card frequency to be 25 Mhz */
    if (CY_RSLT_SUCCESS == result)
    {
        result = cyhal_sdhc_set_frequency(obj, CY_SD_HOST_CLK_25M, true);
    }

    return result;
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
                          cyhal_gpio_t card_detect,
                          cyhal_gpio_t io_volt_sel,
                          cyhal_gpio_t card_pwr_en,
                          cyhal_gpio_t card_mech_write_prot,
                          cyhal_gpio_t led_ctrl,
                          cyhal_gpio_t emmc_reset)
{
    cy_rslt_t result = cyhal_sdhc_init_hw(obj, config, cmd, clk, data0, data1, data2, data3, data4, data5, data6,
            data7, card_detect, io_volt_sel, card_pwr_en, card_mech_write_prot, led_ctrl, emmc_reset);

    /* Initialize card */
    if (result == CY_RSLT_SUCCESS)
    {
        result = cyhal_sdhc_init_card(obj);
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
        #if defined(CY_RTOS_AWARE) || defined(COMPONENT_RTOS_AWARE)
        if (_CYHAL_SDXX_SEMA_NOT_INITED != _cyhal_sdxx_sema_status[obj->resource.block_num])
        {
            cy_rtos_deinit_semaphore(&_cyhal_sdhc_smphr_xfer_done[obj->resource.block_num]);
            _cyhal_sdxx_sema_status[obj->resource.block_num] = _CYHAL_SDXX_SEMA_NOT_INITED;
        }
        #endif /* CY_RTOS_AWARE or COMPONENT_RTOS_AWARE defined */

        Cy_SD_Host_DeInit(obj->base);
        stored_objects[obj->resource.block_num] = NULL;
        _cyhal_sdhc_data_transfer_status[obj->resource.block_num] = _CYHAL_SDHC_NOT_RUNNING;

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
    if ((obj->cd_gpio_cb_enabled) && (CYHAL_NC_PIN_VALUE != obj->pin_card_detect))
    {
        cyhal_gpio_enable_event(obj->pin_card_detect, CYHAL_GPIO_IRQ_BOTH, CYHAL_ISR_PRIORITY_DEFAULT, false);
        obj->cd_gpio_cb_enabled = false;
    }
    _cyhal_utils_release_if_used(&(obj->pin_card_detect));
#endif

#if _CYHAL_SDHC_IO_VOLT_SEL_PRESENT
    _cyhal_utils_release_if_used(&(obj->pin_io_vol_sel));
#endif

#if _CYHAL_SDHC_CARD_IF_PWR_EN_PRESENT
    _cyhal_utils_release_if_used(&(obj->pin_card_pwr_en));
#endif

#if _CYHAL_SDHC_CARD_WRITE_PROT_PRESENT
    _cyhal_utils_release_if_used(&(obj->pin_card_mech_write_prot));
#endif

#if _CYHAL_SDHC_LED_CTRL_PRESENT
    _cyhal_utils_release_if_used(&(obj->pin_led_ctrl));
#endif

#if _CYHAL_SDHC_CARD_EMMC_RESET_PRESENT
    _cyhal_utils_release_if_used(&(obj->pin_emmc_reset));
#endif
}

cy_rslt_t cyhal_sdhc_read(const cyhal_sdhc_t *obj, uint32_t address, uint8_t *data, size_t *length)
{
    CY_ASSERT(_CYHAL_SDHC_NOT_RUNNING == _cyhal_sdhc_data_transfer_status[obj->resource.block_num]);
    _cyhal_sdxx_setup_smphr(obj->resource.block_num);
    cy_rslt_t ret = cyhal_sdhc_read_async(obj, address, data, length);
    /* Waiting for async operation to end */
    if (CY_RSLT_SUCCESS == ret)
    {
        ret = _cyhal_sdxx_waitfor_transfer_complete(obj->resource.block_num, obj->base, false);
    }
    if(CY_RSLT_SUCCESS != ret)
    {
        _cyhal_sdhc_data_transfer_status[obj->resource.block_num] = _CYHAL_SDHC_NOT_RUNNING;
    }
    return ret;
}

cy_rslt_t cyhal_sdhc_write(const cyhal_sdhc_t *obj, uint32_t address, const uint8_t *data, size_t *length)
{
    CY_ASSERT(_CYHAL_SDHC_NOT_RUNNING == _cyhal_sdhc_data_transfer_status[obj->resource.block_num]);
    _cyhal_sdxx_setup_smphr(obj->resource.block_num);
    cy_rslt_t ret = cyhal_sdhc_write_async(obj, address, data, length);
    /* Waiting for async operation to end */
    if (CY_RSLT_SUCCESS == ret)
    {
        ret = _cyhal_sdxx_waitfor_transfer_complete(obj->resource.block_num, obj->base, false);
    }
    if(CY_RSLT_SUCCESS != ret)
    {
        _cyhal_sdhc_data_transfer_status[obj->resource.block_num] = _CYHAL_SDHC_NOT_RUNNING;
    }
    return ret;
}

cy_rslt_t cyhal_sdhc_erase(const cyhal_sdhc_t *obj, uint32_t start_addr, size_t length)
{
    if (obj->pm_transition_pending)
    {
        return CYHAL_SYSPM_RSLT_ERR_PM_PENDING;
    }
    if (0 == length)
    {
        return CYHAL_SDHC_RSLT_ERR_WRONG_PARAM;
    }

    cy_rslt_t ret = CY_RSLT_SUCCESS;
    cy_en_sd_host_erase_type_t eraseType = CY_SD_HOST_ERASE_ERASE;
    uint32_t i = _CYHAL_SDHC_RETRY_TIMES;
    uint32_t cardStatus;

    if (obj->emmc)
    {
         eraseType = CY_SD_HOST_ERASE_TRIM;
    }

    /* First clear out the transfer and command complete statuses */
    Cy_SD_Host_ClearNormalInterruptStatus(obj->base, (CY_SD_HOST_XFER_COMPLETE | CY_SD_HOST_CMD_COMPLETE));

    if (obj->irq_cause & CYHAL_SDHC_CMD_COMPLETE)
    {
        /*  Enabling command complete interrupt mask if corresponding event was enabled by user
        *   _cyhal_sdhc_irq_handler will disable CY_SD_HOST_CMD_COMPLETE mask once interrupt
        *   is generated. */
        Cy_SD_Host_SetNormalInterruptMask(obj->base, Cy_SD_Host_GetNormalInterruptMask(obj->base) |
                CY_SD_HOST_CMD_COMPLETE);
    }

    ret = (cy_rslt_t)Cy_SD_Host_Erase(obj->base, start_addr, (start_addr + length - 1), eraseType, &obj->context);

    if (CY_RSLT_SUCCESS == ret)
    {
        ret =  (cy_rslt_t)_cyhal_sdxx_pollcmdcomplete(obj->base, obj->resource.block_num);
    }

    if (CY_RSLT_SUCCESS == ret)
    {
        if (false == obj->emmc)
        {
            /* polling result */
            ret = CYHAL_SDHC_RSLT_ERR_ERASE_CMPLT_TIMEOUT;

            while (i-- > 0UL)
            {
                cardStatus = Cy_SD_Host_GetCardStatus(obj->base, &obj->context);
                if (((CY_SD_HOST_CARD_TRAN << CY_SD_HOST_CMD13_CURRENT_STATE) |
                    (1UL << CY_SD_HOST_CMD13_READY_FOR_DATA)) == cardStatus)
                {
                    ret = CY_RSLT_SUCCESS;
                    break;
                }
                cyhal_system_delay_us(_CYHAL_SDHC_FUJE_TIMEOUT_MS); /* The Fuje timeout for one block. */
            }
        }
        else
        {
            cyhal_system_delay_ms(_CYHAL_SDHC_EMMC_TRIM_DELAY_MS);
        }
    }

    return ret;
}

cy_rslt_t cyhal_sdhc_read_async(const cyhal_sdhc_t *obj, uint32_t address, uint8_t *data, size_t *length)
{
    if (obj->pm_transition_pending)
    {
        return CYHAL_SYSPM_RSLT_ERR_PM_PENDING;
    }

    cy_rslt_t ret;
    cy_stc_sd_host_write_read_config_t dataConfig;

    /* The pointer to data. */
    dataConfig.data = (uint32_t*)data;
    /* The address to write/read data on the card or eMMC. */
    dataConfig.address = address;
    /* The number of blocks to write/read. */
    dataConfig.numberOfBlocks = (uint32_t)*length;
    dataConfig.autoCommand = (1UL == (uint32_t)*length)
        ? CY_SD_HOST_AUTO_CMD_NONE
        : CY_SD_HOST_AUTO_CMD_AUTO;
    /* The timeout value for the transfer. */
    dataConfig.dataTimeout = obj->data_timeout_tout;
    /* For EMMC cards enable reliable write. */
    dataConfig.enReliableWrite = false;
    dataConfig.enableDma = true;

    /* First clear out the transfer and command complete statuses */
    Cy_SD_Host_ClearNormalInterruptStatus(obj->base, (CY_SD_HOST_XFER_COMPLETE | CY_SD_HOST_CMD_COMPLETE));

    ret = _cyhal_sdxx_prepare_for_transfer(obj->base, false);

    if (CY_RSLT_SUCCESS == ret)
    {
        // The PDL takes care of the cmd complete stage in sdhc mode so we can jump straight
        // to waiting for the xfer complete
        _cyhal_sdhc_data_transfer_status[obj->resource.block_num] = _CYHAL_SDHC_WAIT_XFER_COMPLETE;
        ret = (cy_rslt_t)Cy_SD_Host_Read(obj->base, &dataConfig, &obj->context);
        /* Read operation failed */
        if (CY_RSLT_SUCCESS != ret)
        {
            _cyhal_sdhc_data_transfer_status[obj->resource.block_num] = _CYHAL_SDHC_NOT_RUNNING;
        }
    }

    return ret;
}

cy_rslt_t cyhal_sdhc_write_async(const cyhal_sdhc_t *obj, uint32_t address, const uint8_t *data, size_t *length)
{
    if (obj->pm_transition_pending)
    {
        return CYHAL_SYSPM_RSLT_ERR_PM_PENDING;
    }

    cy_rslt_t ret;
    cy_stc_sd_host_write_read_config_t dataConfig;

    /* The pointer to data. */
    dataConfig.data = (uint32_t*)data;
    /* The address to write/read data on the card or eMMC. */
    dataConfig.address = address;
    /* The number of blocks to write/read. */
    dataConfig.numberOfBlocks = (uint32_t)*length;
    dataConfig.autoCommand = (1UL == (uint32_t)*length)
        ? CY_SD_HOST_AUTO_CMD_NONE
        : CY_SD_HOST_AUTO_CMD_AUTO;
    /* The timeout value for the transfer. */
    dataConfig.dataTimeout = obj->data_timeout_tout;
    /* For EMMC cards enable reliable write. */
    dataConfig.enReliableWrite = false;
    dataConfig.enableDma = true;

    /* First clear out the transfer and command complete statuses */
    Cy_SD_Host_ClearNormalInterruptStatus(obj->base, (CY_SD_HOST_XFER_COMPLETE | CY_SD_HOST_CMD_COMPLETE));

    ret = _cyhal_sdxx_prepare_for_transfer(obj->base, false);

    if (CY_RSLT_SUCCESS == ret)
    {
        // The PDL takes care of the cmd complete stage in sdhc mode so we can jump straight
        // to waiting for the xfer complete
        _cyhal_sdhc_data_transfer_status[obj->resource.block_num] = _CYHAL_SDHC_WAIT_XFER_COMPLETE;
        ret = (cy_rslt_t)Cy_SD_Host_Write(obj->base, &dataConfig, &obj->context);
        /* Write operation failed */
        if (CY_RSLT_SUCCESS != ret)
        {
            _cyhal_sdhc_data_transfer_status[obj->resource.block_num] = _CYHAL_SDHC_NOT_RUNNING;
        }
    }

    return ret;
}

bool cyhal_sdhc_is_busy(const cyhal_sdhc_t *obj)
{
    bool busy_status = true;
    /* Check DAT Line Active */
    uint32_t pState = Cy_SD_Host_GetPresentState(obj->base);
    if ((CY_SD_HOST_DAT_LINE_ACTIVE != (pState & CY_SD_HOST_DAT_LINE_ACTIVE)) &&
        (CY_SD_HOST_CMD_CMD_INHIBIT_DAT != (pState & CY_SD_HOST_CMD_CMD_INHIBIT_DAT)))
    {
        busy_status = false;
    }
    return busy_status || (_CYHAL_SDHC_NOT_RUNNING != _cyhal_sdhc_data_transfer_status[obj->resource.block_num]);
}

cy_rslt_t cyhal_sdhc_abort_async(const cyhal_sdhc_t *obj)
{
    cy_rslt_t ret = (cy_rslt_t)Cy_SD_Host_AbortTransfer(obj->base, &obj->context);
    if (CY_RSLT_SUCCESS == ret)
    {
        _cyhal_sdhc_data_transfer_status[obj->resource.block_num] = _CYHAL_SDHC_NOT_RUNNING;
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
        interruptMask = (enable) ? CYHAL_SDHC_ALL_INTERRUPTS : 0;
    }

    obj->irq_cause = interruptMask;
    /*  CY_SD_HOST_CMD_COMPLETE and CY_SD_HOST_XFER_COMPLETE cannot be always enabled because of SD Host driver limitations.
    *   SDHC HAL transfer APIs are taking care of enabling these statuses. CY_SD_HOST_CMD_COMPLETE is only
    *   enabled if user callback is enabled while CY_SD_HOST_XFER_COMPLETE is enabled by transfer API regardless it was enabled
    *   by user or not. */
    interruptMask &= ((uint32_t) ~CY_SD_HOST_CMD_COMPLETE) & ((uint32_t) ~CY_SD_HOST_XFER_COMPLETE);
    Cy_SD_Host_SetNormalInterruptMask(obj->base, interruptMask);
}

bool cyhal_sdhc_is_card_inserted(const cyhal_sdhc_t *obj)
{
    CY_ASSERT(NULL != obj);
    CY_ASSERT(NULL != obj->base);
    return Cy_SD_Host_IsCardConnected(obj->base);
}

bool cyhal_sdhc_is_card_mech_write_protected(const cyhal_sdhc_t *obj)
{
    CY_ASSERT(NULL != obj);
    CY_ASSERT(NULL != obj->base);
    return Cy_SD_Host_IsWpSet(obj->base);
}

cy_rslt_t cyhal_sdhc_get_block_count(cyhal_sdhc_t *obj, uint32_t * block_count)
{
    CY_ASSERT(NULL != obj);
    CY_ASSERT(NULL != block_count);

    cy_rslt_t result = CYHAL_SDHC_RSLT_ERR_BLOCK_COUNT_GET_FAILURE;

    if ((NULL != obj->base) && ((CY_SD_HOST_SD == obj->context.cardType) || (CY_SD_HOST_EMMC == obj->context.cardType)))
    {
        *block_count = obj->context.maxSectorNum;
        result = CY_RSLT_SUCCESS;
    }

    return result;
}

cy_rslt_t cyhal_sdhc_set_frequency(cyhal_sdhc_t *obj, uint32_t hz, bool negotiate)
{
    CY_ASSERT(NULL != obj);
    cy_rslt_t result = CYHAL_SDHC_RSLT_ERR_SET_FREQ;
    if (NULL != obj->base)
    {
        /*  Assigning for desired freq as for now and this variable
         *  will be updated with achieved freq. */
        uint32_t actual_freq = hz;
        result = _cyhal_sdxx_sdcardchangeclock(obj, false, &actual_freq, obj->low_voltage_io_set, negotiate);
        if (CY_RSLT_SUCCESS == result)
        {
            obj->bus_frequency_hz = actual_freq;
            if (obj->data_timeout_auto_reconfig && (0 != obj->data_timeout_card_clocks_user))
            {
                /* User have data timeout configured, we need to reconfigure it according to new card clock */
                result = cyhal_sdhc_set_data_read_timeout(obj, obj->data_timeout_card_clocks_user, true);
            }
        }
    }
    return result;
}

uint32_t cyhal_sdhc_get_frequency(cyhal_sdhc_t *obj)
{
    CY_ASSERT(NULL != obj);
    return obj->bus_frequency_hz;
}

cy_rslt_t cyhal_sdhc_set_data_read_timeout(cyhal_sdhc_t *obj, uint32_t timeout, bool auto_reconfigure)
{
    CY_ASSERT(NULL != obj);

    uint32_t current_card_clock = cyhal_sdhc_get_frequency(obj);

    /* TMCLK works on 1 MHz in current block implementation if corresponding HF clock is 100 MHz.
    *  This is defined in registers (TOUT_CLK_FREQ[5:0] = 1 and TOUT_CLK_UNIT[7] = 1).  */
    float tout_clk_period_us = _CYHAL_SDHC_EXPECTED_BASE_CLK_FREQ_HZ / (float)obj->block_source_freq_hz;

    uint32_t user_needs_us = (((uint64_t)timeout * 1000000) + current_card_clock - 1) / current_card_clock;

    /* Timeout range from 0x0 to 0xE is valid for PSoC6, which corresponds to
    * TMCLK x 2^13 for 0, TMCLK x 2^14 for 1, ..., TMCLK x 2^27 for 0xE. 0xF is reserved. */
    const uint8_t tout_clk_power_base = _CYHAL_SDHC_TOUT_TMCLK_POW_MIN;
    for (uint8_t tmclk_power = tout_clk_power_base; tmclk_power <= _CYHAL_SDHC_TOUT_TMCLK_POW_MAX; tmclk_power++)
    {
        if (tout_clk_period_us * (1 << tmclk_power) >= user_needs_us)
        {
            obj->data_timeout_tout = tmclk_power - tout_clk_power_base;
            obj->data_timeout_card_clocks_user = timeout;
            obj->data_timeout_auto_reconfig = auto_reconfigure;
            return CY_RSLT_SUCCESS;
        }
    }
    return CYHAL_SDHC_RSLT_ERR_TOUT_CFG;
}

cy_rslt_t cyhal_sdhc_config_data_transfer(cyhal_sdhc_t *obj, cyhal_sdhc_data_config_t *data_config)
{
    CY_ASSERT(NULL != obj);
    CY_ASSERT(NULL != obj->base);
    CY_ASSERT(NULL != data_config);

    if (NULL == data_config->data_ptr)
    {
        return CYHAL_SDHC_RSLT_ERR_WRONG_PARAM;
    }

    cy_stc_sd_host_data_config_t dataConfig = {
        .blockSize = data_config->block_size,
        .numberOfBlock = data_config->number_of_blocks,
        .enableDma = true,
        .autoCommand = (cy_en_sd_host_auto_cmd_t)data_config->auto_command,
        .read = data_config->is_read,
        /* .data is skipped to configure adma2 descriptor for it later */
        .dataTimeout = obj->data_timeout_tout,
        .enableIntAtBlockGap = false,
        .enReliableWrite = false
    };

    uint32_t length = data_config->block_size * data_config->number_of_blocks;

    obj->adma_descriptor_tbl[0] = (1UL << CY_SD_HOST_ADMA_ATTR_VALID_POS) | /* Attr Valid */
                            (1UL << CY_SD_HOST_ADMA_ATTR_END_POS) |   /* Attr End */
                            (0UL << CY_SD_HOST_ADMA_ATTR_INT_POS) |   /* Attr Int */
                            (CY_SD_HOST_ADMA_TRAN << CY_SD_HOST_ADMA_ACT_POS) |
                            (length << CY_SD_HOST_ADMA_LEN_POS);     /* Len */

    obj->adma_descriptor_tbl[1] = (uint32_t)data_config->data_ptr;

    /* The address of the ADMA descriptor table. */
    dataConfig.data = (uint32_t*)&obj->adma_descriptor_tbl[0];

    return (cy_rslt_t) Cy_SD_Host_InitDataTransfer(obj->base, &dataConfig);
}

cy_rslt_t cyhal_sdhc_send_cmd(cyhal_sdhc_t *obj, cyhal_sdhc_cmd_config_t *cmd_config)
{
    CY_ASSERT(NULL != obj);
    CY_ASSERT(NULL != obj->base);
    CY_ASSERT(NULL != cmd_config);

    cy_rslt_t result = CY_RSLT_SUCCESS;
    bool cmd_data_present = (NULL != cmd_config->data_config);

    cy_stc_sd_host_cmd_config_t cmd = {
        .commandIndex = cmd_config->command_index,
        .commandArgument = cmd_config->command_argument,
        .enableCrcCheck = cmd_config->enable_crc_check,
        .enableAutoResponseErrorCheck = false,
        .respType = (cy_en_sd_host_response_type_t)cmd_config->response_type,
        .enableIdxCheck = cmd_config->enable_idx_check,
        .dataPresent = cmd_data_present,
        .cmdType = (cy_en_sd_host_cmd_type_t)cmd_config->command_type
    };

    /* First clear out the transfer and command complete statuses */
    Cy_SD_Host_ClearNormalInterruptStatus(obj->base, (CY_SD_HOST_XFER_COMPLETE | CY_SD_HOST_CMD_COMPLETE));

    if (cmd_data_present)
    {
        result = _cyhal_sdxx_prepare_for_transfer(obj->base, false);
    }

    if (CY_RSLT_SUCCESS == result)
    {
        if (cmd_data_present)
        {
            /* Data will be transfered in scope of this command, so setting _CYHAL_SDHC_WAIT_BOTH (cmd and data)
            *  and seting up xfer complete semaphore */
            _cyhal_sdhc_data_transfer_status[obj->resource.block_num] = _CYHAL_SDHC_WAIT_BOTH;
            _cyhal_sdxx_setup_smphr(obj->resource.block_num);
        }
        else
        {
            /* No data is transfered in this command, just waiting for cmd to complete */
            _cyhal_sdhc_data_transfer_status[obj->resource.block_num] = _CYHAL_SDHC_WAIT_CMD_COMPLETE;
        }
        result = (cy_rslt_t)Cy_SD_Host_SendCommand(obj->base, &cmd);
    }

    if (CY_RSLT_SUCCESS == result)
    {
        result = (cy_rslt_t)_cyhal_sdxx_pollcmdcomplete(obj->base, obj->resource.block_num);
    }

    if (CY_RSLT_SUCCESS != result)
    {
        /* Transfer failed */
        _cyhal_sdhc_data_transfer_status[obj->resource.block_num] = _CYHAL_SDHC_NOT_RUNNING;
    }

    return result;
}

cy_rslt_t cyhal_sdhc_get_response(cyhal_sdhc_t *obj, uint32_t *response, bool large_response)
{
    CY_ASSERT(NULL != obj);
    CY_ASSERT(NULL != obj->base);
    CY_ASSERT(NULL != response);
    return (cy_rslt_t)Cy_SD_Host_GetResponse(obj->base, response, large_response);
}

cy_rslt_t cyhal_sdhc_wait_transfer_complete(cyhal_sdhc_t *obj)
{
    CY_ASSERT(NULL != obj);
    CY_ASSERT(NULL != obj->base);
    return _cyhal_sdxx_waitfor_transfer_complete(obj->resource.block_num, obj->base, false);
}

static cy_rslt_t _cyhal_sdhc_io_volt_switch_seq(cyhal_sdhc_t *obj)
{
    CY_ASSERT(NULL != obj);
    CY_ASSERT(NULL != obj->base);

    uint32_t p_state;
    cy_rslt_t result = CY_RSLT_SUCCESS;

    /* Disable providing the SD Clock. */
    Cy_SD_Host_DisableSdClk(obj->base);

    p_state = Cy_SD_Host_GetPresentState(obj->base) & SDHC_CORE_PSTATE_REG_DAT_3_0_Msk;

    /* Check DAT[3:0]. */
    if (0UL == p_state)
    {
        /* Switch the bus to 1.8 V (Set the IO_VOLT_SEL pin to low)*/
        Cy_SD_Host_ChangeIoVoltage(obj->base, CY_SD_HOST_IO_VOLT_1_8V);

        /* Wait 10 ms to 1.8 voltage regulator to be stable. */
        cyhal_system_delay_ms(_CYHAL_SDHC_1_8_REG_STABLE_TIME_MS);

        /* Check the 1.8V signaling enable. */
        if (true == _FLD2BOOL(SDHC_CORE_HOST_CTRL2_R_SIGNALING_EN,
                            SDHC_CORE_HOST_CTRL2_R(obj->base)))
        {
            /* Enable providing the SD Clock. */
            Cy_SD_Host_EnableSdClk(obj->base);

            /* Wait for the stable CLK */
            cyhal_system_delay_ms(CY_SD_HOST_CLK_RAMP_UP_TIME_MS);

            p_state = Cy_SD_Host_GetPresentState(obj->base) & SDHC_CORE_PSTATE_REG_DAT_3_0_Msk;

            /* Check DAT[3:0]. */
            if (SDHC_CORE_PSTATE_REG_DAT_3_0_Msk != p_state)
            {
                result = CYHAL_SDHC_RSLT_ERR_IO_VOLT_SWITCH_SEQ;
            }
        }
        else
        {
            result = CYHAL_SDHC_RSLT_ERR_IO_VOLT_SWITCH_SEQ;
        }
    }
    else
    {
        result = CYHAL_SDHC_RSLT_ERR_IO_VOLT_SWITCH_SEQ;
    }

    return result;
}

/* Ncc - is time, needed to wait after certain commands before issuing next cmd.
*  Ncc is 8 clock cycles which for 400 kHz is 20 us */
static inline void _cyhal_sdhc_wait_ncc_time_at_400khz(void)
{
    cyhal_system_delay_us(20);
}

static cy_rslt_t _cyhal_sdhc_io_volt_negotiate(cyhal_sdhc_t *obj, cyhal_sdhc_io_voltage_t io_voltage)
{
    /* GO_IDLE (CMD0) command is issued in scope of Cy_SD_Host_SelBusVoltage, which resets
    *  SD bus to 1 bit bus width and 400 khz on the SD card side. Preparing host side for that. */

    /* 1 bit data bus width */
    cy_rslt_t result = cyhal_sdhc_set_bus_width(obj, 1, true);

    if (CY_RSLT_SUCCESS == result)
    {
        /* 400 khz sd bus clock */
        result = cyhal_sdhc_set_frequency(obj, 400000, false);
    }

    if (CY_RSLT_SUCCESS == result)
    {
        bool voltage_1v8 = (io_voltage == CYHAL_SDHC_IO_VOLTAGE_1_8V);
        result = (cy_rslt_t)Cy_SD_Host_SelBusVoltage(obj->base, voltage_1v8, &obj->context);
        /* Card is expected to be in "ready" state now */
    }

    if (CY_RSLT_SUCCESS == result)
    {
        /* GetCid (CMD2) will switch card from "ready" state to "ident" */
        uint32_t cid_reg[4];  /* The Device Identification register. */
        result = (cy_rslt_t)Cy_SD_Host_GetCid(obj->base, cid_reg);
        _cyhal_sdhc_wait_ncc_time_at_400khz();
    }

    if (CY_RSLT_SUCCESS == result)
    {
        /* Cy_SD_Host_GetRca (CMD3) will switch card from "ident" state to "stand-by" state */
        obj->context.RCA = Cy_SD_Host_GetRca(obj->base);
        _cyhal_sdhc_wait_ncc_time_at_400khz();

        /* CMD7 will switch card from "stand-by" state to "transfer" state */
        cyhal_sdhc_cmd_config_t cmd = {
            .command_index = 7,
            .command_argument = obj->context.RCA << _CYHAL_SDHC_RCA_SHIFT,
            .enable_crc_check = false,
            .response_type = CYHAL_SDHC_RESPONSE_LEN_48B,
            .enable_idx_check = false,
            .command_type = CYHAL_SDHC_CMD_NORMAL,
            .data_config = NULL
        };

        result = cyhal_sdhc_send_cmd(obj, &cmd);
    }

    if (CY_RSLT_SUCCESS == result)
    {
        _cyhal_sdhc_wait_ncc_time_at_400khz();
        result = _cyhal_sdxx_waitfor_transfer_complete(obj->resource.block_num, obj->base, false);
    }

    return result;
}

typedef struct
{
    en_hsiom_sel_t      clk;
    en_hsiom_sel_t      cmd;
    #if _CYHAL_SDHC_DATA8_PRESENT
    en_hsiom_sel_t      data[8];
    #else
    en_hsiom_sel_t      data[4];
    #endif
} _cyhal_sdhc_saved_lines_hsiom_t;

/* power cycle config single pin */
static void _cyhal_sdhc_pc_config_single_pin(cyhal_gpio_t *gpio_ptr, en_hsiom_sel_t *hsiom_ptr, bool set_line_low)
{
    GPIO_PRT_Type* port;
    uint8_t pin;

    cyhal_gpio_t gpio = *gpio_ptr;
    if (NC != gpio)
    {
        port = CYHAL_GET_PORTADDR(gpio);
        pin = (uint8_t)CYHAL_GET_PIN(gpio);
        if(set_line_low)
        {
            /* Switching HSIOM to GPIO and set pin to low state */
            *hsiom_ptr = Cy_GPIO_GetHSIOM(port, pin);
            Cy_GPIO_Clr(port, pin);
            Cy_GPIO_SetHSIOM(port, pin, HSIOM_SEL_GPIO);
        }
        else
        {
            /* Restore pin's HSIOM configuration for SDHC block signals */
            Cy_GPIO_SetHSIOM(port, pin, *hsiom_ptr);
        }
    }
}

/* Per SD Spec, during card power down, DAT, CMD and CLK lines should be disconnected or driven to logical 0
*  by the host to avoid operating current being drawn through the signal lines.
*  Lines are set low if set_lines_low true, pins configuration resored if set_lines_low false */
static void _cyhal_sdhc_power_cycle_config_lines(cyhal_sdhc_t *obj, _cyhal_sdhc_saved_lines_hsiom_t *pins_cfg,
    bool set_lines_low)
{
    _cyhal_sdhc_pc_config_single_pin(&obj->pin_clk, &pins_cfg->clk, set_lines_low);
    _cyhal_sdhc_pc_config_single_pin(&obj->pin_cmd, &pins_cfg->cmd, set_lines_low);
    for(size_t i = 0; i < sizeof(pins_cfg->data)/sizeof(pins_cfg->data[0]); ++i)
    {
        _cyhal_sdhc_pc_config_single_pin(&obj->pin_data[i], &pins_cfg->data[i], set_lines_low);
    }
}

static cy_rslt_t _cyhal_sdhc_card_power_cycle(cyhal_sdhc_t *obj)
{
    /* To perform reliable SD card hard reset, Card VDD should drop to below 0.5V for at least 1 ms */
    if (NC == obj->pin_card_pwr_en)
    {
        /* card_pwr_en needs to be provided in order to control card VDD */
        return CYHAL_SDHC_RSLT_ERR_PIN;
    }

    _cyhal_sdhc_saved_lines_hsiom_t pins_cfg;

    /* Drive signal lines logic 0 to avoid card being powered through signal lines */
    _cyhal_sdhc_power_cycle_config_lines(obj, &pins_cfg, true);
    /* Power down the card */
    cyhal_sdhc_enable_card_power(obj, false);
    /* Waiting for 1 ms per spec */
    cyhal_system_delay_us(1000);
    /* Restore signals configuration */
    _cyhal_sdhc_power_cycle_config_lines(obj, &pins_cfg, false);
    /* Power up the card */
    cyhal_sdhc_enable_card_power(obj, true);
    cyhal_system_delay_ms(_CYHAL_SDHC_PWR_RAMP_UP_TIME_MS);

    return CY_RSLT_SUCCESS;
}

cy_rslt_t cyhal_sdhc_set_io_voltage(cyhal_sdhc_t *obj, cyhal_sdhc_io_voltage_t io_voltage, cyhal_sdhc_io_volt_action_type_t io_switch_type)
{
    CY_ASSERT(NULL != obj);
    CY_ASSERT(NULL != obj->base);

    if (NC == obj->pin_io_vol_sel)
    {
        /* Need to have io_volt_sel pin configured in order to switch io voltage */
        return CYHAL_SDHC_RSLT_ERR_PIN;
    }

    if ((CY_SD_HOST_SD != obj->context.cardType) && (CYHAL_SDHC_IO_VOLT_ACTION_NONE != io_switch_type) &&
            (CYHAL_SDHC_IO_VOLTAGE_1_8V == io_voltage))
    {
        /* Negotiation or Voltage Switch sequence can only be performed for SD card */
        return CYHAL_SDHC_RSLT_ERR_UNSUPPORTED;
    }

    cy_rslt_t result = CY_RSLT_SUCCESS;

    switch (io_switch_type)
    {
        case CYHAL_SDHC_IO_VOLT_ACTION_NEGOTIATE:
        {
            /*  Bus width and card frequency settings are initialized to default during negotiation.
            *   Saving the parameters to restore them after I/O switch is done. */
            uint8_t sd_bus_width_before_switch = cyhal_sdhc_get_bus_width(obj);
            uint32_t sd_freq_before_switch = cyhal_sdhc_get_frequency(obj);

            /*  Once the card enters 1.8V signaling mode, it cannot be switched back to 3.3V signaling without power cycle. */
            if ((obj->low_voltage_io_set) && (CYHAL_SDHC_IO_VOLTAGE_3_3V == io_voltage))
            {
                result = _cyhal_sdhc_card_power_cycle(obj);
                if (CY_RSLT_SUCCESS == result)
                {
                    obj->low_voltage_io_desired = false;
                    result = cyhal_sdhc_init_card(obj);
                }
            }
            else
            {
                result = _cyhal_sdhc_io_volt_negotiate(obj, io_voltage);
            }

            /* Return back bus width and frequency regardless of volt select change status */
            (void)cyhal_sdhc_set_bus_width(obj, sd_bus_width_before_switch, true);
            (void)cyhal_sdhc_set_frequency(obj, sd_freq_before_switch, true);
            break;
        }

        case CYHAL_SDHC_IO_VOLT_ACTION_SWITCH_SEQ_ONLY:
            if (CYHAL_SDHC_IO_VOLTAGE_1_8V == io_voltage)
            {
                result = _cyhal_sdhc_io_volt_switch_seq(obj);
                break;
            }
            /* Intentionally left no break to switch IO to 3.3V as it is done for CYHAL_SDHC_IO_VOLT_ACTION_NONE */
            // fall through

        case CYHAL_SDHC_IO_VOLT_ACTION_NONE:
            Cy_SD_Host_ChangeIoVoltage(obj->base, (cy_en_sd_host_io_voltage_t)io_voltage);
            break;

        default:
            /* Illegal io_switch_type value provided */
            CY_ASSERT(false);
    }

    return result;
}

cyhal_sdhc_io_voltage_t cyhal_sdhc_get_io_voltage(cyhal_sdhc_t *obj)
{
    CY_ASSERT(NULL != obj);
    return obj->low_voltage_io_set ? CYHAL_SDHC_IO_VOLTAGE_1_8V : CYHAL_SDHC_IO_VOLTAGE_3_3V;;
}

static inline bool _cyhal_sdhc_is_buswidth_correct(uint8_t sd_data_bits)
{
    return ((1 == sd_data_bits) || (4 == sd_data_bits) || (8 == sd_data_bits));
}

cy_rslt_t cyhal_sdhc_set_bus_width(cyhal_sdhc_t *obj, uint8_t bus_width, bool configure_card)
{
    CY_ASSERT(NULL != obj);
    CY_ASSERT(NULL != obj->base);

    if (!_cyhal_sdhc_is_buswidth_correct(bus_width))
    {
        return CYHAL_SDHC_RSLT_ERR_WRONG_PARAM;
    }

    cy_rslt_t result;

    if (configure_card)
    {
        result = (cy_rslt_t)Cy_SD_Host_SetBusWidth(obj->base, _cyhal_sdhc_convert_buswidth(bus_width), &obj->context);
    }
    else
    {
        result = (cy_rslt_t)Cy_SD_Host_SetHostBusWidth(obj->base, _cyhal_sdhc_convert_buswidth(bus_width));
    }

    if (CY_RSLT_SUCCESS == result)
    {
        obj->bus_width = bus_width;
    }

    return result;
}

uint8_t cyhal_sdhc_get_bus_width(cyhal_sdhc_t *obj)
{
    CY_ASSERT(NULL != obj);
    return obj->bus_width;
}

void cyhal_sdhc_clear_errors(cyhal_sdhc_t *obj)
{
    CY_ASSERT(NULL != obj);
    CY_ASSERT(NULL != obj->base);
    Cy_SD_Host_ClearErrorInterruptStatus(obj->base, _CYHAL_SDHC_ALL_ERR_INTERRUPTS);
}

cyhal_sdhc_error_type_t cyhal_sdhc_get_last_command_errors(cyhal_sdhc_t *obj)
{
    CY_ASSERT(NULL != obj);
    CY_ASSERT(NULL != obj->base);
    return (cyhal_sdhc_error_type_t)Cy_SD_Host_GetErrorInterruptStatus(obj->base);
}

void cyhal_sdhc_software_reset(cyhal_sdhc_t *obj)
{
    CY_ASSERT(NULL != obj);
    CY_ASSERT(NULL != obj->base);
    _cyhal_sdxx_reset(obj->base);
}

cy_rslt_t cyhal_sdhc_enable_card_power(cyhal_sdhc_t *obj, bool enable)
{
    CY_ASSERT(NULL != obj);
    CY_ASSERT(NULL != obj->base);

    if (NC == obj->pin_card_pwr_en)
    {
        /* Need to have card_pwr_en pin configured in order to switch card power state */
        return CYHAL_SDHC_RSLT_ERR_PIN;
    }

    if (enable)
    {
        Cy_SD_Host_EnableCardVoltage(obj->base);
    }
    else
    {
        Cy_SD_Host_DisableCardVoltage(obj->base);
    }
    return CY_RSLT_SUCCESS;
}

cy_rslt_t cyhal_sdio_init(cyhal_sdio_t *obj, cyhal_gpio_t cmd, cyhal_gpio_t clk, cyhal_gpio_t data0, cyhal_gpio_t data1,
        cyhal_gpio_t data2, cyhal_gpio_t data3)
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

    result = _cyhal_sdxx_setup_pin(obj, cmd, cyhal_pin_map_sdhc_card_cmd,
        _CYHAL_SDHC_ELEM_COUNT(cyhal_pin_map_sdhc_card_cmd), &(obj->pin_cmd), _CYHAL_SDHC_NOT_WEAK_FUNC);

    if (CY_RSLT_SUCCESS == result)
    {
        result = _cyhal_sdxx_setup_pin(obj, clk, cyhal_pin_map_sdhc_clk_card,
            _CYHAL_SDHC_ELEM_COUNT(cyhal_pin_map_sdhc_clk_card), &(obj->pin_clk), _CYHAL_SDHC_NOT_WEAK_FUNC);
    }

    if (CY_RSLT_SUCCESS == result)
    {
        result = _cyhal_sdxx_setup_pin(obj, data0, cyhal_pin_map_sdhc_card_dat_3to0,
            _CYHAL_SDHC_ELEM_COUNT(cyhal_pin_map_sdhc_card_dat_3to0), &(obj->pin_data0), _CYHAL_SDHC_NOT_WEAK_FUNC);
    }

    if (CY_RSLT_SUCCESS == result)
    {
        result = _cyhal_sdxx_setup_pin(obj, data1, cyhal_pin_map_sdhc_card_dat_3to0,
            _CYHAL_SDHC_ELEM_COUNT(cyhal_pin_map_sdhc_card_dat_3to0), &(obj->pin_data1), _CYHAL_SDHC_NOT_WEAK_FUNC);
    }

    if (CY_RSLT_SUCCESS == result)
    {
        result = _cyhal_sdxx_setup_pin(obj, data2, cyhal_pin_map_sdhc_card_dat_3to0,
            _CYHAL_SDHC_ELEM_COUNT(cyhal_pin_map_sdhc_card_dat_3to0), &(obj->pin_data2), _CYHAL_SDHC_NOT_WEAK_FUNC);
    }

    if (CY_RSLT_SUCCESS == result)
    {
        result = _cyhal_sdxx_setup_pin(obj, data3, cyhal_pin_map_sdhc_card_dat_3to0,
            _CYHAL_SDHC_ELEM_COUNT(cyhal_pin_map_sdhc_card_dat_3to0), &(obj->pin_data3), _CYHAL_SDHC_NOT_WEAK_FUNC);
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
            stored_objects[obj->resource.block_num] = (void *)obj;
            _cyhal_sdhc_data_transfer_status[obj->resource.block_num] = _CYHAL_SDHC_NOT_RUNNING;

            /* Enable the SDHC block */
            Cy_SD_Host_Enable(obj->base);

            hostConfig.dmaType = CY_SD_HOST_DMA_ADMA2;
            hostConfig.emmc = false;
            hostConfig.enableLedControl = false;

            /* Configure SD Host to operate */
            result = (cy_rslt_t)Cy_SD_Host_Init(obj->base, &hostConfig, &context);

            /* Register SDIO Deep Sleep Callback */
            if (CY_RSLT_SUCCESS == result)
            {
                context.cardType = CY_SD_HOST_SDIO;
                obj->context = context;
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

                #if defined(CY_RTOS_AWARE) || defined(COMPONENT_RTOS_AWARE)
                _cyhal_sdxx_sema_status[obj->resource.block_num] = _CYHAL_SDXX_SEMA_NOT_INITED;
                #endif /* defined(CY_RTOS_AWARE) || defined(COMPONENT_RTOS_AWARE) */

                IRQn_Type irqn = _CYHAL_SDHC_IRQ_N[obj->resource.block_num];
                cy_stc_sysint_t irqCfg = { irqn, CYHAL_ISR_PRIORITY_DEFAULT };
                Cy_SysInt_Init(&irqCfg, _cyhal_sdio_irq_handler);
                NVIC_EnableIRQ(irqn);

                result = (cy_rslt_t)Cy_SD_Host_SetHostBusWidth(obj->base, CY_SD_HOST_BUS_WIDTH_4_BIT);

                /* Change the host SD clock to 400 kHz */
                if (result == CY_RSLT_SUCCESS)
                {
                    uint32_t freq = _CYHAL_SDIO_HOST_CLK_400K;
                    result = _cyhal_sdxx_sdcardchangeclock(obj, true, &freq, true, false);
                    if (result == CY_RSLT_SUCCESS)
                    {
                        obj->frequencyhal_hz = freq;
                        obj->block_size = _CYHAL_SDIO_64B_BLOCK;
                    }
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

        #if defined(CY_RTOS_AWARE) || defined(COMPONENT_RTOS_AWARE)
        if (_CYHAL_SDXX_SEMA_NOT_INITED != _cyhal_sdxx_sema_status[obj->resource.block_num])
        {
            cy_rtos_deinit_semaphore(&_cyhal_sdhc_smphr_xfer_done[obj->resource.block_num]);
            _cyhal_sdxx_sema_status[obj->resource.block_num] = _CYHAL_SDXX_SEMA_NOT_INITED;
        }
        #endif /* CY_RTOS_AWARE or COMPONENT_RTOS_AWARE defined */

        Cy_SD_Host_DeInit(obj->base);
        stored_objects[obj->resource.block_num] = NULL;
        _cyhal_sdhc_data_transfer_status[obj->resource.block_num] = _CYHAL_SDHC_NOT_RUNNING;

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
    cy_rslt_t result = CYHAL_SDIO_RSLT_ERR_CONFIG;

    if ((NULL == obj) || (config == NULL))
    {
        return CYHAL_SDIO_RSLT_ERR_BAD_PARAM;
    }

    if (config->frequencyhal_hz != 0U)
    {
        uint32_t freq = config->frequencyhal_hz;
        result = _cyhal_sdxx_sdcardchangeclock(obj, true, &freq, true, false);
        if (CY_RSLT_SUCCESS == result)
        {
            obj->frequencyhal_hz = freq;
        }
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

    cy_rslt_t                   ret;
    cy_stc_sd_host_cmd_config_t cmd;
    uint32_t                    retry = _CYHAL_SDIO_TRANSFER_TRIES;

    /* Clear out the response */
    if ( response != NULL )
    {
        *response = 0UL;
    }

    do
    {
        /* First clear out the command complete and transfer complete statuses */
        Cy_SD_Host_ClearNormalInterruptStatus(obj->base, CY_SD_HOST_CMD_COMPLETE);

        /* Check if an error occurred on any previous transactions */
        if ( Cy_SD_Host_GetNormalInterruptStatus(obj->base) & CY_SD_HOST_ERR_INTERRUPT )
        {
            /* Reset the block if there was an error. Note a full reset usually
             * requires more time, but this short version is working quite well and
             * successfully clears out the error state.
             */
            Cy_SD_Host_ClearErrorInterruptStatus(obj->base, _CYHAL_SDIO_SET_ALL_INTERRUPTS_MASK);
            _cyhal_sdxx_reset(obj->base);
        }

        cmd.commandIndex                 = (uint32_t)command;
        cmd.commandArgument              = argument;
        cmd.enableCrcCheck               = true;
        cmd.enableAutoResponseErrorCheck = false;
        cmd.respType                     = CY_SD_HOST_RESPONSE_LEN_48;
        cmd.enableIdxCheck               = true;
        cmd.dataPresent                  = false;
        cmd.cmdType                      = CY_SD_HOST_CMD_NORMAL;

        if (obj->irq_cause & CYHAL_SDIO_CMD_COMPLETE)
        {
            /*  Enabling command complete interrupt mask if corresponding event was enabled by user
            *   _cyhal_sdio_irq_handler will disable CY_SD_HOST_CMD_COMPLETE mask once interrupt
            *   is generated. */
            Cy_SD_Host_SetNormalInterruptMask(obj->base, Cy_SD_Host_GetNormalInterruptMask(obj->base) |
                    CY_SD_HOST_CMD_COMPLETE);
        }

        ret = (cy_rslt_t)Cy_SD_Host_SendCommand(obj->base, &cmd);

        if (CY_RSLT_SUCCESS == ret)
        {
            ret = (cy_rslt_t)_cyhal_sdxx_pollcmdcomplete(obj->base, obj->resource.block_num);
        }
    } while ((CY_RSLT_SUCCESS != ret) && (retry-- > 0UL));

    if (CY_RSLT_SUCCESS == ret)
    {
        ret = (cy_rslt_t)Cy_SD_Host_GetResponse(obj->base, response, false);
    }

    return ret;
}

cy_rslt_t cyhal_sdio_bulk_transfer(cyhal_sdio_t *obj, cyhal_transfer_t direction,
                                   uint32_t argument, const uint32_t* data,
                                   uint16_t length, uint32_t* response)
{
    cy_rslt_t ret = CY_RSLT_SUCCESS;
    uint32_t retry = _CYHAL_SDIO_TRANSFER_TRIES;
    _cyhal_sdxx_setup_smphr(obj->resource.block_num);

    do
    {
        ret = cyhal_sdio_transfer_async(obj, direction, argument, data, length);

        if (CY_RSLT_SUCCESS == ret)
        {
            ret = _cyhal_sdxx_waitfor_transfer_complete(obj->resource.block_num, obj->base, true);
        }

        if (CY_RSLT_SUCCESS != ret)
        {
            /*  SDIO Error Handling
            *   SDIO write timeout is expected when doing first write to register
            *   after KSO bit disable (as it goes to AOS core).
            *   This is the only time known that a write timeout occurs.
            *   Issue the reset to recover from error. */
            _cyhal_sdxx_reset(obj->base);
        }

    } while ((CY_RSLT_SUCCESS != ret) && (--retry > 0UL));

    if(CY_RSLT_SUCCESS != ret)
    {
        _cyhal_sdhc_data_transfer_status[obj->resource.block_num] = _CYHAL_SDHC_NOT_RUNNING;
    }

    if ((response != NULL) && (CY_RSLT_SUCCESS == ret))
    {
        *response = 0UL;
        ret = (cy_rslt_t)Cy_SD_Host_GetResponse(obj->base, response, false);
    }

    return ret;
}

/*******************************************************************************
*
*   The asynchronous transfer is implemented on the CY_SD_HOST_XFER_COMPLETE
*   interrupt. The function sets up data and enables the CY_SD_HOST_XFER_COMPLETE
*   interrupt mask, which causes interrupt to occur and handled by _cyhal_sdio_irq_handler
*   which take care of disabling CY_SD_HOST_XFER_COMPLETE mask. This function
*   can also activate CY_SD_HOST_CMD_COMPLETE interrupt mask if it was enabled
*   by user via cyhal_sdio_enable_event function.
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

    cy_rslt_t                    ret;
    uint32_t                     retry = _CYHAL_SDIO_TRANSFER_TRIES;
    cy_stc_sd_host_cmd_config_t  cmd;
    cy_stc_sd_host_data_config_t dat;

    /* Initialize data constants*/
    dat.autoCommand         = CY_SD_HOST_AUTO_CMD_NONE;
    dat.dataTimeout         = 0x0dUL;
    dat.enableIntAtBlockGap = false;
    dat.enReliableWrite     = false;
    dat.enableDma           = true;

    do
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

        /* Check if an error occurred on any previous transactions or reset after the first unsuccessful transfer try */
        if ((Cy_SD_Host_GetNormalInterruptStatus(obj->base) & CY_SD_HOST_ERR_INTERRUPT) ||
            (retry < _CYHAL_SDIO_TRANSFER_TRIES))
        {
            /* Reset the block if there was an error. Note a full reset usually
             * requires more time, but this short version is working quite well and
             * successfully clears out the error state.
             */
            Cy_SD_Host_ClearErrorInterruptStatus(obj->base, _CYHAL_SDIO_SET_ALL_INTERRUPTS_MASK);
            _cyhal_sdxx_reset(obj->base);
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

        length = dat.blockSize * dat.numberOfBlock;

        obj->adma_descriptor_tbl[0] = (1UL << CY_SD_HOST_ADMA_ATTR_VALID_POS) | /* Attr Valid */
                                     (1UL << CY_SD_HOST_ADMA_ATTR_END_POS) |   /* Attr End */
                                     (0UL << CY_SD_HOST_ADMA_ATTR_INT_POS) |   /* Attr Int */
                                     (CY_SD_HOST_ADMA_TRAN << CY_SD_HOST_ADMA_ACT_POS) |
                                     (length << CY_SD_HOST_ADMA_LEN_POS);     /* Len */

        obj->adma_descriptor_tbl[1] = (uint32_t)data;

        /* The address of the ADMA descriptor table. */
        dat.data = (uint32_t*)&obj->adma_descriptor_tbl[0];

        ret = _cyhal_sdxx_prepare_for_transfer(obj->base, true);

        if (CY_RSLT_SUCCESS == ret)
        {
            ret = (cy_rslt_t)Cy_SD_Host_InitDataTransfer(obj->base, &dat);
        }

        if (CY_RSLT_SUCCESS == ret)
        {
            /* Indicate that async transfer in progress */
            _cyhal_sdhc_data_transfer_status[obj->resource.block_num] = _CYHAL_SDHC_WAIT_BOTH;
            ret = (cy_rslt_t)Cy_SD_Host_SendCommand(obj->base, &cmd);
        }

        if (CY_RSLT_SUCCESS == ret)
        {
            ret = (cy_rslt_t)_cyhal_sdxx_pollcmdcomplete(obj->base, obj->resource.block_num);
        }

    } while ((CY_RSLT_SUCCESS != ret) && (--retry > 0UL));

    if (CY_RSLT_SUCCESS != ret)
    {
        /* Transfer failed */
        _cyhal_sdhc_data_transfer_status[obj->resource.block_num] = _CYHAL_SDHC_NOT_RUNNING;
    }

    return ret;
}

bool cyhal_sdio_is_busy(const cyhal_sdio_t *obj)
{
    CY_ASSERT(NULL != obj);
    return (_CYHAL_SDHC_NOT_RUNNING != _cyhal_sdhc_data_transfer_status[obj->resource.block_num]);
}

cy_rslt_t cyhal_sdio_abort_async(const cyhal_sdio_t *obj)
{
    cy_rslt_t ret = CY_RSLT_SUCCESS;

    /* To abort transition reset dat and cmd lines (software reset) */
    _cyhal_sdxx_reset(obj->base);
    _cyhal_sdhc_data_transfer_status[obj->resource.block_num] = _CYHAL_SDHC_NOT_RUNNING;
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

        /*  CY_SD_HOST_CMD_COMPLETE and CY_SD_HOST_XFER_COMPLETE cannot be always enabled because of SD Host driver limitations.
        *   SDHC HAL transfer APIs are taking care of enabling these statuses. CY_SD_HOST_CMD_COMPLETE is only
        *   enabled if corresponding user callback is enabled while CY_SD_HOST_XFER_COMPLETE is enabled by transfer API
        *   regardless it was enabled by user or not. */
        interruptMask &= ((uint32_t) ~CY_SD_HOST_CMD_COMPLETE) & ((uint32_t) ~CY_SD_HOST_XFER_COMPLETE);

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
