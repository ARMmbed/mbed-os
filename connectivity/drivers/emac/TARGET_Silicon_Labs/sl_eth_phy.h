/***************************************************************************//**
 * @file sl_eth_phy.h
 *******************************************************************************
 * @section License
 * <b>(C) Copyright 2017 Silicon Labs, http://www.silabs.com</b>
 *******************************************************************************
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the "License"); you may
 * not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 ******************************************************************************/
#ifndef __SL_ETH_PHY_H__
#define __SL_ETH_PHY_H__

#define  PHY_BMCR          0x00u /* Basic mode control reg.    */
#define  PHY_BMSR          0x01u /* Basic mode status reg.     */
#define  PHY_PHYSID1       0x02u /* PHYS ID 1 reg.             */
#define  PHY_PHYSID2       0x03u /* PHYS ID 2 reg.             */
#define  PHY_ANAR          0x04u /* Advertisement control reg. */
#define  PHY_ANLPAR        0x05u /* Link partner ability reg.  */
#define  PHY_ANER          0x06u /* Expansion reg.             */
#define  PHY_ANNPTR        0x07u /* Next page transmit reg.    */

/* -------------- PHY_BMCR REGISTER BITS -------------- */
#define  BMCR_CTST         (0x1 <<  7) /* Collision test.              */
#define  BMCR_FULLDPLX     (0x1 <<  8) /* Full duplex.                 */
#define  BMCR_ANRESTART    (0x1 <<  9) /* Auto negotiation restart.    */
#define  BMCR_ISOLATE      (0x1 << 10) /* Disconnect Phy from MII.     */
#define  BMCR_PDOWN        (0x1 << 11) /* Power down.                  */
#define  BMCR_ANENABLE     (0x1 << 12) /* Enable auto negotiation.     */
#define  BMCR_SPEED100     (0x1 << 13) /* Select 100Mbps.              */
#define  BMCR_LOOPBACK     (0x1 << 14) /* TXD loopback bits.           */
#define  BMCR_RESET        (0x1 << 15) /* Reset.                       */

/* -------------- PHY_BMSR REGISTER BITS -------------- */
#define  BMSR_ERCAP        (0x1 <<  0) /* Ext-reg capability.          */
#define  BMSR_JCD          (0x1 <<  1) /* Jabber detected.             */
#define  BMSR_LSTATUS      (0x1 <<  2) /* Link status.                 */
#define  BMSR_ANEGCAPABLE  (0x1 <<  3) /* Able to do auto-negotiation. */
#define  BMSR_RFAULT       (0x1 <<  4) /* Remote fault detected.       */
#define  BMSR_ANEGCOMPLETE (0x1 <<  5) /* Auto-negotiation complete.   */
#define  BMSR_10HALF       (0x1 << 11) /* Can do 10mbps, half-duplex.  */
#define  BMSR_10FULL       (0x1 << 12) /* Can do 10mbps, full-duplex.  */
#define  BMSR_100HALF      (0x1 << 13) /* Can do 100mbps, half-duplex. */
#define  BMSR_100FULL      (0x1 << 14) /* Can do 100mbps, full-duplex. */
#define  BMSR_100BASE4     (0x1 << 15) /* Can do 100mbps, 4k packets.  */

/* -------------- PHY_ANAR REGISTER BITS -------------- */
#define  ANAR_SLCT             0x001Fu /* Selector bits.               */
#define  ANAR_CSMA         DEF_BIT_04  /* Only selector supported.     */
#define  ANAR_10HALF       DEF_BIT_05  /* Try for 10mbps half-duplex.  */
#define  ANAR_10FULL       DEF_BIT_06  /* Try for 10mbps full-duplex.  */
#define  ANAR_100HALF      DEF_BIT_07  /* Try for 100mbps half-duplex. */
#define  ANAR_100FULL      DEF_BIT_08  /* Try for 100mbps full-duplex. */
#define  ANAR_100BASE4     DEF_BIT_09  /* Try for 100mbps 4k packets.  */
#define  ANAR_RFAULT       DEF_BIT_13  /* Say we can detect faults.    */
#define  ANAR_LPACK        DEF_BIT_14  /* Ack link partners response.  */
#define  ANAR_NPAGE        DEF_BIT_15  /* Next page bit.               */

#define  ANAR_FULL         (ANAR_100FULL  | ANAR_10FULL  | ANAR_CSMA)
#define  ANAR_ALL          (ANAR_100BASE4 | ANAR_100FULL | ANAR_10FULL | ANAR_100HALF | ANAR_10HALF)

/* ------------- PHY_ANLPAR REGISTER BITS ------------- */
#define  ANLPAR_10HALF     (0x1 <<  5)  /* Can do 10mbps half-duplex.  */
#define  ANLPAR_10FULL     (0x1 <<  6)  /* Can do 10mbps full-duplex.  */
#define  ANLPAR_100HALF    (0x1 <<  7)  /* Can do 100mbps half-duplex. */
#define  ANLPAR_100FULL    (0x1 <<  8)  /* Can do 100mbps full-duplex. */
#define  ANLPAR_100BASE4   (0x1 <<  9)  /* Can do 100mbps 4k packets.  */
#define  ANLPAR_RFAULT     (0x1 << 13)  /* Link partner faulted.       */
#define  ANLPAR_LPACK      (0x1 << 14)  /* Link partner acked us.      */
#define  ANLPAR_NPAGE      (0x1 << 15)  /* Next page bit.              */

#define  ANLPAR_DUPLEX     (ANLPAR_10FULL  | ANLPAR_100FULL)
#define  ANLPAR_100        (ANLPAR_100FULL | ANLPAR_100HALF | ANLPAR_100BASE4)

/* -------------- PHY_ANER REGISTER BITS -------------- */
#define  ANER_NWAY        (0x1 <<  0)   /* Can do N-way auto-negotiation. */
#define  ANER_LCWP        (0x1 <<  1)   /* Got new RX page code word.     */
#define  ANER_ENABLENPAGE (0x1 <<  2)   /* This enables npage words.      */
#define  ANER_NPCAPABLE   (0x1 <<  3)   /* Link partner supports npage.   */
#define  ANER_MFAULTS     (0x1 <<  4)   /* Multiple faults detected.      */

#endif
