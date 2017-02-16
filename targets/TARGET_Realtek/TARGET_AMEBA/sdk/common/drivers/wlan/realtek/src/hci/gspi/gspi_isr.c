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
 ******************************************************************************/
#define _GSPI_ISR_C_

#include <drv_types.h>

#ifdef CONFIG_GSPI_HCI

extern struct recv_buf* rtw_recv_rxfifo(_adapter * padapter, u32 size, struct fifo_more_data* more_data);
u8 spi_read8(struct dvobj_priv *pdvobj, u32 addr, s32 *err);
u16 spi_read16(struct dvobj_priv *pdvobj, u32 addr, s32 *err);
u32 spi_read32(struct dvobj_priv *pdvobj, u32 addr, s32 *err);
s32 spi_write8(struct dvobj_priv *pdvobj, u32 addr, u8 buf, s32 *err);
s32 spi_write16(struct dvobj_priv *pdvobj, u32 addr, u16 buf, s32 *err);
s32 spi_write32(struct dvobj_priv *pdvobj, u32 addr, u32 buf, s32 *err);

void spi_int_dpc(PADAPTER padapter, u32 sdio_hisr)
{
	struct dvobj_priv *pdvobj = padapter->dvobj;

#ifdef CONFIG_LPS_LCLK
	if (sdio_hisr & HCI_HISR_CPWM1)
	{
		struct reportpwrstate_parm report;

		report.state = spi_read8(pdvobj, LOCAL_REG_HCPWM1, NULL);
		if(report.state == 0xEA)
			report.state = PS_STATE_S0;
		else
			report.state = PS_STATE_S2;
		cpwm_int_hdl(padapter, &report);
	}
#endif

	if (sdio_hisr & HCI_HISR_TXERR)
	{
		u32 status;

		status = rtw_read32(padapter, REG_TXDMA_STATUS);
		rtw_write32(padapter, REG_TXDMA_STATUS, status);
		RT_TRACE(_module_hci_ops_c_, _drv_info_, ("%s: SDIO_HISR_TXERR (0x%08x)\n", __func__, status));
	}

#ifdef CONFIG_INTERRUPT_BASED_TXBCN

	#ifdef  CONFIG_INTERRUPT_BASED_TXBCN_EARLY_INT
	if (sdio_hisr & HCI_HISR_BCNERLY_INT)
	#endif
	#ifdef  CONFIG_INTERRUPT_BASED_TXBCN_BCN_OK_ERR
	if (sdio_hisr & (HCI_HISR_TXBCNOK|HCI_HISR_TXBCNERR))
	#endif
	{
		struct mlme_priv *pmlmepriv = &padapter->mlmepriv;

		#if 0 //for debug
		if (sdio_hisr & SDIO_HISR_BCNERLY_INT)
			DBG_8192C("%s: SDIO_HISR_BCNERLY_INT\n", __func__);

		if (sdio_hisr & SDIO_HISR_TXBCNOK)
			DBG_8192C("%s: SDIO_HISR_TXBCNOK\n", __func__);

		if (sdio_hisr & SDIO_HISR_TXBCNERR) {
			u1Byte v422, v550, v419;
			v422 = rtw_read8(padapter, 0x422);
			v419 = rtw_read8(padapter, 0x419);
			v550 = rtw_read8(padapter, 0x550);

			DBG_8192C("%s: SDIO_HISR_TXBCNERR 422=%02x, 419=%02x, 550=%02x\n", __func__, v422, v419, v550);
		}
		#endif

		if(check_fwstate(pmlmepriv, WIFI_AP_STATE))
		{
			//send_beacon(padapter);
			if(pmlmepriv->update_bcn == _TRUE)
			{
				//tx_beacon_hdl(padapter, NULL);
				set_tx_beacon_cmd(padapter);
			}
		}

#if 0//def CONFIG_CONCURRENT_MODE
		if(check_buddy_fwstate(padapter, WIFI_AP_STATE))
		{
			//send_beacon(padapter);
			if(padapter->pbuddy_adapter->mlmepriv.update_bcn == _TRUE)
			{
				//tx_beacon_hdl(padapter, NULL);
				set_tx_beacon_cmd(padapter->pbuddy_adapter);
			}
		}
#endif
	}
#endif //CONFIG_INTERRUPT_BASED_TXBCN

	if (sdio_hisr & HCI_HISR_C2HCMD)
	{
		RT_TRACE(_module_hci_ops_c_, _drv_info_, ("%s: C2H Command\n", __func__));
//TODO
//		rtw_c2h_wk_cmd(padapter);
	}

	if (sdio_hisr & HCI_HISR_RX_REQUEST)// || sdio_hisr & SPI_HISR_RXFOVW)
	{
		struct recv_buf *precvbuf;
		struct fifo_more_data more_data = {0};

		//RT_TRACE(_module_hci_ops_c_,_drv_info_, ("%s: RX Request, size=%d\n", __func__, pdvobj->SdioRxFIFOSize));
		
		sdio_hisr ^= HCI_HISR_RX_REQUEST;

		do {
			more_data.more_data = 0;
			more_data.len = 0;

			if (pdvobj->SdioRxFIFOSize == 0)
			{
				u16 val = 0;
				s32 ret;

				RT_TRACE(_module_hci_ops_c_, _drv_info_, ("%s, %d, read RXFIFOsize again size=%d\n", __FUNCTION__, __LINE__, pdvobj->SdioRxFIFOSize));
				
				val = spi_read16(pdvobj, LOCAL_REG_RX0_REQ_LEN_1_BYTE, &ret);
				if (!ret) {
					pdvobj->SdioRxFIFOSize = val;
					RT_TRACE(_module_hci_ops_c_, _drv_info_, ("%s: RX_REQUEST, read RXFIFOsize again size=%d\n", __func__, pdvobj->SdioRxFIFOSize));
				} else {
					RT_TRACE(_module_hci_ops_c_, _drv_info_, ("%s: RX_REQUEST, read RXFIFOsize ERROR!!\n", __func__));
				}
				RT_TRACE(_module_hci_ops_c_, _drv_info_, ("%s, %d, read RXFIFOsize again size=%d\n", __FUNCTION__, __LINE__, pdvobj->SdioRxFIFOSize));
			}

			if (pdvobj->SdioRxFIFOSize != 0)
			{
#ifdef RTL8723A_SDIO_LOOPBACK
				sd_recv_loopback(padapter, pdvobj->SdioRxFIFOSize);
#else
				if (sdio_hisr & HCI_HISR_RXFOVW)
					RT_TRACE(_module_hci_ops_c_, _drv_info_, ("%s RXFOVW RX\n", __func__));

				precvbuf = rtw_recv_rxfifo(padapter, pdvobj->SdioRxFIFOSize, &more_data);
				if (precvbuf)
					rtw_rxhandler(padapter, precvbuf);

				if (more_data.more_data) {
					pdvobj->SdioRxFIFOSize = more_data.len;
				} else {
					pdvobj->SdioRxFIFOSize = 0;
				}
#endif
				//If Rx_request ISR is set, execute receive tasklet (sdio_hisr & SPI_HISR_RX_REQUEST)
#if defined(CONFIG_ISR_THREAD_MODE_INTERRUPT) && defined(CONFIG_RECV_TASKLET_THREAD)
				rtw_wakeup_task(&padapter->recvtasklet_thread);
#endif
			}

#ifdef CONFIG_INTERRUPT_BASED_TXBCN
{
			//Prevent BCN update not realtime in ap mode - Alex Fang
			struct mlme_priv *pmlmepriv = &padapter->mlmepriv;

			if((check_fwstate(pmlmepriv, WIFI_AP_STATE) == _TRUE) && (pmlmepriv->update_bcn == _TRUE))
				break;
}
#endif
		} while (more_data.more_data);
#ifdef PLATFORM_LINUX
#ifdef CONFIG_GSPI_HCI
		tasklet_schedule(&padapter->recvpriv.recv_tasklet);
#endif
#endif
	}
}


void spi_int_hdl(PADAPTER padapter)
{
	struct dvobj_priv *pdvobj = padapter->dvobj;
	u32 sdio_hisr = 0;
	s32 ret;

	if ((padapter->bDriverStopped == _TRUE) ||
	    (padapter->bSurpriseRemoved == _TRUE))
		return;
	
	sdio_hisr = spi_read32(pdvobj, LOCAL_REG_HISR, &ret);
	if (!ret) {
		RT_TRACE(_module_hci_ops_c_, _drv_err_, ("%s: read SDIO_REG_HISR FAIL!!\n", __func__));
		return;
	}
	pdvobj->SdioRxFIFOSize = spi_read16(pdvobj, LOCAL_REG_RX0_REQ_LEN_1_BYTE, &ret);
	
	if (!ret) {
		RT_TRACE(_module_hci_ops_c_, _drv_err_, ("%s: read SPI_REG_RX0_REQ_LEN FAIL!!\n", __func__));
		return;
	}

	if (sdio_hisr & pdvobj->sdio_himr)
	{
		u32 v32;

		sdio_hisr &= pdvobj->sdio_himr;

		// clear HISR
		v32 = sdio_hisr & MASK_SPI_HISR_CLEAR;
		if (v32) {
			spi_write32(pdvobj, LOCAL_REG_HISR, v32, &ret);
		}

		spi_int_dpc(padapter, sdio_hisr);
	} else {
		RT_TRACE(_module_hci_ops_c_, _drv_err_,
				("%s: HISR(0x%08x) and HIMR(0x%08x) not match!\n",
				__FUNCTION__, sdio_hisr, pdvobj->sdio_himr));
		if(sdio_hisr)
			spi_write32(pdvobj, LOCAL_REG_HISR, sdio_hisr, &ret);
	}
}

#endif
