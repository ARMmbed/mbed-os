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
#define _GSPI_INTF_C_

#include <drv_types.h>

#ifdef CONFIG_SDIO_HCI
#include "wifi_io.h" //from sdio_driver

#ifndef CONFIG_SDIO_HCI
#error "CONFIG_SDIO_HCI should be on!\n"
#endif

struct dvobj_priv *gspi_dvobj_init(void)
{
//	int status = _FAIL;
	struct dvobj_priv *dvobj = NULL;
	PGSPI_DATA pgspi_data;

_func_enter_;

	dvobj = (struct dvobj_priv*)rtw_zmalloc(sizeof(*dvobj));
	if (NULL == dvobj) {
		goto exit;
	}

	pgspi_data = &dvobj->intf_data;
	
	rtw_mutex_init(&pgspi_data->spi_mutex);	
	//pgspi_data->block_transfer_len = 512; //512 blocks r/w is not required for GSPI interface
	//pgspi_data->tx_block_mode = 0;
	//pgspi_data->rx_block_mode = 0;

//	status = _SUCCESS;

	if(wifi_sdio_func) {
		DBG_871X("[gspi_dvobj_init] get wifi_func:%p\n", wifi_sdio_func);	
		dvobj->intf_data.func = wifi_sdio_func;
	} else {		
		DBG_871X("[gspi_dvobj_init] Oops: get wifi sdio function fail");
	}

exit:
_func_exit_;

	return dvobj;
}

void gspi_dvobj_deinit(struct dvobj_priv *dvobj)
{
//TODO
//	struct dvobj_priv *dvobj = spi_get_drvdata(spi);

_func_enter_;
//TODO
//	spi_set_drvdata(spi, NULL);
	if (dvobj) {
//TODO
//		gspi_deinit(dvobj);
		rtw_mutex_free(&dvobj->intf_data.spi_mutex);	
		rtw_mfree((u8*)dvobj, sizeof(*dvobj));
	}

_func_exit_;
}

void sdio_dvobj_interrupt_entry(struct sdio_func *func)
{
	//DBG_871X("[sdio_wifi_interrupt_entry] func :%p\n", func);	
	
	//sdio irq have claim host, we should release it
	//and claim it after SDIO IO, or SDIO IO will deadlock
	rtw_sdio_bus_ops.release_host(func);
	rtw_hci_interrupt_handler(func->drv_priv);
	rtw_sdio_bus_ops.claim_host(func);
}

void sdio_dvobj_request_irq(struct dvobj_priv *dvobj)
{
_func_enter_;
	if(dvobj->intf_data.func) {
		dvobj->intf_data.func->drv_priv = (void*)dvobj->if1;

		rtw_sdio_bus_ops.claim_host(dvobj->intf_data.func);
		rtw_sdio_bus_ops.claim_irq(dvobj->intf_data.func,  sdio_dvobj_interrupt_entry);
		rtw_sdio_bus_ops.release_host(dvobj->intf_data.func);
	}

_func_exit_;
}

void sdio_dvobj_free_irq(struct dvobj_priv *dvobj)
{
_func_enter_;
	if(dvobj->intf_data.func) {
		dvobj->intf_data.func->drv_priv = (void*)dvobj->if1;

		rtw_sdio_bus_ops.claim_host(dvobj->intf_data.func);
		rtw_sdio_bus_ops.release_irq(dvobj->intf_data.func);
		rtw_sdio_bus_ops.release_host(dvobj->intf_data.func);
	}

_func_exit_;
}


static inline u32 ffaddr2deviceId(struct dvobj_priv *pdvobj, u32 addr)
{
	return pdvobj->Queue2Pipe[addr];
}

static s32 rtw_xmit_xmitbuf(_adapter * padapter, struct xmit_buf *pxmitbuf)
{
	struct xmit_priv *pxmitpriv = &padapter->xmitpriv;
	struct dvobj_priv	*pdvobjpriv = adapter_to_dvobj(padapter);
	u32 deviceId;

	RT_TRACE(_module_rtl871x_xmit_c_, _drv_info_, ("@@@rtw_xmit_xmitbuf(): pxmitbuf->len=%d\n", pxmitbuf->len));
	//translate queue index to Device Id
	deviceId = ffaddr2deviceId(pdvobjpriv, pxmitbuf->ff_hwaddr);
	
	rtw_write_port(padapter, deviceId, pxmitbuf->len, (u8*)pxmitbuf->pbuf);

	rtw_free_xmitbuf(pxmitpriv, pxmitbuf);
	return _SUCCESS;
}

s32 sdio_dvobj_xmit_mgnt(_adapter * padapter, struct xmit_frame *pmgntframe)
{
	s32 ret = _SUCCESS;
	struct pkt_attrib	*pattrib;
	struct xmit_buf	*pxmitbuf;
	struct xmit_priv	*pxmitpriv = &padapter->xmitpriv;
	struct	mlme_priv	*pmlmepriv = &(padapter->mlmepriv);
	struct dvobj_priv	*pdvobjpriv = adapter_to_dvobj(padapter);
	u8	*pframe = NULL;
	
	RT_TRACE(_module_rtl871x_xmit_c_, _drv_info_, ("+rtw_xmit_mgnt()\n"));

	pattrib = &pmgntframe->attrib;
	pxmitbuf = pmgntframe->pxmitbuf;
	pframe = (u8 *)(pmgntframe->buf_addr) + TXDESC_OFFSET;

	//rtw_hal_update_txdesc(padapter, pmgntframe, pmgntframe->buf_addr);

	pxmitbuf->len = TXDESC_SIZE + pattrib->last_txcmdsz;
	//pxmitbuf->pg_num = (pxmitbuf->len + 127)/128; // 128 is tx page size
	//pxmitbuf->ptail = pmgntframe->buf_addr + pxmitbuf->len;
	pxmitbuf->ff_hwaddr = rtw_get_ff_hwaddr(pmgntframe);

	rtw_count_tx_stats(padapter, pmgntframe, pattrib->last_txcmdsz);

	//RT_TRACE(_module_rtl871x_xmit_c_, _drv_always_, ("+rtw_xmit_mgnt(): type=%d\n", GetFrameSubType(pframe)));
	if(GetFrameSubType(pframe)==WIFI_BEACON) //dump beacon directly
	{
//When using dedicated xmit frame for issue bcn on ap mode
//free xmit frame for bcn reserved page on station mode - Alex Fang
#if USE_DEDICATED_BCN_TX
		if(check_fwstate(pmlmepriv, WIFI_AP_STATE) != _TRUE) {
			rtw_free_xmitframe(pxmitpriv, pmgntframe);
			pxmitbuf->priv_data = NULL;
		}
		rtw_write_port(padapter, ffaddr2deviceId(pdvobjpriv, pxmitbuf->ff_hwaddr), pxmitbuf->len, pxmitbuf->pbuf);

		if(check_fwstate(pmlmepriv, WIFI_AP_STATE) != _TRUE)
			rtw_free_xmitbuf(pxmitpriv, pxmitbuf);
#else
		rtw_free_xmitframe(pxmitpriv, pmgntframe);
		pxmitbuf->priv_data = NULL;
		rtw_xmit_xmitbuf(padapter, pxmitbuf);
#endif
	}		
	else
	{
		rtw_free_xmitframe(pxmitpriv, pmgntframe);
		pxmitbuf->priv_data = NULL;
		rtw_xmit_xmitbuf(padapter, pxmitbuf);
	}

	if  (ret != _SUCCESS)
		rtw_sctx_done_err(&pxmitbuf->sctx, RTW_SCTX_DONE_UNKNOWN);

	RT_TRACE(_module_rtl871x_xmit_c_, _drv_info_, ("-rtw_xmit_mgnt\n"));
	return ret;
}

s32 sdio_dvobj_xmit_data(_adapter *padapter, struct xmit_frame *pxmitframe)
{
	struct xmit_priv 	*pxmitpriv = &padapter->xmitpriv;
	_irqL irql;
	s32 err;

#ifdef CONFIG_80211N_HT
	if ((pxmitframe->frame_tag == DATA_FRAMETAG) &&
		(pxmitframe->attrib.ether_type != 0x0806) &&
		(pxmitframe->attrib.ether_type != 0x888e) &&
		(pxmitframe->attrib.dhcp_pkt != 1))
	{
		if (padapter->mlmepriv.LinkDetectInfo.bBusyTraffic == _TRUE)
			rtw_issue_addbareq_cmd(padapter, pxmitframe);
	}
#endif

#if USE_SKB_AS_XMITBUF
	rtw_xmitframe_coalesce(padapter, pxmitframe->pkt, pxmitframe);
#endif

	rtw_enter_critical_bh(&pxmitpriv->lock, &irql);
#if 1 //FIX_XMITFRAME_FAULT, move from rtw_xmit(). 
#ifdef CONFIG_AP_MODE
	if(xmitframe_enqueue_for_sleeping_sta(padapter, pxmitframe) == _TRUE)
	{
		rtw_exit_critical_bh(&pxmitpriv->lock, &irql);
		return 1;		
	}
#endif
#endif
	err = rtw_xmitframe_enqueue(padapter, pxmitframe);
	rtw_exit_critical_bh(&pxmitpriv->lock, &irql);
	
	if (err != _SUCCESS) {
		RT_TRACE(_module_rtl871x_xmit_c_, _drv_err_, ("rtw_xmit_data(): enqueue xmitframe fail\n"));
		rtw_free_xmitframe(pxmitpriv, pxmitframe);

		// Trick, make the statistics correct
		pxmitpriv->tx_pkts--;
		pxmitpriv->tx_drop++;
		return _TRUE;
	}
#ifdef CONFIG_CONCURRENT_MODE
	if (padapter->adapter_type > PRIMARY_ADAPTER){
		padapter = padapter->pbuddy_adapter;
	}
#endif

	rtw_wakeup_task(&padapter->xmitThread);

	return _FALSE;
}

const struct host_ctrl_intf_ops hci_ops = {
	gspi_dvobj_init,
	gspi_dvobj_deinit,
	sdio_dvobj_request_irq,
	sdio_dvobj_free_irq
};
#endif
