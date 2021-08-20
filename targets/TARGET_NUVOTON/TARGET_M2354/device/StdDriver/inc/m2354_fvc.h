/**************************************************************************//**
 * @file     fvc.h
 * @version  V3.00
 * @brief    Firmware Version Counter Driver Header
 *
 * @note
 * SPDX-License-Identifier: Apache-2.0
 * Copyright (C) 2020 Nuvoton Technology Corp. All rights reserved.
 *****************************************************************************/
#ifndef __FVC_H__
#define __FVC_H__

#ifdef __cplusplus
extern "C"
{
#endif


/** @addtogroup Standard_Driver Standard Driver
  @{
*/

/** @addtogroup FVC_Driver FVC Driver
  @{
*/

/** @addtogroup FVC_EXPORTED_CONSTANTS FVC Exported Constants
  @{
*/

#define FVC_VCODE   (0x77100000ul)  /*!< The key code for FVC_CTL write. */
#define FVC_BL2     (FVC->BL2)      /*!< Version number of BL2 Firmware  */
#define FVC_BL32    (FVC->BL32)     /*!< Version number of BL32 Firmware */
#define FVC_BL33    (FVC->BL33)     /*!< Version number of BL33 Firmware */
#define FVC_UDF     (FVC->UDF)      /*!< Version number of User Firmware */

/**@}*/ /* end of group FVC_EXPORTED_CONSTANTS */


/** @addtogroup FVC_EXPORTED_FUNCTIONS FVC Exported Functions
  @{
*/

int32_t FVC_Open(void);
void FVC_EnableMonotone(void);
int32_t FVC_SetBL2Ver(uint32_t u32Ver);
int32_t FVC_SetBL32Ver(uint32_t u32Ver);
int32_t FVC_SetBL33Ver(uint32_t u32Ver);
int32_t FVC_SetUDFVer(uint32_t u32Ver);


/**@}*/ /* end of group FVC_EXPORTED_FUNCTIONS */

/**@}*/ /* end of group FVC_Driver */

/**@}*/ /* end of group Standard_Driver */

#ifdef __cplusplus
}
#endif

#endif /* __FVC_H__ */

/*** (C) COPYRIGHT 2019 Nuvoton Technology Corp. ***/
