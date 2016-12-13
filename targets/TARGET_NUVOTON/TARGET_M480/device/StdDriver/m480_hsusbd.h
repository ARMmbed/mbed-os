/**************************************************************************//**
 * @file     hsusbd.h
 * @version  V1.00
 * @brief    M480 HSUSBD driver header file
 *
 * @copyright (C) 2016 Nuvoton Technology Corp. All rights reserved.
 *****************************************************************************/
#ifndef __HSUSBD_H__
#define __HSUSBD_H__

#ifdef __cplusplus
extern "C"
{
#endif


/** @addtogroup M480_Device_Driver M480 Device Driver
  @{
*/

/** @addtogroup M480_HSUSBD_Driver HSUSBD Driver
  @{
*/

/** @addtogroup M480_HSUSBD_EXPORTED_CONSTANTS HSUSBD Exported Constants
  @{
*/
/// @cond HIDDEN_SYMBOLS
#define HSUSBD_MAX_EP     12

#define Maximum(a,b)    (a)>(b) ? (a) : (b)
#define Minimum(a,b)    (a)<(b) ? (a) : (b)


#define CEP     0xff    /*!< Control Endpoint  \hideinitializer */
#define EPA     0       /*!< Endpoint A  \hideinitializer */
#define EPB     1       /*!< Endpoint B  \hideinitializer */
#define EPC     2       /*!< Endpoint C  \hideinitializer */
#define EPD     3       /*!< Endpoint D  \hideinitializer */
#define EPE     4       /*!< Endpoint E  \hideinitializer */
#define EPF     5       /*!< Endpoint F  \hideinitializer */
#define EPG     6       /*!< Endpoint G  \hideinitializer */
#define EPH     7       /*!< Endpoint H  \hideinitializer */
#define EPI     8       /*!< Endpoint I  \hideinitializer */
#define EPJ     9       /*!< Endpoint J  \hideinitializer */
#define EPK     10      /*!< Endpoint K  \hideinitializer */
#define EPL     11      /*!< Endpoint L  \hideinitializer */

///* USB Request Type */
//#define REQ_STANDARD        0x00
//#define REQ_CLASS           0x20
//#define REQ_VENDOR          0x40

///* USB Standard Request */
//#define GET_STATUS          0x00
//#define CLEAR_FEATURE       0x01
//#define SET_FEATURE         0x03
//#define SET_ADDRESS         0x05
//#define GET_DESCRIPTOR      0x06
//#define SET_DESCRIPTOR      0x07
//#define GET_CONFIGURATION   0x08
//#define SET_CONFIGURATION   0x09
//#define GET_INTERFACE       0x0A
//#define SET_INTERFACE       0x0B
//#define SYNC_FRAME          0x0C

///* USB Descriptor Type */
//#define DESC_DEVICE         0x01
//#define DESC_CONFIG         0x02
//#define DESC_STRING         0x03
//#define DESC_INTERFACE      0x04
//#define DESC_ENDPOINT       0x05
//#define DESC_QUALIFIER      0x06
//#define DESC_OTHERSPEED     0x07
//#define DESC_IFPOWER        0x08
//#define DESC_OTG            0x09

///* USB HID Descriptor Type */
//#define DESC_HID            0x21
//#define DESC_HID_RPT        0x22

///* USB Descriptor Length */
//#define LEN_DEVICE          18
//#define LEN_QUALIFIER       10
//#define LEN_CONFIG          9
//#define LEN_INTERFACE       9
//#define LEN_ENDPOINT        7
//#define LEN_OTG             5
//#define LEN_HID             9

///* USB Endpoint Type */
//#define EP_ISO              0x01
//#define EP_BULK             0x02
//#define EP_INT              0x03

//#define EP_INPUT            0x80
//#define EP_OUTPUT           0x00

///* USB Feature Selector */
//#define FEATURE_DEVICE_REMOTE_WAKEUP    0x01
//#define FEATURE_ENDPOINT_HALT           0x00
/// @endcond HIDDEN_SYMBOLS
/********************* Bit definition of CEPCTL register **********************/
#define HSUSBD_CEPCTL_NAKCLR               ((uint32_t)0x00000000)      /*!<NAK clear  \hideinitializer */
#define HSUSBD_CEPCTL_STALL                ((uint32_t)0x00000002)      /*!<Stall  \hideinitializer */
#define HSUSBD_CEPCTL_ZEROLEN              ((uint32_t)0x00000004)      /*!<Zero length packet  \hideinitializer */
#define HSUSBD_CEPCTL_FLUSH                ((uint32_t)0x00000008)      /*!<CEP flush  \hideinitializer */

/********************* Bit definition of EPxRSPCTL register **********************/
#define HSUSBD_EP_RSPCTL_FLUSH             ((uint32_t)0x00000001)      /*!<Buffer Flush  \hideinitializer */
#define HSUSBD_EP_RSPCTL_MODE_AUTO         ((uint32_t)0x00000000)      /*!<Auto-Validate Mode  \hideinitializer */
#define HSUSBD_EP_RSPCTL_MODE_MANUAL       ((uint32_t)0x00000002)      /*!<Manual-Validate Mode  \hideinitializer */
#define HSUSBD_EP_RSPCTL_MODE_FLY          ((uint32_t)0x00000004)      /*!<Fly Mode  \hideinitializer */
#define HSUSBD_EP_RSPCTL_MODE_MASK         ((uint32_t)0x00000006)      /*!<Mode Mask  \hideinitializer */
#define HSUSBD_EP_RSPCTL_TOGGLE            ((uint32_t)0x00000008)      /*!<Clear Toggle bit  \hideinitializer */
#define HSUSBD_EP_RSPCTL_HALT              ((uint32_t)0x00000010)      /*!<Endpoint halt  \hideinitializer */
#define HSUSBD_EP_RSPCTL_ZEROLEN           ((uint32_t)0x00000020)      /*!<Zero length packet IN  \hideinitializer */
#define HSUSBD_EP_RSPCTL_SHORTTXEN         ((uint32_t)0x00000040)      /*!<Packet end  \hideinitializer */
#define HSUSBD_EP_RSPCTL_DISBUF            ((uint32_t)0x00000080)      /*!<Disable buffer  \hideinitializer */

/********************* Bit definition of EPxCFG register **********************/
#define HSUSBD_EP_CFG_VALID                ((uint32_t)0x00000001)      /*!<Endpoint Valid  \hideinitializer */
#define HSUSBD_EP_CFG_TYPE_BULK            ((uint32_t)0x00000002)      /*!<Endpoint type - bulk  \hideinitializer */
#define HSUSBD_EP_CFG_TYPE_INT             ((uint32_t)0x00000004)      /*!<Endpoint type - interrupt  \hideinitializer */
#define HSUSBD_EP_CFG_TYPE_ISO             ((uint32_t)0x00000006)      /*!<Endpoint type - isochronous  \hideinitializer */
#define HSUSBD_EP_CFG_TYPE_MASK            ((uint32_t)0x00000006)      /*!<Endpoint type mask  \hideinitializer */
#define HSUSBD_EP_CFG_DIR_OUT              ((uint32_t)0x00000000)      /*!<OUT endpoint  \hideinitializer */
#define HSUSBD_EP_CFG_DIR_IN               ((uint32_t)0x00000008)      /*!<IN endpoint  \hideinitializer */


/*@}*/ /* end of group M480_HSUSBD_EXPORTED_CONSTANTS */

/** @addtogroup M480_HSUSBD_EXPORTED_STRUCT HSUSBD Exported Struct
  @{
*/


typedef struct HSUSBD_CMD_STRUCT {
    uint8_t  bmRequestType;
    uint8_t  bRequest;
    uint16_t wValue;
    uint16_t wIndex;
    uint16_t wLength;

} S_HSUSBD_CMD_T; /*!<USB Setup Packet Structure */




typedef struct s_hsusbd_info {
    uint8_t *gu8DevDesc;            /*!< Device descriptor */
    uint8_t *gu8ConfigDesc;         /*!< Config descriptor */
    uint8_t **gu8StringDesc;        /*!< Pointer for USB String Descriptor pointers */
    uint8_t *gu8QualDesc;           /*!< Qualifier descriptor */
    uint8_t *gu8FullConfigDesc;     /*!< Full Speed Config descriptor */
    uint8_t *gu8HSOtherConfigDesc;  /*!< Other Speed Config descriptor */
    uint8_t *gu8FSOtherConfigDesc;  /*!< Other Speed Config descriptor */
    uint8_t **gu8HidReportDesc;   /*!< Pointer for HID Report descriptor */
    uint32_t *gu32HidReportSize;  /*!< Pointer for HID Report descriptor Size */
    uint8_t **gu8FSHidReportDesc;   /*!< Pointer for HID Report descriptor */
    uint32_t *gu32FSHidReportSize;  /*!< Pointer for HID Report descriptor Size */

} S_HSUSBD_INFO_T; /*!<USB Information Structure */


/*@}*/ /* end of group M480_HSUSBD_EXPORTED_STRUCT */

/// @cond HIDDEN_SYMBOLS
extern uint32_t g_u32HsEpStallLock;
extern uint8_t g_hsusbd_Configured;
extern uint8_t g_hsusbd_ShortPacket;
extern uint8_t g_hsusbd_CtrlZero;
extern uint8_t g_hsusbd_UsbAddr;
extern uint8_t g_hsusbd_EpHalt[];
extern uint32_t volatile g_hsusbd_DmaDone;
extern uint32_t g_hsusbd_CtrlInSize;
extern S_HSUSBD_INFO_T gsHSInfo;
extern S_HSUSBD_CMD_T gUsbCmd;
/// @endcond /* HIDDEN_SYMBOLS */

/** @addtogroup M480_HSUSBD_EXPORTED_MACROS HSUSBD Exported Macros
  @{
*/

#define HSUSBD_ENABLE_USB()               ((uint32_t)(HSUSBD->PHYCTL |= (HSUSBD_PHYCTL_PHYEN_Msk|HSUSBD_PHYCTL_DPPUEN_Msk))) /*!<Enable USB  \hideinitializer */
#define HSUSBD_DISABLE_USB()              ((uint32_t)(HSUSBD->PHYCTL &= ~HSUSBD_PHYCTL_DPPUEN_Msk)) /*!<Disable USB  \hideinitializer */
#define HSUSBD_ENABLE_PHY()               ((uint32_t)(HSUSBD->PHYCTL |= HSUSBD_PHYCTL_PHYEN_Msk)) /*!<Enable PHY  \hideinitializer */
#define HSUSBD_DISABLE_PHY()              ((uint32_t)(HSUSBD->PHYCTL &= ~HSUSBD_PHYCTL_PHYEN_Msk)) /*!<Disable PHY  \hideinitializer */
#define HSUSBD_SET_SE0()                  ((uint32_t)(HSUSBD->PHYCTL &= ~HSUSBD_PHYCTL_DPPUEN_Msk)) /*!<Enable SE0, Force USB PHY Transceiver to Drive SE0  \hideinitializer */
#define HSUSBD_CLR_SE0()                  ((uint32_t)(HSUSBD->PHYCTL |= HSUSBD_PHYCTL_DPPUEN_Msk)) /*!<Disable SE0  \hideinitializer */
#define HSUSBD_SET_ADDR(addr)             (HSUSBD->FADDR = (addr)) /*!<Set USB address  \hideinitializer */
#define HSUSBD_GET_ADDR()                 ((uint32_t)(HSUSBD->FADDR)) /*!<Get USB address  \hideinitializer */
#define HSUSBD_ENABLE_USB_INT(intr)       (HSUSBD->GINTEN = (intr)) /*!<Enable USB Interrupt  \hideinitializer */
#define HSUSBD_ENABLE_BUS_INT(intr)       (HSUSBD->BUSINTEN = (intr)) /*!<Enable BUS Interrupt  \hideinitializer */
#define HSUSBD_GET_BUS_INT_FLAG()         (HSUSBD->BUSINTSTS)        /*!<Clear Bus interrupt flag  \hideinitializer */
#define HSUSBD_CLR_BUS_INT_FLAG(flag)     (HSUSBD->BUSINTSTS = flag) /*!<Clear Bus interrupt flag  \hideinitializer */
#define HSUSBD_ENABLE_CEP_INT(intr)       (HSUSBD->CEPINTEN = (intr)) /*!<Enable CEP Interrupt  \hideinitializer */
#define HSUSBD_CLR_CEP_INT_FLAG(flag)     (HSUSBD->CEPINTSTS = flag) /*!<Clear CEP interrupt flag  \hideinitializer */
#define HSUSBD_SET_CEP_STATE(flag)        (HSUSBD->CEPCTL = flag) /*!<Set CEP state  \hideinitializer */
#define HSUSBD_START_CEP_IN(size)         (HSUSBD->CEPTXCNT = size) /*!<Start CEP IN Transfer  \hideinitializer */
#define HSUSBD_SET_MAX_PAYLOAD(ep, size)  (HSUSBD->EP[ep].EPMPS = (size)) /*!<Set EPx Maximum Packet Size  \hideinitializer */
#define HSUSBD_ENABLE_EP_INT(ep, intr)    (HSUSBD->EP[ep].EPINTEN = (intr)) /*!<Enable EPx Interrupt  \hideinitializer */
#define HSUSBD_GET_EP_INT_FLAG(ep)        (HSUSBD->EP[ep].EPINTSTS) /*!<Get EPx interrupt flag  \hideinitializer */
#define HSUSBD_CLR_EP_INT_FLAG(ep, flag)  (HSUSBD->EP[ep].EPINTSTS = (flag)) /*!<Clear EPx interrupt flag  \hideinitializer */
#define HSUSBD_SET_DMA_LEN(len)           (HSUSBD->DMACNT = len) /*!<Set DMA transfer length  \hideinitializer */
#define HSUSBD_SET_DMA_ADDR(addr)         (HSUSBD->DMAADDR = addr) /*!<Set DMA transfer address  \hideinitializer */
#define HSUSBD_SET_DMA_READ(epnum)        (HSUSBD->DMACTL = (HSUSBD->DMACTL & ~HSUSBD_DMACTL_EPNUM_Msk) | HSUSBD_DMACTL_DMARD_Msk | epnum | 0x100) /*!<Set DMA transfer type to read \hideinitializer */
#define HSUSBD_SET_DMA_WRITE(epnum)       (HSUSBD->DMACTL = (HSUSBD->DMACTL & ~(HSUSBD_DMACTL_EPNUM_Msk | HSUSBD_DMACTL_DMARD_Msk | 0x100)) | epnum) /*!<Set DMA transfer type to write \hideinitializer */
#define HSUSBD_ENABLE_DMA()               (HSUSBD->DMACTL |= HSUSBD_DMACTL_DMAEN_Msk) /*!<Enable DMA transfer  \hideinitializer */
#define HSUSBD_IS_ATTACHED()              ((uint32_t)(HSUSBD->PHYCTL & HSUSBD_PHYCTL_VBUSDET_Msk)) /*!<Check cable connect state  \hideinitializer */

/*@}*/ /* end of group M480_HSUSBD_EXPORTED_MACROS */

/** @addtogroup M480_HSUSBD_EXPORTED_FUNCTIONS HSUSBD Exported Functions
  @{
*/
/**
  * @brief  HSUSBD_memcpy, Copy bytes hardware limitation
  * @param[in]  u8Dst   Destination pointer.
  * @param[in]  u8Src   Source pointer.
  * @param[in]  i32Size Copy size.
  * @retval None.
  */
static __INLINE void HSUSBD_MemCopy(uint8_t *u8Dst, uint8_t *u8Src, int32_t i32Size)
{
    while (i32Size--) *u8Dst++ = *u8Src++;
}

/**
  * @brief  HSUSBD_ResetDMA
  * @param  None
  * @retval None.
  */
static __INLINE void HSUSBD_ResetDMA(void)
{
    HSUSBD->DMACNT = 0;
    HSUSBD->DMACTL = 0x80;
    HSUSBD->DMACTL = 0x00;
}
/**
  * @brief  HSUSBD_SetEpBufAddr, Set Endpoint buffer address
  * @param[in]  u32Ep      Endpoint Number
  * @param[in]  u32Base    Buffer Start Address
  * @param[in]  u32Len     Buffer length
  * @retval None.
  */
static __INLINE void HSUSBD_SetEpBufAddr(uint32_t u32Ep, uint32_t u32Base, uint32_t u32Len)
{
    if (u32Ep == CEP) {
        HSUSBD->CEPBUFSTART = u32Base;
        HSUSBD->CEPBUFEND   = u32Base + u32Len - 1;
    } else {
        HSUSBD->EP[u32Ep].EPBUFSTART = u32Base;
        HSUSBD->EP[u32Ep].EPBUFEND = u32Base + u32Len - 1;
    }
}

/**
  * @brief  HSUSBD_ConfigEp, Config Endpoint
  * @param[in]  u32Ep      USB endpoint
  * @param[in]  u32EpNum   Endpoint number
  * @param[in]  u32EpType  Endpoint type
  * @param[in]  u32EpDir   Endpoint direction
  * @retval None.
  */
static __INLINE void HSUSBD_ConfigEp(uint32_t u32Ep, uint32_t u32EpNum, uint32_t u32EpType, uint32_t u32EpDir)
{
    if (u32EpType == HSUSBD_EP_CFG_TYPE_BULK)
        HSUSBD->EP[u32Ep].EPRSPCTL = (HSUSBD_EP_RSPCTL_FLUSH|HSUSBD_EP_RSPCTL_MODE_AUTO);
    else if (u32EpType == HSUSBD_EP_CFG_TYPE_INT)
        HSUSBD->EP[u32Ep].EPRSPCTL = (HSUSBD_EP_RSPCTL_FLUSH|HSUSBD_EP_RSPCTL_MODE_MANUAL);
    else if (u32EpType == HSUSBD_EP_CFG_TYPE_ISO)
        HSUSBD->EP[u32Ep].EPRSPCTL = (HSUSBD_EP_RSPCTL_FLUSH|HSUSBD_EP_RSPCTL_MODE_FLY);

    HSUSBD->EP[u32Ep].EPCFG = (u32EpType|u32EpDir|HSUSBD_EP_CFG_VALID|(u32EpNum << 4));
}

/**
  * @brief       Set USB endpoint stall state
  * @param[in]   u32Ep  The USB endpoint ID.
  * @return      None
  * @details     Set USB endpoint stall state for the specified endpoint ID. Endpoint will respond STALL token automatically.
  */
static __INLINE void HSUSBD_SetEpStall(uint32_t u32Ep)
{
    if (u32Ep == CEP)
        HSUSBD_SET_CEP_STATE(HSUSBD_CEPCTL_STALL);
    else {
        HSUSBD->EP[u32Ep].EPRSPCTL = (HSUSBD->EP[u32Ep].EPRSPCTL & 0xf7) | HSUSBD_EP_RSPCTL_HALT;
    }
}

/**
 * @brief       Set USB endpoint stall state
 *
 * @param[in]   u32EpNum         USB endpoint
 * @return      None
 *
 * @details     Set USB endpoint stall state, endpoint will return STALL token.
 */
static __INLINE void HSUSBD_SetStall(uint32_t u32EpNum)
{
    int i;

    if (u32EpNum == 0)
        HSUSBD_SET_CEP_STATE(HSUSBD_CEPCTL_STALL);
    else {
        for (i=0; i<HSUSBD_MAX_EP; i++) {
            if (((HSUSBD->EP[i].EPCFG & 0xf0) >> 4) == u32EpNum) {
                HSUSBD->EP[i].EPRSPCTL = (HSUSBD->EP[i].EPRSPCTL & 0xf7) | HSUSBD_EP_RSPCTL_HALT;
            }
        }
    }
}

/**
  * @brief       Clear USB endpoint stall state
  * @param[in]   u32Ep  The USB endpoint ID.
  * @return      None
  * @details     Clear USB endpoint stall state for the specified endpoint ID. Endpoint will respond ACK/NAK token.
  */
static __INLINE void  HSUSBD_ClearEpStall(uint32_t u32Ep)
{
    HSUSBD->EP[u32Ep].EPRSPCTL = HSUSBD_EP_RSPCTL_TOGGLE;
}

/**
 * @brief       Clear USB endpoint stall state
 *
 * @param[in]   u32EpNum         USB endpoint
 * @return      None
 *
 * @details     Clear USB endpoint stall state, endpoint will return ACK/NAK token.
 */
static __INLINE void HSUSBD_ClearStall(uint32_t u32EpNum)
{
    int i;

    for (i=0; i<HSUSBD_MAX_EP; i++) {
        if (((HSUSBD->EP[i].EPCFG & 0xf0) >> 4) == u32EpNum) {
            HSUSBD->EP[i].EPRSPCTL = HSUSBD_EP_RSPCTL_TOGGLE;
        }
    }
}

/**
  * @brief       Get USB endpoint stall state
  * @param[in]   u32Ep  The USB endpoint ID.
  * @retval      0      USB endpoint is not stalled.
  * @retval      Others USB endpoint is stalled.
  * @details     Get USB endpoint stall state of the specified endpoint ID.
  */
static __INLINE uint32_t HSUSBD_GetEpStall(uint32_t u32Ep)
{
    return (HSUSBD->EP[u32Ep].EPRSPCTL & HSUSBD_EP_RSPCTL_HALT);
}

/**
 * @brief       Get USB endpoint stall state
 *
 * @param[in]   u32EpNum         USB endpoint
 * @retval      0: USB endpoint is not stalled.
 * @retval      non-0: USB endpoint is stalled.
 *
 * @details     Get USB endpoint stall state.
 */
static __INLINE uint32_t HSUSBD_GetStall(uint32_t u32EpNum)
{
    int i;

    for (i=0; i<HSUSBD_MAX_EP; i++) {
        if (((HSUSBD->EP[i].EPCFG & 0xf0) >> 4) == u32EpNum) {
            return (HSUSBD->EP[i].EPRSPCTL & HSUSBD_EP_RSPCTL_HALT);
        }
    }
    return 0;
}


/*-------------------------------------------------------------------------------------------*/
typedef void (*HSUSBD_VENDOR_REQ)(void); /*!<USB Vendor request callback function */
typedef void (*HSUSBD_CLASS_REQ)(void); /*!<USB Class request callback function */
typedef void (*HSUSBD_SET_INTERFACE_REQ)(uint32_t u32AltInterface); /*!<USB Standard request "Set Interface" callback function */

void HSUSBD_Open(S_HSUSBD_INFO_T *param, HSUSBD_CLASS_REQ pfnClassReq, HSUSBD_SET_INTERFACE_REQ pfnSetInterface);
void HSUSBD_Start(void);
void HSUSBD_ProcessSetupPacket(void);
void HSUSBD_StandardRequest(void);
void HSUSBD_UpdateDeviceState(void);
void HSUSBD_PrepareCtrlIn(uint8_t *pu8Buf, uint32_t u32Size);
void HSUSBD_CtrlIn(void);
void HSUSBD_CtrlOut(uint8_t *pu8Buf, uint32_t u32Size);
void HSUSBD_SwReset(void);
void HSUSBD_SetVendorRequest(HSUSBD_VENDOR_REQ pfnVendorReq);



/*@}*/ /* end of group M480_HSUSBD_EXPORTED_FUNCTIONS */

/*@}*/ /* end of group M480_HSUSBD_Driver */

/*@}*/ /* end of group M480_Device_Driver */

#ifdef __cplusplus
}
#endif

#endif //__HSUSBD_H__

/*** (C) COPYRIGHT 2016 Nuvoton Technology Corp. ***/
