/*!
 *****************************************************************************
 * @file:    adi_beep_def_v1.h
 * @brief:   BEEP Device Driver definition
 * @version: $Revision$
 * @date:    $Date$
 *-----------------------------------------------------------------------------
 *
 * Copyright (c) 2010-2014 Analog Devices, Inc.
 *
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 * - Redistributions of source code must retain the above copyright notice,
 *   this list of conditions and the following disclaimer.
 * - Redistributions in binary form must reproduce the above copyright notice,
 *   this list of conditions and the following disclaimer in the documentation
 *   and/or other materials provided with the distribution.
 * - Modified versions of the software must be conspicuously marked as such.
 * - This software is licensed solely and exclusively for use with processors
 *   manufactured by or for Analog Devices, Inc.
 * - This software may not be combined or merged with other code in any manner
 *   that would cause the software to become subject to terms and conditions
 *   which differ from those listed here.
 * - Neither the name of Analog Devices, Inc. nor the names of its
 *   contributors may be used to endorse or promote products derived
 *   from this software without specific prior written permission.
 * - The use of this software may or may not infringe the patent rights of one
 *   or more patent holders.  This license does not release you from the
 *   requirement that you obtain separate licenses from these patent holders
 *   to use this software.
 *
 * THIS SOFTWARE IS PROVIDED BY ANALOG DEVICES, INC. AND CONTRIBUTORS "AS IS" AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, NON-INFRINGEMENT,
 * TITLE, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN
 * NO EVENT SHALL ANALOG DEVICES, INC. OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT,
 * INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, PUNITIVE OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, DAMAGES ARISING OUT OF CLAIMS OF INTELLECTUAL
 * PROPERTY RIGHTS INFRINGEMENT; PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
 * OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 *****************************************************************************/

#ifndef _ADI_BEEP_DEF_V1_H_
#define _ADI_BEEP_DEF_V1_H_

/*! \cond PRIVATE */


/*!
 *****************************************************************************
 * An interrupt mask covering all Beeper interrupts.
 *****************************************************************************/
#define ADI_BEEP_ALL_INTERRUPTS ( ADI_BEEP_INTERRUPT_SEQ_END         \
                                | ADI_BEEP_INTERRUPT_SEQ_NEAR_END    \
                                | ADI_BEEP_INTERRUPT_TONEB_END       \
                                | ADI_BEEP_INTERRUPT_TONEB_START     \
                                | ADI_BEEP_INTERRUPT_TONEA_END       \
                                | ADI_BEEP_INTERRUPT_TONEA_START)

#define ADI_BEEP_TONE_DISABLE               (BITM_BEEP_TONEA_DIS)       /*!<  Beeper tone disable bit                          */

#define ADI_BEEP_TONE_FREQ_BITPOS           (BITP_BEEP_TONEA_FREQ)      /*!<  Beeper tone frequency bitfield position          */
#define ADI_BEEP_TONE_DUR_BITPOS            (BITP_BEEP_TONEA_DUR)       /*!<  Beeper tone duration bitfield position           */

#define ADI_BEEP_TONE_FREQ_MASK             (BITM_BEEP_TONEA_FREQ)      /*!<  Beeper tone frequency bitfield mask              */
#define ADI_BEEP_TONE_DUR_MASK              (BITM_BEEP_TONEA_DUR)       /*!<  Beeper tone duration bitfield mask               */

/*!
 *****************************************************************************
 * \enum ADI_BEEP_STATE
 *
 * BEEP driver state.  Used for internal tracking of the BEEP device initialization
 * progress during the adi_beep_Open().  Also used to insure the BEEP device has been
 * properly initialized as a prerequisite to using the balance of the BEEP API.
 *
 *****************************************************************************/
typedef enum
{
    ADI_BEEP_STATE_UNINITIALIZED= 0,        /*!< BEEP is not initialized.        */
    ADI_BEEP_STATE_INITIALIZED              /*!< BEEP is initialized.            */
} ADI_BEEP_STATE;


/*!
 * \struct ADI_BEEP_DEV_DATA
 * Beeper device internal instance data structure.
 */
typedef struct _ADI_BEEP_DEV_DATA
{
    ADI_BEEP_STATE            InitState;    /*!< Initialization state        */
    IRQn_Type                 IRQn;         /*!< BEEP interrupt number       */
    ADI_CALLBACK              cbFunc;       /*!< Callback function           */
    void*                     cbWatch;      /*!< Callback watch bits         */
} ADI_BEEP_DEV_DATA;


/*! \struct ADI_BEEP_DRIVER_STRUCT
 *  BEEP Device Structure
 */
typedef struct _ADI_BEEP_DRIVER_STRUCT
{
    ADI_BEEP_TypeDef  *const pReg;          /*!< Pointer to register base           */
    ADI_BEEP_DEV_DATA   *pData;             /*!< Pointer to device data structure   */
} ADI_BEEP_DRIVER_STRUCT;


#if (ADI_BEEP_CFG_ENABLE_STATIC_CONFIG_SUPPORT == 1)
/*! \struct ADI_BEEP_STATIC_INIT
 *  conditionally create static initialization data based on adi_beep_config.h settings
 */
typedef struct {
	uint16_t BEEP_CFG;   /*!< Beeper configuration register */
	uint16_t BEEP_STAT;  /*!< Beeper status register */
	uint16_t BEEP_TONEA; /*!< Beeper ToneA register */
	uint16_t BEEP_TONEB; /*!< Beeper ToneB register */
} ADI_BEEP_STATIC_INIT;
#endif

/* alias for the actual device structure */
typedef ADI_BEEP_DRIVER_STRUCT      ADI_BEEP_DRIVER;

/*! \endcond */

#endif /* _ADI_BEEP_DEF_V1_H_ */
