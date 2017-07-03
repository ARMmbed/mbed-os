/***************************************************************************//**
* \file cyip_profile.h
*
* \brief
* PROFILE IP definitions
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

#ifndef _CYIP_PROFILE_H_
#define _CYIP_PROFILE_H_

#include "cyip_headers.h"

/* =========================================================================================================================== */
/* ================                                          PROFILE                                          ================ */
/* =========================================================================================================================== */

#define PROFILE_CNT_STRUCT_SECTION_SIZE 0x00000010UL
#define PROFILE_SECTION_SIZE        0x00010000UL

/**
  * \brief Profile counter structure (PROFILE_CNT_STRUCT)
  */
typedef struct {
  __IOM uint32_t CTL;                           /*!< 0x00000000 Profile counter configuration                                  */
   __IM uint32_t RESERVED;
  __IOM uint32_t CNT;                           /*!< 0x00000008 Profile counter value                                          */
   __IM uint32_t RESERVED1;
} PROFILE_CNT_STRUCT_Type;                      /*!< Size = 12 (0xC)                                                           */

/**
  * \brief Energy Profiler IP (PROFILE)
  */
typedef struct {                                /*!< PROFILE Structure                                                         */
  __IOM uint32_t CTL;                           /*!< 0x00000000 Profile control                                                */
   __IM uint32_t STATUS;                        /*!< 0x00000004 Profile status                                                 */
   __IM uint32_t RESERVED[2];
  __IOM uint32_t CMD;                           /*!< 0x00000010 Profile command                                                */
   __IM uint32_t RESERVED1[491];
  __IOM uint32_t INTR;                          /*!< 0x000007C0 Profile interrupt                                              */
  __IOM uint32_t INTR_SET;                      /*!< 0x000007C4 Profile interrupt set                                          */
  __IOM uint32_t INTR_MASK;                     /*!< 0x000007C8 Profile interrupt mask                                         */
   __IM uint32_t INTR_MASKED;                   /*!< 0x000007CC Profile interrupt masked                                       */
   __IM uint32_t RESERVED2[12];
        PROFILE_CNT_STRUCT_Type CNT_STRUCT[16]; /*!< 0x00000800 Profile counter structure                                      */
} PROFILE_Type;                                 /*!< Size = 2304 (0x900)                                                       */


/* ================================================  PROFILE_CNT_STRUCT.CTL  ================================================= */
#define PROFILE_CNT_STRUCT_CTL_CNT_DURATION_Pos (0UL)           /*!< PROFILE_CNT_STRUCT CTL: CNT_DURATION (Bit 0)              */
#define PROFILE_CNT_STRUCT_CTL_CNT_DURATION_Msk (0x1UL)         /*!< PROFILE_CNT_STRUCT CTL: CNT_DURATION (Bitfield-Mask: 0x01)*/
#define PROFILE_CNT_STRUCT_CTL_REF_CLK_SEL_Pos (4UL)            /*!< PROFILE_CNT_STRUCT CTL: REF_CLK_SEL (Bit 4)               */
#define PROFILE_CNT_STRUCT_CTL_REF_CLK_SEL_Msk (0x70UL)         /*!< PROFILE_CNT_STRUCT CTL: REF_CLK_SEL (Bitfield-Mask: 0x07) */
#define PROFILE_CNT_STRUCT_CTL_MON_SEL_Pos (16UL)               /*!< PROFILE_CNT_STRUCT CTL: MON_SEL (Bit 16)                  */
#define PROFILE_CNT_STRUCT_CTL_MON_SEL_Msk (0x7f0000UL)         /*!< PROFILE_CNT_STRUCT CTL: MON_SEL (Bitfield-Mask: 0x7f)     */
#define PROFILE_CNT_STRUCT_CTL_ENABLED_Pos (31UL)               /*!< PROFILE_CNT_STRUCT CTL: ENABLED (Bit 31)                  */
#define PROFILE_CNT_STRUCT_CTL_ENABLED_Msk (0x80000000UL)       /*!< PROFILE_CNT_STRUCT CTL: ENABLED (Bitfield-Mask: 0x01)     */
/* ================================================  PROFILE_CNT_STRUCT.CNT  ================================================= */
#define PROFILE_CNT_STRUCT_CNT_CNT_Pos    (0UL)                 /*!< PROFILE_CNT_STRUCT CNT: CNT (Bit 0)                       */
#define PROFILE_CNT_STRUCT_CNT_CNT_Msk    (0xffffffffUL)        /*!< PROFILE_CNT_STRUCT CNT: CNT (Bitfield-Mask: 0xffffffff)   */


/* ======================================================  PROFILE.CTL  ====================================================== */
#define PROFILE_CTL_WIN_MODE_Pos          (0UL)                 /*!< PROFILE CTL: WIN_MODE (Bit 0)                             */
#define PROFILE_CTL_WIN_MODE_Msk          (0x1UL)               /*!< PROFILE CTL: WIN_MODE (Bitfield-Mask: 0x01)               */
#define PROFILE_CTL_ENABLED_Pos           (31UL)                /*!< PROFILE CTL: ENABLED (Bit 31)                             */
#define PROFILE_CTL_ENABLED_Msk           (0x80000000UL)        /*!< PROFILE CTL: ENABLED (Bitfield-Mask: 0x01)                */
/* ====================================================  PROFILE.STATUS  ===================================================== */
#define PROFILE_STATUS_WIN_ACTIVE_Pos     (0UL)                 /*!< PROFILE STATUS: WIN_ACTIVE (Bit 0)                        */
#define PROFILE_STATUS_WIN_ACTIVE_Msk     (0x1UL)               /*!< PROFILE STATUS: WIN_ACTIVE (Bitfield-Mask: 0x01)          */
/* ======================================================  PROFILE.CMD  ====================================================== */
#define PROFILE_CMD_START_TR_Pos          (0UL)                 /*!< PROFILE CMD: START_TR (Bit 0)                             */
#define PROFILE_CMD_START_TR_Msk          (0x1UL)               /*!< PROFILE CMD: START_TR (Bitfield-Mask: 0x01)               */
#define PROFILE_CMD_STOP_TR_Pos           (1UL)                 /*!< PROFILE CMD: STOP_TR (Bit 1)                              */
#define PROFILE_CMD_STOP_TR_Msk           (0x2UL)               /*!< PROFILE CMD: STOP_TR (Bitfield-Mask: 0x01)                */
#define PROFILE_CMD_CLR_ALL_CNT_Pos       (8UL)                 /*!< PROFILE CMD: CLR_ALL_CNT (Bit 8)                          */
#define PROFILE_CMD_CLR_ALL_CNT_Msk       (0x100UL)             /*!< PROFILE CMD: CLR_ALL_CNT (Bitfield-Mask: 0x01)            */
/* =====================================================  PROFILE.INTR  ====================================================== */
#define PROFILE_INTR_CNT_OVFLW_Pos        (0UL)                 /*!< PROFILE INTR: CNT_OVFLW (Bit 0)                           */
#define PROFILE_INTR_CNT_OVFLW_Msk        (0xffffffffUL)        /*!< PROFILE INTR: CNT_OVFLW (Bitfield-Mask: 0xffffffff)       */
/* ===================================================  PROFILE.INTR_SET  ==================================================== */
#define PROFILE_INTR_SET_CNT_OVFLW_Pos    (0UL)                 /*!< PROFILE INTR_SET: CNT_OVFLW (Bit 0)                       */
#define PROFILE_INTR_SET_CNT_OVFLW_Msk    (0xffffffffUL)        /*!< PROFILE INTR_SET: CNT_OVFLW (Bitfield-Mask: 0xffffffff)   */
/* ===================================================  PROFILE.INTR_MASK  =================================================== */
#define PROFILE_INTR_MASK_CNT_OVFLW_Pos   (0UL)                 /*!< PROFILE INTR_MASK: CNT_OVFLW (Bit 0)                      */
#define PROFILE_INTR_MASK_CNT_OVFLW_Msk   (0xffffffffUL)        /*!< PROFILE INTR_MASK: CNT_OVFLW (Bitfield-Mask: 0xffffffff)  */
/* ==================================================  PROFILE.INTR_MASKED  ================================================== */
#define PROFILE_INTR_MASKED_CNT_OVFLW_Pos (0UL)                 /*!< PROFILE INTR_MASKED: CNT_OVFLW (Bit 0)                    */
#define PROFILE_INTR_MASKED_CNT_OVFLW_Msk (0xffffffffUL)        /*!< PROFILE INTR_MASKED: CNT_OVFLW (Bitfield-Mask: 0xffffffff)*/


#endif /* _CYIP_PROFILE_H_ */


/* [] END OF FILE */
