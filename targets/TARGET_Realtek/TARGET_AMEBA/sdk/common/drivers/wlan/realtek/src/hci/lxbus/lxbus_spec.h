/******************************************************************************
 *
 * Copyright(c) 2007 - 2012 Realtek Corporation. All rights reserved.
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

#ifndef __LXBUS_SPEC_H__
#define __LXBUS_SPEC_H__

#include 	<lxbus_ops.h>
//#include 	<lxbus_hal.h>


#define 	HAL_INTERFACE_CMD_LEN				0
#define 	HAL_INTERFACE_CMD_STATUS_LEN   	0
#define 	HAL_INTERFACE_OVERHEAD			(HAL_INTERFACE_CMD_LEN+HAL_INTERFACE_CMD_STATUS_LEN)

/* 
  * The following data structure is used for 8195a debug, and should not
  * declared this parameter in release version to save sram usage
  * It is used for debugging tx/rx and r/w pointer
  */
struct hal_debug
{
	unsigned int			int_count;
	unsigned int			crc_err;
	u16					last_write_be;
	u16					last_write_mgt;
	u16					last_closed_be;
	u16					last_closed_mgt;

};


// The following section should be removed? 
#define WLAN_TX_HIQ_DEVICE_ID			4	// 0b[16], 100b[15:13]
#define WLAN_TX_MIQ_DEVICE_ID			5	// 0b[16], 101b[15:13]
#define WLAN_TX_LOQ_DEVICE_ID			6	// 0b[16], 110b[15:13]
#define SDIO_MAX_TX_QUEUE				3	// HIQ, MIQ and LOQ

#endif //__LXBUS_SPEC_H__
