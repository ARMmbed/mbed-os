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
#ifndef _RTW_INTFS_H_
#define _RTW_INTFS_H_

extern u8 rtw_init_default_value(_adapter *padapter);
#ifdef CONFIG_WOWLAN
void rtw_cancel_dynamic_chk_timer(_adapter *padapter);
#endif
extern void rtw_cancel_all_timer(_adapter *padapter);

extern u8 rtw_init_drv_sw(_adapter *padapter);
extern u8 rtw_free_drv_sw(_adapter *padapter);
extern u8 rtw_reset_drv_sw(_adapter *padapter);

extern int rtw_drv_init(ADAPTER *padapter);
extern void rtw_drv_deinit(ADAPTER *Adapter);

extern u32 rtw_start_drv_threads(_adapter *padapter);
extern void rtw_stop_drv_threads (_adapter *padapter);

#endif //_RTW_INTFS_H_
