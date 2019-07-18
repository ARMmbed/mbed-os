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

#define NUMBER_OF_LOGICAL_ENDPOINTS (4)
#define NUMBER_OF_PHYSICAL_ENDPOINTS (NUMBER_OF_LOGICAL_ENDPOINTS * 2)

/* Define physical endpoint numbers */

/*      Endpoint    No.     Type(s)       MaxPacket   DoubleBuffer  */
/*      ----------------    ------------  ----------  ---           */
#define EP0OUT   (0x00)  /* Control       64          No            */
#define EP0IN    (0x80)  /* Control       64          No            */
#define EP1OUT   (0x01)  /* Int/Bulk/Iso  64/64/1023  Yes           */
#define EP1IN    (0x81)  /* Int/Bulk/Iso  64/64/1023  Yes           */
#define EP2OUT   (0x02)  /* Int/Bulk/Iso  64/64/1023  Yes           */
#define EP2IN    (0x82)  /* Int/Bulk/Iso  64/64/1023  Yes           */
#define EP3OUT   (0x03)  /* Int/Bulk/Iso  64/64/1023  Yes           */
#define EP3IN    (0x83)  /* Int/Bulk/Iso  64/64/1023  Yes           */

/* Maximum Packet sizes */
#define MAX_PACKET_SIZE_SETUP (48)
#define MAX_PACKET_SIZE_EP0 (64)
#define MAX_PACKET_SIZE_EP1 (64) /* Int/Bulk */
#define MAX_PACKET_SIZE_EP2 (64) /* Int/Bulk */
#define MAX_PACKET_SIZE_EP3 (200) /* Int/Bulk/iso (44100 stereo 16 bits) */

#define MAX_PACKET_SIZE_EP1_ISO (1023) /* Isochronous */
#define MAX_PACKET_SIZE_EP2_ISO (1023) /* Isochronous */
#define MAX_PACKET_SIZE_EP3_ISO (1023) /* Isochronous */

/* Generic endpoints - intended to be portable accross devices */
/* and be suitable for simple USB devices. */

/* Bulk endpoint */
#define EPBULK_OUT  (EP2OUT)
#define EPBULK_IN   (EP2IN)
#define EPBULK_OUT_callback   EP2_OUT_callback
#define EPBULK_IN_callback    EP2_IN_callback
/* Interrupt endpoint */
#define EPINT_OUT   (EP1OUT)
#define EPINT_IN    (EP1IN)
#define EPINT_OUT_callback    EP1_OUT_callback
#define EPINT_IN_callback     EP1_IN_callback
/* Isochronous endpoint */
#define EPISO_OUT   (EP3OUT)
#define EPISO_IN    (EP3IN)
#define EPISO_OUT_callback    EP3_OUT_callback
#define EPISO_IN_callback     EP3_IN_callback

#define MAX_PACKET_SIZE_EPBULK  (MAX_PACKET_SIZE_EP2)
#define MAX_PACKET_SIZE_EPINT   (MAX_PACKET_SIZE_EP1)
#define MAX_PACKET_SIZE_EPISO   (MAX_PACKET_SIZE_EP3_ISO)
