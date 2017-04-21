/**************************************************************************//**
 * @file     usbd.c
 * @version  V1.00
 * $Revision: 15 $
 * $Date: 14/10/02 4:14p $
 * @brief    NUC472/NUC442 USBD driver source file
 *
 * @note
 * Copyright (C) 2014 Nuvoton Technology Corp. All rights reserved.
*****************************************************************************/
#include "NUC472_442.h"

/** @addtogroup NUC472_442_Device_Driver NUC472/NUC442 Device Driver
  @{
*/

/** @addtogroup NUC472_442_USBD_Driver USBD Driver
  @{
*/


/** @addtogroup NUC472_442_USBD_EXPORTED_FUNCTIONS USBD Exported Functions
  @{
*/
/*--------------------------------------------------------------------------*/
/*!< Global variables for Control Pipe */
S_USBD_CMD_T gUsbCmd;
S_USBD_INFO_T *g_usbd_sInfo;

VENDOR_REQ g_usbd_pfnVendorRequest = NULL;
CLASS_REQ g_usbd_pfnClassRequest = NULL;
SET_INTERFACE_REQ g_usbd_pfnSetInterface = NULL;

static uint8_t *g_usbd_CtrlInPointer = 0;
static uint32_t g_usbd_UsbConfig = 0;
static uint32_t g_usbd_UsbAltInterface = 0;
static uint32_t g_usbd_CtrlMaxPktSize = 64;

#ifdef __ICCARM__
#pragma data_alignment=4
static uint8_t g_usbd_buf[12];
#elif defined (__CC_ARM)
__align(4) static uint8_t g_usbd_buf[12];
#elif defined ( __GNUC__ )
static uint8_t g_usbd_buf[12] __attribute__((aligned (4)));
#endif


uint32_t g_usbd_CtrlZero = 0;
uint32_t g_usbd_UsbAddr = 0;
uint32_t g_usbd_CtrlInSize = 0;
uint32_t g_usbd_ShortPacket = 0;
uint32_t volatile g_usbd_DmaDone = 0;
uint32_t g_usbd_Configured = 0;

/**
 * @brief       USBD Initial
 *
 * @param[in]   param               Descriptor
 * @param[in]   pfnClassReq         Class Request Callback Function
 * @param[in]   pfnSetInterface     SetInterface Request Callback Function
 *
 * @return      None
 *
 * @details     This function is used to initial USBD.
 */
void USBD_Open(S_USBD_INFO_T *param, CLASS_REQ pfnClassReq, SET_INTERFACE_REQ pfnSetInterface)
{
    g_usbd_sInfo = param;
    g_usbd_pfnClassRequest = pfnClassReq;
    g_usbd_pfnSetInterface = pfnSetInterface;

    /* get EP0 maximum packet size */
    g_usbd_CtrlMaxPktSize = g_usbd_sInfo->gu8DevDesc[7];

    /* Initial USB engine */
    /* Enable PHY */
    USBD_ENABLE_PHY();
    /* wait PHY clock ready */
    while (1) {
        USBD->EPAMPS = 0x20;
        if (USBD->EPAMPS == 0x20)
            break;
    }
    /* Force SE0, and then clear it to connect*/
    USBD_SET_SE0();
}

/**
 * @brief       USBD Start
 *
 * @param[in]   None
 *
 * @return      None
 *
 * @details     This function is used to start transfer
 */
void USBD_Start(void)
{
    USBD_CLR_SE0();
}

/**
 * @brief       Process Setup Packet
 *
 * @param[in]   None
 *
 * @return      None
 *
 * @details     This function is used to process Setup packet.
 */
void USBD_ProcessSetupPacket(void)
{
    // Setup packet process
    gUsbCmd.bmRequestType = (uint8_t)(USBD->SETUP1_0 & 0xff);
    gUsbCmd.bRequest = (int8_t)(USBD->SETUP1_0 >> 8) & 0xff;
    gUsbCmd.wValue = (uint16_t)USBD->SETUP3_2;
    gUsbCmd.wIndex = (uint16_t)USBD->SETUP5_4;
    gUsbCmd.wLength = (uint16_t)USBD->SETUP7_6;

    /* USB device request in setup packet: offset 0, D[6..5]: 0=Standard, 1=Class, 2=Vendor, 3=Reserved */
    switch (gUsbCmd.bmRequestType & 0x60) {
    case REQ_STANDARD: { // Standard
        USBD_StandardRequest();
        break;
    }
    case REQ_CLASS: { // Class
        if (g_usbd_pfnClassRequest != NULL) {
            g_usbd_pfnClassRequest();
        }
        break;
    }
    case REQ_VENDOR: { // Vendor
        if (g_usbd_pfnVendorRequest != NULL) {
            g_usbd_pfnVendorRequest();
        }
        break;
    }
    default: { // reserved
        /* Setup error, stall the device */
        USBD_SET_CEP_STATE(USBD_CEPCTL_STALLEN_Msk);
        break;
    }
    }
}

/**
 * @brief       Get Descriptor request
 *
 * @param[in]   None
 *
 * @return      None
 *
 * @details     This function is used to process GetDescriptor request.
 */
int USBD_GetDescriptor(void)
{
    uint32_t u32Len;

    u32Len = gUsbCmd.wLength;
    g_usbd_CtrlZero = 0;

    switch ((gUsbCmd.wValue & 0xff00) >> 8) {
    // Get Device Descriptor
    case DESC_DEVICE: {
        u32Len = Minimum(u32Len, LEN_DEVICE);
        USBD_PrepareCtrlIn((uint8_t *)g_usbd_sInfo->gu8DevDesc, u32Len);
        break;
    }
    // Get Configuration Descriptor
    case DESC_CONFIG: {
        uint32_t u32TotalLen;

        if (USBD->OPER & 0x04) { /* high speed */
            u32TotalLen = g_usbd_sInfo->gu8ConfigDesc[3];
            u32TotalLen = g_usbd_sInfo->gu8ConfigDesc[2] + (u32TotalLen << 8);

            u32Len = Minimum(u32Len, u32TotalLen);
            if ((u32Len % g_usbd_CtrlMaxPktSize) == 0)
                g_usbd_CtrlZero = 1;

            USBD_PrepareCtrlIn((uint8_t *)g_usbd_sInfo->gu8ConfigDesc, u32Len);
        } else { /* full speed */
            u32TotalLen = g_usbd_sInfo->gu8OtherConfigDesc[3];
            u32TotalLen = g_usbd_sInfo->gu8OtherConfigDesc[2] + (u32TotalLen << 8);

            u32Len = Minimum(u32Len, u32TotalLen);
            if ((u32Len % g_usbd_CtrlMaxPktSize) == 0)
                g_usbd_CtrlZero = 1;

            USBD_PrepareCtrlIn((uint8_t *)g_usbd_sInfo->gu8OtherConfigDesc, u32Len);
        }
        break;
    }
    // Get Qualifier Descriptor
    case DESC_QUALIFIER: {
        u32Len = Minimum(u32Len, LEN_QUALIFIER);
        USBD_PrepareCtrlIn((uint8_t *)g_usbd_sInfo->gu8QualDesc, u32Len);
        break;
    }
    // Get Other Speed Descriptor - Full speed
    case DESC_OTHERSPEED: {
        uint32_t u32TotalLen;

        u32TotalLen = g_usbd_sInfo->gu8OtherConfigDesc[3];
        u32TotalLen = g_usbd_sInfo->gu8OtherConfigDesc[2] + (u32TotalLen << 8);

        u32Len = Minimum(u32Len, u32TotalLen);
        if ((u32Len % g_usbd_CtrlMaxPktSize) == 0)
            g_usbd_CtrlZero = 1;

        USBD_PrepareCtrlIn((uint8_t *)g_usbd_sInfo->gu8OtherConfigDesc, u32Len);
        break;
    }
    // Get HID Descriptor
    case DESC_HID: {
        u32Len = Minimum(u32Len, LEN_HID);
        USBD_MemCopy(g_usbd_buf, (uint8_t *)&g_usbd_sInfo->gu8ConfigDesc[LEN_CONFIG+LEN_INTERFACE], u32Len);
        USBD_PrepareCtrlIn(g_usbd_buf, u32Len);
        break;
    }
    // Get Report Descriptor
    case DESC_HID_RPT: {
        if ((u32Len % g_usbd_CtrlMaxPktSize) == 0)
            g_usbd_CtrlZero = 1;

        switch (gUsbCmd.wIndex & 0xff) {
        case 0: {
            u32Len = Minimum(u32Len, g_usbd_sInfo->gu32HidReportSize[0]);
            USBD_PrepareCtrlIn((uint8_t *)g_usbd_sInfo->gu8HidReportDesc[0], u32Len);
            break;
        }
        case 1: {
            u32Len = Minimum(u32Len, g_usbd_sInfo->gu32HidReportSize[1]);
            USBD_PrepareCtrlIn((uint8_t *)g_usbd_sInfo->gu8HidReportDesc[1], u32Len);
            break;
        }
        case 2: {
            u32Len = Minimum(u32Len, g_usbd_sInfo->gu32HidReportSize[2]);
            USBD_PrepareCtrlIn((uint8_t *)g_usbd_sInfo->gu8HidReportDesc[2], u32Len);
            break;
        }
        }
        break;
    }
    // Get String Descriptor
    case DESC_STRING: {
        // Get Language
        if ((gUsbCmd.wValue & 0xff) == 0) {
            u32Len = Minimum(u32Len, 4);
            USBD_PrepareCtrlIn((uint8_t *)g_usbd_sInfo->gu8StrLangDesc, u32Len);
        } else {
            // Get String Descriptor
            switch (gUsbCmd.wValue & 0xff) {
            case 1: {
                u32Len = Minimum(u32Len, g_usbd_sInfo->gu8StrVendorDesc[0]);
                if ((u32Len % g_usbd_CtrlMaxPktSize) == 0)
                    g_usbd_CtrlZero = 1;

                USBD_PrepareCtrlIn((uint8_t *)g_usbd_sInfo->gu8StrVendorDesc, u32Len);
                break;
            }
            case 2: {
                u32Len = Minimum(u32Len, g_usbd_sInfo->gu8StrProductDesc[0]);
                if ((u32Len % g_usbd_CtrlMaxPktSize) == 0)
                    g_usbd_CtrlZero = 1;

                USBD_PrepareCtrlIn((uint8_t *)g_usbd_sInfo->gu8StrProductDesc, u32Len);
                break;
            }
            default:
                // Not support. Reply STALL.
                USBD_SET_CEP_STATE(USBD_CEPCTL_STALLEN_Msk);
                return 1;
            }
        }
        break;
    }
    default:
        // Not support. Reply STALL.
        USBD_SET_CEP_STATE(USBD_CEPCTL_STALLEN_Msk);
        return 1;
    }
    return 0;
}


/**
 * @brief       Process USB standard request
 *
 * @param[in]   None
 *
 * @return      None
 *
 * @details     This function is used to process USB Standard Request.
 */
void USBD_StandardRequest(void)
{
    /* clear global variables for new request */
    g_usbd_CtrlInPointer = 0;
    g_usbd_CtrlInSize = 0;

    if (gUsbCmd.bmRequestType & 0x80) { /* request data transfer direction */
        // Device to host
        switch (gUsbCmd.bRequest) {
        case USBD_GET_CONFIGURATION: {
            // Return current configuration setting
            USBD_PrepareCtrlIn((uint8_t *)&g_usbd_UsbConfig, 1);

            USBD_CLR_CEP_INT_FLAG(USBD_CEPINTSTS_INTKIF_Msk);
            USBD_ENABLE_CEP_INT(USBD_CEPINTEN_INTKIEN_Msk);
            break;
        }
        case USBD_GET_DESCRIPTOR: {
            if (!USBD_GetDescriptor()) {
                USBD_CLR_CEP_INT_FLAG(USBD_CEPINTSTS_INTKIF_Msk);
                USBD_ENABLE_CEP_INT(USBD_CEPINTEN_INTKIEN_Msk);
            }
            break;
        }
        case USBD_GET_INTERFACE: {
            // Return current interface setting
            USBD_PrepareCtrlIn((uint8_t *)&g_usbd_UsbAltInterface, 1);

            USBD_CLR_CEP_INT_FLAG(USBD_CEPINTSTS_INTKIF_Msk);
            USBD_ENABLE_CEP_INT(USBD_CEPINTEN_INTKIEN_Msk);
            break;
        }
        case USBD_GET_STATUS: {
            // Device
            if (gUsbCmd.bmRequestType == 0x80) {
                if (g_usbd_sInfo->gu8ConfigDesc[7] & 0x40)
                    g_usbd_buf[0] = 1; // Self-Powered
                else
                    g_usbd_buf[0] = 0; // bus-Powered
            }
            // Interface
            else if (gUsbCmd.bmRequestType == 0x81)
                g_usbd_buf[0] = 0;
            // Endpoint
            else if (gUsbCmd.bmRequestType == 0x82) {
                uint8_t ep = gUsbCmd.wIndex & 0xF;
                g_usbd_buf[0] = USBD_GetStall(ep)? 1 : 0;
            }
            g_usbd_buf[1] = 0;
            USBD_PrepareCtrlIn(g_usbd_buf, 2);
            USBD_CLR_CEP_INT_FLAG(USBD_CEPINTSTS_INTKIF_Msk);
            USBD_ENABLE_CEP_INT(USBD_CEPINTEN_INTKIEN_Msk);
            break;
        }
        default: {
            /* Setup error, stall the device */
            USBD_SET_CEP_STATE(USBD_CEPCTL_STALLEN_Msk);
            break;
        }
        }
    } else {
        // Host to device
        switch (gUsbCmd.bRequest) {
        case USBD_CLEAR_FEATURE: {
            /* Status stage */
            USBD_CLR_CEP_INT_FLAG(USBD_CEPINTSTS_STSDONEIF_Msk);
            USBD_SET_CEP_STATE(USB_CEPCTL_NAKCLR);
            USBD_ENABLE_CEP_INT(USBD_CEPINTEN_STSDONEIEN_Msk);
            break;
        }
        case USBD_SET_ADDRESS: {
            g_usbd_UsbAddr = (uint8_t)gUsbCmd.wValue;

            // DATA IN for end of setup
            /* Status Stage */
            USBD_CLR_CEP_INT_FLAG(USBD_CEPINTSTS_STSDONEIF_Msk);
            USBD_SET_CEP_STATE(USB_CEPCTL_NAKCLR);
            USBD_ENABLE_CEP_INT(USBD_CEPINTEN_STSDONEIEN_Msk);
            break;
        }
        case USBD_SET_CONFIGURATION: {
            g_usbd_UsbConfig = (uint8_t)gUsbCmd.wValue;
            g_usbd_Configured = 1;
            // DATA IN for end of setup
            /* Status stage */
            USBD_CLR_CEP_INT_FLAG(USBD_CEPINTSTS_STSDONEIF_Msk);
            USBD_SET_CEP_STATE(USB_CEPCTL_NAKCLR);
            USBD_ENABLE_CEP_INT(USBD_CEPINTEN_STSDONEIEN_Msk);
            break;
        }
        case USBD_SET_FEATURE: {
            /* Status stage */
            USBD_CLR_CEP_INT_FLAG(USBD_CEPINTSTS_STSDONEIF_Msk);
            USBD_SET_CEP_STATE(USB_CEPCTL_NAKCLR);
            USBD_ENABLE_CEP_INT(USBD_CEPINTEN_STSDONEIEN_Msk);
            break;
        }
        case USBD_SET_INTERFACE: {
            g_usbd_UsbAltInterface = (uint8_t)gUsbCmd.wValue;
            if (g_usbd_pfnSetInterface != NULL)
                g_usbd_pfnSetInterface(g_usbd_UsbAltInterface);
            /* Status stage */
            USBD_CLR_CEP_INT_FLAG(USBD_CEPINTSTS_STSDONEIF_Msk);
            USBD_SET_CEP_STATE(USB_CEPCTL_NAKCLR);
            USBD_ENABLE_CEP_INT(USBD_CEPINTEN_STSDONEIEN_Msk);
            break;
        }
        default: {
            /* Setup error, stall the device */
            USBD_SET_CEP_STATE(USBD_CEPCTL_STALLEN_Msk);
            break;
        }
        }
    }
}

/**
 * @brief       Update Device State
 *
 * @param[in]   None
 *
 * @return      None
 *
 * @details     This function is used to update Device state when Setup packet complete
 */
void USBD_UpdateDeviceState(void)
{
    switch (gUsbCmd.bRequest) {
    case USBD_SET_ADDRESS: {
        USBD_SET_ADDR(g_usbd_UsbAddr);
        break;
    }
    case USBD_SET_FEATURE: {
        if(gUsbCmd.wValue == FEATURE_ENDPOINT_HALT)
            USBD_SetStall(gUsbCmd.wIndex & 0xF);
        break;
    }
    case USBD_CLEAR_FEATURE: {
        if(gUsbCmd.wValue == FEATURE_ENDPOINT_HALT)
            USBD_ClearStall(gUsbCmd.wIndex & 0xF);
        break;
    }
    default:
        ;
    }
}


/**
 * @brief       Prepare Control IN transaction
 *
 * @param[in]   pu8Buf      Control IN data pointer
 * @param[in]   u32Size     IN transfer size
 *
 * @return      None
 *
 * @details     This function is used to prepare Control IN transfer
 */
void USBD_PrepareCtrlIn(uint8_t *pu8Buf, uint32_t u32Size)
{
    g_usbd_CtrlInPointer = pu8Buf;
    g_usbd_CtrlInSize = u32Size;
}



/**
 * @brief       Start Control IN transfer
 *
 * @param[in]   None
 *
 * @return      None
 *
 * @details     This function is used to start Control IN
 */
void USBD_CtrlIn(void)
{
    int volatile i;
    uint32_t volatile count;

    // Process remained data
    if(g_usbd_CtrlInSize >= g_usbd_CtrlMaxPktSize) {
        // Data size > MXPLD
        for (i=0; i<(g_usbd_CtrlMaxPktSize >> 2); i++, g_usbd_CtrlInPointer+=4)
            USBD->CEPDAT = *(uint32_t *)g_usbd_CtrlInPointer;
        USBD_START_CEP_IN(g_usbd_CtrlMaxPktSize);
        g_usbd_CtrlInSize -= g_usbd_CtrlMaxPktSize;
    } else {
        // Data size <= MXPLD
        for (i=0; i<(g_usbd_CtrlInSize >> 2); i++, g_usbd_CtrlInPointer+=4)
            USBD->CEPDAT = *(uint32_t *)g_usbd_CtrlInPointer;

        count = g_usbd_CtrlInSize % 4;
        for (i=0; i<count; i++)
            USBD->CEPDAT_BYTE = *(uint8_t *)(g_usbd_CtrlInPointer + i);

        USBD_START_CEP_IN(g_usbd_CtrlInSize);
        g_usbd_CtrlInPointer = 0;
        g_usbd_CtrlInSize = 0;
    }
}

/**
 * @brief       Start Control OUT transaction
 *
 * @param[in]   pu8Buf      Control OUT data pointer
 * @param[in]   u32Size     OUT transfer size
 *
 * @return      None
 *
 * @details     This function is used to start Control OUT transfer
 */
void USBD_CtrlOut(uint8_t *pu8Buf, uint32_t u32Size)
{
    int volatile i;

    while(1) {
        if (USBD->CEPINTSTS & USBD_CEPINTSTS_RXPKIF_Msk) {
            for (i=0; i<u32Size; i++)
                *(uint8_t *)(pu8Buf + i) = USBD->CEPDAT_BYTE;
            USBD->CEPINTSTS = USBD_CEPINTSTS_RXPKIF_Msk;
            break;
        }
    }
}

/**
 * @brief       Clear all software flags
 *
 * @param[in]   None
 *
 * @return      None
 *
 * @details     This function is used to clear all software control flag
 */
void USBD_SwReset(void)
{
    // Reset all variables for protocol
    g_usbd_UsbAddr = 0;
    g_usbd_DmaDone = 0;
    g_usbd_ShortPacket = 0;
    g_usbd_Configured = 0;

    // Reset USB device address
    USBD_SET_ADDR(0);
}

/**
 * @brief       USBD Set Vendor Request
 *
 * @param[in]   pfnVendorReq         Vendor Request Callback Function
 *
 * @return      None
 *
 * @details     This function is used to set USBD vendor request callback function
 */
void USBD_SetVendorRequest(VENDOR_REQ pfnVendorReq)
{
    g_usbd_pfnVendorRequest = pfnVendorReq;
}


/*@}*/ /* end of group NUC472_442_GPIO_EXPORTED_FUNCTIONS */

/*@}*/ /* end of group NUC472_442_GPIO_Driver */

/*@}*/ /* end of group NUC472_442_Device_Driver */

/*** (C) COPYRIGHT 2013 Nuvoton Technology Corp. ***/
