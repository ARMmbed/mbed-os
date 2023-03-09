/*
 * Copyright (c) 2015, Freescale Semiconductor, Inc.
 * Copyright 2016-2017 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef _KSZ8081RNB_REGS_H_
#define _KSZ8081RNB_REGS_H_

/*! @brief Defines the KSZ8081-specific PHY registers. */
#define PHY_CONTROL1_REG 0x1EU          /*!< The PHY control one register. */
#define PHY_CONTROL2_REG 0x1FU          /*!< The PHY control two register. */

#define PHY_CONTROL_ID1 0x22U /*!< The PHY ID1*/

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

#endif