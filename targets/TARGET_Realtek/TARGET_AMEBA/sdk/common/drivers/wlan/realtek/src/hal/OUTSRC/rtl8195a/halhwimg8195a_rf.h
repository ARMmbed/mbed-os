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

/*Image2HeaderVersion: 2.23*/
#if (RTL8195A_SUPPORT == 1)
#ifndef __INC_MP_RF_HW_IMG_8195A_H
#define __INC_MP_RF_HW_IMG_8195A_H


/******************************************************************************
*                           RadioA.TXT
******************************************************************************/

void
ODM_ReadAndConfig_MP_8195A_RadioA(/* TC: Test Chip, MP: MP Chip*/
	IN   PDM_ODM_T  pDM_Odm
);
u4Byte ODM_GetVersion_MP_8195A_RadioA(void);

/******************************************************************************
*                           RADIO_DIFF.TXT
******************************************************************************/

extern u4Byte Array_MP_8195A_RADIO_DIFF_LB[50];
extern u4Byte Array_MP_8195A_RADIO_DIFF_MB[50];
extern u4Byte Array_MP_8195A_RADIO_DIFF_HB[50];
void
ODM_ReadAndConfig_MP_8195A_RADIO_DIFF(
     IN   PDM_ODM_T    pDM_Odm,
 	 IN   u4Byte  	   Array[],
 	 IN   u4Byte  	   ArrayLen 
);
u4Byte ODM_GetVersion_MP_8195A_RADIO_DIFF(void);

/******************************************************************************
*                           TxPowerTrack_QFN48.TXT
******************************************************************************/

void
ODM_ReadAndConfig_MP_8195A_TxPowerTrack_QFN48(/* TC: Test Chip, MP: MP Chip*/
	IN   PDM_ODM_T  pDM_Odm
);
u4Byte ODM_GetVersion_MP_8195A_TxPowerTrack_QFN48(void);

/******************************************************************************
*                           TxPowerTrack_QFN56.TXT
******************************************************************************/

void
ODM_ReadAndConfig_MP_8195A_TxPowerTrack_QFN56(/* TC: Test Chip, MP: MP Chip*/
	IN   PDM_ODM_T  pDM_Odm
);
u4Byte ODM_GetVersion_MP_8195A_TxPowerTrack_QFN56(void);

/******************************************************************************
*                           TxPowerTrack_TFBGA96.TXT
******************************************************************************/

void
ODM_ReadAndConfig_MP_8195A_TxPowerTrack_TFBGA96(/* TC: Test Chip, MP: MP Chip*/
	IN   PDM_ODM_T  pDM_Odm
);
u4Byte ODM_GetVersion_MP_8195A_TxPowerTrack_TFBGA96(void);

/******************************************************************************
*                           TXPWR_LMT.TXT
******************************************************************************/

void
ODM_ReadAndConfig_MP_8195A_TXPWR_LMT(/* TC: Test Chip, MP: MP Chip*/
	IN   PDM_ODM_T  pDM_Odm
);
u4Byte ODM_GetVersion_MP_8195A_TXPWR_LMT(void);

/******************************************************************************
*                           TxXtalTrack.TXT
******************************************************************************/

void
ODM_ReadAndConfig_MP_8195A_TxXtalTrack(/* TC: Test Chip, MP: MP Chip*/
	IN   PDM_ODM_T  pDM_Odm
);
u4Byte ODM_GetVersion_MP_8195A_TxXtalTrack(void);

#endif
#endif /* end of HWIMG_SUPPORT*/

