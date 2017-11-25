/***************************************************************************//**
 * @file em_usb.h
 * @brief USB protocol stack library API for EFM32.
 * @version 3.20.14
 *******************************************************************************
 * @section License
 * <b>(C) Copyright 2014 Silicon Labs, http://www.silabs.com</b>
 *******************************************************************************
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 ******************************************************************************/

#ifndef __EM_USB_H
#define __EM_USB_H

#include "em_device.h"
#include "em_assert.h"
#if defined( USB_PRESENT ) && ( USB_COUNT == 1 )
#include "usbconfig.h"
#if defined( USB_DEVICE ) || defined( USB_HOST )

#include <string.h>
#include <stddef.h>
#include "em_common.h"
/* Workaround for em_common naming change so that we don't need to rework the
   entire USB HAL */
#define EFM32_PACK_START(x)        SL_PACK_START(x)
#define EFM32_PACK_END()           SL_PACK_END()
#define EFM32_MIN(a, b)            SL_MIN(a, b)
#define EFM32_MAX(a, b)            SL_MAX(a, b)
#define EFM32_ATTRIBUTE_PACKED     SL_ATTRIBUTE_PACKED
#define EFM32_ATTRIBUTE_ALIGN(X)   SL_ATTRIBUTE_ALIGN(X)
#define EFM32_ALIGN(X)             SL_ALIGN(X)
#define EFM32_WEAK                 SL_WEAK
#define EFM32_ATTRIBUTE_SECTION(X) SL_ATTRIBUTE_SECTION(X)

#if defined( USB_USE_PRINTF )
#include <stdio.h>
#endif

#ifdef __cplusplus
extern "C" {
#endif

#ifdef __CC_ARM
#pragma anon_unions
#endif

/***************************************************************************//**
 * @addtogroup USB
 * @brief USB HOST and DEVICE protocol stacks.
 * @{
 ******************************************************************************/

/***************************************************************************//**
 * @addtogroup USB_COMMON
 * @brief Common parts for both HOST and DEVICE USB stacks, see @ref usb_device
 *        and @ref usb_host pages for device and host library documentation.
 * @{
 ******************************************************************************/

#define SILABS_USB_VID          0x10C4          /**< Silicon Labs Vendor ID, supplied by USB-IF.       */

/* SETUP request, direction of data stage */
#define USB_SETUP_DIR_OUT       0               /**< Setup request data stage OUT direction value.     */
#define USB_SETUP_DIR_IN        1               /**< Setup request data stage IN direction value.      */
#define USB_SETUP_DIR_MASK      0x80            /**< Setup request data stage direction mask.          */
#define USB_SETUP_DIR_D2H       0x80            /**< Setup request data stage IN direction mask.       */
#define USB_SETUP_DIR_H2D       0x00            /**< Setup request data stage OUT direction mask.      */

/* SETUP request type */
#define USB_SETUP_TYPE_STANDARD       0         /**< Standard setup request value.                     */
#define USB_SETUP_TYPE_CLASS          1         /**< Class setup request value.                        */
#define USB_SETUP_TYPE_VENDOR         2         /**< Vendor setup request value.                       */
#define USB_SETUP_TYPE_STANDARD_MASK  0x00      /**< Standard setup request mask.                      */
#define USB_SETUP_TYPE_CLASS_MASK     0x20      /**< Class setup request mask.                         */
#define USB_SETUP_TYPE_VENDOR_MASK    0x40      /**< Vendor setup request mask.                        */

/* SETUP request recipient */
#define USB_SETUP_RECIPIENT_DEVICE    0         /**< Setup request device recipient value.             */
#define USB_SETUP_RECIPIENT_INTERFACE 1         /**< Setup request interface recipient value.          */
#define USB_SETUP_RECIPIENT_ENDPOINT  2         /**< Setup request endpoint recipient value.           */
#define USB_SETUP_RECIPIENT_OTHER     3         /**< Setup request other recipient value.              */

/* SETUP standard request codes for Full Speed devices */
#define GET_STATUS                0             /**< Standard setup request GET_STATUS.                */
#define CLEAR_FEATURE             1             /**< Standard setup request CLEAR_FEATURE.             */
#define SET_FEATURE               3             /**< Standard setup request SET_FEATURE.               */
#define SET_ADDRESS               5             /**< Standard setup request SET_ADDRESS.               */
#define GET_DESCRIPTOR            6             /**< Standard setup request GET_DESCRIPTOR.            */
#define SET_DESCRIPTOR            7             /**< Standard setup request SET_DESCRIPTOR.            */
#define GET_CONFIGURATION         8             /**< Standard setup request GET_CONFIGURATION.         */
#define SET_CONFIGURATION         9             /**< Standard setup request SET_CONFIGURATION.         */
#define GET_INTERFACE             10            /**< Standard setup request GET_INTERFACE.             */
#define SET_INTERFACE             11            /**< Standard setup request SET_INTERFACE.             */
#define SYNCH_FRAME               12            /**< Standard setup request SYNCH_FRAME.               */

/* SETUP class request codes */
#define USB_HID_GET_REPORT        0x01          /**< HID class setup request GET_REPORT.               */
#define USB_HID_GET_IDLE          0x02          /**< HID class setup request GET_IDLE.                 */
#define USB_HID_SET_REPORT        0x09          /**< HID class setup request SET_REPORT.               */
#define USB_HID_SET_IDLE          0x0A          /**< HID class setup request SET_IDLE.                 */
#define USB_HID_SET_PROTOCOL      0x0B          /**< HID class setup request SET_PROTOCOL.             */
#define USB_CDC_SETLINECODING     0x20          /**< CDC class setup request SET_LINE_CODING.          */
#define USB_CDC_GETLINECODING     0x21          /**< CDC class setup request GET_LINE_CODING.          */
#define USB_CDC_SETCTRLLINESTATE  0x22          /**< CDC class setup request SET_CONTROL_LINE_STATE.   */
#define USB_MSD_BOTRESET          0xFF          /**< MSD class setup request Bulk only transfer reset. */
#define USB_MSD_GETMAXLUN         0xFE          /**< MSD class setup request Get Max LUN.              */
#define USB_AUDIO_GET_CUR         0x81          /**< Audio class setup request GET_CUR.                */
#define USB_AUDIO_SET_CUR         0x01          /**< Audio class setup request SET_CUR.                */
#define USB_AUDIO_GET_CUR         0x81          /**< Audio class setup request GET_CUR.                */
#define USB_AUDIO_SET_MIN         0x02          /**< Audio class setup request SET_MIN.                */
#define USB_AUDIO_GET_MIN         0x82          /**< Audio class setup request GET_MIN.                */
#define USB_AUDIO_SET_MAX         0x03          /**< Audio class setup request SET_MAX.                */
#define USB_AUDIO_GET_MAX         0x83          /**< Audio class setup request GET_MAX.                */
#define USB_AUDIO_SET_RES         0x04          /**< Audio class setup request SET_RES.                */
#define USB_AUDIO_GET_RES         0x84          /**< Audio class setup request GET_RES.                */
#define USB_AUDIO_SET_MEM         0x05          /**< Audio class setup request SET_MEM.                */
#define USB_AUDIO_GET_MEM         0x85          /**< Audio class setup request GET_MEM.                */
#define USB_AUDIO_GET_STAT        0xFF          /**< Audio class setup request GET_STAT.               */

/* SETUP command GET/SET_DESCRIPTOR decriptor types */
#define USB_DEVICE_DESCRIPTOR             1     /**< DEVICE descriptor value.                          */
#define USB_CONFIG_DESCRIPTOR             2     /**< CONFIGURATION descriptor value.                   */
#define USB_STRING_DESCRIPTOR             3     /**< STRING descriptor value.                          */
#define USB_MAX_STRING_DESCRIPTOR_CHARS   126   /**< Maximum STRING descriptor bString length.         */
#define USB_INTERFACE_DESCRIPTOR          4     /**< INTERFACE descriptor value.                       */
#define USB_ENDPOINT_DESCRIPTOR           5     /**< ENDPOINT descriptor value.                        */
#define USB_DEVICE_QUALIFIER_DESCRIPTOR   6     /**< DEVICE_QUALIFIER descriptor value.                */
#define USB_OTHER_SPEED_CONFIG_DESCRIPTOR 7     /**< OTHER_SPEED_CONFIGURATION descriptor value.       */
#define USB_INTERFACE_POWER_DESCRIPTOR    8     /**< INTERFACE_POWER descriptor value.                 */
#define USB_INTERFACE_ASSOCIATION_DESCRIPTOR 11 /**< INTERFACE_ASSOCIATION descriptor value.           */
#define USB_HID_DESCRIPTOR                0x21  /**< HID descriptor value.                             */
#define USB_SMARTCARD_DESCRIPTOR          0x21  /**< Smartcard usb-ccid-specific Descriptor Type.      */
#define USB_HID_REPORT_DESCRIPTOR         0x22  /**< HID REPORT descriptor value.                      */
#define USB_CS_INTERFACE_DESCRIPTOR       0x24  /**< Audio Class-specific interface Descriptor Type.   */
#define USB_CS_ENDPOINT_DESCRIPTOR        0x25  /**< Audio Class-specific endpoint Descriptor Type.    */
#define USB_HUB_DESCRIPTOR                0x29  /**< HUB descriptor value.                             */
#define USB_CA_HEADER_DESCRIPTOR          1     /**< Audio Class-Specific AC Interface Header descriptor.*/
#define USB_CA_INPUT_TERMINAL_DESCRIPTOR  2     /**< Audio Class-Specific AC Interface Input Terminal desc. */
#define USB_CA_OUTPUT_TERMINAL_DESCRIPTOR 3     /**< Audio Class-Specific AC Interface Output Terminal desc.*/
#define USB_CA_MIXER_UNIT_DESCRIPTOR      4     /**< Audio Class-Specific AC Interface Mixer descriptor.*/
#define USB_CA_SELECTOR_UNIT_DESCRIPTOR   5     /**< Audio Class-Specific AC Interface Selector desc.  */
#define USB_CA_FEATURE_UNIT_DESCRIPTOR    6     /**< Audio Class-Specific AC Interface Feature desc.   */
#define USB_CA_PROCESSING_UNIT_DESCRIPTOR 7     /**< Audio Class-Specific AC Interface Processing desc.*/
#define USB_CA_EXTENSION_UNIT_DESCRIPTOR  8     /**< Audio Class-Specific AC Interface Extension desc. */
#define USB_CA_EP_GENERAL_DESCRIPTOR      1     /**< Audio Class-Specific general descriptor subtype code.*/
#define USB_CA_AS_GENERAL_DESCRIPTOR      1     /**< Audio Class-Specific AS Interface General descriptor.*/
#define USB_CA_FORMAT_TYPE_DESCRIPTOR     2     /**< Audio Class-Specific AS Interface Format Type desc. */

#define USB_DEVICE_DESCSIZE               18    /**< Device descriptor size.                           */
#define USB_CONFIG_DESCSIZE               9     /**< Configuration descriptor size.                    */
#define USB_INTERFACE_DESCSIZE            9     /**< Interface descriptor size.                        */
#define USB_ENDPOINT_DESCSIZE             7     /**< Endpoint descriptor size.                         */
#define USB_DEVICE_QUALIFIER_DESCSIZE     10    /**< Device qualifier descriptor size.                 */
#define USB_OTHER_SPEED_CONFIG_DESCSIZE   9     /**< Device other speed configuration descriptor size. */
#define USB_INTERFACE_ASSOCIATION_DESCSIZE 8    /**< INTERFACE_ASSOCIATION descriptor size.            */
#define USB_HID_DESCSIZE                  9     /**< HID descriptor size.                              */
#define USB_SMARTCARD_DESCSIZE            54    /**< CCID descriptor size.                             */
#define USB_CDC_HEADER_FND_DESCSIZE       5     /**< CDC Header functional descriptor size.            */
#define USB_CDC_CALLMNG_FND_DESCSIZE      5     /**< CDC Call Management functional descriptor size.   */
#define USB_CDC_ACM_FND_DESCSIZE          4     /**< CDC Abstract Control Management functional descriptor size.*/
#define USB_CA_INPUT_TERMINAL_DESCSIZE    12    /**< Audio Input Terminal descriptor size.             */
#define USB_CA_OUTPUT_TERMINAL_DESCSIZE   9     /**< Audio Output Terminal descriptor size.            */
#define USB_CA_EP_GENERAL_DESCSIZE        7     /**< Audio Class-Specific general descriptor subtype size.*/
#define USB_CA_AS_GENERAL_DESCSIZE        7     /**< Audio Class-Specific AS Interface General desc size.*/
#define USB_CA_STD_AS_ENDPOINT_DESCSZIE   9     /**< Audio-class standard audio stream descriptor size.*/

/* Misc. USB definitions */
#define USB_LS_CTRL_EP_MAXSIZE  8               /**< The max size of low speed control endpoints.      */
#define USB_LS_INTR_EP_MAXSIZE  8               /**< The max size of low speed interrupt endpoints.    */
#define USB_FS_CTRL_EP_MAXSIZE  64              /**< The max size of full speed control endpoints.     */
#define USB_FS_INTR_EP_MAXSIZE  64              /**< The max size of full speed interrupt endpoints.   */
#define USB_FS_BULK_EP_MAXSIZE  64              /**< The max size of full speed bulk endpoints.        */
#define USB_FS_ISOC_EP_MAXSIZE  1023            /**< The max size of full speed isochronous endpoints. */
#define USB_EPTYPE_CTRL         0               /**< Endpoint type control.                            */
#define USB_EPTYPE_ISOC         1               /**< Endpoint type isochron.                           */
#define USB_EPTYPE_BULK         2               /**< Endpoint type bulk.                               */
#define USB_EPTYPE_INTR         3               /**< Endpoint type interrupt.                          */
#define USB_EPSYNC_NO           (0 << 2)        /**< Endpoint synchronization type, none.              */
#define USB_EPSYNC_ASYNC        (1 << 2)        /**< Endpoint synchronization type, asynchronous.      */
#define USB_EPSYNC_ADAPTIVE     (2 << 2)        /**< Endpoint synchronization type, adaptive.          */
#define USB_EPSYNC_SYNC         (3 << 2)        /**< Endpoint synchronization type, synchronous.       */
#define USB_EP_DIR_IN           0x80            /**< Endpoint direction mask.                          */
#define USB_SETUP_PKT_SIZE      8               /**< Setup request packet size.                        */
#define USB_EPNUM_MASK          0x0F            /**< Endpoint number mask.                             */
#define USB_LANGID_ENUS         0x0409          /**< English-United States language id.                */
#define USB_MAX_DEVICE_ADDRESS  127             /**< Maximum allowable device address.                 */

#define CONFIG_DESC_BM_REMOTEWAKEUP 0x20        /**< Configuration descriptor attribute macro.         */
#define CONFIG_DESC_BM_SELFPOWERED  0x40        /**< Configuration descriptor attribute macro.         */
#define CONFIG_DESC_BM_RESERVED_D7  0x80        /**< Configuration descriptor attribute macro.         */
#define CONFIG_DESC_BM_TRANSFERTYPE 0x03        /**< Configuration descriptor transfer type bitmask.   */
#define CONFIG_DESC_MAXPOWER_mA(x)  (((x)+1)/2) /**< Configuration descriptor power macro.             */

#define DEVICE_IS_SELFPOWERED       0x0001      /**< Standard request GET_STATUS bitmask.              */
#define REMOTE_WAKEUP_ENABLED       0x0002      /**< Standard request GET_STATUS bitmask.              */
#define USB_FEATURE_ENDPOINT_HALT         0     /**< Standard request CLEAR/SET_FEATURE bitmask.       */
#define USB_FEATURE_DEVICE_REMOTE_WAKEUP  1     /**< Standard request CLEAR/SET_FEATURE bitmask.       */

#define HUB_FEATURE_PORT_RESET            4     /**< HUB class request CLEAR/SET_PORT_FEATURE feature selector. */
#define HUB_FEATURE_PORT_POWER            8     /**< HUB class request CLEAR/SET_PORT_FEATURE feature selector. */
#define HUB_FEATURE_C_PORT_CONNECTION     16    /**< HUB class request CLEAR/SET_PORT_FEATURE feature selector. */
#define HUB_FEATURE_C_PORT_RESET          20    /**< HUB class request CLEAR/SET_PORT_FEATURE feature selector. */
#define HUB_FEATURE_PORT_INDICATOR        22    /**< HUB class request CLEAR/SET_PORT_FEATURE feature selector. */

#define USB_CLASS_CDC                     2     /**< CDC device/interface class code.                    */
#define USB_CLASS_CDC_DATA                0x0A  /**< CDC Data interface class code.                      */
#define USB_CLASS_CDC_ACM                 2     /**< CDC Abstract Control Model interface subclass code. */
#define USB_CLASS_CDC_HFN                 0     /**< CDC class Header Functional Descriptor subtype.     */
#define USB_CLASS_CDC_CMNGFN              1     /**< CDC class Call Management Functional Descriptor subtype.*/
#define USB_CLASS_CDC_ACMFN               2     /**< CDC class Abstract Control Management Functional Descriptor subtype.*/
#define USB_CLASS_CDC_UNIONFN             6     /**< CDC class Union Functional Descriptor subtype.      */

#define USB_CLASS_HID                     3     /**< HID device/interface class code.                    */
#define USB_CLASS_HID_KEYBOARD            1     /**< HID keyboard interface protocol code.               */
#define USB_CLASS_HID_MOUSE               2     /**< HID mouse interface protocol code.                  */

#define USB_CLASS_HUB                     9     /**< HUB device/interface class code.                    */

#define USB_CLASS_MSD                     8     /**< MSD device/interface class code.                    */
#define USB_CLASS_MSD_BOT_TRANSPORT       0x50  /**< MSD Bulk Only Transport protocol.                   */
#define USB_CLASS_MSD_SCSI_CMDSET         6     /**< MSD Subclass SCSI transparent command set.          */
#define USB_CLASS_MSD_CSW_CMDPASSED       0     /**< MSD BOT Command status wrapper command passed code. */
#define USB_CLASS_MSD_CSW_CMDFAILED       1     /**< MSD BOT Command status wrapper command failed code. */
#define USB_CLASS_MSD_CSW_PHASEERROR      2     /**< MSD BOT Command status wrapper cmd phase error code.*/

#define USB_CLASS_AUDIO                   1     /**< Audio interface class code.                         */
#define USB_CLASS_AUDIO_CONTROL           1     /**< Audio subclass code for control interface.          */
#define USB_CLASS_AUDIO_STREAMING         2     /**< Audio subclass code for streaming interface.        */
#define USB_CLASS_AUDIO_MIDISTREAMING     3     /**< Audio subclass code for midi streaming interface.   */

/*** Triplet for the device descriptor of a composite device using IAD descriptors. ***/
#define USB_CLASS_MISCELLANEOUS           0xEF  /**< MISCELLANEOUS device class code.                    */
#define USB_CLASS_MISC_COMMON_SUBCLASS    2     /**< MISCELLANEOUS Common sub class code.                */
#define USB_CLASS_MISC_IAD_PROTOCOL       1     /**< MISCELLANEOUS Interface Association Descriptor protocol code. */

#define PORT_FULL_SPEED                   1     /**< Full speed return value for USBH_GetPortSpeed(). */
#define PORT_LOW_SPEED                    2     /**< Low speed return value for USBH_GetPortSpeed().  */

#if defined( __GNUC__  )                  /* GCC compilers */
#if defined( __CHAR16_TYPE__ )
typedef __CHAR16_TYPE__ char16_t;
#else
typedef unsigned short char16_t;
#endif

#elif defined( __ICCARM__ )               /* IAR compiler */
#include <uchar.h>

#elif defined( __CC_ARM )                 /* MDK-ARM compiler */
typedef unsigned short char16_t;
#endif

/** Macro for creating USB compliant UTF-16LE UNICODE string descriptors.
 *  @n Example: STATIC_CONST_STRING_DESC( iManufacturer, 'E','n','e','r','g','y',' ','M','i','c','r','o',' ','A','S' );
 *  @note The size of the resulting struct will be two byte larger than a USB string
 *        descriptor. This is to accommodate a terminating null char for the string.
 *        The value assigned to the 'len' member does not take this into account
 *        and is therefore correct usb wise.
 */
#define STATIC_CONST_STRING_DESC( _name, ... )                  \
EFM32_PACK_START( 1 )                                           \
typedef struct                                                  \
{                                                               \
  uint8_t  len;                                                 \
  uint8_t  type;                                                \
  char16_t name[ 1 + sizeof( (char16_t[]){__VA_ARGS__} ) / 2];  \
} __attribute__ ((packed)) _##_name;                            \
EFM32_PACK_END()                                                \
EFM32_ALIGN( 4 )                                                \
EFM32_PACK_START( 1 )                                           \
static const _##_name _name __attribute__ ((aligned(4)))=       \
{                                                               \
  .len  = sizeof( _##_name ) - 2,                               \
  .type = USB_STRING_DESCRIPTOR,                                \
  .name = {__VA_ARGS__},                                        \
  .name[ ( ( sizeof( _##_name ) - 2 ) / 2 ) - 1 ] = '\0'        \
}                                                               \
EFM32_PACK_END()

/** Macro for creating USB compliant language string descriptors.
 *  @n Example: STATIC_CONST_STRING_DESC_LANGID( langID, 0x04, 0x09 );
 */
#define STATIC_CONST_STRING_DESC_LANGID( _name, x, y )      \
EFM32_PACK_START( 1 )                                       \
typedef struct                                              \
{                                                           \
  uint8_t len;                                              \
  uint8_t type;                                             \
  uint8_t name[ 2 ];                                        \
} __attribute__ ((packed)) _##_name;                        \
EFM32_PACK_END()                                            \
EFM32_ALIGN( 4 )                                            \
EFM32_PACK_START( 1 )                                       \
static const _##_name _name __attribute__ ((aligned(4)))=   \
{                                                           \
  .len = 4,                                                 \
  .type = USB_STRING_DESCRIPTOR,                            \
  .name = { y, x }                                          \
}                                                           \
EFM32_PACK_END()

/** Macro for creating WORD (4 byte) aligned uint8_t array with size which
 *  is a multiple of WORD size.
 *  @n Example: @n UBUF( rxBuffer, 37 );  =>  uint8_t rxBuffer[ 40 ];
 */
#if !defined(__GNUC__)
#define        UBUF( x, y ) EFM32_ALIGN( 4 )        uint8_t x[((y)+3)&~3]
#define STATIC_UBUF( x, y ) EFM32_ALIGN( 4 ) static uint8_t x[((y)+3)&~3]
#else
#define        UBUF( x, y ) uint8_t x[((y)+3)&~3] __attribute__ ((aligned(4)))

/** Macro for creating WORD (4 byte) aligned static uint8_t arrays with size which
 *  is a multiple of WORD size.
 *  @n Example: @n STATIC_UBUF( rxBuffer, 37 );  =>  static uint8_t rxBuffer[ 40 ];
 */
#define STATIC_UBUF( x, y ) static uint8_t x[((y)+3)&~3] __attribute__ ((aligned(4)))
#endif


/** @brief USB transfer status enumerator. */
typedef enum
{
  /* NOTE: Please keep in sync with table errMsg[] in em_usbhal.c */
  USB_STATUS_OK              = 0,               /**< No errors detected.                               */
  USB_STATUS_REQ_ERR         = -1,              /**< Setup request error.                              */
  USB_STATUS_EP_BUSY         = -2,              /**< Endpoint is busy.                                 */
  USB_STATUS_REQ_UNHANDLED   = -3,              /**< Setup request not handled.                        */
  USB_STATUS_ILLEGAL         = -4,              /**< Illegal operation attempted.                      */
  USB_STATUS_EP_STALLED      = -5,              /**< Endpoint is stalled.                              */
  USB_STATUS_EP_ABORTED      = -6,              /**< Endpoint transfer was aborted.                    */
  USB_STATUS_EP_ERROR        = -7,              /**< Endpoint transfer error.                          */
  USB_STATUS_EP_NAK          = -8,              /**< Endpoint NAK'ed transfer request.                 */
  USB_STATUS_DEVICE_UNCONFIGURED = -9,          /**< Device is unconfigured.                           */
  USB_STATUS_DEVICE_SUSPENDED    = -10,         /**< Device is suspended.                              */
  USB_STATUS_DEVICE_RESET    = -11,             /**< Device is/was reset.                              */
  USB_STATUS_TIMEOUT         = -12,             /**< Transfer timeout.                                 */
  USB_STATUS_DEVICE_REMOVED  = -13,             /**< Device was removed.                               */
  USB_STATUS_HC_BUSY         = -14,             /**< Host channel is busy.                             */
  USB_STATUS_DEVICE_MALFUNCTION = -15,          /**< Malfunctioning device attached.                   */
  USB_STATUS_PORT_OVERCURRENT = -16,            /**< VBUS shortcircuit/overcurrent failure.            */
} USB_Status_TypeDef;
/** @} (end addtogroup USB_COMMON) */


#if defined( USB_DEVICE )
/***************************************************************************//**
 * @addtogroup USB_DEVICE
 * @brief USB DEVICE protocol stack, see @ref usb_device page for detailed documentation.
 * @{
 ******************************************************************************/

#define USB_PWRSAVE_MODE_OFF       0 /**< No energy saving mode selected.                       */
#define USB_PWRSAVE_MODE_ONSUSPEND 1 /**< Enter USB power-save mode on suspend.                 */
#define USB_PWRSAVE_MODE_ONVBUSOFF 2 /**< Enter USB power-save mode when not attached to host.  */
#define USB_PWRSAVE_MODE_ENTEREM2  4 /**< Enter EM2 while in power-save mode.                   */

#define USB_USBC_32kHz_CLK_LFXO    0 /**< Use 32kHz LFXO clock while in powersave mode.         */
#define USB_USBC_32kHz_CLK_LFRCO   1 /**< Use 32kHz LFRCO clock while in powersave mode.        */

/** @brief USB device state enumerator. */
typedef enum
{
  USBD_STATE_NONE       = 0,                    /**< Device state is undefined/unknown.                */
  USBD_STATE_ATTACHED   = 1,                    /**< Device state is ATTACHED.                         */
  USBD_STATE_POWERED    = 2,                    /**< Device state is POWERED.                          */
  USBD_STATE_DEFAULT    = 3,                    /**< Device state is DEFAULT.                          */
  USBD_STATE_ADDRESSED  = 4,                    /**< Device state is ADDRESSED.                        */
  USBD_STATE_CONFIGURED = 5,                    /**< Device state is CONFIGURED.                       */
  USBD_STATE_SUSPENDED  = 6,                    /**< Device state is SUSPENDED.                        */
  USBD_STATE_LASTMARKER = 7,                    /**< Device state enum end marker.                     */
} USBD_State_TypeDef;
/** @} (end addtogroup USB_DEVICE) */
#endif /* defined( USB_DEVICE ) */

/** @addtogroup USB_COMMON
 *  @{*/

/** @brief USB Setup request package. */
EFM32_PACK_START( 1 )
typedef struct
{
  union
  {
    struct
    {
      union
      {
        struct
        {
          uint8_t Recipient : 5;                /**< Request recipient (device, interface, endpoint or other).*/
          uint8_t Type      : 2;                /**< Request type (standard, class or vendor).         */
          uint8_t Direction : 1;                /**< Transfer direction of SETUP data phase.           */
        };
        uint8_t bmRequestType;                  /**< Request characteristics.                          */
      };
      uint8_t   bRequest;                       /**< Request code.                                     */
      uint16_t  wValue;                         /**< Varies according to request.                      */
      uint16_t  wIndex;                         /**< Index or offset, varies according to request.     */
      uint16_t  wLength;                        /**< Number of bytes to transfer if there is a data stage.*/
    };
  uint32_t  dw[2];
  };
} __attribute__ ((packed)) USB_Setup_TypeDef;
EFM32_PACK_END()


/** @brief USB Device Descriptor. */
EFM32_PACK_START( 1 )
typedef struct
{
  uint8_t  bLength;                             /**< Size of this descriptor in bytes                  */
  uint8_t  bDescriptorType;                     /**< Constant DEVICE Descriptor Type                   */
  uint16_t bcdUSB;                              /**< USB Specification Release Number in Binary-Coded
                                                     Decimal                                           */
  uint8_t  bDeviceClass;                        /**< Class code (assigned by the USB-IF)               */
  uint8_t  bDeviceSubClass;                     /**< Subclass code (assigned by the USB-IF)            */
  uint8_t  bDeviceProtocol;                     /**< Protocol code (assigned by the USB-IF)            */
  uint8_t  bMaxPacketSize0;                     /**< Maximum packet size for endpoint zero             */
  uint16_t idVendor;                            /**< Vendor ID (assigned by the USB-IF)                */
  uint16_t idProduct;                           /**< Product ID (assigned by the manufacturer)         */
  uint16_t bcdDevice;                           /**< Device release number in binary-coded decimal     */
  uint8_t  iManufacturer;                       /**< Index of string descriptor describing manufacturer*/
  uint8_t  iProduct;                            /**< Index of string descriptor describing product     */
  uint8_t  iSerialNumber;                       /**< Index of string descriptor describing the device
                                                     serialnumber                                      */
  uint8_t  bNumConfigurations;                  /**< Number of possible configurations                 */
} __attribute__ ((packed)) USB_DeviceDescriptor_TypeDef;
EFM32_PACK_END()


/** @brief USB Configuration Descriptor. */
EFM32_PACK_START( 1 )
typedef struct
{
  uint8_t  bLength;                             /**< Size of this descriptor in bytes                  */
  uint8_t  bDescriptorType;                     /**< Constant CONFIGURATION Descriptor Type            */
  uint16_t wTotalLength;                        /**< Total length of data returned for this
                                                     configuration. Includes the combined length of all
                                                     descriptors (configuration, interface, endpoint,
                                                     and class- or vendor-specific) returned for this
                                                     configuration.                                    */
  uint8_t  bNumInterfaces;                      /**< Number of interfaces supported by this
                                                     configuration                                     */
  uint8_t  bConfigurationValue;                 /**< Value to use as an argument to the
                                                     SetConfiguration request to select this
                                                     configuration.                                    */
  uint8_t  iConfiguration;                      /**< Index of string descriptor describing this
                                                     configuration.                                    */
  uint8_t  bmAttributes;                        /**< Configuration characteristics.
                                                     @n D7: Reserved (set to one)
                                                     @n D6: Self-powered
                                                     @n D5: Remote Wakeup
                                                     @n D4...0: Reserved (reset to zero)               */
  uint8_t  bMaxPower;                           /**< Maximum power consumption of the USB device, unit
                                                     is 2mA per LSB                                    */
} __attribute__ ((packed)) USB_ConfigurationDescriptor_TypeDef;
EFM32_PACK_END()


/** @brief USB Interface Descriptor. */
EFM32_PACK_START( 1 )
typedef struct
{
  uint8_t bLength;                              /**< Size of this descriptor in bytes.                 */
  uint8_t bDescriptorType;                      /**< Constant INTERFACE Descriptor Type.               */
  uint8_t bInterfaceNumber;                     /**< Number of this interface. Zero-based value
                                                     identifying the index in the array of concurrent
                                                     interfaces supported by this configuration.       */
  uint8_t bAlternateSetting;                    /**< Value used to select this alternate setting for
                                                     the interface identified in the prior field.      */
  uint8_t bNumEndpoints;                        /**< Number of endpoints used by this interface
                                                     (excluding endpoint zero). If this value is zero,
                                                     this interface only uses the Default Control Pipe.*/
  uint8_t bInterfaceClass;                      /**< Class code (assigned by the USB-IF). A value
                                                     of zero is reserved for future standardization. If
                                                     this field is set to FFH, the interface class is
                                                     vendor-specific. All other values are reserved for
                                                     assignment by the USB-IF.                         */
  uint8_t bInterfaceSubClass;                   /**< Subclass code (assigned by the USB-IF). These codes
                                                     are qualified by the value of the bInterfaceClass
                                                     field. If the bInterfaceClass field is reset to
                                                     zero, this field must also be reset to zero. If
                                                     the bInterfaceClass field is not set to FFH, all
                                                     values are reserved forassignment by the USB-IF.  */
  uint8_t bInterfaceProtocol;                   /**< Protocol code (assigned by the USB). These codes
                                                     are qualified by the value of the bInterfaceClass
                                                     and the bInterfaceSubClass fields. If an interface
                                                     supports class-specific requests, this code
                                                     identifies the protocols that the device uses as
                                                     defined by the specification of the device class.
                                                     If this field is reset to zero, the device does
                                                     not use a class-specific protocol on this
                                                     interface. If this field is set to FFH, the device
                                                     uses a vendor-specific protocol for this interface*/
  uint8_t iInterface;                           /**< Index of string descriptor describing this
                                                     interface.                                        */
} __attribute__ ((packed)) USB_InterfaceDescriptor_TypeDef;
EFM32_PACK_END()


/** @brief USB Endpoint Descriptor. */
EFM32_PACK_START( 1 )
typedef struct
{
  uint8_t   bLength;                            /**< Size of this descriptor in bytes                  */
  uint8_t   bDescriptorType;                    /**< Constant ENDPOINT Descriptor Type                 */
  uint8_t   bEndpointAddress;                   /**< The address of the endpoint                       */
  uint8_t   bmAttributes;                       /**< This field describes the endpoint attributes      */
  uint16_t  wMaxPacketSize;                     /**< Maximum packet size for the endpoint              */
  uint8_t   bInterval;                          /**< Interval for polling EP for data transfers        */
} __attribute__ ((packed)) USB_EndpointDescriptor_TypeDef;
EFM32_PACK_END()


/** @brief USB String Descriptor. */
EFM32_PACK_START( 1 )
typedef struct
{
  uint8_t len;                                  /**< Size of this descriptor in bytes.                 */
  uint8_t type;                                 /**< Constant STRING Descriptor Type.                  */
  char16_t name[];                              /**< The string encoded with UTF-16LE UNICODE charset. */
} __attribute__ ((packed)) USB_StringDescriptor_TypeDef;
EFM32_PACK_END()

/** @} (end addtogroup USB_COMMON) */

/*** -------------------- Serial port debug configuration ---------------- ***/

#if defined( DOXY_DOC_ONLY )
/** @addtogroup USB_COMMON
 *  @{*/

/***************************************************************************//**
 * @brief
 *   Transmit a single char on the debug serial port.
 *
 * @note
 *   This function is enabled with \#define DEBUG_USB_API when configuring the
 *   protocol stack in "usbconfig.h".
 *   This is convenient when debugging code, no need to remove use of this
 *   function when debugging has completed.
 *
 * @param[in] c
 *   Char to transmit.
 *
 * @return
 *   The char transmitted.
 ******************************************************************************/
int  USB_PUTCHAR( char c );

/***************************************************************************//**
 * @brief
 *   Transmit a zero terminated string on the debug serial port.
 *
 * @note
 *   This function is enabled with \#define DEBUG_USB_API when configuring the
 *   protocol stack in "usbconfig.h".
 *   This is convenient when debugging code, no need to remove use of this
 *   function when debugging has completed.
 *
 * @param[in] p
 *   Pointer to string to transmit.
 ******************************************************************************/
void USB_PUTS( const char *p );

/***************************************************************************//**
 * @brief
 *   Transmit "printf" formated data on the debug serial port.
 *
 * @note
 *   This function is enabled with \#define USB_USE_PRINTF when configuring the
 *   protocol stack in "usbconfig.h".
 *   This is convenient when debugging code, no need to remove use of this
 *   function when debugging has completed.
 *
 * @param[in] format
 *   Format string (as in printf). No floating point format support.
 ******************************************************************************/
int  USB_PRINTF( const char *format, ... );

/** @} (end addtogroup USB_COMMON) */
#endif /* defined( DOXY_DOC_ONLY ) */

/** @cond DO_NOT_INCLUDE_WITH_DOXYGEN */

/* Hardware constraint, do not change. */
#define MAX_NUM_HOSTCHANNELS  14

/* The DMA engine use one FIFO ram word for each host channel. */
#define MAX_HOST_FIFO_SIZE_INWORDS (512-MAX_NUM_HOSTCHANNELS)/*Unit is 4 bytes*/

#if defined ( USER_PUTCHAR )
  void USB_Puts( const char *p );
  #define USB_PUTS( s )            USB_Puts( s )
  #define USB_PUTCHAR( c )         USER_PUTCHAR( c )
#else
  #define USB_PUTS( s )
  #define USB_PUTCHAR( c )
#endif

#if defined( USB_USE_PRINTF )
  /* Use a printf which don't support floating point formatting */
  #if defined(__ICCARM__) || defined (__CC_ARM) || defined (__CROSSWORKS_ARM)
    #define USB_PRINTF                  printf
  #else
    #define USB_PRINTF                  iprintf
  #endif
#else
  #define USB_PRINTF(...)
#endif /* defined( USB_USE_PRINTF ) */

#if defined( DEBUG_USB_API )
  #define DEBUG_USB_API_PUTS( s )       USB_PUTS( s )
  #define DEBUG_USB_API_PUTCHAR( c )    USB_PUTCHAR( c )
#else
  #define DEBUG_USB_API_PUTS( s )
  #define DEBUG_USB_API_PUTCHAR( c )
#endif /* defined( DEBUG_USB_API ) */

/** @endcond */

/*** -------------------- Common API definitions ------------------------- ***/

/** @cond DO_NOT_INCLUDE_WITH_DOXYGEN */

#if defined( USB_HOST )
  #if defined( NUM_APP_TIMERS )
    #define NUM_QTIMERS ( NUM_HC_USED + 2 + NUM_APP_TIMERS + 1 )
  #else
    #define NUM_QTIMERS ( NUM_HC_USED + 2 + 1 )
  #endif
  /* + 2 for default ctrl. host ch. 0 & 1, + 1 for host port timer  */
#else
  #if defined( NUM_APP_TIMERS )
    #define NUM_QTIMERS ( NUM_APP_TIMERS )
  #else
    #define NUM_QTIMERS 0
  #endif
#endif /* defined( USB_HOST ) */
/** @endcond */

/** @addtogroup USB_COMMON
 *  @{*/

/***************************************************************************//**
 * @brief
 *  USB transfer callback function.
 *
 * @details
 *  The callback function is called when a transfer has completed. An application
 *  should check the status, xferred and optionally the remaining parameters
 *  before deciding if the transfer is usable. In the case where the transfer
 *  is part of a control request data stage, the callback function should
 *  return an appropriate @ref USB_Status_TypeDef status.
 *
 * @param[in] status
 *   The transfer status. See @ref USB_Status_TypeDef.
 *
 * @param[in] xferred
 *   Number of bytes actually transferred.
 *
 * @param[in] remaining
 *   Number of bytes not transferred.
 *
 * @return
 *   @ref USB_STATUS_OK on success, else an appropriate error code.
 ******************************************************************************/
typedef int  (*USB_XferCompleteCb_TypeDef)( USB_Status_TypeDef status, uint32_t xferred, uint32_t remaining );

/***************************************************************************//**
 * @brief
 *  USBTIMER callback function.
 *
 * @details
 *  The callback function is called when an USBTIMER has expired. The callback
 *  is done with interrupts disabled.
 ******************************************************************************/
typedef void (*USBTIMER_Callback_TypeDef)(  void );

char *USB_GetErrorMsgString(            int error );

#if defined( USB_USE_PRINTF )
  void USB_PrintErrorMsgString(         char *pre, int error );
#else
  #define USB_PrintErrorMsgString(      pre, error )
#endif

void  USBTIMER_DelayMs(                 uint32_t msec );
void  USBTIMER_DelayUs(                 uint32_t usec );
void  USBTIMER_Init(                    void );

#if ( NUM_QTIMERS > 0 )
  void  USBTIMER_Start(                 uint32_t id, uint32_t timeout, USBTIMER_Callback_TypeDef callback );
  void  USBTIMER_Stop(                  uint32_t id );
#endif /* ( NUM_QTIMERS > 0 ) */
/** @} (end addtogroup USB_COMMON) */

#if defined( USB_DEVICE )
/** @addtogroup USB_DEVICE
 *  @{*/
/*** -------------------- DEVICE mode API definitions -------------------- ***/

/***************************************************************************//**
 * @brief
 *  USB Reset callback function.
 * @details
 *  Called whenever USB reset signalling is detected on the USB port.
 ******************************************************************************/
typedef void (*USBD_UsbResetCb_TypeDef)( void );

/***************************************************************************//**
 * @brief
 *  USB Start Of Frame (SOF) interrupt callback function.
 *
 * @details
 *  Called at each SOF interrupt (if enabled),
 *
 * @param[in] sofNr
 *   Current frame number. The value rolls over to 0 after 16383 (0x3FFF).
 ******************************************************************************/
typedef void (*USBD_SofIntCb_TypeDef)( uint16_t sofNr );

/***************************************************************************//**
 * @brief
 *  USB State change callback function.
 *
 * @details
 *  Called whenever the device change state.
 *
 * @param[in] oldState
 *   The device USB state just leaved. See @ref USBD_State_TypeDef.
 *
 * @param[in] newState
 *   New (the current) USB device state. See @ref USBD_State_TypeDef.
 ******************************************************************************/
typedef void (*USBD_DeviceStateChangeCb_TypeDef)( USBD_State_TypeDef oldState, USBD_State_TypeDef newState );

/***************************************************************************//**
 * @brief
 *  USB power mode callback function.
 *
 * @details
 *  Called whenever the device stack needs to query if the device is currently
 *  self- or bus-powered. Typically when host has issued an @ref GET_STATUS
 *  setup command.
 *
 * @return
 *  True if self-powered, false otherwise.
 ******************************************************************************/
typedef bool (*USBD_IsSelfPoweredCb_TypeDef)( void );

/***************************************************************************//**
 * @brief
 *  USB setup request callback function.
 *
 * @details
 *  Called on each setup request received from host. This gives the application a
 *  possibility to extend or override standard requests, and to implement class
 *  or vendor specific requests. Return @ref USB_STATUS_OK if the request is
 *  handled, return @ref USB_STATUS_REQ_ERR if it is an illegal request or
 *  return @ref USB_STATUS_REQ_UNHANDLED to pass the request on to the default
 *  request handler.
 *
 * @param[in] setup
 *  Pointer to an USB setup packet. See @ref USB_Setup_TypeDef.
 *
 * @return
 *  An appropriate status/error code. See @ref USB_Status_TypeDef.
 ******************************************************************************/
typedef int  (*USBD_SetupCmdCb_TypeDef)( const USB_Setup_TypeDef *setup );

/** @cond DO_NOT_INCLUDE_WITH_DOXYGEN */
struct USBD_Callbacks_TypeDef;
typedef struct USBD_Callbacks_TypeDef const *USBD_Callbacks_TypeDef_Pointer;
/** @endcond */


/** @brief USB Device stack initialization structure.
 *  @details This structure is passed to @ref USBD_Init() when starting up
 *  the device.                                                             */
typedef struct
{
  const USB_DeviceDescriptor_TypeDef      *deviceDescriptor;      /**< Pointer to a device descriptor.                */
  const uint8_t                           *configDescriptor;      /**< Pointer to a configuration descriptor.         */
  const void * const                      *stringDescriptors;     /**< Pointer to an array of string descriptor pointers.*/
  const uint8_t                           numberOfStrings;        /**< Number of strings in string descriptor array.  */
  const uint8_t                           *bufferingMultiplier;   /**< Pointer to an array defining the size of the
                                                                       endpoint buffers. The size is given in
                                                                       multiples of endpoint size. Generally a value
                                                                       of 1 (single) or 2 (double) buffering should be
                                                                       used.                                          */
  USBD_Callbacks_TypeDef_Pointer          callbacks;              /**< Pointer to struct with callbacks
                                                                       (@ref USBD_Callbacks_TypeDef). These callbacks
                                                                       are used by the device stack to signal events
                                                                       to or query the application.                   */
  const uint32_t                          reserved;               /**< Reserved for future use.                       */
} USBD_Init_TypeDef;


/** @brief USB Device stack callback structure.
 *  @details Callback functions used by the device stack to signal events or
 *  query status to/from the application. See @ref USBD_Init_TypeDef. Assign
 *  members to NULL if your application don't need a specific callback. */
typedef struct USBD_Callbacks_TypeDef
{
  const USBD_UsbResetCb_TypeDef           usbReset;               /**< Called whenever USB reset signalling is detected
                                                                       on the USB port.                                */
  const USBD_DeviceStateChangeCb_TypeDef  usbStateChange;         /**< Called whenever the device change state.        */
  const USBD_SetupCmdCb_TypeDef           setupCmd;               /**< Called on each setup request received from host.*/
  const USBD_IsSelfPoweredCb_TypeDef      isSelfPowered;          /**< Called whenever the device stack needs to query
                                                                       if the device is currently self- or bus-powered.
                                                                       Applies to devices which can operate in both modes.*/
  const USBD_SofIntCb_TypeDef             sofInt;                 /**< Called at each SOF interrupt. If NULL, the device
                                                                       stack will not enable the SOF interrupt.        */
} USBD_Callbacks_TypeDef;


/*** -------------------- DEVICE mode API -------------------------------- ***/

void                USBD_AbortAllTransfers( void );
int                 USBD_AbortTransfer(     int epAddr );
void                USBD_Connect(           void );
void                USBD_Disconnect(        void );
bool                USBD_EpIsBusy(          int epAddr );
USBD_State_TypeDef  USBD_GetUsbState(       void );
const char *        USBD_GetUsbStateName(   USBD_State_TypeDef state );
int                 USBD_Init(              const USBD_Init_TypeDef *p );
int                 USBD_Read(              int epAddr, void *data, int byteCount, USB_XferCompleteCb_TypeDef callback );
int                 USBD_RemoteWakeup(      void );
bool                USBD_SafeToEnterEM2(    void );
int                 USBD_StallEp(           int epAddr );
void                USBD_Stop(              void );
int                 USBD_UnStallEp(         int epAddr );
int                 USBD_Write(             int epAddr, void *data, int byteCount, USB_XferCompleteCb_TypeDef callback );

#ifdef __MBED__
int                 USBD_SetAddress(        uint8_t addr );
int                 USBD_AddEndpoint(       int epAddr, int transferType, int maxPacketSize, int bufferMult );
int                 USBD_EpIsStalled(       int epAddr );
void                USBD_StallEp0(          void );
#endif

/** @} (end addtogroup USB_DEVICE) */
#endif /* defined( USB_DEVICE ) */


#if defined( USB_HOST )
/***************************************************************************//**
 * @addtogroup USB_HOST
 * @brief USB HOST protocol stack, see @ref usb_host page for detailed documentation.
 * @{
 ******************************************************************************/
/*** -------------------- HOST mode API definitions ---------------------- ***/

#define USB_VBUSOVRCUR_PORT_NONE     -1       /**< No overcurrent flag functionality.                   */
#define USB_VBUSOVRCUR_POLARITY_LOW  0        /**< Overcurrent flag pin polarity is low.                */
#define USB_VBUSOVRCUR_POLARITY_HIGH 1        /**< Overcurrent flag pin polarity is high.               */

/** USB HOST endpoint status enumerator. */
typedef enum
{
  H_EP_IDLE             = 0,                      /**< The endpoint is idle.                            */
  H_EP_SETUP            = 1,                      /**< The endpoint is in SETUP stage.                  */
  H_EP_DATA_IN          = 2,                      /**< The endpoint is in DATA IN stage.                */
  H_EP_DATA_OUT         = 3,                      /**< The endpoint is in DATA OUT stage.               */
  H_EP_STATUS_IN        = 4,                      /**< The endpoint is in STATUS IN stage.              */
  H_EP_STATUS_OUT       = 5,                      /**< The endpoint is in STATUS OUT stage.             */
} USBH_EpState_TypeDef;


/** @brief USB HOST endpoint status data.
 *  @details A host application should not manipulate the contents of
 * this struct.                                                             */
typedef struct
{
  USB_Setup_TypeDef               setup;          /**< A SETUP package.                                 */
  uint8_t                         setupErrCnt;    /**< Error counter for SETUP transfers.               */
  USB_EndpointDescriptor_TypeDef  epDesc;         /**< Endpoint descriptor.                             */
  struct USBH_Device_TypeDef      *parentDevice;  /**< The device the endpoint belongs to.              */
  uint8_t                         type;           /**< Endpoint type.                                   */
  uint16_t                        packetSize;     /**< Packet size, current transfer.                   */
  uint8_t                         hcOut;          /**< Host channel number assigned for OUT transfers.  */
  uint8_t                         hcIn;           /**< Host channel number assigned for IN transfers.   */
  bool                            in;             /**< Endpoint direction.                              */
  uint8_t                         toggle;         /**< Endpoint data toggle.                            */
  USBH_EpState_TypeDef            state;          /**< Endpoint state.                                  */
  uint8_t                         addr;           /**< Endpoint address.                                */
  uint8_t                         *buf;           /**< Transfer buffer.                                 */
  volatile bool                   xferCompleted;  /**< Transfer completion flag.                        */
  USB_Status_TypeDef              xferStatus;     /**< Transfer status.                                 */
  USB_XferCompleteCb_TypeDef      xferCompleteCb; /**< Transfer completion callback function.           */
  uint32_t                        xferred;        /**< Number of bytes transferred.                     */
  uint32_t                        remaining;      /**< Number of bytes remaining.                       */
  uint32_t                        timeout;        /**< Transfer timeout.                                */
} USBH_Ep_TypeDef;


/** @brief USB HOST device definition.
 *  @details A host application should not manipulate the contents of
 *  this struct.                                                            */
typedef struct USBH_Device_TypeDef
{
  USB_DeviceDescriptor_TypeDef          devDesc;  /**< The device device descriptor.                    */
  USB_ConfigurationDescriptor_TypeDef   confDesc; /**< The device configuration descriptor.             */
  USB_InterfaceDescriptor_TypeDef       itfDesc;  /**< The device interface descriptor.                 */
  USBH_Ep_TypeDef                       ep0;      /**< Endpoint 0 status data.                          */
  USBH_Ep_TypeDef                       *ep;      /**< Array of endpoint status data.                   */
  int                                   numEp;    /**< Number of endpoints.                             */
  uint8_t                               addr;     /**< The device address.                              */
  uint8_t                               speed;    /**< The device speed (low or full speed).            */
} USBH_Device_TypeDef;


/** @brief USB Host stack initialization structure.
 *  @details This structure is passed to @ref USBH_Init() when starting up the
 *  device. Max accumulated FIFO size is 2K bytes.                          */
typedef struct
{
  uint32_t                  rxFifoSize;     /**< Number of FIFO bytes set aside for IN endpoints.            */
  uint32_t                  nptxFifoSize;   /**< Number of FIFO bytes set aside for OUT CTRL/BULK endoints.  */
  uint32_t                  ptxFifoSize;    /**< Number of FIFO bytes set aside for OUT INTR/ISO endoints.   */
  uint32_t                  reserved;       /**< Reserved for future use.                                    */
} USBH_Init_TypeDef;


/** Default @ref USBH_Init_TypeDef values, provides reasonable Tx/Rx FIFO
 *  partitioning.                                                         */
/* In DMA mode the total available FIFO space is smaller.       */
/* The DMA controller use one FIFO word pr. channel for status. */
/* The unit in the table is byte.                               */
#define USBH_INIT_DEFAULT                                                    \
{                                                                            \
  MAX_HOST_FIFO_SIZE_INWORDS * 2,/* 1024 bytes Rx FIFO size.             */  \
  MAX_HOST_FIFO_SIZE_INWORDS,    /* 512 bytes non-periodic Tx FIFO size. */  \
  MAX_HOST_FIFO_SIZE_INWORDS,    /* 512 bytes periodic Tx FIFO size.     */  \
  0                              /* Reserved.                            */  \
}

/*** -------------------- HOST mode API ---------------------------------- ***/

int     USBH_AssignHostChannel(            USBH_Ep_TypeDef *ep, uint8_t hcnum );
int     USBH_ControlMsg(                   USBH_Ep_TypeDef *ep, uint8_t bmRequestType, uint8_t bRequest, uint16_t wValue, uint16_t wIndex, uint16_t wLength, void *data, int timeout, USB_XferCompleteCb_TypeDef callback );
int     USBH_ControlMsgB(                  USBH_Ep_TypeDef *ep, uint8_t bmRequestType, uint8_t bRequest, uint16_t wValue, uint16_t wIndex, uint16_t wLength, void *data, int timeout );
bool    USBH_DeviceConnected(              void );
int     USBH_GetConfigurationDescriptorB(  USBH_Device_TypeDef *device, void *buf, int len, uint8_t configIndex );
int     USBH_GetDeviceDescriptorB(         USBH_Device_TypeDef *device, void *buf, int len );
uint8_t USBH_GetPortSpeed(                 void );
int     USBH_GetStringB(                   USBH_Device_TypeDef *device, uint8_t *buf, int bufLen, uint8_t stringIndex, uint16_t langID );
int     USBH_Init(                         const USBH_Init_TypeDef *p );
int     USBH_InitDeviceData(               USBH_Device_TypeDef *device, const uint8_t *buf, USBH_Ep_TypeDef *ep, int numEp, uint8_t deviceSpeed );
int     USBH_PortReset(                    void );
int     USBH_PortResume(                   void );
void    USBH_PortSuspend(                  void );
void    USBH_PrintString(                  const char *pre, const USB_StringDescriptor_TypeDef *s, const char *post );

#if defined( USB_USE_PRINTF )
int     USBH_PrintConfigurationDescriptor( const USB_ConfigurationDescriptor_TypeDef *config, int maxLen );
int     USBH_PrintDeviceDescriptor(        const USB_DeviceDescriptor_TypeDef *device );
int     USBH_PrintEndpointDescriptor(      const USB_EndpointDescriptor_TypeDef *endpoint );
int     USBH_PrintInterfaceDescriptor(     const USB_InterfaceDescriptor_TypeDef *interface );
#else
#define USBH_PrintConfigurationDescriptor( config, maxLen )
#define USBH_PrintDeviceDescriptor(        device )
#define USBH_PrintEndpointDescriptor(      endpoint )
#define USBH_PrintInterfaceDescriptor(     interface )
#endif /* defined( USB_USE_PRINTF ) */

int                                  USBH_QueryDeviceB(                uint8_t *buf, size_t bufsize, uint8_t deviceSpeed );
USB_ConfigurationDescriptor_TypeDef* USBH_QGetConfigurationDescriptor( const uint8_t *buf, int configIndex );
USB_DeviceDescriptor_TypeDef*        USBH_QGetDeviceDescriptor(        const uint8_t *buf );
USB_EndpointDescriptor_TypeDef*      USBH_QGetEndpointDescriptor(      const uint8_t *buf, int configIndex, int interfaceIndex, int endpointIndex );
USB_InterfaceDescriptor_TypeDef*     USBH_QGetInterfaceDescriptor(     const uint8_t *buf, int configIndex, int interfaceIndex );

int     USBH_Read(                         USBH_Ep_TypeDef *ep, void *data, int byteCount, int timeout, USB_XferCompleteCb_TypeDef callback );
int     USBH_ReadB(                        USBH_Ep_TypeDef *ep, void *data, int byteCount, int timeout );
int     USBH_SetAddressB(                  USBH_Device_TypeDef *device, uint8_t deviceAddress );
int     USBH_SetAltInterfaceB(             USBH_Device_TypeDef *device, uint8_t interfaceIndex, uint8_t alternateSetting );
int     USBH_SetConfigurationB(            USBH_Device_TypeDef *device, uint8_t configValue );
int     USBH_StallEpB(                     USBH_Ep_TypeDef *ep );
void    USBH_Stop(                         void );
int     USBH_UnStallEpB(                   USBH_Ep_TypeDef *ep );
int     USBH_WaitForDeviceConnectionB(     uint8_t *buf, int timeoutInSeconds );
int     USBH_Write(                        USBH_Ep_TypeDef *ep, void *data, int byteCount, int timeout, USB_XferCompleteCb_TypeDef callback );
int     USBH_WriteB(                       USBH_Ep_TypeDef *ep, void *data, int byteCount, int timeout );

/** @} (end addtogroup USB_HOST) */
#endif /* defined( USB_HOST ) */
/** @} (end addtogroup USB) */

#ifdef __cplusplus
}
#endif

#endif /* defined( USB_DEVICE ) || defined( USB_HOST ) */
#endif /* defined( USB_PRESENT ) && ( USB_COUNT == 1 ) */
#endif /* __EM_USB_H */
