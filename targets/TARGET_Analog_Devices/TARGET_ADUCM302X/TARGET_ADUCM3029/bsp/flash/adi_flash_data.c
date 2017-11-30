/*
 *****************************************************************************
 * @file:    adi_flash_data.c
 * @brief:   Data declaration for Flash Device Driver
 * @date:    $Date$
 *****************************************************************************

Copyright (c) 2016 Analog Devices, Inc.

All rights reserved.

Redistribution and use in source and binary forms, with or without modification,
are permitted provided that the following conditions are met:
  - Redistributions of source code must retain the above copyright notice,
    this list of conditions and the following disclaimer.
  - Redistributions in binary form must reproduce the above copyright notice,
    this list of conditions and the following disclaimer in the documentation
    and/or other materials provided with the distribution.
  - Modified versions of the software must be consciously marked as such.
  - This software is licensed solely and exclusively for use with processors
    manufactured by or for Analog Devices, Inc.
  - This software may not be combined or merged with other code in any manner
    that would cause the software to become subject to terms and conditions
    which differ from those listed here.
  - Neither the name of Analog Devices, Inc. nor the names of its
    contributors may be used to endorse or promote products derived
    from this software without specific prior written permission.
  - The use of this software may or may not infringe the patent rights of one
    or more patent holders.  This license does not release you from the
    requirement that you obtain separate licenses from these patent holders
    to use this software.

THIS SOFTWARE IS PROVIDED BY ANALOG DEVICES, INC. AND CONTRIBUTORS "AS IS"
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, NON-
INFRINGEMENT, TITLE, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL ANALOG DEVICES, INC. OR CONTRIBUTORS BE LIABLE
FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, PUNITIVE OR
CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, DAMAGES ARISING OUT OF
CLAIMS OF INTELLECTUAL PROPERTY RIGHTS INFRINGEMENT; PROCUREMENT OF
SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
POSSIBILITY OF SUCH DAMAGE.

*****************************************************************************/

#ifndef ADI_FEE_DATA_C
#define ADI_FEE_DATA_C

 /*! \cond PRIVATE */

#include <adi_processor.h>
#include "adi_flash_def.h"
#include "adi_flash_config.h"


/* Stores the information about the specific device */
static ADI_FEE_DEVICE_INFO fee_device_info [ADI_FEE_NUM_INSTANCES] =
{
    /* only one flash instance at this time */
    {    pADI_FLCC0,            /* Flash controller pointer         */
        FLCC_EVT_IRQn,          /* Flash PIO interrupt number       */
        DMA0_CH15_DONE_IRQn,    /* Flash DMA interrupt number       */
        FLASH_CHANn,            /* Flash DMA channel (15) number    */
        NULL,                   /* Flash static config info         */
        NULL                    /* Flash driver handle              */
    },
};


/* build Flash Application configuration array */
static ADI_FEE_CONFIG gConfigInfo[ADI_FEE_NUM_INSTANCES] =
{
    /* the one-and-only (so far) instance data for FEE0... */
    {
#if defined (__ADUCM4x50__)
        /* ECC interrupt enable settings (IEN register) */
        ( (ADI_FEE_CFG_ECC_ERROR_RESPONSE      << BITP_FLCC_IEN_ECC_ERROR)
        | (ADI_FEE_CFG_ECC_CORRECTION_RESPONSE << BITP_FLCC_IEN_ECC_CORRECT)
        ),

#elif defined (__ADUCM302x__)

        /* ECC interrupt enable settings (IEN register) */
        ( (ADI_FEE_CFG_ECC_ERROR_RESPONSE      << BITP_FLCC_IEN_ECC_ERROR)),

#endif
        /* timing parameter settings (TIME_PARAM0 register) */
        ( (ADI_FEE_CFG_PARAM0_TNVH1            << BITP_FLCC_TIME_PARAM0_TNVH1)
        | (ADI_FEE_CFG_PARAM0_TERASE           << BITP_FLCC_TIME_PARAM0_TERASE)
        | (ADI_FEE_CFG_PARAM0_TRCV             << BITP_FLCC_TIME_PARAM0_TRCV)
        | (ADI_FEE_CFG_PARAM0_TNVH             << BITP_FLCC_TIME_PARAM0_TNVH)
        | (ADI_FEE_CFG_PARAM0_TPROG            << BITP_FLCC_TIME_PARAM0_TPROG)
        | (ADI_FEE_CFG_PARAM0_TPGS             << BITP_FLCC_TIME_PARAM0_TPGS)
        | (ADI_FEE_CFG_PARAM0_TNVS             << BITP_FLCC_TIME_PARAM0_TNVS)
        | (ADI_FEE_CFG_PARAM0_CLKDIV           << BITP_FLCC_TIME_PARAM0_DIVREFCLK)
        ),
#if defined (__ADUCM4x50__)
            /* more timing parameter settings (TIME_PARAM1 register) */
        ( (ADI_FEE_CFG_PARAM1_WAITESTATES      << BITP_FLCC_TIME_PARAM1_WAITSTATES)
        | (ADI_FEE_CFG_PARAM1_TWK              << BITP_FLCC_TIME_PARAM1_TWK)
        ),

#elif defined (__ADUCM302x__)
        /* more timing parameter settings (TIME_PARAM1 register) */
        ((ADI_FEE_CFG_PARAM1_TWK              << BITP_FLCC_TIME_PARAM1_TWK)),

#endif
        /* system interrupt abort enables (ABORT_EN_XX registers) */
        (ADI_FEE_CFG_ABORT_EN_LO),
        (ADI_FEE_CFG_ABORT_EN_HI),

        /* ECC configuration register settings (ECC_CFG register) */
        (((ADI_FEE_CFG_ECC_START_PAGE          << FEE_PAGE_SHIFT) & BITM_FLCC_ECC_CFG_PTR)
#if (ADI_FEE_CFG_ENABLE_ECC_FOR_INFO_SPACE == 1u)
        | (BITM_FLCC_ECC_CFG_INFOEN)
#endif
#if (ADI_FEE_CFG_ENABLE_ECC == 1u)
        | (BITM_FLCC_ECC_CFG_EN)
#endif
        )
    }  /* end device 0 settings */
};

/*! \endcond */


#endif /* ADI_FEE_DATA_C */
