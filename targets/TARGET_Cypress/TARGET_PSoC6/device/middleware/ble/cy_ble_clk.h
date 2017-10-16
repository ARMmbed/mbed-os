/***************************************************************************//**
* \file cy_ble_clk.h
* \version 2.0
*
* \brief
*  Contains the function prototypes and constants for the BLE clock.
*
********************************************************************************
* \copyright
* Copyright 2017, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#ifndef CY_BLE_CLK_H
#define CY_BLE_CLK_H

#include "cy_device_headers.h"
#include "gpio/cy_gpio.h"
#include "syslib/cy_syslib.h"
#include <stddef.h>

/* C binding of definitions if building with C++ compiler */
#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#define CY_BLE_CLK_ID                                        (0x05ul << 18u)

#define CY_BLE_PORT2_CFG_VAL                                 (0x66666666u)
#define CY_BLE_PORT3_CFG_VAL                                 (0x66EEE666u)
#define CY_BLE_PORT4_CFG_VAL                                 (0x6666E666u)

#define CY_BLE_PORT_CFG_OUT_VAL                              (0xFFFF0000u)

#define CY_BLE_PORT2_HSIOM_SEL0                              (0x1C1C1C1Cu)
#define CY_BLE_PORT2_HSIOM_SEL1                              (0x1C1C1C1Cu)
#define CY_BLE_PORT3_HSIOM_SEL0                              (0x1A1A1A1Cu)
#define CY_BLE_PORT3_HSIOM_SEL1                              (0x00001A1Au)
#define CY_BLE_PORT4_HSIOM_SEL0                              (0x1C001A1Au)
#define CY_BLE_PORT4_HSIOM_SEL1                              (0x00000000u)

#define CY_BLE_DEFAULT_OSC_STARTUP_DELAY_LF                  (25u)
#define CY_BLE_DEFAULT_CAP_TRIM_VALUE                        (32u)
#define CY_BLE_DEFAULT_ECO_FREQ                              (CY_BLE_BLESS_ECO_FREQ_32MHZ)
#define CY_BLE_DEFAULT_ECO_DIV                               (CY_BLE_SYS_ECO_CLK_DIV_4)

#define CY_BLE_DEFAULT_HVLDO_STARTUP_DELAY                   (6UL)
#define CY_BLE_DEFAULT_ISOLATE_DEASSERT_DELAY                (0UL)
#define CY_BLE_DEFAULT_ACT_TO_SWITCH_DELAY                   (0UL)
#define CY_BLE_DEFAULT_HVLDO_DISABLE_DELAY                   (1UL)

#define CY_BLE_DEFAULT_ACT_STARTUP_DELAY                     (0UL)
#define CY_BLE_DEFAULT_DIG_LDO_STARTUP_DELAY                 (0UL)

#define CY_BLE_DEFAULT_XTAL_DISABLE_DELAY                    (1UL)
#define CY_BLE_DEFAULT_DIG_LDO_DISABLE_DELAY                 (0UL)
#define CY_BLE_DEFAULT_VDDR_STABLE_DELAY                     (1UL)

#define CY_BLE_DEFAULT_RCB_CTRL_LEAD                         (0x2UL)
#define CY_BLE_DEFAULT_RCB_CTRL_LAG                          (0x2UL)
#define CY_BLE_DEFAULT_RCB_CTRL_DIV                          (0x1UL)     /* LL 8 MHz / 2 */
#define CY_BLE_DEFAULT_RCB_CTRL_FREQ                         (4000000UL) /* Default RCB clock is 4 MHz */

#define CY_BLE_DEFAULT_ECO_CLK_FREQ_32MHZ                    (32000000UL)
#define CY_BLE_DEFAULT_ECO_CLK_FREQ_16MHZ                    (16000000UL)

#define CY_BLE_MXD_RADIO_CLK_BUF_EN_VAL                      (CY_BLE_PMU_MODE_TRANSITION_REG_CLK_ANA_DIG_EN_BIT | \
                                                              CY_BLE_PMU_MODE_TRANSITION_REG_RST_ACT_N_BIT)
#define CY_BLE_MXD_RADIO_DIG_CLK_OUT_EN_VAL                  (CY_BLE_PMU_PMU_CTRL_REG_CLK_CMOS_SEL_BIT)

/* Radio registers */
#define CY_BLE_PMU_MODE_TRANSITION_REG                       (0x1e02u)
#define CY_BLE_PMU_MODE_TRANSITION_REG_CLK_ANA_DIG_EN_BIT    (uint16_t)(1UL << 12u)
#define CY_BLE_PMU_MODE_TRANSITION_REG_RST_ACT_N_BIT         (uint16_t)(1UL << 11u)

#define CY_BLE_PMU_PMU_CTRL_REG                              (0x1e03u)
#define CY_BLE_PMU_PMU_CTRL_REG_CLK_CMOS_SEL_BIT             (uint16_t)(1UL << 10u)

#define CY_BLE_RF_DCXO_CFG_REG                               (0x1e08u)
#define CY_BLE_RF_DCXO_CFG_REG_DCXO_CAP_SHIFT                (1u)
#define CY_BLE_RF_DCXO_CFG_REG_DCXO_CAP_MASK                 (0xffUL)
#define CY_BLE_RF_DCXO_CFG_REG_VALUE                         (0x1001u)

#define CY_BLE_RF_DCXO_CFG2_REG                              (0x1e0fu)
#define CY_BLE_RF_DCXO_CFG2_REG_VALUE                        (0x6837u)

#define CY_BLE_RF_DCXO_BUF_CFG_REG                           (0x1e09u)
#define CY_BLE_RF_DCXO_BUF_CFG_REG_XTAL_32M_SEL_BIT          (uint16_t)(1UL << 6u)
#define CY_BLE_RF_DCXO_BUF_CFG_REG_BUF_AMP_SEL_SHIFT         (4u)
#define CY_BLE_RF_DCXO_BUF_CFG_REG_BUF_AMP_SEL_MASK          (0x03UL)
#define CY_BLE_RF_DCXO_BUF_CFG_REG_CLK_DIV_SHIFT             (0u)
#define CY_BLE_RF_DCXO_BUF_CFG_REG_CLK_DIV_MASK              (0x0fUL)

#define CY_BLE_DELAY_TIME                                    (1u) /* in us */
#define CY_BLE_RCB_TIMEOUT                                   (1000u / CY_BLE_DELAY_TIME)   /* 1ms */
#define CY_BLE_VIO_TIMEOUT                                   (2000u / CY_BLE_DELAY_TIME)   /* 2ms */
#define CY_BLE_ACT_TIMEOUT                                   (950000u / CY_BLE_DELAY_TIME) /* 950ms */
#define CY_BLE_RCB_RETRIES                                   (10u)



/**
 * \addtogroup group_ble_clock_api_data_types
 * @{
 */
/***************************************
* Data Types
***************************************/

/** BLE Radio ECO clock divider */
typedef enum
{
    CY_BLE_MXD_RADIO_CLK_DIV_1 = 0u,
    CY_BLE_MXD_RADIO_CLK_DIV_2 = 1u,
    CY_BLE_MXD_RADIO_CLK_DIV_4 = 2u,
    CY_BLE_MXD_RADIO_CLK_DIV_8 = 4u,
    CY_BLE_MXD_RADIO_CLK_DIV_16 = 8u
} cy_en_ble_mxd_radio_clk_div_t;

/** Sine wave buffer output capability select */
typedef enum
{
    CY_BLE_MXD_RADIO_CLK_BUF_AMP_16M_SMALL = 0u,
    CY_BLE_MXD_RADIO_CLK_BUF_AMP_16M_LARGE = 1u,
    CY_BLE_MXD_RADIO_CLK_BUF_AMP_32M_SMALL = 2u,
    CY_BLE_MXD_RADIO_CLK_BUF_AMP_32M_LARGE = 3u
} cy_en_ble_mxd_radio_clk_buf_amp_t;

/** BLESS clock divider */
typedef enum
{
    CY_BLE_BLESS_XTAL_CLK_DIV_1 = 0u,
    CY_BLE_BLESS_XTAL_CLK_DIV_2 = 1u,
    CY_BLE_BLESS_XTAL_CLK_DIV_4 = 2u,
    CY_BLE_BLESS_XTAL_CLK_DIV_8 = 3u
}cy_en_ble_bless_xtal_clk_div_config_llclk_div_t;

/** BLE ECO Clock Frequency. */
typedef enum
{
    /** ECO Frequency of 16MHz */
    CY_BLE_BLESS_ECO_FREQ_16MHZ,

    /** ECO Frequency of 32MHz */
    CY_BLE_BLESS_ECO_FREQ_32MHZ
} cy_en_ble_bless_eco_freq_t;

/** BLE ECO System clock divider */
typedef enum
{
    /** Link Layer clock divider = 1*/
    CY_BLE_SYS_ECO_CLK_DIV_1 = 0x00u,

    /** Link Layer clock divider = 2*/
    CY_BLE_SYS_ECO_CLK_DIV_2,

    /** Link Layer clock divider = 4*/
    CY_BLE_SYS_ECO_CLK_DIV_4,

    /** Link Layer clock divider = 8*/
    CY_BLE_SYS_ECO_CLK_DIV_8,

    /** Invalid Link Layer clock divider*/
    CY_BLE_SYS_ECO_CLK_DIV_INVALID
} cy_en_ble_bless_sys_eco_clk_div_t;

/** BLE ECO Clock return value */
typedef enum
{
    /** ECO started successfully */
    CY_BLE_ECO_SUCCESS = 0x00ul,

    /** Invalid input param values */
    CY_BLE_ECO_BAD_PARAM = CY_PDL_STATUS_ERROR | CY_BLE_CLK_ID | 0x0001ul,

    /** RCB is not available for Firmware control to restart ECO */
    CY_BLE_ECO_RCB_CONTROL_LL = CY_PDL_STATUS_ERROR | CY_BLE_CLK_ID | 0x0002ul,

    /** ECO already started */
    CY_BLE_ECO_ALREADY_STARTED = CY_PDL_STATUS_ERROR | CY_BLE_CLK_ID | 0x0003ul,
    
    /** Hardware error */
    CY_BLE_ECO_HARDWARE_ERROR = CY_PDL_STATUS_ERROR | CY_BLE_CLK_ID | 0x0004ul,
    
} cy_en_ble_eco_status_t;


/** BLE ECO configuration parameters */
typedef struct
{
    /** ECO crystal startup time in multiple of 31.25us (startup_time_from_user min - 31.25us)
     * ecoXtalStartUpTime = startup_time_from_user/31.25 */
    uint8_t ecoXtalStartUpTime;

    /** ECO crystal load capacitance - In multiple of 0.075pF (pF_from_user min - 7.5pF, pF_from_user max - 26.625pF)
     * loadcap = ((pF_from_user - 7.5)/0.075) */
    uint8_t                           loadCap;

    /** ECO Frequency. */
    cy_en_ble_bless_eco_freq_t        ecoFreq;

    /** System divider for ECO clock */
    cy_en_ble_bless_sys_eco_clk_div_t ecoSysDiv;
} cy_stc_ble_bless_eco_cfg_params_t;
/** @} */

/**
 * \addtogroup group_ble_clock_api
 * @{
 */

/***************************************
*   Function Prototypes
***************************************/
cy_en_ble_eco_status_t Cy_BLE_EcoStart(const cy_stc_ble_bless_eco_cfg_params_t *ecoConfig);
void Cy_BLE_EcoStop(void);

/** @} */


/***************************************
*   Private Function Prototypes
***************************************/
uint8_t Cy_BLE_HAL_IsEcoCpuClockSrc(void);
uint8_t Cy_BLE_HAL_IsWcoLfclkSrc(void);
uint32_t Cy_BLE_HAL_EcoGetFrequency(void);
void Cy_BLE_HAL_Init(void);


#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* CY_BLE_CLK_H */


/* [] END OF FILE */
