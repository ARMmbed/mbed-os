
#if (MBED_CONF_TARGET_USB_DEVICE_HSUSBD == 0)
#define NU_MAX_EPX_BUFSIZE      4096
#else
#define NU_MAX_EPX_BUFSIZE      4096
#endif

#define NU_EP2EPL(ep)           ((ep) >> 1)

#if (MBED_CONF_TARGET_USB_DEVICE_HSUSBD == 0)
#define NU_EP2EPH(ep)           (((ep) >> 1) + 1)
#define NU_EPL2EPH(ep)					((ep) + 1)
#define NU_EPH2EPL(ep)					((ep) - 1)
#define NUMBER_OF_PHYSICAL_ENDPOINTS    8
#else
#define NU_EP2EPH(ep)           (((ep) >> 1) - 1)
#define NU_EPX2EP(ep)          ((ep == CEP) ? EP0OUT : ((ep) - EPA + EP1OUT))
#define NU_EPL2EPH(ep)					((ep) - 1)
#define NU_EPH2EPL(ep)					((ep) + 1)
#define NUMBER_OF_PHYSICAL_ENDPOINTS    12
#endif

#define NU_EP_DIR_Pos           0
#define NU_EP_DIR_Msk           (1 << NU_EP_DIR_Pos)
#define NU_EP_DIR_OUT           0
#define NU_EP_DIR_IN            1

#define NU_EP_TYPE(ep)          (((ep) & NU_EP_TYPE_Msk) >> NU_EP_TYPE_Pos)
#define NU_EP_NUM(ep)           (((ep) & NU_EP_NUM_Msk) >> NU_EP_NUM_Pos)
#define NU_EP_DIR(ep)           (((ep) & NU_EP_DIR_Msk) >> NU_EP_DIR_Pos)
#define NU_EP_NUM_DIR(ep)       ((NU_EP_NUM(ep) << 1) | NU_EP_DIR(ep))

#define EP0OUT      (0)
#define EP0IN       (1)
#define EP1OUT      (2)
#define EP1IN       (3)
#define EP2OUT      (4)
#define EP2IN       (5)
#define EP3OUT      (6)
#define EP3IN       (7)
#define EP4OUT     	(8)
#define EP4IN      	(9)
#define EP5OUT     (10)
#define EP5IN      (11)
#define EP6OUT     (12)
#define EP6IN      (13)

/* Maximum Packet sizes */
#define MAX_PACKET_SIZE_EP0   64
#define MAX_PACKET_SIZE_EP1   64
#define MAX_PACKET_SIZE_EP2   64
#define MAX_PACKET_SIZE_EP3   0x60
#define MAX_PACKET_SIZE_EP4  	64
#define MAX_PACKET_SIZE_EP5  	64
#define MAX_PACKET_SIZE_EP6  	64
#define MAX_PACKET_SIZE_EP7  	64

#if (MBED_CONF_TARGET_USB_DEVICE_HSUSBD == 1)
#define MAX_PACKET_SIZE_EP8  	64
#define MAX_PACKET_SIZE_EP9  	64
#define MAX_PACKET_SIZE_EP10  64
#define MAX_PACKET_SIZE_EP11  64
#endif

/* Generic endpoints - intended to be portable accross devices */
/* and be suitable for simple USB devices. */

/* Bulk endpoints */
#define EPBULK_OUT            EP5OUT
#define EPBULK_IN             EP6IN
#define EPBULK_OUT_callback   EP5_OUT_callback
#define EPBULK_IN_callback    EP6_IN_callback
/* Interrupt endpoints */
#define EPINT_OUT             EP1OUT
#define EPINT_IN              EP2IN
#define EPINT_OUT_callback    EP1_OUT_callback
#define EPINT_IN_callback     EP2_IN_callback
/* Isochronous endpoints */
#define EPISO_OUT             EP3OUT
#define EPISO_IN              EP4IN
#define EPISO_OUT_callback    EP3_OUT_callback
#define EPISO_IN_callback     EP4_IN_callback

#define MAX_PACKET_SIZE_EPBULK  64
#define MAX_PACKET_SIZE_EPINT   64
#define MAX_PACKET_SIZE_EPISO   1023

#if (MBED_CONF_TARGET_USB_DEVICE_HSUSBD == 1)
#define HSUSBD_GET_EP_MAX_PAYLOAD(ep)		HSUSBD->EP[ep].EPMPS
#define HSUSBD_GET_EP_DATA_COUNT(ep)		(HSUSBD->EP[ep].EPDATCNT & 0xFFFFF)
#define HSUSBD_SET_EP_SHORT_PACKET(ep) 		HSUSBD->EP[ep].EPRSPCTL = ((HSUSBD->EP[ep].EPRSPCTL & 0x10) | 0x40)
#define HSUSBD_GET_EP_INT_EN(ep)			HSUSBD->EP[ep].EPINTEN
#endif
