
/**
 * @file xmc_dma.h
 * @date 2016-10-28
 *
 * @cond
 *********************************************************************************************************************
 * XMClib v2.1.16 - XMC Peripheral Driver Library 
 *
 * Copyright (c) 2015-2017, Infineon Technologies AG
 * All rights reserved.                        
 *                                             
 * Redistribution and use in source and binary forms, with or without modification,are permitted provided that the 
 * following conditions are met:   
 *                                                                              
 * Redistributions of source code must retain the above copyright notice, this list of conditions and the following 
 * disclaimer.                        
 * 
 * Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following 
 * disclaimer in the documentation and/or other materials provided with the distribution.                       
 * 
 * Neither the name of the copyright holders nor the names of its contributors may be used to endorse or promote 
 * products derived from this software without specific prior written permission.                                           
 *                                                                              
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, 
 * INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE  
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE  FOR ANY DIRECT, INDIRECT, INCIDENTAL, 
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR  
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, 
 * WHETHER IN CONTRACT, STRICT LIABILITY,OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE 
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.                                                  
 *                                                                              
 * To improve the quality of the software, users are encouraged to share modifications, enhancements or bug fixes with 
 * Infineon Technologies AG dave@infineon.com).                                                          
 *********************************************************************************************************************
 *
 * Change History
 * --------------
 *
 * 2015-02-20:
 *     - Initial <br>
 *
 * 2015-06-20:
 *     - Add the declarations for the following APIs: <br>
 *       XMC_DMA_DisableRequestLine, XMC_DMA_ClearRequestLine, <br>
 *       XMC_DMA_CH_ClearSourcePeripheralRequest, <br>
 *       XMC_DMA_CH_ClearDestinationPeripheralRequest <br>
 *     - Documentation updates <br>
 *     - Removed version macros and declaration of GetDriverVersion API <br>
 *
 * 2016-10-28:
 *     - Fix documentation on maximum block size
 *
 * @endcond
 */

#ifndef XMC_DMA_H
#define XMC_DMA_H

/*******************************************************************************
 * HEADER FILES
 *******************************************************************************/

#include "xmc_common.h"

#if defined (GPDMA0)

#include "xmc_dma_map.h"

/**
 * @addtogroup XMClib XMC Peripheral Library
 * @{
 */

/**
 * @addtogroup DMA
 * @brief General purpose DMA (GPDMA) driver for the XMC4000 microcontroller family
 *
 * The GPDMA is a highly configurable DMA controller that allows high-speed data transfers
 * between peripherals and memories. Complex data transfers can be done with minimal
 * intervention of the processor, making CPU available for other operations.
 *
 * GPDMA provides extensive support for XMC microcontroller peripherals like A/D, D/A
 * converters and timers. Data transfers through communication interfaces (USIC) using the
 * GPDMA increase efficiency and parallelism for real-time applications.
 *
 * The DMA low level driver provides functions to configure and initialize the GPDMA
 * hardware peripheral.
 * @{
 */

/*******************************************************************************
 * MACROS
 *******************************************************************************/

#if defined (GPDMA0)
#define XMC_DMA0 ((XMC_DMA_t *)GPDMA0_CH0_BASE) /**< DMA module 0 */
#define XMC_DMA0_NUM_CHANNELS 8
#endif

#if defined (GPDMA1)
#define XMC_DMA1 ((XMC_DMA_t *)GPDMA1_CH0_BASE) /**< DMA module 1, only available in XMC45xx series */
#define XMC_DMA1_NUM_CHANNELS 4
#endif

/*******************************************************************************
 * ENUMS
 *******************************************************************************/

/**
 * DMA transfer types
 */
typedef enum XMC_DMA_CH_TRANSFER_TYPE
{
  XMC_DMA_CH_TRANSFER_TYPE_SINGLE_BLOCK,                                /**< Single block */
  XMC_DMA_CH_TRANSFER_TYPE_MULTI_BLOCK_SRCADR_CONTIGUOUS_DSTADR_RELOAD, /**< Multi-block: src address contiguous, dst address reload */
  XMC_DMA_CH_TRANSFER_TYPE_MULTI_BLOCK_SRCADR_RELOAD_DSTADR_CONTIGUOUS, /**< Multi-block: src address reload, dst address contiguous */
  XMC_DMA_CH_TRANSFER_TYPE_MULTI_BLOCK_SRCADR_RELOAD_DSTADR_RELOAD,     /**< Multi-block: src address reload, dst address reload */
  XMC_DMA_CH_TRANSFER_TYPE_MULTI_BLOCK_SRCADR_CONTIGUOUS_DSTADR_LINKED, /**< Multi-block: src address contiguous, dst address linked */
  XMC_DMA_CH_TRANSFER_TYPE_MULTI_BLOCK_SRCADR_RELOAD_DSTADR_LINKED,     /**< Multi-block: src address reload, dst address linked */
  XMC_DMA_CH_TRANSFER_TYPE_MULTI_BLOCK_SRCADR_LINKED_DSTADR_CONTIGUOUS, /**< Multi-block: src address linked, dst address contiguous */
  XMC_DMA_CH_TRANSFER_TYPE_MULTI_BLOCK_SRCADR_LINKED_DSTADR_RELOAD,     /**< Multi-block: src address linked, dst address reload */
  XMC_DMA_CH_TRANSFER_TYPE_MULTI_BLOCK_SRCADR_LINKED_DSTADR_LINKED      /**< Multi-block: src address linked, dst address linked */
} XMC_DMA_CH_TRANSFER_TYPE_t;

/**
 * DMA transfer flow modes
 */
typedef enum XMC_DMA_CH_TRANSFER_FLOW
{
  XMC_DMA_CH_TRANSFER_FLOW_M2M_DMA    = 0x0UL, /**< Memory to memory (DMA flow controller) */
  XMC_DMA_CH_TRANSFER_FLOW_M2P_DMA    = 0x1UL, /**< Memory to peripheral (DMA flow controller) */
  XMC_DMA_CH_TRANSFER_FLOW_P2M_DMA    = 0x2UL, /**< Peripheral to memory (DMA flow controller) */
  XMC_DMA_CH_TRANSFER_FLOW_P2P_DMA    = 0x3UL, /**< Peripheral to peripheral (DMA flow controller) */
  XMC_DMA_CH_TRANSFER_FLOW_P2M_PER    = 0x4UL, /**< Peripheral to memory (Peripheral flow controller) */
  XMC_DMA_CH_TRANSFER_FLOW_P2P_SRCPER = 0x5UL, /**< Peripheral to peripheral (Source peripheral flow controller) */
  XMC_DMA_CH_TRANSFER_FLOW_M2P_PER    = 0x6UL, /**< Memory to peripheral (Peripheral flow controller) */
  XMC_DMA_CH_TRANSFER_FLOW_P2P_DSTPER = 0x7UL  /**< Peripheral to peripheral (Destination peripheral flow controller) */
} XMC_DMA_CH_TRANSFER_FLOW_t;

/**
 * Valid burst length values
 */
typedef enum XMC_DMA_CH_BURST_LENGTH
{
  XMC_DMA_CH_BURST_LENGTH_1 = 0x0UL, /**< Burst length: 1 word */
  XMC_DMA_CH_BURST_LENGTH_4 = 0x1UL, /**< Burst length: 4 words */
  XMC_DMA_CH_BURST_LENGTH_8 = 0x2UL  /**< Burst length: 8 words */
} XMC_DMA_CH_BURST_LENGTH_t;

/**
 * Valid transfer width values
 */
typedef enum XMC_DMA_CH_TRANSFER_WIDTH
{
  XMC_DMA_CH_TRANSFER_WIDTH_8  = 0x0UL, /**< 8-bit transfer width */
  XMC_DMA_CH_TRANSFER_WIDTH_16 = 0x1UL, /**< 16-bit transfer width */
  XMC_DMA_CH_TRANSFER_WIDTH_32 = 0x2UL  /**< 32-bit transfer width */
} XMC_DMA_CH_TRANSFER_WIDTH_t;

/**
 * DMA address count mode
 */
typedef enum XMC_DMA_CH_ADDRESS_COUNT_MODE
{
  XMC_DMA_CH_ADDRESS_COUNT_MODE_INCREMENT = 0x0UL, /**< Address count mode: increment */
  XMC_DMA_CH_ADDRESS_COUNT_MODE_DECREMENT = 0x1UL, /**< Address count mode: decrement */
  XMC_DMA_CH_ADDRESS_COUNT_MODE_NO_CHANGE = 0x2UL  /**< Address count mode: no change */
} XMC_DMA_CH_ADDRESS_COUNT_MODE_t;

/**
 * DMA channel priorities
 */
typedef enum XMC_DMA_CH_PRIORITY
{
  XMC_DMA_CH_PRIORITY_0 = 0x0UL << GPDMA0_CH_CFGL_CH_PRIOR_Pos, /**< DMA channel priority 0 (low) */
  XMC_DMA_CH_PRIORITY_1 = 0x1UL << GPDMA0_CH_CFGL_CH_PRIOR_Pos, /**< DMA channel priority 1 */
  XMC_DMA_CH_PRIORITY_2 = 0x2UL << GPDMA0_CH_CFGL_CH_PRIOR_Pos, /**< DMA channel priority 2 */
  XMC_DMA_CH_PRIORITY_3 = 0x3UL << GPDMA0_CH_CFGL_CH_PRIOR_Pos, /**< DMA channel priority 3 */
  XMC_DMA_CH_PRIORITY_4 = 0x4UL << GPDMA0_CH_CFGL_CH_PRIOR_Pos, /**< DMA channel priority 4 */
  XMC_DMA_CH_PRIORITY_5 = 0x5UL << GPDMA0_CH_CFGL_CH_PRIOR_Pos, /**< DMA channel priority 5 */
  XMC_DMA_CH_PRIORITY_6 = 0x6UL << GPDMA0_CH_CFGL_CH_PRIOR_Pos, /**< DMA channel priority 6 */
  XMC_DMA_CH_PRIORITY_7 = 0x7UL << GPDMA0_CH_CFGL_CH_PRIOR_Pos  /**< DMA channel priority 7 (high) */
} XMC_DMA_CH_PRIORITY_t;

/**
 * Source handshake interface
 */
typedef enum XMC_DMA_CH_SRC_HANDSHAKING
{
  XMC_DMA_CH_SRC_HANDSHAKING_HARDWARE = 0x0UL << GPDMA0_CH_CFGL_HS_SEL_SRC_Pos, /**< Source: hardware handshake */
  XMC_DMA_CH_SRC_HANDSHAKING_SOFTWARE = 0x1UL << GPDMA0_CH_CFGL_HS_SEL_SRC_Pos  /**< Source: software handshake */
} XMC_DMA_CH_SRC_HANDSHAKING_t;

/**
 * Destination handshake interface
 */
typedef enum XMC_DMA_CH_DST_HANDSHAKING
{
  XMC_DMA_CH_DST_HANDSHAKING_HARDWARE = 0x0UL << GPDMA0_CH_CFGL_HS_SEL_DST_Pos, /**< Destination: hardware handshake */
  XMC_DMA_CH_DST_HANDSHAKING_SOFTWARE = 0x1UL << GPDMA0_CH_CFGL_HS_SEL_DST_Pos  /**< Destination: software handshake */
} XMC_DMA_CH_DST_HANDSHAKING_t;

/**
 * DMA hardware handshaking interface
 * Hardware handshaking available only if DMA is flow controller
 */
typedef enum XMC_DMA_CH_HARDWARE_HANDSHAKING_IF
{
  XMC_DMA_CH_HARDWARE_HANDSHAKING_IF_0 = 0x0UL, /**< Hardware handshaking interface 0 */
  XMC_DMA_CH_HARDWARE_HANDSHAKING_IF_1 = 0x1UL, /**< Hardware handshaking interface 1 */
  XMC_DMA_CH_HARDWARE_HANDSHAKING_IF_2 = 0x2UL, /**< Hardware handshaking interface 2 */
  XMC_DMA_CH_HARDWARE_HANDSHAKING_IF_3 = 0x3UL, /**< Hardware handshaking interface 3 */
  XMC_DMA_CH_HARDWARE_HANDSHAKING_IF_4 = 0x4UL, /**< Hardware handshaking interface 4 */
  XMC_DMA_CH_HARDWARE_HANDSHAKING_IF_5 = 0x5UL, /**< Hardware handshaking interface 5 */
  XMC_DMA_CH_HARDWARE_HANDSHAKING_IF_6 = 0x6UL, /**< Hardware handshaking interface 6 */
  XMC_DMA_CH_HARDWARE_HANDSHAKING_IF_7 = 0x7UL  /**< Hardware handshaking interface 7 */
} XMC_DMA_CH_HARDWARE_HANDSHAKING_IF_t;

/**
 * DMA events
 */
typedef enum XMC_DMA_CH_EVENT
{
  XMC_DMA_CH_EVENT_TRANSFER_COMPLETE        = 0x1UL, /**< Transfer complete event */
  XMC_DMA_CH_EVENT_BLOCK_TRANSFER_COMPLETE  = 0x2UL, /**< Block transfer complete event */
  XMC_DMA_CH_EVENT_SRC_TRANSACTION_COMPLETE = 0x4UL, /**< Source transaction complete event */
  XMC_DMA_CH_EVENT_DST_TRANSACTION_COMPLETE = 0x8UL, /**< Destination transaction complete event */
  XMC_DMA_CH_EVENT_ERROR                    = 0x10UL /**< DMA error event */
} XMC_DMA_CH_EVENT_t;

/**
 * Transaction types
 */
typedef enum XMC_DMA_CH_TRANSACTION_TYPE
{
  XMC_DMA_CH_TRANSACTION_TYPE_SINGLE, /**< Single DMA transaction */
  XMC_DMA_CH_TRANSACTION_TYPE_BURST   /**< Burst transaction */
} XMC_DMA_CH_TRANSACTION_TYPE_t;

/**
 * DMA channel status values
 */
typedef enum XMC_DMA_CH_STATUS
{
  XMC_DMA_CH_STATUS_OK,    /**< DMA status OK */
  XMC_DMA_CH_STATUS_ERROR, /**< DMA status error */
  XMC_DMA_CH_STATUS_BUSY   /**< DMA is busy */
} XMC_DMA_CH_STATUS_t;

/*******************************************************************************
 * DATA STRUCTURES
 *******************************************************************************/

/**
 * DMA channel configuration structure <br>
 *
 * The structure represents a collection of all hardware registers used
 * to configure the DMA channel. It is contained within the ::XMC_DMA_t
 * structure. It's registers can be used to configure DMA transfer details
 * like source address, destination address, block size, incrementation
 * modes and the like.
 */
 
typedef struct {
  __IO uint32_t  SAR;
  __I  uint32_t  RESERVED0;
  __IO uint32_t  DAR;
  __I  uint32_t  RESERVED1;
  __IO uint32_t  LLP;
  __I  uint32_t  RESERVED2;
  __IO uint32_t  CTLL;
  __IO uint32_t  CTLH;
  __IO uint32_t  SSTAT;
  __I  uint32_t  RESERVED3;
  __IO uint32_t  DSTAT;
  __I  uint32_t  RESERVED4;
  __IO uint32_t  SSTATAR;
  __I  uint32_t  RESERVED5;
  __IO uint32_t  DSTATAR;
  __I  uint32_t  RESERVED6;
  __IO uint32_t  CFGL;
  __IO uint32_t  CFGH;
  __IO uint32_t  SGR;
  __I  uint32_t  RESERVED7;
  __IO uint32_t  DSR;
  __I  uint32_t  RESERVED8;
} GPDMA_CH_t;

/**
 * DMA device structure <br>
 *
 * The structure represents a collection of all hardware registers used
 * to configure the GPDMA peripheral on the XMC4000 series of microcontrollers.
 * The registers can be accessed with ::XMC_DMA0 and ::XMC_DMA1.
 */
typedef struct {
  GPDMA_CH_t CH[8];

  __IO uint32_t  RAWCHEV[10];
  __I  uint32_t  STATUSCHEV[10];
  __IO uint32_t  MASKCHEV[10];
  __O  uint32_t  CLEARCHEV[10];
  __I  uint32_t  STATUSGLEV;
  __I  uint32_t  RESERVED20;
  __IO uint32_t  REQSRCREG;
  __I  uint32_t  RESERVED21;
  __IO uint32_t  REQDSTREG;
  __I  uint32_t  RESERVED22;
  __IO uint32_t  SGLREQSRCREG;
  __I  uint32_t  RESERVED23;
  __IO uint32_t  SGLREQDSTREG;
  __I  uint32_t  RESERVED24;
  __IO uint32_t  LSTSRCREG;
  __I  uint32_t  RESERVED25;
  __IO uint32_t  LSTDSTREG;
  __I  uint32_t  RESERVED26;
  __IO uint32_t  DMACFGREG;
  __I  uint32_t  RESERVED27;
  __IO uint32_t  CHENREG;
  __I  uint32_t  RESERVED28;
  __I  uint32_t  ID;
  __I  uint32_t  RESERVED29[19];
  __I  uint32_t  TYPE;
  __I  uint32_t  VERSION;
} XMC_DMA_t;

/* Anonymous structure/union guard start */
#if defined (__CC_ARM)
  #pragma push
  #pragma anon_unions
#elif defined (__TASKING__)
  #pragma warning 586
#endif

/**
 * DMA channel linked list item.
 * Note: Needs to be word aligned
 */
typedef struct XMC_DMA_LLI
{
  uint32_t src_addr;                            /**< Source address */
  uint32_t dst_addr;                            /**< Destination address */
  struct XMC_DMA_LLI *llp;                      /**< Linked list pointer of type XMC_DMA_LLI_t */
  union
  {
    struct
    {
      uint32_t enable_interrupt: 1;             /**< Enable interrupts? */
      uint32_t dst_transfer_width: 3;           /**< Destination transfer width */
      uint32_t src_transfer_width: 3;           /**< Source transfer width */
      uint32_t dst_address_count_mode: 2;       /**< Destination address count mode */
      uint32_t src_address_count_mode: 2;       /**< Source address count mode */
      uint32_t dst_burst_length: 3;             /**< Destination burst length */
      uint32_t src_burst_length: 3;             /**< Source burst length */
      uint32_t enable_src_gather: 1;            /**< Enable source gather? */
      uint32_t enable_dst_scatter: 1;           /**< Enable destination scatter? */
      uint32_t : 1;                             /**< Reserved bits */
      uint32_t transfer_flow: 3;                /**< DMA transfer flow */
      uint32_t : 4;                             /**< Reserved bits */
      uint32_t enable_dst_linked_list: 1;       /**< Enable destination linked list? */
      uint32_t enable_src_linked_list: 1;       /**< Enable source linked list? */
      uint32_t : 3;                             /**< Reserved bits */
    };
    uint32_t control;
  };
  uint32_t block_size;                          /**< Transfer block size */
  uint32_t src_status;                          /**< Source status */
  uint32_t dst_status;                          /**< Destination status */
} XMC_DMA_LLI_t;

typedef XMC_DMA_LLI_t **XMC_DMA_LIST_t;         /**< Type definition for a linked list pointer */

/**
 * DMA channel configuration structure
 */
typedef struct XMC_DMA_CH_CONFIG
{
  union
  {
    uint32_t control;
    struct
    {
      uint32_t enable_interrupt: 1;             /**< Enable interrupts? */
      uint32_t dst_transfer_width: 3;           /**< Destination transfer width */
      uint32_t src_transfer_width: 3;           /**< Source transfer width */
      uint32_t dst_address_count_mode: 2;       /**< Destination address count mode */
      uint32_t src_address_count_mode: 2;       /**< Source address count mode */
      uint32_t dst_burst_length: 3;             /**< Destination burst length */
      uint32_t src_burst_length: 3;             /**< Source burst length */
      uint32_t enable_src_gather: 1;            /**< Enable source gather? */
      uint32_t enable_dst_scatter: 1;           /**< Enable destination scatter? */
      uint32_t : 1;                             
      uint32_t transfer_flow: 3;                /**< DMA transfer flow */
      uint32_t : 9;                             
    };
  };

  uint32_t src_addr;                            /**< Source address */
  uint32_t dst_addr;                            /**< Destination address */
  XMC_DMA_LLI_t *linked_list_pointer;           /**< Linked list pointer */
  
  union
  {
    uint32_t src_gather_control;
    struct
    {
      uint32_t src_gather_interval: 20;         /**< Source gather interval */
      uint32_t src_gather_count: 12;            /**< Source gather count */
    };
  };

  union
  {
    uint32_t dst_scatter_control;
    struct
    {
      uint32_t dst_scatter_interval: 20;        /**< Destination scatter interval */
      uint32_t dst_scatter_count: 12;           /**< Destination scatter count */
    };
  };
  
  uint16_t block_size;                          /**< Block size for DMA controlled transfers [max. 4095]*/
  XMC_DMA_CH_TRANSFER_TYPE_t transfer_type;     /**< DMA transfer type */
  XMC_DMA_CH_PRIORITY_t priority;               /**< DMA channel priority */
  XMC_DMA_CH_SRC_HANDSHAKING_t src_handshaking; /**< DMA source handshaking interface */
  uint8_t src_peripheral_request;               /**< Source peripheral request */
  XMC_DMA_CH_DST_HANDSHAKING_t dst_handshaking; /**< DMA destination handshaking interface */
  uint8_t dst_peripheral_request;               /**< Destination peripheral request */
} XMC_DMA_CH_CONFIG_t;

/* Anonymous structure/union guard end */
#if defined (__CC_ARM)
  #pragma pop
#elif defined (__TASKING__)
  #pragma warning restore
#endif

/**
 * DMA channel event handler
 */
typedef void (*XMC_DMA_CH_EVENT_HANDLER_t)(XMC_DMA_CH_EVENT_t event);

/*******************************************************************************
 * API PROTOTYPES
 *******************************************************************************/

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @param dma A constant pointer to XMC_DMA_t, pointing to the GPDMA base address
 * @return None
 *
 * \par<b>Description: </b><br>
 * Initialize the GPDMA peripheral <br>
 *
 * \par
 * The function initializes a prioritized list of DMA channels and enables the GPDMA
 * peripheral.
 */
void XMC_DMA_Init(XMC_DMA_t *const dma);

/**
 * @param dma A constant pointer to XMC_DMA_t, pointing to the GPDMA base address
 * @return None
 *
 * \par<b>Description: </b><br>
 * Enable the GPDMA peripheral <br>
 *
 * \par
 * The function de-asserts the GPDMA peripheral reset. In addition, it un-gates the
 * GPDMA0 peripheral clock for all XMC4000 series of microcontrollers with an exception
 * of the XMC4500 microcontroller. The XMC4500 doesn't support gating.
 */
void XMC_DMA_Enable(XMC_DMA_t *const dma);

/**
 * @param dma A constant pointer to XMC_DMA_t, pointing to the GPDMA base address
 * @return None
 *
 * \par<b>Description: </b><br>
 * Disable the GPDMA peripheral <br>
 *
 * \par
 * The function asserts the GPDMA peripheral reset. In addition, it gates the GPDMA0
 * peripheral clock for all XMC4000 series of microcontrollers with an exception of
 * the XMC4500 microcontroller. The XMC4500 doesn't support gating.
 */
void XMC_DMA_Disable(XMC_DMA_t *const dma);

/**
 * @param dma A constant pointer to XMC_DMA_t, pointing to the GPDMA base address
 * @return bool
 *
 * \par<b>Description: </b><br>
 * Check if the GPDMA peripheral is enabled <br>
 *
 * \par
 * For the XMC4500 microcontroller, the function checks if the GPDMA module is asserted
 * and returns "false" if it is. In addition, it also checks if the clock is gated
 * for the other XMC4000 series of microcontrollers. It returns "true" if the peripheral
 * is enabled.
 */
bool XMC_DMA_IsEnabled(const XMC_DMA_t *const dma);

/**
 * @param dma A constant pointer to XMC_DMA_t, pointing to the GPDMA base address
 * @return uint32_t DMA event status
 *
 * \par<b>Description: </b><br>
 * Get DMA event status <br>
 *
 * \par
 * The function returns the collective (global) status of GPDMA events. The following
 * lists the various DMA events and their corresponding enumeration. The return value
 * of this function may then be masked with any one of the following enumerations to
 * obtain the status of individual DMA events. <br>
 *
 * \par
 * Transfer complete -> ::XMC_DMA_CH_EVENT_TRANSFER_COMPLETE                       <br>
 * Block transfer complete -> ::XMC_DMA_CH_EVENT_BLOCK_TRANSFER_COMPLETE           <br>
 * Source transaction complete -> ::XMC_DMA_CH_EVENT_SRC_TRANSACTION_COMPLETE      <br>
 * Destination transaction complete -> ::XMC_DMA_CH_EVENT_DST_TRANSACTION_COMPLETE <br>
 * DMA error event -> ::XMC_DMA_CH_EVENT_ERROR                                     <br>
 */
__STATIC_INLINE uint32_t XMC_DMA_GetEventStatus(XMC_DMA_t *const dma)
{
  return (dma->STATUSGLEV);
}

/**
 * @param dma A constant pointer to XMC_DMA_t, pointing to the GPDMA base address
 * @return uint32_t DMA transfer complete status
 *
 * \par<b>Description: </b><br>
 * Get transfer complete status <br>
 *
 * \par
 * The function returns GPDMA transfer complete interrupt status. <br>
 */
__STATIC_INLINE uint32_t XMC_DMA_GetChannelsTransferCompleteStatus(XMC_DMA_t *const dma)
{
  return (dma->STATUSCHEV[0]);
}

/**
 * @param dma A constant pointer to XMC_DMA_t, pointing to the GPDMA base address
 * @return uint32_t DMA block complete status
 *
 * \par<b>Description: </b><br>
 * Get block transfer complete status <br>
 *
 * \par
 * The function returns GPDMA block transfer complete interrupt status. <br>
 */
__STATIC_INLINE uint32_t XMC_DMA_GetChannelsBlockCompleteStatus(XMC_DMA_t *const dma)
{
  return (dma->STATUSCHEV[2]);
}

/**
 * @param dma A constant pointer to XMC_DMA_t, pointing to the GPDMA base address
 * @return uint32_t DMA event status
 *
 * \par<b>Description: </b><br>
 * Get source transaction complete status <br>
 *
 * \par
 * The function returns the source transaction complete interrupt status. <br>
 *
 * \par<b>Note: </b><br>
 * If the source peripheral is memory, the source transaction complete interrupt is
 * ignored.
 */
__STATIC_INLINE uint32_t XMC_DMA_GetChannelsSourceTransactionCompleteStatus(XMC_DMA_t *const dma)
{
  return (dma->STATUSCHEV[4]);
}

/**
 * @param dma A constant pointer to XMC_DMA_t, pointing to the GPDMA base address
 * @return uint32_t DMA event status
 *
 * \par<b>Description: </b><br>
 * Get destination transaction complete status <br>
 *
 * \par
 * The function returns the destination transaction complete interrupt status <br>
 *
 * \par<b>Note: </b><br>
 * If the destination peripheral is memory, the destination transaction complete
 * interrupt is ignored.
 */
__STATIC_INLINE uint32_t XMC_DMA_GetChannelsDestinationTransactionCompleteStatus(XMC_DMA_t *const dma)
{
  return (dma->STATUSCHEV[6]);
}

/**
 * @param dma A constant pointer to XMC_DMA_t, pointing to the GPDMA base address
 * @return uint32_t DMA error event status
 *
 * \par<b>Description: </b><br>
 * Get DMA error event status <br>
 *
 * \par
 * The function returns error interrupt status. <br>
 */
__STATIC_INLINE uint32_t XMC_DMA_GetChannelsErrorStatus(XMC_DMA_t *const dma)
{
  return (dma->STATUSCHEV[8]);
}

/**
 * @param dma A constant pointer to XMC_DMA_t, pointing to the GPDMA base address
 * @param line Which DLR (DMA line router) line should the function use?
 * @param peripheral Which hardware peripheral is the GPDMA communicating with?
 * @return None
 *
 * \par<b>Description: </b><br>
 * Enable request line <br>
 *
 * \par
 * The function enables a DLR (DMA line router) line and selects a service request
 * source, resulting in the trigger of a DMA transfer. <br>
 *
 * \par<b>Note: </b><br>
 * The DLR is used for a DMA transfer typically involving a peripheral; For example,
 * the ADC peripheral may use the DLR in hardware handshaking mode to transfer
 * ADC conversion values to a destination memory block.
 */
void XMC_DMA_EnableRequestLine(XMC_DMA_t *const dma, uint8_t line, uint8_t peripheral);

/**
 * @param dma A constant pointer to XMC_DMA_t, pointing to the GPDMA base address
 * @param line Which DLR (DMA line router) line should the function use?
 * @return None
 *
 * \par<b>Description: </b><br>
 * Disable request line <br>
 *
 * \par
 * The function disables a DLR (DMA line router) line by clearing appropriate bits
 * in the LNEN register. <br>
 */
void XMC_DMA_DisableRequestLine(XMC_DMA_t *const dma, uint8_t line);

/**
 * @param dma A constant pointer to XMC_DMA_t, pointing to the GPDMA base address
 * @param line Which DLR (DMA line router) line should the function use?
 * @return None
 *
 * \par<b>Description: </b><br>
 * Clear request line <br>
 *
 * \par
 * The function clears a DLR (DMA line router) request line. <br>
 */
void XMC_DMA_ClearRequestLine(XMC_DMA_t *const dma, uint8_t line);

/**
 * @param dma A constant pointer to XMC_DMA_t, pointing to the GPDMA base address
 * @param line The line for which the overrun status is requested
 * @return bool "true" if overrun occured, "false" otherwise
 *
 * \par<b>Description: </b><br>
 * Get overrun status of a DLR line <br>
 *
 * \par
 * The DLR module's OVERSTAT register keeps track of DMA service request overruns.
 * Should an overrun occur, the bit corresponding to the used DLR line is set. The
 * function simply reads this status and returns "true" if an overrun is detected
 * It returns "false" if an overrun isn't registered.
 */
bool XMC_DMA_GetOverrunStatus(XMC_DMA_t *const dma, const uint8_t line);

/**
 * @param dma A constant pointer to XMC_DMA_t, pointing to the GPDMA base address
 * @param line The line for which the overrun status must be cleared
 * @return None
 *
 * \par<b>Description: </b><br>
 * Clear overrun status of a DLR line <br>
 *
 * \par
 * The function clears the overrun status of a line by setting the corresponding
 * line bit in the DLR's OVERCLR register.
 */
void XMC_DMA_ClearOverrunStatus(XMC_DMA_t *const dma, const uint8_t line);

/**
 * @param dma A constant pointer to XMC_DMA_t, pointing to the GPDMA base address
 * @param channel The GPDMA channel (number) which needs to be initialized
 * @param config A constant pointer to ::XMC_DMA_CH_CONFIG_t, pointing to a const
 *               channel configuration
 * @return XMC_DMA_CH_STATUS_t Initialization status
 *
 * \par<b>Description: </b><br>
 * Initialize a GPDMA channel with provided channel configuration <br>
 *
 * \par
 * The function sets up the following channel configuration parameters for a GPDMA
 * channel (specified by the parameter channel): <br>
 * 1) Source and destination addresses (and linked list address if requested) <br>
 * 2) Source and destination handshaking interface (hardware or software?)    <br>
 * 3) Scatter/gather configuration                                            <br>
 * 4) Source and destination peripheral request (DMA is the flow controller)  <br>
 * 5) Transfer flow and type                                                  <br>
 *
 * \par
 * The function returns one of the following values: <br>
 * 1) In case the DMA channel is not enabled: ::XMC_DMA_CH_STATUS_BUSY        <br>
 * 2) If the GPDMA module itself is not enabled: ::XMC_DMA_CH_STATUS_ERROR    <br>
 * 3) If the configuration was successful: ::XMC_DMA_CH_STATUS_OK             <br>
 *
 * \par
 * Once the initialization is successful, calling ::XMC_DMA_CH_Enable() will trigger
 * a GPDMA transfer.
 */
XMC_DMA_CH_STATUS_t XMC_DMA_CH_Init(XMC_DMA_t *const dma, const uint8_t channel, const XMC_DMA_CH_CONFIG_t *const config);

/**
 * @param dma A constant pointer to XMC_DMA_t, pointing to the GPDMA base address
 * @param channel Which DMA channel should be enabled?
 * @return None
 *
 * \par<b>Description: </b><br>
 * Enable a GPDMA channel <br>
 *
 * \par
 * The function sets the GPDMA's CHENREG register to enable a DMA channel. Please
 * ensure that the GPDMA module itself is enabled before calling this function.
 * See ::XMC_DMA_Enable() for details.
 */
__STATIC_INLINE void XMC_DMA_CH_Enable(XMC_DMA_t *const dma, const uint8_t channel)
{
  dma->CHENREG = (uint32_t)(0x101UL << channel);   
}

/**
 * @param dma A constant pointer to XMC_DMA_t, pointing to the GPDMA base address
 * @param channel Which DMA channel should be disabled?
 * @return None
 *
 * \par<b>Description: </b><br>
 * Disable a GPDMA channel <br>
 *
 * \par
 * The function resets the GPDMA's CHENREG register to disable a DMA channel.
 */
void XMC_DMA_CH_Disable(XMC_DMA_t *const dma, const uint8_t channel);

/**
 * @param dma A constant pointer to XMC_DMA_t, pointing to the GPDMA base address
 * @param channel Which DMA channel should be disabled?
 * @return bool
 *
 * \par<b>Description: </b><br>
 * Check if a GPDMA channel is enabled <br>
 *
 * \par
 * The function reads the GPDMA's CHENREG register to check if a DMA channel is
 * enabled or not. The function returns "true" is the requested channel is enabled,
 * "false" otherwise.
 */
bool XMC_DMA_CH_IsEnabled(XMC_DMA_t *const dma, const uint8_t channel);

/**
 * @param dma A constant pointer to XMC_DMA_t, pointing to the GPDMA base address
 * @param channel Which DMA channel should suspend transfer?
 * @return None
 *
 * \par<b>Description: </b><br>
 * Suspend a GPDMA channel transfer <br>
 *
 * \par
 * The function sets the CH_SUSP bit of the GPDMA's GFGL register to initiate a
 * DMA transfer suspend. The function may be called after enabling the DMA channel.
 * Please see ::XMC_DMA_CH_Enable() for more information.
 *
 * \par<b>Related API: </b><br>
 * ::XMC_DMA_CH_Resume() <br>
 */
void XMC_DMA_CH_Suspend(XMC_DMA_t *const dma, const uint8_t channel);

/**
 * @param dma A constant pointer to XMC_DMA_t, pointing to the GPDMA base address
 * @param channel Which DMA channel should resume transfer?
 * @return None
 *
 * \par<b>Description: </b><br>
 * Resume a GPDMA channel <br>
 *
 * \par
 * The function clears the CH_SUSP bit of the GPDMA's GFGL register to resume a
 * DMA transfer. The function may be called after enabling the DMA channel. Please
 * see ::XMC_DMA_CH_Enable() for more information.
 *
 * \par<b>Related API: </b><br>
 * ::XMC_DMA_CH_Suspend() <br>
 */
void XMC_DMA_CH_Resume(XMC_DMA_t *const dma, const uint8_t channel);

/**
 * @param dma A constant pointer to XMC_DMA_t, pointing to the GPDMA base address
 * @param channel A DMA channel
 * @param addr source address
 * @return None
 *
 * \par<b>Description: </b><br>
 * This function sets the source address of the specified channel <br>
 *
 * \par
 * The function may be called after enabling the DMA channel. Please
 * see ::XMC_DMA_CH_Enable() for more information.
 *
 * \par<b>Related API: </b><br>
 * ::XMC_DMA_CH_SetDestinationAddress() <br>
 */
__STATIC_INLINE void XMC_DMA_CH_SetSourceAddress(XMC_DMA_t *const dma, const uint8_t channel, uint32_t addr)
{
  dma->CH[channel].SAR = addr;
}

/**
 * @param dma A constant pointer to XMC_DMA_t, pointing to the GPDMA base address
 * @param channel A DMA channel
 * @param addr destination address
 * @return None
 *
 * \par<b>Description: </b><br>
 * This function sets the destination address of the specified channel <br>
 *
 * \par
 * The function may be called after enabling the DMA channel. Please
 * see ::XMC_DMA_CH_Enable() for more information.
 *
 * \par<b>Related API: </b><br>
 * ::XMC_DMA_CH_SetSourceAddress() <br>
 */
__STATIC_INLINE void XMC_DMA_CH_SetDestinationAddress(XMC_DMA_t *const dma, const uint8_t channel, uint32_t addr)
{
  dma->CH[channel].DAR = addr;
}

/**
 * @param dma A constant pointer to XMC_DMA_t, pointing to the GPDMA base address
 * @param channel A DMA channel
 * @param block_size Transfer size [1-2048]
 * @return None
 *
 * \par<b>Description: </b><br>
 * This function sets the block size of a transfer<br>
 *
 * \par
 * The function may be called after enabling the DMA channel. Please
 * see ::XMC_DMA_CH_Enable() for more information.
 *
 */
__STATIC_INLINE void XMC_DMA_CH_SetBlockSize(XMC_DMA_t *const dma, const uint8_t channel, uint32_t block_size)
{
  dma->CH[channel].CTLH = block_size;
}

/**
 * @param dma A constant pointer to XMC_DMA_t, pointing to the GPDMA base address
 * @param channel A DMA channel
 * @param ll_ptr linked list pointer
 * @return None
 *
 * \par<b>Description: </b><br>
 * This function sets the linked list pointer<br>
 *
 * \par
 * The function may be called after enabling the DMA channel. Please
 * see ::XMC_DMA_CH_Enable() for more information.
 *
 */
__STATIC_INLINE void XMC_DMA_CH_SetLinkedListPointer(XMC_DMA_t *const dma, const uint8_t channel, XMC_DMA_LLI_t *ll_ptr)
{
  dma->CH[channel].LLP = (uint32_t)ll_ptr;
}

/**
 * @param dma A constant pointer to XMC_DMA_t, pointing to the GPDMA base address
 * @param channel Which DMA channel should be checked for a suspended transfer?
 * @return bool
 *
 * \par<b>Description: </b><br>
 * Check if a GPDMA  <br>
 *
 * \par
 * The function reads the CH_SUSP bit of the GPDMA's GFGL register to check if a
 * DMA transfer for the requested channel has been suspended. The function returns
 * "true" if it detects a transfer suspension or "false" if it doesn't.
 *
 * \par<b>Related API: </b><br>
 * ::XMC_DMA_CH_Suspend(), ::XMC_DMA_CH_Resume() <br>
 */
bool XMC_DMA_CH_IsSuspended(XMC_DMA_t *const dma, const uint8_t channel);

/**
 * @param dma A constant pointer to XMC_DMA_t, pointing to the GPDMA base address
 * @param channel The channel for which the event(s) need(s) to be enabled
 * @param event A valid GPDMA event (::XMC_DMA_CH_EVENT_t) or a valid combination
 *              of logically OR'd GPDMA events
 * @return None
 *
 * \par<b>Description: </b><br>
 * Enable GPDMA event(s) <br>
 *
 * \par
 * The following events are supported by the GPDMA peripheral: <br>
 * 1) Transfer complete event                <br>
 * 2) Block transfer complete event          <br>
 * 3) Source transaction complete event      <br>
 * 4) Destination transaction complete event <br>
 * 5) DMA error event                        <br>
 *
 * \par
 * The function can be used to enable one (or more) of the aforementioned events.
 * Once the events have been enabled, ::XMC_DMA_CH_SetEventHandler() API can be
 * used to set a callback function.
 */
void XMC_DMA_CH_EnableEvent(XMC_DMA_t *const dma, const uint8_t channel, const uint32_t event);

/**
 * @param dma A constant pointer to XMC_DMA_t, pointing to the GPDMA base address
 * @param channel The channel for which the event(s) need(s) to be disabled
 * @param event A valid GPDMA event (::XMC_DMA_CH_EVENT_t) or a valid combination
 *              of logically OR'd GPDMA events
 * @return None
 *
 * \par<b>Description: </b><br>
 * Disable GPDMA event(s) <br>
 *
 * \par
 * The following events are supported by the GPDMA peripheral: <br>
 * 1) Transfer complete event                <br>
 * 2) Block transfer complete event          <br>
 * 3) Source transaction complete event      <br>
 * 4) Destination transaction complete event <br>
 * 5) DMA error event                        <br>
 *
 * \par
 * The function can be used to disable one (or more) of the aforementioned events.
 */
void XMC_DMA_CH_DisableEvent(XMC_DMA_t *const dma, const uint8_t channel, const uint32_t event);

/**
 * @param dma A constant pointer to XMC_DMA_t, pointing to the GPDMA base address
 * @param channel The channel for which the event(s) need(s) to be disabled
 * @param event A valid GPDMA event (::XMC_DMA_CH_EVENT_t) or a valid combination
 *              of logically OR'd GPDMA events
 * @return None
 *
 * \par<b>Description: </b><br>
 * Clear GPDMA event status <br>
 *
 * \par
 * The following events are supported by the GPDMA peripheral: <br>
 * 1) Transfer complete event                <br>
 * 2) Block transfer complete event          <br>
 * 3) Source transaction complete event      <br>
 * 4) Destination transaction complete event <br>
 * 5) DMA error event                        <br>
 *
 * \par
 * The function is used to clear the status of one (or more) of the aforementioned
 * events. Typically, its use is in the GPDMA interrupt handler function. Once an
 * event is detected, an appropriate callback function must run and the event status
 * should be cleared.
 */
void XMC_DMA_CH_ClearEventStatus(XMC_DMA_t *const dma, const uint8_t channel, const uint32_t event);

/**
 * @param dma A constant pointer to XMC_DMA_t, pointing to the GPDMA base address
 * @param channel The channel for which the event(s) status must be obtained
 * @return Event status
 *
 * \par<b>Description: </b><br>
 * Get GPDMA channel event status <br>
 *
 * \par
 * The function is used obtain the status of one (or more) of the aforementioned
 * events. The return value may then be masked with any one of the following
 * enumerations to obtain the status of individual DMA events. <br>
 *
 * \par
 * Transfer complete -> ::XMC_DMA_CH_EVENT_TRANSFER_COMPLETE                       <br>
 * Block transfer complete -> ::XMC_DMA_CH_EVENT_BLOCK_TRANSFER_COMPLETE           <br>
 * Source transaction complete -> ::XMC_DMA_CH_EVENT_SRC_TRANSACTION_COMPLETE      <br>
 * Destination transaction complete -> ::XMC_DMA_CH_EVENT_DST_TRANSACTION_COMPLETE <br>
 * DMA error event -> ::XMC_DMA_CH_EVENT_ERROR                                     <br>
 *
 * \par
 * Typically, its use is in the GPDMA interrupt handler function. Once an event is
 * detected, an appropriate callback function must run and the event status should
 * be cleared.
 */
uint32_t XMC_DMA_CH_GetEventStatus(XMC_DMA_t *const dma, const uint8_t channel);

/**
 * @param dma A constant pointer to XMC_DMA_t, pointing to the GPDMA base address
 * @param channel Which DMA channel is used for source gather?
 * @param interval Gather interval
 * @param count Gather count
 * @return None
 *
 * \par<b>Description: </b><br>
 * Enable source gather <br>
 *
 * \par
 * The function is used to enable the source gather feature in the GPDMA peripheral.
 * The user must also specify the gather count and interval. Once the configuration
 * is successful, calling ::XMC_DMA_CH_EnableEvent() will initiate source gather.
 * This function is normally used in conjunction with destination scatter. Please
 * see ::XMC_DMA_CH_EnableDestinationScatter() for additional information.
 */
void XMC_DMA_CH_EnableSourceGather(XMC_DMA_t *const dma, const uint8_t channel, uint32_t interval, uint16_t count);

/**
 * @param dma A constant pointer to XMC_DMA_t, pointing to the GPDMA base address
 * @param channel The source gather for which DMA channel is to be disabled?
 * @return None
 *
 * \par<b>Description: </b><br>
 * Disable source gather <br>
 *
 * \par
 * The function is used to disable the source gather feature in the GPDMA peripheral.
 */
void XMC_DMA_CH_DisableSourceGather(XMC_DMA_t *const dma, const uint8_t channel);

/**
 * @param dma A constant pointer to XMC_DMA_t, pointing to the GPDMA base address
 * @param channel Which DMA channel is used for destination scatter?
 * @param interval Scatter interval
 * @param count Scatter count
 * @return None
 *
 * \par<b>Description: </b><br>
 * Enable destination scatter <br>
 *
 * \par
 * The function is used to enable the destination scatter feature in the GPDMA
 * peripheral. The user must also specify the scatter count and interval. Once
 * the configuration is successful, calling ::XMC_DMA_CH_EnableEvent() will
 * initiate destination gather. This function is normally used in conjunction
 * with source gather. Please see ::XMC_DMA_CH_EnableSourceGather() for
 * additional information.
 */
void XMC_DMA_CH_EnableDestinationScatter(XMC_DMA_t *const dma, const uint8_t channel, uint32_t interval, uint16_t count);

/**
 * @param dma A constant pointer to XMC_DMA_t, pointing to the GPDMA base address
 * @param channel The source gather for which DMA channel is to be disabled?
 * @return None
 *
 * \par<b>Description: </b><br>
 * Disable source gather <br>
 *
 * \par
 * The function is used to disable the destination scatter feature in the GPDMA
 * peripheral.
 */
void XMC_DMA_CH_DisableDestinationScatter(XMC_DMA_t *const dma, const uint8_t channel);

/**
 * @param dma A constant pointer to XMC_DMA_t, pointing to the GPDMA base address
 * @param channel Which DMA channel is being used?
 * @param type Transaction type: Single/burst mode
 * @param last Specify "true" if it is the last source request trigger, "false"
 *             otherwise
 * @return None
 *
 * \par<b>Description: </b><br>
 * Trigger source request <br>
 *
 * \par
 * The function can be used for GPDMA transfers involving a peripheral in software
 * handshaking mode viz. Memory -> peripheral and peripheral -> peripheral.
 *
 * \par
 * One would typically use this function in a (destination) peripheral's event
 * callback function to trigger the source request.
 */
void XMC_DMA_CH_TriggerSourceRequest(XMC_DMA_t *const dma, const uint8_t channel, const XMC_DMA_CH_TRANSACTION_TYPE_t type, bool last);

/**
 * @param dma A constant pointer to XMC_DMA_t, pointing to the GPDMA base address
 * @param channel Which DMA channel is being used?
 * @param type Transaction type: Single/burst mode
 * @param last Specify "true" if it is the last destination request trigger, "false"
 *             otherwise
 * @return None
 *
 * \par<b>Description: </b><br>
 * Trigger destination request <br>
 *
 * \par
 * The function can be used for GPDMA transfers involving a peripheral in software
 * handshaking mode viz. Peripheral -> memory and peripheral -> peripheral.
 *
 * \par
 * One would typically use this function in a (source) peripheral's event
 * callback function to trigger the destination request.
 */
void XMC_DMA_CH_TriggerDestinationRequest(XMC_DMA_t *const dma, const uint8_t channel, const XMC_DMA_CH_TRANSACTION_TYPE_t type, bool last);

/**
 * @param dma A constant pointer to XMC_DMA_t, pointing to the GPDMA base address
 * @param channel The channel for which the source address must be reloaded
 * @return None
 *
 * \par<b>Description: </b><br>
 * Enable source address reload <br>
 *
 * \par
 * The function is used to automatically reload the source DMA address (from its
 * initial value) at the end of every block in a multi-block transfer. The auto-
 * reload will begin soon after the DMA channel initialization (configured for a
 * multi-block transaction).
 */
void XMC_DMA_CH_EnableSourceAddressReload(XMC_DMA_t *const dma, const uint8_t channel);

/**
 * @param dma A constant pointer to XMC_DMA_t, pointing to the GPDMA base address
 * @param channel The channel for which the source address reload must be disabled
 * @return None
 *
 * \par<b>Description: </b><br>
 * Disable source address reload <br>
 *
 * \par
 * The source DMA address can be automatically reloaded from its initial value at
 * the end of every block in a multi-block transfer. To disable this feature, use
 * this function.
 */
void XMC_DMA_CH_DisableSourceAddressReload(XMC_DMA_t *const dma, const uint8_t channel);

/**
 * @param dma A constant pointer to XMC_DMA_t, pointing to the GPDMA base address
 * @param channel The channel for which the destination address must be reloaded
 * @return None
 *
 * \par<b>Description: </b><br>
 * Enable source address reload <br>
 *
 * \par
 * The function is used to automatically reload the destination DMA address (from
 * its initial value) at the end of every block in a multi-block transfer. The auto-
 * reload will begin soon after the DMA channel initialization (configured for a
 * multi-block transaction).
 */
void XMC_DMA_CH_EnableDestinationAddressReload(XMC_DMA_t *const dma, const uint8_t channel);

/**
 * @param dma A constant pointer to XMC_DMA_t, pointing to the GPDMA base address
 * @param channel The channel for which the destination address reload must be
 *                disabled
 * @return None
 *
 * \par<b>Description: </b><br>
 * Disable destination address reload <br>
 *
 * \par
 * The destination DMA address can be automatically reloaded from its initial value
 * at the end of every block in a multi-block transfer. To disable this feature,
 * use this function.
 */
void XMC_DMA_CH_DisableDestinationAddressReload(XMC_DMA_t *const dma, const uint8_t channel);

/**
 * @param dma A constant pointer to XMC_DMA_t, pointing to the GPDMA base address
 * @param channel Which DMA channel is participating in a multi-block transfer?
 * @return None
 *
 * \par<b>Description: </b><br>
 * Trigger the end of a multi-block transfer <br>
 *
 * \par
 * The function is used signal the end of multi-block DMA transfer. It clears the
 * RELOAD_SRC and RELOAD_DST bits of the CFGL register to keep the source and
 * destination addresses from getting updated. The function is typically used in
 * an event handler to signal that the next block getting transferred is the last
 * block in the transfer sequence.
 */
void XMC_DMA_CH_RequestLastMultiblockTransfer(XMC_DMA_t *const dma, const uint8_t channel);

/**
 * @param dma A constant pointer to XMC_DMA_t, pointing to the GPDMA base address
 * @param channel The channel for which the event handler is being registered
 * @param event_handler The event handler which will be invoked when the DMA event
 *                      occurs
 * @return None
 *
 * \par<b>Description: </b><br>
 * Set a GPDMA event handler to service GPDMA events <br>
 *
 * \par
 * The function is used to register user callback functions for servicing DMA events.
 * Call this function after enabling the GPDMA events (See ::XMC_DMA_CH_EnableEvent())
 */
void XMC_DMA_CH_SetEventHandler(XMC_DMA_t *const dma, const uint8_t channel, XMC_DMA_CH_EVENT_HANDLER_t event_handler);

/**
 * @param dma A constant pointer to XMC_DMA_t, pointing to the GPDMA base address
 * @param channel The source peripheral request for which DMA channel is to be cleared?
 * @return None
 *
 * \par<b>Description: </b><br>
 * Clear source peripheral request <br>
 *
 * \par
 * The function is used to clear the source peripheral request for a given DMA
 * channel.
 */
void XMC_DMA_CH_ClearSourcePeripheralRequest(XMC_DMA_t *const dma, uint8_t channel);

/**
 * @param dma A constant pointer to XMC_DMA_t, pointing to the GPDMA base address
 * @param channel The destination peripheral request for which DMA channel is to be cleared?
 * @return None
 *
 * \par<b>Description: </b><br>
 * Clear destination peripheral request <br>
 *
 * \par
 * The function is used to clear the destination peripheral request for a given DMA
 * channel.
 */
void XMC_DMA_CH_ClearDestinationPeripheralRequest(XMC_DMA_t *const dma, uint8_t channel);

/**
 * @param dma A constant pointer to XMC_DMA_t, pointing to the GPDMA base address
 * @return None
 *
 * \par<b>Description: </b><br>
 * Default GPDMA IRQ handler <br>
 *
 * \par
 * The function implements a default GPDMA IRQ handler. It can be used within the
 * following (device specific) routines: <br>
 * 1) GPDMA0_0_IRQHandler <br>
 * 2) GPDMA1_0_IRQHandler <br>
 *
 * The function handles the enabled GPDMA events and runs the user callback function
 * registered by the user to service the event. To register a callback function,
 * see ::XMC_DMA_CH_SetEventHandler()
 */
void XMC_DMA_IRQHandler(XMC_DMA_t *const dma);

#ifdef __cplusplus
}
#endif

/**
 * @} (end addtogroup DMA)
 */

/**
 * @} (end addtogroup XMClib)
 */

#endif /* defined (GPDMA0) */
#endif /* XMC_DMA_H */
