/***************************************************************************//**
* \file cyip_hsiom.h
*
* \brief
* HSIOM IP definitions
*
* \note
* Generated 2/9/2017 by CyDeviceHeaderGenerator v1.1.0.47
* from the register map configuration rev#961378
*
********************************************************************************
* \copyright
* Copyright 2016-2017, Cypress Semiconductor Corporation. All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#ifndef _CYIP_HSIOM_H_
#define _CYIP_HSIOM_H_

#include "cyip_headers.h"

/* =========================================================================================================================== */
/* ================                                           HSIOM                                           ================ */
/* =========================================================================================================================== */

#define HSIOM_PRT_SECTION_SIZE      0x00000010UL
#define HSIOM_SECTION_SIZE          0x00004000UL

/**
  * \brief HSIOM port registers (HSIOM_PRT)
  */
typedef struct {
  __IOM uint32_t PORT_SEL0;                     /*!< 0x00000000 Port selection 0                                               */
  __IOM uint32_t PORT_SEL1;                     /*!< 0x00000004 Port selection 1                                               */
   __IM uint32_t RESERVED[2];
} HSIOM_PRT_Type;                               /*!< Size = 8 (0x8)                                                            */

/**
  * \brief High Speed IO Matrix (HSIOM) (HSIOM)
  */
typedef struct {                                /*!< HSIOM Structure                                                           */
        HSIOM_PRT_Type PRT[128];                /*!< 0x00000000 HSIOM port registers                                           */
   __IM uint32_t RESERVED[1536];
  __IOM uint32_t AMUX_SPLIT_CTL[64];            /*!< 0x00002000 AMUX splitter cell control                                     */
} HSIOM_Type;                                   /*!< Size = 8448 (0x2100)                                                      */


/* ==================================================  HSIOM_PRT.PORT_SEL0  ================================================== */
#define HSIOM_PRT_PORT_SEL0_IO0_SEL_Pos   (0UL)                 /*!< HSIOM_PRT PORT_SEL0: IO0_SEL (Bit 0)                      */
#define HSIOM_PRT_PORT_SEL0_IO0_SEL_Msk   (0x1fUL)              /*!< HSIOM_PRT PORT_SEL0: IO0_SEL (Bitfield-Mask: 0x1f)        */
#define HSIOM_PRT_PORT_SEL0_IO1_SEL_Pos   (8UL)                 /*!< HSIOM_PRT PORT_SEL0: IO1_SEL (Bit 8)                      */
#define HSIOM_PRT_PORT_SEL0_IO1_SEL_Msk   (0x1f00UL)            /*!< HSIOM_PRT PORT_SEL0: IO1_SEL (Bitfield-Mask: 0x1f)        */
#define HSIOM_PRT_PORT_SEL0_IO2_SEL_Pos   (16UL)                /*!< HSIOM_PRT PORT_SEL0: IO2_SEL (Bit 16)                     */
#define HSIOM_PRT_PORT_SEL0_IO2_SEL_Msk   (0x1f0000UL)          /*!< HSIOM_PRT PORT_SEL0: IO2_SEL (Bitfield-Mask: 0x1f)        */
#define HSIOM_PRT_PORT_SEL0_IO3_SEL_Pos   (24UL)                /*!< HSIOM_PRT PORT_SEL0: IO3_SEL (Bit 24)                     */
#define HSIOM_PRT_PORT_SEL0_IO3_SEL_Msk   (0x1f000000UL)        /*!< HSIOM_PRT PORT_SEL0: IO3_SEL (Bitfield-Mask: 0x1f)        */
/* ==================================================  HSIOM_PRT.PORT_SEL1  ================================================== */
#define HSIOM_PRT_PORT_SEL1_IO4_SEL_Pos   (0UL)                 /*!< HSIOM_PRT PORT_SEL1: IO4_SEL (Bit 0)                      */
#define HSIOM_PRT_PORT_SEL1_IO4_SEL_Msk   (0x1fUL)              /*!< HSIOM_PRT PORT_SEL1: IO4_SEL (Bitfield-Mask: 0x1f)        */
#define HSIOM_PRT_PORT_SEL1_IO5_SEL_Pos   (8UL)                 /*!< HSIOM_PRT PORT_SEL1: IO5_SEL (Bit 8)                      */
#define HSIOM_PRT_PORT_SEL1_IO5_SEL_Msk   (0x1f00UL)            /*!< HSIOM_PRT PORT_SEL1: IO5_SEL (Bitfield-Mask: 0x1f)        */
#define HSIOM_PRT_PORT_SEL1_IO6_SEL_Pos   (16UL)                /*!< HSIOM_PRT PORT_SEL1: IO6_SEL (Bit 16)                     */
#define HSIOM_PRT_PORT_SEL1_IO6_SEL_Msk   (0x1f0000UL)          /*!< HSIOM_PRT PORT_SEL1: IO6_SEL (Bitfield-Mask: 0x1f)        */
#define HSIOM_PRT_PORT_SEL1_IO7_SEL_Pos   (24UL)                /*!< HSIOM_PRT PORT_SEL1: IO7_SEL (Bit 24)                     */
#define HSIOM_PRT_PORT_SEL1_IO7_SEL_Msk   (0x1f000000UL)        /*!< HSIOM_PRT PORT_SEL1: IO7_SEL (Bitfield-Mask: 0x1f)        */


/* =================================================  HSIOM.AMUX_SPLIT_CTL  ================================================== */
#define HSIOM_AMUX_SPLIT_CTL_SWITCH_AA_SL_Pos (0UL)             /*!< HSIOM AMUX_SPLIT_CTL: SWITCH_AA_SL (Bit 0)                */
#define HSIOM_AMUX_SPLIT_CTL_SWITCH_AA_SL_Msk (0x1UL)           /*!< HSIOM AMUX_SPLIT_CTL: SWITCH_AA_SL (Bitfield-Mask: 0x01)  */
#define HSIOM_AMUX_SPLIT_CTL_SWITCH_AA_SR_Pos (1UL)             /*!< HSIOM AMUX_SPLIT_CTL: SWITCH_AA_SR (Bit 1)                */
#define HSIOM_AMUX_SPLIT_CTL_SWITCH_AA_SR_Msk (0x2UL)           /*!< HSIOM AMUX_SPLIT_CTL: SWITCH_AA_SR (Bitfield-Mask: 0x01)  */
#define HSIOM_AMUX_SPLIT_CTL_SWITCH_AA_S0_Pos (2UL)             /*!< HSIOM AMUX_SPLIT_CTL: SWITCH_AA_S0 (Bit 2)                */
#define HSIOM_AMUX_SPLIT_CTL_SWITCH_AA_S0_Msk (0x4UL)           /*!< HSIOM AMUX_SPLIT_CTL: SWITCH_AA_S0 (Bitfield-Mask: 0x01)  */
#define HSIOM_AMUX_SPLIT_CTL_SWITCH_BB_SL_Pos (4UL)             /*!< HSIOM AMUX_SPLIT_CTL: SWITCH_BB_SL (Bit 4)                */
#define HSIOM_AMUX_SPLIT_CTL_SWITCH_BB_SL_Msk (0x10UL)          /*!< HSIOM AMUX_SPLIT_CTL: SWITCH_BB_SL (Bitfield-Mask: 0x01)  */
#define HSIOM_AMUX_SPLIT_CTL_SWITCH_BB_SR_Pos (5UL)             /*!< HSIOM AMUX_SPLIT_CTL: SWITCH_BB_SR (Bit 5)                */
#define HSIOM_AMUX_SPLIT_CTL_SWITCH_BB_SR_Msk (0x20UL)          /*!< HSIOM AMUX_SPLIT_CTL: SWITCH_BB_SR (Bitfield-Mask: 0x01)  */
#define HSIOM_AMUX_SPLIT_CTL_SWITCH_BB_S0_Pos (6UL)             /*!< HSIOM AMUX_SPLIT_CTL: SWITCH_BB_S0 (Bit 6)                */
#define HSIOM_AMUX_SPLIT_CTL_SWITCH_BB_S0_Msk (0x40UL)          /*!< HSIOM AMUX_SPLIT_CTL: SWITCH_BB_S0 (Bitfield-Mask: 0x01)  */


#endif /* _CYIP_HSIOM_H_ */


/* [] END OF FILE */
