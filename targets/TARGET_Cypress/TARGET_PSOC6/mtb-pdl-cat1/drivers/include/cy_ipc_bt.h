/***************************************************************************//**
* \file cy_ipc_bt.h
* \version 1.0
*
* \brief
* Provides an API declaration for the BT IPC driver.
*
********************************************************************************
* \copyright
* Copyright 2016-2020 Cypress Semiconductor Corporation
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

#ifndef CY_IPC_BT_H
#define CY_IPC_BT_H

/******************************************************************************/
/* Include files                                                              */
/******************************************************************************/

#include "cy_device.h"

#if defined (CY_IP_MXIPC)

#include "cy_ipc_drv.h"
#include "cy_syslib.h"
#include "cy_sysint.h"

/**
* \addtogroup group_ipc_bt IPC bluetooth sub-system layer (IPC_BTSS)
* \{
* The BT IPC  functions provide a method to transfer Host Controller Interface (HCI)
* and Hight Priority Controller (HPC) messages between the MCU and BT SS.
*
* Include cy_ipc_bt.h. Alternatively include cy_pdl.h
* to get access to all functions and declarations in the PDL.
*
* The data needs to be provided in an array along with the payload type indicator.
* Depending on the payload length, the message gets formatted in to short or long
* message. If the payload length is less than 7 bytes, then it is considered as a short
* message. Short messages are packed and sent using IPC channel DATA0 and DATA1
* registers. Any payload length greater than 7 bytes will be sent using the buffers in
* the shared memory.
*
* It supports callback function for notification on receiving the HCI messages from the BT SS
* HCI notification callback function is registered during the initialization of the driver.
* It also supports multiple callback registration for notification  on receiving the
* HPC messages.
*
*     \defgroup group_ipc_bt_macros Macros
*       Macro definitions are used in the driver
*
*     \defgroup group_ipc_bt_functions Functions
*       Functions are used in the driver
*
*     \defgroup group_ipc_bt_data_structures Data Structures
*       Data structures are used in the driver
*
*     \defgroup group_ipc_bt_enums Enumerated Types
*       Enumerations are used in the driver
* \}
*
*/

/**
* \addtogroup group_ipc_bt_macros
* \{
*/

/** Software PDL driver ID for IPC BT  functions */
#define CY_BT_IPC_ID CY_PDL_DRV_ID(0x22u)

/** Return prefix for BT IPC driver function status codes */
#define CY_BT_IPC_ID_INFO    (uint32_t)( CY_BT_IPC_ID | CY_PDL_STATUS_INFO )

/** Return prefix for BT IPC driver function warning return values */
#define CY_BT_IPC_ID_WARNING (uint32_t)( CY_BT_IPC_ID | CY_PDL_STATUS_WARNING)

/** Return prefix for BT IPC driver function error return values */
#define CY_BT_IPC_ID_ERROR   (uint32_t)( CY_BT_IPC_ID | CY_PDL_STATUS_ERROR)

/** Maximum HPC callback supported */
#define MAX_BT_IPC_HPC_CB 5

/** Maximum number of buffers in the pool */
#define MAX_BUF_COUNT 10

/** \} group_ipc_bt_macros */


/** Maximum length of short message */
#define MAX_SHORT_MESG_LENGTH 7

/** Buffer descriptor length in init message */
#define BUFFER_DESCRIPTION_LEN 7

/** size of cy_stc_ipc_msg_subheader_t structure */
#define SUB_HEADER_SIZE 4

/**
* \addtogroup group_ipc_bt_enums
* \{
*/
/**
* This is a list of ENUMs used for function return status.
*/
typedef enum
{
    CY_BT_IPC_DRV_SUCCESS            =(uint32_t)(0x00u),                    /**<  BT IPC API return for no error */
    CY_BT_IPC_DRV_ERROR              =(uint32_t)(CY_BT_IPC_ID_ERROR | 1ul), /**<  BT IPC API return for general error */
    CY_BT_IPC_DRV_ERROR_BAD_HANDLE   =(uint32_t)(CY_BT_IPC_ID_ERROR | 2ul), /**<  BT IPC API return for bad handle */
    CY_BT_IPC_DRV_ERROR_CH_BUSY      =(uint32_t)(CY_BT_IPC_ID_ERROR | 3ul), /**<  BT IPC API return for channel busy */
    CY_BT_IPC_DRV_ERROR_LOCK_REL     =(uint32_t)(CY_BT_IPC_ID_ERROR | 4ul), /**<  BT IPCAPI return for channel release error */
    CY_BT_IPC_DRV_ERROR_LOCK_ACQUIRE =(uint32_t)(CY_BT_IPC_ID_ERROR | 5ul), /**<  BT IPC API return for channel release error */
    CY_BT_IPC_DRV_ERROR_BUF_GET =(uint32_t)(CY_BT_IPC_ID_ERROR | 6ul), /**<  BT IPC API return for get buffer error */
    CY_BT_IPC_DRV_ERROR_BUF_FULL =(uint32_t)(CY_BT_IPC_ID_ERROR | 7ul), /**<  BT IPC API return for buffer pool full error */
    CY_BT_IPC_DRV_ERROR_PARAM =(uint32_t)(CY_BT_IPC_ID_ERROR | 8ul), /**<  BT IPC API return for bad parameter error */
} cy_en_btipcdrv_status_t;

/**
* This is a list of ENUMs used for buffer types.
*/
typedef enum
{
    CY_BT_IPC_CTRL_BUF        = (0x00u),                    /**<  Control buffer type */
    CY_BT_IPC_HCI_BREDR_BUF   = (0x01u),                    /**<  BREDR buffer type */
    CY_BT_IPC_HCI_BLE_BUF     = (0x02u),                    /**<  BLE buffer type */
    CY_BT_IPC_HCI_ISOC_BUF    = (0x03u),                    /**<  ISOC buffer type */
    CY_BT_IPC_HCI_INVALID_BUF   = (0xFFu),                  /**<  Invalid buffer type */
} cy_en_btipc_buftype_t;

/**
* This is a list of ENUMs used for HCI packet type.
*/
typedef enum
{
    CY_BT_IPC_HCI_IGNORE  = (0x00u),                        /**<  HCI payload type ignore */
    CY_BT_IPC_HCI_CMD     = (0x01u),                        /**<  HCI payload type command */
    CY_BT_IPC_HCI_ACL     = (0x02u),                        /**<  HCI payload type ACL */
    CY_BT_IPC_HCI_SCO     = (0x03u),                        /**<  HCI payload type SCO */
    CY_BT_IPC_HCI_EVT     = (0x04u),                        /**<  HCI payload type Event */
    CY_BT_IPC_HCI_ISO     = (0x05u),                        /**<  HCI payload type ISO */
    CY_BT_IPC_HCI_DIAG    = (0x07u),                        /**<  HCI payload type Diagnostics */
    CY_BT_IPC_HCI_MPAF    = (0x0Au),                        /**<  HCI payload type MPAF */
    CY_BT_IPC_HCI_SLIPH5  = (0xC0u),                        /**<  HCI payload type SLIPH5 */
    CY_BT_IPC_HCI_LONG    = (0xFFu),                        /**<  HCI payload type long message */
} cy_en_btipc_hcipti_t;

/**
* This is a list of ENUMs used for HPC packet type.
*/
typedef enum
{
    CY_BT_IPC_HPC_BUF_FREE       = (0x00u),                 /**<  HPC payload type free buffer */
    CY_BT_IPC_HPC_BUF_CHSIZE     = (0x01u),                 /**<  HPC payload type change buffer size */
    CY_BT_IPC_HPC_BUF_ALLOC      = (0x02u),                 /**<  HPC payload type allocate buffer */
    CY_BT_IPC_HPC_BUF_REQTRNG    = (0x03u),                 /**<  HPC payload type request TRNG */
    CY_BT_IPC_HPC_BUF_AVAIL      = (0x04u),                 /**<  HPC payload type buffer available */
    CY_BT_IPC_HPC_LONG           = (0xFFu),                 /**<  HPC payload type long message */
} cy_en_btipc_hpcpti_t;

/**
* This is a list of ENUMs used for bt boot  type.
*/
typedef enum
{
    CY_BT_IPC_COLD_BOOT       = (0x00u),                    /**<  BT cold boot */
    CY_BT_IPC_SW_RST          = (0x01u),                    /**<  BT software reset boot */
    CY_BT_IPC_ERR_RST         = (0x02u),                    /**<  BT error reset */
} cy_en_btipc_boottype_t;

/** \} group_ipc_bt_enums */

/*
 * This section defines the system level constants required to define
 * callback arrays for the Cypress pipe and the user pipe.  These defines
 * are used for both the max callback count and maximum clients.
*/

/** Typedef for bt ipc  callback function pointer */
typedef void (* cy_ipc_bt_callback_ptr_t)(uint32_t * msgPtr);

/** Typedef for bt ipc channel release callback function pointer */
typedef void (* cy_ipc_bt_relcallback_ptr_t)(void);

/** Typedef for bt ipc irq function pointer */
typedef void (* cy_ipc_bt_irq_handler_t)(void);

/**
* \addtogroup group_ipc_bt_data_structures
* \{
*/

/**
* This is the definition of an allocation msg strcuture.
*/
typedef struct cy_stc_ipc_msg_alloc_t
{
    uint8_t pti;                                    /**< Payload type indicator */
    uint8_t bufType;                                /**< Buffer type indicator */
    uint16_t bufSize;                               /**< Buffer size */
    uint8_t *bufAddr;                               /**< Pointer to the buffer */
} cy_stc_ipc_msg_alloc_t;

/**
* This is the definition of a long message strcuture.
*/
typedef struct cy_stc_ipc_msg_buff_t
{
    uint8_t pti;                                    /**< Payload type indicator */
    uint8_t actualPti;                              /**< Actual PTI for the long messages */
    uint16_t bufSize;                               /**< Buffer size */
    uint8_t *bufAddr;                               /**< Pointer to the buffer */
} cy_stc_ipc_msg_buff_t;

/**
* This is the definition of a short msg strcuture.
*/
typedef struct cy_stc_ipc_msg_short_t
{
    uint8_t pti;                                    /**< Payload type indicator */
    uint8_t db0;                                    /**< Payload byte 0 for short message */
    uint8_t db1;                                    /**< Payload byte 1 for short message */
    uint8_t db2;                                    /**< Payload byte 2 for short message */
    uint8_t db3;                                    /**< Payload byte 3 for short message */
    uint8_t db4;                                    /**< Payload byte 4 for short message */
    uint8_t db5;                                    /**< Payload byte 5 for short message */
    uint8_t db6;                                    /**< Payload byte 6 for short message */
} cy_stc_ipc_msg_short_t;

/** \} group_ipc_bt_data_structures */

/**
* This is the definition of an init msg strcuture.
*/
typedef __PACKED_STRUCT cy_stc_ipc_msg_init_t
{
    uint8_t msgId;                                  /**< Message ID for HPC message */
    uint8_t bootType;                               /**< BT boot type indicator */
    uint8_t payLoadLen;                             /**< Payload length in bytes */
} cy_stc_ipc_msg_init_t;

/**
* This is the definition of an sub header msg strcuture.
*/
typedef __PACKED_STRUCT cy_stc_ipc_msg_subheader_t
{
    uint8_t msgId;                                  /**< Message ID for HPC message */
    uint8_t dummy;                                  /**< Dummy byte to be igonored */
    uint16_t payLoadLen;                            /**< Payload length in bytes */
} cy_stc_ipc_msg_subheader_t;

/**
* This is the definition of buffer descriptor strcuture.
*/
typedef __PACKED_STRUCT cy_stc_ipc_bt_buf_t
{
    cy_en_btipc_buftype_t bufType;                  /**< Buffer type indicator */
    uint16_t bufLen;                                /**< Buffer length */
    uint8_t *bufPtr;                                /**< Pointer to the buffer */
} cy_stc_ipc_bt_buf_t;

/* Following are the the HCI packet header typedefs */
/**
* This is the definition of CMD header strcuture.
*/
typedef __PACKED_STRUCT BTHCI_CMD_HDR_t
{
    uint16_t opcode;
    uint8_t params_len;
} BTHCI_CMD_HDR_t;

/**
* This is the definition of event header strcuture.
*/
typedef __PACKED_STRUCT BTHCI_EVENT_HDR_t
{
    uint8_t   code;
    uint8_t   params_len;
} BTHCI_EVENT_HDR_t;

/**
* This is the definition of ACL header strcuture.
*/
typedef __PACKED_STRUCT BTHCI_ACL_HDR_t
{
    uint16_t  conn_handle_and_flags;
    uint16_t  data_len;
} BTHCI_ACL_HDR_t;

/**
* This is the definition of ISO header strcuture.
*/
typedef __PACKED_STRUCT BTHCI_ISO_HDR_t
{
    uint16_t  conn_handle_and_flags;
    uint16_t  data_len;
} BTHCI_ISO_HDR_t;

/**
* This is the definition of SCO header strcuture.
*/
typedef __PACKED_STRUCT BTHCI_SCO_HDR_t
{
    __PACKED_UNION
    {
        uint16_t  audioChannel;
        uint16_t  connectionHandle;
    }   u;
    uint8_t  data_len;
} BTHCI_SCO_HDR_t;

/** Typedef for internal ipc  callback function pointer */
typedef void (* cy_ipc_bt_int_cb_ptr_t)(void *btIpcContext, uint32_t * msgPtr);


/**
* \addtogroup group_ipc_bt_data_structures
* \{
*/

/**
* This is the definition of BT IPC context strcuture.
*/
typedef struct cy_stc_ipc_bt_context_t
{
    uint32_t ulChannelHCI;                                    /**< HCI Channel used to send to BLE */
    uint32_t dlChannelHCI;                                    /**< HCI Channel used to receive from BLE */

    uint32_t ulChannelHPC;                                    /**< HPC Channel used to send to BLE */
    uint32_t dlChannelHPC;                                    /**< HPC Channel used to receive from BLE */

    uint32_t intStuctureSelf;                                 /**< Interrupt struture for the MCU */
    uint32_t intStucturePeer;                                 /**< Interrupt struture for the BLE */

    uint32_t intPeerMask;                                     /**< Interrupt mask for the peer */

    uint32_t dlNotifyMask;                                    /**< Down link notify mask */
    uint32_t ulReleaseMask;                                   /**< Up link release mask */

    cy_stc_sysint_t ipcIntConfig;                             /**< IPC Interrupt configuration structure */

    cy_ipc_bt_irq_handler_t irqHandlerPtr;                    /**< This handler will be removed later once similations are done */

    cy_ipc_bt_int_cb_ptr_t internal_hpc_notify_cb;            /**< This callback will be removed later once similations are done */

    cy_ipc_bt_callback_ptr_t dlNotifyCallbackPtr;             /**< Callback function called when the DL mesg is recevieved */

    cy_ipc_bt_relcallback_ptr_t ulReleaseCallbackPtr;         /**< Callback function called when the UL channel is released */

    cy_ipc_bt_callback_ptr_t hpcNotifyCallbackPtr[MAX_BT_IPC_HPC_CB];  /**< Array of callback pointers registered for control channel notification */

    cy_stc_ipc_bt_buf_t buffPool[MAX_BUF_COUNT];              /**< Storage for pool of buffers sent by BLE for communication */
} cy_stc_ipc_bt_context_t;

/**
* This is the definition of BT IPC configuration strcuture.
*/
typedef struct cy_stc_ipc_bt_config_t
{
    uint32_t ulChannelHCI;                                      /**< HCI Channel used to send to BLE */
    uint32_t dlChannelHCI;                                      /**< HCI Channel used to receive from BLE */

    uint32_t ulChannelHPC;                                      /**< HPC Channel used to send to BLE */
    uint32_t dlChannelHPC;                                      /**< HPC Channel used to receive from BLE */

    uint32_t intStuctureSelf;                                   /**< Interrupt struture for the MCU */
    uint32_t intStucturePeer;                                   /**< Interrupt struture for the BLE */

    cy_stc_sysint_t ipcIntConfig;                               /**< IPC Interrupt configuration structure */

    cy_ipc_bt_irq_handler_t irqHandlerPtr;                      /**< This handler will be removed later once similations are done */

    cy_ipc_bt_int_cb_ptr_t internal_hpc_notify_cb;              /**< This callback will be removed later once similations are done */

    cy_ipc_bt_callback_ptr_t dlNotifyCallbackPtr;               /**< Callback function called when the DL mesg is recevieved */
    cy_ipc_bt_relcallback_ptr_t ulReleaseCallbackPtr;           /**< Callback function called when the UL channel is released */
} cy_stc_ipc_bt_config_t;

/** \} group_ipc_bt_data_structures */

/**
* \addtogroup group_ipc_bt_functions
* \{
*/
/*******************************************************************************
* Function Name: Cy_BTIPC_Init
****************************************************************************//**
*
* This function initializes the bluetooth IPC. The IPC channels used for HCI and HPC packet exchange
* between MCU and BT SS are initialized.
*
* \note In general, this function is during application initializaation.
*
* \param btIpcConfig
* This is the pointer to the bt ipc configuration structure.
*
* \param btIpcContext
* This is the pointer to the bt ipc context structure.
*
* \return
*    CY_BT_IPC_DRV_SUCCESS:          BT IPC API return for no error
*    CY_BT_IPC_DRV_ERROR:            BT IPC API return for general error
*    CY_BT_IPC_DRV_ERROR_BAD_HANDLE: BT IPC API return for bad handle
*
*******************************************************************************/
cy_en_btipcdrv_status_t Cy_BTIPC_Init(cy_stc_ipc_bt_context_t *btIpcContext, cy_stc_ipc_bt_config_t * btIpcConfig);

/*******************************************************************************
* Function Name: Cy_BTIPC_Deinit
****************************************************************************//**
*
* This function de-initializes the bluetooth IPC. All the registered callbacks are deleted.
*
* \note In general, this function is called when the application does not need BT IPC service
* anymore.
*
* \param btIpcContext
* This is the pointer to the bt ipc context structure.
*
* \return
*    CY_BT_IPC_DRV_SUCCESS:          BT IPC API return for no error
*    CY_BT_IPC_DRV_ERROR_BAD_HANDLE: BT IPC API return for bad handle
*
*******************************************************************************/
cy_en_btipcdrv_status_t Cy_BTIPC_Deinit(cy_stc_ipc_bt_context_t *btIpcContext);

/*******************************************************************************
* Function Name: Cy_BTIPC_HCI_getPTI
****************************************************************************//**
*
* This function gets the payload type indicator and the legth of the HCI packet received
* from BTSS. This function can be called to find out the type of HCI packet and allocate
* buffer of approriate size to copy the packet data.
*
* \note In general, this function is called before reading the packet to allocate memory
* of required size.
*
* \param pti
* This is the pointer to payload type indicator which is returned by the function.
*
* \param p_length
* This is the pointer to the length of the payload filled by the function.
*
* \param msgPtr
* This is the pointer to buffer which contains the IPC data recevied in DATA0 and DATA1
* registers.
*
* \return
*    CY_BT_IPC_DRV_SUCCESS:          BT IPC API return for no error
*    CY_BT_IPC_DRV_ERROR_BAD_HANDLE: BT IPC API return for bad handle
*
*******************************************************************************/
cy_en_btipcdrv_status_t Cy_BTIPC_HCI_getPTI (cy_en_btipc_hcipti_t *pti, uint32_t *p_length, uint32_t *msgPtr);

/*******************************************************************************
* Function Name: Cy_BTIPC_GetBuffer
****************************************************************************//**
*
* This function gets the free buffer form the pool of shared memory buffers.
* between MCU and BT SS are initialized.
*
* \param bufPtr
* This is the pointer to the pointer whic will be filled by the function with
* the addressbt of the buffer.
*
* \param bufType
* This is the buffer type that is being requested.
*
* \param btIpcContext
* This is the pointer to the bt ipc context structure.
*
* \return
*    CY_BT_IPC_DRV_SUCCESS:          BT IPC API return for no error
*    CY_BT_IPC_DRV_ERROR_BAD_HANDLE: BT IPC API return for bad handle
*    CY_BT_IPC_DRV_ERROR_BUF_GET:    BT IPC API return for get buffer error
*
*******************************************************************************/
cy_en_btipcdrv_status_t Cy_BTIPC_GetBuffer (cy_stc_ipc_bt_context_t *btIpcContext, void **bufPtr, cy_en_btipc_buftype_t bufType);

/*******************************************************************************
* Function Name: Cy_BTIPC_PutBuffer
****************************************************************************//**
*
* This function puts the free buffer back to the buffer pool
*
* \param bufDecriptor
* This is the pointer to the buffer descriptor.
*
* \param btIpcContext
* This is the pointer to the bt ipc context structure.
*
* \return
*    CY_BT_IPC_DRV_SUCCESS:          BT IPC API return for no error
*    CY_BT_IPC_DRV_ERROR_BAD_HANDLE: BT IPC API return for bad handle
*    CY_BT_IPC_DRV_ERROR_BUF_FULL:   BT IPC API return for buffer pool full error
*
*******************************************************************************/
cy_en_btipcdrv_status_t Cy_BTIPC_PutBuffer(cy_stc_ipc_bt_context_t *btIpcContext, cy_stc_ipc_bt_buf_t *bufDecriptor);

/*******************************************************************************
* Function Name: Cy_BTIPC_HCI_Write
****************************************************************************//**
*
* This function sends the HCI packet form the BT stack to the BTSS.
*
* \param pti
* This is the HCI payload type indicator.
*
* \param data
* This is the pointer to the buffer holding the HCI payload.
*
* \param length
* This is the length of the HCI payload in number of bytes.
*
* \param btIpcContext
* This is the pointer to the bt ipc context structure
*
* \return
*    CY_BT_IPC_DRV_SUCCESS:            BT IPC API return for no error
*    CY_BT_IPC_DRV_ERROR_BAD_HANDLE:   BT IPC API return for bad handle
*    CY_BT_IPC_DRV_ERROR_LOCK_ACQUIRE: BT IPC API return for buffer pool full error
*
*******************************************************************************/
cy_en_btipcdrv_status_t Cy_BTIPC_HCI_Write(cy_stc_ipc_bt_context_t *btIpcContext, cy_en_btipc_hcipti_t pti, void *data, size_t length);

/*******************************************************************************
* Function Name: Cy_BTIPC_HPC_Write
****************************************************************************//**
*
* This function sends the HPC packet form the MCU to the BTSS.
*
* \param data
* This is the pointer to the buffer holding the HPC payload.
*
* \param length
* This is the length of the HPC payload in number of bytes.
*
* \param btIpcContext
* This is the pointer to the bt ipc context structure
*
* \return
*    CY_BT_IPC_DRV_SUCCESS:            BT IPC API return for no error
*    CY_BT_IPC_DRV_ERROR_BAD_HANDLE:   BT IPC API return for bad handle
*    CY_BT_IPC_DRV_ERROR_LOCK_ACQUIRE: BT IPC API return for buffer pool full error
*
*******************************************************************************/
cy_en_btipcdrv_status_t Cy_BTIPC_HPC_Write(cy_stc_ipc_bt_context_t *btIpcContext, void *data, size_t length);

/*******************************************************************************
* Function Name: Cy_BTIPC_HCI_Read
****************************************************************************//**
*
* This function reads the HCI packet received by MCU from BTSS.
*
* \param pti
* This is the HCI payload type indicator.
*
* \param data
* This is the pointer to the buffer for receiving the HCI payload.
*
* \param pLength
* This is the pointer to the size of the payload received in number of bytes
*
* \param btIpcContext
* This is the pointer to the bt ipc context structure
*
* \return
*    CY_BT_IPC_DRV_SUCCESS:             BT IPC API return for no error
*    CY_BT_IPC_DRV_ERROR_BAD_HANDLE:    BT IPC API return for bad handle
*    CY_BT_IPC_DRV_ERROR_LOCK_ACQUIRE:  BT IPC API return for buffer pool full error
*    CY_BT_IPC_DRV_ERROR_LOCK_REL:      BT IPCAPI return for channel release error
*
*******************************************************************************/
cy_en_btipcdrv_status_t Cy_BTIPC_HCI_Read (cy_stc_ipc_bt_context_t *btIpcContext, cy_en_btipc_hcipti_t pti, void *data, size_t* pLength);

/*******************************************************************************
* Function Name: Cy_BTIPC_HCI_RelChannel
****************************************************************************//**
*
* This function releases the HCI channel after handling the notification from the
* BT SS
*
* \param buf
* This is the pointer to the buffer received from BT SS in DATA0 and DATA1 channel
* registers.
*
* \param btIpcContext
* This is the pointer to the bt ipc context structure
*
* \return
*    CY_BT_IPC_DRV_SUCCESS:             BT IPC API return for no error
*    CY_BT_IPC_DRV_ERROR_BAD_HANDLE:    BT IPC API return for bad handle
*    CY_BT_IPC_DRV_ERROR_LOCK_REL:      BT IPCAPI return for channel release error
*
*******************************************************************************/
cy_en_btipcdrv_status_t Cy_BTIPC_HCI_RelChannel(cy_stc_ipc_bt_context_t *btIpcContext, void * buf);

/*******************************************************************************
* Function Name: Cy_BTIPC_HPC_RelChannel
****************************************************************************//**
*
* This function releases the HPC channel after handling the notification from the
* BT SS
*
* \param buf
* This is the pointer to the buffer received from BT SS in DATA0 and DATA1 channel
* registers.
*
* \param btIpcContext
* This is the pointer to the bt ipc context structure
*
* \return
*    CY_BT_IPC_DRV_SUCCESS:             BT IPC API return for no error
*    CY_BT_IPC_DRV_ERROR_BAD_HANDLE:    BT IPC API return for bad handle
*    CY_BT_IPC_DRV_ERROR_LOCK_REL:      BT IPCAPI return for channel release error
*
*******************************************************************************/
cy_en_btipcdrv_status_t Cy_BTIPC_HPC_RelChannel(cy_stc_ipc_bt_context_t *btIpcContext, void * buf);

/*******************************************************************************
* Function Name: Cy_BTIPC_HPC_RegisterCb
****************************************************************************//**
*
* This function registers the callback for HPC message nofitications from BT SS
*
* \param hpcNotifyCallbackPtr
* This is the pointer to the function to be called for HPC notifications.
*
* \param btIpcContext
* This is the pointer to the bt ipc context structure
*
* \return
*    CY_BT_IPC_DRV_SUCCESS:             BT IPC API return for no error
*    CY_BT_IPC_DRV_ERROR_BAD_HANDLE:    BT IPC API return for bad handle
*    CY_BT_IPC_DRV_ERROR:               BT IPC API return for general error
*
*******************************************************************************/
cy_en_btipcdrv_status_t Cy_BTIPC_HPC_RegisterCb(cy_stc_ipc_bt_context_t *btIpcContext, cy_ipc_bt_callback_ptr_t hpcNotifyCallbackPtr);

/*******************************************************************************
* Function Name: Cy_BTIPC_HPC_UnregisterCb
****************************************************************************//**
*
* This function un-registers the callback function registered for HPC message
* nofitications from BT SS
*
* \param hpcNotifyCallbackPtr
* This is the pointer to the function to be un-registered.
*
* \param btIpcContext
* This is the pointer to the bt ipc context structure
*
* \return
*    CY_BT_IPC_DRV_SUCCESS:             BT IPC API return for no error
*    CY_BT_IPC_DRV_ERROR_BAD_HANDLE:    BT IPC API return for bad handle
*    CY_BT_IPC_DRV_ERROR:               BT IPC API return for general error
*
*******************************************************************************/
cy_en_btipcdrv_status_t Cy_BTIPC_HPC_UnregisterCb(cy_stc_ipc_bt_context_t *btIpcContext, cy_ipc_bt_callback_ptr_t hpcNotifyCallbackPtr);

/** \} group_ipc_bt_functions */

void Cy_BTIPC_IRQ_Handler(cy_stc_ipc_bt_context_t *btIpcContext);

/* This function prototype will be removed once the simulations are done */
void Cy_BTIPC_HPC_Notify(void *btIpcContext, uint32_t * msgPtr);


/** \} group_ipc_bt */

#ifdef __cplusplus
}
#endif

#endif /* CY_IP_MXIPC */

#endif /* !defined (CY_IPC_BT_H) */

/* [] END OF FILE */
