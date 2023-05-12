/*
 * Copyright 2023 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _CLOCK_CONFIG_H_
#define _CLOCK_CONFIG_H_

#include "fsl_common.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define BOARD_XTAL0_CLK_HZ                         24000000U  /*!< Board xtal0 frequency in Hz */

#define BOARD_XTAL32K_CLK_HZ                          32768U  /*!< Board xtal32k frequency in Hz */
/*******************************************************************************
 ************************ BOARD_InitBootClocks function ************************
 ******************************************************************************/

#if defined(__cplusplus)
extern "C" {
#endif /* __cplusplus*/

/*!
 * @brief This function executes default configuration of clocks.
 *
 */
void BOARD_InitBootClocks(void);

#if defined(__cplusplus)
}
#endif /* __cplusplus*/

/*******************************************************************************
 ********************* Configuration BOARD_ClockFullSpeed **********************
 ******************************************************************************/
/*******************************************************************************
 * Definitions for BOARD_ClockFullSpeed configuration
 ******************************************************************************/
#define BOARD_CLOCKFULLSPEED_CORE_CLOCK           528000000U  /*!< Core clock frequency: 528000000Hz */

/* Clock outputs (values are in Hz): */
#define BOARD_CLOCKFULLSPEED_AHB_CLK_ROOT             528000000UL
#define BOARD_CLOCKFULLSPEED_CAN_CLK_ROOT             40000000UL
#define BOARD_CLOCKFULLSPEED_CKIL_SYNC_CLK_ROOT       32768UL
#define BOARD_CLOCKFULLSPEED_CLKO1_CLK                0UL
#define BOARD_CLOCKFULLSPEED_CLKO2_CLK                0UL
#define BOARD_CLOCKFULLSPEED_CLK_1M                   1000000UL
#define BOARD_CLOCKFULLSPEED_CLK_24M                  24000000UL
#define BOARD_CLOCKFULLSPEED_CSI_CLK_ROOT             12000000UL
#define BOARD_CLOCKFULLSPEED_ENET_125M_CLK            2400000UL
#define BOARD_CLOCKFULLSPEED_ENET_25M_REF_CLK         1200000UL
#define BOARD_CLOCKFULLSPEED_ENET_REF_CLK             0UL
#define BOARD_CLOCKFULLSPEED_ENET_TX_CLK              0UL
#define BOARD_CLOCKFULLSPEED_FLEXIO1_CLK_ROOT         30000000UL
#define BOARD_CLOCKFULLSPEED_FLEXIO2_CLK_ROOT         30000000UL
#define BOARD_CLOCKFULLSPEED_FLEXSPI_CLK_ROOT         160000000UL
#define BOARD_CLOCKFULLSPEED_GPT1_IPG_CLK_HIGHFREQ    24000000UL
#define BOARD_CLOCKFULLSPEED_GPT2_IPG_CLK_HIGHFREQ    24000000UL
#define BOARD_CLOCKFULLSPEED_IPG_CLK_ROOT             132000000UL
#define BOARD_CLOCKFULLSPEED_LCDIF_CLK_ROOT           67500000UL
#define BOARD_CLOCKFULLSPEED_LPI2C_CLK_ROOT           10000000UL
#define BOARD_CLOCKFULLSPEED_LPSPI_CLK_ROOT           37402597UL
#define BOARD_CLOCKFULLSPEED_LVDS1_CLK                1056000000UL
#define BOARD_CLOCKFULLSPEED_MQS_MCLK                 63529411UL
#define BOARD_CLOCKFULLSPEED_PERCLK_CLK_ROOT          24000000UL
#define BOARD_CLOCKFULLSPEED_PLL7_MAIN_CLK            24000000UL
#define BOARD_CLOCKFULLSPEED_SAI1_CLK_ROOT            63529411UL
#define BOARD_CLOCKFULLSPEED_SAI1_MCLK1               63529411UL
#define BOARD_CLOCKFULLSPEED_SAI1_MCLK2               63529411UL
#define BOARD_CLOCKFULLSPEED_SAI1_MCLK3               30000000UL
#define BOARD_CLOCKFULLSPEED_SAI2_CLK_ROOT            63529411UL
#define BOARD_CLOCKFULLSPEED_SAI2_MCLK1               63529411UL
#define BOARD_CLOCKFULLSPEED_SAI2_MCLK2               0UL
#define BOARD_CLOCKFULLSPEED_SAI2_MCLK3               30000000UL
#define BOARD_CLOCKFULLSPEED_SAI3_CLK_ROOT            63529411UL
#define BOARD_CLOCKFULLSPEED_SAI3_MCLK1               63529411UL
#define BOARD_CLOCKFULLSPEED_SAI3_MCLK2               0UL
#define BOARD_CLOCKFULLSPEED_SAI3_MCLK3               30000000UL
#define BOARD_CLOCKFULLSPEED_SEMC_CLK_ROOT            66000000UL
#define BOARD_CLOCKFULLSPEED_SPDIF0_CLK_ROOT          30000000UL
#define BOARD_CLOCKFULLSPEED_SPDIF0_EXTCLK_OUT        0UL
#define BOARD_CLOCKFULLSPEED_TRACE_CLK_ROOT           117333333UL
#define BOARD_CLOCKFULLSPEED_UART_CLK_ROOT            24000000UL
#define BOARD_CLOCKFULLSPEED_USBPHY1_CLK              0UL
#define BOARD_CLOCKFULLSPEED_USBPHY2_CLK              0UL
#define BOARD_CLOCKFULLSPEED_USDHC1_CLK_ROOT          198000000UL
#define BOARD_CLOCKFULLSPEED_USDHC2_CLK_ROOT          198000000UL

/*! @brief Arm PLL set for BOARD_ClockFullSpeed configuration.
 */
extern const clock_arm_pll_config_t armPllConfig_BOARD_ClockFullSpeed;
/*! @brief Usb1 PLL set for BOARD_ClockFullSpeed configuration.
 */
extern const clock_usb_pll_config_t usb1PllConfig_BOARD_ClockFullSpeed;
/*! @brief Sys PLL for BOARD_ClockFullSpeed configuration.
 */
extern const clock_sys_pll_config_t sysPllConfig_BOARD_ClockFullSpeed;

/*******************************************************************************
 * API for BOARD_ClockFullSpeed configuration
 ******************************************************************************/
#if defined(__cplusplus)
extern "C" {
#endif /* __cplusplus*/

/*!
 * @brief This function executes configuration of clocks.
 *
 */
void BOARD_ClockFullSpeed(void);

#if defined(__cplusplus)
}
#endif /* __cplusplus*/

/*******************************************************************************
 ********************* Configuration BOARD_ClockOverdrive **********************
 ******************************************************************************/
/*******************************************************************************
 * Definitions for BOARD_ClockOverdrive configuration
 ******************************************************************************/
#define BOARD_CLOCKOVERDRIVE_CORE_CLOCK           600000000U  /*!< Core clock frequency: 600000000Hz */

/* Clock outputs (values are in Hz): */
#define BOARD_CLOCKOVERDRIVE_AHB_CLK_ROOT             600000000UL
#define BOARD_CLOCKOVERDRIVE_CAN_CLK_ROOT             40000000UL
#define BOARD_CLOCKOVERDRIVE_CKIL_SYNC_CLK_ROOT       32768UL
#define BOARD_CLOCKOVERDRIVE_CLKO1_CLK                0UL
#define BOARD_CLOCKOVERDRIVE_CLKO2_CLK                0UL
#define BOARD_CLOCKOVERDRIVE_CLK_1M                   1000000UL
#define BOARD_CLOCKOVERDRIVE_CLK_24M                  24000000UL
#define BOARD_CLOCKOVERDRIVE_CSI_CLK_ROOT             12000000UL
#define BOARD_CLOCKOVERDRIVE_ENET_125M_CLK            2400000UL
#define BOARD_CLOCKOVERDRIVE_ENET_25M_REF_CLK         1200000UL
#define BOARD_CLOCKOVERDRIVE_ENET_REF_CLK             0UL
#define BOARD_CLOCKOVERDRIVE_ENET_TX_CLK              0UL
#define BOARD_CLOCKOVERDRIVE_FLEXIO1_CLK_ROOT         30000000UL
#define BOARD_CLOCKOVERDRIVE_FLEXIO2_CLK_ROOT         30000000UL
#define BOARD_CLOCKOVERDRIVE_FLEXSPI_CLK_ROOT         160000000UL
#define BOARD_CLOCKOVERDRIVE_GPT1_IPG_CLK_HIGHFREQ    24000000UL
#define BOARD_CLOCKOVERDRIVE_GPT2_IPG_CLK_HIGHFREQ    24000000UL
#define BOARD_CLOCKOVERDRIVE_IPG_CLK_ROOT             150000000UL
#define BOARD_CLOCKOVERDRIVE_LCDIF_CLK_ROOT           67500000UL
#define BOARD_CLOCKOVERDRIVE_LPI2C_CLK_ROOT           10000000UL
#define BOARD_CLOCKOVERDRIVE_LPSPI_CLK_ROOT           37402597UL
#define BOARD_CLOCKOVERDRIVE_LVDS1_CLK                1200000000UL
#define BOARD_CLOCKOVERDRIVE_MQS_MCLK                 63529411UL
#define BOARD_CLOCKOVERDRIVE_PERCLK_CLK_ROOT          24000000UL
#define BOARD_CLOCKOVERDRIVE_PLL7_MAIN_CLK            24000000UL
#define BOARD_CLOCKOVERDRIVE_SAI1_CLK_ROOT            63529411UL
#define BOARD_CLOCKOVERDRIVE_SAI1_MCLK1               63529411UL
#define BOARD_CLOCKOVERDRIVE_SAI1_MCLK2               63529411UL
#define BOARD_CLOCKOVERDRIVE_SAI1_MCLK3               30000000UL
#define BOARD_CLOCKOVERDRIVE_SAI2_CLK_ROOT            63529411UL
#define BOARD_CLOCKOVERDRIVE_SAI2_MCLK1               63529411UL
#define BOARD_CLOCKOVERDRIVE_SAI2_MCLK2               0UL
#define BOARD_CLOCKOVERDRIVE_SAI2_MCLK3               30000000UL
#define BOARD_CLOCKOVERDRIVE_SAI3_CLK_ROOT            63529411UL
#define BOARD_CLOCKOVERDRIVE_SAI3_MCLK1               63529411UL
#define BOARD_CLOCKOVERDRIVE_SAI3_MCLK2               0UL
#define BOARD_CLOCKOVERDRIVE_SAI3_MCLK3               30000000UL
#define BOARD_CLOCKOVERDRIVE_SEMC_CLK_ROOT            75000000UL
#define BOARD_CLOCKOVERDRIVE_SPDIF0_CLK_ROOT          30000000UL
#define BOARD_CLOCKOVERDRIVE_SPDIF0_EXTCLK_OUT        0UL
#define BOARD_CLOCKOVERDRIVE_TRACE_CLK_ROOT           117333333UL
#define BOARD_CLOCKOVERDRIVE_UART_CLK_ROOT            24000000UL
#define BOARD_CLOCKOVERDRIVE_USBPHY1_CLK              0UL
#define BOARD_CLOCKOVERDRIVE_USBPHY2_CLK              0UL
#define BOARD_CLOCKOVERDRIVE_USDHC1_CLK_ROOT          198000000UL
#define BOARD_CLOCKOVERDRIVE_USDHC2_CLK_ROOT          198000000UL

/*! @brief Arm PLL set for BOARD_ClockOverdrive configuration.
 */
extern const clock_arm_pll_config_t armPllConfig_BOARD_ClockOverdrive;
/*! @brief Usb1 PLL set for BOARD_ClockOverdrive configuration.
 */
extern const clock_usb_pll_config_t usb1PllConfig_BOARD_ClockOverdrive;
/*! @brief Sys PLL for BOARD_ClockOverdrive configuration.
 */
extern const clock_sys_pll_config_t sysPllConfig_BOARD_ClockOverdrive;

/*******************************************************************************
 * API for BOARD_ClockOverdrive configuration
 ******************************************************************************/
#if defined(__cplusplus)
extern "C" {
#endif /* __cplusplus*/

/*!
 * @brief This function executes configuration of clocks.
 *
 */
void BOARD_ClockOverdrive(void);

#if defined(__cplusplus)
}
#endif /* __cplusplus*/

/*******************************************************************************
 ********************* Configuration BOARD_ClockLowPower ***********************
 ******************************************************************************/
/*******************************************************************************
 * Definitions for BOARD_ClockLowPower configuration
 ******************************************************************************/
#define BOARD_CLOCKLOWPOWER_CORE_CLOCK             24000000U  /*!< Core clock frequency: 24000000Hz */

/* Clock outputs (values are in Hz): */
#define BOARD_CLOCKLOWPOWER_AHB_CLK_ROOT              24000000UL
#define BOARD_CLOCKLOWPOWER_CAN_CLK_ROOT              2000000UL
#define BOARD_CLOCKLOWPOWER_CKIL_SYNC_CLK_ROOT        0UL
#define BOARD_CLOCKLOWPOWER_CLKO1_CLK                 0UL
#define BOARD_CLOCKLOWPOWER_CLKO2_CLK                 0UL
#define BOARD_CLOCKLOWPOWER_CLK_1M                    1000000UL
#define BOARD_CLOCKLOWPOWER_CLK_24M                   24000000UL
#define BOARD_CLOCKLOWPOWER_CSI_CLK_ROOT              12000000UL
#define BOARD_CLOCKLOWPOWER_ENET_125M_CLK             2400000UL
#define BOARD_CLOCKLOWPOWER_ENET_25M_REF_CLK          1200000UL
#define BOARD_CLOCKLOWPOWER_ENET_REF_CLK              0UL
#define BOARD_CLOCKLOWPOWER_ENET_TX_CLK               0UL
#define BOARD_CLOCKLOWPOWER_FLEXIO1_CLK_ROOT          1500000UL
#define BOARD_CLOCKLOWPOWER_FLEXIO2_CLK_ROOT          1500000UL
#define BOARD_CLOCKLOWPOWER_FLEXSPI_CLK_ROOT          24000000UL
#define BOARD_CLOCKLOWPOWER_GPT1_IPG_CLK_HIGHFREQ     24000000UL
#define BOARD_CLOCKLOWPOWER_GPT2_IPG_CLK_HIGHFREQ     24000000UL
#define BOARD_CLOCKLOWPOWER_IPG_CLK_ROOT              12000000UL
#define BOARD_CLOCKLOWPOWER_LCDIF_CLK_ROOT            3000000UL
#define BOARD_CLOCKLOWPOWER_LPI2C_CLK_ROOT            3000000UL
#define BOARD_CLOCKLOWPOWER_LPSPI_CLK_ROOT            3000000UL
#define BOARD_CLOCKLOWPOWER_LVDS1_CLK                 24000000UL
#define BOARD_CLOCKLOWPOWER_MQS_MCLK                  3000000UL
#define BOARD_CLOCKLOWPOWER_PERCLK_CLK_ROOT           24000000UL
#define BOARD_CLOCKLOWPOWER_PLL7_MAIN_CLK             24000000UL
#define BOARD_CLOCKLOWPOWER_SAI1_CLK_ROOT             3000000UL
#define BOARD_CLOCKLOWPOWER_SAI1_MCLK1                3000000UL
#define BOARD_CLOCKLOWPOWER_SAI1_MCLK2                3000000UL
#define BOARD_CLOCKLOWPOWER_SAI1_MCLK3                1500000UL
#define BOARD_CLOCKLOWPOWER_SAI2_CLK_ROOT             3000000UL
#define BOARD_CLOCKLOWPOWER_SAI2_MCLK1                3000000UL
#define BOARD_CLOCKLOWPOWER_SAI2_MCLK2                0UL
#define BOARD_CLOCKLOWPOWER_SAI2_MCLK3                1500000UL
#define BOARD_CLOCKLOWPOWER_SAI3_CLK_ROOT             3000000UL
#define BOARD_CLOCKLOWPOWER_SAI3_MCLK1                3000000UL
#define BOARD_CLOCKLOWPOWER_SAI3_MCLK2                0UL
#define BOARD_CLOCKLOWPOWER_SAI3_MCLK3                1500000UL
#define BOARD_CLOCKLOWPOWER_SEMC_CLK_ROOT             24000000UL
#define BOARD_CLOCKLOWPOWER_SPDIF0_CLK_ROOT           1500000UL
#define BOARD_CLOCKLOWPOWER_SPDIF0_EXTCLK_OUT         0UL
#define BOARD_CLOCKLOWPOWER_TRACE_CLK_ROOT            6000000UL
#define BOARD_CLOCKLOWPOWER_UART_CLK_ROOT             24000000UL
#define BOARD_CLOCKLOWPOWER_USBPHY1_CLK               0UL
#define BOARD_CLOCKLOWPOWER_USBPHY2_CLK               0UL
#define BOARD_CLOCKLOWPOWER_USDHC1_CLK_ROOT           12000000UL
#define BOARD_CLOCKLOWPOWER_USDHC2_CLK_ROOT           12000000UL


/*******************************************************************************
 * API for BOARD_ClockLowPower configuration
 ******************************************************************************/
#if defined(__cplusplus)
extern "C" {
#endif /* __cplusplus*/

/*!
 * @brief This function executes configuration of clocks.
 *
 */
void BOARD_ClockLowPower(void);

#if defined(__cplusplus)
}
#endif /* __cplusplus*/

#endif /* _CLOCK_CONFIG_H_ */

