/*
 * Copyright (c) 2013 - 2014, Freescale Semiconductor, Inc.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * o Redistributions of source code must retain the above copyright notice, this list
 *   of conditions and the following disclaimer.
 *
 * o Redistributions in binary form must reproduce the above copyright notice, this
 *   list of conditions and the following disclaimer in the documentation and/or
 *   other materials provided with the distribution.
 *
 * o Neither the name of Freescale Semiconductor, Inc. nor the names of its
 *   contributors may be used to endorse or promote products derived from this
 *   software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#if !defined(__FSL_SMC_HAL_H__)
#define __FSL_SMC_HAL_H__

#include <stdint.h>
#include <stdbool.h>
#include <assert.h>
#include "fsl_device_registers.h"
#include "fsl_smc_features.h"

/*! @addtogroup smc_hal*/
/*! @{*/

/*! @file fsl_smc_hal.h */

/*******************************************************************************
 * Definitions
 ******************************************************************************/


/*! @brief Power Modes in PMSTAT*/
typedef enum _power_mode_stat {
    kStatRun    = 0x01,             /*!< 0000_0001 - Current power mode is RUN*/
    kStatStop   = 0x02,             /*!< 0000_0010 - Current power mode is STOP*/
    kStatVlpr   = 0x04,             /*!< 0000_0100 - Current power mode is VLPR*/
    kStatVlpw   = 0x08,             /*!< 0000_1000 - Current power mode is VLPW*/
    kStatVlps   = 0x10,             /*!< 0001_0000 - Current power mode is VLPS*/
    kStatLls    = 0x20,             /*!< 0010_0000 - Current power mode is LLS*/
    kStatVlls   = 0x40,             /*!< 0100_0000 - Current power mode is VLLS*/
    kStatHsrun  = 0x80              /*!< 1000_0000 - Current power mode is HSRUN*/
} power_mode_stat_t;

/*! @brief Power Modes Protection*/
typedef enum _power_modes_protect {
    kAllowHsrun,                    /*!< Allow High Speed Run mode*/
    kAllowVlp,                      /*!< Allow Very-Low-Power Modes*/
    kAllowLls,                      /*!< Allow Low-Leakage Stop Mode*/
    kAllowVlls,                     /*!< Allow Very-Low-Leakage Stop Mode*/
    kAllowMax
} power_modes_protect_t;

/*!
 * @brief Run mode definition
 */
typedef enum _smc_run_mode {
    kSmcRun,                                /*!< normal RUN mode*/
    kSmcReservedRun,
    kSmcVlpr,                               /*!< Very-Low-Power RUN mode*/
    kSmcHsrun                               /*!< High Speed Run mode (HSRUN)*/
} smc_run_mode_t;

/*!
 * @brief Stop mode definition
 */
typedef enum _smc_stop_mode {
    kSmcStop,                               /*!< Normal STOP mode*/
    kSmcReservedStop1,                      /*!< Reserved*/
    kSmcVlps,                               /*!< Very-Low-Power STOP mode*/
    kSmcLls,                                /*!< Low-Leakage Stop mode*/
    kSmcVlls                                /*!< Very-Low-Leakage Stop mode*/
} smc_stop_mode_t;

/*!
 * @brief VLLS/LLS stop sub mode definition
 */
typedef enum _smc_stop_submode {
    kSmcStopSub0,                               
    kSmcStopSub1,                               
    kSmcStopSub2,                               
    kSmcStopSub3                                
} smc_stop_submode_t;

/*! @brief Low Power Wake Up on Interrupt option*/
typedef enum _smc_lpwui_option {
    kSmcLpwuiEnabled,                        /*!< Low Power Wake Up on Interrupt enabled*/
    kSmcLpwuiDisabled                        /*!< Low Power Wake Up on Interrupt disabled*/
} smc_lpwui_option_t;

/*! @brief Partial STOP option*/
typedef enum _smc_pstop_option {
    kSmcPstopStop,                          /*!< STOP - Normal Stop mode*/
    kSmcPstopStop1,                         /*!< Partial Stop with both system and bus clocks disabled*/
    kSmcPstopStop2,                         /*!< Partial Stop with system clock disabled and bus clock enabled*/
    kSmcPstopReserved,
} smc_pstop_option_t;

/*! @brief POR option*/
typedef enum _smc_por_option {
    kSmcPorEnabled,                        /*!< POR detect circuit is enabled in VLLS0*/
    kSmcPorDisabled                        /*!< POR detect circuit is disabled in VLLS0*/
} smc_por_option_t;

/*! @brief LPO power option*/
typedef enum _smc_lpo_option {
    kSmcLpoEnabled,                        /*!< LPO clock is enabled in LLS/VLLSx*/
    kSmcLpoDisabled                        /*!< LPO clock is disabled in LLS/VLLSx*/
} smc_lpo_option_t;

/*! @brief Power mode control options*/
typedef enum _smc_power_options {
    kSmcOptionLpwui,                        /*!< Low Power Wake Up on Interrupt*/
    kSmcOptionPropo                         /*!< POR option*/
} smc_power_options_t;

/*! @brief Power mode protection configuration*/
typedef struct _smc_power_mode_protection_config {
    bool                vlpProt;            /*!< VLP protect*/
    bool                llsProt;            /*!< LLS protect */
    bool                vllsProt;           /*!< VLLS protect*/
} smc_power_mode_protection_config_t;

/*******************************************************************************
 * API
 ******************************************************************************/

#if defined(__cplusplus)
extern "C" {
#endif /* __cplusplus*/

/*! @name System mode controller APIs*/
/*@{*/

/*!
 * @brief Configures all power mode protection settings.
 *
 * This function  configures the power mode protection settings for
 * supported power modes in the specified chip family. The available power modes
 * are defined in the smc_power_mode_protection_config_t. An application should provide
 * the protect settings for all supported power modes on the chip. This
 * should be done at an early system level initialization stage. See the reference manual
 * for details. This register can only write once after the power reset. If the user has only a single option to set,
 * either use this function or use the individual set function.
 * 
 * 
 * @param protectConfig Configurations for the supported power mode protect settings
 *                      - See smc_power_mode_protection_config_t for details.
 */
void smc_hal_config_power_mode_protection(smc_power_mode_protection_config_t *protectConfig);

/*!
 * @brief Configures the individual power mode protection settings.
 *
 * This function  only configures the power mode protection settings for
 * a specified power mode on the specified chip family. The available power modes
 * are defined in the smc_power_mode_protection_config_t. See the reference manual
 * for details. This register can only write once after the power reset.
 *
 * @param protect Power mode to set for protection
 * @param allow   Allow or not allow the power mode protection
 */
void smc_hal_set_power_mode_protection(power_modes_protect_t protect, bool allow);

/*!
 * @brief Gets the the current power mode protection setting.
 *
 * This function  gets the current power mode protection settings for
 * a specified power mode.
 *
 * @param protect Power mode to set for protection
 * @return state  Status of the protection setting
 *                - true: Allowed
 *                - false: Not allowed
*/
bool smc_hal_get_power_mode_protection(power_modes_protect_t protect);

/*!
 * @brief Configures the the RUN mode control setting.
 *
 * This function  sets the run mode settings, for example, normal run mode,
 * very lower power run mode, etc. See the smc_run_mode_t for supported run
 * mode on the chip family and the reference manual for details about the 
 * run mode.
 *
 * @param runMode Run mode setting defined in smc_run_mode_t
 */
void smc_hal_power_mode_config_run(smc_run_mode_t runMode);

/*!
 * @brief Gets  the current RUN mode configuration setting.
 *
 * This function  gets the run mode settings. See the smc_run_mode_t 
 * for a supported run mode on the chip family and the reference manual for 
 * details about the run mode.
 *
 * @return setting Run mode configuration setting
*/
smc_run_mode_t smc_hal_power_mode_get_run_config(void);

/*!
 * @brief Configures  the STOP mode control setting.
 *
 * This function  sets the stop mode settings, for example, normal stop mode,
 * very lower power stop mode, etc. See the  smc_stop_mode_t for supported stop
 * mode on the chip family and the reference manual for details about the 
 * stop mode.
 *
 * @param stopMode Stop mode defined in smc_stop_mode_t
 */
void smc_hal_power_mode_config_stop(smc_stop_mode_t stopMode);

/*!
 * @brief Gets the current STOP mode control settings.
 *
 * This function  gets the stop mode settings, for example, normal stop mode,
 * very lower power stop mode, etc. See the  smc_stop_mode_t for supported stop
 * mode on the chip family and the reference manual for details about the 
 * stop mode.
 *
 * @return setting Current stop mode configuration setting
*/
smc_stop_mode_t smc_hal_power_mode_get_stop_config(void);

/*!
 * @brief Configures the stop sub mode control setting.
 *
 * This function  sets the stop submode settings. Some of the stop mode 
 * further supports submodes. See the  smc_stop_submode_t for supported
 * stop submodes and the  reference manual for details about the submodes
 * for a specific stop mode.
 *
 * @param stopSubMode Stop submode setting defined in smc_stop_submode_t
 *
 * @return none
 */
void smc_hal_power_mode_config_stop_submode(smc_stop_submode_t stopSubMode);

/*!
 * @brief Gets the current stop submode configuration settings. 
 *
 * This function  gets the stop submode settings. Some of the stop mode 
 * further support  submodes. See the smc_stop_submode_t for supported
 * stop submodes and the reference manual for details about the submode
 * for a specific stop mode.
 *
 * @return setting Current stop submode setting
*/
smc_stop_submode_t smc_hal_power_mode_get_stop_submode_config(void);

#if FSL_FEATURE_SMC_HAS_PORPO
/*!
 * @brief Configures the POR (power-on-reset) option.
 *
 * This function  sets the POR power option setting. It controls whether the
 * POR detect circuit (for brown-out detection) is enabled in a certain stop mode.
 * The setting either enables or disables the above feature when the POR 
 * occurs. See the reference manual for details.
 *
 * @param option POR option setting refer to smc_por_option_t
 */
void smc_hal_config_por_power_option(smc_por_option_t option);

/*!
 * @brief Gets the configuration settings for the POR option.
 *
 * This function  sets the POR power option setting. See the configuration function
 * header for details.
 *
 * @return option Current POR option setting
*/
smc_por_option_t smc_hal_get_por_power_config(void);
#endif

#if FSL_FEATURE_SMC_HAS_PSTOPO
/*!
 * @brief Configures the PSTOPOR (Partial Stop Option).
 *
 * This function  sets the PSTOPOR option. It controls whether a Partial 
 * Stop mode is entered when the STOPM=STOP. When entering a Partial Stop mode from the
 * RUN mode, the PMC, MCG and Flash remain fully powered allowing the device 
 * to wakeup almost instantaneously at the expense of a higher power consumption.
 * In PSTOP2, only the system clocks are gated, which allows the peripherals running on bus
 * clock to remain fully functional. In PSTOP1, both system and bus clocks are
 * gated. Refer to the smc_pstop_option_t for supported options. See the reference
 * manual for details.
 *
 * @param option PSTOPOR option setting defined in smc_pstop_option_t
 */
void smc_hal_config_pstop_power_option(smc_pstop_option_t option);

/*!
 * @brief Gets the configuration of the PSTOPO option.
 *
 * This function  gets the current PSTOPOR option setting. See the  configuration
 * function for more details.
 *
 * @return option Current PSTOPOR option setting
*/
smc_por_option_t smc_hal_get_pstop_power_config(void);
#endif

#if FSL_FEATURE_SMC_HAS_LPOPO
/*!
 * @brief Configures the LPO option setting.
 *
 * This function  sets the LPO option setting. It controls whether the 1 kHZ
 * LPO clock is enabled in a certain lower power stop modes. See the 
 * smc_lpo_option_t for supported options and the reference manual for 
 * details about this option.
 *
 * @param option LPO option setting defined in smc_lpo_option_t
 */
void smc_hal_config_lpo_power_option(smc_lpo_option_t option);

/*!
 * @brief Gets the  settings of the LPO option. 
 *
 * This function  gets the current LPO option setting. See the  configuration 
 * function for details.
 *
 * @return option Current LPO option setting
*/
smc_por_option_t smc_hal_get_lpo_power_config(void);
#endif

#if FSL_FEATURE_SMC_HAS_LPWUI
/*!
 * @brief Configures the LPWUI (Low Power Wake Up on interrupt) option.
 *
 * This function  sets the LPWUI option and cause the system to exit
 * to normal RUN mode when any active interrupt occurs while in a specific lower
 * power mode. See the  smc_lpwui_option_t for supported options and the  
 * reference manual for more details about this option.
 *
 * @param option LPWUI option setting defined in smc_lpwui_option_t
 */
void smc_hal_config_lpwui_option(smc_lpwui_option_t option);

/*!
 * @brief Gets the current LPWUI option.
 *
 * This function  gets the LPWUI option. See the configuration function for more
 * details.
 *
 * @return setting Current LPWAUI option setting
*/
smc_lpwui_option_t smc_hal_get_lpwui_config(void);
#endif

/*!
 * @brief Gets the current power mode stat.
 *
 * This function  returns the current power mode stat. Once application
 * switches the power mode, it should always check the stat to check whether it 
 * runs into the specified mode or not. An application  should  check 
 * this mode before switching to a different mode. The system  requires that
 * only certain modes can switch to other specific modes. See the 
 * reference manual for details and the _power_mode_stat for information about
 * the power stat.
 *
 * @return stat  Current power mode stat
*/
uint8_t smc_hal_get_power_mode_stat(void);

/*@}*/

#if defined(__cplusplus)
}
#endif /* __cplusplus*/

/*! @}*/

#endif /* __FSL_SMC_HAL_H__*/
/*******************************************************************************
 * EOF
 ******************************************************************************/

