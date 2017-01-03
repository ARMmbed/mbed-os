/* mbed Microcontroller Library
 * Copyright (c) 2015-2016 Nuvoton
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
 */
#define NU_MAX_EPX_BUFSIZE      4096
#define NU_EP2EPL(ep)           ((ep) >> 1)
#define NU_EP2EPH(ep)           (((ep) >> 1) - 1)
#define NU_EPX2EP(ep)           ((ep == CEP) ? EP0OUT : ((ep) - EPA + EP1OUT))
#define NU_EPL2EPH(ep)          ((ep) - 1)
#define NU_EPH2EPL(ep)          ((ep) + 1)
#define NU_EP_DIR_Pos           0
#define NU_EP_DIR_Msk           (1 << NU_EP_DIR_Pos)
#define NU_EP_DIR_OUT           0
#define NU_EP_DIR_IN            1

#define NU_EP_TYPE(ep)          (((ep) & NU_EP_TYPE_Msk) >> NU_EP_TYPE_Pos)
#define NU_EP_NUM(ep)           (((ep) & NU_EP_NUM_Msk) >> NU_EP_NUM_Pos)
#define NU_EP_DIR(ep)           (((ep) & NU_EP_DIR_Msk) >> NU_EP_DIR_Pos)
#define NU_EP_NUM_DIR(ep)       ((NU_EP_NUM(ep) << 1) | NU_EP_DIR(ep))

#define NUMBER_OF_PHYSICAL_ENDPOINTS    12

#define EP0OUT      (0)
#define EP0IN       (1)
#define EP1OUT      (2)
#define EP1IN       (3)
#define EP2OUT      (4)
#define EP2IN       (5)
#define EP3OUT      (6)
#define EP3IN       (7)
#define EP4OUT      (8)
#define EP4IN       (9)
#define EP5OUT     (10)
#define EP5IN      (11)
#define EP6OUT     (12)
#define EP6IN      (13)

/* Maximum Packet sizes */
#define MAX_PACKET_SIZE_EP0     64
#define MAX_PACKET_SIZE_EP1     64
#define MAX_PACKET_SIZE_EP2     64
#define MAX_PACKET_SIZE_EP3     0x60
#define MAX_PACKET_SIZE_EP4     64
#define MAX_PACKET_SIZE_EP5     64
#define MAX_PACKET_SIZE_EP6     64
#define MAX_PACKET_SIZE_EP7     64
#define MAX_PACKET_SIZE_EP8     64
#define MAX_PACKET_SIZE_EP9     64
#define MAX_PACKET_SIZE_EP10    64
#define MAX_PACKET_SIZE_EP11    64

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

#define USBD_GET_EP_MAX_PAYLOAD(ep)     (*((__IO uint32_t *) ((uint32_t)&USBD->EPAMPS + (uint32_t)((ep)*0x28)))) 
#define USBD_GET_EP_DATA_COUNT(ep)      ((*((__IO uint32_t *) ((uint32_t)&USBD->EPADATCNT + (uint32_t)((ep)*0x28)))) & 0xFFFFF)
#define USBD_SET_EP_SHORT_PACKET(ep)    (*((__IO uint32_t *) ((uint32_t)&USBD->EPARSPCTL + (uint32_t)((ep)*0x28))) = (*((__IO uint32_t *) ((uint32_t)&USBD->EPARSPCTL + (uint32_t)((ep)*0x28)))) & 0x10 | 0x40)
#define USBD_GET_EP_INT_EN(ep)          (*((__IO uint32_t *) ((uint32_t)&USBD->EPAINTEN + (uint32_t)((ep)*0x28))))
