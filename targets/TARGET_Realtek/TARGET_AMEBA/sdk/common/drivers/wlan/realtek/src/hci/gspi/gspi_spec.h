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
 *******************************************************************************/

#ifndef __GSPI_SPEC_H__
#define __GSPI_SPEC_H__


#define SPI_LOCAL_DOMAIN 				0x0
#define WLAN_IOREG_DOMAIN 			0x8
#define FW_FIFO_DOMAIN 				0x4
#define TX_HIQ_DOMAIN 					0xc
#define TX_MIQ_DOMAIN 					0xd
#define TX_LOQ_DOMAIN 					0xe
#define RX_RXFIFO_DOMAIN 				0x1f

//IO Bus domain address mapping
#define DEFUALT_OFFSET					0x0
#define LOCAL_OFFSET    					0x10250000
#define SPI_LOCAL_OFFSET    				0x10250000
#define WLAN_IOREG_OFFSET   			0x10260000
#define FW_FIFO_OFFSET 	    			0x10270000
#define TX_HIQ_OFFSET	    				0x10310000
#define TX_MIQ_OFFSET					0x1032000
#define TX_LOQ_OFFSET					0x10330000
#define RX_RXOFF_OFFSET	    			0x10340000

#define WLAN_TX_HIQ_DEVICE_ID			4	// 0b[16], 100b[15:13]
#define WLAN_TX_MIQ_DEVICE_ID 		5	// 0b[16], 101b[15:13]
#define WLAN_TX_LOQ_DEVICE_ID 		6	// 0b[16], 110b[15:13]
#define WLAN_TX_EXQ_DEVICE_ID		3	// 0b[16], 011b[15:13]
#define WLAN_RX0FF_DEVICE_ID 			7	// 0b[16], 111b[15:13]
#define WLAN_IOREG_DEVICE_ID 			8	// 1b[16]

//SPI Tx Free Page Index
#define HI_QUEUE_IDX           			0
#define MID_QUEUE_IDX         			1
#define LOW_QUEUE_IDX 	      			2
#define PUBLIC_QUEUE_IDX    				3

#define MAX_TX_QUEUE			3		// HIQ, MIQ and LOQ
#define MAX_RX_QUEUE			1

//SPI Local registers
#ifdef CONFIG_RTL8188F
#define SPI_REG_TX_CTRL					0x0000 // SPI Tx Control
#define SPI_REG_STATUS_RECOVERY		0x0004
#define SPI_REG_INT_TIMEOUT		   	0x0006
#define SPI_REG_HIMR					0x0014 // SPI Host Interrupt Mask
#define SPI_REG_HISR					0x0018 // SPI Host Interrupt Service Routine
#define SPI_REG_RX0_REQ_LEN			0x001C // RXDMA Request Length
#define SPI_REG_FREE_TXPG				0x0020 // Free Tx Buffer Page
#define SPI_REG_HTSFR_INFO				0x0030 // HTSF Informaion
#define SPI_REG_HCPWM1					0x0038 // HCI Current Power Mode 1
#define SPI_REG_HCPWM2					0x003A // HCI Current Power Mode 2
#define SPI_REG_HRPWM1					0x0080 // HCI Request Power Mode 1 need check for 8188f???
#define SPI_REG_HPS_CLKR				0x0084 // HCI Power Save Clock
#define SPI_REG_HSUS_CTRL				0x0086 // SPI HCI Suspend Control
#else
#define SPI_REG_TX_CTRL					0x0000 // SPI Tx Control
#define SPI_REG_STATUS_RECOVERY		0x0004
#define SPI_REG_INT_TIMEOUT		   	0x0006
#define SPI_REG_HIMR					0x0014 // SPI Host Interrupt Mask
#define SPI_REG_HISR					0x0018 // SPI Host Interrupt Service Routine
#define SPI_REG_RX0_REQ_LEN			0x001C // RXDMA Request Length
#define SPI_REG_FREE_TXPG				0x0020 // Free Tx Buffer Page
#define SPI_REG_HCPWM1					0x0024 // HCI Current Power Mode 1
#define SPI_REG_HCPWM2					0x0026 // HCI Current Power Mode 2
#define SPI_REG_HTSFR_INFO				0x0030 // HTSF Informaion
#define SPI_REG_HRPWM1					0x0080 // HCI Request Power Mode 1
#define SPI_REG_HRPWM2					0x0082 // HCI Request Power Mode 2
#define SPI_REG_HPS_CLKR				0x0084 // HCI Power Save Clock
#define SPI_REG_HSUS_CTRL				0x0086 // SPI HCI Suspend Control
#define SPI_REG_HIMR_ON				0x0090 //SPI Host Extension Interrupt Mask Always
#define SPI_REG_HISR_ON				0x0091 //SPI Host Extension Interrupt Status Always
#define SPI_REG_CFG						0x00F0 //SPI Configuration Register
#endif 

#define LOCAL_REG_TX_CTRL 	               			(SPI_REG_TX_CTRL  |SPI_LOCAL_OFFSET)
#define LOCAL_REG_STATUS_RECOVERY		   	(SPI_REG_STATUS_RECOVERY  |SPI_LOCAL_OFFSET)
#define LOCAL_REG_INT_TIMEOUT		   	   		(SPI_REG_INT_TIMEOUT  |SPI_LOCAL_OFFSET)
#define LOCAL_REG_HIMR 	                  			(SPI_REG_HIMR |SPI_LOCAL_OFFSET)
#define LOCAL_REG_HISR 	                   			(SPI_REG_HISR |SPI_LOCAL_OFFSET)
#define LOCAL_REG_RX0_REQ_LEN_1_BYTE 	   	(SPI_REG_RX0_REQ_LEN |SPI_LOCAL_OFFSET)
#define LOCAL_REG_FREE_TXPG 	               		(SPI_REG_FREE_TXPG |SPI_LOCAL_OFFSET)
#define LOCAL_REG_HRPWM1						(SPI_REG_HRPWM1 |SPI_LOCAL_OFFSET)
#define LOCAL_REG_HCPWM1						(SPI_REG_HCPWM1 |SPI_LOCAL_OFFSET)
#define LOCAL_REG_SUSPEND_NORMAL                   (SPI_REG_HSUS_CTRL|SPI_LOCAL_OFFSET)
#define HCI_HIMR_DISABLED				0

//SPI HIMR MASK diff with SDIO
#ifdef CONFIG_RTL8188F
#define HCI_HISR_RX_REQUEST    			BIT(0)
#define HCI_HISR_AVAL					BIT(1)
#define HCI_HISR_TXERR					BIT(2)
#define HCI_HISR_RXERR					BIT(3)
#define HCI_HISR_TXFOVW				BIT(4)
#define HCI_HISR_RXFOVW				BIT(5)
#define HCI_HISR_TXBCNOK				BIT(6)
#define HCI_HISR_TXBCNERR				BIT(7)
#define HCI_HISR_BCNERLY_INT			BIT(16)
#define HCI_HISR_C2HCMD				BIT(17)
#define HCI_HISR_CPWM1					BIT(18)
#define HCI_HISR_CPWM2					BIT(19)
#define HCI_HISR_HSISR_IND				BIT(20)
#define HCI_HISR_GTINT3_IND			BIT(21)
#define HCI_HISR_GTINT4_IND			BIT(22)
#define HCI_HISR_PSTIMEOUT				BIT(23)
#define HCI_HISR_OCPINT					BIT(24)
#define HCI_HISR_ATIMEND				BIT(25)
#define HCI_HISR_ATIMEND_E				BIT(26)
#define HCI_HISR_CTWEND				BIT(27)
#define HCI_HISR_TSF_BIT32_TOGGLE		BIT(29)
#define HCI_HISR_PSTIMEOUT_E			BIT(30)
//SPI HIMR MASK diff with SDIO
#define HCI_HIMR_RX_REQUEST    			BIT(0)
#define HCI_HIMR_AVAL					BIT(1)
#define HCI_HIMR_TXERR					BIT(2)
#define HCI_HIMR_RXERR					BIT(3)
#define HCI_HIMR_TXFOVW				BIT(4)
#define HCI_HIMR_RXFOVW				BIT(5)
#define HCI_HIMR_TXBCNOK				BIT(6)
#define HCI_HIMR_TXBCNERR				BIT(7)
#define HCI_HIMR_BCNERLY_INT			BIT(16)
#define HCI_HIMR_C2HCMD				BIT(17)
#define HCI_HIMR_CPWM1					BIT(18)
#define HCI_HIMR_CPWM2					BIT(19)
#define HCI_HIMR_HSISR_IND				BIT(20)
#define HCI_HIMR_GTINT3_IND			BIT(21)
#define HCI_HIMR_GTINT4_IND			BIT(22)
#define HCI_HIMR_PSTIMEOUT				BIT(23)
#define HCI_HIMR_OCPINT				BIT(24)
#define HCI_HIMR_ATIMEND				BIT(25)
#define HCI_HIMR_ATIMEND_E				BIT(26)
#define HCI_HIMR_CTWEND				BIT(27)
#define HCI_HIMR_TSF_BIT32_TOGGLE		BIT(29)
#define HCI_HIMR_PSTIMEOUT_E			BIT(30)
#else
#define HCI_HISR_RX_REQUEST    			BIT(0)
#define HCI_HISR_AVAL					BIT(1)
#define HCI_HISR_TXERR					BIT(2)
#define HCI_HISR_RXERR					BIT(3)
#define HCI_HISR_TXFOVW				BIT(4)
#define HCI_HISR_RXFOVW				BIT(5)
#define HCI_HISR_TXBCNOK				BIT(6)
#define HCI_HISR_TXBCNERR				BIT(7)
#define HCI_HISR_BCNERLY_INT			BIT(16)
#define HCI_HISR_ATIMEND				BIT(17)
#define HCI_HISR_ATIMEND_E				BIT(18)
#define HCI_HISR_CTWEND				BIT(19)
#define HCI_HISR_C2HCMD				BIT(20)
#define HCI_HISR_CPWM1					BIT(21)
#define HCI_HISR_CPWM2					BIT(22)
#define HCI_HISR_HSISR_IND				BIT(23)
#define HCI_HISR_GTINT3_IND				BIT(24)
#define HCI_HISR_GTINT4_IND				BIT(25)
#define HCI_HISR_PSTIMEOUT				BIT(26)
#define HCI_HISR_OCPINT					BIT(27)
#define HCI_HISR_TSF_BIT32_TOGGLE		BIT(29)
//SPI HIMR MASK diff with SDIO
#define HCI_HIMR_RX_REQUEST    			BIT(0)
#define HCI_HIMR_AVAL					BIT(1)
#define HCI_HIMR_TXERR					BIT(2)
#define HCI_HIMR_RXERR					BIT(3)
#define HCI_HIMR_TXFOVW				BIT(4)
#define HCI_HIMR_RXFOVW				BIT(5)
#define HCI_HIMR_TXBCNOK				BIT(6)
#define HCI_HIMR_TXBCNERR				BIT(7)
#define HCI_HIMR_BCNERLY_INT			BIT(16)
#define HCI_HIMR_ATIMEND				BIT(17)
#define HCI_HIMR_ATIMEND_E				BIT(18)
#define HCI_HIMR_CTWEND				BIT(19)
#define HCI_HIMR_C2HCMD				BIT(20)
#define HCI_HIMR_CPWM1					BIT(21)
#define HCI_HIMR_CPWM2					BIT(22)
#define HCI_HIMR_HSISR_IND				BIT(23)
#define HCI_HIMR_GTINT3_IND				BIT(24)
#define HCI_HIMR_GTINT4_IND				BIT(25)
#define HCI_HIMR_PSTIMEOUT				BIT(26)
#define HCI_HIMR_OCPINT					BIT(27)
#define HCI_HIMR_TSF_BIT32_TOGGLE		BIT(29)
#endif
#define MASK_SPI_HISR_CLEAR		(HCI_HIMR_TXERR |\
									HCI_HIMR_RXERR |\
									HCI_HIMR_TXFOVW |\
									HCI_HIMR_RXFOVW |\
									HCI_HIMR_TXBCNOK |\
									HCI_HIMR_TXBCNERR |\
									HCI_HIMR_C2HCMD |\
									HCI_HIMR_CPWM1 |\
									HCI_HIMR_CPWM2 |\
									HCI_HIMR_HSISR_IND |\
									HCI_HIMR_GTINT3_IND |\
									HCI_HIMR_GTINT4_IND |\
									HCI_HIMR_PSTIMEOUT |\
									HCI_HIMR_OCPINT)

#define REG_LEN_FORMAT(pcmd, x) 			SET_BITS_TO_LE_4BYTE(pcmd, 0, 8, x)//(x<<(unsigned int)24)
#define REG_ADDR_FORMAT(pcmd,x) 			SET_BITS_TO_LE_4BYTE(pcmd, 8, 16, x)//(x<<(unsigned int)16)
#define REG_DOMAIN_ID_FORMAT(pcmd,x) 		SET_BITS_TO_LE_4BYTE(pcmd, 24, 5, x)//(x<<(unsigned int)0)
#define REG_FUN_FORMAT(pcmd,x) 			SET_BITS_TO_LE_4BYTE(pcmd, 29, 2, x)//(x<<(unsigned int)5)
#define REG_RW_FORMAT(pcmd,x) 				SET_BITS_TO_LE_4BYTE(pcmd, 31, 1, x)//(x<<(unsigned int)7)

#define FIFO_LEN_FORMAT(pcmd, x) 			SET_BITS_TO_LE_4BYTE(pcmd, 0, 16, x)//(x<<(unsigned int)24)
//#define FIFO_ADDR_FORMAT(pcmd,x) 			SET_BITS_TO_LE_4BYTE(pcmd, 8, 16, x)//(x<<(unsigned int)16)
#define FIFO_DOMAIN_ID_FORMAT(pcmd,x) 	SET_BITS_TO_LE_4BYTE(pcmd, 24, 5, x)//(x<<(unsigned int)0)
#define FIFO_FUN_FORMAT(pcmd,x) 			SET_BITS_TO_LE_4BYTE(pcmd, 29, 2, x)//(x<<(unsigned int)5)
#define FIFO_RW_FORMAT(pcmd,x) 			SET_BITS_TO_LE_4BYTE(pcmd, 31, 1, x)//(x<<(unsigned int)7)


//get status dword0
#define GET_STATUS_PUB_PAGE_NUM(status)		LE_BITS_TO_4BYTE(status, 24, 8)
#define GET_STATUS_HI_PAGE_NUM(status)		LE_BITS_TO_4BYTE(status, 18, 6)
#define GET_STATUS_MID_PAGE_NUM(status)		LE_BITS_TO_4BYTE(status, 12, 6)
#define GET_STATUS_LOW_PAGE_NUM(status)		LE_BITS_TO_4BYTE(status, 6, 6)
#define GET_STATUS_HISR_HI6BIT(status)			LE_BITS_TO_4BYTE(status, 0, 6)

//get status dword1
#define GET_STATUS_HISR_MID8BIT(status)		LE_BITS_TO_4BYTE(status + 4, 24, 8)
#define GET_STATUS_HISR_LOW8BIT(status)		LE_BITS_TO_4BYTE(status + 4, 16, 8)
#define GET_STATUS_ERROR(status)		    		LE_BITS_TO_4BYTE(status + 4, 17, 1)
#define GET_STATUS_INT(status)		        		LE_BITS_TO_4BYTE(status + 4, 16, 1)
#define GET_STATUS_RX_LENGTH(status)			LE_BITS_TO_4BYTE(status + 4, 0, 16)


#define RXDESC_SIZE	24

#define TX_FREE_PG_QUEUE			4	// The number of Tx FIFO free page
#define TX_FIFO_PAGE_SZ 			128

struct spi_more_data {
	unsigned long more_data;
	unsigned long len;
};

extern BUS_DRV_OPS_T bus_driver_ops;

extern u8 spi_query_status_info(struct dvobj_priv *pdvobj);

extern void spi_set_intf_ops(struct _io_ops *pops);

#endif //__GSPI_SPEC_H__
