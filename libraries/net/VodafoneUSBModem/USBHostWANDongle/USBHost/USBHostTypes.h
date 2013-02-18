/* Copyright (c) 2010-2012 mbed.org, MIT License
*
* Permission is hereby granted, free of charge, to any person obtaining a copy of this software
* and associated documentation files (the "Software"), to deal in the Software without
* restriction, including without limitation the rights to use, copy, modify, merge, publish,
* distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the
* Software is furnished to do so, subject to the following conditions:
*
* The above copyright notice and this permission notice shall be included in all copies or
* substantial portions of the Software.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING
* BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
* NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM,
* DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

#ifndef USB_INC_H
#define USB_INC_H

#include "mbed.h"

enum USB_TYPE {
    USB_TYPE_DISCONNECTED = -10,
    USB_TYPE_NOTFOUND = -9,
    USB_TYPE_BADCONFIG = -8,
    USB_TYPE_FREE = -7,
    USB_TYPE_IDLE = -6,
    USB_TYPE_PROCESSING = -5,
    USB_TYPE_HALTED = -4, //Transfer on an ep is stalled
    USB_TYPE_BUSY = -3,
    USB_TYPE_TDFAIL = -2,
    USB_TYPE_ERROR = -1,
    USB_TYPE_OK = 0
};

#define AUDIO_CLASS 0x01
#define CDC_CLASS   0x02
#define MSD_CLASS   0x08
#define HID_CLASS   0x03


// From NXP's USBHostLite stack's usbhost_lpc17xx.h 
// Only the types names have been changed to avoid unecessary typedefs 


/*
**************************************************************************************************************
*                                                 NXP USB Host Stack
*
*                                     (c) Copyright 2008, NXP SemiConductors
*                                     (c) Copyright 2008, OnChip  Technologies LLC
*                                                 All Rights Reserved
*
*                                                  www.nxp.com
*                                               www.onchiptech.com
*
* File           : usbhost_lpc17xx.h
* Programmer(s)  : Ravikanth.P
* Version        :
*
**************************************************************************************************************
*/


// ------------------ HcControl Register ---------------------  
#define  OR_CONTROL_PLE                 0x00000004
#define  OR_CONTROL_CLE                 0x00000010
#define  OR_CONTROL_BLE                 0x00000020
#define  OR_CONTROL_HCFS                0x000000C0
#define  OR_CONTROL_HC_OPER             0x00000080
// ----------------- HcCommandStatus Register ----------------- 
#define  OR_CMD_STATUS_HCR              0x00000001
#define  OR_CMD_STATUS_CLF              0x00000002
#define  OR_CMD_STATUS_BLF              0x00000004
// --------------- HcInterruptStatus Register ----------------- 
#define  OR_INTR_STATUS_WDH             0x00000002
#define  OR_INTR_STATUS_RHSC            0x00000040
#define  OR_INTR_STATUS_UE              0x00000010
// --------------- HcInterruptEnable Register ----------------- 
#define  OR_INTR_ENABLE_WDH             0x00000002
#define  OR_INTR_ENABLE_RHSC            0x00000040
#define  OR_INTR_ENABLE_MIE             0x80000000
// ---------------- HcRhDescriptorA Register ------------------ 
#define  OR_RH_STATUS_LPSC              0x00010000
#define  OR_RH_STATUS_DRWE              0x00008000
// -------------- HcRhPortStatus[1:NDP] Register -------------- 
#define  OR_RH_PORT_CCS                 0x00000001
#define  OR_RH_PORT_PRS                 0x00000010
#define  OR_RH_PORT_CSC                 0x00010000
#define  OR_RH_PORT_PRSC                0x00100000
#define  OR_RH_PORT_LSDA                0x00000200


//
//**************************************************************************************************************
//*                                               FRAME INTERVAL
//**************************************************************************************************************
//

#define  FI                     0x2EDF           // 12000 bits per frame (-1)
#define  DEFAULT_FMINTERVAL     ((((6 * (FI - 210)) / 7) << 16) | FI)

//
//**************************************************************************************************************
//*                                       ENDPOINT DESCRIPTOR CONTROL FIELDS
//**************************************************************************************************************
//

#define  ED_SKIP            (uint32_t) (0x00001000)        // Skip this ep in queue

//
//**************************************************************************************************************
//*                                       TRANSFER DESCRIPTOR CONTROL FIELDS
//**************************************************************************************************************
//

#define  TD_ROUNDING        (uint32_t) (0x00040000)        // Buffer Rounding                             
#define  TD_SETUP           (uint32_t)(0)                  // Direction of Setup Packet                   
#define  TD_IN              (uint32_t)(0x00100000)         // Direction In                                
#define  TD_OUT             (uint32_t)(0x00080000)         // Direction Out                               
#define  TD_DELAY_INT(x)    (uint32_t)((x) << 21)          // Delay Interrupt                             
#define  TD_TOGGLE_0        (uint32_t)(0x02000000)         // Toggle 0                                    
#define  TD_TOGGLE_1        (uint32_t)(0x03000000)         // Toggle 1                                    
#define  TD_CC              (uint32_t)(0xF0000000)         // Completion Code                             

//
//**************************************************************************************************************
//*                                       USB STANDARD REQUEST DEFINITIONS
//**************************************************************************************************************
//

#define  DEVICE_DESCRIPTOR                     (1)
#define  CONFIGURATION_DESCRIPTOR              (2)
#define  INTERFACE_DESCRIPTOR                  (4)
#define  ENDPOINT_DESCRIPTOR                   (5)
#define  HID_DESCRIPTOR                        (33)

//  ----------- Control RequestType Fields  ----------- 
#define  USB_DEVICE_TO_HOST         0x80
#define  USB_HOST_TO_DEVICE         0x00
#define  USB_REQUEST_TYPE_CLASS     0x20
#define  USB_RECIPIENT_DEVICE       0x00
#define  USB_RECIPIENT_INTERFACE    0x01

// -------------- USB Standard Requests  -------------- 
#define  SET_ADDRESS                 5
#define  GET_DESCRIPTOR              6
#define  SET_CONFIGURATION           9
#define  SET_INTERFACE              11

// -------------- USB Descriptor Length  -------------- 
#define DEVICE_DESCRIPTOR_LENGTH            0x12
#define CONFIGURATION_DESCRIPTOR_LENGTH     0x09


//
//**************************************************************************************************************
//*                                       TYPE DEFINITIONS
//**************************************************************************************************************
//

// ----------- HostController EndPoint Descriptor ------------- 
typedef struct hcEd {               
    volatile  uint32_t  control;        // Endpoint descriptor control                              
    volatile  uint32_t  tailTD;         // Physical address of tail in Transfer descriptor list     
    volatile  uint32_t  headTD;         // Physcial address of head in Transfer descriptor list     
    volatile  uint32_t  nextED;         // Physical address of next Endpoint descriptor             
} HCED;

// ------------ HostController Transfer Descriptor ------------ 
typedef struct hcTd {                       
    volatile  uint32_t  control;        // Transfer descriptor control                              
    volatile  uint32_t  currBufPtr;     // Physical address of current buffer pointer               
    volatile  uint32_t  nextTD;         // Physical pointer to next Transfer Descriptor             
    volatile  uint32_t  bufEnd;         // Physical address of end of buffer                        
} HCTD;

// ----------- Host Controller Communication Area ------------  
typedef struct hcca {                   
    volatile  uint32_t  IntTable[32];   // Interrupt Table                                          
    volatile  uint32_t  FrameNumber;    // Frame Number                                             
    volatile  uint32_t  DoneHead;       // Done Head                                                
    volatile  uint8_t   Reserved[116];  // Reserved for future use                                  
    volatile  uint8_t   Unknown[4];     // Unused                                                   
} HCCA;



#endif
