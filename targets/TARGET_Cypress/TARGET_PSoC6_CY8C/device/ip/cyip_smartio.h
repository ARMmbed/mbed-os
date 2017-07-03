/***************************************************************************//**
* \file cyip_smartio.h
*
* \brief
* SMARTIO IP definitions
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

#ifndef _CYIP_SMARTIO_H_
#define _CYIP_SMARTIO_H_

#include "cyip_headers.h"

/* =========================================================================================================================== */
/* ================                                          SMARTIO                                          ================ */
/* =========================================================================================================================== */

#define SMARTIO_PRT_SECTION_SIZE    0x00000100UL
#define SMARTIO_SECTION_SIZE        0x00010000UL

/**
  * \brief Programmable IO port registers (SMARTIO_PRT)
  */
typedef struct {
  __IOM uint32_t CTL;                           /*!< 0x00000000 Control register                                               */
   __IM uint32_t RESERVED[3];
  __IOM uint32_t SYNC_CTL;                      /*!< 0x00000010 Synchronization control register                               */
   __IM uint32_t RESERVED1[3];
  __IOM uint32_t LUT_SEL[8];                    /*!< 0x00000020 LUT component input selection                                  */
  __IOM uint32_t LUT_CTL[8];                    /*!< 0x00000040 LUT component control register                                 */
   __IM uint32_t RESERVED2[24];
  __IOM uint32_t DU_SEL;                        /*!< 0x000000C0 Data unit component input selection                            */
  __IOM uint32_t DU_CTL;                        /*!< 0x000000C4 Data unit component control register                           */
   __IM uint32_t RESERVED3[10];
  __IOM uint32_t DATA;                          /*!< 0x000000F0 Data register                                                  */
   __IM uint32_t RESERVED4[3];
} SMARTIO_PRT_Type;                             /*!< Size = 244 (0xF4)                                                         */

/**
  * \brief Programmable IO configuration (SMARTIO)
  */
typedef struct {                                /*!< SMARTIO Structure                                                         */
        SMARTIO_PRT_Type PRT[128];              /*!< 0x00000000 Programmable IO port registers                                 */
} SMARTIO_Type;                                 /*!< Size = 32768 (0x8000)                                                     */


/* ====================================================  SMARTIO_PRT.CTL  ==================================================== */
#define SMARTIO_PRT_CTL_BYPASS_Pos        (0UL)                 /*!< SMARTIO_PRT CTL: BYPASS (Bit 0)                           */
#define SMARTIO_PRT_CTL_BYPASS_Msk        (0xffUL)              /*!< SMARTIO_PRT CTL: BYPASS (Bitfield-Mask: 0xff)             */
#define SMARTIO_PRT_CTL_CLOCK_SRC_Pos     (8UL)                 /*!< SMARTIO_PRT CTL: CLOCK_SRC (Bit 8)                        */
#define SMARTIO_PRT_CTL_CLOCK_SRC_Msk     (0x1f00UL)            /*!< SMARTIO_PRT CTL: CLOCK_SRC (Bitfield-Mask: 0x1f)          */
#define SMARTIO_PRT_CTL_HLD_OVR_Pos       (24UL)                /*!< SMARTIO_PRT CTL: HLD_OVR (Bit 24)                         */
#define SMARTIO_PRT_CTL_HLD_OVR_Msk       (0x1000000UL)         /*!< SMARTIO_PRT CTL: HLD_OVR (Bitfield-Mask: 0x01)            */
#define SMARTIO_PRT_CTL_PIPELINE_EN_Pos   (25UL)                /*!< SMARTIO_PRT CTL: PIPELINE_EN (Bit 25)                     */
#define SMARTIO_PRT_CTL_PIPELINE_EN_Msk   (0x2000000UL)         /*!< SMARTIO_PRT CTL: PIPELINE_EN (Bitfield-Mask: 0x01)        */
#define SMARTIO_PRT_CTL_ENABLED_Pos       (31UL)                /*!< SMARTIO_PRT CTL: ENABLED (Bit 31)                         */
#define SMARTIO_PRT_CTL_ENABLED_Msk       (0x80000000UL)        /*!< SMARTIO_PRT CTL: ENABLED (Bitfield-Mask: 0x01)            */
/* =================================================  SMARTIO_PRT.SYNC_CTL  ================================================== */
#define SMARTIO_PRT_SYNC_CTL_IO_SYNC_EN_Pos (0UL)               /*!< SMARTIO_PRT SYNC_CTL: IO_SYNC_EN (Bit 0)                  */
#define SMARTIO_PRT_SYNC_CTL_IO_SYNC_EN_Msk (0xffUL)            /*!< SMARTIO_PRT SYNC_CTL: IO_SYNC_EN (Bitfield-Mask: 0xff)    */
#define SMARTIO_PRT_SYNC_CTL_CHIP_SYNC_EN_Pos (8UL)             /*!< SMARTIO_PRT SYNC_CTL: CHIP_SYNC_EN (Bit 8)                */
#define SMARTIO_PRT_SYNC_CTL_CHIP_SYNC_EN_Msk (0xff00UL)        /*!< SMARTIO_PRT SYNC_CTL: CHIP_SYNC_EN (Bitfield-Mask: 0xff)  */
/* ==================================================  SMARTIO_PRT.LUT_SEL  ================================================== */
#define SMARTIO_PRT_LUT_SEL_LUT_TR0_SEL_Pos (0UL)               /*!< SMARTIO_PRT LUT_SEL: LUT_TR0_SEL (Bit 0)                  */
#define SMARTIO_PRT_LUT_SEL_LUT_TR0_SEL_Msk (0xfUL)             /*!< SMARTIO_PRT LUT_SEL: LUT_TR0_SEL (Bitfield-Mask: 0x0f)    */
#define SMARTIO_PRT_LUT_SEL_LUT_TR1_SEL_Pos (8UL)               /*!< SMARTIO_PRT LUT_SEL: LUT_TR1_SEL (Bit 8)                  */
#define SMARTIO_PRT_LUT_SEL_LUT_TR1_SEL_Msk (0xf00UL)           /*!< SMARTIO_PRT LUT_SEL: LUT_TR1_SEL (Bitfield-Mask: 0x0f)    */
#define SMARTIO_PRT_LUT_SEL_LUT_TR2_SEL_Pos (16UL)              /*!< SMARTIO_PRT LUT_SEL: LUT_TR2_SEL (Bit 16)                 */
#define SMARTIO_PRT_LUT_SEL_LUT_TR2_SEL_Msk (0xf0000UL)         /*!< SMARTIO_PRT LUT_SEL: LUT_TR2_SEL (Bitfield-Mask: 0x0f)    */
/* ==================================================  SMARTIO_PRT.LUT_CTL  ================================================== */
#define SMARTIO_PRT_LUT_CTL_LUT_Pos       (0UL)                 /*!< SMARTIO_PRT LUT_CTL: LUT (Bit 0)                          */
#define SMARTIO_PRT_LUT_CTL_LUT_Msk       (0xffUL)              /*!< SMARTIO_PRT LUT_CTL: LUT (Bitfield-Mask: 0xff)            */
#define SMARTIO_PRT_LUT_CTL_LUT_OPC_Pos   (8UL)                 /*!< SMARTIO_PRT LUT_CTL: LUT_OPC (Bit 8)                      */
#define SMARTIO_PRT_LUT_CTL_LUT_OPC_Msk   (0x300UL)             /*!< SMARTIO_PRT LUT_CTL: LUT_OPC (Bitfield-Mask: 0x03)        */
/* ==================================================  SMARTIO_PRT.DU_SEL  =================================================== */
#define SMARTIO_PRT_DU_SEL_DU_TR0_SEL_Pos (0UL)                 /*!< SMARTIO_PRT DU_SEL: DU_TR0_SEL (Bit 0)                    */
#define SMARTIO_PRT_DU_SEL_DU_TR0_SEL_Msk (0xfUL)               /*!< SMARTIO_PRT DU_SEL: DU_TR0_SEL (Bitfield-Mask: 0x0f)      */
#define SMARTIO_PRT_DU_SEL_DU_TR1_SEL_Pos (8UL)                 /*!< SMARTIO_PRT DU_SEL: DU_TR1_SEL (Bit 8)                    */
#define SMARTIO_PRT_DU_SEL_DU_TR1_SEL_Msk (0xf00UL)             /*!< SMARTIO_PRT DU_SEL: DU_TR1_SEL (Bitfield-Mask: 0x0f)      */
#define SMARTIO_PRT_DU_SEL_DU_TR2_SEL_Pos (16UL)                /*!< SMARTIO_PRT DU_SEL: DU_TR2_SEL (Bit 16)                   */
#define SMARTIO_PRT_DU_SEL_DU_TR2_SEL_Msk (0xf0000UL)           /*!< SMARTIO_PRT DU_SEL: DU_TR2_SEL (Bitfield-Mask: 0x0f)      */
#define SMARTIO_PRT_DU_SEL_DU_DATA0_SEL_Pos (24UL)              /*!< SMARTIO_PRT DU_SEL: DU_DATA0_SEL (Bit 24)                 */
#define SMARTIO_PRT_DU_SEL_DU_DATA0_SEL_Msk (0x3000000UL)       /*!< SMARTIO_PRT DU_SEL: DU_DATA0_SEL (Bitfield-Mask: 0x03)    */
#define SMARTIO_PRT_DU_SEL_DU_DATA1_SEL_Pos (28UL)              /*!< SMARTIO_PRT DU_SEL: DU_DATA1_SEL (Bit 28)                 */
#define SMARTIO_PRT_DU_SEL_DU_DATA1_SEL_Msk (0x30000000UL)      /*!< SMARTIO_PRT DU_SEL: DU_DATA1_SEL (Bitfield-Mask: 0x03)    */
/* ==================================================  SMARTIO_PRT.DU_CTL  =================================================== */
#define SMARTIO_PRT_DU_CTL_DU_SIZE_Pos    (0UL)                 /*!< SMARTIO_PRT DU_CTL: DU_SIZE (Bit 0)                       */
#define SMARTIO_PRT_DU_CTL_DU_SIZE_Msk    (0x7UL)               /*!< SMARTIO_PRT DU_CTL: DU_SIZE (Bitfield-Mask: 0x07)         */
#define SMARTIO_PRT_DU_CTL_DU_OPC_Pos     (8UL)                 /*!< SMARTIO_PRT DU_CTL: DU_OPC (Bit 8)                        */
#define SMARTIO_PRT_DU_CTL_DU_OPC_Msk     (0xf00UL)             /*!< SMARTIO_PRT DU_CTL: DU_OPC (Bitfield-Mask: 0x0f)          */
/* ===================================================  SMARTIO_PRT.DATA  ==================================================== */
#define SMARTIO_PRT_DATA_DATA_Pos         (0UL)                 /*!< SMARTIO_PRT DATA: DATA (Bit 0)                            */
#define SMARTIO_PRT_DATA_DATA_Msk         (0xffUL)              /*!< SMARTIO_PRT DATA: DATA (Bitfield-Mask: 0xff)              */


#endif /* _CYIP_SMARTIO_H_ */


/* [] END OF FILE */
