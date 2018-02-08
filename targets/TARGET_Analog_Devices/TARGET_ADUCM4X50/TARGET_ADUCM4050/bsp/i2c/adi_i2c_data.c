/*
 *****************************************************************************
 * @file:    adi_i2c_data.c
 * @brief:   Data declaration for I2C Device Driver
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
  - Modified versions of the software must be coni2ccuously marked as such.
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

#ifndef ADI_I2C_DATA_C
#define ADI_I2C_DATA_C

 /*! \cond PRIVATE */

#include <adi_processor.h>
#include "adi_i2c_def.h"
#include "adi_i2c_config.h"

/* Stores the information about the specific device */
static ADI_I2C_DEVICE_INFO i2c_device_info [ADI_I2C_NUM_INSTANCES] =
{
    /* fixed instance data for the singular I2C0 controller */
    {
        I2C_MST_EVT_IRQn,              /* pio interrupt number         */
        (ADI_I2C_TypeDef *)pADI_I2C0,  /* i2c controller pointer       */
        NULL,                          /* pointer to user config data  */
        NULL                           /* i2c device handle (user mem) */
    },

    /* no other i2c instances at this time */
};

/* build I2C Application configuration array */
static ADI_I2C_CONFIG gConfigInfo[ADI_I2C_NUM_INSTANCES] =
{
    /* the one-and-only (so far) instance data for I2C, I2C0... */
    {
        /**** I2C_MCTL Master Control register *** */
        (
            /* note: Master IENMTX and IENMRX (transmit and receive interrupts) are managed dynamically */
            ( ADI_I2C_CFG_MCTL_MXMITDEC     << BITP_I2C_MCTL_MXMITDEC     ) |
            ( ADI_I2C_CFG_MCTL_IENCMP       << BITP_I2C_MCTL_IENCMP       ) |
            ( ADI_I2C_CFG_MCTL_IENACK       << BITP_I2C_MCTL_IENACK       ) |
            ( ADI_I2C_CFG_MCTL_IENALOST     << BITP_I2C_MCTL_IENALOST     ) |
            ( ADI_I2C_CFG_MCTL_STRETCHSCL   << BITP_I2C_MCTL_STRETCHSCL   ) |
            ( ADI_I2C_CFG_MCTL_LOOPBACK     << BITP_I2C_MCTL_LOOPBACK     ) |
            ( ADI_I2C_CFG_MCTL_COMPLETE     << BITP_I2C_MCTL_COMPLETE     ) |
            ( ADI_I2C_CFG_MCTL_MASEN        << BITP_I2C_MCTL_MASEN        )
        ),

        /**** I2C_DIV Clock Divider register *** */
        (
            ( ADI_I2C_CFG_DIV_HIGH          << BITP_I2C_DIV_HIGH          ) |
            ( ADI_I2C_CFG_DIV_LOW           << BITP_I2C_DIV_LOW           )
        ),

        /**** I2C_SHCTL Shared Control register *** */
        (
            ( ADI_I2C_CFG_SHCTL_RST         << BITP_I2C_TCTL_FILTEROFF    )
        ),

        /**** I2C_TCTL Timing control register *** */
        (
            ( ADI_I2C_CFG_TCTL_FILTEROFF    << BITP_I2C_SHCTL_RST         ) |
            ( ADI_I2C_CFG_TCTL_THDATIN      << BITP_I2C_TCTL_THDATIN      )
        ),

        /**** I2C_ASTRETCH Master Clock Stretch register *** */
        (
            ( ADI_I2C_CFG_ASTRETCH_MST      << BITP_I2C_ASTRETCH_SCL_MST  )
        ),

        /**** Target Slave configuration value (not a register) *** */
        (
            ( ADI_I2C_CFG_SLAVE_ADDRESS                                   )
        ),
    }
};

/*! \endcond */


#endif /* ADI_I2C_DATA_C */
