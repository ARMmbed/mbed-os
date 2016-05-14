/**
 * \file
 *
 * \brief FLEXCOM driver for SAM.
 *
 * Copyright (c) 2014-2015 Atmel Corporation. All rights reserved.
 *
 * \asf_license_start
 *
 * \page License
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * 3. The name of Atmel may not be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * 4. This software may only be redistributed and used in connection with an
 *    Atmel microcontroller product.
 *
 * THIS SOFTWARE IS PROVIDED BY ATMEL "AS IS" AND ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT ARE
 * EXPRESSLY AND SPECIFICALLY DISCLAIMED. IN NO EVENT SHALL ATMEL BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
 * STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 * ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 * \asf_license_stop
 *
 */
/*
 * Support and FAQ: visit <a href="http://www.atmel.com/design-support/">Atmel Support</a>
 */

#ifndef FLEXCOM_H_INCLUDED
#define FLEXCOM_H_INCLUDED

#include "compiler.h"
#include "status_codes.h"

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
