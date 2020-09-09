//*****************************************************************************
//
//  am_hal_ios.h
//! @file
//!
//! @brief Functions for interfacing with the IO Slave module
//!
//! @addtogroup ios3 IO Slave (SPI/I2C)
//! @ingroup apollo3hal
//! @{
//
//*****************************************************************************

//*****************************************************************************
//
// Copyright (c) 2020, Ambiq Micro
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
//
// 1. Redistributions of source code must retain the above copyright notice,
// this list of conditions and the following disclaimer.
//
// 2. Redistributions in binary form must reproduce the above copyright
// notice, this list of conditions and the following disclaimer in the
// documentation and/or other materials provided with the distribution.
//
// 3. Neither the name of the copyright holder nor the names of its
// contributors may be used to endorse or promote products derived from this
// software without specific prior written permission.
//
// Third party software included in this distribution is subject to the
// additional license terms as defined in the /docs/licenses directory.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
// ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
// LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
// CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
// SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
// INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
// CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
// ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
// POSSIBILITY OF SUCH DAMAGE.
//
// This is part of revision 2.4.2 of the AmbiqSuite Development Package.
//
//*****************************************************************************
// SPDX-License-Identifier: BSD-3-Clause
#ifndef AM_HAL_IOS_H
#define AM_HAL_IOS_H

#ifdef __cplusplus
extern "C"
{
#endif

//*****************************************************************************
//
// CMSIS-style macro for handling a variable IOS module number.
//
#define IOSLAVEn(n) ((IOSLAVE_Type*)(IOSLAVE_BASE + (n * (IOSLAVE_BASE - IOSLAVE_BASE))))
//*****************************************************************************

//*****************************************************************************
//
//! @name Interface Configuration
//! @brief Macro definitions for configuring the physical interface of the IO
//! Slave
//!
//! These macros may be used with the am_hal_ios_config_t structure to set the
//! physical parameters of the SPI/I2C slave module.
//!
//! @{
//
//*****************************************************************************
#define AM_HAL_IOS_USE_SPI           _VAL2FLD(IOSLAVE_CFG_IFCSEL, IOSLAVE_CFG_IFCSEL_SPI)
#define AM_HAL_IOS_SPIMODE_0         _VAL2FLD(IOSLAVE_CFG_SPOL,   IOSLAVE_CFG_SPOL_SPI_MODES_0_3)
#define AM_HAL_IOS_SPIMODE_1         _VAL2FLD(IOSLAVE_CFG_SPOL,   IOSLAVE_CFG_SPOL_SPI_MODES_1_2)
#define AM_HAL_IOS_SPIMODE_2         _VAL2FLD(IOSLAVE_CFG_SPOL,   IOSLAVE_CFG_SPOL_SPI_MODES_1_2)
#define AM_HAL_IOS_SPIMODE_3         _VAL2FLD(IOSLAVE_CFG_SPOL,   IOSLAVE_CFG_SPOL_SPI_MODES_0_3)

#define AM_HAL_IOS_USE_I2C           _VAL2FLD(IOSLAVE_CFG_IFCSEL, IOSLAVE_CFG_IFCSEL_I2C)
#define AM_HAL_IOS_I2C_ADDRESS(n)    _VAL2FLD(IOSLAVE_CFG_I2CADDR, n)

#define AM_HAL_IOS_LSB_FIRST         _VAL2FLD(IOSLAVE_CFG_LSB, 1)
//! @}

//*****************************************************************************
//
//! @name Register Access Interrupts
//! @brief Macro definitions for register access interrupts.
//!
//! These macros may be used with any of the
//!
//! @{
//
//*****************************************************************************
#define AM_HAL_IOS_ACCESS_INT_00     _VAL2FLD(IOSLAVE_REGACCINTEN_REGACC, (uint32_t)1 << 31)
#define AM_HAL_IOS_ACCESS_INT_01     _VAL2FLD(IOSLAVE_REGACCINTEN_REGACC, (uint32_t)1 << 30)
#define AM_HAL_IOS_ACCESS_INT_02     _VAL2FLD(IOSLAVE_REGACCINTEN_REGACC, (uint32_t)1 << 29)
#define AM_HAL_IOS_ACCESS_INT_03     _VAL2FLD(IOSLAVE_REGACCINTEN_REGACC, (uint32_t)1 << 28)
#define AM_HAL_IOS_ACCESS_INT_04     _VAL2FLD(IOSLAVE_REGACCINTEN_REGACC, (uint32_t)1 << 27)
#define AM_HAL_IOS_ACCESS_INT_05     _VAL2FLD(IOSLAVE_REGACCINTEN_REGACC, (uint32_t)1 << 26)
#define AM_HAL_IOS_ACCESS_INT_06     _VAL2FLD(IOSLAVE_REGACCINTEN_REGACC, (uint32_t)1 << 25)
#define AM_HAL_IOS_ACCESS_INT_07     _VAL2FLD(IOSLAVE_REGACCINTEN_REGACC, (uint32_t)1 << 24)
#define AM_HAL_IOS_ACCESS_INT_08     _VAL2FLD(IOSLAVE_REGACCINTEN_REGACC, (uint32_t)1 << 23)
#define AM_HAL_IOS_ACCESS_INT_09     _VAL2FLD(IOSLAVE_REGACCINTEN_REGACC, (uint32_t)1 << 22)
#define AM_HAL_IOS_ACCESS_INT_0A     _VAL2FLD(IOSLAVE_REGACCINTEN_REGACC, (uint32_t)1 << 21)
#define AM_HAL_IOS_ACCESS_INT_0B     _VAL2FLD(IOSLAVE_REGACCINTEN_REGACC, (uint32_t)1 << 20)
#define AM_HAL_IOS_ACCESS_INT_0C     _VAL2FLD(IOSLAVE_REGACCINTEN_REGACC, (uint32_t)1 << 19)
#define AM_HAL_IOS_ACCESS_INT_0D     _VAL2FLD(IOSLAVE_REGACCINTEN_REGACC, (uint32_t)1 << 18)
#define AM_HAL_IOS_ACCESS_INT_0E     _VAL2FLD(IOSLAVE_REGACCINTEN_REGACC, (uint32_t)1 << 17)
#define AM_HAL_IOS_ACCESS_INT_0F     _VAL2FLD(IOSLAVE_REGACCINTEN_REGACC, (uint32_t)1 << 16)
#define AM_HAL_IOS_ACCESS_INT_13     _VAL2FLD(IOSLAVE_REGACCINTEN_REGACC, (uint32_t)1 << 15)
#define AM_HAL_IOS_ACCESS_INT_17     _VAL2FLD(IOSLAVE_REGACCINTEN_REGACC, (uint32_t)1 << 14)
#define AM_HAL_IOS_ACCESS_INT_1B     _VAL2FLD(IOSLAVE_REGACCINTEN_REGACC, (uint32_t)1 << 13)
#define AM_HAL_IOS_ACCESS_INT_1F     _VAL2FLD(IOSLAVE_REGACCINTEN_REGACC, (uint32_t)1 << 12)
#define AM_HAL_IOS_ACCESS_INT_23     _VAL2FLD(IOSLAVE_REGACCINTEN_REGACC, (uint32_t)1 << 11)
#define AM_HAL_IOS_ACCESS_INT_27     _VAL2FLD(IOSLAVE_REGACCINTEN_REGACC, (uint32_t)1 << 10)
#define AM_HAL_IOS_ACCESS_INT_2B     _VAL2FLD(IOSLAVE_REGACCINTEN_REGACC, (uint32_t)1 << 9)
#define AM_HAL_IOS_ACCESS_INT_2F     _VAL2FLD(IOSLAVE_REGACCINTEN_REGACC, (uint32_t)1 << 8)
#define AM_HAL_IOS_ACCESS_INT_33     _VAL2FLD(IOSLAVE_REGACCINTEN_REGACC, (uint32_t)1 << 7)
#define AM_HAL_IOS_ACCESS_INT_37     _VAL2FLD(IOSLAVE_REGACCINTEN_REGACC, (uint32_t)1 << 6)
#define AM_HAL_IOS_ACCESS_INT_3B     _VAL2FLD(IOSLAVE_REGACCINTEN_REGACC, (uint32_t)1 << 5)
#define AM_HAL_IOS_ACCESS_INT_3F     _VAL2FLD(IOSLAVE_REGACCINTEN_REGACC, (uint32_t)1 << 4)
#define AM_HAL_IOS_ACCESS_INT_43     _VAL2FLD(IOSLAVE_REGACCINTEN_REGACC, (uint32_t)1 << 3)
#define AM_HAL_IOS_ACCESS_INT_47     _VAL2FLD(IOSLAVE_REGACCINTEN_REGACC, (uint32_t)1 << 2)
#define AM_HAL_IOS_ACCESS_INT_4B     _VAL2FLD(IOSLAVE_REGACCINTEN_REGACC, (uint32_t)1 << 1)
#define AM_HAL_IOS_ACCESS_INT_4F     _VAL2FLD(IOSLAVE_REGACCINTEN_REGACC, (uint32_t)1 << 0)
#define AM_HAL_IOS_ACCESS_INT_ALL    0xFFFFFFFF
//! @}

//*****************************************************************************
//
//! @name I/O Slave Interrupts
//! @brief Macro definitions for I/O slave (IOS) interrupts.
//!
//! These macros may be used with any of the
//!
//! @{
//
//*****************************************************************************
#define AM_HAL_IOS_INT_FSIZE            IOSLAVE_INTEN_FSIZE_Msk
#define AM_HAL_IOS_INT_FOVFL            IOSLAVE_INTEN_FOVFL_Msk
#define AM_HAL_IOS_INT_FUNDFL           IOSLAVE_INTEN_FUNDFL_Msk
#define AM_HAL_IOS_INT_FRDERR           IOSLAVE_INTEN_FRDERR_Msk
#define AM_HAL_IOS_INT_GENAD            IOSLAVE_INTEN_GENAD_Msk
#define AM_HAL_IOS_INT_IOINTW           IOSLAVE_INTEN_IOINTW_Msk
#define AM_HAL_IOS_INT_XCMPWR           IOSLAVE_INTEN_XCMPWR_Msk
#define AM_HAL_IOS_INT_XCMPWF           IOSLAVE_INTEN_XCMPWF_Msk
#define AM_HAL_IOS_INT_XCMPRR           IOSLAVE_INTEN_XCMPRR_Msk
#define AM_HAL_IOS_INT_XCMPRF           IOSLAVE_INTEN_XCMPRF_Msk
#define AM_HAL_IOS_INT_ALL              0xFFFFFFFF
//! @}

//*****************************************************************************
//
//! @name I/O Slave Interrupts triggers
//! @brief Macro definitions for I/O slave (IOS) interrupts.
//!
//! These macros may be used with am_hal_ios_interrupt_set and am_hal_ios_interrupt_clear
//!
//! @{
//
//*****************************************************************************
#define AM_HAL_IOS_IOINTCTL_INT0    (0x01)
#define AM_HAL_IOS_IOINTCTL_INT1    (0x02)
#define AM_HAL_IOS_IOINTCTL_INT2    (0x04)
#define AM_HAL_IOS_IOINTCTL_INT3    (0x08)
#define AM_HAL_IOS_IOINTCTL_INT4    (0x10)
#define AM_HAL_IOS_IOINTCTL_INT5    (0x20)
//! @}

//*****************************************************************************
//
// External variable definitions
//
//*****************************************************************************

//*****************************************************************************
//
//! @brief LRAM pointer
//!
//! Pointer to the base of the IO Slave LRAM.
//
//*****************************************************************************
extern volatile uint8_t * const am_hal_ios_pui8LRAM;

//*****************************************************************************
//
//! @brief Configuration structure for the IO slave module.
//!
//! This structure may be used along with the am_hal_ios_config() function to
//! select key parameters of the IO Slave module. See the descriptions of each
//! parameter within this structure for more information on what they control.
//
//*****************************************************************************
typedef struct
{
    //
    //! Interface Selection
    //!
    //! This word selects the physical behavior of the IO Slave port. For SPI
    //! mode, this word should be the logical OR of one or more of the
    //! following:
    //!
    //!     AM_HAL_IOS_USE_SPI
    //!     AM_HAL_IOS_SPIMODE_0
    //!     AM_HAL_IOS_SPIMODE_1
    //!     AM_HAL_IOS_SPIMODE_2
    //!     AM_HAL_IOS_SPIMODE_3
    //!
    //! For I2C mode, use the logical OR of one or more of these values instead
    //! (where n is the 7 or 10-bit I2C address to use):
    //!
    //!     AM_HAL_IOS_USE_I2C
    //!     AM_HAL_IOS_I2C_ADDRESS(n)
    //!
    //! Also, in any mode, you may OR in this value to reverse the order of
    //! incoming data bits.
    //!
    //!     AM_HAL_IOS_LSB_FIRST
    //
    uint32_t ui32InterfaceSelect;

    //
    //! Read-Only section
    //!
    //! The IO Slave LRAM is split into three main sections. The first section
    //! is a "Direct Write" section, which may be accessed for reads or write
    //! either directly through the Apollo CPU, or over the SPI/I2C bus. The
    //! "Direct Write" section always begins at LRAM offset 0x0. At the end of
    //! the normal "Direct Write" space, there is a "Read Only" space, which is
    //! read/write accessible to the Apollo CPU, but read-only over the I2C/SPI
    //! Bus. This word selects the base address of this "Read Only" space.
    //!
    //! This value may be set to any multiple of 8 between 0x0 and 0x78,
    //! inclusive. For the configuration to be valid, \e ui32ROBase must also
    //! be less than or equal to \e ui32FIFOBase
    //!
    //! @note The address given here is in units of BYTES. Since the location
    //! of the "Read Only" space may only be set in 8-byte increments, this
    //! value must be a multiple of 8.
    //!
    //! For the avoidance of doubt this means 0x80 is 128 bytes. These functions
    //! will shift right by 8 internally.
    //
    uint32_t ui32ROBase;

    //
    //! FIFO section
    //!
    //! After the "Direct Access" and "Read Only" sections is a section of LRAM
    //! allocated to a FIFO. This section is accessible by the Apollo CPU
    //! through the FIFO control registers, and accessible on the SPI/I2C bus
    //! through the 0x7F address. This word selects the base address of the
    //! FIFO space. The FIFO will extend from the address specified here to the
    //! address specified in \e ui32RAMBase.
    //!
    //! This value may be set to any multiple of 8 between 0x0 and 0x78,
    //! inclusive. For the configuration to be valid, \e ui32FIFOBase must also
    //! be greater than or equal to \e ui32ROBase.
    //!
    //! @note The address given here is in units of BYTES. Since the location
    //! of the "FIFO" space may only be set in 8-byte increments, this value
    //! must be a multiple of 8.
    //!
    //! For the avoidance of doubt this means 0x80 is 128 bytes. These functions
    //! will shift right by 8 internally.
    //
    uint32_t ui32FIFOBase;

    //
    //! RAM section
    //!
    //! At the end of the IOS LRAM, the user may allocate a "RAM" space that
    //! can only be accessed by the Apollo CPU. This space will not interact
    //! with the SPI/I2C bus at all, and may be used as general-purpose memory.
    //! Unlike normal SRAM, this section of LRAM will retain its state through
    //! Deep Sleep, so it may be used as a data retention space for
    //! ultra-low-power applications.
    //!
    //! This value may be set to any multiple of 8 between 0x0 and 0x100,
    //! inclusive. For the configuration to be valid, \e ui32RAMBase must also
    //! be greater than or equal to \e ui32FIFOBase.
    //!
    //! @note The address given here is in units of BYTES. Since the location
    //! of the "FIFO" space may only be set in 8-byte increments, this value
    //! must be a multiple of 8.
    //!
    //! For the avoidance of doubt this means 0x80 is 128 bytes. These functions
    //! will shift right by 8 internally.
    //
    uint32_t ui32RAMBase;

    //
    //! FIFO threshold
    //!
    //! The IO Slave module will trigger an interrupt when the number of
    //! entries in the FIFO drops below this number of bytes.
    //
    uint32_t ui32FIFOThreshold;

    //
    // Pointer to an SRAM
    //
    uint8_t *pui8SRAMBuffer;
    uint32_t ui32SRAMBufferCap;
}
am_hal_ios_config_t;

typedef enum
{
    // Request with arg
    AM_HAL_IOS_REQ_HOST_INTSET = 0,
    AM_HAL_IOS_REQ_HOST_INTCLR,
    AM_HAL_IOS_REQ_HOST_INTGET,
    AM_HAL_IOS_REQ_HOST_INTEN_GET,
    AM_HAL_IOS_REQ_READ_GADATA,
    AM_HAL_IOS_REQ_ARG_MAX,

    // Request without arg
    AM_HAL_IOS_REQ_READ_POLL = AM_HAL_IOS_REQ_ARG_MAX,
    AM_HAL_IOS_REQ_FIFO_UPDATE_CTR,
    AM_HAL_IOS_REQ_FIFO_BUF_CLR,
    AM_HAL_IOS_REQ_MAX
} am_hal_ios_request_e;

typedef struct
{
    uint8_t           *pui8Data;
    volatile uint32_t ui32WriteIndex;
    volatile uint32_t ui32ReadIndex;
    volatile uint32_t ui32Length;
    uint32_t          ui32Capacity;
}am_hal_ios_buffer_t;

//*****************************************************************************
//
// External function definitions
//
//*****************************************************************************
extern uint32_t am_hal_ios_uninitialize(void *pHandle);
extern uint32_t am_hal_ios_initialize(uint32_t ui32Module, void **ppHandle);
extern uint32_t am_hal_ios_enable(void *pHandle);
extern uint32_t am_hal_ios_disable(void *pHandle);

// the following interrupts go back to the NVIC
extern uint32_t am_hal_ios_configure(void *pHandle, am_hal_ios_config_t *psConfig);
extern uint32_t am_hal_ios_interrupt_enable(void *pHandle, uint32_t ui32IntMask);
extern uint32_t am_hal_ios_interrupt_disable(void *pHandle, uint32_t ui32IntMask);
extern uint32_t am_hal_ios_interrupt_clear(void *pHandle, uint32_t ui32IntMask);
extern uint32_t am_hal_ios_interrupt_status_get(void *pHandle, bool bEnabledOnly, uint32_t *pui32IntStatus);
extern uint32_t am_hal_ios_interrupt_service(void *pHandle, uint32_t ui32IntMask);
// Returns the number of bytes actually written
extern uint32_t am_hal_ios_fifo_write(void *pHandle, uint8_t *pui8Data, uint32_t ui32NumBytes, uint32_t *pui32WrittenBytes);
extern uint32_t am_hal_ios_fifo_space_used(void *pHandle, uint32_t *pui32UsedSpace);
extern uint32_t am_hal_ios_fifo_space_left(void *pHandle, uint32_t *pui32LeftSpace);

extern uint32_t am_hal_ios_power_ctrl(void *pHandle, am_hal_sysctrl_power_state_e ePowerState, bool bRetainState);
extern uint32_t am_hal_ios_control(void *pHandle, am_hal_ios_request_e eReq, void *pArgs);


#ifdef __cplusplus
}
#endif

#endif // AM_HAL_IOS_H

//*****************************************************************************
//
// End Doxygen group.
//! @}
//
//*****************************************************************************
