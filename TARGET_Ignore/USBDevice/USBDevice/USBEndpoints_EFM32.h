
#ifndef TARGET_EFM32HG_STK3400
# define NUMBER_OF_LOGICAL_ENDPOINTS   (6)
#else
# define NUMBER_OF_LOGICAL_ENDPOINTS   (3)
#endif

#define NUMBER_OF_PHYSICAL_ENDPOINTS  (NUMBER_OF_LOGICAL_ENDPOINTS * 2)
#define NUMBER_OF_ENDPOINTS           (NUMBER_OF_PHYSICAL_ENDPOINTS + 2)  /* Includes EP0 */

#define EP0OUT      (0)
#define EP0IN       (1)
#define EP1OUT      (2)
#define EP1IN       (3)
#define EP2OUT      (4)
#define EP2IN       (5)
#define EP3OUT      (6)
#define EP3IN       (7)
#ifndef TARGET_EFM32HG_STK3400
# define EP4OUT     (8)
# define EP4IN      (9)
# define EP5OUT     (10)
# define EP5IN      (11)
# define EP6OUT     (12)
# define EP6IN      (13)
#endif

#define USB_EP_TO_INDEX(ep) (ep)
#define USB_EP_TO_ADDR(ep)  (((ep)>>1) | (((ep) & 1   ) ? 0x80 : 0x00))
#define USB_ADDR_TO_EP(ep)  (((ep)<<1) | (((ep) & 0x80) ? 0x01 : 0x00))

/* Maximum Packet sizes */

#define MAX_PACKET_SIZE_EP0   64
#define MAX_PACKET_SIZE_EP1   64
#define MAX_PACKET_SIZE_EP2   64
#define MAX_PACKET_SIZE_EP3   64
#ifndef TARGET_EFM32HG_STK3400
# define MAX_PACKET_SIZE_EP4  64
# define MAX_PACKET_SIZE_EP5  64
# define MAX_PACKET_SIZE_EP6  64
#endif

/* Generic endpoints - intended to be portable accross devices */
/* and be suitable for simple USB devices. */

/* Bulk endpoints */
#define EPBULK_OUT            EP2OUT
#define EPBULK_IN             EP2IN
#define EPBULK_OUT_callback   EP2_OUT_callback
#define EPBULK_IN_callback    EP2_IN_callback
/* Interrupt endpoints */
#define EPINT_OUT             EP1OUT
#define EPINT_IN              EP1IN
#define EPINT_OUT_callback    EP1_OUT_callback
#define EPINT_IN_callback     EP1_IN_callback
/* Isochronous endpoints */
#define EPISO_OUT             EP3OUT
#define EPISO_IN              EP3IN
#define EPISO_OUT_callback    EP3_OUT_callback
#define EPISO_IN_callback     EP3_IN_callback

#define MAX_PACKET_SIZE_EPBULK  64
#define MAX_PACKET_SIZE_EPINT   64
#define MAX_PACKET_SIZE_EPISO   1023
