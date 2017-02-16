/******************************************************************************
 *
 * Copyright(c) 2007 - 2011 Realtek Corporation. All rights reserved.
 *                                        
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of version 2 of the GNU General Public License as
 * published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along with
 * this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110, USA
 *
 *
 ******************************************************************************/


#ifndef	__ROM_ODM_INTERFACE_H__
#define __ROM_ODM_INTERFACE_H__

#include "hal_api.h"

//
// =========== Macro Define
//
#define ODM_Read1Byte(pDM_Odm, RegAddr) HAL_READ8(WIFI_REG_BASE, RegAddr)
#define ODM_Read2Byte(pDM_Odm, RegAddr) HAL_READ16(WIFI_REG_BASE, RegAddr)
#define ODM_Read4Byte(pDM_Odm, RegAddr) HAL_READ32(WIFI_REG_BASE, RegAddr)
#define ODM_Write1Byte(pDM_Odm, RegAddr, Data) HAL_WRITE8(WIFI_REG_BASE, RegAddr, Data)
#define ODM_Write2Byte(pDM_Odm, RegAddr, Data) HAL_WRITE16(WIFI_REG_BASE, addr, value)
#define ODM_Write4Byte(pDM_Odm, RegAddr, Data) HAL_WRITE32(WIFI_REG_BASE, addr, value)
#if (RTL8195A_SUPPORT == 1)
#define ODM_GetMACReg(pDM_Odm,RegAddr, BitMask) PHY_QueryBBReg_8195A((pDM_Odm->Adapter), (RegAddr), (BitMask))
#define ODM_SetMACReg(pDM_Odm, RegAddr, BitMask, Data) PHY_SetBBReg_8195A((pDM_Odm->Adapter), (RegAddr), (BitMask), (Data))
#define ODM_GetBBReg(pDM_Odm, RegAddr, BitMask) PHY_QueryBBReg_8195A((pDM_Odm->Adapter), (RegAddr), (BitMask))
#define ODM_SetBBReg(pDM_Odm, RegAddr, BitMask, Data) PHY_SetBBReg_8195A((pDM_Odm->Adapter), (RegAddr), (BitMask), (Data))
#endif

#if (RTL8711B_SUPPORT ==1)
#define ODM_GetMACReg(pDM_Odm,RegAddr, BitMask) PHY_QueryBBReg_8711B((pDM_Odm->Adapter), (RegAddr), (BitMask))
#define ODM_SetMACReg(pDM_Odm, RegAddr, BitMask, Data) PHY_SetBBReg_8711B((pDM_Odm->Adapter), (RegAddr), (BitMask), (Data))
#define ODM_GetBBReg(pDM_Odm, RegAddr, BitMask) PHY_QueryBBReg_8711B((pDM_Odm->Adapter), (RegAddr), (BitMask))
#define ODM_SetBBReg(pDM_Odm, RegAddr, BitMask, Data) PHY_SetBBReg_8711B((pDM_Odm->Adapter), (RegAddr), (BitMask), (Data))
#endif
//
// =========== Extern Variable
//

//
// =========== EXtern Function Prototype
//


#endif	// __ROM_ODM_INTERFACE_H__

