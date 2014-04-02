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
#if !defined(__FSL_PMC_HAL_H__)
#define __FSL_PMC_HAL_H__

#include <stdint.h>
#include <stdbool.h>
#include <assert.h>
#include "fsl_pmc_features.h"

/*! @addtogroup pmc_hal*/
/*! @{*/

/*! @file fsl_pmc_hal.h */

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*! @brief Low-Voltage Warning Voltage Select*/
typedef enum _pmc_lvwv_select {
    kPmcLvwvLowTrip,            /*!< Low trip point selected (VLVW = VLVW1)*/
    kPmcLvwvMid1Trip,           /*!< Mid 1 trip point selected (VLVW = VLVW2)*/
    kPmcLvwvMid2Trip,           /*!< Mid 2 trip point selected (VLVW = VLVW3)*/
    kPmcLvwvHighTrip            /*!< High trip point selected (VLVW = VLVW4)*/
} pmc_lvwv_select_t;

/*! @brief Low-Voltage Detect Voltage Select*/
typedef enum _pmc_lvdv_select {
    kPmcLvdvLowTrip,            /*!< Low trip point selected (V LVD = V LVDL )*/
    kPmcLvdvHighTrip,           /*!< High trip point selected (V LVD = V LVDH )*/
} pmc_lvdv_select_t;


/*******************************************************************************
 * API
 ******************************************************************************/

#if defined(__cplusplus)
extern "C" {
#endif /* __cplusplus*/

/*! @name Power Management Controller Control APIs*/
/*@{*/


/*!
 * @brief Low-Voltage Detect Interrupt Enable
 *
 * This function  enables  the interrupt for the low voltage detection. When
 * enabled, if the LVDF (Low Voltage Detect Flag) is set, a hardware interrupt
 *  occurs.
 *
 */
static inline void pmc_hal_enable_low_voltage_detect_interrupt(void)
{
    BW_PMC_LVDSC1_LVDIE(1);
}

/*!
 * @brief Low-Voltage Detect Interrupt Disable (use polling)
 *
 * This function  disables the the interrupt for low voltage detection. When
 * disabled, an application can only check the low voltage through polling the LVDF
 * (Low Voltage Detect Flag).
 *
 */
static inline void pmc_hal_disable_low_voltage_detect_interrupt(void)
{
    BW_PMC_LVDSC1_LVDIE(0);
}

/*!
 * @brief Low-Voltage Detect Hardware Reset Enable (write once)
 *
 * This function  enables the  hardware reset for the low voltage detection. 
 * When enabled, if the LVDF (Low Voltage Detect Flag) is set, a hardware reset
 * occurs. This setting is a write-once-only; Additional writes are 
 * ignored.
 *
 */
static inline void pmc_hal_enable_low_voltage_detect_reset(void)
{
    BW_PMC_LVDSC1_LVDRE(1);
}

/*!
 * @brief Low-Voltage Detect Hardware Reset Disable
 *
 * This function  disables the the hardware reset for low voltage detection. 
 * When disabled, if the LVDF (Low Voltage Detect Flag) is set, a hardware reset
 * does not occur. This setting is a write-once-only; Additional writes are 
 * ignored.
 *
 */
static inline void pmc_hal_disable_low_voltage_detect_reset(void)
{
    BW_PMC_LVDSC1_LVDRE(0);
}

/*!
 * @brief Low-Voltage Detect Acknowledge
 *
 * This function acknowledges the low voltage detection errors (write 1 to
 * clear LVDF).
 *
 */
static inline void pmc_hal_low_voltage_detect_ack(void)
{
    BW_PMC_LVDSC1_LVDACK(1);
}

/*!
 * @brief Low-Voltage Detect Flag Read
 *
 * This function  reads the current LVDF status. If it returns 1,  low
 * voltage event is detected.
 *
 * @return status Current low voltage detect flag
 *                - true: Low-Voltage detected
 *                - false: Low-Voltage not detected
 */
static inline bool pmc_hal_get_low_voltage_detect_flag(void)
{
    return BR_PMC_LVDSC1_LVDF;
}

/*!
 * @brief Sets the Low-Voltage Detect Voltage Select
 *
 * This function  sets the low voltage detect voltage select. It  sets
 * the low voltage detect trip point voltage (Vlvd). An application can select
 * either a low-trip or a high-trip point. See a chip reference manual for details.
 *
 * @param select Voltage select setting defined in pmc_lvdv_select_t
 */
static inline void pmc_hal_set_low_voltage_detect_voltage_select(pmc_lvdv_select_t select)
{
    BW_PMC_LVDSC1_LVDV(select);
}

/*!
 * @brief Gets the Low-Voltage Detect Voltage Select
 *
 * This function  gets the low voltage detect voltage select. It  gets 
 * the low voltage detect trip point voltage (Vlvd). An application can select
 * either a low-trip or a high-trip point. See a chip reference manual for details.
 *
 * @return select Current voltage select setting
 */
static inline pmc_lvdv_select_t pmc_hal_get_low_voltage_detect_voltage_select(void)
{
    return (pmc_lvdv_select_t)BR_PMC_LVDSC1_LVDV;
}


/*!
 * @brief Low-Voltage Warning Interrupt Enable
 *
 * This function  enables the  interrupt for the low voltage warning 
 * detection. When enabled, if the LVWF (Low Voltage Warning Flag) is set, 
 * a hardware interrupt  occurs.
 *
 */
static inline void pmc_hal_enable_low_voltage_warning_interrupt(void)
{
    BW_PMC_LVDSC2_LVWIE(1);
}

/*!
 * @brief Low-Voltage Warning Interrupt Disable (use polling)
 *
 * This function  disables  the interrupt for the low voltage warning 
 * detection. When disabled, if the LVWF (Low Voltage Warning Flag) is set, 
 * a hardware interrupt does not occur.
 *
 */
static inline void pmc_hal_disable_low_voltage_warning_interrupt(void)
{
    BW_PMC_LVDSC2_LVWIE(0);
}

/*!
 * @brief Low-Voltage Warning Acknowledge
 * 
 * This function acknowledges the low voltage warning errors (write 1 to
 * clear LVWF).
 *
 */
static inline void pmc_hal_low_voltage_warning_ack(void)
{
    BW_PMC_LVDSC2_LVWACK(1);
}

/*!
 * @brief Low-Voltage Warning Flag Read
 *
 * This function polls the current LVWF status. When 1 is returned, it 
 * indicates a low-voltage warning event. LVWF is set when V Supply transitions
 * below the trip point or after reset and V Supply is already below the V LVW.
 *
 * @return status Current LVWF status
                  - true: Low-Voltage Warning Flag is set.
                  - false: the  Low-Voltage Warning does not happen.
 */
static inline bool pmc_hal_get_low_voltage_warning_flag(void)
{
    return BR_PMC_LVDSC2_LVWF;
}

/*!
 * @brief Sets the Low-Voltage Warning Voltage Select.
 *
 * This function  sets the low voltage warning voltage select. It  sets
 * the low voltage warning trip point voltage (Vlvw). An application can select
 * either a low, mid1, mid2 and a high-trip point. See a chip reference manual for 
 * details and the  pmc_lvwv_select_t for supported settings.
 * 
 * @param select Low voltage warning select setting
 */
static inline void pmc_hal_set_low_voltage_warning_voltage_select(pmc_lvwv_select_t select)
{
    BW_PMC_LVDSC2_LVWV(select);
}

/*!
 * @brief Gets the Low-Voltage Warning Voltage Select.
 *
 * This function  gets the low voltage warning voltage select. It  gets
 * the low voltage warning trip point voltage (Vlvw). See the pmc_lvwv_select_t
 * for  supported settings.
 *
 * @return select Current low voltage warning select setting
 */
static inline pmc_lvwv_select_t pmc_hal_get_low_voltage_warning_voltage_select(void)
{
    return (pmc_lvwv_select_t)BR_PMC_LVDSC2_LVWV;
}

#if FSL_FEATURE_SMC_HAS_BGEN
/*!
 * @brief Enables the Bandgap in VLPx Operation.
 *
 * This function  enables the bandgap in lower power modes of operation (VLPx, 
 * LLS, and VLLSx). When on-chip peripherals require the bandgap voltage 
 * reference in low power modes of operation, set BGEN to continue to enable
 * the bandgap operation.
 *
 */
static inline void pmc_hal_enable_bandgap_in_low_power_mode(void)
{
    BW_PMC_REGSC_BGEN(1);
}

/*!
 * @brief Disables the Bandgap in the VLPx Operation.
 *
 * This function  disables the bandgap in lower power modes of operation (VLPx, 
 * LLS, and VLLSx). When the bandgap voltage reference is not needed in low 
 * power modes, disable BGEN to avoid excess power consumption.
 *
 */
static inline void pmc_hal_disable_bandgap_in_low_power_mode(void)
{
    BW_PMC_REGSC_BGEN(0);
}
#endif

/*!
 * @brief Enables the Bandgap Buffer.
 *
 * This function  enables the Bandgap buffer.
 *
 */
static inline void pmc_hal_enable_bandgap_buffer(void)
{
    BW_PMC_REGSC_BGBE(1);
}

/*!
 * @brief Disables the Bandgap Buffer.
 *
 * This function  disables the Bandgap buffer.
 *
 */
static inline void pmc_hal_disable_bandgap_buffer(void)
{
    BW_PMC_REGSC_BGBE(0);
}

/*!
 * @brief Gets the Acknowledge Isolation.
 *
 * This function  reads the Acknowledge Isolation setting that indicates 
 * whether certain peripherals and the I/O pads are in a latched state as 
 * a result of having been in the VLLS mode. 
 *
 * @return value ACK isolation
 *               0 - Peripherals and I/O pads are in a normal run state.
 *               1 - Certain peripherals and I/O pads are in an isolated and
 *                   latched state.
 */
static inline uint8_t pmc_hal_get_ack_isolation(void)
{
    return BR_PMC_REGSC_ACKISO;
}

/*!
 * @brief Clears an Acknowledge Isolation.
 *
 * This function  clears the ACK Isolation flag. Writing one to this setting
 * when it is set releases the I/O pads and certain peripherals to their normal
 * run mode state.
 *
 */
static inline void pmc_hal_clear_ack_isolation(void)
{
    BW_PMC_REGSC_ACKISO(1);
}

/*!
 * @brief Gets the Regulator regulation status.
 *
 * This function  returns the regulator to a run regulation status. It provides
 * the current status of the internal voltage regulator.
 *
 * @return value Regulation status
 *               0 - Regulator is in a stop regulation or in transition to/from it.
 *               1 - Regulator is in a run regulation.
 *
 */
static inline uint8_t pmc_hal_get_regulator_status(void)
{
    return BR_PMC_REGSC_REGONS;
}

/*@}*/

#if defined(__cplusplus)
}
#endif /* __cplusplus*/

/*! @}*/

#endif /* __FSL_PMC_HAL_H__*/
/*******************************************************************************
 * EOF
 ******************************************************************************/

