/**************************************************************************//**
 * @file     fvc.c
 * @version  V3.00
 * @brief    FVC driver source file
 *
 * SPDX-License-Identifier: Apache-2.0
 * @copyright (C) 2020 Nuvoton Technology Corp. All rights reserved.
*****************************************************************************/
#include "NuMicro.h"

/** @addtogroup Standard_Driver Standard Driver
  @{
*/

/** @addtogroup FVC_Driver FVC Driver
  @{
*/


/** @addtogroup FVC_EXPORTED_FUNCTIONS FVC Exported Functions
  @{
*/




/**
  * @brief      Initial firmware version counter
  * @param      None
  * @retval     0       Successful
  * @retval     -1      Failed
  * @details    FVC needs to be initialed before using it. This function is used to initial the FVC.
  *
  */
int32_t FVC_Open(void)
{
    int32_t timeout;

    /* Just return when it is ready */
    if(FVC->STS & FVC_STS_RDY_Msk)
        return 0;

    /* Init FVC */
    FVC->CTL = FVC_VCODE | FVC_CTL_INIT_Msk;

    /* Waiting for ready */
    timeout = 0x100000;
    while((FVC->STS & FVC_STS_RDY_Msk) == 0)
    {
        if(timeout-- < 0)
        {
            /* Init timeout. */
            return -1;
        }
    }

    return 0;
}



/**
  * @brief      Enable anti version rollback
  * @details    FVC can limit the version number to be increased only to avoid version rollback.
  *             This function is used to enable it.
  *
  */
void FVC_EnableMonotone(void)
{
    FVC->CTL = FVC_VCODE | FVC_CTL_MONOEN_Msk;
    /* Waiting if FVC is in busy */
    while(FVC->STS & FVC->STS & FVC_STS_BUSY_Msk) {}
}

/**
  * @brief      Set BL2 version
  * @param[in]  ver     Version Number. It could be 0~63.
  * @retval     0       Successful
  * @retval     -1      Failed
  * @details    Set version number of BL2
  *
  */
int32_t FVC_SetBL2Ver(uint32_t u32Ver)
{
    if(u32Ver >= 64)
        return -1;

    FVC->BL2 = (FVC->BL2 << 16) | (u32Ver & 0x3ful);
    while(FVC->STS & FVC_STS_BUSY_Msk) {}
    if(FVC->BL2 != u32Ver)
        return -1;

    return 0;
}

/**
  * @brief      Set BL32 version
  * @param[in]  ver     Version Number. It could be 0~63.
  * @retval     0       Successful
  * @retval     -1      Failed
  * @details    Set version number of BL32
  *
  */
int32_t FVC_SetBL32Ver(uint32_t u32Ver)
{
    if(u32Ver >= 64)
        return -1;

    FVC->BL32 = (FVC->BL32 << 16) | (u32Ver & 0x3ful);
    while(FVC->STS & FVC_STS_BUSY_Msk) {}
    if(FVC->BL32 != u32Ver)
        return -1;

    return 0;
}


/**
  * @brief      Set BL33 version
  * @param[in]  ver     Version Number. It could be 0~255.
  * @retval     0       Successful
  * @retval     -1      Failed
  * @details    Set version number of BL33
  *
  */
int32_t FVC_SetBL33Ver(uint32_t u32Ver)
{
    if(u32Ver >= 256)
        return -1;

    FVC->BL33 = (FVC->BL33 << 16) | (u32Ver & 0xfful);
    while(FVC->STS & FVC_STS_BUSY_Msk) {}
    if(FVC->BL33 != u32Ver)
        return -1;

    return 0;
}


/**
  * @brief      Set version for user firmware
  * @param[in]  ver     Version Number. It could be 0~255.
  * @retval     0       Successful
  * @retval     -1      Failed
  * @details    Set version number of user firmware
  *
  */
int32_t FVC_SetUDFVer(uint32_t u32Ver)
{
    if(u32Ver >= 256)
        return -1;

    FVC->UDF = (FVC->UDF << 16) | (u32Ver & 0xfful);
    while(FVC->STS & FVC_STS_BUSY_Msk) {}
    if(FVC->UDF != u32Ver)
        return -1;

    return 0;
}


/**@}*/ /* end of group FVC_EXPORTED_FUNCTIONS */

/**@}*/ /* end of group FVC_Driver */

/**@}*/ /* end of group Standard_Driver */

