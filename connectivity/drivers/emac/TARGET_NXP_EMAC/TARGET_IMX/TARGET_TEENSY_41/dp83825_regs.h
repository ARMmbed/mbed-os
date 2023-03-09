/*
 * Copyright (c) 2015, Freescale Semiconductor, Inc.
 * Copyright 2016-2017 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef _DP83825_REGS_H_
#define _DP83825_REGS_H_

/*! @brief Defines the DP83825-specific PHY registers. */
#define PHY_PHYSTS_REG 0x10 ///< Phy Status Reg
#define PHY_BISCR_REG 0x16 ///< Built-In Self Test Control Register
#define PHY_RCSR_REG 0x17 ///< Receive Clock Select Register
#define PHY_LEDCR_REG 0x18 ///< LED Control Register

#define PHY_CONTROL_ID1 0x2000U /*!< The PHY ID1*/

/*!@brief Defines the mask flag of operation mode in control two register*/
#define PHY_CTL2_REMOTELOOP_MASK 0x0004U    /*!< The PHY remote loopback mask. */
#define PHY_CTL2_REFCLK_SELECT_MASK 0x0080U /*!< The PHY RMII reference clock select. */
#define PHY_CTL1_10HALFDUPLEX_MASK 0x0001U  /*!< The PHY 10M half duplex mask. */
#define PHY_CTL1_100HALFDUPLEX_MASK 0x0002U /*!< The PHY 100M half duplex mask. */
#define PHY_CTL1_10FULLDUPLEX_MASK 0x0005U  /*!< The PHY 10M full duplex mask. */
#define PHY_CTL1_100FULLDUPLEX_MASK 0x0006U /*!< The PHY 100M full duplex mask. */
#define PHY_CTL1_SPEEDUPLX_MASK 0x0007U     /*!< The PHY speed and duplex mask. */
#define PHY_CTL1_ENERGYDETECT_MASK 0x10U    /*!< The PHY signal present on rx differential pair. */
#define PHY_CTL1_LINKUP_MASK 0x100U         /*!< The PHY link up. */
#define PHY_LINK_READY_MASK (PHY_CTL1_ENERGYDETECT_MASK | PHY_CTL1_LINKUP_MASK)

// Bits for PHYSTS register
#define PHY_PHYSTS_DUPLEX_Msk (1 << 2)
#define PHY_PHYSTS_SPEED_Msk (1 << 1)

// Bits for BISCR register
#define PHY_BISCR_LOOPBACK_Msk (0b11111)
#define PHY_BISCR_LOOPBACK_Pos 0
#define PHY_BISCR_LOOPBACK_DIGITAL_LOOPBACK_10M_Val 0x1
#define PHY_BISCR_LOOPBACK_DIGITAL_LOOPBACK_100M_Val 0x4
#define PHY_BISCR_LOOPBACK_ANALOG_LOOPBACK_Val 0x8

// Bits for RCSR register
#define PHY_RCSR_RX_ELASTICITY_Pos 0
#define PHY_RCSR_RX_ELASTICITY_Msk 0b11
#define PHY_RCSR_RMII_CLK_SEL_Msk (1 << 7)

// Bits for LEDCR register
#define PHY_LEDCR_POLARITY_Msk (1 << 7)
#define PHY_LEDCR_BLINK_RATE_Msk (0b11 << 9)
#define PHY_LEDCR_BLINK_RATE_Pos 9


#endif