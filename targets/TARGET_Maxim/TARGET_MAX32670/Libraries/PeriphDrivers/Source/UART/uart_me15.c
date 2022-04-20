/* ****************************************************************************
 * Copyright (C) 2022 Maxim Integrated Products, Inc., All Rights Reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included
 * in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
 * OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL MAXIM INTEGRATED BE LIABLE FOR ANY CLAIM, DAMAGES
 * OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
 * ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 *
 * Except as contained in this notice, the name of Maxim Integrated
 * Products, Inc. shall not be used except as stated in the Maxim Integrated
 * Products, Inc. Branding Policy.
 *
 * The mere transfer of this software does not imply any licenses
 * of trade secrets, proprietary technology, copyrights, patents,
 * trademarks, maskwork rights, or any other form of intellectual
 * property whatsoever. Maxim Integrated Products, Inc. retains all
 * ownership rights.
 *
 *************************************************************************** */
#ifdef __CC_ARM  // Keil 
#pragma diag_suppress 68  // integer conversion resulted in a change of sign
#endif

#include "uart.h"
#include "mxc_device.h"
#include "mxc_pins.h"
#include "mxc_assert.h"
#include "uart_revb.h"
#include "uart_common.h"
#include "mcr_regs.h"
#include "dma.h"

void MXC_UART_DMACallback (int ch, int error)
{
    MXC_UART_RevB_DMACallback (ch, error);
}

int MXC_UART_AsyncCallback(mxc_uart_regs_t* uart, int retVal)
{
    return MXC_UART_RevB_AsyncCallback ((mxc_uart_revb_regs_t*) uart, retVal);
}

int MXC_UART_AsyncStop(mxc_uart_regs_t* uart)
{
    return MXC_UART_RevB_AsyncStop ((mxc_uart_revb_regs_t*) uart);
}

int MXC_UART_Init(mxc_uart_regs_t* uart, unsigned int baud, mxc_uart_clock_t clock, sys_map_t map)
{
    int retval;
    
    retval = MXC_UART_Shutdown(uart);
    
    if (retval) {
        return retval;
    }
    
    switch (clock) {
    case MXC_UART_EXT_CLK:
        MXC_GPIO_Config(&gpio_cfg_extclk);
        break;
        
    case MXC_UART_ERTCO_CLK:
        MXC_SYS_ClockSourceEnable(MXC_SYS_CLOCK_ERTCO);
        break;
        
    case MXC_UART_IBRO_CLK:
        MXC_SYS_ClockSourceEnable(MXC_SYS_CLOCK_IBRO);
        break;
        
    case MXC_UART_ERFO_CLK:
        MXC_SYS_ClockSourceEnable(MXC_SYS_CLOCK_ERFO);
        break;

    case MXC_UART_INRO_CLK:
    	MXC_SYS_ClockSourceEnable(MXC_SYS_CLOCK_INRO);
    	break;

    default:
        break;
    }
    
    switch (MXC_UART_GET_IDX(uart)) {
    case 0:
        if (map == MAP_B) {
            MXC_GPIO_Config(&gpio_cfg_uart0b);
        } else {
            MXC_GPIO_Config(&gpio_cfg_uart0a);
        }
        MXC_SYS_ClockEnable(MXC_SYS_PERIPH_CLOCK_UART0);
        break;
 
    case 1:
        if (map == MAP_B) {
            MXC_GPIO_Config(&gpio_cfg_uart1b);
        } else {
            MXC_GPIO_Config(&gpio_cfg_uart1a);
        }
        MXC_SYS_ClockEnable(MXC_SYS_PERIPH_CLOCK_UART1);
        break;

    case 2:
        MXC_GPIO_Config(&gpio_cfg_uart2b);
        MXC_SYS_ClockEnable(MXC_SYS_PERIPH_CLOCK_UART2);
        break;
        
    case 3:
        MXC_GPIO_Config(&gpio_cfg_uart3);
        MXC_SYS_ClockEnable(MXC_SYS_PERIPH_CLOCK_UART3);
        break;

    default:
        return E_NOT_SUPPORTED;
    }
    
    return MXC_UART_RevB_Init ((mxc_uart_revb_regs_t*) uart, baud, (mxc_uart_revb_clock_t)clock);
}

int MXC_UART_Shutdown(mxc_uart_regs_t* uart)
{
    switch (MXC_UART_GET_IDX(uart)) {
    case 0:
        MXC_SYS_Reset_Periph(MXC_SYS_RESET0_UART0);
        MXC_SYS_ClockDisable(MXC_SYS_PERIPH_CLOCK_UART0);
        break;
    
    case 1:
        MXC_SYS_Reset_Periph(MXC_SYS_RESET0_UART1);
        MXC_SYS_ClockDisable(MXC_SYS_PERIPH_CLOCK_UART1);
        break;
 
    case 2:      
        MXC_SYS_Reset_Periph(MXC_SYS_RESET0_UART2);
        MXC_SYS_ClockDisable(MXC_SYS_PERIPH_CLOCK_UART2);
        break;
     
    case 3:
        MXC_SYS_Reset_Periph(MXC_SYS_RESET_UART3);
        MXC_SYS_ClockDisable(MXC_SYS_PERIPH_CLOCK_UART3);
        break;

    default:
        return E_NOT_SUPPORTED;
    }
    
    return E_NO_ERROR;
}

int MXC_UART_ReadyForSleep(mxc_uart_regs_t* uart)
{
    return MXC_UART_RevB_ReadyForSleep ((mxc_uart_revb_regs_t*) uart);
}

int MXC_UART_SetFrequency(mxc_uart_regs_t* uart, unsigned int baud, mxc_uart_clock_t clock)
{
    int freq;
    unsigned int mod = 0;
    unsigned int clkdiv = 0;
    int div = 8;

    if (MXC_UART_GET_IDX (uart) < 0) {
        return E_BAD_PARAM;
    }

    // check if the uart is LPUART
    if(uart == MXC_UART3) {
        // OSR default value
        uart->osr = 5;
                
        switch (clock) {
            case MXC_UART_APB_CLK:
                uart->ctrl |= MXC_S_UART_CTRL_BCLKSRC_PERIPHERAL_CLOCK;
                div = (1 << (MXC_GCR->pclkdiv & MXC_F_GCR_PCLKDIV_AON_CLKDIV)) * 8;
                clkdiv = ((SystemCoreClock / div) / baud);
                mod = ((SystemCoreClock / div) % baud);
                break;

            case MXC_UART_EXT_CLK:
                uart->ctrl |= MXC_S_UART_CTRL_BCLKSRC_EXTERNAL_CLOCK;
                break;

            case MXC_UART_ERTCO_CLK:
                uart->ctrl |= MXC_S_UART_CTRL_BCLKSRC_CLK2;
                uart->ctrl |= MXC_F_UART_CTRL_FDM;
                clkdiv = ((ERTCO_FREQ * 2) / baud);
                mod = ((ERTCO_FREQ * 2) % baud);

                if (baud > 2400) {
                    uart->osr = 0;
                } else {
                    uart->osr = 1;
                }
                break;

            case MXC_UART_INRO_CLK:
                uart->ctrl |= MXC_S_UART_CTRL_BCLKSRC_CLK3;
                uart->ctrl |= MXC_F_UART_CTRL_FDM;
                clkdiv = ((INRO_FREQ * 2) / baud);
                mod = ((INRO_FREQ * 2) % baud);

                if (baud > 2400) {
                    uart->osr = 0;
                } else {
                    uart->osr = 1;
                }
                break;

            default:
                return E_BAD_PARAM;
        }

        if(!clkdiv || mod > (baud / 2)) {
            clkdiv++;
        }
        uart->clkdiv = clkdiv;

        freq = MXC_UART_GetFrequency (uart);
    } 
    else {
		unsigned int clkDiv = 0, mod = 0;
		if (MXC_UART_GET_IDX ((mxc_uart_regs_t*) uart) < 0) {
			return E_BAD_PARAM;
		}
		
		// OSR default value
		uart->osr = 5;

		switch (clock) {
			case MXC_UART_REVB_APB_CLK:
			clkDiv = (PeripheralClock / baud);
			mod = (PeripheralClock % baud);
			break;

			case MXC_UART_REVB_EXT_CLK:
			uart->ctrl |= MXC_S_UART_REVB_CTRL_BCLKSRC_EXTERNAL_CLOCK;
			break;

			//case MXC_UART_IBRO_CLK:
			case MXC_UART_REVB_CLK2:
			clkDiv = (IBRO_FREQ / baud);
			mod = (IBRO_FREQ % baud);
			uart->ctrl |= MXC_S_UART_REVB_CTRL_BCLKSRC_CLK2;
			break;

			//case MXC_UART_ERFO:
			case MXC_UART_REVB_CLK3:
			clkDiv = (ERFO_FREQ / baud);
			mod = (ERFO_FREQ % baud);
			uart->ctrl |= MXC_S_UART_REVB_CTRL_BCLKSRC_CLK3;
			break;

			default:
			return E_BAD_PARAM;
		}
		
		if(!clkDiv || mod > (baud / 2)) {
			clkDiv++;
		}
		uart->clkdiv = clkDiv;
		freq = MXC_UART_GetFrequency ((mxc_uart_regs_t*) uart);			
	}
    if(freq > 0) {
        // Enable baud clock and wait for it to become ready.
        uart->ctrl |= MXC_F_UART_CTRL_BCLKEN;
        while(((uart->ctrl & MXC_F_UART_CTRL_BCLKRDY) >> MXC_F_UART_CTRL_BCLKRDY_POS) == 0);
    }   

    return freq;
}

int MXC_UART_GetFrequency(mxc_uart_regs_t* uart)
{
    int periphClock = 0;
    int div = 8;
    
    if (MXC_UART_GET_IDX (uart) < 0) {
        return E_BAD_PARAM;
    }

    // check if UARt is LP UART
    if(uart == MXC_UART3) {
        if ((uart->ctrl & MXC_F_UART_CTRL_BCLKSRC) == MXC_S_UART_CTRL_BCLKSRC_EXTERNAL_CLOCK) {
            return E_NOT_SUPPORTED;
        }
        else if((uart->ctrl & MXC_F_UART_CTRL_BCLKSRC) == MXC_S_UART_CTRL_BCLKSRC_PERIPHERAL_CLOCK) {
            div = (1 << (MXC_GCR->pclkdiv & MXC_F_GCR_PCLKDIV_AON_CLKDIV)) * 8;
            periphClock = SystemCoreClock / div;
        }
        else if((uart->ctrl & MXC_F_UART_CTRL_BCLKSRC) == MXC_S_UART_CTRL_BCLKSRC_CLK2) {
            periphClock = ERTCO_FREQ * 2;
        }
        else if((uart->ctrl & MXC_F_UART_CTRL_BCLKSRC) == MXC_S_UART_CTRL_BCLKSRC_CLK3) {
            periphClock = INRO_FREQ * 2;
        }
        else {
            return E_BAD_PARAM;
        }
        return (periphClock / uart->clkdiv);
    }
    else {
        //return MXC_UART_RevB_GetFrequency ((mxc_uart_revb_regs_t*) uart);
		
		if (MXC_UART_GET_IDX ((mxc_uart_regs_t*) uart) < 0) {
			return E_BAD_PARAM;
		}

		if ((uart->ctrl & MXC_F_UART_REVB_CTRL_BCLKSRC) == MXC_S_UART_REVB_CTRL_BCLKSRC_EXTERNAL_CLOCK) {
			return E_NOT_SUPPORTED;
		}
		else if((uart->ctrl & MXC_F_UART_REVB_CTRL_BCLKSRC) == MXC_S_UART_REVB_CTRL_BCLKSRC_PERIPHERAL_CLOCK) {
			periphClock = PeripheralClock;
		}
		else if((uart->ctrl & MXC_F_UART_REVB_CTRL_BCLKSRC) == MXC_S_UART_REVB_CTRL_BCLKSRC_CLK2) {
			periphClock = IBRO_FREQ; 
		}
		else if((uart->ctrl & MXC_F_UART_REVB_CTRL_BCLKSRC) == MXC_S_UART_REVB_CTRL_BCLKSRC_CLK3) {
			periphClock = ERFO_FREQ;
		}
		else {
			return E_BAD_PARAM;
		}

		return (periphClock / uart->clkdiv);
    }    
}

int MXC_UART_SetDataSize(mxc_uart_regs_t* uart, int dataSize)
{
    return MXC_UART_RevB_SetDataSize ((mxc_uart_revb_regs_t*) uart, dataSize);
}

int MXC_UART_SetStopBits(mxc_uart_regs_t* uart, mxc_uart_stop_t stopBits)
{
    return MXC_UART_RevB_SetStopBits ((mxc_uart_revb_regs_t*) uart, stopBits);
}

int MXC_UART_SetParity(mxc_uart_regs_t* uart, mxc_uart_parity_t parity)
{
    return MXC_UART_RevB_SetParity ((mxc_uart_revb_regs_t*) uart, parity);
}

int MXC_UART_SetFlowCtrl(mxc_uart_regs_t* uart, mxc_uart_flow_t flowCtrl, int rtsThreshold, sys_map_t map)
{
    if(flowCtrl == MXC_UART_FLOW_EN) {
        switch (MXC_UART_GET_IDX(uart)) {
        case 0:
            if (map == MAP_B) {
                MXC_GPIO_Config(&gpio_cfg_uart0b_flow);
            } else {
                MXC_GPIO_Config(&gpio_cfg_uart0a_flow);
            }
            break;

        case 1:
            if (map == MAP_B) {
                MXC_GPIO_Config(&gpio_cfg_uart1b_flow);
            } else {
                MXC_GPIO_Config(&gpio_cfg_uart1a_flow);
            }
            break;

        case 2:
            MXC_GPIO_Config(&gpio_cfg_uart2b_flow);
            break;

        case 3:
            MXC_GPIO_Config(&gpio_cfg_uart3_flow);
            break;

        default:
            return E_BAD_PARAM;
        }
    }
    else {
        switch (MXC_UART_GET_IDX(uart)) {
        case 0:
            if (map == MAP_B) {
                MXC_GPIO_Config(&gpio_cfg_uart0b_flow_disable);
            } else {
                MXC_GPIO_Config(&gpio_cfg_uart0a_flow_disable);
            }
            break;

        case 1:
            if (map == MAP_B) {
                MXC_GPIO_Config(&gpio_cfg_uart1b_flow_disable);
            } else {
                MXC_GPIO_Config(&gpio_cfg_uart1a_flow_disable);
            }
            break;

        case 2:
            MXC_GPIO_Config(&gpio_cfg_uart2b_flow_disable);
            break;

        case 3:
            MXC_GPIO_Config(&gpio_cfg_uart3_flow_disable);
            break;

        default:
            return E_BAD_PARAM;
        }        
    }
    
    return MXC_UART_RevB_SetFlowCtrl ((mxc_uart_revb_regs_t*) uart, flowCtrl, rtsThreshold);
}

int MXC_UART_SetClockSource(mxc_uart_regs_t* uart, mxc_uart_clock_t clock)
{
    return MXC_UART_RevB_SetClockSource ((mxc_uart_revb_regs_t*) uart, (mxc_uart_revb_clock_t)clock);
}

int MXC_UART_GetActive(mxc_uart_regs_t* uart)
{
    return MXC_UART_RevB_GetActive ((mxc_uart_revb_regs_t*) uart);
}

int MXC_UART_AbortTransmission(mxc_uart_regs_t* uart)
{
    return MXC_UART_RevB_AbortTransmission ((mxc_uart_revb_regs_t*) uart);
}

int MXC_UART_ReadCharacterRaw(mxc_uart_regs_t* uart)
{
    return MXC_UART_RevB_ReadCharacterRaw ((mxc_uart_revb_regs_t*) uart);
}

int MXC_UART_WriteCharacterRaw(mxc_uart_regs_t* uart, uint8_t character)
{
    return MXC_UART_RevB_WriteCharacterRaw ((mxc_uart_revb_regs_t*) uart, character);
}

int MXC_UART_ReadCharacter(mxc_uart_regs_t* uart)
{
    return MXC_UART_Common_ReadCharacter (uart);
}

int MXC_UART_WriteCharacter(mxc_uart_regs_t* uart, uint8_t character)
{
    return MXC_UART_Common_WriteCharacter (uart, character);
}

int MXC_UART_Read(mxc_uart_regs_t* uart, uint8_t* buffer, int* len)
{
    return MXC_UART_RevB_Read ((mxc_uart_revb_regs_t*) uart, buffer, len);
}

int MXC_UART_Write(mxc_uart_regs_t* uart, const uint8_t* byte, int* len)
{
    return MXC_UART_RevB_Write ((mxc_uart_revb_regs_t*) uart, byte, len);
}

unsigned int MXC_UART_ReadRXFIFO(mxc_uart_regs_t* uart, unsigned char* bytes,
                                 unsigned int len)
{
    return MXC_UART_RevB_ReadRXFIFO ((mxc_uart_revb_regs_t*) uart, bytes, len);
    
}

int MXC_UART_ReadRXFIFODMA(mxc_uart_regs_t* uart, unsigned char* bytes,
                           unsigned int len, mxc_uart_dma_complete_cb_t callback)
{
    mxc_dma_config_t config;
    
    int uart_num = MXC_UART_GET_IDX(uart);
    
    switch (uart_num) {
    case 0:
        config.reqsel = MXC_DMA_REQUEST_UART0RX;
        break;
        
    case 1:
        config.reqsel = MXC_DMA_REQUEST_UART1RX;
        break;
        
    case 2:
        config.reqsel = MXC_DMA_REQUEST_UART2RX;
        break;
        
    default:
        return E_BAD_PARAM;
        break;
    }

   return MXC_UART_RevB_ReadRXFIFODMA ((mxc_uart_revb_regs_t*) uart, bytes, len, callback, config);
}

unsigned int MXC_UART_GetRXFIFOAvailable(mxc_uart_regs_t* uart)
{
    return MXC_UART_RevB_GetRXFIFOAvailable ((mxc_uart_revb_regs_t*) uart);
}

unsigned int MXC_UART_WriteTXFIFO(mxc_uart_regs_t* uart, const unsigned char* bytes,
                                  unsigned int len)
{
    return MXC_UART_RevB_WriteTXFIFO ((mxc_uart_revb_regs_t*) uart, bytes, len);
}

int MXC_UART_WriteTXFIFODMA(mxc_uart_regs_t* uart, const unsigned char* bytes,
                            unsigned int len, mxc_uart_dma_complete_cb_t callback)
{
    mxc_dma_config_t config;
    
    int uart_num = MXC_UART_GET_IDX(uart);
    
    switch (uart_num) {
    case 0:
        config.reqsel = MXC_DMA_REQUEST_UART0TX;
        break;
       
    case 1:
        config.reqsel = MXC_DMA_REQUEST_UART1TX;
        break;

    case 2:
        config.reqsel = MXC_DMA_REQUEST_UART2TX;
        break;
        
    default:
        return E_BAD_PARAM;
        break;
    }

   return MXC_UART_RevB_WriteTXFIFODMA ((mxc_uart_revb_regs_t*) uart, bytes, len, callback, config);
}

unsigned int MXC_UART_GetTXFIFOAvailable(mxc_uart_regs_t* uart)
{
    return MXC_UART_RevB_GetTXFIFOAvailable ((mxc_uart_revb_regs_t*) uart);
}

int MXC_UART_ClearRXFIFO(mxc_uart_regs_t* uart)
{
    return MXC_UART_RevB_ClearRXFIFO ((mxc_uart_revb_regs_t*) uart);
}

int MXC_UART_ClearTXFIFO(mxc_uart_regs_t* uart)
{
    return MXC_UART_RevB_ClearTXFIFO ((mxc_uart_revb_regs_t*) uart);
}

int MXC_UART_SetRXThreshold(mxc_uart_regs_t* uart, unsigned int numBytes)
{
    return MXC_UART_RevB_SetRXThreshold ((mxc_uart_revb_regs_t*) uart, numBytes);
}

unsigned int MXC_UART_GetRXThreshold(mxc_uart_regs_t* uart)
{
    return MXC_UART_RevB_GetRXThreshold ((mxc_uart_revb_regs_t*) uart);
}

int MXC_UART_SetTXThreshold(mxc_uart_regs_t* uart, unsigned int numBytes)
{
    // TX threshold is fixed at half the length of FIFO
    return E_NOT_SUPPORTED;
}

unsigned int MXC_UART_GetTXThreshold(mxc_uart_regs_t* uart)
{
    // TX threshold is fixed at half the length of FIFO
    return E_NOT_SUPPORTED;
}

unsigned int MXC_UART_GetFlags(mxc_uart_regs_t* uart)
{
    return MXC_UART_RevB_GetFlags ((mxc_uart_revb_regs_t*) uart);
}

int MXC_UART_ClearFlags(mxc_uart_regs_t* uart, unsigned int flags)
{
    return MXC_UART_RevB_ClearFlags ((mxc_uart_revb_regs_t*) uart, flags);
}

int MXC_UART_EnableInt(mxc_uart_regs_t* uart, unsigned int mask)
{
    return MXC_UART_RevB_EnableInt ((mxc_uart_revb_regs_t*) uart, mask);
}

int MXC_UART_DisableInt(mxc_uart_regs_t* uart, unsigned int mask)
{
    return MXC_UART_RevB_DisableInt ((mxc_uart_revb_regs_t*) uart, mask);
}

unsigned int MXC_UART_GetStatus(mxc_uart_regs_t* uart)
{
    return MXC_UART_RevB_GetStatus ((mxc_uart_revb_regs_t*) uart);
}

int MXC_UART_Transaction(mxc_uart_req_t* req)
{
    return MXC_UART_RevB_Transaction ((mxc_uart_revb_req_t*) req);
}

int MXC_UART_TransactionAsync(mxc_uart_req_t* req)
{
    return MXC_UART_RevB_TransactionAsync ((mxc_uart_revb_req_t*) req);
}

int MXC_UART_TransactionDMA(mxc_uart_req_t* req)
{
    return MXC_UART_RevB_TransactionDMA ((mxc_uart_revb_req_t*) req);
}

int MXC_UART_AbortAsync(mxc_uart_regs_t* uart)
{
    return MXC_UART_RevB_AbortAsync ((mxc_uart_revb_regs_t*) uart);
}

int MXC_UART_AsyncHandler(mxc_uart_regs_t* uart)
{
    return MXC_UART_RevB_AsyncHandler ((mxc_uart_revb_regs_t*) uart);
}

uint32_t MXC_UART_GetAsyncTXCount(mxc_uart_req_t* req)
{
	return req->txCnt;
}

uint32_t MXC_UART_GetAsyncRXCount(mxc_uart_req_t* req)
{
	return req->rxCnt;
}
