/**
 * \file
 *
 * \brief FLEXCOM driver for SAM.
 *
 * Copyright (c) 2014-2020 Microchip Technology Inc. and its subsidiaries.
 *
 * \asf_license_start
 *
 * \page License
 *
 * Subject to your compliance with these terms, you may use Microchip
 * software and any derivatives exclusively with Microchip products.
 * It is your responsibility to comply with third party license terms applicable
 * to your use of third party software (including open source software) that
 * may accompany Microchip software.
 *
 * THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS". NO WARRANTIES,
 * WHETHER EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE,
 * INCLUDING ANY IMPLIED WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY,
 * AND FITNESS FOR A PARTICULAR PURPOSE. IN NO EVENT WILL MICROCHIP BE
 * LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE, INCIDENTAL OR CONSEQUENTIAL
 * LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND WHATSOEVER RELATED TO THE
 * SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP HAS BEEN ADVISED OF THE
 * POSSIBILITY OR THE DAMAGES ARE FORESEEABLE.  TO THE FULLEST EXTENT
 * ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS IN ANY WAY
 * RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT OF FEES, IF ANY,
 * THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.
 *
 * \asf_license_stop
 *
 */
/*
 * Support and FAQ: visit <a href="https://www.microchip.com/support/">Microchip Support</a>
 */

#ifndef FLEXCOM_H_INCLUDED
#define FLEXCOM_H_INCLUDED

#include "compiler.h"
#include "status_codes.h"
#include "pic32cx.h"

/// @cond 0
/**INDENT-OFF**/
#ifdef __cplusplus
extern "C" {
#endif
/**INDENT-ON**/
/// @endcond

/**
 * \defgroup sam_drivers_flexcom_group Flexible Serial Communication Controller
 * (FLEXCOM) Driver.
 *
 * \par Purpose
 *
 * The Flexible Serial Communication Controller (FLEXCOM) offers several
 * serial communication protocols that are managed by the three submodules
 * USART, SPI, and TWI.
 * @{
 */

/**
 * FLEXCOM opration Mode
 */
enum flexcom_opmode {
	FLEXCOM_NONE  = FLEXCOM_MR_OPMODE_NO_COM,
	FLEXCOM_USART = FLEXCOM_MR_OPMODE_USART,
	FLEXCOM_SPI   = FLEXCOM_MR_OPMODE_SPI,
	FLEXCOM_TWI   = FLEXCOM_MR_OPMODE_TWI,
};

void flexcom_enable(Flexcom *p_flexcom);
void flexcom_disable(Flexcom *p_flexcom);
void flexcom_set_opmode(Flexcom *p_flexcom, enum flexcom_opmode opmode);
void flexcom_get_opmode(Flexcom *p_flexcom, enum flexcom_opmode *opmode);
void flexcom_write(Flexcom *p_flexcom, uint32_t data);
void flexcom_read(Flexcom *p_flexcom, uint32_t *data);
Pdc *flexcom_get_pdc_base(Flexcom *p_flexcom);

/** @} */

/// @cond 0
/**INDENT-OFF**/
#ifdef __cplusplus
}
#endif
/**INDENT-ON**/
/// @endcond

/**
 * \page sam_flexcom_quickstart Quickstart guide for SAM FLEXCOM module
 *
 * This is the quickstart guide for the "SAM FLEXCOM module", with
 * instructions on how to configure and use the module in a selection of use
 * cases. All details related to these submodules(USART/SPI/TWI)
 * are provided in independent sections.
 *
 * The use cases contain several code fragments. The code fragments in the
 * steps for setup can be copied into a custom initialization function, while
 * the steps for usage can be copied into, e.g.the main application function.
 *
 * \section flexcom_use_cases FLEXCOM use cases
 * - \ref flexcom_basic_use_case
 *
 * \section flexcom_basic_use_case Basic use case
 *
 * \subsection sam_flexcom_quickstart_prereq Prerequisites
 * -# \ref sysclk_group "System Clock Management (Sysclock)"
 *
 *
 * \section flexcom_basic_use_case_setup Setup steps
 * \subsection flexcom_basic_use_case_setup_code Example code
 * -# Enable flexcom module access:
 *   - \code flexcom_enable(FLEXCOM0); \endcode
 * -# Set  flexcom operation module:
 *   - \code flexcom_set_opmode(FLEXCOM0, FLEXCOM_USART); \endcode
 *
 * \section flexcom_basic_use_case_usage Usage steps
 * \subsection flexcom_basic_use_case_usage_flow Workflow
 * -# Use write/read function to access the data
 * \code
	 flexcom_write(FLEXCOM0, data);
	 flexcom_read(FLEXCOM0, &data);
 \endcode
 */

#endif /* FLEXCOM_H_INCLUDED */
