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
#ifndef __RTW_EFUSE_H__
#define __RTW_EFUSE_H__

#if (RTL8195A_SUPPORT == 1)
#include "rtl8195a.h"
#endif
#if (RTL8711B_SUPPORT == 1)
#include "ameba_soc.h"
#endif
/*--------------------------Define efuse Parameters-------------------------*/
#define	EFUSE_ERROE_HANDLE		1

#define	PG_STATE_HEADER 		0x01
#define	PG_STATE_WORD_0			0x02
#define	PG_STATE_WORD_1			0x04
#define	PG_STATE_WORD_2			0x08
#define	PG_STATE_WORD_3			0x10
#define	PG_STATE_DATA			0x20

#define	PG_SWBYTE_H				0x01
#define	PG_SWBYTE_L				0x02

#define	PGPKT_DATA_SIZE			8

#define	EFUSE_WIFI				0
#define	EFUSE_BT				1

enum _EFUSE_DEF_TYPE {
	TYPE_EFUSE_MAX_SECTION				= 0,
	TYPE_EFUSE_REAL_CONTENT_LEN			= 1,
	TYPE_AVAILABLE_EFUSE_BYTES_BANK		= 2,
	TYPE_AVAILABLE_EFUSE_BYTES_TOTAL	= 3,
	TYPE_EFUSE_MAP_LEN					= 4,
	TYPE_EFUSE_PROTECT_BYTES_BANK		= 5,
	TYPE_EFUSE_CONTENT_LEN_BANK			= 6,
};

#define EFUSE_MAP_SIZE			512
#define EFUSE_MAX_SIZE			256

#define	EFUSE_MAX_MAP_LEN		512
#define	EFUSE_MAX_HW_SIZE		256
#define	EFUSE_MAX_SECTION_BASE	16

#define EXT_HEADER(header) ((header & 0x1F ) == 0x0F)
#define ALL_WORDS_DISABLED(wde)	((wde & 0x0F) == 0x0F)
#define GET_HDR_OFFSET_2_0(header) ( (header & 0xE0) >> 5)

#define		EFUSE_REPEAT_THRESHOLD_		3
#define		EFUSE_MAX_WORD_UNIT			4

//=============================================
//	The following is for BT Efuse definition
//=============================================
#define		EFUSE_BT_MAX_MAP_LEN	1024
#define		EFUSE_MAX_BANK			4
#define		EFUSE_MAX_BT_BANK		(EFUSE_MAX_BANK-1)
//=============================================

/*--------------------------Define flash Parameters-------------------------*/
#if CONFIG_ADAPTOR_INFO_CACHING_FLASH
#if defined CONFIG_RTL8195A || defined(CONFIG_RTL8711B)
	#define FLASH_MAX_SIZE			FLASH_CAL_DATA_SIZE
	#define FLASH_MAGIC_NUMBER		0x8195 // magic number
	#define FLASH_HEADER_SIZE		4 // 2: address, 2: length
#endif
#endif // CONFIG_ADAPTOR_INFO_CACHING_FLASH
/*------------------------------Define structure----------------------------*/ 
typedef struct PG_PKT_STRUCT_A{
	u8 offset;
	u8 word_en;
	u8 data[8];	
	u8 word_cnts;
}PGPKT_STRUCT,*PPGPKT_STRUCT;

#ifdef HAL_EFUSE_MEMORY
typedef struct _EFUSE_HAL{
	u8	fakeEfuseBank;
	u32	fakeEfuseUsedBytes;
	u8	fakeEfuseContent[EFUSE_MAX_HW_SIZE];
	u8	fakeEfuseInitMap[EFUSE_MAX_MAP_LEN];
	u8	fakeEfuseModifiedMap[EFUSE_MAX_MAP_LEN];
	
	u16	BTEfuseUsedBytes;
	u8	BTEfuseUsedPercentage;
	u8	BTEfuseContent[EFUSE_MAX_BT_BANK][EFUSE_MAX_HW_SIZE];
	u8	BTEfuseInitMap[EFUSE_BT_MAX_MAP_LEN];
	u8	BTEfuseModifiedMap[EFUSE_BT_MAX_MAP_LEN];

	u16	fakeBTEfuseUsedBytes;
	u8	fakeBTEfuseContent[EFUSE_MAX_BT_BANK][EFUSE_MAX_HW_SIZE];
	u8	fakeBTEfuseInitMap[EFUSE_BT_MAX_MAP_LEN];
	u8	fakeBTEfuseModifiedMap[EFUSE_BT_MAX_MAP_LEN];
}EFUSE_HAL, *PEFUSE_HAL;
#endif // HAL_EFUSE_MEMORY

/*------------------------Export global variable----------------------------*/
#if CONFIG_FAKE_EFUSE
extern u8 fakeEfuseBank;
extern u32 fakeEfuseUsedBytes;
extern u8 fakeEfuseContent[];
extern u8 fakeEfuseInitMap[];
extern u8 fakeEfuseModifiedMap[];
#endif

#ifdef CONFIG_BT_COEXIST
extern u32 BTEfuseUsedBytes;
extern u8 BTEfuseContent[EFUSE_MAX_BT_BANK][EFUSE_MAX_HW_SIZE];
extern u8 BTEfuseInitMap[];
extern u8 BTEfuseModifiedMap[];
#if CONFIG_FAKE_EFUSE
extern u32 fakeBTEfuseUsedBytes;
extern u8 fakeBTEfuseContent[EFUSE_MAX_BT_BANK][EFUSE_MAX_HW_SIZE];
extern u8 fakeBTEfuseInitMap[];
extern u8 fakeBTEfuseModifiedMap[];
#endif
#endif

/*------------------------Export global variable----------------------------*/

u8	efuse_GetCurrentSize(_adapter * padapter, u16 *size);
u8	rtw_efuse_access(_adapter * padapter, u8 bRead, u16 start_addr, u16 cnts, u8 *data);
u8	rtw_efuse_map_read(_adapter * padapter, u16 addr, u16 cnts, u8 *data);
u8	rtw_efuse_map_write(_adapter * padapter, u16 addr, u16 cnts, u8 *data);
u8	rtw_BT_efuse_map_read(_adapter * padapter, u16 addr, u16 cnts, u8 *data);
u8 	rtw_BT_efuse_map_write(_adapter * padapter, u16 addr, u16 cnts, u8 *data);

u16	Efuse_GetCurrentSize(_adapter * pAdapter, u8 efuseType, BOOLEAN bPseudoTest);
u8	Efuse_CalculateWordCnts(u8 word_en);
void	EFUSE_GetEfuseDefinition(_adapter * pAdapter, u8 efuseType, u8 type, void *pOut, BOOLEAN bPseudoTest);
u8	efuse_OneByteRead(_adapter * pAdapter, u16 addr, u8 *data, BOOLEAN	 bPseudoTest);
u8	efuse_OneByteWrite(_adapter * pAdapter, u16 addr, u8 data, BOOLEAN	 bPseudoTest);

void	Efuse_PowerSwitch(_adapter * pAdapter,u8	bWrite,u8	 PwrState);
//int 	Efuse_PgPacketRead(_adapter * pAdapter, u8 offset, u8 *data, BOOLEAN bPseudoTest);
int 	Efuse_PgPacketWrite(_adapter * pAdapter, u8 offset, u8 word_en, u8 *data, BOOLEAN bPseudoTest);
void	efuse_WordEnableDataRead(u8 word_en, u8 *sourdata, u8 *targetdata);
u8	Efuse_WordEnableDataWrite(_adapter * pAdapter, u16 efuse_addr, u8 word_en, u8 *data, BOOLEAN bPseudoTest);

void	EFUSE_ShadowMapUpdate(_adapter * pAdapter, u8 efuseType, BOOLEAN bPseudoTest);
void	EFUSE_ShadowRead(_adapter * pAdapter, u8 Type, u16 Offset, u32 *Value);

#endif

