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
#ifndef _HCI_INTFS_H_
#define _HCI_INTFS_H_

#include <autoconf.h>

struct host_ctrl_intf_ops 
{
	struct dvobj_priv * (*dvobj_init)(void);
	void (*dvobj_deinit)(struct dvobj_priv *dvobj);
	void (*dvobj_request_irq)(struct dvobj_priv *dvobj);
	void (*dvobj_free_irq)(struct dvobj_priv *dvobj);
};

extern struct dvobj_priv *hci_dvobj_init(void);
extern void hci_dvobj_deinit(struct dvobj_priv *dvobj);
extern void hci_dvobj_request_irq(struct dvobj_priv *dvobj);
extern void hci_dvobj_free_irq(struct dvobj_priv *dvobj);

#if defined(CONFIG_GSPI_HCI)
#define hci_bus_intf_type RTW_GSPI
#define hci_set_intf_ops spi_set_intf_ops
#define hci_intf_start	rtw_hal_enable_interrupt
#define hci_intf_stop	rtw_hal_disable_interrupt
extern s32 gspi_dvobj_xmit_mgnt(_adapter * padapter, struct xmit_frame *pmgntframe);
extern s32 gspi_dvobj_xmit_data(_adapter *padapter, struct xmit_frame *pxmitframe);
#endif

#if defined(CONFIG_SDIO_HCI) 
#define hci_bus_intf_type RTW_SDIO
#define hci_set_intf_ops sdio_set_intf_ops
#define hci_intf_start	rtw_hal_enable_interrupt
#define hci_intf_stop	rtw_hal_disable_interrupt
extern s32 sdio_dvobj_xmit_mgnt(_adapter * padapter, struct xmit_frame *pmgntframe);
extern s32 sdio_dvobj_xmit_data(_adapter *padapter, struct xmit_frame *pxmitframe);

#endif

#if defined(CONFIG_LX_HCI)
#define hci_bus_intf_type RTW_LXBUS
#define hci_set_intf_ops lxbus_set_intf_ops
#define hci_intf_start	rtw_hal_enable_interrupt
#define hci_intf_stop	hci_lxbus_intf_stop
void hci_lxbus_intf_stop(_adapter *padapter);
u32 lextra_bus_dma_Interrupt (void* data);
#endif


#endif //_HCI_INTFS_H_

