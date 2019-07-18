/* mbed Microcontroller Library
 * Copyright (c) 2018-2018 ARM Limited
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

#define NUMBER_OF_LOGICAL_ENDPOINTS (16)
#define NUMBER_OF_PHYSICAL_ENDPOINTS (NUMBER_OF_LOGICAL_ENDPOINTS * 2)

/* Define physical endpoint numbers */

/*      Endpoint    No.     Type(s)       MaxPacket   DoubleBuffer  */
/*      ----------------    ------------  ----------  ---           */
#define EP0OUT      (0x00)  /* Control       64          No            */
#define EP0IN       (0x80)  /* Control       64          No            */
#define EP1OUT      (0x01)  /* Interrupt     64          No            */
#define EP1IN       (0x81)  /* Interrupt     64          No            */
#define EP2OUT      (0x02)  /* Bulk          64          Yes           */
#define EP2IN       (0x82)  /* Bulk          64          Yes           */
#define EP3OUT      (0x03)  /* Isochronous   1023        Yes           */
#define EP3IN       (0x83)  /* Isochronous   1023        Yes           */
#define EP4OUT      (0x04)  /* Interrupt     64          No            */
#define EP4IN       (0x84)  /* Interrupt     64          No            */
#define EP5OUT      (0x05)  /* Bulk          64          Yes           */
#define EP5IN       (0x85)  /* Bulk          64          Yes           */
#define EP6OUT      (0x06)  /* Isochronous   1023        Yes           */
#define EP6IN       (0x86)  /* Isochronous   1023        Yes           */
#define EP7OUT      (0x07)  /* Interrupt     64          No            */
#define EP7IN       (0x87)  /* Interrupt     64          No            */
#define EP8OUT      (0x08)  /* Bulk          64          Yes           */
#define EP8IN       (0x88)  /* Bulk          64          Yes           */
#define EP9OUT      (0x09)  /* Isochronous   1023        Yes           */
#define EP9IN       (0x89)  /* Isochronous   1023        Yes           */
#define EP10OUT     (0x0A)  /* Interrupt     64          No            */
#define EP10IN      (0x8A)  /* Interrupt     64          No            */
#define EP11OUT     (0x0B)  /* Bulk          64          Yes           */
#define EP11IN      (0x8B)  /* Bulk          64          Yes           */
#define EP12OUT     (0x0C)  /* Isochronous   1023        Yes           */
#define EP12IN      (0x8C)  /* Isochronous   1023        Yes           */
#define EP13OUT     (0x0D)  /* Interrupt     64          No            */
#define EP13IN      (0x8D)  /* Interrupt     64          No            */
#define EP14OUT     (0x0E)  /* Bulk          64          Yes           */
#define EP14IN      (0x8E)  /* Bulk          64          Yes           */
#define EP15OUT     (0x0F)  /* Bulk          64          Yes           */
#define EP15IN      (0x8F)  /* Bulk          64          Yes           */

/* Maximum Packet sizes */

#define MAX_PACKET_SIZE_EP0  (64)
#define MAX_PACKET_SIZE_EP1  (64)
#define MAX_PACKET_SIZE_EP2  (64)
#define MAX_PACKET_SIZE_EP3  (1023)
#define MAX_PACKET_SIZE_EP4  (64)
#define MAX_PACKET_SIZE_EP5  (64)
#define MAX_PACKET_SIZE_EP6  (1023)
#define MAX_PACKET_SIZE_EP7  (64)
#define MAX_PACKET_SIZE_EP8  (64)
#define MAX_PACKET_SIZE_EP9  (1023)
#define MAX_PACKET_SIZE_EP10 (64)
#define MAX_PACKET_SIZE_EP11 (64)
#define MAX_PACKET_SIZE_EP12 (1023)
#define MAX_PACKET_SIZE_EP13 (64)
#define MAX_PACKET_SIZE_EP14 (64)
#define MAX_PACKET_SIZE_EP15 (64)

/* Generic endpoints - intended to be portable accross devices */
/* and be suitable for simple USB devices. */

/* Bulk endpoints */
#define EPBULK_OUT  (EP2OUT)
#define EPBULK_IN   (EP2IN)
#define EPBULK_OUT_callback   EP2_OUT_callback
#define EPBULK_IN_callback    EP2_IN_callback
/* Interrupt endpoints */
#define EPINT_OUT   (EP1OUT)
#define EPINT_IN    (EP1IN)
#define EPINT_OUT_callback    EP1_OUT_callback
#define EPINT_IN_callback     EP1_IN_callback
/* Isochronous endpoints */
#define EPISO_OUT   (EP3OUT)
#define EPISO_IN    (EP3IN)
#define EPISO_OUT_callback    EP3_OUT_callback
#define EPISO_IN_callback     EP3_IN_callback

#define MAX_PACKET_SIZE_EPBULK  (MAX_PACKET_SIZE_EP2)
#define MAX_PACKET_SIZE_EPINT   (MAX_PACKET_SIZE_EP1)
#define MAX_PACKET_SIZE_EPISO   (MAX_PACKET_SIZE_EP3)
