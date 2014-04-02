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

#include "fsl_i2c_hal.h"

/* Computes the number of elements in an array.*/
#define ARRAY_SIZE(x)	(sizeof(x) / sizeof((x)[0]))

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*! @brief An entry in the I2C divider table.*/
/*!*/
/*! This struct pairs the value of the I2C_F.ICR bitfield with the resulting*/
/*! clock divider value.*/
typedef struct I2CDividerTableEntry {
    uint8_t icr;            /*!< F register ICR value.*/
    uint16_t sclDivider;    /*!< SCL clock divider.*/
} i2c_divider_table_entry_t;

/*******************************************************************************
 * Variables
 ******************************************************************************/

/*! @brief I2C divider values.*/
/*!*/
/*! This table is taken from the I2C Divider and Hold values section of the*/
/*! reference manual. In the original table there are, in some cases, multiple*/
/*! entries with the same divider but different hold values. This table*/
/*! includes only one entry for every divider, selecting the lowest hold value.*/
const i2c_divider_table_entry_t kI2CDividerTable[] = {
        /* ICR  Divider*/
        { 0x00, 20 },
        { 0x01, 22 },
        { 0x02, 24 },
        { 0x03, 26 },
        { 0x04, 28 },
        { 0x05, 30 },
        { 0x09, 32 },
        { 0x06, 34 },
        { 0x0a, 36 },
        { 0x07, 40 },
        { 0x0c, 44 },
        { 0x0d, 48 },
        { 0x0e, 56 },
        { 0x12, 64 },
        { 0x0f, 68 },
        { 0x13, 72 },
        { 0x14, 80 },
        { 0x15, 88 },
        { 0x19, 96 },
        { 0x16, 104 },
        { 0x1a, 112 },
        { 0x17, 128 },
        { 0x1c, 144 },
        { 0x1d, 160 },
        { 0x1e, 192 },
        { 0x22, 224 },
        { 0x1f, 240 },
        { 0x23, 256 },
        { 0x24, 288 },
        { 0x25, 320 },
        { 0x26, 384 },
        { 0x2a, 448 },
        { 0x27, 480 },
        { 0x2b, 512 },
        { 0x2c, 576 },
        { 0x2d, 640 },
        { 0x2e, 768 },
        { 0x32, 896 },
        { 0x2f, 960 },
        { 0x33, 1024 },
        { 0x34, 1152 },
        { 0x35, 1280 },
        { 0x36, 1536 },
        { 0x3a, 1792 },
        { 0x37, 1920 },
        { 0x3b, 2048 },
        { 0x3c, 2304 },
        { 0x3d, 2560 },
        { 0x3e, 3072 },
        { 0x3f, 3840 }
    };

/*******************************************************************************
 * Code
 ******************************************************************************/

/* Documentation for this function is in fsl_i2c_hal.h.*/
void i2c_hal_init(uint32_t instance, const i2c_config_t * config, uint32_t sourceClockInHz)
{
    assert(config);
    
    /* Enable module if requested.*/
    if (config->enableModule)
    {
        i2c_hal_enable(instance);
    }
    else
    {
        i2c_hal_disable(instance);
    }
    
    /* Set baud rate if nonzero.*/
    if (config->baudRate_kbps)
    {
        i2c_hal_set_baud(instance, sourceClockInHz, config->baudRate_kbps, NULL);
    }
    
    /* Configure miscellaneous options.*/
    i2c_hal_set_independent_slave_baud(instance, config->useIndependentSlaveBaud);
    i2c_hal_set_dma_enable(instance, config->enableDma);
    i2c_hal_set_high_drive(instance, config->enableHighDrive);
    i2c_hal_set_wakeup_enable(instance, config->enableWakeup);
    i2c_hal_set_glitch_filter(instance, config->glitchFilterWidth);
    
    /* Configure the slave address options.*/
    if (config->use10bitSlaveAddress)
    {
        i2c_hal_set_slave_address_10bit(instance, config->slaveAddress);
    }
    else
    {
        i2c_hal_set_slave_address_7bit(instance, (uint8_t)((config->slaveAddress) & 0xff));
    }
    
    i2c_hal_set_general_call_enable(instance, config->enableGeneralCallAddress);
    i2c_hal_set_slave_range_address_enable(instance, config->enableRangeAddressMatch);
    
    /* Enable or disable the I2C interrupt.*/
    if (config->enableInterrupt)
    {
        i2c_hal_enable_interrupt(instance);
    }
    else
    {
        i2c_hal_disable_interrupt(instance);
    }
    
    /* Configure stop detect related features.*/
#if FSL_FEATURE_I2C_HAS_STOP_DETECT
#if FSL_FEATURE_I2C_HAS_STOP_HOLD_OFF
    i2c_hal_set_stop_holdoff(instance, config->enableStopModeHoldoff);
#endif /* FSL_FEATURE_I2C_HAS_STOP_HOLD_OFF*/
    
    if (config->enableBusStopInterrupt)
    {
        i2c_hal_enable_bus_stop_interrupt(instance);
    }
    else
    {
        i2c_hal_disable_bus_stop_interrupt(instance);
    }
#endif /* FSL_FEATURE_I2C_HAS_STOP_DETECT*/
}

/* Documentation for this function is in fsl_i2c_hal.h.*/
void i2c_hal_reset(uint32_t instance)
{
    assert(instance < HW_I2C_INSTANCE_COUNT);
    
    HW_I2C_A1_WR(instance, 0u);
    HW_I2C_F_WR(instance, 0u);
    HW_I2C_C1_WR(instance, 0u);
    HW_I2C_S_WR(instance, 0u);
    HW_I2C_D_WR(instance, 0u);
    HW_I2C_C2_WR(instance, 0u);
    HW_I2C_FLT_WR(instance, 0u);
    HW_I2C_RA_WR(instance, 0u);
    
#if FSL_FEATURE_I2C_HAS_SMBUS
    HW_I2C_SMB_WR(instance, 0u);
    HW_I2C_A2_WR(instance, 0xc2u);
    HW_I2C_SLTH_WR(instance, 0u);
    HW_I2C_SLTL_WR(instance, 0u);
#endif /* FSL_FEATURE_I2C_HAS_SMBUS*/
}

/* Documentation for this function is in fsl_i2c_hal.h.*/
uint32_t i2c_hal_get_max_baud(uint32_t instance, uint32_t sourceClockInHz)
{
    assert(instance < HW_I2C_INSTANCE_COUNT);

    return (sourceClockInHz / (1U * 20U)) / 1000;
}

/* Documentation for this function is in fsl_i2c_hal.h.*/
i2c_status_t i2c_hal_set_baud(uint32_t instance, uint32_t sourceClockInHz, uint32_t kbps,
                                  uint32_t * absoluteError_Hz)
{
    assert(instance < HW_I2C_INSTANCE_COUNT);
        
    /* Check if the requested frequency is greater than the max supported baud.*/
    if ((kbps * 1000U) > (sourceClockInHz / (1U * 20U)))
    {
        return kStatus_I2C_OutOfRange;
    }
    
    uint32_t mult;
    uint32_t hz = kbps * 1000u;
    uint32_t bestError = 0xffffffffu;
    uint32_t bestMult = 0u;
    uint32_t bestIcr = 0u;

    /* Search for the settings with the lowest error.*/
    /**/
    /* mult is the MULT field of the I2C_F register, and ranges from 0-2. It selects the*/
    /* multiplier factor for the divider.*/
    for (mult = 0u; (mult <= 2u) && (bestError != 0); ++mult)
    {
        uint32_t multiplier = 1u << mult;
        
        /* Scan table to find best match.*/
        uint32_t i;
        for (i = 0u; i < ARRAY_SIZE(kI2CDividerTable); ++i)
        {
            uint32_t computedRate = sourceClockInHz / (multiplier * kI2CDividerTable[i].sclDivider);
            uint32_t absError = hz > computedRate ? hz - computedRate : computedRate - hz;
            
            if (absError < bestError)
            {
                bestMult = mult;
                bestIcr = kI2CDividerTable[i].icr;
                bestError = absError;
                
                /* If the error is 0, then we can stop searching because we won't find a*/
                /* better match.*/
                if (absError == 0)
                {
                    break;
                }
            }
        }
    }

    /* Set the resulting error.*/
    if (absoluteError_Hz)
    {
        *absoluteError_Hz = bestError;
    }
    
    /* Set frequency register based on best settings.*/
    HW_I2C_F_WR(instance, BF_I2C_F_MULT(bestMult) | BF_I2C_F_ICR(bestIcr));
    
    return kStatus_I2C_Success;
}

/* Documentation for this function is in fsl_i2c_hal.h.*/
void i2c_hal_send_start(uint32_t instance)
{
    assert(instance < HW_I2C_INSTANCE_COUNT);
    
    /* Check if we're in a master mode transfer.*/
    if (HW_I2C_C1(instance).B.MST)
    {
#if FSL_FEATURE_I2C_HAS_ERRATA_6070
        /* Errata 6070: Repeat start cannot be generated if the I2Cx_F[MULT] field is set to a*/
        /* non- zero value.*/
        /**/
        /* The workaround is to either always keep MULT set to 0, or to temporarily set it to*/
        /* 0 while performing the repeated start and then restore it.*/
        uint32_t savedMult = 0;
        if (HW_I2C_F(instance).B.MULT != 0)
        {
            savedMult = HW_I2C_F(instance).B.MULT;
            HW_I2C_F(instance).B.MULT = 0;
        }
#endif /* FSL_FEATURE_I2C_HAS_ERRATA_6070*/

        /* We are already in a transfer, so send a repeated start.*/
        HW_I2C_C1_SET(instance, BM_I2C_C1_RSTA);

#if FSL_FEATURE_I2C_HAS_ERRATA_6070
        if (savedMult)
        {
            HW_I2C_F(instance).B.MULT = savedMult;
        }
#endif /* FSL_FEATURE_I2C_HAS_ERRATA_6070*/
    }
    else
    {
        /* Initiate a transfer by sending the start signal.*/
        HW_I2C_C1_SET(instance, BM_I2C_C1_MST | BM_I2C_C1_TX);
    }
}

/* Documentation for this function is in fsl_i2c_hal.h.*/
void i2c_hal_set_slave_address_7bit(uint32_t instance, uint8_t address)
{
    assert(instance < HW_I2C_INSTANCE_COUNT);
    
    /* Set 7-bit slave address.*/
    HW_I2C_A1_WR(instance, address << 1U);
    
    /* Disable the address extension option, selecting 7-bit mode.*/
    HW_I2C_C2_CLR(instance, BM_I2C_C2_ADEXT);
}

/* Documentation for this function is in fsl_i2c_hal.h.*/
void i2c_hal_set_slave_address_10bit(uint32_t instance, uint16_t address)
{
    uint8_t temp;
    assert(instance < HW_I2C_INSTANCE_COUNT);
    
    /* Set bottom 7 bits of slave address.*/
    temp = address & 0x7f;
    HW_I2C_A1_WR(instance, temp << 1U);
    
    /* Enable 10-bit address extension.*/
    HW_I2C_C2_SET(instance, BM_I2C_C2_ADEXT);
    
    /* Set top 3 bits of slave address.*/
    BW_I2C_C2_AD(instance, address >> 7U);
}

/*******************************************************************************
 * EOF
 ******************************************************************************/

