/***************************************************************************//**
* \file cy_dmac.h
* \version 1.20
*
* \brief
* The header file of the DMAC driver.
*
********************************************************************************
* \copyright
* Copyright 2018-2020 Cypress Semiconductor Corporation
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
* \addtogroup group_dmac
* \{
* Configures the DMA Controller block, channels and descriptors.
*
* The functions and other declarations used in this driver are in cy_dmac.h.
* You can include cy_pdl.h to get access to all functions
* and declarations in the PDL.
*
* The DMA Controller channel can be used in any project to transfer data
* without CPU intervention basing on a hardware trigger signal from another component.
*
* The DMA Controller block has a set of registers, a base hardware address,
* and supports multiple channels. Many API functions for the DMAC driver require
* a base hardware address and channel number.
* Ensure that you use the correct hardware address for the DMA Controller block in use.
*
* Features:
* * Multiple channels (device specific).
* * Four priority levels for each channel.
* * Descriptor chaining.
* * Configurable data transfer width/masking: byte, half-word (2-byte), and word (4-byte).
* * Configurable source and destination addresses.
* * Single transfer, 1D, 2D, memory copy and scatter transfer descriptor types are supported.
* * Configurable input/output triggers and interrupts.
*
* \section group_dmac_configuration Configuration Considerations
*
* To set up a DMAC driver, initialize a descriptor,
* initialize and enable a channel, and enable the DMAC block.
*
* To set up a descriptor, provide the configuration parameters for the
* descriptor in the \ref cy_stc_dmac_descriptor_config_t structure. Then call the
* \ref Cy_DMAC_Descriptor_Init function to initialize the descriptor in SRAM. You can
* modify the source and destination addresses dynamically by calling
* \ref Cy_DMAC_Descriptor_SetSrcAddress and \ref Cy_DMAC_Descriptor_SetDstAddress.
*
* To set up a DMAC channel, provide a filled \ref cy_stc_dmac_channel_config_t
* structure. Call the \ref Cy_DMAC_Channel_Init function, specifying the channel
* number. Use \ref Cy_DMAC_Channel_Enable to enable the configured DMAC channel.
*
* Call \ref Cy_DMAC_Channel_Enable for each DMAC channel in use.
*
* When configured, another peripheral typically triggers the DMAC channel. The trigger is
* connected to the DMAC channel using the trigger multiplexer. The trigger multiplexer
* driver has a software trigger you can use in firmware to trigger the DMAC channel. See the
* <a href="group__group__trigmux.html">Trigger Multiplexer</a> documentation.
*
* The following is a simplified structure of the DMAC driver API interdependencies
* in a typical user application:
* \image html dmac.png
*
* <B>NOTE:</B> Even if a DMAC channel is enabled, it is not operational until
* the DMAC block is enabled using function \ref Cy_DMAC_Enable.\n
* <B>NOTE:</B> If the DMAC descriptor is configured to generate an interrupt,
* the interrupt must be enabled using the \ref Cy_DMAC_Channel_SetInterruptMask
* function for each DMAC channel.
*
* For example:
* \snippet dmac/snippet/main.c snippet_Cy_DMAC_Enable
*
* \section group_dmac_more_information More Information.
* See the DMAC chapter of the device technical reference manual (TRM).
*
* \section group_dmac_changelog Changelog
*
* <table class="doxtable">
*   <tr><th>Version</th><th>Changes</th><th>Reason for Change</th></tr>
*   <tr>
*     <td>1.20</td>
*     <td>Fixed MISRA 2012 violations.</td>
*     <td>MISRA 2012 compliance.</td>
*   </tr>
*   <tr>
*     <td>1.10.1</td>
*     <td>Minor documentation updates.</td>
*     <td>Documentation enhancement.</td>
*   </tr>
*   <tr>
*     <td>1.10</td>
*     <td>The \ref Cy_DMAC_Channel_ClearInterrupt is changed.</td>
*     <td>Minor defect fixing.</td>
*   </tr>
*   <tr>
*     <td>1.0</td>
*     <td>The initial version.</td>
*     <td></td>
*   </tr>
* </table>
*
* \defgroup group_dmac_macros Macros
* \defgroup group_dmac_macros_interrupt_masks Interrupt Masks
* \defgroup group_dmac_functions Functions
* \{
* \defgroup group_dmac_block_functions Block Functions
* \defgroup group_dmac_channel_functions Channel Functions
* \defgroup group_dmac_descriptor_functions Descriptor Functions
* \}
* \defgroup group_dmac_data_structures Data Structures
* \defgroup group_dmac_enums Enumerated Types
*/

#if !defined (CY_DMAC_H)
#define CY_DMAC_H

#include "cy_device.h"

#if defined (CY_IP_M4CPUSS_DMAC) || defined (CY_IP_MXAHBDMAC)

#include "cy_syslib.h"
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

#if defined(__cplusplus)
extern "C" {
#endif

CY_MISRA_DEVIATE_BLOCK_START('MISRA C-2012 Rule 10.8', 13, \
'Value extracted from _VAL2FLD macro will not exceed enum range.');

/******************************************************************************
 * Macro definitions                                                          *
 ******************************************************************************/

/**
* \addtogroup group_dmac_macros
* \{
*/

/** The driver major version */
#define CY_DMAC_DRV_VERSION_MAJOR       1

/** The driver minor version */
#define CY_DMAC_DRV_VERSION_MINOR       20

/** The DMAC driver identifier */
#define CY_DMAC_ID                      (CY_PDL_DRV_ID(0x3FU))

/** The minimum X/Y Count API parameters */
#define CY_DMAC_LOOP_COUNT_MIN          (1UL)
/** The maximum X/Y Count API parameters */
#define CY_DMAC_LOOP_COUNT_MAX          (65536UL)
/** The maximum X Count API parameter for scatter transfer */
#define CY_DMAC_SCATTER_COUNT_MAX       (32768UL)

/** The minimum X/Y Increment API parameters */
#define CY_DMAC_LOOP_INCREMENT_MIN      (-32768L)
/** The maximum X/Y Increment API parameters */
#define CY_DMAC_LOOP_INCREMENT_MAX      (32767L)

/**
* \addtogroup group_dmac_macros_interrupt_masks Interrupt Masks
* \{
*/

/** Bit 0: Completion of data transfer(s) as specified by the descriptor's interruptType setting. */
#define CY_DMAC_INTR_COMPLETION         (DMAC_CH_V2_INTR_COMPLETION_Msk)
/** Bit 1: Bus error for a load from the source. */
#define CY_DMAC_INTR_SRC_BUS_ERROR      (DMAC_CH_V2_INTR_SRC_BUS_ERROR_Msk)
/** Bit 2: Bus error for a store to the destination. */
#define CY_DMAC_INTR_DST_BUS_ERROR      (DMAC_CH_V2_INTR_DST_BUS_ERROR_Msk)
/** Bit 3: Misalignment of the source address. */
#define CY_DMAC_INTR_SRC_MISAL          (DMAC_CH_V2_INTR_SRC_MISAL_Msk)
/** Bit 4: Misalignment of the destination address. */
#define CY_DMAC_INTR_DST_MISAL          (DMAC_CH_V2_INTR_DST_MISAL_Msk)
/** Bit 5: The channel is enabled and the current descriptor pointer is "0". */
#define CY_DMAC_INTR_CURR_PTR_NULL      (DMAC_CH_V2_INTR_CURR_PTR_NULL_Msk)
/** Bit 6: The channel is disabled and the data transfer engine is busy. */
#define CY_DMAC_INTR_ACTIVE_CH_DISABLED (DMAC_CH_V2_INTR_ACTIVE_CH_DISABLED_Msk)
/** Bit 7: Bus error for a load of the descriptor. */
#define CY_DMAC_INTR_DESCR_BUS_ERROR    (DMAC_CH_V2_INTR_DESCR_BUS_ERROR_Msk)

/** \} group_dmac_macros_interrupt_masks */

/** \} group_dmac_macros */


/**
* \addtogroup group_dmac_enums
* \{
*/

/** Contains the options for the descriptor type */
typedef enum
{
    CY_DMAC_SINGLE_TRANSFER  = 0U,         /**< Single transfer.  */
    CY_DMAC_1D_TRANSFER      = 1U,         /**< 1D transfer.      */
    CY_DMAC_2D_TRANSFER      = 2U,         /**< 2D transfer.      */
    CY_DMAC_MEMORY_COPY      = 3U,         /**< Memory copy.      */
    CY_DMAC_SCATTER_TRANSFER = 4U          /**< Scatter transfer. */
} cy_en_dmac_descriptor_type_t;

/** Contains the options for the interrupt, trig-in and trig-out type parameters of the descriptor */
typedef enum
{
    CY_DMAC_1ELEMENT    = 0U,              /**< One element transfer.             */
    CY_DMAC_X_LOOP      = 1U,              /**< One X loop transfer.              */
    CY_DMAC_DESCR       = 2U,              /**< One descriptor transfer.          */
    CY_DMAC_DESCR_CHAIN = 3U               /**< Entire descriptor chain transfer. */
} cy_en_dmac_trigger_type_t;

/** Contains the options for the data size */
typedef enum
{
    CY_DMAC_BYTE     = 0U,                 /**< One byte.               */
    CY_DMAC_HALFWORD = 1U,                 /**< Half word (two bytes).  */
    CY_DMAC_WORD     = 2U                  /**< Full word (four bytes). */
} cy_en_dmac_data_size_t;

/** Contains the options for descriptor retriggering */
typedef enum
{
    CY_DMAC_RETRIG_IM      = 0U,           /**< Retrigger immediately.              */
    CY_DMAC_RETRIG_4CYC    = 1U,           /**< Retrigger after 4 Clk_Slow cycles.  */
    CY_DMAC_RETRIG_16CYC   = 2U,           /**< Retrigger after 16 Clk_Slow cycles. */
    CY_DMAC_WAIT_FOR_REACT = 3U            /**< Wait for trigger reactivation.      */
} cy_en_dmac_retrigger_t;

/** Contains the options for the transfer size */
typedef enum
{
    CY_DMAC_TRANSFER_SIZE_DATA = 0U,       /**< As specified by dataSize. */
    CY_DMAC_TRANSFER_SIZE_WORD = 1U,       /**< A full word (four bytes). */
} cy_en_dmac_transfer_size_t;

/** Contains  the options for the state of the channel when the descriptor is completed   */
typedef enum
{
    CY_DMAC_CHANNEL_ENABLED  = 0U,         /**< Channel stays enabled. */
    CY_DMAC_CHANNEL_DISABLED = 1U          /**< Channel is disabled.   */
} cy_en_dmac_channel_state_t;

/** Contains the return values of the DMAC driver */
typedef enum
{
    CY_DMAC_SUCCESS          = 0x0UL,      /**< Success. */
    CY_DMAC_BAD_PARAM        = CY_DMAC_ID | CY_PDL_STATUS_ERROR | 0x1UL /**< The input parameters passed to the DMAC API are not valid. */
} cy_en_dmac_status_t;

/** \} group_dmac_enums */

/** \cond Macros for the conditions used by CY_ASSERT calls */

#define CY_DMAC_IS_LOOP_COUNT_VALID(count)      (((count) >= CY_DMAC_LOOP_COUNT_MIN) && ((count) <= CY_DMAC_LOOP_COUNT_MAX))
#define CY_DMAC_IS_SCATTER_COUNT_VALID(count)   (((count) >= CY_DMAC_LOOP_COUNT_MIN) && ((count) <= CY_DMAC_SCATTER_COUNT_MAX))
#define CY_DMAC_IS_LOOP_INCR_VALID(incr)        (((incr) >= CY_DMAC_LOOP_INCREMENT_MIN) && ((incr) <= CY_DMAC_LOOP_INCREMENT_MAX))
#define CY_DMAC_IS_PRIORITY_VALID(prio)         ((prio) <= 3UL)

#define CY_DMAC_INTR_MASK                       (CY_DMAC_INTR_COMPLETION         | \
                                                 CY_DMAC_INTR_SRC_BUS_ERROR      | \
                                                 CY_DMAC_INTR_DST_BUS_ERROR      | \
                                                 CY_DMAC_INTR_SRC_MISAL          | \
                                                 CY_DMAC_INTR_DST_MISAL          | \
                                                 CY_DMAC_INTR_CURR_PTR_NULL      | \
                                                 CY_DMAC_INTR_ACTIVE_CH_DISABLED | \
                                                 CY_DMAC_INTR_DESCR_BUS_ERROR)

#define CY_DMAC_IS_INTR_MASK_VALID(intr)        (0UL == ((intr) & ((uint32_t) ~CY_DMAC_INTR_MASK)))

#define CY_DMAC_IS_RETRIGGER_VALID(retrigger)   ((CY_DMAC_RETRIG_IM      == (retrigger)) || \
                                                 (CY_DMAC_RETRIG_4CYC    == (retrigger)) || \
                                                 (CY_DMAC_RETRIG_16CYC   == (retrigger)) || \
                                                 (CY_DMAC_WAIT_FOR_REACT == (retrigger)))

#define CY_DMAC_IS_TRIG_TYPE_VALID(trigType)    ((CY_DMAC_1ELEMENT    == (trigType)) || \
                                                 (CY_DMAC_X_LOOP      == (trigType)) || \
                                                 (CY_DMAC_DESCR       == (trigType)) || \
                                                 (CY_DMAC_DESCR_CHAIN == (trigType)))

#define CY_DMAC_IS_XFER_SIZE_VALID(xferSize)    ((CY_DMAC_TRANSFER_SIZE_DATA == (xferSize)) || \
                                                 (CY_DMAC_TRANSFER_SIZE_WORD == (xferSize)))

#define CY_DMAC_IS_CHANNEL_STATE_VALID(state)   ((CY_DMAC_CHANNEL_ENABLED  == (state)) || \
                                                 (CY_DMAC_CHANNEL_DISABLED == (state)))

#define CY_DMAC_IS_DATA_SIZE_VALID(dataSize)    ((CY_DMAC_BYTE     == (dataSize)) || \
                                                 (CY_DMAC_HALFWORD == (dataSize)) || \
                                                 (CY_DMAC_WORD     == (dataSize)))

#define CY_DMAC_IS_TYPE_VALID(descrType)        ((CY_DMAC_SINGLE_TRANSFER  == (descrType)) || \
                                                 (CY_DMAC_1D_TRANSFER      == (descrType)) || \
                                                 (CY_DMAC_2D_TRANSFER      == (descrType)) || \
                                                 (CY_DMAC_MEMORY_COPY      == (descrType)) || \
                                                 (CY_DMAC_SCATTER_TRANSFER == (descrType)))

#define CY_DMAC_IS_CH_NR_VALID(chNr)            (CY_DMAC_CH_NR > (chNr))

/** \endcond */


/**
* \addtogroup group_dmac_data_structures
* \{
*/


/**
* DMAC descriptor structure type. It is a user-declared structure
* allocated in RAM. The DMAC HW requires a pointer to this structure to work with it.
*
* For advanced users: the descriptor can be allocated even in flash, however the user
* have to predefine all the structure items with constants manually,
* because the descriptor "Set" API functions (including \ref Cy_DMAC_Descriptor_Init())
* don't work with read-only descriptors.
*/
typedef struct
{
    uint32_t ctl;                    /*!< 0x00000000 Descriptor control */
    uint32_t src;                    /*!< 0x00000004 Descriptor source */
    uint32_t dst;                    /*!< 0x00000008 Descriptor destination */
    uint32_t xSize;                  /*!< 0x0000000C Descriptor X loop size */
    uint32_t xIncr;                  /*!< 0x00000010 Descriptor X loop increment */
    uint32_t ySize;                  /*!< 0x00000014 Descriptor Y loop size */
    uint32_t yIncr;                  /*!< 0x00000010 Descriptor Y loop increment */
    uint32_t nextPtr;                /*!< 0x00000014 Descriptor next pointer */
} cy_stc_dmac_descriptor_t;

/** \cond The next type-specific descriptor types are ONLY for internal API implementation. */

typedef cy_stc_dmac_descriptor_t cy_stc_dmac_dscr_2d_t;

typedef struct
{
    uint32_t ctl;
    uint32_t src;
    uint32_t dst;
    uint32_t nextPtr;
} cy_stc_dmac_dscr_single_t;

typedef struct
{
    uint32_t ctl;
    uint32_t src;
    uint32_t xSize;
    uint32_t nextPtr;
} cy_stc_dmac_dscr_scatter_t;

typedef struct
{
    uint32_t ctl;
    uint32_t src;
    uint32_t dst;
    uint32_t xSize;
    uint32_t nextPtr;
} cy_stc_dmac_dscr_memcpy_t;

typedef struct
{
    uint32_t ctl;
    uint32_t src;
    uint32_t dst;
    uint32_t xSize;
    uint32_t xIncr;
    uint32_t nextPtr;
} cy_stc_dmac_dscr_1d_t;

/** \endcond */


/**
* This structure is a configuration structure pre-initialized by user
* and passed as a parameter to the \ref Cy_DMAC_Descriptor_Init().
* It can be allocated in RAM/flash (on user's choice).
* In case of flash allocation there is a possibility to reinitialize the descriptor in runtime.
* This structure has all the parameters of the descriptor as separate parameters.
* Most of these parameters are represented in the \ref cy_stc_dmac_descriptor_t structure as bit fields.
*/
typedef struct
{
    cy_en_dmac_retrigger_t       retrigger;       /**< Specifies whether the DMA controller should wait for the input trigger to be deactivated. */
    cy_en_dmac_trigger_type_t    interruptType;   /**< Sets the event that triggers an interrupt. See \ref cy_en_dmac_trigger_type_t. */
    cy_en_dmac_trigger_type_t    triggerOutType;  /**< Sets the event that triggers an output. See \ref cy_en_dmac_trigger_type_t. */
    cy_en_dmac_channel_state_t   channelState;    /**< Specifies whether the channel is enabled or disabled on completion of descriptor see \ref cy_en_dmac_channel_state_t. */
    cy_en_dmac_trigger_type_t    triggerInType;   /**< Sets what type of transfer is triggered. See \ref cy_en_dmac_trigger_type_t. */
    bool                         dataPrefetch;    /**< Source data transfers are initiated as soon as the channel is enabled, the current descriptor pointer is NOT "0"
                                                   *   and there is space available in the channel's data FIFO.
                                                   */
    cy_en_dmac_data_size_t       dataSize;        /**< The size of the data bus for transfer. See \ref cy_en_dmac_data_size_t.
                                                   *   For memory copy and scatter descriptors this setting will be ignored.
                                                   */
    cy_en_dmac_transfer_size_t   srcTransferSize; /**< The source transfer size.
                                                   *   For memory copy and scatter descriptors this setting will be ignored.
                                                   */
    cy_en_dmac_transfer_size_t   dstTransferSize; /**< The destination transfer size.
                                                   *   For memory copy and scatter descriptors this setting will be ignored.
                                                   */
    cy_en_dmac_descriptor_type_t descriptorType;  /**< The type of the descriptor. See \ref cy_en_dmac_descriptor_type_t. */
    void *                       srcAddress;      /**< The source address of the transfer. */
    void *                       dstAddress;      /**< The destination address of the transfer. */
    int32_t                      srcXincrement;   /**< The address increment of the source after each X-loop transfer. Valid range is -32768...32767. */
    int32_t                      dstXincrement;   /**< The address increment of the destination after each X-loop transfer. Valid range is -32768...32767. */
    uint32_t                     xCount;          /**< The number of transfers in an X-loop. Valid range (for all descriptors except scatter transfer) is 1...65536.
                                                   *   For memory copy descriptors, the X count is a nubmer of bytes (not a data transfer size).
                                                   *   For scatter descriptors, the X count is a nubmer of [address, data] pairs (two words each). Valid range is 1...32768.
                                                   */
    int32_t                      srcYincrement;   /**< The address increment of the source after each Y-loop transfer. Valid range is -32768...32767. */
    int32_t                      dstYincrement;   /**< The address increment of the destination after each Y-loop transfer. Valid range is -32768...32767. */
    uint32_t                     yCount;          /**< The number of X-loops in the Y-loop. Valid range is 1...65536. */
    cy_stc_dmac_descriptor_t *   nextDescriptor;  /**< The next descriptor to chain after completion. A NULL value will signify no chaining. */
} cy_stc_dmac_descriptor_config_t;

/** This structure holds the initialization values for the DMAC channel */
typedef struct
{
    cy_stc_dmac_descriptor_t * descriptor;     /**< The DMAC descriptor associated with the channel being initialized.           */
    uint32_t priority;                         /**< This parameter specifies the channel's priority.                            */
    bool     enable;                           /**< This parameter specifies if the channel is enabled after initializing.      */
    bool     bufferable;                       /**< This parameter specifies whether a write transaction can complete.
                                                *   without waiting for the destination to accept the write transaction data.
                                                */
} cy_stc_dmac_channel_config_t;

/** \} group_dmac_data_structures */


/**
* \addtogroup group_dmac_functions
* \{
*/

__STATIC_INLINE void     Cy_DMAC_Enable             (DMAC_Type * base);
__STATIC_INLINE void     Cy_DMAC_Disable            (DMAC_Type * base);
__STATIC_INLINE uint32_t Cy_DMAC_GetActiveChannel   (DMAC_Type const * base);


/**
* \addtogroup group_dmac_channel_functions
* \{
*/
     cy_en_dmac_status_t Cy_DMAC_Channel_Init                    (DMAC_Type       * base, uint32_t channel, cy_stc_dmac_channel_config_t const * config);
                void     Cy_DMAC_Channel_DeInit                  (DMAC_Type       * base, uint32_t channel);
__STATIC_INLINE void     Cy_DMAC_Channel_SetDescriptor           (DMAC_Type       * base, uint32_t channel, cy_stc_dmac_descriptor_t const * descriptor);
__STATIC_INLINE void     Cy_DMAC_Channel_Enable                  (DMAC_Type       * base, uint32_t channel);
__STATIC_INLINE void     Cy_DMAC_Channel_Disable                 (DMAC_Type       * base, uint32_t channel);
__STATIC_INLINE void     Cy_DMAC_Channel_SetPriority             (DMAC_Type       * base, uint32_t channel, uint32_t priority);
__STATIC_INLINE uint32_t Cy_DMAC_Channel_GetPriority             (DMAC_Type const * base, uint32_t channel);
__STATIC_INLINE   void * Cy_DMAC_Channel_GetCurrentSrcAddress    (DMAC_Type           const * base, uint32_t channel);
__STATIC_INLINE   void * Cy_DMAC_Channel_GetCurrentDstAddress    (DMAC_Type           const * base, uint32_t channel);
__STATIC_INLINE uint32_t Cy_DMAC_Channel_GetCurrentXloopIndex    (DMAC_Type const * base, uint32_t channel);
__STATIC_INLINE uint32_t Cy_DMAC_Channel_GetCurrentYloopIndex    (DMAC_Type const * base, uint32_t channel);
__STATIC_INLINE cy_stc_dmac_descriptor_t *
                         Cy_DMAC_Channel_GetCurrentDescriptor    (DMAC_Type const * base, uint32_t channel);
__STATIC_INLINE uint32_t Cy_DMAC_Channel_GetInterruptStatus      (DMAC_Type const * base, uint32_t channel);
__STATIC_INLINE void     Cy_DMAC_Channel_ClearInterrupt          (DMAC_Type       * base, uint32_t channel, uint32_t interrupt);
__STATIC_INLINE void     Cy_DMAC_Channel_SetInterrupt            (DMAC_Type       * base, uint32_t channel, uint32_t interrupt);
__STATIC_INLINE uint32_t Cy_DMAC_Channel_GetInterruptMask        (DMAC_Type const * base, uint32_t channel);
__STATIC_INLINE void     Cy_DMAC_Channel_SetInterruptMask        (DMAC_Type       * base, uint32_t channel, uint32_t interrupt);
__STATIC_INLINE uint32_t Cy_DMAC_Channel_GetInterruptStatusMasked(DMAC_Type const * base, uint32_t channel);

/** \} group_dmac_channel_functions */


/**
* \addtogroup group_dmac_descriptor_functions
* \{
*/

 cy_en_dmac_status_t Cy_DMAC_Descriptor_Init  (cy_stc_dmac_descriptor_t * descriptor, cy_stc_dmac_descriptor_config_t const * config);
                void Cy_DMAC_Descriptor_DeInit(cy_stc_dmac_descriptor_t * descriptor);

                void Cy_DMAC_Descriptor_SetNextDescriptor   (cy_stc_dmac_descriptor_t * descriptor, cy_stc_dmac_descriptor_t const * nextDescriptor);
                void Cy_DMAC_Descriptor_SetDescriptorType   (cy_stc_dmac_descriptor_t * descriptor, cy_en_dmac_descriptor_type_t descriptorType);
__STATIC_INLINE void Cy_DMAC_Descriptor_SetSrcAddress       (cy_stc_dmac_descriptor_t * descriptor, void const * srcAddress);
__STATIC_INLINE void Cy_DMAC_Descriptor_SetDstAddress       (cy_stc_dmac_descriptor_t * descriptor, void const * dstAddress);
                void Cy_DMAC_Descriptor_SetXloopDataCount   (cy_stc_dmac_descriptor_t * descriptor, uint32_t xCount);
__STATIC_INLINE void Cy_DMAC_Descriptor_SetYloopDataCount   (cy_stc_dmac_descriptor_t * descriptor, uint32_t yCount);
__STATIC_INLINE void Cy_DMAC_Descriptor_SetXloopSrcIncrement(cy_stc_dmac_descriptor_t * descriptor, int32_t srcXincrement);
__STATIC_INLINE void Cy_DMAC_Descriptor_SetXloopDstIncrement(cy_stc_dmac_descriptor_t * descriptor, int32_t dstXincrement);
__STATIC_INLINE void Cy_DMAC_Descriptor_SetYloopSrcIncrement(cy_stc_dmac_descriptor_t * descriptor, int32_t srcYincrement);
__STATIC_INLINE void Cy_DMAC_Descriptor_SetYloopDstIncrement(cy_stc_dmac_descriptor_t * descriptor, int32_t dstYincrement);
__STATIC_INLINE void Cy_DMAC_Descriptor_SetInterruptType    (cy_stc_dmac_descriptor_t * descriptor, cy_en_dmac_trigger_type_t interruptType);
__STATIC_INLINE void Cy_DMAC_Descriptor_SetTriggerInType    (cy_stc_dmac_descriptor_t * descriptor, cy_en_dmac_trigger_type_t triggerInType);
__STATIC_INLINE void Cy_DMAC_Descriptor_SetTriggerOutType   (cy_stc_dmac_descriptor_t * descriptor, cy_en_dmac_trigger_type_t triggerOutType);
__STATIC_INLINE void Cy_DMAC_Descriptor_SetDataSize         (cy_stc_dmac_descriptor_t * descriptor, cy_en_dmac_data_size_t dataSize);
__STATIC_INLINE void Cy_DMAC_Descriptor_SetSrcTransferSize  (cy_stc_dmac_descriptor_t * descriptor, cy_en_dmac_transfer_size_t srcTransferSize);
__STATIC_INLINE void Cy_DMAC_Descriptor_SetDstTransferSize  (cy_stc_dmac_descriptor_t * descriptor, cy_en_dmac_transfer_size_t dstTransferSize);
__STATIC_INLINE void Cy_DMAC_Descriptor_SetRetrigger        (cy_stc_dmac_descriptor_t * descriptor, cy_en_dmac_retrigger_t retrigger);
__STATIC_INLINE void Cy_DMAC_Descriptor_SetChannelState     (cy_stc_dmac_descriptor_t * descriptor, cy_en_dmac_channel_state_t channelState);

               cy_stc_dmac_descriptor_t *    Cy_DMAC_Descriptor_GetNextDescriptor   (cy_stc_dmac_descriptor_t const * descriptor);
__STATIC_INLINE cy_en_dmac_descriptor_type_t Cy_DMAC_Descriptor_GetDescriptorType   (cy_stc_dmac_descriptor_t const * descriptor);
__STATIC_INLINE void *                       Cy_DMAC_Descriptor_GetSrcAddress       (cy_stc_dmac_descriptor_t const * descriptor);
__STATIC_INLINE void *                       Cy_DMAC_Descriptor_GetDstAddress       (cy_stc_dmac_descriptor_t const * descriptor);
                uint32_t                     Cy_DMAC_Descriptor_GetXloopDataCount   (cy_stc_dmac_descriptor_t const * descriptor);
__STATIC_INLINE uint32_t                     Cy_DMAC_Descriptor_GetYloopDataCount   (cy_stc_dmac_descriptor_t const * descriptor);
__STATIC_INLINE int32_t                      Cy_DMAC_Descriptor_GetXloopSrcIncrement(cy_stc_dmac_descriptor_t const * descriptor);
__STATIC_INLINE int32_t                      Cy_DMAC_Descriptor_GetXloopDstIncrement(cy_stc_dmac_descriptor_t const * descriptor);
__STATIC_INLINE int32_t                      Cy_DMAC_Descriptor_GetYloopSrcIncrement(cy_stc_dmac_descriptor_t const * descriptor);
__STATIC_INLINE int32_t                      Cy_DMAC_Descriptor_GetYloopDstIncrement(cy_stc_dmac_descriptor_t const * descriptor);
__STATIC_INLINE cy_en_dmac_trigger_type_t    Cy_DMAC_Descriptor_GetInterruptType    (cy_stc_dmac_descriptor_t const * descriptor);
__STATIC_INLINE cy_en_dmac_trigger_type_t    Cy_DMAC_Descriptor_GetTriggerInType    (cy_stc_dmac_descriptor_t const * descriptor);
__STATIC_INLINE cy_en_dmac_trigger_type_t    Cy_DMAC_Descriptor_GetTriggerOutType   (cy_stc_dmac_descriptor_t const * descriptor);
__STATIC_INLINE cy_en_dmac_data_size_t       Cy_DMAC_Descriptor_GetDataSize         (cy_stc_dmac_descriptor_t const * descriptor);
__STATIC_INLINE cy_en_dmac_transfer_size_t   Cy_DMAC_Descriptor_GetSrcTransferSize  (cy_stc_dmac_descriptor_t const * descriptor);
__STATIC_INLINE cy_en_dmac_transfer_size_t   Cy_DMAC_Descriptor_GetDstTransferSize  (cy_stc_dmac_descriptor_t const * descriptor);
__STATIC_INLINE cy_en_dmac_retrigger_t       Cy_DMAC_Descriptor_GetRetrigger        (cy_stc_dmac_descriptor_t const * descriptor);
__STATIC_INLINE cy_en_dmac_channel_state_t   Cy_DMAC_Descriptor_GetChannelState     (cy_stc_dmac_descriptor_t const * descriptor);

/** \} group_dmac_descriptor_functions */


/***************************************
*    In-line Function Implementation
***************************************/


/**
* \addtogroup group_dmac_block_functions
* \{
*/


/*******************************************************************************
* Function Name: Cy_DMAC_Enable
****************************************************************************//**
*
* Enables the DMACblock.
*
* \param base
* The pointer to the hardware DMAC block.
*
* \funcusage
* \snippet dmac/snippet/main.c snippet_Cy_DMAC_Enable
*
*******************************************************************************/
__STATIC_INLINE void Cy_DMAC_Enable(DMAC_Type * base)
{
    DMAC_CTL(base) |= DMAC_V2_CTL_ENABLED_Msk;
}


/*******************************************************************************
* Function Name: Cy_DMAC_Disable
****************************************************************************//**
*
* Disables the DMACblock.
*
* \param base
* The pointer to the hardware DMAC block.
*
* \funcusage
* \snippet dmac/snippet/main.c snippet_Cy_DMAC_Disable
*
*******************************************************************************/
__STATIC_INLINE void Cy_DMAC_Disable(DMAC_Type * base)
{
    DMAC_CTL(base) &= (uint32_t) ~DMAC_V2_CTL_ENABLED_Msk;
}


/*******************************************************************************
* Function Name: Cy_DMAC_GetActiveChannel
****************************************************************************//**
*
* Returns the status of the active/pending channels of the DMACblock.
*
* \param base
* The pointer to the hardware DMAC block.
*
* \return
* Returns a bit-field with all of the currently active/pending channels in the
* DMAC block.
*
* \funcusage
* \snippet dmac/snippet/main.c snippet_Cy_DMAC_Disable
*
*******************************************************************************/
__STATIC_INLINE uint32_t Cy_DMAC_GetActiveChannel(DMAC_Type const * base)
{
    return(_FLD2VAL(DMAC_V2_ACTIVE_ACTIVE, DMAC_ACTIVE(base)));
}

/** \} group_dmac_block_functions */


/**
* \addtogroup group_dmac_descriptor_functions
* \{
*/


/*******************************************************************************
* Function Name: Cy_DMAC_Descriptor_SetSrcAddress
****************************************************************************//**
*
* Sets the source address parameter for the specified descriptor.
*
* \param descriptor
* The descriptor structure instance.
*
* \param srcAddress
* The source address value for the descriptor.
*
* \funcusage
* \snippet dmac/snippet/main.c snippet_Cy_DMAC_Descriptor_SetterFunctions
*
*******************************************************************************/
__STATIC_INLINE void Cy_DMAC_Descriptor_SetSrcAddress(cy_stc_dmac_descriptor_t * descriptor, void const * srcAddress)
{
    descriptor->src = (uint32_t) srcAddress;
}


/*******************************************************************************
* Function Name: Cy_DMAC_Descriptor_GetSrcAddress
****************************************************************************//**
*
* Returns the source address of the specified descriptor.
*
* \param descriptor
* The descriptor structure instance.
*
* \return
* The source address value of the descriptor.
*
* \funcusage
* \snippet dmac/snippet/main.c snippet_Cy_DMAC_Descriptor_GetterFunctions
*
*******************************************************************************/
__STATIC_INLINE void * Cy_DMAC_Descriptor_GetSrcAddress(cy_stc_dmac_descriptor_t const * descriptor)
{
    return ((void *) descriptor->src);
}


/*******************************************************************************
* Function Name: Cy_DMAC_Descriptor_SetDstAddress
****************************************************************************//**
*
* Sets the destination address parameter for the specified descriptor.
*
* \param descriptor
* The descriptor structure instance.
*
* \param dstAddress
* The destination address value for the descriptor.
*
* \funcusage
* \snippet dmac/snippet/main.c snippet_Cy_DMAC_Descriptor_SetterFunctions
*
*******************************************************************************/
__STATIC_INLINE void Cy_DMAC_Descriptor_SetDstAddress(cy_stc_dmac_descriptor_t * descriptor, void const * dstAddress)
{
    CY_ASSERT_L1(CY_DMAC_SCATTER_TRANSFER != Cy_DMAC_Descriptor_GetDescriptorType(descriptor));

    descriptor->dst = (uint32_t) dstAddress;
}


/*******************************************************************************
* Function Name: Cy_DMAC_Descriptor_GetDstAddress
****************************************************************************//**
*
* Returns the destination address of the specified descriptor.
*
* \param descriptor
* The descriptor structure instance.
*
* \return
* The destination address value of the descriptor.
*
* \funcusage
* \snippet dmac/snippet/main.c snippet_Cy_DMAC_Descriptor_GetterFunctions
*
*******************************************************************************/
__STATIC_INLINE void * Cy_DMAC_Descriptor_GetDstAddress(cy_stc_dmac_descriptor_t const * descriptor)
{
    CY_ASSERT_L1(CY_DMAC_SCATTER_TRANSFER != Cy_DMAC_Descriptor_GetDescriptorType(descriptor));

    return ((void *) descriptor->dst);
}


/*******************************************************************************
* Function Name: Cy_DMAC_Descriptor_SetInterruptType
****************************************************************************//**
*
* Sets the interrupt type parameter for the specified descriptor.
*
* \param descriptor
* The descriptor structure instance.
*
* \param interruptType
* The interrupt type set for the descriptor. \ref cy_en_dmac_trigger_type_t
*
* \funcusage
* \snippet dmac/snippet/main.c snippet_Cy_DMAC_Descriptor_SetterFunctions
*
*******************************************************************************/
__STATIC_INLINE void Cy_DMAC_Descriptor_SetInterruptType(cy_stc_dmac_descriptor_t * descriptor, cy_en_dmac_trigger_type_t interruptType)
{
    CY_ASSERT_L3(CY_DMAC_IS_TRIG_TYPE_VALID(interruptType));

    CY_REG32_CLR_SET(descriptor->ctl, DMAC_CH_V2_DESCR_CTL_INTR_TYPE, interruptType);
}


/*******************************************************************************
* Function Name: Cy_DMAC_Descriptor_GetInterruptType
****************************************************************************//**
*
* Returns the Interrupt-Type of the specified descriptor.
*
* \param descriptor
* The descriptor structure instance.
*
* \return
* The Interrupt-Type \ref cy_en_dmac_trigger_type_t.
*
* \funcusage
* \snippet dmac/snippet/main.c snippet_Cy_DMAC_Descriptor_GetterFunctions
*
*******************************************************************************/
__STATIC_INLINE cy_en_dmac_trigger_type_t Cy_DMAC_Descriptor_GetInterruptType(cy_stc_dmac_descriptor_t const * descriptor)
{
    return((cy_en_dmac_trigger_type_t) _FLD2VAL(DMAC_CH_V2_DESCR_CTL_INTR_TYPE, descriptor->ctl));
}


/*******************************************************************************
* Function Name: Cy_DMAC_Descriptor_SetTriggerInType
****************************************************************************//**
*
* Sets the Trigger In Type parameter for the specified descriptor.
*
* \param descriptor
* The descriptor structure instance.
*
* \param triggerInType
* The Trigger In Type parameter \ref cy_en_dmac_trigger_type_t
*
* \funcusage
* \snippet dmac/snippet/main.c snippet_Cy_DMAC_Descriptor_SetterFunctions
*
*******************************************************************************/
__STATIC_INLINE void Cy_DMAC_Descriptor_SetTriggerInType(cy_stc_dmac_descriptor_t * descriptor, cy_en_dmac_trigger_type_t triggerInType)
{
    CY_ASSERT_L3(CY_DMAC_IS_TRIG_TYPE_VALID(triggerInType));

    CY_REG32_CLR_SET(descriptor->ctl, DMAC_CH_V2_DESCR_CTL_TR_IN_TYPE, triggerInType);
}


/*******************************************************************************
* Function Name: Cy_DMAC_Descriptor_GetTriggerInType
****************************************************************************//**
*
* Returns the Trigger In Type of the specified descriptor.
*
* \param descriptor
* The descriptor structure instance.
*
* \return
* The Trigger In Type \ref cy_en_dmac_trigger_type_t
*
* \funcusage
* \snippet dmac/snippet/main.c snippet_Cy_DMAC_Descriptor_GetterFunctions
*
*******************************************************************************/
__STATIC_INLINE cy_en_dmac_trigger_type_t Cy_DMAC_Descriptor_GetTriggerInType(cy_stc_dmac_descriptor_t const * descriptor)
{
    return((cy_en_dmac_trigger_type_t) _FLD2VAL(DMAC_CH_V2_DESCR_CTL_TR_IN_TYPE, descriptor->ctl));
}


/*******************************************************************************
* Function Name: Cy_DMAC_Descriptor_SetTriggerOutType
****************************************************************************//**
*
* Sets the Trigger Out Type parameter for the specified descriptor.
*
* \param descriptor
* The descriptor structure instance.
*
* \param triggerOutType
* The Trigger Out Type set for the descriptor. \ref cy_en_dmac_trigger_type_t
*
* \funcusage
* \snippet dmac/snippet/main.c snippet_Cy_DMAC_Descriptor_SetterFunctions
*
*******************************************************************************/
__STATIC_INLINE void Cy_DMAC_Descriptor_SetTriggerOutType(cy_stc_dmac_descriptor_t * descriptor, cy_en_dmac_trigger_type_t triggerOutType)
{
    CY_ASSERT_L3(CY_DMAC_IS_TRIG_TYPE_VALID(triggerOutType));

    CY_REG32_CLR_SET(descriptor->ctl, DMAC_CH_V2_DESCR_CTL_TR_OUT_TYPE, triggerOutType);
}


/*******************************************************************************
* Function Name: Cy_DMAC_Descriptor_GetTriggerOutType
****************************************************************************//**
*
* Returns the Trigger Out Type of the specified descriptor.
*
* \param descriptor
* The descriptor structure instance.
*
* \return
* The Trigger Out Type parameter \ref cy_en_dmac_trigger_type_t.
*
* \funcusage
* \snippet dmac/snippet/main.c snippet_Cy_DMAC_Descriptor_GetterFunctions
*
*******************************************************************************/
__STATIC_INLINE cy_en_dmac_trigger_type_t Cy_DMAC_Descriptor_GetTriggerOutType(cy_stc_dmac_descriptor_t const * descriptor)
{
    return((cy_en_dmac_trigger_type_t) _FLD2VAL(DMAC_CH_V2_DESCR_CTL_TR_OUT_TYPE, descriptor->ctl));
}


/*******************************************************************************
* Function Name: Cy_DMAC_Descriptor_SetDataSize
****************************************************************************//**
*
* Sets the Data Element Size parameter for the specified descriptor.
*
* \param descriptor
* The descriptor structure instance.
*
* \param dataSize
* The Data Element Size \ref cy_en_dmac_data_size_t
*
* \funcusage
* \snippet dmac/snippet/main.c snippet_Cy_DMAC_Descriptor_SetterFunctions
*
*******************************************************************************/
__STATIC_INLINE void Cy_DMAC_Descriptor_SetDataSize(cy_stc_dmac_descriptor_t * descriptor, cy_en_dmac_data_size_t dataSize)
{
    CY_ASSERT_L3(CY_DMAC_IS_DATA_SIZE_VALID(dataSize));

    CY_REG32_CLR_SET(descriptor->ctl, DMAC_CH_V2_DESCR_CTL_DATA_SIZE, dataSize);
}


/*******************************************************************************
* Function Name: Cy_DMAC_Descriptor_GetDataSize
****************************************************************************//**
*
* Returns the Data Element Size of the specified descriptor.
*
* \param descriptor
* The descriptor structure instance.
*
* \return
* The Data Element Size \ref cy_en_dmac_data_size_t.
*
* \funcusage
* \snippet dmac/snippet/main.c snippet_Cy_DMAC_Descriptor_GetterFunctions
*
*******************************************************************************/
__STATIC_INLINE cy_en_dmac_data_size_t Cy_DMAC_Descriptor_GetDataSize(cy_stc_dmac_descriptor_t const * descriptor)
{
    return((cy_en_dmac_data_size_t) _FLD2VAL(DMAC_CH_V2_DESCR_CTL_DATA_SIZE, descriptor->ctl));
}


/*******************************************************************************
* Function Name: Cy_DMAC_Descriptor_SetSrcTransferSize
****************************************************************************//**
*
* Sets the Source Transfer Size parameter for the specified descriptor.
*
* \param descriptor
* The descriptor structure instance.
*
* \param srcTransferSize
* The Source Transfer Size \ref cy_en_dmac_transfer_size_t.
*
* \funcusage
* \snippet dmac/snippet/main.c snippet_Cy_DMAC_Descriptor_SetterFunctions
*******************************************************************************/
__STATIC_INLINE void Cy_DMAC_Descriptor_SetSrcTransferSize(cy_stc_dmac_descriptor_t * descriptor, cy_en_dmac_transfer_size_t srcTransferSize)
{
    CY_ASSERT_L3(CY_DMAC_IS_XFER_SIZE_VALID(srcTransferSize));

    CY_REG32_CLR_SET(descriptor->ctl, DMAC_CH_V2_DESCR_CTL_SRC_TRANSFER_SIZE, srcTransferSize);
}


/*******************************************************************************
* Function Name: Cy_DMAC_Descriptor_GetSrcTransferSize
****************************************************************************//**
*
* Returns the Source Transfer Size of the specified descriptor.
*
* \param descriptor
* The descriptor structure instance.
*
* \return
* The Source Transfer Size \ref cy_en_dmac_transfer_size_t.
*
* \funcusage
* \snippet dmac/snippet/main.c snippet_Cy_DMAC_Descriptor_GetterFunctions
*
*******************************************************************************/
__STATIC_INLINE cy_en_dmac_transfer_size_t Cy_DMAC_Descriptor_GetSrcTransferSize(cy_stc_dmac_descriptor_t const * descriptor)
{
    return((cy_en_dmac_transfer_size_t) _FLD2VAL(DMAC_CH_V2_DESCR_CTL_SRC_TRANSFER_SIZE, descriptor->ctl));
}


/*******************************************************************************
* Function Name: Cy_DMAC_Descriptor_SetDstTransferSize
****************************************************************************//**
*
* Sets the Destination Transfer Size parameter for the specified descriptor.
*
* \param descriptor
* The descriptor structure instance.
*
* \param dstTransferSize
* The Destination Transfer Size \ref cy_en_dmac_transfer_size_t.
*
* \funcusage
* \snippet dmac/snippet/main.c snippet_Cy_DMAC_Descriptor_SetterFunctions
*
*******************************************************************************/
__STATIC_INLINE void Cy_DMAC_Descriptor_SetDstTransferSize(cy_stc_dmac_descriptor_t * descriptor, cy_en_dmac_transfer_size_t dstTransferSize)
{
    CY_ASSERT_L3(CY_DMAC_IS_XFER_SIZE_VALID(dstTransferSize));

    CY_REG32_CLR_SET(descriptor->ctl, DMAC_CH_V2_DESCR_CTL_DST_TRANSFER_SIZE, dstTransferSize);
}


/*******************************************************************************
* Function Name: Cy_DMAC_Descriptor_GetDstTransferSize
****************************************************************************//**
*
* Returns the Destination Transfer Size of the specified descriptor.
*
* \param descriptor
* The descriptor structure instance.
*
* \return
* The Destination Transfer Size \ref cy_en_dmac_transfer_size_t
*
* \funcusage
* \snippet dmac/snippet/main.c snippet_Cy_DMAC_Descriptor_GetterFunctions
*
*******************************************************************************/
__STATIC_INLINE cy_en_dmac_transfer_size_t Cy_DMAC_Descriptor_GetDstTransferSize(cy_stc_dmac_descriptor_t const * descriptor)
{
    return((cy_en_dmac_transfer_size_t) _FLD2VAL(DMAC_CH_V2_DESCR_CTL_DST_TRANSFER_SIZE, descriptor->ctl));
}


/*******************************************************************************
* Function Name: Cy_DMAC_Descriptor_SetRetrigger
****************************************************************************//**
*
* Sets the retrigger value which specifies whether the controller should
* wait for the input trigger to be deactivated.
*
* \param descriptor
* The descriptor structure instance.
*
* \param retrigger
* The \ref cy_en_dmac_retrigger_t parameter specifies whether the controller
* should wait for the input trigger to be deactivated.
*
* \funcusage
* \snippet dmac/snippet/main.c snippet_Cy_DMAC_Descriptor_SetterFunctions
*
*******************************************************************************/
__STATIC_INLINE void Cy_DMAC_Descriptor_SetRetrigger(cy_stc_dmac_descriptor_t * descriptor, cy_en_dmac_retrigger_t retrigger)
{
    CY_ASSERT_L3(CY_DMAC_IS_RETRIGGER_VALID(retrigger));

    CY_REG32_CLR_SET(descriptor->ctl, DMAC_CH_V2_DESCR_CTL_WAIT_FOR_DEACT, retrigger);
}


/*******************************************************************************
* Function Name: Cy_DMAC_Descriptor_GetRetrigger
****************************************************************************//**
*
* Returns a value which specifies whether the controller should
* wait for the input trigger to be deactivated.
*
* \param descriptor
* The descriptor structure instance.
*
* \return
* The Retrigger setting \ref cy_en_dmac_retrigger_t.
*
* \funcusage
* \snippet dmac/snippet/main.c snippet_Cy_DMAC_Descriptor_GetterFunctions
*
*******************************************************************************/
__STATIC_INLINE cy_en_dmac_retrigger_t Cy_DMAC_Descriptor_GetRetrigger(cy_stc_dmac_descriptor_t const * descriptor)
{
    return((cy_en_dmac_retrigger_t) _FLD2VAL(DMAC_CH_V2_DESCR_CTL_WAIT_FOR_DEACT, descriptor->ctl));
}


/*******************************************************************************
* Function Name: Cy_DMAC_Descriptor_GetDescriptorType
****************************************************************************//**
*
* Returns the descriptor's type of the specified descriptor.
*
* \param descriptor
* The descriptor structure instance.
*
* \return
* The descriptor type \ref cy_en_dmac_descriptor_type_t
*
* \funcusage
* \snippet dmac/snippet/main.c snippet_Cy_DMAC_Descriptor_GetterFunctions
*
*******************************************************************************/
__STATIC_INLINE cy_en_dmac_descriptor_type_t Cy_DMAC_Descriptor_GetDescriptorType(cy_stc_dmac_descriptor_t const * descriptor)
{
    return((cy_en_dmac_descriptor_type_t) _FLD2VAL(DMAC_CH_V2_DESCR_CTL_DESCR_TYPE, descriptor->ctl));
}


/*******************************************************************************
* Function Name: Cy_DMAC_Descriptor_SetChannelState
****************************************************************************//**
*
* Sets the channel state on completion of the specified descriptor.
*
* \param descriptor
* The descriptor structure instance.
*
* \param channelState
* The channel state \ref cy_en_dmac_channel_state_t.
*
* \funcusage
* \snippet dmac/snippet/main.c snippet_Cy_DMAC_Descriptor_SetterFunctions
*
*******************************************************************************/
__STATIC_INLINE void Cy_DMAC_Descriptor_SetChannelState(cy_stc_dmac_descriptor_t * descriptor, cy_en_dmac_channel_state_t channelState)
{
    CY_ASSERT_L3(CY_DMAC_IS_CHANNEL_STATE_VALID(channelState));

    CY_REG32_CLR_SET(descriptor->ctl, DMAC_CH_V2_DESCR_CTL_CH_DISABLE, channelState);
}


/*******************************************************************************
* Function Name: Cy_DMAC_Descriptor_GetChannelState
****************************************************************************//**
*
* Returns the channel state on completion of the specified descriptor.
*
* \param descriptor
* The descriptor structure instance.
*
* \return
* The Channel State setting \ref cy_en_dmac_channel_state_t
*
* \funcusage
* \snippet dmac/snippet/main.c snippet_Cy_DMAC_Descriptor_GetterFunctions
*
*******************************************************************************/
__STATIC_INLINE cy_en_dmac_channel_state_t Cy_DMAC_Descriptor_GetChannelState(cy_stc_dmac_descriptor_t const * descriptor)
{
    return((cy_en_dmac_channel_state_t) _FLD2VAL(DMAC_CH_V2_DESCR_CTL_CH_DISABLE, descriptor->ctl));
}


/*******************************************************************************
* Function Name: Cy_DMAC_Descriptor_SetXloopSrcIncrement
****************************************************************************//**
*
* Sets the source increment parameter for the X loop of the specified
* descriptor (for 1D or 2D descriptors only).
*
* \param descriptor
* The descriptor structure instance.
*
* \param srcXincrement
* The value of the source increment. The valid range is -32768 ... 32767.
*
* \funcusage
* \snippet dmac/snippet/main.c snippet_Cy_DMAC_Descriptor_SetterFunctions
*
*******************************************************************************/
__STATIC_INLINE void Cy_DMAC_Descriptor_SetXloopSrcIncrement(cy_stc_dmac_descriptor_t * descriptor, int32_t srcXincrement)
{
    CY_ASSERT_L1(CY_DMAC_SINGLE_TRANSFER != Cy_DMAC_Descriptor_GetDescriptorType(descriptor));
    CY_ASSERT_L2(CY_DMAC_IS_LOOP_INCR_VALID(srcXincrement));

    CY_REG32_CLR_SET(descriptor->xIncr, DMAC_CH_V2_DESCR_X_INCR_SRC_X, srcXincrement);
}


/*******************************************************************************
* Function Name: Cy_DMAC_Descriptor_GetXloopSrcIncrement
****************************************************************************//**
*
* Returns the source increment parameter for the X loop of the specified
* descriptor (for 1D or 2D descriptors only).
*
* \param descriptor
* The descriptor structure instance.
*
* \return
* The value of the source increment. The range is -32768 ... 32767.
*
* \funcusage
* \snippet dmac/snippet/main.c snippet_Cy_DMAC_Descriptor_GetterFunctions
*
*******************************************************************************/
__STATIC_INLINE int32_t Cy_DMAC_Descriptor_GetXloopSrcIncrement(cy_stc_dmac_descriptor_t const * descriptor)
{
    CY_ASSERT_L1(CY_DMAC_SINGLE_TRANSFER != Cy_DMAC_Descriptor_GetDescriptorType(descriptor));

    return ((int32_t) _FLD2VAL(DMAC_CH_V2_DESCR_X_INCR_SRC_X, descriptor->xIncr));
}


/*******************************************************************************
* Function Name: Cy_DMAC_Descriptor_SetXloopDstIncrement
****************************************************************************//**
*
* Sets the destination increment parameter for the X loop for the specified
* descriptor (for 1D or 2D descriptors only).
*
* \param descriptor
* The descriptor structure instance.
*
* \param dstXincrement
* The value of the destination increment. The valid range is -32768 ... 32767.
*
* \funcusage
* \snippet dmac/snippet/main.c snippet_Cy_DMAC_Descriptor_SetterFunctions
*
*******************************************************************************/
__STATIC_INLINE void Cy_DMAC_Descriptor_SetXloopDstIncrement(cy_stc_dmac_descriptor_t * descriptor, int32_t dstXincrement)
{
    CY_ASSERT_L1(CY_DMAC_SINGLE_TRANSFER != Cy_DMAC_Descriptor_GetDescriptorType(descriptor));
    CY_ASSERT_L2(CY_DMAC_IS_LOOP_INCR_VALID(dstXincrement));

    CY_REG32_CLR_SET(descriptor->xIncr, DMAC_CH_V2_DESCR_X_INCR_DST_X, dstXincrement);
}


/*******************************************************************************
* Function Name: Cy_DMAC_Descriptor_GetXloopDstIncrement
****************************************************************************//**
*
* Returns the destination increment parameter for the X loop of the specified
* descriptor (for 1D or 2D descriptors only).
*
* \param descriptor
* The descriptor structure instance.
*
* \return
* The value of the destination increment. The range is -32768 ... 32767.
*
* \funcusage
* \snippet dmac/snippet/main.c snippet_Cy_DMAC_Descriptor_GetterFunctions
*
*******************************************************************************/
__STATIC_INLINE int32_t Cy_DMAC_Descriptor_GetXloopDstIncrement(cy_stc_dmac_descriptor_t const * descriptor)
{
    CY_ASSERT_L1(CY_DMAC_SINGLE_TRANSFER != Cy_DMAC_Descriptor_GetDescriptorType(descriptor));

    return ((int32_t) _FLD2VAL(DMAC_CH_V2_DESCR_X_INCR_DST_X, descriptor->xIncr));
}


/*******************************************************************************
* Function Name: Cy_DMAC_Descriptor_SetYloopDataCount
****************************************************************************//**
*
* Sets the number of data elements for the Y loop of the specified descriptor
* (for 2D descriptors only).
*
* \param descriptor
* The descriptor structure instance.
*
* \param yCount
* The number of X loops to execute in the Y loop. The valid range is 1 ... 65536.
*
* \funcusage
* \snippet dmac/snippet/main.c snippet_Cy_DMAC_Descriptor_SetterFunctions
*
*******************************************************************************/
__STATIC_INLINE void Cy_DMAC_Descriptor_SetYloopDataCount(cy_stc_dmac_descriptor_t * descriptor, uint32_t yCount)
{
    CY_ASSERT_L1(CY_DMAC_2D_TRANSFER == Cy_DMAC_Descriptor_GetDescriptorType(descriptor));
    CY_ASSERT_L2(CY_DMAC_IS_LOOP_COUNT_VALID(yCount));
    /* Convert the data count from the user's range (1-65536) into the machine range (0-65535). */
    CY_REG32_CLR_SET(descriptor->ySize, DMAC_CH_V2_DESCR_Y_SIZE_Y_COUNT, yCount - 1UL);
}


/*******************************************************************************
* Function Name: Cy_DMAC_Descriptor_GetYloopDataCount
****************************************************************************//**
*
* Returns the number of X loops to execute in the Y loop of the specified
* descriptor (for 2D descriptors only).
*
* \param descriptor
* The descriptor structure instance.
*
* \return
* The number of X loops to execute in the Y loop. The range is 1 ... 65536.
*
* \funcusage
* \snippet dmac/snippet/main.c snippet_Cy_DMAC_Descriptor_GetterFunctions
*
*******************************************************************************/
__STATIC_INLINE uint32_t Cy_DMAC_Descriptor_GetYloopDataCount(cy_stc_dmac_descriptor_t const * descriptor)
{
    CY_ASSERT_L1(CY_DMAC_2D_TRANSFER == Cy_DMAC_Descriptor_GetDescriptorType(descriptor));
    /* Convert the data count from the machine range (0-65535) into the user's range (1-65536). */
    return (_FLD2VAL(DMAC_CH_V2_DESCR_Y_SIZE_Y_COUNT, descriptor->ySize) + 1UL);
}


/*******************************************************************************
* Function Name: Cy_DMAC_Descriptor_SetYloopSrcIncrement
****************************************************************************//**
*
* Sets the source increment parameter for the Y loop for the specified
* descriptor (for 2D descriptors only).
*
* \param descriptor
* The descriptor structure instance.
*
* \param srcYincrement
* The value of the source increment. The valid range is -32768 ... 32767.
*
* \funcusage
* \snippet dmac/snippet/main.c snippet_Cy_DMAC_Descriptor_SetterFunctions
*
*******************************************************************************/
__STATIC_INLINE void Cy_DMAC_Descriptor_SetYloopSrcIncrement(cy_stc_dmac_descriptor_t * descriptor, int32_t srcYincrement)
{
    CY_ASSERT_L1(CY_DMAC_2D_TRANSFER == Cy_DMAC_Descriptor_GetDescriptorType(descriptor));
    CY_ASSERT_L2(CY_DMAC_IS_LOOP_INCR_VALID(srcYincrement));

    CY_REG32_CLR_SET(descriptor->yIncr, DMAC_CH_V2_DESCR_Y_INCR_SRC_Y, srcYincrement);
}


/*******************************************************************************
* Function Name: Cy_DMAC_Descriptor_GetYloopSrcIncrement
****************************************************************************//**
*
* Returns the source increment parameter for the Y loop of the specified
* descriptor (for 2D descriptors only).
*
* \param descriptor
* The descriptor structure instance.
*
* \return
* The value of source increment. The range is -32768 ... 32767.
*
* \funcusage
* \snippet dmac/snippet/main.c snippet_Cy_DMAC_Descriptor_GetterFunctions
*
*******************************************************************************/
__STATIC_INLINE int32_t Cy_DMAC_Descriptor_GetYloopSrcIncrement(cy_stc_dmac_descriptor_t const * descriptor)
{
    CY_ASSERT_L1(CY_DMAC_2D_TRANSFER == Cy_DMAC_Descriptor_GetDescriptorType(descriptor));

    return ((int32_t) _FLD2VAL(DMAC_CH_V2_DESCR_Y_INCR_SRC_Y, descriptor->yIncr));
}


/*******************************************************************************
* Function Name: Cy_DMAC_Descriptor_SetYloopDstIncrement
****************************************************************************//**
*
* Sets the destination increment parameter for the Y loop of the specified
* descriptor (for 2D descriptors only).
*
* \param descriptor
* The descriptor structure instance.
*
* \param dstYincrement
* The value of the destination increment. The valid range is -32768 ... 32767.
*
* \funcusage
* \snippet dmac/snippet/main.c snippet_Cy_DMAC_Descriptor_SetterFunctions
*
*******************************************************************************/
__STATIC_INLINE void Cy_DMAC_Descriptor_SetYloopDstIncrement(cy_stc_dmac_descriptor_t * descriptor, int32_t dstYincrement)
{
    CY_ASSERT_L1(CY_DMAC_2D_TRANSFER == Cy_DMAC_Descriptor_GetDescriptorType(descriptor));
    CY_ASSERT_L2(CY_DMAC_IS_LOOP_INCR_VALID(dstYincrement));

    CY_REG32_CLR_SET(descriptor->yIncr, DMAC_CH_V2_DESCR_Y_INCR_DST_Y, dstYincrement);
}


/*******************************************************************************
* Function Name: Cy_DMAC_Descriptor_GetYloopDstIncrement
****************************************************************************//**
*
* Returns the destination increment parameter for the Y loop of the specified
* descriptor (for 2D descriptors only).
*
* \param descriptor
* The descriptor structure instance.
*
* \return
* The value of the destination increment. The range is -32768 ... 32767.
*
* \funcusage
* \snippet dmac/snippet/main.c snippet_Cy_DMAC_Descriptor_GetterFunctions
*
*******************************************************************************/
__STATIC_INLINE int32_t Cy_DMAC_Descriptor_GetYloopDstIncrement(cy_stc_dmac_descriptor_t const * descriptor)
{
    CY_ASSERT_L1(CY_DMAC_2D_TRANSFER == Cy_DMAC_Descriptor_GetDescriptorType(descriptor));

    return ((int32_t) _FLD2VAL(DMAC_CH_V2_DESCR_Y_INCR_DST_Y, descriptor->yIncr));
}

/** \} group_dmac_descriptor_functions */


/**
* \addtogroup group_dmac_channel_functions
* \{
*/


/*******************************************************************************
* Function Name: Cy_DMAC_Channel_SetDescriptor
****************************************************************************//**
*
* Sets a descriptor as current for the specified DMACchannel.
*
* \param base
* The pointer to the hardware DMAC block.
*
* \param channel
* The channel number.
*
* \param descriptor
* This is the descriptor to be associated with the channel.
*
* \funcusage
* \snippet dmac/snippet/main.c snippet_Cy_DMAC_Enable
*
*******************************************************************************/
__STATIC_INLINE void Cy_DMAC_Channel_SetDescriptor(DMAC_Type * base, uint32_t channel, cy_stc_dmac_descriptor_t const * descriptor)
{
    CY_ASSERT_L1(CY_DMAC_IS_CH_NR_VALID(channel));

    DMAC_CH_CURR(base, channel) = (uint32_t)descriptor;
}


/*******************************************************************************
* Function Name: Cy_DMAC_Channel_Enable
****************************************************************************//**
*
* Enables a DMAC channel.
*
* \param base
* The pointer to the hardware DMAC block.
*
* \param channel
* The  channel number.
*
* \funcusage
* \snippet dmac/snippet/main.c snippet_Cy_DMAC_Enable
*
*******************************************************************************/
__STATIC_INLINE void Cy_DMAC_Channel_Enable(DMAC_Type * base, uint32_t channel)
{
    CY_ASSERT_L1(CY_DMAC_IS_CH_NR_VALID(channel));

    DMAC_CH_CTL(base, channel) |= DMAC_CH_V2_CTL_ENABLED_Msk;
}


/*******************************************************************************
* Function Name: Cy_DMAC_Channel_Disable
****************************************************************************//**
*
* Disables a DMACchannel.
*
* \param base
* The pointer to the hardware DMAC block.
*
* \param channel
* The channel number.
*
* \funcusage
* \snippet dmac/snippet/main.c snippet_Cy_DMAC_Disable
*
*******************************************************************************/
__STATIC_INLINE void Cy_DMAC_Channel_Disable(DMAC_Type * base, uint32_t channel)
{
    CY_ASSERT_L1(CY_DMAC_IS_CH_NR_VALID(channel));

    DMAC_CH_CTL(base, channel) &= (uint32_t) ~DMAC_CH_V2_CTL_ENABLED_Msk;
}


/*******************************************************************************
* Function Name: Cy_DMAC_Channel_SetPriority
****************************************************************************//**
*
* The function is used to set a priority for the DMAC channel.
*
* \param base
* The pointer to the hardware DMAC block.
*
* \param channel
* The channel number.
*
* \param priority
* The priority to be set for the DMAC channel. The allowed values are 0,1,2,3.
*
* \funcusage
* \snippet dmac/snippet/main.c snippet_Cy_DMAC_Enable
*
*******************************************************************************/
__STATIC_INLINE void Cy_DMAC_Channel_SetPriority(DMAC_Type * base, uint32_t channel, uint32_t priority)
{
    CY_ASSERT_L1(CY_DMAC_IS_CH_NR_VALID(channel));
    CY_ASSERT_L2(CY_DMAC_IS_PRIORITY_VALID(priority));

    CY_REG32_CLR_SET(DMAC_CH_CTL(base, channel), DMAC_CH_V2_CTL_PRIO, priority);
}


/*******************************************************************************
* Function Name: Cy_DMAC_Channel_GetPriority
****************************************************************************//**
*
* Returns the priority of the DMAC channel.
*
* \param base
* The pointer to the hardware DMAC block.
*
* \param channel
* The channel number.
*
* \return
* The priority of the channel.
*
* \funcusage
* \snippet dmac/snippet/main.c snippet_Cy_DMAC_Disable
*
*******************************************************************************/
__STATIC_INLINE uint32_t Cy_DMAC_Channel_GetPriority(DMAC_Type const * base, uint32_t channel)
{
    CY_ASSERT_L1(CY_DMAC_IS_CH_NR_VALID(channel));

    return ((uint32_t) _FLD2VAL(DMAC_CH_V2_CTL_PRIO, DMAC_CH_CTL(base, channel)));
}


/*******************************************************************************
* Function Name: Cy_DMAC_Channel_GetCurrentSrcAddress
****************************************************************************//**
*
* Returns the source address being used for the current transfer.
*
* \param base
* The pointer to the hardware DMAC block.
*
* \param channel
* The channel number.
*
* \return
* Returns the pointer to the source of transfer.
*
* \funcusage
* \snippet dmac/snippet/main.c snippet_Cy_DMAC_Channel_GetCurrentSrcAddress
*
*******************************************************************************/
__STATIC_INLINE void * Cy_DMAC_Channel_GetCurrentSrcAddress(DMAC_Type const * base, uint32_t channel)
{
    CY_ASSERT_L1(CY_DMAC_IS_CH_NR_VALID(channel));

    return ((void *)(DMAC_CH_DESCR_SRC(base, channel)));
}


/*******************************************************************************
* Function Name: Cy_DMAC_Channel_GetCurrentDstAddress
****************************************************************************//**
*
* Returns the destination address being used for the current transfer.
*
* \param base
* The pointer to the hardware DMAC block.
*
* \param channel
* The channel number.
*
* \return
* Returns the pointer to the destination of transfer.
*
* \funcusage
* \snippet dmac/snippet/main.c snippet_Cy_DMAC_Channel_GetCurrentSrcAddress
*
*******************************************************************************/
__STATIC_INLINE void * Cy_DMAC_Channel_GetCurrentDstAddress(DMAC_Type const * base, uint32_t channel)
{
    CY_ASSERT_L1(CY_DMAC_IS_CH_NR_VALID(channel));

    return ((void *)(DMAC_CH_DESCR_DST(base, channel)));
}


/*******************************************************************************
* Function Name: Cy_DMAC_Channel_GetCurrentXloopIndex
****************************************************************************//**
*
* Returns the current transfer X-loop index.
*
* \param base
* The pointer to the hardware DMAC block.
*
* \param channel
* The channel number.
*
* \return
* Returns the current transfer X-loop index.
*
* \funcusage
* \snippet dmac/snippet/main.c snippet_Cy_DMAC_Channel_GetCurrentloopIndex
*
*******************************************************************************/
__STATIC_INLINE uint32_t Cy_DMAC_Channel_GetCurrentXloopIndex(DMAC_Type const * base, uint32_t channel)
{
    CY_ASSERT_L1(CY_DMAC_IS_CH_NR_VALID(channel));

    return (_FLD2VAL(DMAC_CH_V2_IDX_X, DMAC_CH_IDX(base, channel)));
}


/*******************************************************************************
* Function Name: Cy_DMAC_Channel_GetCurrentYloopIndex
****************************************************************************//**
*
* Returns the current transfer X-loop index.
*
* \param base
* The pointer to the hardware DMAC block.
*
* \param channel
* The channel number.
*
* \return
* Returns the current transfer X-loop index.
*
* \funcusage
* \snippet dmac/snippet/main.c snippet_Cy_DMAC_Channel_GetCurrentloopIndex
*
*******************************************************************************/
__STATIC_INLINE uint32_t Cy_DMAC_Channel_GetCurrentYloopIndex(DMAC_Type const * base, uint32_t channel)
{
    CY_ASSERT_L1(CY_DMAC_IS_CH_NR_VALID(channel));

    return (_FLD2VAL(DMAC_CH_V2_IDX_Y, DMAC_CH_IDX(base, channel)));
}


/*******************************************************************************
* Function Name: Cy_DMAC_Channel_GetCurrentDescriptor
****************************************************************************//**
*
* Returns the descriptor that is active in the channel.
*
* \param base
* The pointer to the hardware DMAC block.
*
* \param channel
* The channel number.
*
* \return
* The pointer to the descriptor associated with the channel.
*
* \funcusage
* \snippet dmac/snippet/main.c snippet_Cy_DMAC_Descriptor_Deinit
*
*******************************************************************************/
__STATIC_INLINE cy_stc_dmac_descriptor_t * Cy_DMAC_Channel_GetCurrentDescriptor(DMAC_Type const * base, uint32_t channel)
{
    CY_ASSERT_L1(CY_DMAC_IS_CH_NR_VALID(channel));

    return ((cy_stc_dmac_descriptor_t*)(DMAC_CH_CURR(base, channel)));
}


/*******************************************************************************
* Function Name: Cy_DMAC_Channel_GetInterruptStatus
****************************************************************************//**
*
* Returns the interrupt status of the specified channel.
*
* \param base
* The pointer to the hardware DMAC block.
*
* \param channel
* The channel number.
*
* \return
* The interrupt status, see \ref group_dmac_macros_interrupt_masks.
*
* \funcusage
* \snippet dmac/snippet/main.c snippet_Cy_DMAC_GetInterruptStatus
*
*******************************************************************************/
__STATIC_INLINE uint32_t Cy_DMAC_Channel_GetInterruptStatus(DMAC_Type const * base, uint32_t channel)
{
    CY_ASSERT_L1(CY_DMAC_IS_CH_NR_VALID(channel));

    return (DMAC_CH_INTR(base, channel));
}


/*******************************************************************************
* Function Name: Cy_DMAC_Channel_ClearInterrupt
****************************************************************************//**
*
* Clears the interrupt status of the specified channel.
*
* \param base
* The pointer to the hardware DMAC block.
*
* \param channel
* The channel number.
*
* \param interrupt
* The interrupt mask, see \ref group_dmac_macros_interrupt_masks.
*
* \funcusage
* \snippet dmac/snippet/main.c snippet_Cy_DMAC_ClearInterrupt
*
*******************************************************************************/
__STATIC_INLINE void Cy_DMAC_Channel_ClearInterrupt(DMAC_Type * base, uint32_t channel, uint32_t interrupt)
{
    CY_ASSERT_L1(CY_DMAC_IS_CH_NR_VALID(channel));
    CY_ASSERT_L2(CY_DMAC_IS_INTR_MASK_VALID(interrupt));

    DMAC_CH_INTR(base, channel) = interrupt;
    (void) DMAC_CH_INTR(base, channel);
}


/*******************************************************************************
* Function Name: Cy_DMAC_Channel_SetInterrupt
****************************************************************************//**
*
* Sets the interrupt for the specified channel.
*
* \param base
* The pointer to the hardware DMAC block.
*
* \param channel
* The channel number.
*
* \param interrupt
* The interrupt mask. See \ref group_dmac_macros_interrupt_masks.
*
* \funcusage
* \snippet dmac/snippet/main.c snippet_Cy_DMAC_SetInterruptMask
*
*******************************************************************************/
__STATIC_INLINE void Cy_DMAC_Channel_SetInterrupt(DMAC_Type * base, uint32_t channel, uint32_t interrupt)
{
    CY_ASSERT_L1(CY_DMAC_IS_CH_NR_VALID(channel));
    CY_ASSERT_L2(CY_DMAC_IS_INTR_MASK_VALID(interrupt));

    DMAC_CH_INTR_SET(base, channel) = interrupt;
}


/*******************************************************************************
* Function Name: Cy_DMAC_Channel_GetInterruptMask
****************************************************************************//**
*
* Returns the interrupt mask value of the specified channel.
*
* \param base
* The pointer to the hardware DMAC block.
*
* \param channel
* The channel number.
*
* \return
* The interrupt mask value. See \ref group_dmac_macros_interrupt_masks.
*
* \funcusage
* \snippet dmac/snippet/main.c snippet_Cy_DMAC_SetInterruptMask
*
*******************************************************************************/
__STATIC_INLINE uint32_t Cy_DMAC_Channel_GetInterruptMask(DMAC_Type const * base, uint32_t channel)
{
    CY_ASSERT_L1(CY_DMAC_IS_CH_NR_VALID(channel));

    return (DMAC_CH_INTR_MASK(base, channel));
}


/*******************************************************************************
* Function Name: Cy_DMAC_Channel_SetInterruptMask
****************************************************************************//**
*
* Sets an interrupt mask value for the specified channel.
*
* \param base
* The pointer to the hardware DMAC block.
*
* \param channel
* The channel number.
*
* \param interrupt
* The interrupt mask, see \ref group_dmac_macros_interrupt_masks.
*
* \funcusage
* \snippet dmac/snippet/main.c snippet_Cy_DMAC_SetInterruptMask
*
*******************************************************************************/
__STATIC_INLINE void Cy_DMAC_Channel_SetInterruptMask(DMAC_Type * base, uint32_t channel, uint32_t interrupt)
{
    CY_ASSERT_L1(CY_DMAC_IS_CH_NR_VALID(channel));
    CY_ASSERT_L2(CY_DMAC_IS_INTR_MASK_VALID(interrupt));
    DMAC_CH_INTR_MASK(base, channel) = interrupt;
}


/*******************************************************************************
* Function Name: Cy_DMAC_Channel_GetInterruptStatusMasked
****************************************************************************//**
*
* Returns the logical AND of the corresponding INTR and INTR_MASK fields
* in a single-load operation.
*
* \param base
* The pointer to the hardware DMAC block.
*
* \param channel
* The channel number.
*
* \return
* The masked interrupt status. See \ref group_dmac_macros_interrupt_masks.
*
* \funcusage
* \snippet dmac/snippet/main.c snippet_Cy_DMAC_ClearInterrupt
*
*******************************************************************************/
__STATIC_INLINE uint32_t Cy_DMAC_Channel_GetInterruptStatusMasked(DMAC_Type const * base, uint32_t channel)
{
    CY_ASSERT_L1(CY_DMAC_IS_CH_NR_VALID(channel));

    return (DMAC_CH_INTR_MASKED(base, channel));
}

/** \} group_dmac_channel_functions */

/** \} group_dmac_functions */

CY_MISRA_BLOCK_END('MISRA C-2012 Rule 10.8');

#if defined(__cplusplus)
}
#endif

#endif /* CY_IP_M4CPUSS_DMAC, CY_IP_MXAHBDMAC */

#endif  /* !defined (CY_DMAC_H) */

/** \} group_dmac */


/* [] END OF FILE */
