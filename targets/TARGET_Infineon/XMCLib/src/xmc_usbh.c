/**
 * @file xmc_usbh.c
 * @date 2016-06-30
 *
 * @cond
 **********************************************************************************
 * XMClib v2.1.16 - XMC Peripheral Driver Library
 *
 * Copyright (c) 2015-2017, Infineon Technologies AG
 * All rights reserved.                        
 *                                             
 * Redistribution and use in source and binary forms, with or without modification,are permitted provided that the 
 * following conditions are met:   
 *                                                                              
 * Redistributions of source code must retain the above copyright notice, this list of conditions and the following 
 * disclaimer.                        
 * 
 * Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following
 * disclaimer in the documentation and/or other materials provided with the distribution.                       
 * 
 * Neither the name of the copyright holders nor the names of its contributors may be used to endorse or promote 
 * products derived from this software without specific prior written permission.                                           
 *                                                                              
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, 
 * INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE  
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE  FOR ANY DIRECT, INDIRECT, INCIDENTAL, 
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR  
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, 
 * WHETHER IN CONTRACT, STRICT LIABILITY,OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE 
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.                                                  
 *                                                                              
 * (To improve the quality of the software, users are encouraged to share modifications, enhancements or bug fixes with 
 * Infineon Technologies AG dave@infineon.com).                                                          
 *
 *********************************************************************************************************************
 *
 * Change History
 * --------------
 *
 * 2016-06-30:
 *     - Initial <br>
 * 2016-09-01:
 *     - Removed Keil specific exclusion<br>
 *      
 * @endcond 
 *
 */

#include <stdint.h>
#include <string.h>

#include "xmc_usbh.h"

#if((UC_SERIES == XMC45) || (UC_SERIES == XMC44) || (UC_SERIES == XMC43) || (UC_SERIES == XMC47) || (UC_SERIES == XMC48))

/*Function provides transfer result*/
static uint32_t XMC_USBH_PipeTransferGetResult (XMC_USBH_PIPE_HANDLE pipe_hndl);
/*Updates the power state of the driver*/
static int32_t XMC_USBH_PowerControl (XMC_USBH_POWER_STATE_t state);

/*********************************************************** USBH Driver ***************************************************************** */

/*Macro to represent USB host driver version*/
#define XMC_USBH_DRV_VERSION ((uint16_t)((uint16_t)XMC_LIB_MINOR_VERSION << 8U)|XMC_LIB_PATCH_VERSION)
/*Macro used to gate PHY clock and AHB clock*/
#define XMC_USBH_PHY_CLK_STOP  (0x03U)
/*Macro used to ungate PHY clock and AHB clock*/
#define XMC_USBH_PHY_CLK_UNGATE  (0x100U)

/* Driver Version */
static const XMC_USBH_DRIVER_VERSION_t xmc_usbh_driver_version = { XMC_USBH_API_VERSION, XMC_USBH_DRV_VERSION };

/*Variables to hold selected VBUS port pin*/
XMC_GPIO_PORT_t * VBUS_port = XMC_GPIO_PORT3;
uint32_t VBUS_pin = 2U;

/*Array to track nack events on each pipe*/
bool is_nack[USBH0_MAX_PIPE_NUM];

/* Driver Capabilities */
static const XMC_USBH_CAPABILITIES_t xmc_usbh_driver_capabilities = {
  0x0001U,      /* Root HUB available Ports Mask */
  0U,           /* Automatic SPLIT packet handling */
  1U,           /* Signal Connect event */
  1U,           /* Signal Disconnect event */
  0U            /* Signal Overcurrent event */
};
/* Driver state and registers */
static XMC_USBH0_DEVICE_t XMC_USBH0_device/* __attribute__((section ("RW_IRAM1")))*/ = {
   (USB0_GLOBAL_TypeDef *)(USB0_BASE),    /** Global register interface            */
   ((USB0_CH_TypeDef *)(USB0_CH0_BASE)),  /** Host channel interface               */
   0,                                     /** Port event callback; set during init */
   0,                                     /** Pipe event callback; set during init */
   false,                                 /** init status */
   XMC_USBH_POWER_OFF,                    /** USB Power status */
   false                                  /** Port reset state */
};

/*USB host pipe information. The array stores information related to packet id, data toggle,
 * pending data transfer information, periodic transfer interval, received data size etc for each
 * pipe.*/
volatile XMC_USBH0_pipe_t pipe[USBH0_MAX_PIPE_NUM];

/* FIFO sizes in bytes (total available memory for FIFOs is 1.25 kB) */
#define RX_FIFO_SIZE           (1128U)   /* RxFIFO size */
#define TX_FIFO_SIZE_NON_PERI  (64U)     /* Non-periodic Tx FIFO size */
#define TX_FIFO_SIZE_PERI      (1024U)   /* Periodic Tx FIFO size */

/*Stores data FIFO pointer for each pipe*/
static uint32_t *XMC_USBH0_dfifo_ptr[USBH0_MAX_PIPE_NUM];

/* Local functions */
/**
 * @param   enable Enable (XMC_USBH_CLOCK_GATING_ENABLE) or disable(XMC_USBH_CLOCK_GATING_DISABLE) clock gating
 * @return  None
 * \par<b>Description:</b><br>
 * Enable/disable clock gating depending if feature is supported.
*/
__INLINE static void XMC_lClockGating(uint8_t enable)
{
#if defined(CLOCK_GATING_SUPPORTED)
if (enable == XMC_USBH_CLOCK_GATING_ENABLE)
    XMC_SCU_CLOCK_GatePeripheralClock(XMC_SCU_PERIPHERAL_CLOCK_USB0);
   if (enable == XMC_USBH_CLOCK_GATING_DISABLE)
    XMC_SCU_CLOCK_UngatePeripheralClock(XMC_SCU_PERIPHERAL_CLOCK_USB0);
#endif
  return;
}

/**
 * @param   ptr_ch Pointer to Channel
 * @return  None
 * \par<b>Description:</b><br>
 * Triggers halt of a channel.
*/
__INLINE static void XMC_lTriggerHaltChannel(USB0_CH_TypeDef *ptr_ch)
{
  ptr_ch->HCINTMSK = USB_CH_HCINT_ChHltd_Msk; /* Enable halt interrupt */
  ptr_ch->HCCHAR  |= (uint32_t)(USB_CH_HCCHAR_ChEna_Msk | USB_CH_HCCHAR_ChDis_Msk);
  return;
}

/**
 * @param   ptr_pipe Pointer to Pipe
 * @param   ptr_ch   Pointer to Channel
 * @return  bool \n
 *          true = success,\n
 *          false = fail
 * \par<b>Description:</b><br>
 * Start transfer on Pipe. The function uses transfer complete interrupts to transfer data more than maximum
 * packet size. It takes care of updating data toggle information in subsequent packets related to the same data transfer.
*/
static bool XMC_lStartTransfer (XMC_USBH0_pipe_t *ptr_pipe, USB0_CH_TypeDef *ptr_ch) {
  uint32_t  hcchar;
  uint32_t  hctsiz;
  uint32_t  hcintmsk;
  uint32_t  num_remaining_transfer;
  uint32_t  num_remaining_fifo;
  uint32_t  num_remaining_queue;
  uint32_t  txsts = 0U;
  uint32_t  pckt_num;
  uint32_t  max_pckt_size;
  uint8_t   *ptr_src = ptr_pipe->data;
  uint32_t  *ptr_dest = NULL;
  uint16_t  cnt;
  uint32_t loc_index;
  bool status;

  if (!(XMC_USBH0_device.global_register->HPRT & USB_HPRT_PrtConnSts_Msk))
  {
    status = false;
  }
  else
  {
    /* Save channel characteristic register to local variable */
    hcchar   = ptr_ch->HCCHAR;
    /* Save transfer size register to local variable */
    hctsiz   = ptr_ch->HCTSIZ_BUFFERMODE;
    hcintmsk = 0U;
    cnt      = 0U;

    /* Prepare transfer */
    /* Reset EPDir (transfer direction = output) and enable channel */
    hcchar &= (uint32_t)(~(uint32_t)(USB_CH_HCCHAR_EPDir_Msk | USB_CH_HCCHAR_ChDis_Msk));
    hcchar |= (uint32_t)USB_CH_HCCHAR_ChEna_Msk;

    /* Enable default interrupts needed for all transfers */
    hcintmsk  = (USB_CH_HCINTMSK_XactErrMsk_Msk  |
                 USB_CH_HCINTMSK_XferComplMsk_Msk |
                 USB_CH_HCINTMSK_NakMsk_Msk    |
                 USB_CH_HCINTMSK_StallMsk_Msk)  ;
    /* Keep PID */
    hctsiz &=  (uint32_t)USB_CH_HCTSIZ_BUFFERMODE_Pid_Msk;

    /* Packet specific setup */
    switch (ptr_pipe->packet & XMC_USBH_PACKET_TOKEN_Msk) {
      case XMC_USBH_PACKET_IN:
        /* set transfer direction to input */
        hcchar   |=  (uint32_t)USB_CH_HCCHAR_EPDir_Msk;
        /* Enable IN transfer specific interrupts */
        hcintmsk  |= (uint32_t)( USB_CH_HCINTMSK_DataTglErrMsk_Msk  |
                       USB_CH_HCINTMSK_BblErrMsk_Msk |
                       USB_CH_HCINTMSK_AckMsk_Msk    |
                       USB_CH_HCINTMSK_NakMsk_Msk )  ;
        break;
      case XMC_USBH_PACKET_OUT:
        break;
      case XMC_USBH_PACKET_SETUP:
        hctsiz   &= (uint32_t)~USB_CH_HCTSIZ_BUFFERMODE_Pid_Msk  ;
        hctsiz   |= (uint32_t)USB_CH_HCTSIZx_DPID_MDATA;
        break;
      default:
        break;
    }
    /* Prepare PID */
    switch (ptr_pipe->packet & XMC_USBH_PACKET_DATA_Msk) {
      case XMC_USBH_PACKET_DATA0:
        hctsiz   &= (uint32_t)~USB_CH_HCTSIZ_BUFFERMODE_Pid_Msk;
        hctsiz   |= (uint32_t)USB_CH_HCTSIZx_DPID_DATA0;
        break;
      case XMC_USBH_PACKET_DATA1:
        hctsiz   &= (uint32_t)~USB_CH_HCTSIZ_BUFFERMODE_Pid_Msk;
        hctsiz   |= (uint32_t)USB_CH_HCTSIZx_DPID_DATA1;
        break;
      default:
        break;
    }

    /* Prepare odd/even frame */
    if ((XMC_USBH0_device.global_register->HFNUM & 1U) != 0U) {
      hcchar &= (uint32_t)~USB_CH_HCCHAR_OddFrm_Msk;
    } else {
      hcchar |= (uint32_t)USB_CH_HCCHAR_OddFrm_Msk;
    }
  
    /* Get transfer type specific status */
    switch (ptr_pipe->ep_type) {
      case XMC_USBH_ENDPOINT_CONTROL:
      case XMC_USBH_ENDPOINT_BULK:
        if (!(hcchar & USB_CH_HCCHAR_EPDir_Msk)) {
          txsts = XMC_USBH0_device.global_register->GNPTXSTS;
        }
        break;
      case XMC_USBH_ENDPOINT_ISOCHRONOUS:
      case XMC_USBH_ENDPOINT_INTERRUPT:
        if (!(hcchar & USB_CH_HCCHAR_EPDir_Msk)) {
          txsts = XMC_USBH0_device.global_register->HPTXSTS;
        }
        break;
      default:
        break;
    }
  
    /* Calculate remaining transfer size */
    num_remaining_transfer = ptr_pipe->num - ptr_pipe->num_transferred_total;
    /* Limit transfer to available space inside fifo/queue if OUT transaction */
    if ((uint32_t)(hcchar & USB_CH_HCCHAR_EPDir_Msk) == 0U) {
      max_pckt_size =  ptr_pipe->ep_max_packet_size;
      num_remaining_fifo = (uint32_t)((uint32_t)(txsts & 0x0000FFFFU) <<  2);
      num_remaining_queue  = (uint32_t)((uint32_t)(txsts & 0x00FF0000U) >> 16);
      if (num_remaining_transfer > num_remaining_fifo) {
        num_remaining_transfer = num_remaining_fifo;
      }
      pckt_num = (uint32_t)((num_remaining_transfer + (max_pckt_size - 1U)) / max_pckt_size);
      if (pckt_num > num_remaining_queue) {
        pckt_num = num_remaining_queue;
      }
      if (num_remaining_transfer > (pckt_num * max_pckt_size)) {
        num_remaining_transfer = pckt_num * max_pckt_size;
      }
      cnt = (uint16_t)((num_remaining_transfer + 3U) / 4U);
      ptr_src  = ptr_pipe->data + ptr_pipe->num_transferred_total;
      loc_index = ((USB0_CH_TypeDef *)ptr_ch - (USB0_CH_TypeDef *)(XMC_USBH0_device.host_channel_registers));
      ptr_dest = (uint32_t *)XMC_USBH0_dfifo_ptr[loc_index];
      /* For OUT/SETUP transfer num_transferring represents num of bytes to be sent */
      ptr_pipe->num_transferring = num_remaining_transfer;
    }
    else {
      /* For IN transfer num_transferring is zero */
      ptr_pipe->num_transferring = 0U;
    }
    /* Set packet count and transfer size */
    if (num_remaining_transfer != 0U) {
      hctsiz |= (((num_remaining_transfer + ptr_pipe->ep_max_packet_size) - 1U) / ptr_pipe->ep_max_packet_size) << 19U;
      hctsiz |=   num_remaining_transfer;
    } else {                                                 /* Zero length packet */
      hctsiz |= ((uint32_t)1U << USB_CH_HCTSIZ_BUFFERMODE_PktCnt_Pos); /* Packet count = 1 */
      hctsiz |= 0U;                                        /* Transfer size = 0 */
    }
    NVIC_DisableIRQ (USB0_0_IRQn);
    ptr_ch->HCINTMSK = hcintmsk;                  /* Enable channel interrupts */
    ptr_ch->HCTSIZ_BUFFERMODE = hctsiz;           /* Write ch transfer size */
    ptr_ch->HCCHAR = hcchar;                      /* Write ch characteristics */
    while (cnt != 0U) {                           /* Load data */
#if defined __TASKING__/*tasking*/
      *ptr_dest = *((__unaligned uint32_t *)ptr_src);
#else/* defined (__GNUC__) || defined (__CC_ARM) || defined (__ICCARM__)*/
      *ptr_dest = *((__packed uint32_t *)ptr_src);
#endif
      ptr_src  += 4U;
      cnt--;
    }
    NVIC_EnableIRQ  (USB0_0_IRQn);                /* Enable OTG interrupt */
    status = true;
  }
  return status;
}

/* USB driver API functions */
/**
 * @return      \ref XMC_USBH_DRIVER_VERSION_t
 * \par<b>Description:</b><br>
 * Get driver version.
*/
static XMC_USBH_DRIVER_VERSION_t XMC_USBH_GetVersion (void) { return xmc_usbh_driver_version; }

/**
 * @return  \ref XMC_USBH_CAPABILITIES_t
 * \par<b>Description:</b><br>
 * Get driver capabilities.
*/
static XMC_USBH_CAPABILITIES_t XMC_USBH_GetCapabilities (void) { return xmc_usbh_driver_capabilities; }

/**
 * @param   cb_port_event  Pointer to port event callback function \ref ARM_USBH_SignalPortEvent
 * @param   cb_pipe_event  Pointer to pipe event callback function \ref ARM_USBH_SignalPipeEvent
 * @return  int32_t \ref Execution_status. 0 if execution is successful.
 *
 * \par<b>Description:</b><br>
 * Initialize USB Host Interface. Registers callback functions to be executed on port event and pipe event.
 * Initializes FIFO address for each pipe. Configures P3.2 as the VBUS charge pump enable pin.\n
 *
 *  \par<b>Related APIs:</b><BR>
 *  XMC_USBH_Select_VBUS(), XMC_USBH_Uninitialize() \n
*/
static int32_t XMC_USBH_Initialize (XMC_USBH_SignalPortEvent_t cb_port_event,
                                XMC_USBH_SignalPipeEvent_t cb_pipe_event) {

  uint32_t channel;
  int32_t status = XMC_USBH_DRIVER_OK;
  if (XMC_USBH0_device.init_done == true)
  {
    /*return ok since initialized*/
  }
  else
  {
    /* assign callbacks */
    XMC_USBH0_device.SignalPortEvent_cb = cb_port_event;
    XMC_USBH0_device.SignalPipeEvent_cb = cb_pipe_event;

    /* assign fifo start addresses */
    for (channel = 0U; channel < USBH0_MAX_PIPE_NUM; channel++) {
      XMC_USBH0_dfifo_ptr[channel] = (uint32_t *)((uint32_t)USB0_BASE + ((channel + 1U) * 0x01000U));
    }

    XMC_GPIO_SetMode(VBUS_port, (uint8_t)VBUS_pin, XMC_GPIO_MODE_OUTPUT_PUSH_PULL_ALT1);
  
    XMC_USBH0_device.init_done = true;
  }
  return status;
}

/**
 * @return int32_t \ref Execution_status. Returns 0 to indicate success.
 * \par<b>Description:</b><br>
 * De-initialize USB Host Interface. Sets the driver power state as powered off. Disables VBUS.\n
 *
 * \par<b>Related APIs:</b><BR>
 *  XMC_USBH_Select_VBUS(), XMC_USBH_Initialize(), XMC_USBH_PortVbusOnOff() \n
*/
static int32_t XMC_USBH_Uninitialize (void) {
  XMC_USBH0_device.init_done = false;
  (void)XMC_USBH_PowerControl(XMC_USBH_POWER_OFF);
  return XMC_USBH_DRIVER_OK;
}

/**
 * @param  state  Power state. \ref XMC_USBH_POWER_STATE_t
 * @return int32_t \ref Execution_status. Returns 0 if successful.
 * \par<b>Description:</b><br>
 * Control USB Host Interface Power. If power state is set to \ref XMC_USBH_POWER_FULL,
 * it initializes the peripheral and enables VBUS. If power state is set to \ref XMC_USBH_POWER_OFF,
 * disables the peripheral and the VBUS.\n
 *
 * \par<b>Related APIs:</b><BR>
 *  XMC_USBH_Select_VBUS(), XMC_USBH_Initialize(), XMC_USBH_PortVbusOnOff(), XMC_USBH_Uninitialize() \n
*/
static int32_t XMC_USBH_PowerControl (XMC_USBH_POWER_STATE_t state) {
  int32_t status = XMC_USBH_DRIVER_OK;
  uint32_t loc_value;
  switch (state) {
    case XMC_USBH_POWER_LOW:
      status = XMC_USBH_DRIVER_ERROR_UNSUPPORTED;
      break;
    case XMC_USBH_POWER_OFF:
      NVIC_DisableIRQ  (USB0_0_IRQn);
      NVIC_ClearPendingIRQ (USB0_0_IRQn); /* Clear pending interrupt */
      XMC_USBH0_device.power_state =  state; /* Clear powered flag */
      XMC_USBH0_device.global_register->GAHBCFG  &= (uint32_t)(~USB_GAHBCFG_GlblIntrMsk_Msk); /* Disable USB interrupts */
      XMC_lClockGating((uint8_t)XMC_USBH_CLOCK_GATING_ENABLE); /* Enable Clock Gating */
      XMC_USBH0_device.global_register->PCGCCTL  |=  (uint32_t)USB_PCGCCTL_StopPclk_Msk; /* Stop PHY clock */
      XMC_SCU_POWER_DisableUsb(); /* Disable Power USB */
      XMC_SCU_RESET_AssertPeripheralReset(XMC_SCU_PERIPHERAL_RESET_USB0); /* reset USB */
      XMC_USBH0_device.port_reset_active =  false; /* Reset variables */
      memset((void *)(pipe), 0, (USBH0_MAX_PIPE_NUM * sizeof(XMC_USBH0_pipe_t)));
      break;
    case XMC_USBH_POWER_FULL:
      if (XMC_USBH0_device.init_done == false)
      {
        status = XMC_USBH_DRIVER_ERROR;
        break;
      } /* not initialized */
      if (XMC_USBH0_device.power_state == XMC_USBH_POWER_FULL)
      {
        status = XMC_USBH_DRIVER_OK;
        break;
      } /* already powered */
      XMC_lClockGating((uint8_t)XMC_USBH_CLOCK_GATING_DISABLE); /* disable clock gating */
      (void)XMC_USBH_osDelay(2U);
      XMC_SCU_RESET_DeassertPeripheralReset(XMC_SCU_PERIPHERAL_RESET_USB0); /* deassert reset USB */
      (void)XMC_USBH_osDelay(2U);
      (void)XMC_USBH_osDelay(100U);
      XMC_SCU_POWER_EnableUsb(); /* Enable Power USB */

      /* On-chip Full-speed PHY */
      XMC_USBH0_device.global_register->PCGCCTL  &=  (uint32_t)~USB_PCGCCTL_StopPclk_Msk;  /* Start PHY clock */
      XMC_USBH0_device.global_register->GUSBCFG  |=  (uint32_t)USB_GUSBCFG_PHYSel_Msk;    /* Full-speed transceiver */

      while ((XMC_USBH0_device.global_register->GRSTCTL & USB_GRSTCTL_AHBIdle_Msk) == 0U) /* wait until AHB master state machine is idle */
      {
        /*Wait*/
      }

      XMC_USBH0_device.global_register->GRSTCTL |=  (uint32_t)USB_GRSTCTL_CSftRst_Msk; /* Core soft reset */

      while ((XMC_USBH0_device.global_register->GRSTCTL & USB_GRSTCTL_CSftRst_Msk)  != 0U) /* wait soft reset confirmation */
      {
        /*Wait*/
      }
      (void)XMC_USBH_osDelay(100U);

      XMC_USBH0_device.port_reset_active =  false; /* Reset variables */
      memset((void *)(pipe), 0, (USBH0_MAX_PIPE_NUM * sizeof(XMC_USBH0_pipe_t)));

      /*Created local copy of GUSBCFG to avoid side effects*/
      loc_value = XMC_USBH0_device.global_register->GUSBCFG;
      if (((loc_value & USB_GUSBCFG_ForceHstMode_Msk) == 0U) || \
          ((loc_value & USB_GUSBCFG_ForceDevMode_Msk) != 0U))
      {
        XMC_USBH0_device.global_register->GUSBCFG &= (uint32_t)~USB_GUSBCFG_ForceDevMode_Msk;          /* Clear force device mode */
        XMC_USBH0_device.global_register->GUSBCFG |=  (uint32_t)USB_GUSBCFG_ForceHstMode_Msk;          /* Force host mode */
        (void)XMC_USBH_osDelay(100U);
      }

      /* FS only, even if HS is supported */
      XMC_USBH0_device.global_register->HCFG     |=  (uint32_t)(0x200U | USB_CH_HCFG_FSLSSUP(1));

      /* Rx FIFO setting */
      XMC_USBH0_device.global_register->GRXFSIZ   = (RX_FIFO_SIZE/4U);
      /* Non-periodic Tx FIFO setting */
      XMC_USBH0_device.global_register->GNPTXFSIZ_HOSTMODE = (((uint32_t)(TX_FIFO_SIZE_NON_PERI/4U) << 16) |  (RX_FIFO_SIZE / 4U));
      /* Periodic Tx FIFO setting */
      XMC_USBH0_device.global_register->HPTXFSIZ  = ((uint32_t)(TX_FIFO_SIZE_PERI / 4U) << 16U) | ((RX_FIFO_SIZE + TX_FIFO_SIZE_NON_PERI) / 4U);
      /* Enable channel interrupts */
      XMC_USBH0_device.global_register->HAINTMSK  = ((uint32_t)1U << USBH0_MAX_PIPE_NUM) - 1U;
      /* Unmask interrupts */
      XMC_USBH0_device.global_register->GINTMSK_HOSTMODE   = (
                        USB_GINTSTS_HOSTMODE_DisconnInt_Msk |
                        USB_GINTMSK_HOSTMODE_HChIntMsk_Msk    |
                        USB_GINTMSK_HOSTMODE_PrtIntMsk_Msk   |
                        USB_GINTMSK_HOSTMODE_RxFLvlMsk_Msk |
                        USB_GINTMSK_HOSTMODE_SofMsk_Msk  |
                        USB_GINTMSK_HOSTMODE_WkUpIntMsk_Msk
                       )   ;
      /* Set powered state */
      XMC_USBH0_device.power_state = state;
      /* Enable interrupts */
      XMC_USBH0_device.global_register->GAHBCFG  |=  (uint32_t)USB_GAHBCFG_GlblIntrMsk_Msk;
      /* Set highest interrupt priority */
      NVIC_SetPriority (USB0_0_IRQn, 0U);
      NVIC_EnableIRQ   (USB0_0_IRQn);
      break;
    default:
      status = XMC_USBH_DRIVER_ERROR_UNSUPPORTED;
  }
  return status;
}

/**
 * @param port Root HUB Port Number. Only one port(0) is supported.
 * @param vbus VBUS state - \n
 *                 - \b false VBUS off
 *                 - \b true  VBUS on
 * @return int32_t \ref Execution_status. Returns 0 if successful.
 *
 * \par<b>Description:</b><br>
 * Set USB port VBUS on/off.
*/
static int32_t XMC_USBH_PortVbusOnOff (uint8_t port, bool vbus) {
  int32_t status = XMC_USBH_DRIVER_OK;
  if (XMC_USBH0_device.power_state == XMC_USBH_POWER_OFF)
  {
    status = XMC_USBH_DRIVER_ERROR;
  }
  else
  {
    if (port != 0U)
    {
      status = XMC_USBH_DRIVER_ERROR_PARAMETER;
    }
    else
    {
      if (vbus != 0U) {
        /* Port power on */
        XMC_USBH0_device.global_register->HPRT |=  (uint32_t)USB_HPRT_PrtPwr_Msk;
        XMC_GPIO_SetMode(VBUS_port, (uint8_t)VBUS_pin, XMC_GPIO_MODE_OUTPUT_PUSH_PULL_ALT1);
      } else {
        /* Port power off */
        XMC_USBH0_device.global_register->HPRT &= (uint32_t)~USB_HPRT_PrtPwr_Msk;
        XMC_GPIO_SetMode(VBUS_port, (uint8_t)VBUS_pin, XMC_GPIO_MODE_INPUT_TRISTATE);
      }
    }
  }
  return status;
}

/**
 * @param port Root HUB Port Number. Only one port(0) is supported.
 * @return int32_t Execution status. \ref Execution_status
 * \par<b>Description:</b><br>
 * Do USB port reset. Port reset should honor the requirement of 50ms delay before enabling.
 * The function depends on implementation of XMC_USBH_osDelay() for 1ms delay to achieve required delay.
 *
*/
static int32_t XMC_USBH_PortReset (uint8_t port) {
  uint32_t hprt;
  int32_t status = XMC_USBH_DRIVER_OK;
  if (XMC_USBH0_device.power_state == XMC_USBH_POWER_OFF)
  {
    status = XMC_USBH_DRIVER_ERROR;
  }
  else
  {
    if (port != 0U)
    {
      status = XMC_USBH_DRIVER_ERROR_PARAMETER;
    }

    XMC_USBH0_device.port_reset_active = true;
    hprt  =  XMC_USBH0_device.global_register->HPRT;
    hprt &= (uint32_t)~USB_HPRT_PrtEna_Msk;                            /* Disable port */
    hprt |= (uint32_t)USB_HPRT_PrtRst_Msk;                            /* Port reset */
    XMC_USBH0_device.global_register->HPRT = hprt;
    (void)XMC_USBH_osDelay(50U);                                            /* wait at least 50ms */
    hprt &= (uint32_t)~USB_HPRT_PrtRst_Msk;                            /* Clear port reset */
    XMC_USBH0_device.global_register->HPRT = hprt;
    (void)XMC_USBH_osDelay(50U);                                            /* wait for ISR */

    /*Wait for the port to be enabled*/
    while ((XMC_USBH0_device.global_register->HPRT & USB_HPRT_PrtEna_Msk) == 0U)
    {
      /*wait*/
    }

    if (XMC_USBH0_device.port_reset_active == true)
    {
      XMC_USBH0_device.port_reset_active = false;
      status = XMC_USBH_DRIVER_ERROR;                               /* reset not confirmed inside ISR */
    }
  }

  return status;
}

/**
 * @param port USB port number. Only one port(0) is supported.
 * @return      \ref Execution_status
 * \par<b>Description:</b><br>
 * Suspend USB Port (stop generating SOFs).\n
 *
 * \par<b>Related APIs:</b><BR>
 * XMC_USBH_PortResume() \n
*/
static int32_t XMC_USBH_PortSuspend (uint8_t port)
{
  int32_t status = XMC_USBH_DRIVER_OK;
  uint32_t hprt;
  if (XMC_USBH0_device.power_state == XMC_USBH_POWER_OFF)
  {
    status = XMC_USBH_DRIVER_ERROR;
  }
  else
  {
    if (port != 0U)
    {
      status = XMC_USBH_DRIVER_ERROR_PARAMETER;
    }
    else
    {
      hprt = XMC_USBH0_device.global_register->HPRT;
      hprt &= (uint32_t)~(USB_HPRT_PrtEna_Msk);
      hprt |= (uint32_t)USB_HPRT_PrtSusp_Msk;
      XMC_USBH0_device.global_register->HPRT = hprt;
      /* Stop PHY clock after suspending the bus*/
      XMC_USBH0_device.global_register->PCGCCTL |= XMC_USBH_PHY_CLK_STOP;

    }
  }
  return status;
}

/**
 * @param port USB port number. Only one port(0) is supported.
 * @return \ref Execution_status
 * \par<b>Description:</b><br>
 * Resume suspended USB port (start generating SOFs).\n
 *
 * \par<b>Related APIs:</b><BR>
 * XMC_USBH_PortSuspend() \n
*/
static int32_t XMC_USBH_PortResume (uint8_t port)
{
  int32_t status = XMC_USBH_DRIVER_OK;
  uint32_t hprt;
  if (XMC_USBH0_device.power_state == XMC_USBH_POWER_OFF)
  {
    status = XMC_USBH_DRIVER_ERROR;
  }
  else
  {
    if (port != 0U)
    {
      status = XMC_USBH_DRIVER_ERROR_PARAMETER;
    }
    else
    {
      /*Ungate PHY clock*/
      XMC_USBH0_device.global_register->PCGCCTL = XMC_USBH_PHY_CLK_UNGATE;
      /*Set resume bit*/
      hprt = XMC_USBH0_device.global_register->HPRT;
      hprt &= (uint32_t)~(USB_HPRT_PrtEna_Msk);
      hprt |= (uint32_t)USB_HPRT_PrtRes_Msk;
      XMC_USBH0_device.global_register->HPRT = hprt;

      (void)XMC_USBH_osDelay(20U);

      hprt = XMC_USBH0_device.global_register->HPRT;
      hprt &= (uint32_t)~(USB_HPRT_PrtEna_Msk);
      hprt &= (uint32_t)~((uint32_t)USB_HPRT_PrtRes_Msk);
      XMC_USBH0_device.global_register->HPRT = hprt;
    }
  }
  
  return status;
}

/**
 * @param port USB port number. Only one port(0) is supported.
 * @return XMC_USBH_PORT_STATE_t Port State
 *
 * \par<b>Description:</b><br>
 * Get current USB port state. The state indicates if the port is connected, port speed
 * and port overcurrent status.
*/
static XMC_USBH_PORT_STATE_t XMC_USBH_PortGetState (uint8_t port)
{
  XMC_USBH_PORT_STATE_t port_state = { 0U, 0U, 0U };
  uint32_t hprt;
  
  if (XMC_USBH0_device.power_state == XMC_USBH_POWER_OFF)
  {
    /*Do not update the port state*/
  }
  else
  {
    if (port != 0U)
    {
      /*Do not update the port state*/
    }
    else
    {
      hprt = XMC_USBH0_device.global_register->HPRT;
      if(((hprt & USB_HPRT_PrtConnSts_Msk) != 0U))
      {
        port_state.connected   = 1U;
      }
      else
      {
        port_state.connected   = 0U;
      }
      port_state.overcurrent = 0U;

      switch ((uint32_t)((uint32_t)(hprt & USB_HPRT_PrtSpd_Msk) >> USB_HPRT_PrtSpd_Pos)) {
        case 1U: /* Full speed */
         port_state.speed = XMC_USBH_SPEED_FULL;
         break;
        default:
         break;
      }
    }
  }
  return port_state;
}

/**
 * @param dev_addr Device address
 * @param dev_speed  Device speed
 * @param hub_addr Hub address. This value should be 0 since hub is not supported.
 * @param hub_port  USB port number. Only one port(0) is supported.
 * @param ep_addr Device endpoint address \n
 *                - ep_addr.0..3: Address \n
 *                - ep_addr.7:    Direction\n
 * @param ep_type Endpoint type (ARM_USB_ENDPOINT_xxx)
 * @param ep_max_packet_size Endpoint maximum packet size
 * @param ep_interval Endpoint polling interval
 * @return XMC_USBH_PIPE_HANDLE Pipe handle is a pointer to pipe hardware base address.
 *
 * \par<b>Description:</b><br>
 * Create/allocate a pipe configured with input parameters. The function looks for an unused pipe and configures with input parameters.
 *
 * \par<b>Related APIs:</b><BR>
 * XMC_USBH_PipeModify(), XMC_USBH_PipeDelete(), XMC_USBH_PipeReset(), XMC_USBH_PipeTransfer() \n
*/
static XMC_USBH_PIPE_HANDLE XMC_USBH_PipeCreate (uint8_t dev_addr, uint8_t dev_speed, uint8_t hub_addr, uint8_t hub_port, uint8_t ep_addr, uint8_t ep_type, uint16_t ep_max_packet_size, uint8_t  ep_interval) {
  XMC_USBH0_pipe_t    *ptr_pipe;
  USB0_CH_TypeDef *ptr_ch;
  uint32_t         i;
  uint32_t loc_val;
  
  if (XMC_USBH0_device.power_state == XMC_USBH_POWER_OFF)
  {
    ptr_ch = (USB0_CH_TypeDef *)NULL;
  }
  else
  {
   /* get first free pipe available */
    ptr_ch = (USB0_CH_TypeDef *)(XMC_USBH0_device.host_channel_registers);

    for (i = 0U; i < USBH0_MAX_PIPE_NUM; i++) {
      if ((ptr_ch->HCCHAR & 0x3FFFFFFFU) == 0U)
      {
        break;
      }
      ptr_ch++;
    }
  
    /* free pipe found? */
    if (i == USBH0_MAX_PIPE_NUM)
    { 
      ptr_ch = (USB0_CH_TypeDef *)NULL;
    }
    else
    {
      ptr_pipe = (XMC_USBH0_pipe_t *)(&pipe[(ptr_ch - (USB0_CH_TypeDef *)(XMC_USBH0_device.host_channel_registers))]);

      memset((void *)ptr_pipe, 0, sizeof(XMC_USBH0_pipe_t));  /* Initialize pipe structure */

      /* Fill in all fields of Endpoint Descriptor */
      /*Get the end point direction from the MSB of address*/
      loc_val = 0U;
      if (((ep_addr >> 7U) & 0x1U) == 0U)
      {
        loc_val = 1U;
      }
      ptr_ch->HCCHAR = ((uint32_t)(USB_CH_HCCHARx_MPS(ep_max_packet_size))|
                       USB_CH_HCCHARx_EPNUM(ep_addr)) |
                       (uint32_t)(USB_CH_HCCHAR_EPDir_Msk * loc_val) |
                       (USB_CH_HCCHARx_EPTYPE (ep_type) ) |
                       (USB_CH_HCCHARx_DEVADDR (dev_addr) ) ;
      /* Store Pipe settings */
      ptr_pipe->ep_max_packet_size = ep_max_packet_size;
      ptr_pipe->ep_type            = ep_type;
      switch (ep_type) {
        case XMC_USBH_ENDPOINT_CONTROL:
        case XMC_USBH_ENDPOINT_BULK:
          break;
        case XMC_USBH_ENDPOINT_ISOCHRONOUS:
        case XMC_USBH_ENDPOINT_INTERRUPT:
          if (ep_interval > 0U) {
            ptr_pipe->interval_reload = ep_interval;
          }
          ptr_pipe->interval = ptr_pipe->interval_reload;
          loc_val = ((((uint32_t)ep_max_packet_size >> 11U) + 1U) & 3U);
          ptr_ch->HCCHAR |= (uint32_t)USB_CH_HCCHARx_MCEC(loc_val);
          break;
        default:
          break;
      }
    }
  }
  return ((XMC_USBH_EP_HANDLE)ptr_ch);
}

/**
 * @param pipe_hndl Pointer returned by the pipe create function. It is the hardware based address of a USB channel.
 * @param dev_addr Device address to be configured for the pipe.
 * @param dev_speed  Device speed class.
 * @param hub_addr Hub address. It should be 0 since hub is not supported.
 * @param hub_port USB port number. Only one port(0) is supported.
 * @param ep_max_packet_size Endpoint maximum packet size
 * @return Execution_status
 *
 * \par<b>Description:</b><br>
 * Modify an existing pipe with input parameters. It can be used to configure the pipe after receiving configuration details from the device.
 *
 * \par<b>Related APIs:</b><BR>
 * XMC_USBH_PipeCreate(), XMC_USBH_PipeDelete(), XMC_USBH_PipeReset(), XMC_USBH_PipeTransfer() \n
*/
static int32_t XMC_USBH_PipeModify (XMC_USBH_PIPE_HANDLE pipe_hndl, uint8_t dev_addr, uint8_t dev_speed, uint8_t hub_addr, uint8_t hub_port, uint16_t ep_max_packet_size) {
  XMC_USBH0_pipe_t    *ptr_pipe;
  USB0_CH_TypeDef *ptr_ch;
  uint32_t   hcchar;
  int32_t status = XMC_USBH_DRIVER_OK;

  if (XMC_USBH0_device.power_state == XMC_USBH_POWER_OFF)
  {
    status = XMC_USBH_DRIVER_ERROR;
  }
  else
  {
    if (pipe_hndl  == 0U)
    {
      status = XMC_USBH_DRIVER_ERROR_PARAMETER;
    }
    else
    {
      ptr_ch   = (USB0_CH_TypeDef *)(pipe_hndl);
      ptr_pipe = (XMC_USBH0_pipe_t *)(&pipe[(ptr_ch - (USB0_CH_TypeDef *)(XMC_USBH0_device.host_channel_registers))]);
      if (ptr_pipe->in_use != 0U)
      {
        status = XMC_USBH_DRIVER_ERROR_BUSY;
      }
      else
      {
        /* Fill in all fields of channel */
        hcchar  =   ptr_ch->HCCHAR;
        /* Clear fields */
        hcchar &= (uint32_t)~(USB_CH_HCCHAR_MPS_Msk | USB_CH_HCCHAR_DevAddr_Msk)  ;
        /* Set fields */
        hcchar |= (uint32_t)(USB_CH_HCCHARx_MPS(ep_max_packet_size) | (USB_CH_HCCHARx_DEVADDR(dev_addr)));
        ptr_ch->HCCHAR = hcchar;

        ptr_pipe->ep_max_packet_size = ep_max_packet_size;
      }
    }
  }
  
  return status;
}

/**
 * @param pipe_hndl Pointer returned by the pipe create function. It is the hardware based address of a USB channel.
 * @return Execution_status
 *
 * \par<b>Description:</b><br>
 * Delete pipe from active pipes list. After it is deleted, it can be assigned to new pipe request.
 *
 * \par<b>Related APIs:</b><BR>
 * XMC_USBH_PipeCreate(), XMC_USBH_PipeModify(), XMC_USBH_PipeReset(), XMC_USBH_PipeTransfer() \n
*/
static int32_t XMC_USBH_PipeDelete (XMC_USBH_PIPE_HANDLE pipe_hndl) {
  XMC_USBH0_pipe_t    *ptr_pipe;
  USB0_CH_TypeDef *ptr_ch;
  int32_t status = XMC_USBH_DRIVER_OK;

  if (XMC_USBH0_device.power_state == XMC_USBH_POWER_OFF)
  {
    status = XMC_USBH_DRIVER_ERROR;
  }
  else
  {
    if (pipe_hndl == 0U)
    {
      status = XMC_USBH_DRIVER_ERROR_PARAMETER;
    }
    else
    {
      ptr_ch   = (USB0_CH_TypeDef *)(pipe_hndl);
      ptr_pipe = (XMC_USBH0_pipe_t    *)(&pipe[(ptr_ch - (USB0_CH_TypeDef *)(XMC_USBH0_device.host_channel_registers))]);
      if (ptr_pipe->in_use != 0U)
      {
        status = XMC_USBH_DRIVER_ERROR_BUSY;
      }
      else
      {
        ptr_ch->HCCHAR            = 0U;
        ptr_ch->HCINT             = 0U;
        ptr_ch->HCINTMSK          = 0U;
        ptr_ch->HCTSIZ_BUFFERMODE = 0U;

        memset((void *)ptr_pipe, 0, sizeof(XMC_USBH0_pipe_t));
      }
    }
  }

  return status;
}

/**
 * @param pipe_hndl Pointer returned by the pipe create function. It is the hardware based address of a USB channel.
 * @return Execution_status
 * \par<b>Description:</b><br>
 * Reset pipe by clearing the interrupt mask and resetting the transfer control register.\n
 *
 * \par<b>Related APIs:</b><BR>
 * XMC_USBH_PipeCreate(), XMC_USBH_PipeModify(), XMC_USBH_PipeDelete(), XMC_USBH_PipeTransfer() \n
*/
static int32_t XMC_USBH_PipeReset (XMC_USBH_PIPE_HANDLE pipe_hndl) {
  XMC_USBH0_pipe_t    *ptr_pipe;
  USB0_CH_TypeDef *ptr_ch;
  int32_t status = XMC_USBH_DRIVER_OK;

  if (XMC_USBH0_device.power_state == XMC_USBH_POWER_OFF)
  {
    status = XMC_USBH_DRIVER_ERROR;
  }
  else
  {
    if (pipe_hndl  == 0U)
    {
      status = XMC_USBH_DRIVER_ERROR_PARAMETER;
    }
    else
    {
      ptr_ch   = (USB0_CH_TypeDef *)(pipe_hndl);
      ptr_pipe = (XMC_USBH0_pipe_t    *)(&pipe[(ptr_ch - (USB0_CH_TypeDef *)(XMC_USBH0_device.host_channel_registers))]);
      if (ptr_pipe->in_use != 0U)
      {
        status = XMC_USBH_DRIVER_ERROR_BUSY;
      }
      else
      {
        ptr_ch->HCINT    = 0U;
        ptr_ch->HCINTMSK = 0U;
        ptr_ch->HCTSIZ_BUFFERMODE   = 0U;
      }
    }
  }
  return status;
}

/**
 * @param pipe_hndl Pointer returned by the pipe create function. It is the hardware based address of a USB channel.
 * @param packet Packet information with bit masks to represent packet data toggle information and packet type.\n
 *               \ref XMC_USBH_PACKET_DATA0 / \ref XMC_USBH_PACKET_DATA1, \ref XMC_USBH_PACKET_SETUP /
 *               \ref XMC_USBH_PACKET_OUT / \ref XMC_USBH_PACKET_IN
 * @param data Pointer to buffer with data to send or for received data to be stored.
 * @param num Number of data bytes to transfer
 * @return Execution_status
 *
 *  \par<b>Description:</b><BR>
 *  Transfer packets through USB Pipe. Handles transfer of multiple packets using the pipe transfer complete event.
 *  The pipe event callback function will be called when the transfer is completed.\n
 *
 * \par<b>Related APIs:</b><BR>
 * XMC_USBH_PipeCreate(), XMC_USBH_PipeModify(), XMC_USBH_PipeDelete(), XMC_USBH_PipeReset() \n
*/
static int32_t XMC_USBH_PipeTransfer (XMC_USBH_PIPE_HANDLE pipe_hndl, uint32_t packet, uint8_t *data, uint32_t num) {
  XMC_USBH0_pipe_t *ptr_pipe;
  int32_t status = XMC_USBH_DRIVER_OK;

  if (XMC_USBH0_device.power_state == XMC_USBH_POWER_OFF)
  { 
    status = XMC_USBH_DRIVER_ERROR;
  }
  else
  {

    if(!(((((packet & XMC_USBH_PACKET_TOKEN_Msk) == XMC_USBH_PACKET_OUT) ||
        ((packet & XMC_USBH_PACKET_TOKEN_Msk) == XMC_USBH_PACKET_IN))) ||
        ((packet & XMC_USBH_PACKET_TOKEN_Msk) == XMC_USBH_PACKET_SETUP )))
    {
      status = XMC_USBH_DRIVER_ERROR_PARAMETER;
    }
    else
    {
      if (pipe_hndl  == 0U)
      {
        status = XMC_USBH_DRIVER_ERROR_PARAMETER;
      }
      else
      {
        if ((XMC_USBH0_device.global_register->HPRT & USB_HPRT_PrtConnSts_Msk) == 0U)
        {
          status = XMC_USBH_DRIVER_ERROR;
        }
        else
        {
          ptr_pipe = (XMC_USBH0_pipe_t *)(&pipe[((USB0_CH_TypeDef *)pipe_hndl - (USB0_CH_TypeDef *)(XMC_USBH0_device.host_channel_registers))]);
          if (ptr_pipe->in_use != 0U)
          {
            status = XMC_USBH_DRIVER_ERROR_BUSY;
          }
          else
          {
            /* Prepare transfer information */
            ptr_pipe->packet                = packet;
            ptr_pipe->data                  = data;
            ptr_pipe->num                   = num;
            ptr_pipe->num_transferred_total = 0U;
            ptr_pipe->num_transferring      = 0U;
            ptr_pipe->in_use                = 0U;
            ptr_pipe->transfer_active       = 0U;
            ptr_pipe->interrupt_triggered   = 0U;
            ptr_pipe->event                 = 0U;

            if ((ptr_pipe->ep_type == (uint8_t)XMC_USBH_ENDPOINT_INTERRUPT) && (ptr_pipe->interval != 0U)) {
              ptr_pipe->in_use              = 1U; /* transfer will be started inside interrupt (SOF) */
            } else {
              ptr_pipe->transfer_active     = 1U;
              ptr_pipe->in_use              = 1U;
              if(XMC_lStartTransfer (ptr_pipe, (USB0_CH_TypeDef *)pipe_hndl) == false)
              {
                status = XMC_USBH_DRIVER_ERROR;
              }
            }
          }
        }
      }
    }
  }
  return status;
}

/**
 * @param pipe_hndl Pointer returned by the pipe create function. It is the hardware based address of a USB channel.
 * @return uint32_t Number of successfully transferred data bytes
 *
 *  \par<b>Description:</b><BR>
 *  Get result of USB Pipe transfer.
 *
 * \par<b>Related APIs:</b><BR>
 * XMC_USBH_PipeCreate(), XMC_USBH_PipeModify(), XMC_USBH_PipeDelete(), XMC_USBH_PipeTransfer() \n
*/
static uint32_t XMC_USBH_PipeTransferGetResult (XMC_USBH_PIPE_HANDLE pipe_hndl) {
  uint32_t status;
  if (pipe_hndl == 0U)
  {
    status = 0U;
  }
  else
  {
    status = (pipe[((USB0_CH_TypeDef *)pipe_hndl - (USB0_CH_TypeDef *)(XMC_USBH0_device.host_channel_registers))].num_transferred_total);
  }
  return status;
}

/**
 * @param pipe_hndl Pointer returned by the pipe create function. It is the hardware based address of a USB channel.
 * @return Execution_status
 *
 * \par<b>Description:</b><BR>
 * Abort current USB Pipe transfer.\n
 *
 * \par<b>Related APIs:</b><BR>
 * XMC_USBH_PipeCreate(), XMC_USBH_PipeModify(), XMC_USBH_PipeDelete(), XMC_USBH_PipeTransfer() \n
*/
static int32_t XMC_USBH_PipeTransferAbort (XMC_USBH_PIPE_HANDLE pipe_hndl) {
  XMC_USBH0_pipe_t *ptr_pipe;
  USB0_CH_TypeDef *ptr_ch;
  uint32_t timeout;
  int32_t status = XMC_USBH_DRIVER_ERROR;
  
  ptr_ch = (USB0_CH_TypeDef *) pipe_hndl;
  
  if (XMC_USBH0_device.power_state == XMC_USBH_POWER_OFF)
  {
    /*Error in power state*/
  }
  else
  {
    if (pipe_hndl  == 0U)
    {
      status = XMC_USBH_DRIVER_ERROR_PARAMETER;
    }
    else
    {
      ptr_pipe = (XMC_USBH0_pipe_t *)(&pipe[(ptr_ch - (USB0_CH_TypeDef *)(XMC_USBH0_device.host_channel_registers))]);

      if (ptr_pipe->in_use != 0U) {
        ptr_pipe->in_use = 0U;
        /* Disable channel if not yet halted */
        if ((ptr_ch->HCINT & USB_CH_HCINT_ChHltd_Msk) == 0U)
        {
          if (ptr_ch->HCCHAR & USB_CH_HCCHAR_ChEna_Msk)
          {
            ptr_ch->HCINTMSK = 0U;
            (void)XMC_USBH_osDelay(1U);
            if (ptr_ch->HCINT & USB_CH_HCINT_NAK_Msk) {
              ptr_ch->HCINT  =  USB_CH_HCINTx_ALL;    /* Clear all interrupts */
              status = XMC_USBH_DRIVER_OK;
            }
            else
            {
              ptr_ch->HCINT  =  USB_CH_HCINTx_ALL;      /* Clear all interrupts */
              ptr_ch->HCCHAR =  (uint32_t)(ptr_ch->HCCHAR | USB_CH_HCCHAR_ChEna_Msk | USB_CH_HCCHAR_ChDis_Msk);

              /* wait until channel is halted */
              for (timeout = 0U; timeout < 5000U; timeout++) {
                if (ptr_ch->HCINT & USB_CH_HCINT_ChHltd_Msk) {
                  ptr_ch->HCINT = USB_CH_HCINTx_ALL;
                  status = XMC_USBH_DRIVER_OK;
                }
              }
            }
          }
        }
      }
    }
  }

  return status;
}

/**
 * @return Frame number.
 *
 * \par<b>Description:</b><BR>
 * Get current USB Frame Number.
*/
static uint16_t XMC_USBH_GetFrameNumber (void)
{
  uint16_t status;
  
  if (XMC_USBH0_device.power_state == XMC_USBH_POWER_OFF)
  {
    status = 0U;
  }
  else
  {
    status = (uint16_t)((XMC_USBH0_device.global_register->HFNUM) & 0xFFFU);
  }
  return status;
}

/**
 * @param gintsts USB port interrupt status flag.
 *
 * \par<b>Description:</b><BR>
 * USB host interrupt handler. It updates port and pipe state information based on different events
 * generated by the peripheral. It propagates the port events to the callback function registered by the user
 * during initialization. When a pipe transfer complete event is detected, it checks if any further data is available
 * to be transmitted on the same pipe and continues transmission until data is available. A pipe event is also propagated
 * to the user provided pipe event callback function. A transfer complete event will be propagated only when all the data
 * is transmitted for an OUT transaction.
 *
*/
void XMC_USBH_HandleIrq (uint32_t gintsts) {
  XMC_USBH0_pipe_t *ptr_pipe;
  USB0_CH_TypeDef *ptr_ch;
  uint32_t hprt, haint, hcint, pktcnt, mpsiz;
  uint32_t ch;
  uint8_t *ptr_data;
  uint32_t *dfifo;
  uint32_t grxsts, bcnt, dat, len, len_rest;
  
  /* Host port interrupt */
  if ((gintsts & USB_GINTSTS_HOSTMODE_PrtInt_Msk) != 0U) {
    hprt = XMC_USBH0_device.global_register->HPRT;
    /* Clear port enable */
    XMC_USBH0_device.global_register->HPRT = hprt & (uint32_t)(~USB_HPRT_PrtEna_Msk);
    if ((hprt & USB_HPRT_PrtConnDet_Msk) != 0U) {
      XMC_USBH0_device.global_register->HCFG =  (0x200U | (USB_CH_HCFG_FSLSPCS(1) |
                                                    USB_CH_HCFG_FSLSSUP(1)));
      /* Ignore connect under reset */
      if (XMC_USBH0_device.port_reset_active == false) {
        XMC_USBH0_device.SignalPortEvent_cb(0U, XMC_USBH_EVENT_CONNECT);
      }
    }
    if ((hprt & USB_HPRT_PrtEnChng_Msk) != 0U) { /* If port enable changed */
      if ((hprt & USB_HPRT_PrtEna_Msk) != 0U) {  /* If device connected */
        if (XMC_USBH0_device.port_reset_active == true) {
          XMC_USBH0_device.port_reset_active = false;
          XMC_USBH0_device.SignalPortEvent_cb(0U, XMC_USBH_EVENT_RESET);
        }
      }
    }
  }
  
  /* Disconnect interrupt */
  if ((gintsts & USB_GINTSTS_HOSTMODE_DisconnInt_Msk) != 0U) {
    XMC_USBH0_device.global_register->GINTSTS_HOSTMODE = USB_GINTSTS_HOSTMODE_DisconnInt_Msk; /* Clear disconnect interrupt */
    /* Ignore disconnect under reset */
    if ( XMC_USBH0_device.port_reset_active == false) {
      ptr_ch   = (USB0_CH_TypeDef *)(XMC_USBH0_device.host_channel_registers);
      ptr_pipe = (XMC_USBH0_pipe_t    *)(pipe);
      for (ch = 0U; ch < USBH0_MAX_PIPE_NUM; ch++) {
        if (ptr_pipe->in_use != 0U) {
          ptr_pipe->in_use = 0U;
          ptr_ch->HCINT    = USB_CH_HCINTx_ALL;                                            /* Clear all interrupts */
          ptr_ch->HCINTMSK = USB_CH_HCINT_ChHltd_Msk;                           /* Enable halt interrupt */
          ptr_ch->HCCHAR  |= (uint32_t)(USB_CH_HCCHAR_ChEna_Msk | USB_CH_HCCHAR_ChDis_Msk); /* Activate Halt */
          XMC_USBH0_device.SignalPipeEvent_cb((XMC_USBH_EP_HANDLE)ptr_ch, XMC_USBH_EVENT_BUS_ERROR);
        }
        ptr_ch++;
        ptr_pipe++;
      }
      XMC_USBH0_device.SignalPortEvent_cb(0U, XMC_USBH_EVENT_DISCONNECT);
    }
  }
  /* Handle receive fifo not-empty interrupt */
  if ((gintsts & USB_GINTSTS_HOSTMODE_RxFLvl_Msk) != 0U) {
    XMC_USBH0_device.global_register->GINTMSK_HOSTMODE &= (uint32_t)~USB_GINTMSK_HOSTMODE_RxFLvlMsk_Msk;
    grxsts     = (XMC_USBH0_device.global_register->GRXSTSP_HOSTMODE);    
    /* IN Data Packet received ? */
    if ((uint32_t)((grxsts >> 17U) & 0x0FU) == (uint32_t)USB_GRXSTSR_HOSTMODE_PktSts_IN_DATA_PKT) {
      ch         = (uint32_t)(grxsts & USB_GRXSTSR_DEVICEMODE_EPNum_Msk);
      bcnt       = ((uint32_t)(grxsts & USB_GRXSTSR_DEVICEMODE_BCnt_Msk) >> USB_GRXSTSR_DEVICEMODE_BCnt_Pos);
      dfifo      = (uint32_t *)XMC_USBH0_dfifo_ptr[ch];
      ptr_data   =  pipe[ch].data + pipe[ch].num_transferred_total;
      len        =  bcnt / 4U; /* Received number of 32-bit data */
      len_rest   =  bcnt & 3U; /* Number of bytes left */
      /* Read data from fifo */
      /* Read 32 bit sized  data */
      while (len != 0U) {
#if defined __TASKING__/*tasking*/
        *((__unaligned uint32_t *)ptr_data) = *dfifo;
#else /* defined (__GNUC__) || defined (__CC_ARM) || defined (__ICCARM__)*/
        *((__packed uint32_t *)ptr_data) = *dfifo;
#endif

        ptr_data += 4U;
        len--;
      }
      /* Read 8 bit sized data */
      if (len_rest != 0U) {
#if defined __TASKING__/*tasking*/
        dat = *((__unaligned uint32_t *)dfifo);
#else /* defined (__GNUC__) || defined (__CC_ARM) || defined (__ICCARM__)*/
        dat = *((__packed uint32_t *)dfifo);
#endif
        while (len_rest != 0U) {
          *ptr_data = (uint8_t)dat;
          ptr_data++;
          dat >>= 8;
          len_rest--;
        }
      }
      pipe[ch].num_transferring      += bcnt;
      pipe[ch].num_transferred_total += bcnt;
    }
    XMC_USBH0_device.global_register->GINTMSK_HOSTMODE |= (uint32_t)USB_GINTMSK_HOSTMODE_RxFLvlMsk_Msk;
  }

  /* Handle sof interrupt */
  if ((gintsts & USB_GINTSTS_HOSTMODE_Sof_Msk) != 0U) { /* If start of frame interrupt */
    XMC_USBH0_device.global_register->GINTSTS_HOSTMODE =  USB_GINTSTS_HOSTMODE_Sof_Msk; /* Clear SOF interrupt */
    ptr_pipe = (XMC_USBH0_pipe_t *)(pipe);
    for (ch = 0U; ch < USBH0_MAX_PIPE_NUM; ch++) {
      /* If interrupt transfer is active handle period (interval) */
      if ((ptr_pipe->ep_type == (uint8_t)XMC_USBH_ENDPOINT_INTERRUPT) && (ptr_pipe->in_use == 1U)) {
        if (ptr_pipe->interval != 0U)
        {
          ptr_pipe->interval--;
          if (ptr_pipe->interval == 0U)
          {
            ptr_pipe->interval = ptr_pipe->interval_reload;
            ptr_pipe->interrupt_triggered = 1U;
          }
        }
      }
      ptr_pipe++;
    }
  }
  
  /* Handle host ctrl interrupt */
  if ((gintsts & USB_GINTSTS_HOSTMODE_HChInt_Msk) != 0U) {
    haint = XMC_USBH0_device.global_register->HAINT;
    for (ch = 0U; ch < USBH0_MAX_PIPE_NUM; ch++) {
      /* Check for interrupt of all channels */
      if ((haint & (uint32_t)((uint32_t)1U << ch)) != 0U) {
        haint     &= (uint32_t)~((uint32_t)1U << ch);
        ptr_ch     =  (USB0_CH_TypeDef *)(XMC_USBH0_device.host_channel_registers) + ch;
        ptr_pipe   =  (XMC_USBH0_pipe_t    *)(&pipe[ch]);
        /*Local variable for HCINT*/
        dat = ptr_ch->HCINT;
        hcint      =  (uint32_t)(dat & ptr_ch->HCINTMSK);
        if ((hcint & USB_CH_HCINT_ChHltd_Msk) != 0U) {                                  /* channel halted ? */
          ptr_ch->HCINTMSK = 0U;                                                        /*  disable all channel interrupts */
          ptr_ch->HCINT    = USB_CH_HCINTx_ALL;                                                    /*  clear all interrupts */
          ptr_pipe->transfer_active = 0U;                                               /*  set status transfer not active */
          hcint = 0U;
        }
        if ((hcint & USB_CH_HCINT_XferCompl_Msk) != 0U) {                               /* data transfer finished ? */
          ptr_ch->HCINT   = USB_CH_HCINTx_ALL;                                                     /*  clear all interrupts */
          if ((ptr_ch->HCCHAR & USB_CH_HCCHAR_EPDir_Msk) == 0U) {                       /*  endpoint OUT ? */
            ptr_ch->HCINTMSK = 0U;                                                      /*   disable all channel interrupts */
            ptr_pipe->transfer_active = 0U;                                             /*   transfer not in progress */
            ptr_pipe->num_transferred_total += ptr_pipe->num_transferring;              /*   admin OUT transfer status */
            ptr_pipe->num_transferring       = 0U;                                      /*   admin OUT transfer status */
            if (ptr_pipe->num_transferred_total == ptr_pipe->num) {                     /*   all bytes transferred ? */
              ptr_pipe->in_use = 0U;                                                    /*    release pipe */
              ptr_pipe->event = (uint8_t)XMC_USBH_EVENT_TRANSFER_COMPLETE;                       /*    prepare event notification */
            }                                                                           
            hcint = 0U;                                                                 
          }                                                                             
          if ((ptr_ch->HCCHAR & USB_CH_HCCHAR_EPDir_Msk) != 0U) {                       /*  endpoint IN ? */
            ptr_pipe->in_use = 0U;                                                      /*   release pipe */
            ptr_pipe->event = (uint8_t)XMC_USBH_EVENT_TRANSFER_COMPLETE;                         /*   prepare event notification */
            XMC_lTriggerHaltChannel(ptr_ch);                                            /*   trigger channel halt */
          }
        }
        if ((hcint & USB_CH_HCINTMSK_AckMsk_Msk) != 0U) {                                /* ACK received ? */
          ptr_ch->HCINT = USB_CH_HCINTMSK_AckMsk_Msk;                                    /*  clear ACK interrupt */
          is_nack[ch] = false;
          if ((ptr_ch->HCCHAR & USB_CH_HCCHAR_EPDir_Msk) != 0U) {                        /*  endpoint IN ? */
            if ((ptr_pipe->num != ptr_pipe->num_transferred_total) &&                    /*   if all data was not transferred */
                (ptr_pipe->num_transferring != 0U)                 &&                    /*   if zero-length packet was not received */
               ((ptr_pipe->num_transferred_total%ptr_pipe->ep_max_packet_size) == 0U)){  /*   if short packet was not received */
               ptr_ch->HCCHAR |= (uint32_t)USB_CH_HCCHAR_ChEna_Msk;                                /*    trigger next transfer */
            }
          } else {                                                                        /* endpoint OUT */
            XMC_lTriggerHaltChannel(ptr_ch);                                              /*  trigger channel halt */
          } 
          hcint = 0U;
        }
        /*local variable for HCCHAR*/
        dat = ptr_ch->HCCHAR;
        if (((hcint & (USB_CH_HCINTMSK_StallMsk_Msk |                                    /* STALL */
                      USB_CH_HCINTMSK_NakMsk_Msk   |                                     /* or NAK */
                      USB_CH_HCINTx_ERRORS )) != 0U) &&                                  /* or transaction error */
            ((dat & USB_CH_HCCHAR_EPDir_Msk) == 0U))
        {                        /* and endpoint OUT */

            pktcnt = (uint32_t)((ptr_ch->HCTSIZ_BUFFERMODE & USB_CH_HCTSIZ_BUFFERMODE_PktCnt_Msk)  /*  administrate OUT transfer status */
                      >> USB_CH_HCTSIZ_BUFFERMODE_PktCnt_Pos);   
            mpsiz  = (ptr_ch->HCCHAR      ) & 0x000007FFU;
            if ((ptr_pipe->num_transferring >= mpsiz) && (pktcnt > 0U)) {
              ptr_pipe->num_transferred_total += (uint32_t)(ptr_pipe->num_transferring - (mpsiz * pktcnt));
            }
            ptr_pipe->num_transferring = 0U;
        }

        if ((hcint & USB_CH_HCINTMSK_NakMsk_Msk)!=0U) {                                /* if NAK */
            is_nack[ch] = true;
            ptr_pipe->event |= (uint8_t)XMC_USBH_EVENT_HANDSHAKE_NAK;
            ptr_ch->HCINT = USB_CH_HCINTMSK_NakMsk_Msk;                                /*    clear NAK interrupt */
            if ((ptr_ch->HCCHAR & USB_CH_HCCHAR_EPDir_Msk) != 0U) {                    /*    endpoint IN ? */
              if (ptr_pipe->ep_type == (uint8_t)XMC_USBH_ENDPOINT_INTERRUPT) {                   /*     is endpoint of type interrupt ? */
                XMC_lTriggerHaltChannel(ptr_ch);                                       /*      trigger channel halt (after halted will be restarted in next sof) */
              } else {                                                                 /*     is endpoint not of type interrupt ?*/
                ptr_ch->HCCHAR |= (uint32_t)USB_CH_HCCHAR_ChEna_Msk;                             /*      trigger next transfer */
              }
            } else { /* If endpoint OUT */                                             /*    endpoint OUT ? */
              XMC_lTriggerHaltChannel(ptr_ch);                                         /*     trigger channel halt */
            }
            hcint = 0U;
        }

        if ((hcint & USB_CH_HCINTMSK_StallMsk_Msk) != 0U) {                              /* if STALL */
            /*Reset the packet data toggle*/
            ptr_ch->HCINT   = USB_CH_HCINTMSK_StallMsk_Msk;                              /*  clear STALL interrupt */
            ptr_pipe->in_use = 0U;                                                       /*  release pipe */
            ptr_pipe->packet &= (uint32_t)(~XMC_USBH_PACKET_DATA_Msk);
            ptr_pipe->packet   |=  (uint32_t)XMC_USBH_PACKET_DATA0;
            ptr_pipe->event = (uint8_t)XMC_USBH_EVENT_HANDSHAKE_STALL;                            /*  prepare event notification */
            XMC_lTriggerHaltChannel(ptr_ch);                                             /*  trigger channel halt */
            hcint = 0U;
        } 
        if ((hcint & USB_CH_HCINTx_ERRORS) != 0U) {                                      /* if transaction error */
            ptr_ch->HCINT = USB_CH_HCINTx_ERRORS;                                        /*  clear all error interrupt */
            ptr_pipe->in_use = 0U;                                                       /*  release pipe */
            ptr_pipe->event = (uint8_t)XMC_USBH_EVENT_BUS_ERROR;                                  /*  prepare event notification */
            XMC_lTriggerHaltChannel(ptr_ch);                                             /*  trigger channel halt */
            hcint = 0U;
        }
        if ((ptr_pipe->transfer_active == 0U) && (ptr_pipe->in_use == 0U) && (ptr_pipe->event != 0U)) {
          XMC_USBH0_device.SignalPipeEvent_cb((XMC_USBH_EP_HANDLE)ptr_ch, (uint32_t)ptr_pipe->event);
          ptr_pipe->event  = 0U;
        }
      }
    }
  }
  /*Check if remote wakeup event detected*/
  if ((gintsts & USB_GINTSTS_HOSTMODE_WkUpInt_Msk) != 0U)
  {
    XMC_USBH0_device.global_register->GINTSTS_HOSTMODE =  USB_GINTSTS_HOSTMODE_WkUpInt_Msk; /* Clear wakeup interrupt */
    /*Recover PHY clock*/
    XMC_USBH0_device.global_register->PCGCCTL = XMC_USBH_PHY_CLK_UNGATE;
    /*Callback function execution*/
    XMC_USBH0_device.SignalPortEvent_cb(0U, XMC_USBH_EVENT_REMOTE_WAKEUP);
  }

  /* Handle restarts of unfinished transfers (due to NAK or ACK) */
  ptr_pipe = (XMC_USBH0_pipe_t *)(pipe);
  for (ch = 0U; ch < USBH0_MAX_PIPE_NUM; ch++) {
    if ((ptr_pipe->in_use == 1U) && (ptr_pipe->transfer_active == 0U)) {
      /* Restart periodic transfer if not in progress and interval expired */
      if (ptr_pipe->ep_type != (uint8_t)XMC_USBH_ENDPOINT_INTERRUPT) 
      {
            /*Data toggle if NACK not received*/
            if (!is_nack[ch])
            {
              switch (ptr_pipe->packet & (uint32_t)XMC_USBH_PACKET_DATA_Msk)
              {
                case XMC_USBH_PACKET_DATA0:
                  ptr_pipe->packet   &= (uint32_t)~XMC_USBH_PACKET_DATA_Msk;
                  ptr_pipe->packet   |= (uint32_t)XMC_USBH_PACKET_DATA1;
                  break;
                case XMC_USBH_PACKET_DATA1:
                  ptr_pipe->packet   &= (uint32_t)~XMC_USBH_PACKET_DATA_Msk;
                  ptr_pipe->packet   |= (uint32_t)XMC_USBH_PACKET_DATA0;
                  break;
                default:
                  break;
              }
            }
            else
            {
              is_nack[ch] = false;
            }
      }
      if (((ptr_pipe->ep_type == (uint8_t)XMC_USBH_ENDPOINT_INTERRUPT)&&(ptr_pipe->interrupt_triggered == 1U))||
                (ptr_pipe->ep_type != (uint8_t)XMC_USBH_ENDPOINT_INTERRUPT))
      {
        ptr_pipe->interrupt_triggered = 0U;
        ptr_pipe->transfer_active = 1U;
        (void)XMC_lStartTransfer (ptr_pipe, (((USB0_CH_TypeDef *)(XMC_USBH0_device.host_channel_registers)) + ch));
      }
    }
    ptr_pipe++;
  }
}

/*Function provides host mode interrupt status*/
uint32_t XMC_USBH_GetInterruptStatus(void)
{
  return XMC_USBH0_device.global_register->GINTSTS_HOSTMODE;
}

/*Function selects the port pin used as DRIVEVBUS*/
void XMC_USBH_Select_VBUS(XMC_GPIO_PORT_t* port, uint32_t pin)
{
  VBUS_port = port;
  VBUS_pin = pin;

  /*Configure the port pin alternate function*/
  XMC_GPIO_SetMode(VBUS_port, (uint8_t)VBUS_pin, XMC_GPIO_MODE_OUTPUT_PUSH_PULL_ALT1);
}

/*Function asserts the remote wakeup request by device by clearing the resume bit*/
void XMC_USBH_TurnOffResumeBit(void)
{
  uint32_t hprt;
  /*Clear resume bit*/
  hprt = XMC_USBH0_device.global_register->HPRT;
  hprt &= (uint32_t)~(USB_HPRT_PrtEna_Msk);
  hprt &= (uint32_t)~((uint32_t)USB_HPRT_PrtRes_Msk);
  XMC_USBH0_device.global_register->HPRT = hprt;
}



/*USB host driver assembling all the implementation into a single CMSIS compliant structure type*/
XMC_USBH_DRIVER_t Driver_USBH0 = {
  XMC_USBH_GetVersion,
  XMC_USBH_GetCapabilities,
  XMC_USBH_Initialize,
  XMC_USBH_Uninitialize,
  XMC_USBH_PowerControl,
  XMC_USBH_PortVbusOnOff,
  XMC_USBH_PortReset,
  XMC_USBH_PortSuspend,
  XMC_USBH_PortResume,
  XMC_USBH_PortGetState,
  XMC_USBH_PipeCreate,
  XMC_USBH_PipeModify,
  XMC_USBH_PipeDelete,
  XMC_USBH_PipeReset,
  XMC_USBH_PipeTransfer,
  XMC_USBH_PipeTransferGetResult,
  XMC_USBH_PipeTransferAbort,
  XMC_USBH_GetFrameNumber
};


/*Weak definition of delay function*/
__WEAK uint8_t XMC_USBH_osDelay(uint32_t MS)
{
  /*A precise time delay implementation for this function has to be provided*/
  while (1)
  {
    /*Wait*/
  }
}

#endif
