//*****************************************************************************
//
//! @file am_hal_iom.h
//!
//! @brief Functions for accessing and configuring the IO Master module
//!
//! @addtogroup hal Hardware Abstraction Layer (HAL)
//! @addtogroup iom3 IO Master (SPI/I2C)
//! @ingroup hal
//! @{

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

#ifndef AM_HAL_IOM_H
#define AM_HAL_IOM_H

#include "am_hal_status.h"
#include "am_hal_sysctrl.h"


//*****************************************************************************
//
//! CMSIS-Style macro for handling a variable IOM module number.
//
//*****************************************************************************
#define IOMn(n) ((IOM0_Type*)(IOM0_BASE + (n * (IOM1_BASE - IOM0_BASE))))

//
// Maximum time to wait for hardware to finish a blocking transaction
// This is an escape to allow for bailing out in case of faulty peripheral
// (e.g. a device pulling the I2C clock low)
//
#define AM_HAL_IOM_MAX_BLOCKING_WAIT    500000 // 0.5 sec

//
//  AM_HAL_IOM_CQ=1 will use the Command Queue in nonblocking transfers.
//         0 uses plain DMA (w/o CQ) in nonblocking transfers.
//  This should be enabled only for A1 silicon.
//
#define AM_HAL_IOM_CQ                   1

// Size guideline for allocation of application supploed buffers
#define AM_HAL_IOM_CQ_ENTRY_SIZE               (24 * sizeof(uint32_t))
#define AM_HAL_IOM_HIPRIO_ENTRY_SIZE           (8 * sizeof(uint32_t))

#define AM_HAL_IOM_SC_CLEAR(flag)              ((flag) << 16)
#define AM_HAL_IOM_SC_SET(flag)                ((flag))

// For IOM - Need to Clear the flag for unpausing
#define AM_HAL_IOM_SC_UNPAUSE(flag)            AM_HAL_IOM_SC_CLEAR(flag)
#define AM_HAL_IOM_SC_PAUSE(flag)              AM_HAL_IOM_SC_SET(flag)

// Use this macro to directly control the flags
#define AM_HAL_IOM_SETCLR(iom, scVal)           \
    do {                                        \
        IOMn((iom))->CQSETCLEAR = (scVal);      \
    } while (0);

// Flags 5, 7 & 6 are reserved by HAL
#define AM_HAL_IOM_PAUSE_FLAG_RESV             (IOM0_CQPAUSEEN_CQPEN_SWFLAGEN7 | IOM0_CQPAUSEEN_CQPEN_SWFLAGEN6 | IOM0_CQPAUSEEN_CQPEN_SWFLAGEN5)
#define AM_HAL_IOM_SC_RESV_MASK                (AM_HAL_IOM_PAUSE_FLAG_RESV | (AM_HAL_IOM_PAUSE_FLAG_RESV << 8) | (AM_HAL_IOM_PAUSE_FLAG_RESV << 16))


// We use SWFLAGEN7 to control SW pausing Command Queue - default unPause
// We use SWFLAGEN6 to pause on the sequece loopback - default Pause
#define AM_HAL_IOM_PAUSE_FLAG_IDX      (_VAL2FLD(IOM0_CQPAUSEEN_CQPEN, IOM0_CQPAUSEEN_CQPEN_IDXEQ))
#define AM_HAL_IOM_PAUSE_FLAG_CQ       (_VAL2FLD(IOM0_CQPAUSEEN_CQPEN, IOM0_CQPAUSEEN_CQPEN_SWFLAGEN7))
#define AM_HAL_IOM_PAUSE_FLAG_SEQLOOP  (_VAL2FLD(IOM0_CQPAUSEEN_CQPEN, IOM0_CQPAUSEEN_CQPEN_SWFLAGEN6))
#define AM_HAL_IOM_PAUSE_FLAG_BLOCK    (_VAL2FLD(IOM0_CQPAUSEEN_CQPEN, IOM0_CQPAUSEEN_CQPEN_SWFLAGEN5))

// By default - we Pause CQ for no more entries, or force pause from SW
#define AM_HAL_IOM_PAUSE_DEFAULT       AM_HAL_IOM_PAUSE_FLAG_IDX
#define AM_HAL_IOM_CQP_PAUSE_DEFAULT   (AM_HAL_IOM_PAUSE_FLAG_IDX | AM_HAL_IOM_PAUSE_FLAG_CQ)

//*****************************************************************************
//
// IOM Specific status codes
//
//*****************************************************************************
typedef enum
{
    // Error in hardware command issued or illegal access by SW
    AM_HAL_IOM_ERR_INVALID_OPER = AM_HAL_STATUS_MODULE_SPECIFIC_START,
    // Loss of I2C multi-master arbitration
    AM_HAL_IOM_ERR_I2C_ARB,
    // I2C NAK
    AM_HAL_IOM_ERR_I2C_NAK,
} am_hal_iom_err_e;

//*****************************************************************************
//
// General defines
//
//*****************************************************************************
#define AM_HAL_IOM_FIFO_SIZE_MAX        32
#define AM_HAL_IOM_MAX_OFFSETSIZE       3
#define AM_HAL_IOM_MAX_TXNSIZE_SPI      4095
#define AM_HAL_IOM_MAX_TXNSIZE_I2C      4095
#define AM_HAL_IOM_MAX_CS_SPI           3

//*****************************************************************************
//
//! @brief enumeration types for the IOM.
//
//*****************************************************************************

//
// IOM mode enumerations
//
typedef enum
{
    AM_HAL_IOM_SPI_MODE,
    AM_HAL_IOM_I2C_MODE,
    AM_HAL_IOM_NUM_MODES
} am_hal_iom_mode_e;

//
// Transmit or receive enumerations.
// Make these enums consistent with the IOM CMD register values.
//
typedef enum
{
    AM_HAL_IOM_TX,
    AM_HAL_IOM_RX,
    AM_HAL_IOM_FULLDUPLEX,
} am_hal_iom_dir_e;

//
// Enumerate the SPI modes.  Note that these are arranged per the ordering of
// SPHA (bit1) and SPOL (bit0) in the IOM.MSPICFG register.
//
typedef enum
{
    AM_HAL_IOM_SPI_MODE_0,                  // CPOL = 0; CPHA = 0
    AM_HAL_IOM_SPI_MODE_2,                  // CPOL = 1; CPHA = 0
    AM_HAL_IOM_SPI_MODE_1,                  // CPOL = 0; CPHA = 1
    AM_HAL_IOM_SPI_MODE_3,                  // CPOL = 1; CPHA = 1
} am_hal_iom_spi_mode_e;


//*****************************************************************************
//
//! @brief Transfer callback function prototype
//
//*****************************************************************************
typedef void (*am_hal_iom_callback_t)(void *pCallbackCtxt, uint32_t transactionStatus);
//
//*****************************************************************************
//
//! @brief Configuration structure for the IOM.
//
//*****************************************************************************
typedef struct
{
    //
    //! Select the interface mode, SPI or I2C
    //
    am_hal_iom_mode_e eInterfaceMode;

    //
    //! Select the interface clock frequency
    //
    uint32_t ui32ClockFreq;

    //
    //! Select the SPI clock mode (polarity/phase). Ignored for I2C operation.
    //
    am_hal_iom_spi_mode_e eSpiMode;

    //
    // Non-Blocking transaction memory configuration
    // Set length and pointer to Transfer Control Buffer.
    // Length is in 4 byte multiples
    //
    uint32_t *pNBTxnBuf;
    uint32_t ui32NBTxnBufLength;
}
am_hal_iom_config_t;

//*****************************************************************************
//
//! Configuration structure for an individual SPI device.
//
//*****************************************************************************
typedef struct
{
    //
    //! IOM module to use for communicating with this device.
    //
    uint32_t ui32Module;

    //
    //! Chip select signal that should be used for this device.
    //
    uint32_t ui32ChipSelect;

    //
    //! Additional options that will ALWAYS be ORed into the command word.
    //
    uint32_t ui32Options;
}
am_hal_iom_spi_device_t;

//*****************************************************************************
//
//! Configuration structure for an individual I2C device.
//
//*****************************************************************************
typedef struct
{
    //
    //! IOM module to use for communicating with this device.
    //
    uint32_t ui32Module;

    //
    //! I2C address associated with this device.
    //
    uint32_t ui32BusAddress;

    //
    //! Additional options that will ALWAYS be ORed into the command word.
    //
    uint32_t ui32Options;
}
am_hal_iom_i2c_device_t;

//*****************************************************************************
//
//! @brief Status structure for the IOM.
//
//*****************************************************************************
typedef struct
{
    //
    // IOM status.
    //
    bool     bStatIdle;
    bool     bStatCmdAct;
    bool     bStatErr;

    //!
    //! DMA status
    //! One of:
    //!     AM_HAL_IOM_STATUS_DMA_IN_PROGRESS
    //!     AM_HAL_IOM_STATUS_XFER_COMPLETE
    //!     AM_HAL_IOM_STATUS_DMAERR
    //!
    uint32_t ui32DmaStat;

    uint32_t ui32MaxTransactions;
    uint32_t ui32NumPendTransactions;
}
am_hal_iom_status_t;

//
// transfer structure
//
typedef struct
{
    union
    {
        //
        //! Chip enable (chip select) for this transaction on this device.
        //
        uint32_t ui32SpiChipSelect;
        uint32_t ui32I2CDevAddr;
    } uPeerInfo;

    //
    //! Instruction length (0,1,2, or 3).
    //
    uint32_t ui32InstrLen;

    //
    //! Device Instruction (aka Command). Often used as the offset.
    //
    uint32_t ui32Instr;

    //
    //! Number of bytes to transfer
    //
    uint32_t ui32NumBytes;

    //
    //! Transfer Direction (Transmit/Receive)
    //
    am_hal_iom_dir_e eDirection;

    //
    //! Buffer
    //
    uint32_t *pui32TxBuffer;
    uint32_t *pui32RxBuffer;

    //
    // Continue - holds the SPI or I2C bus for multiple transactions.
    //
    bool     bContinue;

    //
    // Repeat Count
    //
    uint8_t  ui8RepeatCount;

    //
    //! DMA: Priority 0 = Low (best effort); 1 = High (service immediately)
    //
    uint8_t  ui8Priority;

    //! Command Queue Advanced control on gating conditions for transaction to start
    //
    uint32_t              ui32PauseCondition;
    //! Command Queue Advanced Post-Transaction status setting
    uint32_t              ui32StatusSetClr;

} am_hal_iom_transfer_t;

typedef struct
{
    bool                        bLoop;
    //! Command Queue Transaction Gating
    uint32_t                    ui32PauseCondition;
    //! Command Queue Post-Transaction status setting
    uint32_t                    ui32StatusSetClr;
} am_hal_iom_seq_end_t;

typedef struct
{
  uint8_t                     *pBuf; // Buffer provided to store the high priority transaction context
  uint32_t                    size; // Size of buffer in bytes
} am_hal_iom_hiprio_cfg_t;

#define AM_HAL_IOM_DCX_INVALID  0xFF
typedef struct
{
    uint8_t     cs; // CS for which this configuration applies
    uint8_t     dcx; // alternate CS line used for DCX - AM_HAL_IOM_DCX_INVALID indicates DCX is not used
} am_hal_iom_dcx_cfg_t;

typedef struct
{
    //! Command Queue Advanced control on gating conditions for transaction to start
    uint32_t              ui32PauseCondition;
    //! Command Queue Advanced Post-Transaction status setting
    uint32_t              ui32StatusSetClr;
    am_hal_cmdq_entry_t    *pCQEntry;
    uint32_t               numEntries;
    am_hal_iom_callback_t  pfnCallback;
    void                   *pCallbackCtxt;
    uint32_t               *pJmpAddr;
} am_hal_iom_cq_raw_t;

typedef enum
{
    // Used to set/clear 8 CQ Pause flags - reserved flags are defined as AM_HAL_IOM_PAUSE_FLAG_RESV
    // Pass uint32_t as pArgs
    AM_HAL_IOM_REQ_FLAG_SETCLR = 0,
    // Pass uint32_t as pArgs
    AM_HAL_IOM_REQ_SPI_LSB,
    // Pass uint32_t as pArgs
    AM_HAL_IOM_REQ_SPI_FULLDUPLEX,
    // Pass uint32_t as pArgs
    AM_HAL_IOM_REQ_SPI_RDTHRESH,
    // Pass uint32_t as pArgs
    AM_HAL_IOM_REQ_SPI_WRTHRESH,
    // Pause the CQ gracefully
    // pArgs N/A
    AM_HAL_IOM_REQ_PAUSE,
    // Unpause the CQ
    // pArgs N/A
    AM_HAL_IOM_REQ_UNPAUSE,
    // Get in and out of Sequence Mode - which allows building a sequence, which either runs once, or repeats
    // Pass in bool as pArgs - true/false
    AM_HAL_IOM_REQ_SET_SEQMODE,
    // pArgs N/A
    AM_HAL_IOM_REQ_SEQ_END,
    // Initialize configuration for high priority trasactions
    // These transactions take precedence over existing CQ transactions
    // Pass am_hal_iom_hiprio_cfg_t * as pArgs
    AM_HAL_IOM_REQ_INIT_HIPRIO,
    // Create a block of transactions which are not paused in between
    // pArgs N/A
    AM_HAL_IOM_REQ_START_BLOCK,
    // pArgs N/A
    AM_HAL_IOM_REQ_END_BLOCK,
    // Control the DCX line
    // Pass am_hal_iom_dcx_cfg_t * as pArgs
    AM_HAL_IOM_REQ_SET_DCX,
    // Raw CQ transaction
    // Pass am_hal_iom_cq_raw_t * as pArgs
    AM_HAL_IOM_REQ_CQ_RAW,
    AM_HAL_IOM_REQ_MAX
} am_hal_iom_request_e;

#define am_hal_iom_buffer(A)            \
union                                   \
{                                       \
    uint32_t words[(A + 3) >> 2];       \
    uint8_t bytes[A];                   \
}

//*****************************************************************************
//
//! @name IOM Clock Frequencies
//! @brief Macro definitions for common SPI and I2C clock frequencies.
//!
//! These macros may be used with the ui32ClockFrequency member of the
//! am_hal_iom_config_t structure to set the clock frequency of the serial
//! interfaces.
//!
//! This list of frequencies is not exhaustive by any means. If your desired
//! frequency is not in this list, simply set ui32ClockFrequency to the
//! desired frequency (in Hz) when calling am_hal_iom_config().
//
//*****************************************************************************
#define AM_HAL_IOM_48MHZ    48000000
#define AM_HAL_IOM_24MHZ    24000000
#define AM_HAL_IOM_16MHZ    16000000
#define AM_HAL_IOM_12MHZ    12000000
#define AM_HAL_IOM_8MHZ      8000000
#define AM_HAL_IOM_6MHZ      6000000
#define AM_HAL_IOM_4MHZ      4000000
#define AM_HAL_IOM_3MHZ      3000000
#define AM_HAL_IOM_2MHZ      2000000
#define AM_HAL_IOM_1_5MHZ    1500000
#define AM_HAL_IOM_1MHZ      1000000
#define AM_HAL_IOM_750KHZ     750000
#define AM_HAL_IOM_500KHZ     500000
#define AM_HAL_IOM_400KHZ     400000
#define AM_HAL_IOM_375KHZ     375000
#define AM_HAL_IOM_250KHZ     250000
#define AM_HAL_IOM_125KHZ     125000
#define AM_HAL_IOM_100KHZ     100000
#define AM_HAL_IOM_50KHZ       50000
#define AM_HAL_IOM_10KHZ       10000

// Max Frequency supported in HAL
#define AM_HAL_IOM_MAX_FREQ AM_HAL_IOM_48MHZ

//*****************************************************************************
//
// IOM Interrupts
//
//*****************************************************************************
#define AM_HAL_IOM_INT_CQERR            IOM0_INTEN_CQERR_Msk        // Error during command queue operations
#define AM_HAL_IOM_INT_CQUPD            IOM0_INTEN_CQUPD_Msk        // Command queue operation  performed a register write with the register address bit 0 set to 1.
#define AM_HAL_IOM_INT_CQPAUSED         IOM0_INTEN_CQPAUSED_Msk     // Command queue operation paused
#define AM_HAL_IOM_INT_DERR             IOM0_INTEN_DERR_Msk         // DMA error received
#define AM_HAL_IOM_INT_DCMP             IOM0_INTEN_DCMP_Msk         // DMA transfer complete
#define AM_HAL_IOM_INT_ARB              IOM0_INTEN_ARB_Msk          // Arbitration loss
#define AM_HAL_IOM_INT_STOP             IOM0_INTEN_STOP_Msk         // STOP command
#define AM_HAL_IOM_INT_START            IOM0_INTEN_START_Msk        // START command
#define AM_HAL_IOM_INT_ICMD             IOM0_INTEN_ICMD_Msk         // ILLEGAL command
#define AM_HAL_IOM_INT_IACC             IOM0_INTEN_IACC_Msk         // Illegal FIFO access
#define AM_HAL_IOM_INT_NAK              IOM0_INTEN_NAK_Msk          // I2C NAK
#define AM_HAL_IOM_INT_FOVFL            IOM0_INTEN_FOVFL_Msk        // Write FIFO overflow
#define AM_HAL_IOM_INT_FUNDFL           IOM0_INTEN_FUNDFL_Msk       // Read FIFO underflow
#define AM_HAL_IOM_INT_THR              IOM0_INTEN_THR_Msk          // FIFO threshold interrupt
#define AM_HAL_IOM_INT_CMDCMP           IOM0_INTEN_CMDCMP_Msk       // Command complete


#define AM_HAL_IOM_INT_SWERR            (AM_HAL_IOM_INT_ICMD | AM_HAL_IOM_INT_IACC | AM_HAL_IOM_INT_FOVFL | AM_HAL_IOM_INT_FUNDFL)
#define AM_HAL_IOM_INT_I2CARBERR        (AM_HAL_IOM_INT_ARB)
#define AM_HAL_IOM_INT_INTERR           (AM_HAL_IOM_INT_CQERR | AM_HAL_IOM_INT_DERR)
#define AM_HAL_IOM_INT_ALL              0xFFFFFFFF
//
// Unsuccessful end of a transaction results in one more more of the following
//
#define AM_HAL_IOM_INT_ERR          (AM_HAL_IOM_INT_SWERR | AM_HAL_IOM_INT_I2CARBERR | AM_HAL_IOM_INT_INTERR | AM_HAL_IOM_INT_NAK)

#ifdef __cplusplus
extern "C"
{
#endif

//*****************************************************************************
//
//! @brief IOM initialization function
//!
//! @param ui32Module   - module instance.
//! @param handle       - returns the handle for the module instance.
//!
//! This function accepts a module instance, allocates the interface and then
//! returns a handle to be used by the remaining interface functions.
//!
//! @return status      - generic or interface specific status.
//
//*****************************************************************************
extern uint32_t am_hal_iom_initialize(uint32_t ui32Module, void **ppHandle);

//*****************************************************************************
//
//! @brief IOM configuration function
//!
//! @param handle       - handle for the IOM.
//! @param pConfig      - pointer to the IOM specific configuration.
//!
//! This function configures the interface settings for the IO Master.
//!
//! @return status      - generic or interface specific status.
//
//*****************************************************************************
extern uint32_t am_hal_iom_configure(void *pHandle, am_hal_iom_config_t *psConfig);

//*****************************************************************************
//
//! @brief IOM enable function
//!
//! @param handle       - handle for the interface.
//!
//! This function enables the IOM for operation.
//!
//! @return status      - generic or interface specific status.
//
//*****************************************************************************
extern uint32_t am_hal_iom_enable(void *pHandle);

//*****************************************************************************
//
//! @brief IOM disable function
//!
//! @param handle       - handle for the interface.
//!
//! This function disables the IOMaster from operation.
//!
//! @return status      - generic or interface specific status.
//
//*****************************************************************************
extern uint32_t am_hal_iom_disable(void *pHandle);

//*****************************************************************************
//
//! @brief IOM control function
//!
//! @param handle       - handle for the IOM.
//! @param eReq         - device specific special request code.
//! @param pArgs        - pointer to the request specific arguments.
//!
//! This function allows advanced settings
//!
//! @return status      - generic or interface specific status.
//
//*****************************************************************************
extern uint32_t am_hal_iom_control(void *pHandle, am_hal_iom_request_e eReq, void *pArgs);

//*****************************************************************************
//
//! @brief IOM status function
//!
//! @param handle       - handle for the interface.
//! @param psStatus     - pointer to an interface specific structure used to
//!                       return the status of the interface.
//!
//! This function returns the current status of the interface.
//!
//! @return status      - generic or interface specific status.
//
//*****************************************************************************
extern uint32_t am_hal_iom_status_get(void *pHandle, am_hal_iom_status_t *psStatus);

//*****************************************************************************
//
//! @brief IOM enable interrupts function
//!
//! @param handle       - handle for the interface.
//! @param ui32IntMask  - interface specific interrupt mask.
//!
//! This function enables the specific indicated interrupts.
//!
//! The following are valid enable bits, any of which can be ORed together.
//!   AM_REG_IOM_INTEN_CQERR_M      // Error during command queue operations
//!   AM_REG_IOM_INTEN_CQCMP_M      // Command queue operation complete
//!   AM_REG_IOM_INTEN_DERR_M       // DMA error received
//!   AM_REG_IOM_INTEN_DCMP_M       // DMA transfer complete
//!   AM_REG_IOM_INTEN_ARB_M        // Arbitration loss
//!   AM_REG_IOM_INTEN_STOP_M       // STOP command
//!   AM_REG_IOM_INTEN_START_M      // START command
//!   AM_REG_IOM_INTEN_ICMD         // ILLEGAL command
//!   AM_REG_IOM_INTEN_IACC_M       // Illegal FIFO access
//!   AM_REG_IOM_INTEN_NAK_M        // I2C NAK
//!   AM_REG_IOM_INTEN_FOVFL_M      // Write FIFO overflow
//!   AM_REG_IOM_INTEN_FUNDFL_M     // Read FIFO underflow
//!   AM_REG_IOM_INTEN_THR_M        // FIFO threshold interrupt
//!   AM_REG_IOM_INTEN_CMDCMP_M     // Command complete
//!
//! @return status      - generic or interface specific status.
//
//*****************************************************************************
extern uint32_t am_hal_iom_interrupt_enable(void *pHandle, uint32_t ui32IntMask);

//*****************************************************************************
//
//! @brief IOM disable interrupts function
//!
//! @param handle       - handle for the interface.
//! @param ui32IntMask  - interface specific interrupt mask.
//!
//! This function disables the specified interrupts.
//!
//! @return status      - generic or interface specific status.
//!
//! The following are valid disable bits, any of which can be ORed together.
//!   AM_REG_IOM_INTEN_CQERR_M      // Error during command queue operations
//!   AM_REG_IOM_INTEN_CQCMP_M      // Command queue operation complete
//!   AM_REG_IOM_INTEN_DERR_M       // DMA error received
//!   AM_REG_IOM_INTEN_DCMP_M       // DMA transfer complete
//!   AM_REG_IOM_INTEN_ARB_M        // Arbitration loss
//!   AM_REG_IOM_INTEN_STOP_M       // STOP command
//!   AM_REG_IOM_INTEN_START_M      // START command
//!   AM_REG_IOM_INTEN_ICMD         // ILLEGAL command
//!   AM_REG_IOM_INTEN_IACC_M       // Illegal FIFO access
//!   AM_REG_IOM_INTEN_NAK_M        // I2C NAK
//!   AM_REG_IOM_INTEN_FOVFL_M      // Write FIFO overflow
//!   AM_REG_IOM_INTEN_FUNDFL_M     // Read FIFO underflow
//!   AM_REG_IOM_INTEN_THR_M        // FIFO threshold interrupt
//!   AM_REG_IOM_INTEN_CMDCMP_M     // Command complete
//
//*****************************************************************************
extern uint32_t am_hal_iom_interrupt_disable(void *pHandle, uint32_t ui32IntMask);

//*****************************************************************************
//
//! @brief IOM get interrupt status
//!
//! @param handle         - handle for the interface.
//! @param pui32IntStatus - pointer to a uint32_t to return the interrupt status
//!
//! This function returns the interrupt status for the given peripheral.
//!
//! The following are valid status bits.
//!   AM_REG_IOM_INTSTAT_CQERR_M    // Error during command queue operations
//!   AM_REG_IOM_INTSTAT_CQCMP_M    // Command queue operation complete
//!   AM_REG_IOM_INTSTAT_DERR_M     // DMA error received
//!   AM_REG_IOM_INTSTAT_DCMP_M     // DMA transfer complete
//!   AM_REG_IOM_INTSTAT_ARB_M      // Arbitration loss
//!   AM_REG_IOM_INTSTAT_STOP_M     // STOP command
//!   AM_REG_IOM_INTSTAT_START_M    // START command
//!   AM_REG_IOM_INTSTAT_ICMD       // ILLEGAL command
//!   AM_REG_IOM_INTSTAT_IACC_M     // Illegal FIFO access
//!   AM_REG_IOM_INTSTAT_NAK_M      // I2C NAK
//!   AM_REG_IOM_INTSTAT_FOVFL_M    // Write FIFO overflow
//!   AM_REG_IOM_INTSTAT_FUNDFL_M   // Read FIFO underflow
//!   AM_REG_IOM_INTSTAT_THR_M      // FIFO threshold interrupt
//!   AM_REG_IOM_INTSTAT_CMDCMP_M   // Command complete
//!
//! @return status      - generic or interface specific status.
//
//*****************************************************************************
extern uint32_t am_hal_iom_interrupt_status_get(void *pHandle, bool bEnabledOnly,
                                                uint32_t *pui32IntStatus);

//*****************************************************************************
//
//! @brief IOM interrupt clear
//!
//! @param handle         - handle for the interface.
//! @param ui32IntMask  - interface specific interrupt mask.
//!
//! This function clears the interrupts for the given peripheral.
//!
//! The following are valid clear bits, any of which can be ORed together.
//!   AM_REG_IOM_INTCLR_CQERR_M     // Error during command queue operations
//!   AM_REG_IOM_INTCLR_CQCMP_M     // Command queue operation complete
//!   AM_REG_IOM_INTCLR_DERR_M      // DMA error received
//!   AM_REG_IOM_INTCLR_DCMP_M      // DMA transfer complete
//!   AM_REG_IOM_INTCLR_ARB_M       // Arbitration loss
//!   AM_REG_IOM_INTCLR_STOP_M      // STOP command
//!   AM_REG_IOM_INTCLR_START_M     // START command
//!   AM_REG_IOM_INTCLR_ICMD        // ILLEGAL command
//!   AM_REG_IOM_INTCLR_IACC_M      // Illegal FIFO access
//!   AM_REG_IOM_INTCLR_NAK_M       // I2C NAK
//!   AM_REG_IOM_INTCLR_FOVFL_M     // Write FIFO overflow
//!   AM_REG_IOM_INTCLR_FUNDFL_M    // Read FIFO underflow
//!   AM_REG_IOM_INTCLR_THR_M       // FIFO threshold interrupt
//!   AM_REG_IOM_INTCLR_CMDCMP_M    // Command complete
//!
//! @return status      - generic or interface specific status.
//
//*****************************************************************************
extern uint32_t am_hal_iom_interrupt_clear(void *pHandle, uint32_t ui32IntMask);

//*****************************************************************************
//
//! @brief IOM interrupt service routine
//!
//! @param handle       - handle for the interface.
//! @param ui32IntMask  - interface specific interrupt mask indicating
//!                       interrupts to be serviced
//!
//! This function is designed to be called from within the user defined ISR
//! (am_iom_isr) in order to service the non-blocking, queued, or DMA processing
//! for a given module instance.
//!
//! @return status      - generic or interface specific status.
//
//*****************************************************************************
extern uint32_t am_hal_iom_interrupt_service(void *pHandle, uint32_t ui32IntMask);

//*****************************************************************************
//
//! @brief IOM power control function
//!
//! @param handle       - handle for the interface.
//! @param ePowerState  - the desired power state to move the peripheral to.
//! @param retainState  - flag (if true) to save/restore perhipheral state upon
//!                       power state change.
//!
//! This function updates the peripheral to a given power state.
//!
//! @return status      - generic or interface specific status.
//
//*****************************************************************************
extern uint32_t am_hal_iom_power_ctrl(void *pHandle,
                                      am_hal_sysctrl_power_state_e ePowerState,
                                      bool retainState);

//*****************************************************************************
//
//! @brief IOM blocking transfer function
//!
//! @param handle       - handle for the interface.
//! @param pTransaction - pointer to the transaction control structure.
//!
//! This function performs a transaction on the IOM in PIO mode.  It handles
//! half duplex transactions only (TX or RX).
//!
//! @return status      - generic or interface specific status.
//
//*****************************************************************************
extern uint32_t am_hal_iom_blocking_transfer(void *pHandle,
                                             am_hal_iom_transfer_t *psTransaction);

//*****************************************************************************
//
//! @brief IOM non-blocking transfer function
//!
//! @param handle       - handle for the interface.
//! @param pTransaction - pointer to the uniform transaction control structure.
//! @param pfnCallback  - pointer the callback function to be executed when
//!                       transaction is complete can be set to NULL).
//! @param pCallbackCtxt- context registered which is passed on to the callback
//!                       function
//!
//! This function performs a transaction on the interface.  It handles both full
//! and half duplex transactions.  The callback is executed when the transaction
//! is complete.
//!
//! @return status      - generic or interface specific status.
//
//*****************************************************************************
extern uint32_t am_hal_iom_nonblocking_transfer(void *pHandle,
                                                am_hal_iom_transfer_t *psTransaction,
                                                am_hal_iom_callback_t pfnCallback,
                                                void *pCallbackCtxt);

//*****************************************************************************
//
//! @brief IOM uninitialize function
//!
//! @param handle       - returns the handle for the module instance.
//!
//! This function accepts a handle to the initialized interface and returns
//! the peripheral instance to a known uninitialized state.
//!
//! @return status      - generic or interface specific status.
//
//*****************************************************************************
//
// Uninitialize the interface and return the handle to a known state.
//
extern uint32_t am_hal_iom_uninitialize(void *pHandle);

//*****************************************************************************
//
//! @brief Perform a Full Duplex transaction.
//!
//! @param handle       - handle for the interface.
//!
//! @return HAL status of the operation.
//
//*****************************************************************************
uint32_t
am_hal_iom_spi_blocking_fullduplex(void *pHandle,
                                   am_hal_iom_transfer_t *psTransaction);
//
// IOM High Priority transfer function
//
uint32_t am_hal_iom_highprio_transfer(void *pHandle,
                                      am_hal_iom_transfer_t *psTransaction,
                                      am_hal_iom_callback_t pfnCallback,
                                      void *pCallbackCtxt);


#ifdef __cplusplus
}
#endif

//*****************************************************************************
//
// End Doxygen group.
//! @}
//
//*****************************************************************************
#endif // AM_HAL_IOM_H
