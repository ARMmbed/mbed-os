/**
 * @file xmc_usbd.c
 * @date 2015-06-20
 *
 * @cond
 **********************************************************************************
 * XMClib v2.1.16 - XMC Peripheral Driver Library 
 *
 * Copyright (c) 2015-2017, Infineon Technologies AG
 * All rights reserved.                        
 *                                             
 * Redistribution and use in source and binary forms, with or without           
 * modification,are permitted provided that the following conditions are met:   
 *                                                                              
 *   Redistributions of source code must retain the above copyright notice,      
 *   this list of conditions and the following disclaimer.                        
 * 
 *   Redistributions in binary form must reproduce the above copyright notice,   
 *   this list of conditions and the following disclaimer in the documentation    
 *   and/or other materials provided with the distribution.                       
 * 
 *   Neither the name of the copyright holders nor the names of its contributors 
 *   may be used to endorse or promote products derived from this software without
 *   specific prior written permission.                                           
 *                                                                              
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"  
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE    
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE   
 * ARE  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE   
 * LIABLE  FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR         
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF         
 * SUBSTITUTE GOODS OR  SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS    
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN      
 * CONTRACT, STRICT LIABILITY,OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)       
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE   
 * POSSIBILITY OF SUCH DAMAGE.                                                  
 *                                                                              
 * To improve the quality of the software, users are encouraged to share        
 * modifications, enhancements or bug fixes with Infineon Technologies AG       
 * dave@infineon.com).                                                          
 **********************************************************************************
 *
 * Change History
 * --------------
 *
 * 2015-02-16:
 *     - Initial Version.<br>           
 * 2015-03-18:
 *     - Updated the XMC_USBD_EndpointStall() to fix issue on USB clear stall. <br>
 *     - Updated the XMC_USBD_EndpointConfigure() to fix issue in EP0 configuration.<br>
 *     - Updated the XMC_USBD_IRQHandler()(Removed the DAVE_CE check on SOF event).<br>
 * 2015-06-20:
 *     - Removed GetDriverVersion API.<br>
 *     - Updated the XMC_USBD_IsEnumDone() API.<br>
 *     - Updated the copy right in the file header.<br>
 *     - Updated the XMC_USBD_Disable() API to gate the clock after programming the SCU registers.<br>
 *
 * @endcond 
 *
 */


/*******************************************************************************
 * HEADER FILES
 *******************************************************************************/
#include <xmc_usbd.h>

#if defined(USB0)
	
/**< macro to check the maximum number of endpoints used*/
#define XMC_USBD_CHECK_INPUT_MAX_NUM_EPS(usbd_max_num_eps) \
  ((usbd_max_num_eps ==  XMC_USBD_MAX_NUM_EPS_1 ) || \
   (usbd_max_num_eps ==  XMC_USBD_MAX_NUM_EPS_2 ) || \
   (usbd_max_num_eps ==  XMC_USBD_MAX_NUM_EPS_3 ) || \
   (usbd_max_num_eps ==  XMC_USBD_MAX_NUM_EPS_4 ) || \
   (usbd_max_num_eps ==  XMC_USBD_MAX_NUM_EPS_5 ) || \
   (usbd_max_num_eps ==  XMC_USBD_MAX_NUM_EPS_6 ) || \
   (usbd_max_num_eps == XMC_USBD_MAX_NUM_EPS_7 ))

/*******************************************************************************
 *GLOBAL DATA
 *******************************************************************************/
/*
 * Endpoint Out Fifo Size
 */
uint32_t XMC_USBD_EP_OUT_BUFFERSIZE[7] = {0U,0U,0U,0U,0U,0U,0U};
/*
 * Endpoint In Fifo Size
 */
uint32_t XMC_USBD_EP_IN_BUFFERSIZE[7] = {0U,0U,0U,0U,0U,0U,0U};
/*
 * Device definition
 */
 XMC_USBD_DEVICE_t xmc_device;
#ifdef __GNUC__															  /*GCC*/
/*
 * Endpoint Out Fifo
 */
static __attribute__((aligned(4))) uint8_t XMC_USBD_EP_OUT_BUFFER[7][256] __attribute__((section("USB_RAM")));
/*
 * Endpoint In Fifo
 */
static __attribute__((aligned(4))) uint8_t XMC_USBD_EP_IN_BUFFER[7][256] __attribute__((section("USB_RAM")));
#endif
#if defined(__ICCARM__)
#pragma data_alignment=4
/*
 * Endpoint Out Fifo
 */
static uint8_t XMC_USBD_EP_OUT_BUFFER[7][256] @ ".dram";
/*
 * Endpoint In Fifo
 */
#pragma data_alignment=4
static uint8_t XMC_USBD_EP_IN_BUFFER[7][256] @ ".dram";
#endif
#if defined(__CC_ARM)
/*
 * Endpoint Out Fifo
 */
static __attribute__((aligned(4))) uint8_t XMC_USBD_EP_OUT_BUFFER[7][256] __attribute__((section ("RW_IRAM1")));
/*
 * Endpoint In Fifo
 */
static __attribute__((aligned(4))) uint8_t XMC_USBD_EP_IN_BUFFER[7][256] __attribute__((section ("RW_IRAM1")));
#endif
XMC_USBD_t *usbd_init;

/*******************************************************************************
 *LOCAL ROUTINES
 *******************************************************************************/
/*Local routines prototypes*/
uint8_t XMC_USBD_lDeviceActive(const XMC_USBD_t *const obj);
static void XMC_USBD_lReadFifo(const uint32_t ep_num,const uint32_t byte_count);
static uint32_t XMC_USBD_lWriteFifo(XMC_USBD_EP_t *ep);
static void XMC_USBD_lFlushTXFifo(const uint8_t fifo_num);
static void XMC_USBD_lFlushRXFifo(void);
static uint8_t XMC_USBD_lAssignTXFifo(void);
static void XMC_USBD_lStartReadXfer(XMC_USBD_EP_t *const ep);
static void XMC_USBD_lStartWriteXfer(XMC_USBD_EP_t *const ep);
static void XMC_USBD_lHandleEnumDone(void);
static void XMC_USBD_lHandleOEPInt(const XMC_USBD_t *const obj);
static void XMC_USBD_lHandleRxFLvl(void);
static void XMC_USBD_lHandleIEPInt(const XMC_USBD_t *const obj);
static void XMC_USBD_lUnassignFifo(const uint8_t fifo_nr);
static void XMC_USBD_lHandleUSBReset(const XMC_USBD_t *const obj);
static void XMC_USBD_lHandleOTGInt(void);
static void XMC_USBD_lClearEventOTG(uint32_t event);

/**
 *  The device driver
 */
const XMC_USBD_DRIVER_t Driver_USBD0 =
{
  .GetCapabilities = XMC_USBD_GetCapabilities,
  .Initialize = XMC_USBD_Init,
  .Uninitialize = XMC_USBD_Uninitialize,
  .DeviceConnect = XMC_USBD_DeviceConnect,
  .DeviceDisconnect = XMC_USBD_DeviceDisconnect,
  .DeviceGetState = XMC_USBD_DeviceGetState,
  .DeviceSetAddress = XMC_USBD_DeviceSetAddress,
  .EndpointConfigure = XMC_USBD_EndpointConfigure,
  .EndpointUnconfigure = XMC_USBD_EndpointUnconfigure,
  .EndpointStall = XMC_USBD_EndpointStall,
  .EndpointReadStart = XMC_USBD_EndpointReadStart,
  .EndpointRead = XMC_USBD_EndpointRead,
  .EndpointWrite = XMC_USBD_EndpointWrite,
  .EndpointAbort = XMC_USBD_EndpointAbort,
  .GetFrameNumber = XMC_USBD_GetFrameNumber,
  .IsEnumDone = XMC_USBD_IsEnumDone
};

/**
 * @brief Checks if device is active
 *
 * Therefore the endpoint inInUse flag are checked and if one endpoint is in use, 1 is returned,
 * else 0 is returned.
 * @return 1 if an endpoint is active else 0
 */
uint8_t XMC_USBD_lDeviceActive(const XMC_USBD_t *const obj) 
{
  uint8_t i;
  uint8_t result = 0U;
  for (i = 0U; i < (uint8_t)obj->usbd_max_num_eps; i++)
  {
    if (xmc_device.ep[i].inInUse || xmc_device.ep[i].outInUse)
    {
      result = 1U;
    }
  }
  return result;
}


/**
 * @brief Read data from the rx fifo
 *
 * The data from the fifo is copied in to the buffer specified by @ref xfer_buffer and
 * the transfer values get updated. If the endpoint is disabled or the buffer not existent
 * the function exits.
 *
 * @arg ep_num the endpoint to read for
 * @arg byte_count the byte count to read
 */
static void XMC_USBD_lReadFifo(const uint32_t ep_num,const uint32_t byte_count) 
{
  XMC_USBD_EP_t * ep = &xmc_device.ep[ep_num];
  uint32_t word_count;
  uint32_t temp_data;
  uint32_t temp_word_count;
  volatile uint32_t *fifo = xmc_device.fifo[0U];
  uint32_t i;
  depctl_data_t data;
  data.d32 = xmc_device.endpoint_out_register[ep_num]->doepctl;
  word_count = (byte_count >> 2U );
  temp_word_count = (word_count << 2U);
  /* Check if ep is enabled and has buffer */
  if (!data.b.usbactep)
  {
    /*Do Nothing*/
  }
  else if (ep->xferBuffer == NULL)
  {
	/*Do Nothing*/
  }
  else
  {
    /* store the data */
    for (i = 0U;i < word_count; i++)
    {
      *(((uint32_t*)ep->xferBuffer)+i) = *fifo;
    }
    /* space is not devidable by 4 */
    if (byte_count!=temp_word_count)
    {
      temp_data = *fifo;
      for (i = 0U;(temp_word_count + i) < byte_count;i++)
      {
        ep->xferBuffer[(word_count << 2)+i] = (uint8_t)((temp_data & ((uint32_t)0xFFU << (i * 8U))) >> (i * 8U));
      }
    }

    /* save the amount of data */
    ep->xferCount += byte_count;
    ep->xferBuffer += byte_count;
  }
}

/**
 * @brief Write data to an endpoint fifo
 *
 * The data from the @ref xfer_buffer gets copied in to the tx fifo of the endpoint until the buffer has been read
 *completely or the tx fifo is full. The transfer values are not updated.
 *
 * @arg[in] ep the endpoint to use
 * @return the number of bytes written to the fifo
 */
static uint32_t XMC_USBD_lWriteFifo(XMC_USBD_EP_t *const ep) 
{
  dtxfsts_data_t freeSpace; 
  volatile uint32_t *fifo;
  uint32_t byte_count;
  uint32_t word_count;
  uint32_t result;
  uint32_t i;
  fifo = xmc_device.fifo[ep->address_u.address_st.number]; /* fifo */
  freeSpace.d32 = xmc_device.endpoint_in_register[ep->address_u.address_st.number]->dtxfsts;
  /* calculate the length and the amount of dwords to copy based on the fifo status */
  byte_count = ep->xferLength - ep->xferCount;
  if (!byte_count)
  {
    result = 0U;
  }
  else
  {
    /* add the unaligned bytes to the word count to compare with the fifo space */
    word_count = ((uint32_t)byte_count + 3U) >> 2U;
    if (word_count > (uint32_t)freeSpace.b.txfspcavail )
    {
      word_count = (uint32_t)freeSpace.b.txfspcavail;
      byte_count = (uint32_t)word_count << (uint32_t)2U;
    }

    /* copy data dword wise */
    for (i = 0U; i < word_count;ep->xferBuffer+= 4U)
    {
      *fifo = *(uint32_t*)ep->xferBuffer;
      i++;
    }
    result=byte_count;
  }
  return result;
}

/**
 * @brief Flush a tx fifo
 *
 * @param[in] fifo_num Fifo number to flush
 *
 * @note Use 0x10 as parameter to flush all tx fifos.
 */
static void XMC_USBD_lFlushTXFifo(const uint8_t fifo_num) 
{
  volatile grstctl_t data;  
  uint32_t count;
  data.d32 = 0U;
  /*flush fifo */
  data.b.txfflsh = 1U;
  data.b.txfnum = fifo_num;
  xmc_device.global_register->grstctl = data.d32;
  for (count = 0U;count < 1000U; count++){}
  do
  {
   data.d32 = xmc_device.global_register->grstctl;
  } while (data.b.txfflsh);
    count = 0U;
  while (count++ < 1000U)
  {
    /* wait 3 phy clocks */
  }
}

/**
 * @brief Flush the rx fifo
 */
static void XMC_USBD_lFlushRXFifo(void) 
{
  volatile grstctl_t data;  
  uint32_t count;
	
	data.d32 = 0U;
  data.b.rxfflsh = 1U;
  /* flush FIFO */
  xmc_device.global_register->grstctl = data.d32;
  do
  {
    for (count = 0U; count < 1000U; count++){}
    data.d32 = xmc_device.global_register->grstctl;
  } while (data.b.rxfflsh);
  count = 0U;
  while (count++ < 1000U)
  {
    /* wait 3 phy clocks */
  }
}

/*
 * Support Functions
 */

/**
 * @brief Assign a free tx fifo
 *
 * A free tx fifo will be searched and the number will be returned.
 *
 * @return Fifo number for a free fifo
 */
static uint8_t XMC_USBD_lAssignTXFifo(void) 
{
  uint16_t mask = 1U;
  uint8_t i = 0U;
  uint8_t result = 0U;
  while( (i < (uint8_t)XMC_USBD_NUM_TX_FIFOS)&&((xmc_device.txfifomsk & mask) != 0U))
  {
    mask = (uint16_t)(mask << 1U);
    i++;
  }
  if ((xmc_device.txfifomsk & mask) == 0U)
  {
    xmc_device.txfifomsk |= mask;
    result=i;
  }
  return result;
}

/**
 * @brief Free a tx fifo
 *
 * Mark an used tx fifo as free.
 * @param[in] fifo_nr Fifo number to free
 */
static void XMC_USBD_lUnassignFifo(const uint8_t fifo_nr) 
{
  xmc_device.txfifomsk = (uint16_t)((uint32_t)xmc_device.txfifomsk & (uint32_t)(~((uint32_t)((uint32_t)1U << fifo_nr))));
}

/**
 * @brief Start a transfer for an out endpoint
 *
 * Based on the transfer values of the endpoint, the out endpoint registers will be programmed
 * to start a new out transfer.
 *
 * @note No checking of the transfer values are done in this function. Be sure,
 * that the transfer values are reasonable (e.g. buffer size is not exceeded).
 *
 * @param[in] ep Endpoint to start the transfer
 */
static void XMC_USBD_lStartReadXfer(XMC_USBD_EP_t *const ep) 
{
  deptsiz_data_t data;
  depctl_data_t epctl;
	
  data.d32 =  0U;
  if ((ep->xferTotal - ep->xferLength) > ep->maxTransferSize)
  {
    ep->xferLength += ep->maxTransferSize;
  }
  else
  {
    ep->xferLength = ep->xferTotal;
  }
  if (ep->address_u.address_st.number == 0U)
  {
	/* Setup the endpoint to receive 3 setup packages and one normal package.*/
	/* Cast the data pointer to use only one variable */
    deptsiz0_data_t *ep0_data = (deptsiz0_data_t*)&data;
    ep0_data->b.pktcnt = 0x1U;
    ep0_data->b.supcnt = 0x3U;
    ep0_data->b.xfersize = (uint8_t)ep->xferTotal;
  }
  else
  {
    /* If requested length is zero, just receive one zero length packet */
    if (ep->xferLength == 0U)
    {
      data.b.xfersize = 0U;
      data.b.pktcnt = 1U;
    }
    else
    {
      /* setup endpoint to recive a amount of packages by given size */
      data.b.pktcnt = (uint16_t)(((ep->xferLength - ep->xferCount) + (ep->maxPacketSize -(uint32_t)1U))/ep->maxPacketSize);
      data.b.xfersize =(uint32_t)(ep->xferLength - ep->xferCount);
    }
  }
  if(usbd_init->usbd_transfer_mode == XMC_USBD_USE_DMA)
  {
    /* Programm dma address if needed */
    xmc_device.endpoint_out_register[ep->address_u.address_st.number]->doepdma = (uint32_t)(ep->xferBuffer);
  }
  /* setup endpoint size and enable endpoint */
  xmc_device.endpoint_out_register[ep->address_u.address_st.number]->doeptsiz = data.d32;
 
  epctl.d32 = xmc_device.endpoint_out_register[ep->address_u.address_st.number]->doepctl;
  epctl.b.cnak = 1U;
  epctl.b.epena = 1U;
  xmc_device.endpoint_out_register[ep->address_u.address_st.number]->doepctl = epctl.d32;
}

/**
 * @brief Start a new in transfer
 *
 * Based on the transfer values of the endpoint the in endpoint registers will be programmed
 * to start a new in transfer
 *
 * @param[in] ep Endpoint to start the transfer
 */
static void XMC_USBD_lStartWriteXfer(XMC_USBD_EP_t *const ep) 
{
  deptsiz_data_t size;
  depctl_data_t ctl;
	
  size.d32 = 0U;  
  ctl.d32 = xmc_device.endpoint_in_register[ep->address_u.address_st.number]->diepctl;

  if ((ep->xferTotal - ep->xferLength)  < ep->maxTransferSize)
  {
    ep->xferLength = ep->xferTotal;
  }
  else
  {
    ep->xferLength += ep->maxTransferSize;
  }
  if (ep->xferLength == 0U)
  {
    size.b.xfersize = 0U;
    size.b.pktcnt = 1U;
  }
  else
  {
    if (ep->address_u.address_st.number == 0U)
    {
      size.b.pktcnt = 1U;
      /* ep->maxXferSize equals maxPacketSize */
      size.b.xfersize = (uint32_t)(ep->xferLength - ep->xferCount);
    }
    else
    {
      size.b.xfersize =(uint32_t)(ep->xferLength - ep->xferCount);
      size.b.pktcnt = (uint16_t)(((uint16_t)(ep->xferLength - ep->xferCount) + (uint16_t)((uint16_t)ep->maxPacketSize - 1U))/
    		 ep->maxPacketSize);
    }
    if(usbd_init->usbd_transfer_mode == XMC_USBD_USE_DMA)
    {
      /* Program dma*/
      xmc_device.endpoint_in_register[ep->address_u.address_st.number]->diepdma = (uint32_t)ep->xferBuffer;
    }
    if(usbd_init->usbd_transfer_mode == XMC_USBD_USE_FIFO)
    {
      /* enable fifo empty interrupt */
			xmc_device.device_register->dtknqr4_fifoemptymsk |= (uint32_t)((uint32_t)1U << (uint8_t)ep->address_u.address_st.number);
    }
  }

  /* Program size of transfer and enable endpoint */
  xmc_device.endpoint_in_register[ep->address_u.address_st.number]->dieptsiz = size.d32;
  ctl.b.epena = 1U;
  ctl.b.cnak = 1U;
  xmc_device.endpoint_in_register[ep->address_u.address_st.number]->diepctl = ctl.d32;
}


/**
 * @brief Handles the USBD reset interrupt
 *
 * When ever the host sets the bus into reset condition the usb otg_core generates
 * an interrupt, which is handled by this function. It resets the complete otg_core
 * into the default state.
 */
static void XMC_USBD_lHandleUSBReset(const XMC_USBD_t *const obj) 
{
  uint32_t i;
  depctl_data_t epctl;
  dctl_data_t dctl;
  fifosize_data_t gnptxfsiz;
  daint_data_t daint;
  dcfg_data_t dcfg;
	
  /* Clear the Remote Wakeup Signaling */
  dctl.d32 = xmc_device.device_register->dctl;
  dctl.b.rmtwkupsig = 1U;
  xmc_device.device_register->dctl = dctl.d32;

  /* enable naks for all eps */
  for (i = 0U;i < (uint8_t)XMC_USBD_NUM_EPS;i++)
  {
    epctl.d32 = xmc_device.endpoint_out_register[i]->doepctl;
		epctl.b.snak = 1U;
		epctl.b.stall = 0U;
		xmc_device.endpoint_out_register[i]->doepctl = epctl.d32;
  }

  /* Configure fifos */
  /* Calculate the size of the rx fifo */
  xmc_device.global_register->grxfsiz = 64U;
  /* Calculate the size of the tx fifo for ep 0 */  
  gnptxfsiz.d32 = 0U;
  gnptxfsiz.b.depth = 16U;
  gnptxfsiz.b.startaddr = 64U;
  xmc_device.global_register->gnptxfsiz = gnptxfsiz.d32;
  /* calculate the size for the rest */
  for (i = 1U;i < (uint8_t)XMC_USBD_NUM_TX_FIFOS;i++)
  {
    xmc_device.global_register->dtxfsiz[i- 1U] = (uint32_t)(((256U + (i*(64U)))/4U) | ((uint32_t)16U << 16U));
  }

  /* flush the fifos for proper operation */
  XMC_USBD_lFlushTXFifo(0x10U); /* 0x10 == all fifos, see doc */
  XMC_USBD_lFlushTXFifo(0x0U);
  XMC_USBD_lFlushRXFifo();
  /* Flush learning queue not needed due to fifo config */
  /* enable ep0 interrupts */  
  daint.d32 = 0U;
  daint.b.inep0 = 1U;
  daint.b.outep0 = 1U;
  xmc_device.device_register->daintmsk = daint.d32;

  /* enable endpoint interrupts */
  /* out ep interrupts */
  XMC_USBD_EnableEventOUTEP(((uint32_t)XMC_USBD_EVENT_OUT_EP_TX_COMPLET | (uint32_t)XMC_USBD_EVENT_OUT_EP_DISABLED |
		  	  	  	  	  	  (uint32_t)XMC_USBD_EVENT_OUT_EP_SETUP | (uint32_t)XMC_USBD_EVENT_OUT_EP_AHB_ERROR));

  /*in ep interrupts */
  XMC_USBD_EnableEventINEP(((uint32_t)XMC_USBD_EVENT_IN_EP_TX_COMPLET | (uint32_t)XMC_USBD_EVENT_IN_EP_DISABLED |
		  (uint32_t)XMC_USBD_EVENT_IN_EP_AHB_ERROR | (uint32_t)XMC_USBD_EVENT_IN_EP_TIMEOUT));


  /* Clear device Address */  
  dcfg.d32 = xmc_device.device_register->dcfg;
  dcfg.b.devaddr = 0U;
  xmc_device.device_register->dcfg = dcfg.d32;

  if(obj->usbd_transfer_mode == XMC_USBD_USE_FIFO)
  {
    /* Clear Empty interrupt */
    xmc_device.device_register->dtknqr4_fifoemptymsk = 0U;
  }

  xmc_device.ep[0U].outInUse = 0U;
  xmc_device.ep[0U].inInUse = 0U;

  xmc_device.DeviceEvent_cb(XMC_USBD_EVENT_RESET);

  /* clear reset intr */
  XMC_USBD_ClearEvent(XMC_USBD_EVENT_RESET);
}

/**
* @brief Handle OTG Interrupt
*
* It detects especially connect and disconnect events.
*/
static void XMC_USBD_lHandleOTGInt(void) 
{
  gotgint_data_t data;
  data.d32 = xmc_device.global_register->gotgint;
  if (data.b.sesenddet)
  {
		xmc_device.IsPowered = 0U;
		xmc_device.DeviceEvent_cb(XMC_USBD_EVENT_POWER_OFF);
  }
  XMC_USBD_lClearEventOTG(data.d32);

}

/**
 * @brief Interrupt handler for device enumeration done.
 *
 * Handles the enumeration done from dwc_otg, when the host has enumerated the device.
 */
static void XMC_USBD_lHandleEnumDone(void) 
{
  /* Normaly we need to check dctl
   * We are always fullspeed, so max it up. */
  depctl_data_t epctl;
  gusbcfg_data_t gusbcfg;
	
  epctl.d32=xmc_device.endpoint_in_register[0U]->diepctl;
  epctl.b.mps = 0x00U; /* 64 Byte, this is also automatically set for out ep */
  xmc_device.endpoint_in_register[0U]->diepctl = epctl.d32;

  /* update device connected flag */
  xmc_device.IsConnected = 1U;
  xmc_device.IsPowered = 1U;

  xmc_device.DeviceEvent_cb(XMC_USBD_EVENT_CONNECT);

  /* Set Trim */  
  gusbcfg.d32 = xmc_device.global_register->gusbcfg;
  gusbcfg.b.usbtrdtim = 9U; /* default value for LS/FS */
  xmc_device.global_register->gusbcfg = gusbcfg.d32;

  /* clear interrupt */
  XMC_USBD_ClearEvent(XMC_USBD_EVENT_ENUMDONE);
}


/**
 * @brief Handles all interrupts for all out endpoints
 *
 * The interrupt handler first checks, which endpoint has caused the interrupt and then
 * determines, which interrupt should be handled.
 */
static void XMC_USBD_lHandleOEPInt(const XMC_USBD_t *const obj) 
{
  daint_data_t daint;
  daint_data_t daintmsk;
  doepmsk_data_t doepmsk;
  doepint_data_t doepint;
  deptsiz_data_t doeptsiz;
  XMC_USBD_EP_t *ep;
  uint16_t temp;
  uint16_t temp1;
  uint16_t mask;
  uint8_t ep_num;
	
  daint.d32 = xmc_device.device_register->daint;
  
  daintmsk.d32 = xmc_device.device_register->daintmsk;
  
  doepmsk.d32 = xmc_device.device_register->doepmsk;  
  
  mask = daint.ep.out & daintmsk.ep.out;
  ep_num = 0U;
  doeptsiz.d32 = 0U;
 
  while ((uint16_t)mask >> ep_num)
  {
    temp1 = (mask >> (uint16_t)ep_num);
    temp = temp1 & 0x1U;
    if (temp)
    {
      /* load register data for endpoint */
      ep = &xmc_device.ep[ep_num];
      doepint.d32 = xmc_device.endpoint_out_register[ep_num]->doepint & doepmsk.d32;
      if(obj->usbd_transfer_mode == XMC_USBD_USE_DMA)
      {
        doeptsiz.d32 = xmc_device.endpoint_out_register[ep_num]->doeptsiz;
      }
	  /* Setup Phase Complete */
      if (doepint.b.setup)
      {
        /* ep0 not stalled any more */
        ep->isStalled = 0U;
        if(obj->usbd_transfer_mode == XMC_USBD_USE_DMA)
        {
          /* calculate size for setup packet */
		  ep->outBytesAvailable = (uint32_t)(((uint32_t)XMC_USBD_SETUP_COUNT -
		  (uint32_t)((deptsiz0_data_t*)&doeptsiz)->b.supcnt)*(uint32_t)XMC_USBD_SETUP_SIZE);
        }
		if(obj->usbd_transfer_mode == XMC_USBD_USE_FIFO)
		{
			ep->outBytesAvailable += ep->xferCount;
		}
		ep->outInUse = 0U;
		xmc_device.EndpointEvent_cb(0U,XMC_USBD_EP_EVENT_SETUP); /* signal endpoint event */
				/* clear the interrupt */
		XMC_USBD_ClearEventOUTEP((uint32_t)XMC_USBD_EVENT_OUT_EP_SETUP,ep_num);
      }
      if (doepint.b.xfercompl)
      {
        if(obj->usbd_transfer_mode == XMC_USBD_USE_DMA)
        {
          uint32_t bytes = (ep->xferLength - ep->xferCount) - doeptsiz.b.xfersize;
          ep->xferCount += bytes;
          ep->xferBuffer += bytes;
				}
        if (ep->xferTotal == ep->xferLength)
        {
          ep->outBytesAvailable = ep->xferCount;
          ep->outInUse = 0U;
          xmc_device.EndpointEvent_cb(ep_num,XMC_USBD_EP_EVENT_OUT);
        }
        else
        {
          XMC_USBD_lStartReadXfer(ep);
        }

      }

      XMC_USBD_ClearEventOUTEP(doepint.d32,ep_num);
    }
    ep_num++;
  }

  /* clear interrupt */
  XMC_USBD_ClearEvent(XMC_USBD_EVENT_OUTEP);
}

/**
 * @brief Handles all interrupts for all in endpoints
 *
 * The interrupt handler first checks, which endpoint has caused the interrupt and then
 * determines, which interrupt should be handled.
 */
static void XMC_USBD_lHandleIEPInt(const XMC_USBD_t *const obj) 
{
  XMC_USBD_EP_t *ep;
  daint_data_t daint;
  diepmsk_data_t diepmsk;
  diepint_data_t diepint;
  deptsiz_data_t dieptsiz;
  uint16_t temp;
  uint16_t temp1;
  uint16_t mask;
  uint8_t ep_num;
  uint32_t inepint;
	
  daint.d32 = xmc_device.device_register->daint;
  
  diepmsk.d32 = xmc_device.device_register->diepmsk;
  
  dieptsiz.d32 = 0U;
  mask = daint.ep.in;
  ep_num = 0U;
  
  while ((uint16_t)mask >> ep_num)
  {
    temp1 = ((uint16_t)mask >> (uint16_t)ep_num);
    temp = (uint16_t)temp1 & (uint16_t)0x1U;
    if ((uint16_t)temp)
    {
      ep = &xmc_device.ep[ep_num];
			inepint = (uint32_t)xmc_device.endpoint_in_register[ep_num]->diepint;
      diepint.d32 = inepint &
      ((((uint32_t)((uint32_t)xmc_device.device_register->dtknqr4_fifoemptymsk >> ep->address_u.address_st.number) &
                     0x1U) << 7U) | (uint32_t)diepmsk.d32);
      if(obj->usbd_transfer_mode == XMC_USBD_USE_DMA)
      {
        dieptsiz.d32 = xmc_device.endpoint_in_register[ep_num]->dieptsiz;
      }
      if(obj->usbd_transfer_mode == XMC_USBD_USE_FIFO)
      {
        if (diepint.b.emptyintr)
        {
          uint32_t bytes;
          bytes = XMC_USBD_lWriteFifo(ep);
          ep->xferCount += bytes;
          ep->xferBuffer += bytes;
        }
      }
      if (diepint.b.xfercompl)
      {
        if(obj->usbd_transfer_mode == XMC_USBD_USE_DMA)
        {
          /* update xfer values */
          if ((dieptsiz.b.pktcnt == 0U) && (dieptsiz.b.xfersize == 0U))
          {
            uint32_t Bytes = ep->xferLength - ep->xferCount;
            ep->xferCount += Bytes;
            ep->xferBuffer += Bytes;
          }
        }
        if (ep->xferTotal==ep->xferLength)
        {
          ep->inInUse = 0U;
          if(obj->usbd_transfer_mode == XMC_USBD_USE_FIFO)
          {
            /* mask fifo empty interrupt */
            xmc_device.device_register->dtknqr4_fifoemptymsk =
            (uint32_t)(xmc_device.device_register->dtknqr4_fifoemptymsk & ~(((uint32_t)1U << ep_num)));
          }
          xmc_device.EndpointEvent_cb(0x80U | ep_num,XMC_USBD_EP_EVENT_IN);
        }
        else
        {
          /* start next step of transfer */
          XMC_USBD_lStartWriteXfer(ep);
        }

      }

      XMC_USBD_ClearEventINEP((uint32_t)diepint.d32,ep_num);
    }
    ep_num++;
  }
  XMC_USBD_ClearEvent(XMC_USBD_EVENT_INEP);
}

/**
 * @brief RX Fifo interrupt handler
 *
 * This function handles the interrupt, when the rx fifo is not empty anymore.
 */
static void XMC_USBD_lHandleRxFLvl(void) 
{
  device_grxsts_data_t data;	
  data.d32 = xmc_device.global_register->grxstsp;

  switch (data.b.pktsts)
	{
    case XMC_USBD_GRXSTS_PKTSTS_GOUTNAK:
      break;
    case XMC_USBD_GRXSTS_PKTSTS_OUTCMPL:
      break;
    case XMC_USBD_GRXSTS_PKTSTS_OUTDATA:
      XMC_USBD_lReadFifo((uint32_t)data.b.epnum,(uint32_t)data.b.bcnt);
      break;
    case XMC_USBD_GRXSTS_PKTSTS_SETUP:
      XMC_USBD_lReadFifo((uint32_t)data.b.epnum,(uint32_t)data.b.bcnt);
      break;
    case XMC_USBD_GRXSTS_PKTSTS_SETUPCMPL:
      break;
    default:
      break;
	}
  /* no need to clear */
}

/**
 * @brief Global interrupt handler
 *
 * The handler first checks, which global interrupt has caused the interrupt
 * and then dispatches interrupt to the corresponding sub-handler.
 */
void XMC_USBD_IRQHandler(const XMC_USBD_t *const obj) 
{
  gintmsk_data_t gintmsk;
  gintsts_data_t data;
	
  gintmsk.d32 = xmc_device.global_register->gintmsk;
  data.d32 = xmc_device.global_register->gintsts & gintmsk.d32;
	
  if (data.b.sofintr)
  {
    xmc_device.DeviceEvent_cb(XMC_USBD_EVENT_SOF);
    XMC_USBD_ClearEvent(XMC_USBD_EVENT_SOF);
  }
  if(obj->usbd_transfer_mode == XMC_USBD_USE_FIFO)
  {
    if (data.b.rxstsqlvl)
    {
      /* Masked that interrupt so its only done once */
      gintmsk.b.rxstsqlvl = 0U;
      xmc_device.global_register->gintmsk = gintmsk.d32;
      XMC_USBD_lHandleRxFLvl(); /* handle the interrupt */
      gintmsk.b.rxstsqlvl = 1U;
      xmc_device.global_register->gintmsk = gintmsk.d32;
    }
  }
  if (data.b.erlysuspend)
  {
    XMC_USBD_ClearEvent(XMC_USBD_EVENT_EARLYSUSPEND);
  }
  if (data.b.usbsuspend)
  {
    xmc_device.DeviceEvent_cb(XMC_USBD_EVENT_SUSPEND);
    XMC_USBD_ClearEvent(XMC_USBD_EVENT_SUSPEND);
  }
  if (data.b.wkupintr)
  {
    xmc_device.DeviceEvent_cb(XMC_USBD_EVENT_REMOTE_WAKEUP);
    XMC_USBD_ClearEvent(XMC_USBD_EVENT_REMOTE_WAKEUP);
  }
  if (data.b.sessreqintr)
  {
    xmc_device.IsPowered = 1U;
    xmc_device.DeviceEvent_cb(XMC_USBD_EVENT_POWER_ON);
    XMC_USBD_ClearEvent(XMC_USBD_EVENT_POWER_ON);
  }
  if (data.b.usbreset)
  {
    XMC_USBD_lHandleUSBReset(obj);
  }
  if (data.b.enumdone)
  {
    XMC_USBD_lHandleEnumDone();
  }
  if (data.b.inepint)
  {
    XMC_USBD_lHandleIEPInt(obj);
  }
  if (data.b.outepintr)
  {
		XMC_USBD_lHandleOEPInt(obj);
  }
	if (data.b.otgintr)
  {
	  XMC_USBD_lHandleOTGInt();
  }

}


/*******************************************************************************
 * API IMPLEMENTATION
 *******************************************************************************/
/**
 * Enables the USB0 module
 **/
void XMC_USBD_Enable(void) 
{
#if defined(CLOCK_GATING_SUPPORTED)
  XMC_SCU_CLOCK_UngatePeripheralClock(XMC_SCU_PERIPHERAL_CLOCK_USB0);
#endif
  /* Reset and power up */
  XMC_SCU_RESET_DeassertPeripheralReset(XMC_SCU_PERIPHERAL_RESET_USB0);
  XMC_SCU_POWER_EnableUsb();
}

/**
 * Disables the USB0 module
 **/
void XMC_USBD_Disable(void) 
{
  /* Clear Reset and power up */
  XMC_SCU_RESET_AssertPeripheralReset(XMC_SCU_PERIPHERAL_RESET_USB0);
#if defined(CLOCK_GATING_SUPPORTED)
  XMC_SCU_CLOCK_GatePeripheralClock(XMC_SCU_PERIPHERAL_CLOCK_USB0);
#endif
  XMC_SCU_POWER_DisableUsb();
}

/**
 * Clear the USB device event
 **/
void XMC_USBD_ClearEvent(const XMC_USBD_EVENT_t event) 
{
  gintsts_data_t clear;
  clear.d32 = 0U;
  switch(event)
  {
    case (XMC_USBD_EVENT_POWER_ON):
	  clear.b.sessreqintr = 1U;
	  break;
    case (XMC_USBD_EVENT_RESET):
	  clear.b.usbreset = 1U;
	  break;
    case (XMC_USBD_EVENT_SUSPEND):
	  clear.b.usbsuspend = 1U;
	  break;
    case (XMC_USBD_EVENT_RESUME):
	  clear.b.wkupintr = 1U;
	  break;
    case (XMC_USBD_EVENT_REMOTE_WAKEUP):
	  clear.b.wkupintr = 1U;
	  break;
    case (XMC_USBD_EVENT_SOF):
	  clear.b.sofintr = 1U;
	  break;
    case (XMC_USBD_EVENT_EARLYSUSPEND):
	  clear.b.erlysuspend = 1U;
	  break;
    case (XMC_USBD_EVENT_ENUMDONE):
	  clear.b.enumdone = 1U;
	  break;
    case (XMC_USBD_EVENT_OUTEP):
	  clear.b.outepintr = 1U;
	  break;
		default:
		break;
	}
	xmc_device.global_register->gintsts = clear.d32;
}

/**
 * Clear the USB OTG events
 **/
static void XMC_USBD_lClearEventOTG(uint32_t event)
{
  gotgint_data_t clear = { .d32 = 0U};
  clear.d32 = event;
  xmc_device.global_register->gotgint = clear.d32;
}

/**
 * Clear the USB IN EP events
 **/
void XMC_USBD_ClearEventINEP(uint32_t event,const uint8_t ep_num)
{
  diepint_data_t clear;
  clear.d32 = event;
  xmc_device.endpoint_in_register[ep_num]->diepint = clear.d32;
}

/**
 * Clear the USB OUT EP events
 **/
void XMC_USBD_ClearEventOUTEP(uint32_t event,const uint8_t ep_num)
{
  doepint_data_t clear;
  clear.d32 = event;
  xmc_device.endpoint_out_register[ep_num]->doepint = clear.d32;
}

/**
 * Enable the USB OUT EP events
 **/
void XMC_USBD_EnableEventOUTEP(uint32_t event)
{
	doepint_data_t doepint;
	doepint.d32 = event;
	xmc_device.device_register->doepmsk |= doepint.d32;
}

/**
 * Enable the USB IN EP events
 **/
void XMC_USBD_EnableEventINEP(uint32_t event)
{
	diepint_data_t diepint;
	diepint.d32 = event;
	xmc_device.device_register->diepmsk |= diepint.d32;
}

/**
 * Gets the USB device capabilities
 **/
XMC_USBD_CAPABILITIES_t XMC_USBD_GetCapabilities()
{
  XMC_USBD_CAPABILITIES_t cap={0U};
  cap.event_connect = 1U;
  cap.event_disconnect = 1U;
#if UC_SERIES == 45
  cap.event_power_off = 1U;
  cap.event_power_on = 1U;
#else
  cap.event_power_off = 0U;
  cap.event_power_on = 0U;
#endif
  cap.event_high_speed = 0U;
  cap.event_remote_wakeup = 1U;
  cap.event_reset = 1U;
  cap.event_resume = 1U;
  cap.event_suspend = 1U;
  cap.reserved = 0U;
  return cap;
}

/**
 * Initializes the USB device
 **/
XMC_USBD_STATUS_t XMC_USBD_Init(XMC_USBD_t *obj)
{
  uint8_t *XMC_USBD_BASE_ADDRESS;
  uint32_t i;
  gahbcfg_data_t gahbcfg;
  gusbcfg_data_t gusbcfg;
  dcfg_data_t dcfg;
  dctl_data_t dctl;
  gintmsk_data_t gintmsk;

  XMC_ASSERT("XMC_USBD_Init: obj.usbd_max_num_eps not of type XMC_USBD_MAX_NUM_EPS_t",
 		      XMC_USBD_CHECK_INPUT_MAX_NUM_EPS(obj->usbd_max_num_eps))
  
  XMC_USBD_Enable();
  
  usbd_init = obj;

  /* Filling out buffer size */
  for(i = 0U;i < (uint32_t)XMC_USBD_NUM_EPS;i++)
  {
	  XMC_USBD_EP_OUT_BUFFERSIZE[i] = XMC_USBD_EP0_BUFFER_SIZE;
	  XMC_USBD_EP_IN_BUFFERSIZE[i] 	= XMC_USBD_EP0_BUFFER_SIZE;
  }

  /* clear device status */
  memset((void*)&xmc_device,0x0U,sizeof(XMC_USBD_DEVICE_t));

  /* assign callbacks */
  xmc_device.DeviceEvent_cb = obj->cb_xmc_device_event;
  xmc_device.EndpointEvent_cb = obj->cb_endpoint_event;
  XMC_USBD_BASE_ADDRESS = (uint8_t *)(obj->usbd);
  /* assign register address */
  xmc_device.global_register = (dwc_otg_core_global_regs_t*)(obj->usbd);
  xmc_device.device_register = ((dwc_otg_device_global_regs_t*)(XMC_USBD_BASE_ADDRESS + DWC_DEV_GLOBAL_REG_OFFSET));
  for (i = 0U;i < (uint32_t)XMC_USBD_NUM_EPS;i++)
  {
    xmc_device.endpoint_in_register[i] = (dwc_otg_dev_in_ep_regs_t*)(XMC_USBD_BASE_ADDRESS + DWC_DEV_IN_EP_REG_OFFSET +
    ((uint32_t)DWC_EP_REG_OFFSET*i));
  }
  for (i = 0U;i < (uint32_t)XMC_USBD_NUM_EPS;i++)
  {
    xmc_device.endpoint_out_register[i] = (dwc_otg_dev_out_ep_regs_t*)(XMC_USBD_BASE_ADDRESS +
    									  DWC_DEV_OUT_EP_REG_OFFSET +
    									  ((uint32_t)DWC_EP_REG_OFFSET*i));
  }
  for (i = 0U;i < (uint32_t)XMC_USBD_NUM_TX_FIFOS;i++)
  {
    xmc_device.fifo[i] = (uint32_t*)(XMC_USBD_BASE_ADDRESS +
    						XMC_USBD_TX_FIFO_REG_OFFSET +
    						(i * XMC_USBD_TX_FIFO_OFFSET));
  }
  /* obj data structure for endpoint 0 */
  /* Done by driver core */
  /* configure ahb details */  
  gahbcfg.d32 = xmc_device.global_register->gahbcfg;
  gahbcfg.b.glblintrmsk = 1U; /* enable interrupts ( global mask ) */
  gahbcfg.b.nptxfemplvl_txfemplvl = 1U;
  if(obj->usbd_transfer_mode == XMC_USBD_USE_DMA)
  {
    /* Enable dma if needed */
    gahbcfg.b.dmaenable = 1U; /* enable dma if needed */
  }
  else
  {
    gahbcfg.b.dmaenable = 0U;
  }
  xmc_device.global_register->gahbcfg = gahbcfg.d32;
  /* configure usb details */  
  gusbcfg.d32= xmc_device.global_register->gusbcfg;
  gusbcfg.b.force_dev_mode = 1U; /* force us into device mode */
  gusbcfg.b.srpcap = 1U; /* enable session request protocoll */
  xmc_device.global_register->gusbcfg = gusbcfg.d32;

  /* Device init */
  /* configure device speed */  
  dcfg.d32 = xmc_device.device_register->dcfg;
  dcfg.b.devspd = XMC_USBD_DCFG_DEVSPD_FS;
  dcfg.b.descdma = 0U;
  xmc_device.device_register->dcfg = dcfg.d32;
  /* configure device functions */  
  dctl.d32 = xmc_device.device_register->dctl;
  dctl.b.sftdiscon = 1U; /* disconnect the device until its connected by the user */
  /* all other config is done by default register value */
  xmc_device.device_register->dctl = dctl.d32;
  /* flush the fifos for proper operation */
  XMC_USBD_lFlushTXFifo((uint8_t)0x10U); /* 0x10 == all fifos, see doc */
  XMC_USBD_lFlushRXFifo();
  /* Enable Global Interrupts */
  /* clear interrupt status bits prior to unmasking */
  xmc_device.global_register->gintmsk = 0U; /* disable all interrupts */
  xmc_device.global_register->gintsts = 0xFFFFFFFFU; /* clear all interrupts */
 
  gintmsk.d32 = 0U;
  /* enable common interrupts */
  gintmsk.b.modemismatch = 1U;
  gintmsk.b.otgintr = 1U;
  gintmsk.b.sessreqintr = 1U;
  /* enable device interrupts */
  gintmsk.b.usbreset = 1U;
  gintmsk.b.enumdone = 1U;
  gintmsk.b.erlysuspend = 1U;
  gintmsk.b.usbsuspend = 1U;
  gintmsk.b.wkupintr = 1U;
  gintmsk.b.sofintr = 1U;
  if(obj->usbd_transfer_mode == XMC_USBD_USE_FIFO)
  {
    gintmsk.b.rxstsqlvl = 1U;
  }
  gintmsk.b.outepintr = 1U;
  gintmsk.b.inepintr = 1U;
  xmc_device.global_register->gintmsk = gintmsk.d32;
  return XMC_USBD_STATUS_OK;
}

/**
 * Uninitializes the USB device
 **/
XMC_USBD_STATUS_t XMC_USBD_Uninitialize() 
{
  /* Disconnect the device */
  dctl_data_t dctl;
	dctl.d32 = xmc_device.device_register->dctl;
  dctl.b.sftdiscon = 1U;
  xmc_device.device_register->dctl = dctl.d32;
  /* clean up */
  memset((void*)&xmc_device,0U,sizeof(xmc_device));
  return XMC_USBD_STATUS_OK;
}

/**
 * Connects the USB device to host
 **/
XMC_USBD_STATUS_t XMC_USBD_DeviceConnect() 
{
  /* Just disable softdisconnect */
  dctl_data_t dctl;
	dctl.d32 = xmc_device.device_register->dctl;
  dctl.b.sftdiscon = 0U;
  xmc_device.device_register->dctl = dctl.d32;
  return XMC_USBD_STATUS_OK;
}

/**
 * Disconnects the USB device from host
 **/
XMC_USBD_STATUS_t XMC_USBD_DeviceDisconnect() 
{
  dctl_data_t dctl;
	dctl.d32 = xmc_device.device_register->dctl;
  dctl.b.sftdiscon = 1U;
  xmc_device.device_register->dctl = dctl.d32;
  return XMC_USBD_STATUS_OK;
}

/**
 * Gets the USB device state.
 **/
XMC_USBD_STATE_t XMC_USBD_DeviceGetState(const XMC_USBD_t *const obj) 
{
  XMC_USBD_STATE_t state={0U};
  state.speed = XMC_USBD_SPEED_FULL;
  state.connected = xmc_device.IsConnected;
  state.active = XMC_USBD_lDeviceActive(obj);
  state.powered = xmc_device.IsPowered;
  return state;
}

/**
 * Prepares the endpoint to read next OUT packet
 **/
XMC_USBD_STATUS_t XMC_USBD_EndpointReadStart(const uint8_t ep_addr, uint32_t size) 
{
  XMC_USBD_EP_t *ep = &xmc_device.ep[ep_addr & (uint8_t)XMC_USBD_EP_NUM_MASK];
  XMC_USBD_STATUS_t result;
	
  if (ep->outInUse || !ep->isConfigured)
  {
    result = XMC_USBD_STATUS_ERROR;
  }
  else
  {
    /* short the length to buffer size if needed */
    if (size > ep->outBufferSize)
    {
      size = ep->outBufferSize;
    }
    /* set ep values */
    ep->xferTotal = size;
    ep->xferCount = 0U;
    ep->xferLength = 0U;
    ep->xferBuffer = ep->outBuffer;
    ep->outBytesAvailable = 0U;
    XMC_USBD_lStartReadXfer(ep);
    result= XMC_USBD_STATUS_OK;
  }
  return result;
}

/**
 * Reads the number of bytes from the USB OUT endpoint
 **/
int32_t XMC_USBD_EndpointRead(const uint8_t ep_num,uint8_t * buffer,uint32_t length) 
{
  XMC_USBD_EP_t *ep = &xmc_device.ep[ep_num];
  if (length > ep->outBytesAvailable)
  {
    length = ep->outBytesAvailable;
  }
  memcpy(buffer,&ep->outBuffer[ep->outOffset],length);
  ep->outBytesAvailable -= length;
  if (ep->outBytesAvailable)
  {
    ep->outOffset += length;
  }
  else
  {
    ep->outOffset = 0U;
  }
  return (int32_t)length;
}

/**
 * Writes number of bytes in to the USB IN endpoint.
 **/
int32_t XMC_USBD_EndpointWrite(const uint8_t ep_num,const uint8_t * buffer,uint32_t length) 
{
  XMC_USBD_EP_t * ep = &xmc_device.ep[ep_num & (uint8_t)XMC_USBD_EP_NUM_MASK];
  int32_t result;
  if (!ep->isConfigured)
  {
    result = (int32_t)XMC_USBD_STATUS_ERROR;
  }
  else if (ep->inInUse == 1U)
  {
    result=(int32_t)0;
  }
  else
  {
    if (length > ep->inBufferSize)
    {
      length = ep->inBufferSize;
    }
    /* copy data into input buffer for DMA and FIFO mode */
		memcpy(ep->inBuffer,(const void *)buffer,length);
		ep->xferBuffer = ep->inBuffer;
    ep->xferTotal = length;
    /* set transfer values */
    ep->xferLength = 0U;
    ep->xferCount = 0U;
    ep->inInUse = 1U;
    /* start the transfer */
    XMC_USBD_lStartWriteXfer(ep);
    result=(int32_t)ep->xferTotal;
  }
  return result;
}

/**
 * Sets the USB device address.
 **/
XMC_USBD_STATUS_t XMC_USBD_DeviceSetAddress(const uint8_t address,const XMC_USBD_SET_ADDRESS_STAGE_t stage) 
{
  dcfg_data_t data;
  data.d32 = xmc_device.device_register->dcfg;
  if (stage == XMC_USBD_SET_ADDRESS_STAGE_SETUP)
  {
    data.b.devaddr = address;
    xmc_device.device_register->dcfg = data.d32;
  }
	return XMC_USBD_STATUS_OK;
}

/**
 * Set/clear stall on the selected endpoint.
 **/
XMC_USBD_STATUS_t XMC_USBD_EndpointStall(const uint8_t ep_addr, const bool stall) 
{
  depctl_data_t data;
  XMC_USBD_EP_t *ep = &xmc_device.ep[(ep_addr & (uint8_t)XMC_USBD_EP_NUM_MASK)];
  if (stall)
  {
    if (ep_addr & (uint8_t)XMC_USBD_ENDPOINT_DIRECTION_MASK)
    {
      /*set stall bit */
      data.d32 = xmc_device.endpoint_in_register[ep->address_u.address_st.number]->diepctl;
      data.b.stall = 1U;
      xmc_device.endpoint_in_register[ep->address_u.address_st.number]->diepctl = data.d32;
    }
    else
    {
      /*set stall bit */
      data.d32 = xmc_device.endpoint_out_register[ep->address_u.address_st.number]->doepctl;
      data.b.stall = 1U;
      xmc_device.endpoint_out_register[ep->address_u.address_st.number]->doepctl = data.d32;
    }
    ep->isStalled = 1U;
  }
  else
  {
    /* just clear stall bit */
		if (ep_addr & (uint8_t)XMC_USBD_ENDPOINT_DIRECTION_MASK)
		{
			data.d32 = xmc_device.endpoint_in_register[ep->address_u.address_st.number]->diepctl;
			data.b.stall = 0U;
			data.b.setd0pid = 1U; /* reset pid to 0 */
			xmc_device.endpoint_in_register[ep->address_u.address_st.number]->diepctl = data.d32;
		}
		else
		{
			data.d32 = xmc_device.endpoint_out_register[ep->address_u.address_st.number]->doepctl;
			data.b.stall = 0U;
			data.b.setd0pid = 1U; /* reset pid to 0 */
			xmc_device.endpoint_out_register[ep->address_u.address_st.number]->doepctl = data.d32;
		}
		ep->isStalled = 0U;
	}
	return XMC_USBD_STATUS_OK;
}

/**
 * Aborts the data transfer on the selected endpoint
 **/
XMC_USBD_STATUS_t XMC_USBD_EndpointAbort(const uint8_t ep_addr) {
  XMC_USBD_EP_t *ep = &xmc_device.ep[ep_addr & (uint8_t)XMC_USBD_ENDPOINT_NUMBER_MASK];
  if (ep->address_u.address_st.direction)
  {
    ep->inInUse = 0U;
  }
  if (!ep->address_u.address_st.direction)
  {
    ep->outInUse = 0U;
  }
  ep->isStalled = 0U;
  ep->outBytesAvailable = 0U;
  ep->outOffset = 0U;
  ep->xferLength = 0U;
  ep->xferCount = 0U;
  ep->xferTotal = 0U;

  return XMC_USBD_STATUS_OK;
}

/**
 * Configures the given endpoint
 **/
XMC_USBD_STATUS_t XMC_USBD_EndpointConfigure(const uint8_t ep_addr,
		                                     const XMC_USBD_ENDPOINT_TYPE_t ep_type,
		                                     const uint16_t ep_max_packet_size) 
{
  daint_data_t daintmsk;
  XMC_USBD_EP_t *ep;
  daintmsk.d32 = xmc_device.device_register->daintmsk;  
  ep =&xmc_device.ep[ep_addr & (uint32_t)XMC_USBD_ENDPOINT_NUMBER_MASK];
  memset((void*)ep,0x0U,sizeof(XMC_USBD_EP_t)); /* clear endpoint structure */
  /* do ep configuration */
  ep->address_u.address = ep_addr;
  ep->isConfigured = 1U;
  ep->maxPacketSize = (uint8_t)ep_max_packet_size;
  if (ep->address_u.address != 0U)
  {
    ep->maxTransferSize = (uint32_t)XMC_USBD_MAX_TRANSFER_SIZE;
  }
  else
  {
    ep->maxTransferSize = (uint32_t)XMC_USBD_MAX_TRANSFER_SIZE_EP0;
  }
  /* transfer buffer */
  ep->inBuffer = XMC_USBD_EP_IN_BUFFER[ep->address_u.address_st.number];
  ep->outBuffer = XMC_USBD_EP_OUT_BUFFER[ep->address_u.address_st.number];
  /* buffer size*/
  ep->inBufferSize = XMC_USBD_EP_IN_BUFFERSIZE[ep->address_u.address_st.number];
  ep->outBufferSize = XMC_USBD_EP_OUT_BUFFERSIZE[ep->address_u.address_st.number];
  /* is in */
  if ((ep->address_u.address_st.direction == 1U) || (ep_type == XMC_USBD_ENDPOINT_TYPE_CONTROL))
  {
    depctl_data_t data;
    data.d32 = xmc_device.endpoint_in_register[ep->address_u.address_st.number]->diepctl;
		/*enable endpoint */
		data.b.usbactep = 1U;
		/* set ep type */
		data.b.eptype = (uint8_t)ep_type;
		/* set mps */
    if (ep_type == XMC_USBD_ENDPOINT_TYPE_CONTROL)
    {
			switch(ep_max_packet_size)
			{
				case (64U):
				data.b.mps = 0x0U;
				break;
				case (32U):
				data.b.mps = 0x1U;
				break;
				case (16U):
				data.b.mps = 0x2U;
				break;
				case (8U):
				data.b.mps = 0x3U;
				break;
				default:
				break;
			}
		}
		else
		{
			data.b.mps = ep_max_packet_size;
		}
		/* set first data0 pid */
		data.b.setd0pid = 1U;
		/* clear stall */
		data.b.stall = 0U;
		/* set tx fifo */
		ep->txFifoNum = XMC_USBD_lAssignTXFifo(); /* get tx fifo */
		data.b.txfnum = ep->txFifoNum;
		xmc_device.endpoint_in_register[ep->address_u.address_st.number]->diepctl = data.d32; /* configure endpoint */
		daintmsk.ep.in |= (uint16_t)((uint16_t)1U << (uint8_t)ep->address_u.address_st.number); /* enable interrupts for endpoint */
  }
  if ((ep->address_u.address_st.direction == 0U) || (ep_type == XMC_USBD_ENDPOINT_TYPE_CONTROL))
  {
    /* is out */
		depctl_data_t data;
		data.d32 = xmc_device.endpoint_out_register[ep->address_u.address_st.number]->doepctl;
		/*enable endpoint */
		data.b.usbactep = 1U;
		/* set ep type */
		data.b.eptype = (uint8_t)ep_type;
    /* set mps */
    if (ep_type == XMC_USBD_ENDPOINT_TYPE_CONTROL)
		{
			switch(ep_max_packet_size)
			{
				case (64U):
					data.b.mps = 0x0U;
					break;
				case (32U):
					data.b.mps = 0x1U;
					break;
				case (16U):
					data.b.mps = 0x2U;
					break;
				case (8U):
					data.b.mps = 0x3U;
					break;
				default:
					break;
			}
		}
		else
		{
			data.b.mps = ep_max_packet_size;
		}
		/* set first data0 pid */
		data.b.setd0pid = 1U;
		/* clear stall */
		data.b.stall =(uint8_t) 0U;
		xmc_device.endpoint_out_register[ep->address_u.address_st.number]->doepctl = data.d32; /* configure endpoint */
		daintmsk.ep.out |=(uint16_t) ((uint16_t)1U << (uint8_t)ep->address_u.address_st.number); /* enable interrupts */
  }
  xmc_device.device_register->daintmsk = daintmsk.d32;
  return XMC_USBD_STATUS_OK;
}

/**
 * Unconfigure the selected endpoint.
 **/
XMC_USBD_STATUS_t XMC_USBD_EndpointUnconfigure(const uint8_t ep_addr) 
{
  XMC_USBD_EP_t *ep = &xmc_device.ep[ep_addr & (uint8_t)XMC_USBD_ENDPOINT_NUMBER_MASK];
  depctl_data_t data;
	daint_data_t daintmsk;
	XMC_USBD_STATUS_t result;
	uint32_t number_temp;
  data.d32 = 0U;  
  daintmsk.d32 = xmc_device.device_register->daintmsk;  
  number_temp = (uint32_t)((uint32_t)1U << (uint8_t)ep->address_u.address_st.number);
  /* if not configured return an error */
  if (!ep->isConfigured)
  {
     result = XMC_USBD_STATUS_ERROR;
  }
  else
  {
    /* disable the endpoint, deactivate it and only send naks */
    data.b.usbactep = 0U;
    data.b.epdis =  1U;
    data.b.snak =  1U;
    data.b.stall =  0U;
    ep->isConfigured =  0U;
    ep->isStalled =  0U;
    ep->outInUse =  0U;
    ep->inInUse =  0U;
    /* chose register based on the direction. Control Endpoint need both */
    if ((ep->address_u.address_st.direction == 1U) || (ep->type == (uint8_t)XMC_USBD_ENDPOINT_TYPE_CONTROL))
    {
      /* disable endpoint configuration */
      xmc_device.endpoint_in_register[ep->address_u.address_st.number]->diepctl = data.d32;
      /* disable interrupts */
      daintmsk.ep.in = (uint16_t)((uint32_t)daintmsk.ep.in & (~(uint32_t)number_temp));
    }
    if ((ep->address_u.address_st.direction == 0U) || (ep->type == (uint8_t)XMC_USBD_ENDPOINT_TYPE_CONTROL))
    {
      xmc_device.endpoint_out_register[ep->address_u.address_st.number]->doepctl = data.d32;
      daintmsk.ep.out = (uint16_t)((uint32_t)daintmsk.ep.out & (~(uint32_t)number_temp));
      if(usbd_init->usbd_transfer_mode == XMC_USBD_USE_FIFO)
      {
        xmc_device.device_register->dtknqr4_fifoemptymsk &= ~number_temp;
      }
    }
    xmc_device.device_register->daintmsk = daintmsk.d32;
    XMC_USBD_lUnassignFifo(ep->txFifoNum); /* free fifo */
    result = XMC_USBD_STATUS_OK;
  }
  return result;
}

/**
 * Gets the current USB frame number
 **/
uint16_t XMC_USBD_GetFrameNumber(void) 
{
  uint16_t result;
  dsts_data_t dsts;
  dsts.d32 = xmc_device.device_register->dsts;
  result = (uint16_t)dsts.b.soffn;
  return result;
}

/**
 * Gets the USB speed enumeration completion status.
 * This should not be used for the actual USB enumeration completion status. For the actual USB enumeration status,
 * the application layer should check for the completion of USB standard request Set configuration.
 **/
uint32_t XMC_USBD_IsEnumDone(void)
{
  return (uint32_t)((uint8_t)xmc_device.IsConnected && (uint8_t)xmc_device.IsPowered);
}

/***
 * MISRA C 2004 Deviations
 *  
 *	1. cast from pointer to pointer [MISRA 2004 Rule 11.4]
 *  2. cast from pointer to unsigned int [Encompasses MISRA 2004 Rule 11.1], [MISRA 2004 Rule 11.3]
 *  3. call to function 'memset()' not made in the  presence of a prototype [MISRA 2004 Rule 8.1]
 *  4. No explicit type for symbol '_Bool', int assumed 
 */
#endif /* defined(USB0) */
