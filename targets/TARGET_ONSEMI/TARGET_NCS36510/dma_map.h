/**
******************************************************************************
* @file dma_map.h
* @brief DMA hw module register map
* @internal
* @author ON Semiconductor
* $Rev: 3415 $
* $Date: 2015-06-05 13:29:52 +0530 (Fri, 05 Jun 2015) $
******************************************************************************
 * Copyright 2016 Semiconductor Components Industries LLC (d/b/a �ON Semiconductor�).
 * All rights reserved.  This software and/or documentation is licensed by ON Semiconductor
 * under limited terms and conditions.  The terms and conditions pertaining to the software
 * and/or documentation are available at http://www.onsemi.com/site/pdf/ONSEMI_T&C.pdf
 * (�ON Semiconductor Standard Terms and Conditions of Sale, Section 8 Software�) and
 * if applicable the software license agreement.  Do not use this software and/or
 * documentation unless you have carefully read and you agree to the limited terms and
 * conditions.  By using this software and/or documentation, you agree to the limited
 * terms and conditions.
*
* THIS SOFTWARE IS PROVIDED "AS IS".  NO WARRANTIES, WHETHER EXPRESS, IMPLIED
* OR STATUTORY, INCLUDING, BUT NOT LIMITED TO, IMPLIED WARRANTIES OF
* MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE APPLY TO THIS SOFTWARE.
* ON SEMICONDUCTOR SHALL NOT, IN ANY CIRCUMSTANCES, BE LIABLE FOR SPECIAL,
* INCIDENTAL, OR CONSEQUENTIAL DAMAGES, FOR ANY REASON WHATSOEVER.
* @endinternal
*
* @ingroup dma
*
* @details
*/

#ifndef DMA_MAP_H_
#define DMA_MAP_H_

/*************************************************************************************************
*                                                                                                *
*  Header files                                                                                  *
*                                                                                                *
*************************************************************************************************/

#include "architecture.h"

/**************************************************************************************************
*                                                                                                 *
*  Type definitions                                                                               *
*                                                                                                 *
**************************************************************************************************/

/** DMA control HW registers structure overlay */
typedef struct {
    union {
        struct {
            __IO uint32_t ENABLE: 1;       /**< DMA enable:  1 to enable; 0 to disable */
            __IO uint32_t MODE    : 2;     /**< DMA mode: 00 � Memory to memory; 01 � Memory to peripheral; 10 � Peripheral to memory; 11 � Peripheral to peripheral */
        } BITS;
        __IO uint32_t WORD;
    } CONTROL;                           /**< Control register */
    __IO uint32_t SOURCE;                /**< Address of source, read to get the number of bytes written */
    __IO uint32_t DESTINATION;           /**< Address of destination, read to get the number of bytes written  */
    __IO uint32_t SIZE;                  /**< Lenght of the entire transfer */
    union {
        struct {
            __I uint32_t COMPLETED: 1;           /**< Done: 0 � Not complete, 1 � Complete */
            __I uint32_t SOURCE_ERROR: 1;        /**< Source Error: 0 � No Error, 1 � Error */
            __I uint32_t DESTINATION_ERROR: 1;   /**< Destination Error: 0 � No Error, 1 � Source Error */
        } BITS;
        __I uint32_t WORD;
    } STATUS;                                    /**< Status register */
    union {
        struct {
            __IO uint32_t COMPLETED: 1;           /**< A write of �1� enables the interrupt generated by a DMA transfer complete */
            __IO uint32_t SOURCE_ERROR: 1;        /**< A write of �1� enables the interrupt generated by an error on the source side of the DMA transfer */
            __IO uint32_t DESTINATION_ERROR: 1;   /**< A write of �1� enables the interrupt generated by an error on the destination side of the DMA transfer */
        } BITS;
        __IO uint32_t WORD;
    } INT_ENABLE;                                /**< Interrupt enable */
    union {
        struct {
            __IO uint32_t COMPLETED: 1;           /**< A write clears the interrupt generated by a DMA transfer complete */
            __IO uint32_t SOURCE_ERROR: 1;        /**< A write clears the interrupt generated by an error on the source side of the DMA transfer */
            __IO uint32_t DESTINATION_ERROR: 1;   /**< A write clears the interrupt generated by an error on the destination side of the DMA transfer */
        } BITS;
        __IO uint32_t WORD;
    } INT_CLEAR;                                /**< Interrupt clear */
    union {
        struct {
            __I uint32_t COMPLETED: 1;           /**< Transfer complete interrupt */
            __I uint32_t SOURCE_ERROR: 1;        /**< Source error interrupt */
            __I uint32_t DESTINATION_ERROR: 1;   /**< Destination error interrupt */
        } BITS;
        __I uint32_t WORD;
    } INT_STATUS;                                /**< Interrupt status */
} DmaReg_t, *DmaReg_pt;
#endif /* DMA_MAP_H_ */
