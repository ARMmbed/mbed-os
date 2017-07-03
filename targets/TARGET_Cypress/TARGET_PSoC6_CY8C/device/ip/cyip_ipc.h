/***************************************************************************//**
* \file cyip_ipc.h
*
* \brief
* IPC IP definitions
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

#ifndef _CYIP_IPC_H_
#define _CYIP_IPC_H_

#include "cyip_headers.h"

/* =========================================================================================================================== */
/* ================                                            IPC                                            ================ */
/* =========================================================================================================================== */

#define IPC_STRUCT_SECTION_SIZE     0x00000020UL
#define IPC_INTR_STRUCT_SECTION_SIZE 0x00000020UL
#define IPC_SECTION_SIZE            0x00010000UL

/**
  * \brief IPC structure (IPC_STRUCT)
  */
typedef struct {
   __IM uint32_t ACQUIRE;                       /*!< 0x00000000 IPC acquire                                                    */
   __OM uint32_t RELEASE;                       /*!< 0x00000004 IPC release                                                    */
   __OM uint32_t NOTIFY;                        /*!< 0x00000008 IPC notification                                               */
  __IOM uint32_t DATA;                          /*!< 0x0000000C IPC data                                                       */
   __IM uint32_t LOCK_STATUS;                   /*!< 0x00000010 IPC lock status                                                */
   __IM uint32_t RESERVED[3];
} IPC_STRUCT_Type;                              /*!< Size = 20 (0x14)                                                          */

/**
  * \brief IPC interrupt structure (IPC_INTR_STRUCT)
  */
typedef struct {
  __IOM uint32_t INTR;                          /*!< 0x00000000 Interrupt                                                      */
  __IOM uint32_t INTR_SET;                      /*!< 0x00000004 Interrupt set                                                  */
  __IOM uint32_t INTR_MASK;                     /*!< 0x00000008 Interrupt mask                                                 */
   __IM uint32_t INTR_MASKED;                   /*!< 0x0000000C Interrupt masked                                               */
   __IM uint32_t RESERVED[4];
} IPC_INTR_STRUCT_Type;                         /*!< Size = 16 (0x10)                                                          */

/**
  * \brief IPC (IPC)
  */
typedef struct {                                /*!< IPC Structure                                                             */
        IPC_STRUCT_Type STRUCT[16];             /*!< 0x00000000 IPC structure                                                  */
   __IM uint32_t RESERVED[896];
        IPC_INTR_STRUCT_Type INTR_STRUCT[16];   /*!< 0x00001000 IPC interrupt structure                                        */
} IPC_Type;                                     /*!< Size = 4608 (0x1200)                                                      */


/* ==================================================  IPC_STRUCT.ACQUIRE  =================================================== */
#define IPC_STRUCT_ACQUIRE_P_Pos          (0UL)                 /*!< IPC_STRUCT ACQUIRE: P (Bit 0)                             */
#define IPC_STRUCT_ACQUIRE_P_Msk          (0x1UL)               /*!< IPC_STRUCT ACQUIRE: P (Bitfield-Mask: 0x01)               */
#define IPC_STRUCT_ACQUIRE_NS_Pos         (1UL)                 /*!< IPC_STRUCT ACQUIRE: NS (Bit 1)                            */
#define IPC_STRUCT_ACQUIRE_NS_Msk         (0x2UL)               /*!< IPC_STRUCT ACQUIRE: NS (Bitfield-Mask: 0x01)              */
#define IPC_STRUCT_ACQUIRE_PC_Pos         (4UL)                 /*!< IPC_STRUCT ACQUIRE: PC (Bit 4)                            */
#define IPC_STRUCT_ACQUIRE_PC_Msk         (0xf0UL)              /*!< IPC_STRUCT ACQUIRE: PC (Bitfield-Mask: 0x0f)              */
#define IPC_STRUCT_ACQUIRE_MS_Pos         (8UL)                 /*!< IPC_STRUCT ACQUIRE: MS (Bit 8)                            */
#define IPC_STRUCT_ACQUIRE_MS_Msk         (0xf00UL)             /*!< IPC_STRUCT ACQUIRE: MS (Bitfield-Mask: 0x0f)              */
#define IPC_STRUCT_ACQUIRE_SUCCESS_Pos    (31UL)                /*!< IPC_STRUCT ACQUIRE: SUCCESS (Bit 31)                      */
#define IPC_STRUCT_ACQUIRE_SUCCESS_Msk    (0x80000000UL)        /*!< IPC_STRUCT ACQUIRE: SUCCESS (Bitfield-Mask: 0x01)         */
/* ==================================================  IPC_STRUCT.RELEASE  =================================================== */
#define IPC_STRUCT_RELEASE_INTR_RELEASE_Pos (0UL)               /*!< IPC_STRUCT RELEASE: INTR_RELEASE (Bit 0)                  */
#define IPC_STRUCT_RELEASE_INTR_RELEASE_Msk (0xffffUL)          /*!< IPC_STRUCT RELEASE: INTR_RELEASE (Bitfield-Mask: 0xffff)  */
/* ===================================================  IPC_STRUCT.NOTIFY  =================================================== */
#define IPC_STRUCT_NOTIFY_INTR_NOTIFY_Pos (0UL)                 /*!< IPC_STRUCT NOTIFY: INTR_NOTIFY (Bit 0)                    */
#define IPC_STRUCT_NOTIFY_INTR_NOTIFY_Msk (0xffffUL)            /*!< IPC_STRUCT NOTIFY: INTR_NOTIFY (Bitfield-Mask: 0xffff)    */
/* ====================================================  IPC_STRUCT.DATA  ==================================================== */
#define IPC_STRUCT_DATA_DATA_Pos          (0UL)                 /*!< IPC_STRUCT DATA: DATA (Bit 0)                             */
#define IPC_STRUCT_DATA_DATA_Msk          (0xffffffffUL)        /*!< IPC_STRUCT DATA: DATA (Bitfield-Mask: 0xffffffff)         */
/* ================================================  IPC_STRUCT.LOCK_STATUS  ================================================= */
#define IPC_STRUCT_LOCK_STATUS_P_Pos      (0UL)                 /*!< IPC_STRUCT LOCK_STATUS: P (Bit 0)                         */
#define IPC_STRUCT_LOCK_STATUS_P_Msk      (0x1UL)               /*!< IPC_STRUCT LOCK_STATUS: P (Bitfield-Mask: 0x01)           */
#define IPC_STRUCT_LOCK_STATUS_NS_Pos     (1UL)                 /*!< IPC_STRUCT LOCK_STATUS: NS (Bit 1)                        */
#define IPC_STRUCT_LOCK_STATUS_NS_Msk     (0x2UL)               /*!< IPC_STRUCT LOCK_STATUS: NS (Bitfield-Mask: 0x01)          */
#define IPC_STRUCT_LOCK_STATUS_PC_Pos     (4UL)                 /*!< IPC_STRUCT LOCK_STATUS: PC (Bit 4)                        */
#define IPC_STRUCT_LOCK_STATUS_PC_Msk     (0xf0UL)              /*!< IPC_STRUCT LOCK_STATUS: PC (Bitfield-Mask: 0x0f)          */
#define IPC_STRUCT_LOCK_STATUS_MS_Pos     (8UL)                 /*!< IPC_STRUCT LOCK_STATUS: MS (Bit 8)                        */
#define IPC_STRUCT_LOCK_STATUS_MS_Msk     (0xf00UL)             /*!< IPC_STRUCT LOCK_STATUS: MS (Bitfield-Mask: 0x0f)          */
#define IPC_STRUCT_LOCK_STATUS_ACQUIRED_Pos (31UL)              /*!< IPC_STRUCT LOCK_STATUS: ACQUIRED (Bit 31)                 */
#define IPC_STRUCT_LOCK_STATUS_ACQUIRED_Msk (0x80000000UL)      /*!< IPC_STRUCT LOCK_STATUS: ACQUIRED (Bitfield-Mask: 0x01)    */


/* =================================================  IPC_INTR_STRUCT.INTR  ================================================== */
#define IPC_INTR_STRUCT_INTR_RELEASE_Pos  (0UL)                 /*!< IPC_INTR_STRUCT INTR: RELEASE (Bit 0)                     */
#define IPC_INTR_STRUCT_INTR_RELEASE_Msk  (0xffffUL)            /*!< IPC_INTR_STRUCT INTR: RELEASE (Bitfield-Mask: 0xffff)     */
#define IPC_INTR_STRUCT_INTR_NOTIFY_Pos   (16UL)                /*!< IPC_INTR_STRUCT INTR: NOTIFY (Bit 16)                     */
#define IPC_INTR_STRUCT_INTR_NOTIFY_Msk   (0xffff0000UL)        /*!< IPC_INTR_STRUCT INTR: NOTIFY (Bitfield-Mask: 0xffff)      */
/* ===============================================  IPC_INTR_STRUCT.INTR_SET  ================================================ */
#define IPC_INTR_STRUCT_INTR_SET_RELEASE_Pos (0UL)              /*!< IPC_INTR_STRUCT INTR_SET: RELEASE (Bit 0)                 */
#define IPC_INTR_STRUCT_INTR_SET_RELEASE_Msk (0xffffUL)         /*!< IPC_INTR_STRUCT INTR_SET: RELEASE (Bitfield-Mask: 0xffff) */
#define IPC_INTR_STRUCT_INTR_SET_NOTIFY_Pos (16UL)              /*!< IPC_INTR_STRUCT INTR_SET: NOTIFY (Bit 16)                 */
#define IPC_INTR_STRUCT_INTR_SET_NOTIFY_Msk (0xffff0000UL)      /*!< IPC_INTR_STRUCT INTR_SET: NOTIFY (Bitfield-Mask: 0xffff)  */
/* ===============================================  IPC_INTR_STRUCT.INTR_MASK  =============================================== */
#define IPC_INTR_STRUCT_INTR_MASK_RELEASE_Pos (0UL)             /*!< IPC_INTR_STRUCT INTR_MASK: RELEASE (Bit 0)                */
#define IPC_INTR_STRUCT_INTR_MASK_RELEASE_Msk (0xffffUL)        /*!< IPC_INTR_STRUCT INTR_MASK: RELEASE (Bitfield-Mask: 0xffff)*/
#define IPC_INTR_STRUCT_INTR_MASK_NOTIFY_Pos (16UL)             /*!< IPC_INTR_STRUCT INTR_MASK: NOTIFY (Bit 16)                */
#define IPC_INTR_STRUCT_INTR_MASK_NOTIFY_Msk (0xffff0000UL)     /*!< IPC_INTR_STRUCT INTR_MASK: NOTIFY (Bitfield-Mask: 0xffff) */
/* ==============================================  IPC_INTR_STRUCT.INTR_MASKED  ============================================== */
#define IPC_INTR_STRUCT_INTR_MASKED_RELEASE_Pos (0UL)           /*!< IPC_INTR_STRUCT INTR_MASKED: RELEASE (Bit 0)              */
#define IPC_INTR_STRUCT_INTR_MASKED_RELEASE_Msk (0xffffUL)      /*!< IPC_INTR_STRUCT INTR_MASKED: RELEASE (Bitfield-Mask: 0xffff)*/
#define IPC_INTR_STRUCT_INTR_MASKED_NOTIFY_Pos (16UL)           /*!< IPC_INTR_STRUCT INTR_MASKED: NOTIFY (Bit 16)              */
#define IPC_INTR_STRUCT_INTR_MASKED_NOTIFY_Msk (0xffff0000UL)   /*!< IPC_INTR_STRUCT INTR_MASKED: NOTIFY (Bitfield-Mask: 0xffff)*/


#endif /* _CYIP_IPC_H_ */


/* [] END OF FILE */
