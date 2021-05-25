/***************************************************************************//**
* \file cy_mxsdiodev.h
* \version 1.0
*
* This file provides an API declaration of the MXSDIODEV driver
*
********************************************************************************
* \copyright
* Copyright 2020 Cypress Semiconductor Corporation
* SPDX-License-Identifier: Apache-2.0
*
* Licensed under the Apache License, Version 2.0 (the "License");
* you may not use this file except in compliance with the License.
* You may obtain a copy of the License at
*
*     http://www.apache.org/licenses/LICENSE-2.0
*
* Unless required by applicable law or agreed to in writing, software
* distributed under the License is distributed on an "AS IS" BASIS,
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
* See the License for the specific language governing permissions and
* limitations under the License.
*******************************************************************************/

/**
* \addtogroup group_sdio_dev
* \{
* **MXSDIODEV**: The SDIO 3.0 Device (mxsdiodev) is a IP core that has been
* previously integrated in many of Cypress's WLAN Chip tapeouts. The IP is designed to mate
* with standards compliant to SDIO Host. SDIO Device has in built DMA for external host communication.
* 
* - Supports SDIO version 3.0 including the new UHS-I modes
*    - DS: Default speed (DS) up to 25 MHz, including 1- and 4-bit modes (3.3V signaling).
*    - HS: High-speed up to 50 MHz (3.3V signaling).
*    - SDR12: SDR up to 25 MHz (1.8V signaling).
*    - SDR25: SDR up to 50 MHz (1.8V signaling).
*    - SDR50: SDR up to 100 MHz (1.8V signaling).
* - Backwards compatible with SDIO 2.0 Hosts
* - The following three functions are supported:
*   - Function 0 Standard SDIO function (max. BlockSize/ByteCount = 32B)
*   - Function 1 Backplane Function to access the internal SoC address space (max. BlockSize/ByteCount = 64B)
*   - Function 2 WLAN Function for efficient WLAN packet transfer through DMA (max. BlockSize/ByteCount = 512B)
* - AHB Lite DMA capable Master
* - AHB Lite Slave for MMIO programming
*
* Host software uses Function 2 to Read and Write framed data from/to the sdio device.
*
* \section group_sdio_dev_more_information More Information
* For more information on the SDIO Device, refer to the technical reference manual (TRM).
*
* \section group_sdio_dev_MISRA MISRA-C Compliance
* The SDIO Device driver does not have any specific deviations.
*
* \section group_sdio_dev_Changelog Changelog
* <table class="doxtable">
*   <tr><th>Version</th><th>Changes</th><th>Reason for Change</th></tr>
*   <tr>
*     <td>1.0</td>
*     <td>Initial version</td>
*     <td></td>
*   </tr>
* </table>
*
* \defgroup group_sdio_dev_macros_general_purpose General Purpose Macros
* \defgroup group_sdio_dev_macros Macros
* \defgroup group_sdio_dev_functions Functions
* \defgroup group_sdio_dev_interrupt_functions Interrupt
* \defgroup group_sdio_dev_data_structures Data Structures
* \defgroup group_sdio_dev_enums Enumerated Types
*/
#if !defined (CY_MXSDIODEV_H)
#define CY_MXSDIODEV_H

#include "cy_device.h"

#if defined (CY_IP_MXSDIODEV)

#include <stddef.h>
#include "cy_syslib.h"

#if defined(__cplusplus)
extern "C" {
#endif

/**
* \addtogroup group_sdio_dev_macros_general_purpose
* \{
*/

/** Driver major version */
#define CY_SDIO_DEV_DRV_VERSION_MAJOR       1

/** Driver minor version */
#define CY_SDIO_DEV_DRV_VERSION_MINOR       0

/** DMA Channel */
#define MXSDIODEV_DMA_CHANNEL                0

/******************************************************************************
* API Constants
******************************************************************************/

/** SDIO Device ID */
#define CY_SDIO_DEV_ID                         (CY_PDL_DRV_ID(0x51U))

/** Manufacturer ID Size in tuples (2 bytes)*/
#define CY_SDIO_DEV_MANFID_SIZE                (3)        

/** Frame tag length */
#define CY_SDIO_DEV_FRAMETAG_LEN            (4)

/* rx header flags */
/** CRC error detected */
#define RXF_CRC        0x0001
/** write frame out of sync */
#define RXF_WOOS    0x0002        
/** write frame terminated */
#define RXF_WF_TERM    0x0004      
/** write frame aborted */  
#define RXF_ABORT    0x0008        
/** bad frame */
#define RXF_DISCARD    (RXF_CRC | RXF_WOOS | RXF_WF_TERM | RXF_ABORT)    

/**
* \defgroup group_sdio_dev_macros_callback_events SDIO Device Callback Events
* \{
* Macros to check SDIO events passed by \ref cy_cb_sdio_dev_handle_events_t callback.
* Each event is encoded in a separate bit, and therefore it is possible to
* notify about multiple events.
*/

/**
* Indicates the slave read packet from master
*/
#define CY_SDIO_DEV_RECEIVE_CMPLT_EVENT        (0x00000010UL)

/**
* Indicates the slave transmitted packet
*/
#define CY_SDIO_DEV_TRANSMIT_CMPLT_EVENT        (0x00000020UL)

/**
* Indicates the receive packet error
*/
#define CY_SDIO_DEV_RECEIVE_PKT_ERROR             (0x00000030UL)

/** \} group_sdio_dev_macros_callback_events */

/** \cond INTERNAL */

#define CY_SDIO_DEV_RETRY_TIME                (1000UL) /* The number loops to make the timeout in msec. */
#define CY_SDIO_DEV_TIMEOUT_MS                (3U)     /* The Command complete timeout. */

/* Interrupt Status */
#define    I_ERRORS            (MXSDIO_INTSTATUS_DESCERR_Msk | \
                             MXSDIO_INTSTATUS_RCVDESCUF_Msk | \
                             MXSDIO_INTSTATUS_RCVFIFOOF_Msk | \
                             MXSDIO_INTSTATUS_XMTFIFOUF_Msk)    /** DMA Errors */

/* Default Interrupt mask */
#define DEF_INTMASK            (I_ERRORS | \
                             MXSDIO_INTSTATUS_TOSBMAIL_Msk | \
                             MXSDIO_INTSTATUS_RCVINT_Msk | \
                             MXSDIO_INTSTATUS_XMTINT_Msk | \
                             MXSDIO_INTSTATUS_RDOUTOFSYNC_Msk)

#define DEF_SDIO_INTMASK    (DEF_INTMASK | \
                             MXSDIO_INTSTATUS_SRESET_Msk | \
                             MXSDIO_INTSTATUS_IOE2CHANGE_Msk)

/* tohostmailbox bits corresponding to intstatus bits */
#define HMB_INT_ACK    (1 << 0)    /* To Host Mailbox Dev Interrupt ACK */
#define HMB_FRAME_IND    (1 << 2)    /* To Host Mailbox Frame Indication */
#define HMB_HOST_INT    (1 << 3)    /* To Host Mailbox Miscellaneous Interrupt */
#define HMB_MASK    0x0000000f    /* To Host Mailbox Mask */

/* Descriptor control flags 1 */
#define D64_CTRL_COREFLAGS    0x0ff00000    /* core specific flags */
#define    D64_CTRL1_NOTPCIE    ((uint32)1 << 18)    /* buirst size control */
#define    D64_CTRL1_EOT        ((uint32)1 << 28)    /* end of descriptor table */
#define    D64_CTRL1_IOC        ((uint32)1 << 29)    /* interrupt on completion */
#define    D64_CTRL1_EOF        ((uint32)1 << 30)    /* end of frame */
#define    D64_CTRL1_SOF        ((uint32)1 << 31)    /* start of frame */

/* Descriptor control flags 2 */
#define D64_CTRL2_BC_USABLE_MASK    0x00003fff  /* buffer byte count. real data len must <= 16KB */
#define    D64_CTRL2_BC_MASK    0x00007fff    /* buffer byte count. real data len must <= 16KB */
#define    D64_CTRL2_AE        0x00030000    /* address extension bits */
#define    D64_CTRL2_AE_SHIFT    16
#define D64_CTRL2_PARITY    0x00040000      /* parity bit */

/** \endcond */

/** \endcond */

/** \} group_sdio_dev_macros_general_purpose */

/**
* \addtogroup group_sdio_dev_enums
* \{
*/

/******************************************************************************
 * Enumerations
 *****************************************************************************/
 
/** SDIO Device error codes. */
typedef enum 
{
    CY_SDIO_DEV_SUCCESS                     = 0U,  /**< Successful. */
    CY_SDIO_DEV_ERROR                       = CY_SDIO_DEV_ID | CY_PDL_STATUS_ERROR | 1U,  /**< Non-specific error code. */
    CY_SDIO_DEV_ERROR_INVALID_PARAMETER     = CY_SDIO_DEV_ID | CY_PDL_STATUS_ERROR | 2U,  /**< The provided parameter is not valid. */
    CY_SDIO_DEV_ERROR_OPERATION_IN_PROGRESS = CY_SDIO_DEV_ID | CY_PDL_STATUS_ERROR | 3U,  /**< A conflicting or requested operation is still in progress. */
    CY_SDIO_DEV_ERROR_UNINITIALIZED         = CY_SDIO_DEV_ID | CY_PDL_STATUS_ERROR | 4U,  /**< The module (or part of it) was not initialized properly. */
    CY_SDIO_DEV_ERROR_TIMEOUT               = CY_SDIO_DEV_ID | CY_PDL_STATUS_ERROR | 5U,  /**< A Time Out error occurred */
    CY_SDIO_DEV_OPERATION_INPROGRESS        = CY_SDIO_DEV_ID | CY_PDL_STATUS_ERROR | 6U,  /**< The indicator for operation in progress. */
    CY_SDIO_DEV_ERROR_UNUSABLE_CARD         = CY_SDIO_DEV_ID | CY_PDL_STATUS_ERROR | 7U,  /**< The card is unusable. */
    CY_SDIO_DEV_ERROR_DISCONNECTED          = CY_SDIO_DEV_ID | CY_PDL_STATUS_ERROR | 8U   /**< The card is disconnected. */
} cy_en_sdio_dev_status_t;

/** DMA transmit state. */
typedef enum 
{
    CY_SDIO_DEV_XS_DISABLED,    /**< disabled */
    CY_SDIO_DEV_XS_ACTIVE,        /**< active */
    CY_SDIO_DEV_XS_IDLE,        /**< idle wait */
    CY_SDIO_DEV_XS_STOPPED,        /**< stopped */
    CY_SDIO_DEV_XS_SUSP            /**< suspend pending */
} cy_en_sdio_dev_transmit_state_t;

/** DMA receive state. */
typedef enum 
{
    CY_SDIO_DEV_RS_DISABLED,    /**< disabled */
    CY_SDIO_DEV_RS_ACTIVE,        /**< active */
    CY_SDIO_DEV_RS_IDLE,        /**< idle wait */
    CY_SDIO_DEV_RS_STOPPED,        /**< stopped */
    CY_SDIO_DEV_RS_SUSP            /**< suspend pending */
} cy_en_sdio_dev_receive_state_t;

/** SDIO Device Manufacturer ID configuration. */
typedef enum 
{
    CY_SDIO_DEV_USE_DEFAULT_MANF_ID                     = 0U,  /**< Use default manufacturer ID from CIS area. */
    CY_SDIO_DEV_USER_PROVIDED_MANF_ID                   = 1U,  /**< Use user provided manufacture ID from config structure. */
} cy_en_sdio_dev_manf_id_config_t;

/** SDIO Device VERSION */
typedef enum 
{
    CY_SDIO_DEV_VERSION_3_0                     = 0U,  /**< Use SDIO Version 3.0. */
    CY_SDIO_DEV_VERSION_2_0                     = 1U,  /**< Use SDIO Version 2.0. */
} cy_en_sdio_dev_ver_config_t;

/** SDIO Device Block Size */
typedef enum 
{
    CY_SDIO_DEV_BLK_SIZE_512                     = 0U,  /**< Use F2 Block Size as 512 bytes. */
    CY_SDIO_DEV_BLK_SIZE_256                     = 1U,  /**< Use F2 Block Size as 256 bytes. */
    CY_SDIO_DEV_BLK_SIZE_128                     = 2U,  /**< Use F2 Block Size as 128 bytes. */
    CY_SDIO_DEV_BLK_SIZE_64                      = 3U,  /**< Use F2 Block Size as 64 bytes. */
} cy_en_sdio_dev_blk_size_config_t;

/** SDIO Device Max Speed */
typedef enum 
{
    CY_SDIO_DEV_SDR12                     = 0U,  /**< SDR12 - Max Clock Frequency 25 Mhz. */
    CY_SDIO_DEV_SDR25                     = 1U,  /**< SDR25 - Max Clock Frequency 50 Mhz. */
    CY_SDIO_DEV_SDR50                     = 2U,  /**< SDR50 - Max Clock Frequency 100 Mhz. */
    CY_SDIO_DEV_SDR104                    = 3U,  /**< SDR104 - Max Clock Frequency 208 Mhz. */
    CY_SDIO_DEV_DDR50                     = 4U,  /**< DDR50 - Max Clock Frequency 50 Mhz. */
} cy_en_sdio_dev_max_speed_config_t;

/** \} group_sdio_dev_enums */

/**
* \addtogroup group_sdio_dev_data_structures
* \{
*/

/**
* Provides the typedef for the callback function called in the
* \ref Cy_SDIO_DEV_Interrupt to notify the user about occurrences of
* \ref group_sdio_dev_macros_callback_events.
*/
typedef void (* cy_cb_sdio_dev_handle_events_t)(uint32_t event);

/** \} group_sdio_dev_data_structures */

/******************************************************************************
 * Structures
 *****************************************************************************/

/** SDIO Device configuration structure. */
typedef struct
{
    cy_en_sdio_dev_ver_config_t                version;        /**< SDIO protocol version supported. */
    cy_en_sdio_dev_manf_id_config_t            use_default_manfid;    /**< SDIO Device manufacturer ID configuration. If this field is set to
                                             CY_SDIO_DEV_USER_PROVIDED_MANF_ID, SDIO Device uses user provided
                                             manufaturer ID using manfid field. Otherwise, default manufacturer ID. */
    uint32_t                        manfid[CY_SDIO_DEV_MANFID_SIZE];    /**< 6 bytes or 3 tuples of user provided manufacturer ID. This will be used
                                             only when use_default_manfid is set to CY_SDIO_DEV_USER_PROVIDED_MANF_ID */
    cy_en_sdio_dev_blk_size_config_t                    blk_size;                 /**< This field specifies the block size user for function 2 data transferes.
                                             Minimum value is CY_SDIO_DEV_BLK_SIZE_64 and Maximum value CY_SDIO_DEV_BLK_SIZE_512. */
    cy_en_sdio_dev_max_speed_config_t            max_speed_config;    /**< This field specifies maximum speed configuration allowed for this SDIO device.
                                             Minimum value is CY_SDIO_DEV_SDR12 and Maximum value CY_SDIO_DEV_DDR50. */
} cy_stc_sdio_dev_config_t;

/** \cond INTERNAL */

/** Context structure. */
typedef struct
{   
    cy_en_sdio_dev_max_speed_config_t            current_speed_config;    /**< Max speed configuration set */
    cy_en_sdio_dev_blk_size_config_t            current_blk_size;       /**< Minimum value is 1 and Maximum value for F2 supported is 512. */
    cy_cb_sdio_dev_handle_events_t                 cbEvents;
    uint8_t                         *rx_buffer;
    uint32_t                        max_rx_buf_len;
    uint8_t                            *tx_buffer;
    uint32_t                        tx_buf_len;
} cy_stc_sdio_dev_context_t;

/**
 * DMA Descriptor
 * Descriptors are only read by the hardware, never written back.
 */
typedef volatile struct {
    uint32_t    ctrl1;        /**< misc control bits */
    uint32_t    ctrl2;        /**< buffer count and address extension */
    uint32_t    addrlow;    /**< memory address of the date buffer, bits 31:0 */
    uint32_t    addrhigh;    /**< memory address of the date buffer, bits 63:32 */
} cy_stc_sdio_dev_dma_descr_t;

/* Receive Frame Status Header */
typedef volatile struct {
    uint16 len;
    uint16 flags;
} cy_stc_sdio_dev_rx_header_t;

/** \endcond */


/**
* \addtogroup group_sdio_dev_functions
* \{
*/

/*******************************************************************************
* Function Name: Cy_SDIO_DEV_Init
****************************************************************************//**
*
* This API will initialise MXSDIO IP with the configuration parameters
*
* \param base
* The pointer to a MXSDIODEV instance.
*
* \param *config
* The pointer to the MXSDIO device configuration structure.
*
* \param context
* The pointer to the context structure \ref cy_stc_sdio_dev_context_t allocated
* by the user. The structure is used during the SDIO device operation for internal
* configuration and data retention. The user must not modify anything
* in this structure.
* If using only the SDIO device functions which do not require context, pass NULL
* as the pointer to the context.
*
* \return \ref cy_en_sdio_dev_status_t
*
*******************************************************************************/
cy_en_sdio_dev_status_t Cy_SDIO_DEV_Init(MXSDIO_Type *base, cy_stc_sdio_dev_config_t const *config, cy_stc_sdio_dev_context_t *context);

/*******************************************************************************
* Function Name: Cy_SDIO_DEV_DeInit
****************************************************************************//**
*
* Restores the MXSDIO block registers back to default.
*
* \param *base
* The pointer to a MXSDIODEV instance.
*
*******************************************************************************/
void Cy_SDIO_DEV_DeInit(MXSDIO_Type *base);

/*******************************************************************************
* Function Name: Cy_SDIO_DEV_Enable
****************************************************************************//**
*
* This API will enable MXSDIODEV IP and allow TX and RX operations
*
* \param base
* The pointer to a MXSDIODEV instance.
*
* \funcusage
*
*******************************************************************************/
cy_en_sdio_dev_status_t Cy_SDIO_DEV_Enable(MXSDIO_Type *base);

/*******************************************************************************
* Function Name: Cy_SDIO_DEV_Disable
****************************************************************************//**
*
* Disables the MXSDIODEV IP from its operation.
* 
* \param base
* The pointer to a MXSDIODEV instance.
*
* \funcusage
*
*******************************************************************************/
cy_en_sdio_dev_status_t Cy_SDIO_DEV_Disable(MXSDIO_Type *base);

/*******************************************************************************
* Function Name: Cy_SD_Host_GetSdStatus
****************************************************************************//**
*
*  Returns the SDIO status from the MXSDIO block.
*
* \param *base
* The pointer to a MXSDIODEV instance.
*
* \param *sdStatus
*  The pointer to store the status of MXSDIO Function 2
*
* \return \ref cy_en_sd_host_status_t
*
*******************************************************************************/
cy_en_sdio_dev_status_t Cy_SDIO_DEV_GetStatus(MXSDIO_Type const *base, bool *sdStatus);

/*******************************************************************************
* Function Name: Cy_SDIO_DEV_ConfigReadBuf
****************************************************************************//**
*
*  Returns the SDIO status from the MXSDIO block.
*
* \param *base
* The pointer to a MXSDIODEV instance.
*
* \param *buffer
* *buffer
*
* \param *context
* *context
*
* \param size
* size
*
* \return \ref cy_en_sd_host_status_t
*
*******************************************************************************/
cy_en_sdio_dev_status_t Cy_SDIO_DEV_ConfigReadBuf(MXSDIO_Type const *base, uint8_t *buffer, uint32_t size, cy_stc_sdio_dev_context_t *context);

/*******************************************************************************
* Function Name: Cy_SDIO_DEV_Interrupt
****************************************************************************//**
*
* The Interrupt Service Routine for the MXSDIO. The interrupt code will be
* responsible for the DMA operations on DMA interrupts during ongoing transfers.
* The user must place a call to this interrupt function in the interrupt
* routine corresponding to the interrupt attached to the MXSDIO. If the
* user does not do this, will break: the functionality of all the API functions in
* the MXSDIO driver that use MXSDIO interrupts to affect transfers.
*
* \param base
* The pointer to a MXSDIODEV instance.
*
* \param context
* Passes a configuration structure that contains the transfer parameters of the
* MXSDIODEV block.
*
*******************************************************************************/
void Cy_SDIO_DEV_Interrupt(MXSDIO_Type *base, cy_stc_sdio_dev_context_t *context);

/*******************************************************************************
* Function Name: Cy_SDIO_DEV_RegisterEventCallback
****************************************************************************//**
*
* Registers a callback function that notifies that
* \ref group_sdio_dev_macros_callback_events occurred in the
* \ref Cy_SDIO_DEV_Interrupt.
*
* \param base
* The pointer to the MXSDIO instance.
*
* \param callback
* The pointer to a callback function.
* See \ref cy_cb_sdio_dev_handle_events_t for the function prototype.
*
* \param context
* The pointer to context structure \ref cy_stc_sdio_dev_context_t allocated by
* the user. The structure is used while the SDIO Device operation for internal
* configuration and data retention. The user should not modify anything in
* this structure.
*
* \note
* To remove the callback, pass NULL as the pointer to the callback function.
*
*******************************************************************************/
void Cy_SDIO_DEV_RegisterEventCallback(MXSDIO_Type const *base, cy_cb_sdio_dev_handle_events_t callback, cy_stc_sdio_dev_context_t *context);

/*******************************************************************************
* Function Name: Cy_SDIO_DEV_Transmit_Buffer
****************************************************************************//**
*
* This function is used to transmit data over SDIO interface. This
* function uses the internal DMA to implement the transmit functionality. The
* function sets up an interrupt to trigger the Transmit completion event and at the
* end of the transmission, the callback \ref cy_cb_sdio_dev_handle_events_t is executed.
*
* \param base
* Holds the base address of the SMIF block registers.
*
* \param buffer
* The pointer to the data to be transferred. First 4 bytes of the buffer
* should be reserved for frame header and actual buffer to be transferredIf
* should start from fifth byte. If this pointer is a NULL, then the
* function returns CY_SDIO_DEV_ERROR.
* 
* \param size
* The size of buffer. Must be > 0 and not greater than 65527.
*
* \param context
* Passes a configuration structure that contains the transfer parameters of the
* MXSDIO_Type block.
*
* \return A status of a transmission.
*       - \ref CY_SDIO_DEV_SUCCESS
*       - \ref CY_SDIO_DEV_ERROR
*
*******************************************************************************/
cy_en_sdio_dev_status_t  Cy_SDIO_DEV_Transmit_Buffer(MXSDIO_Type const *base, uint8_t *buffer, uint16_t size, cy_stc_sdio_dev_context_t *context);

/*******************************************************************************
* Function Name: Cy_SDIO_DEV_Transmit_Buffer
****************************************************************************//**
*
* This function is used to transmit data over SDIO interface. This
* function uses the internal DMA to implement the transmit functionality. The
* function blocks until the buffer gets transmitted and return to caller.
*
* \param base
* Holds the base address of the SMIF block registers.
*
* \param buffer
* The pointer to the data to be transferred. First 4 bytes of the buffer
* should be reserved for frame header and actual buffer to be transferredIf
* should start from fifth byte. If this pointer is a NULL, then the
* function returns CY_SDIO_DEV_ERROR.
* 
* \param buf_len buf_len
*
* \return A status of a transmission.
*       - \ref CY_SDIO_DEV_SUCCESS
*       - \ref CY_SDIO_DEV_ERROR
*
*******************************************************************************/
cy_en_sdio_dev_status_t  Cy_SDIO_DEV_Transmit_Buffer_Blocking(MXSDIO_Type const *base, uint8_t *buffer, uint16_t buf_len);

/*******************************************************************************
* Function Name: Cy_SDIO_DEV_Receive_Buffer
****************************************************************************//**
*
* This function is used to receive data over SDIO interface. This
* function uses the internal DMA to implement the receive functionality. The
* function sets up an interrupt to trigger the receive completion event and at the
* end of the reception, the callback \ref cy_cb_sdio_dev_handle_events_t is executed.
*
* \param base
* Holds the base address of the SMIF block registers.
*
* \param context
* Passes a configuration structure that contains the transfer parameters of the
* MXSDIO_Type block.
*
* \return A status of a transmission.
*       - \ref CY_SDIO_DEV_SUCCESS
*       - \ref CY_SDIO_DEV_ERROR
*
*******************************************************************************/
cy_en_sdio_dev_status_t  Cy_SDIO_DEV_Receive_Buffer(MXSDIO_Type const *base, cy_stc_sdio_dev_context_t *context);

/*******************************************************************************
* Function Name: Cy_SDIO_DEV_Receive_Buffer_Blocking
****************************************************************************//**
*
* This function is used to receive data over SDIO interface. This
* function uses the internal DMA to implement the receive functionality. The
* function blocks until the buffer received and return to caller.
*
* \param base
* Holds the base address of the SMIF block registers.
*
* \param buffer
* The pointer to the data received. If this pointer is a NULL, then the
* function returns CY_SDIO_DEV_ERROR.
* 
* \param buf_len
* buf_len
*
* \return A status of a transmission.
*       - \ref CY_SDIO_DEV_SUCCESS
*       - \ref CY_SDIO_DEV_ERROR
*
*******************************************************************************/
cy_en_sdio_dev_status_t Cy_SDIO_DEV_Receive_Buffer_Blocking(MXSDIO_Type const *base, char **buffer, uint32_t *buf_len);

/** \} group_sdio_dev_functions */

/**
* \addtogroup group_sdio_dev_interrupt_functions
* \{
*/
/*******************************************************************************
* Function Name: Cy_SDIO_DEV_GetInterruptStatus
****************************************************************************//**
*
* This function gets the interrupt status information from interrupt status register
*
* \param base
* Holds the base address of the MXSDIO block registers.
*
* \return Returns the interrupt status. 
*
*******************************************************************************/
uint32_t Cy_SDIO_DEV_GetInterruptStatus(MXSDIO_Type const *base);

/*******************************************************************************
* Function Name: Cy_SDIO_DEV_ClearInterrupt
****************************************************************************//**
*
*  Clears the selected MXSDIO interrupt status.
*
* \param *base
* Holds the base address of the MXSDIO block registers.
*
* \param status
*     The bitmask of statuses to clear.
*
*******************************************************************************/
void Cy_SDIO_DEV_ClearInterruptStatus(MXSDIO_Type const *base, uint32_t status);

/*******************************************************************************
* Function Name: Cy_SDIO_DEV_GetInterruptMask
****************************************************************************//**
*
*  Returns which interrupts are masked to cause an interrupt.
*
* \param *base
* Holds the base address of the MXSDIO block registers.
*
* \return uint32_t
* The Bit field of which interrupts are masked to cause an interrupt.
*
*******************************************************************************/
uint32_t Cy_SDIO_DEV_GetInterruptMask(MXSDIO_Type const *base);

/*******************************************************************************
* Function Name: Cy_SDIO_DEV_SetInterruptMask
****************************************************************************//**
*
*  Setting a bit in this register allows the enabled status to cause an interrupt.
*
* \param *base
* Holds the base address of the MXSDIO block registers.
*
* \param interrupt
*  The Bit field of which interrupts can cause an interrupt.
*
*******************************************************************************/
void Cy_SDIO_DEV_SetInterruptMask(MXSDIO_Type const *base, uint32_t interrupt);

/** \} group_sdio_dev_interrupt_functions */

#if defined(__cplusplus)
}
#endif

#endif /* CY_MXSDIODEV_H */

#endif /* CY_IP_MXSDIODEV */
/** \} group_sdio_dev */

/* [] END OF FILE */
