/***************************************************************************//**
* \file cy_ephy.c
* \version 1.0
*
* Provides an API implementation of the Ethernet PHY driver
*
********************************************************************************
* \copyright
* Copyright 2020, Cypress Semiconductor Corporation
* SPDX-License-Identifier: Apache-2.0
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
*******************************************************************************/

#include "cy_device.h"

#if defined (CY_IP_MXETH)

#include "cy_ephy.h"

#if defined(__cplusplus)
extern "C" {
#endif

#define DEFAULT_PHY_ADDRESS     0
#define CY_EPHY_INVALID_VALUE   0xFFFF

cy_en_ephy_status_t Cy_EPHY_Init( cy_stc_ephy_t *phy, phy_read_handle fnRead, phy_write_handle fnWrite )
{
    CY_ASSERT_L2(phy != NULL);
    CY_ASSERT_L2(fnRead != NULL);
    CY_ASSERT_L2(fnWrite != NULL);

    phy->fnPhyRead = fnRead;
    phy->fnPhyWrite = fnWrite;
    phy->phyId = CY_EPHY_INVALID_VALUE;
    phy->state = CY_EPHY_DOWN;
    phy->anar=0;
    phy->bmcr=0;

    return CY_EPHY_SUCCESS;
}

/* discover connected phy. updated phy-id in ephy structure */
cy_en_ephy_status_t Cy_EPHY_Discover( cy_stc_ephy_t *phy )
{
    uint32_t phyAddress = DEFAULT_PHY_ADDRESS;
    uint32_t ulLowerID = 0;
    uint32_t ulUpperID = 0;
    uint32_t ulPhyID = 0;

    CY_ASSERT_L2(phy->fnPhyRead != NULL);
    CY_ASSERT_L2(phy->fnPhyWrite != NULL);

    phy->fnPhyRead( phyAddress, PHYREG_03_PHYSID2, &ulLowerID );
    if ( CY_EPHY_INVALID_VALUE == ulLowerID )
        return CY_EPHY_ERROR;
    /* A valid PHY id can not be all zeros or all ones. */
    if( ( ulLowerID != ( uint16_t )~0UL ) && ( ulLowerID != ( uint16_t )0UL ) )
    {
        phy->fnPhyRead( phyAddress, PHYREG_02_PHYSID1, &ulUpperID );
        if ( CY_EPHY_INVALID_VALUE == ulUpperID )
            return CY_EPHY_ERROR;
        ulPhyID = ( _VAL2FLD( PHYID_ID1, ulUpperID ) | _VAL2FLD( PHYID_ID2, ulLowerID ) );
    }
    phy->phyId = ulPhyID;
    return CY_EPHY_SUCCESS;
}

cy_en_ephy_status_t Cy_EPHY_Reset(cy_stc_ephy_t *phy)
{
    uint32_t ulConfig;
    uint32_t phyAddress = DEFAULT_PHY_ADDRESS;
    uint32_t delay, max_delay=10;

    CY_ASSERT_L2(phy->fnPhyRead != NULL);
    CY_ASSERT_L2(phy->fnPhyWrite != NULL);

    /* Read Control register. */
    phy->fnPhyRead( phyAddress, PHYREG_00_BMCR, &ulConfig );
    phy->fnPhyWrite( phyAddress, PHYREG_00_BMCR, ( ulConfig | PHYBMCR_RESET_Msk ) );
    /* The reset should last less than a second. */
    for( delay=0; delay < max_delay; delay++ )
    {
        phy->fnPhyRead( phyAddress, PHYREG_00_BMCR, &ulConfig );
        if( _FLD2VAL(PHYBMCR_RESET, ulConfig) == 0 )
        {
            break;
        }
        Cy_SysLib_Delay(100);
    }

    /* Clear the reset bits. */
    phy->fnPhyRead( phyAddress, PHYREG_00_BMCR, &ulConfig );
    phy->fnPhyWrite( phyAddress, PHYREG_00_BMCR, ( ulConfig & ( ~PHYBMCR_RESET_Msk ) ) );
    Cy_SysLib_Delay(50);
    return CY_EPHY_SUCCESS;
}

cy_en_ephy_status_t Cy_EPHY_Configure( cy_stc_ephy_t *phy, cy_stc_ephy_config_t *config )
{
    uint32_t ulConfig, ulAdvertise = 0;
    uint32_t phyAddress = DEFAULT_PHY_ADDRESS;

    CY_ASSERT_L2(phy->fnPhyRead != NULL);
    CY_ASSERT_L2(phy->fnPhyWrite != NULL);

    if( ( config->speed == CY_EPHY_SPEED_AUTO ) && ( config->duplex == CY_EPHY_DUPLEX_AUTO ) )
    {
        ulAdvertise = PHYANAR_CSMA_Msk | PHYANAR_ALL_Msk;
    }
    else
    {
        if( config->speed == CY_EPHY_SPEED_AUTO )
        {
            if( config->duplex == CY_EPHY_DUPLEX_FULL )
            {
                ulAdvertise |= PHYANAR_10BASE_T_FD_Msk | PHYANAR_100BASE_TX_FD_Msk;
            }
            else
            {
                ulAdvertise |= PHYANAR_10BASE_T_Msk | PHYANAR_100BASE_TX_Msk;
            }
        }
        else if( config->duplex == CY_EPHY_DUPLEX_AUTO )
        {
            if( config->speed == CY_EPHY_SPEED_10 )
            {
                ulAdvertise |= PHYANAR_10BASE_T_Msk | PHYANAR_10BASE_T_FD_Msk;
            }
            else
            {
                ulAdvertise |= PHYANAR_100BASE_TX_Msk | PHYANAR_100BASE_TX_FD_Msk;
            }
        }
        else if( config->speed == CY_EPHY_SPEED_100 )
        {
            if( config->duplex == CY_EPHY_DUPLEX_FULL )
            {
                ulAdvertise |= PHYANAR_100BASE_TX_FD_Msk;
            }
            else
            {
                ulAdvertise |= PHYANAR_100BASE_TX_Msk;
            }
        }
        else
        {
            if( config->duplex == CY_EPHY_DUPLEX_FULL )
            {
                ulAdvertise |= PHYANAR_10BASE_T_FD_Msk;
            }
            else
            {
                ulAdvertise |= PHYANAR_10BASE_T_Msk;
            }
        }
    }

    /* Send a reset commando to a set of PHY-ports. */
    Cy_EPHY_Reset( phy );

    /* Write advertise register. */
    phy->fnPhyWrite( phyAddress, PHYREG_04_ADVERTISE, ulAdvertise );

//    Cy_SysLib_Delay(500);

    /* Read Control register. */
    phy->fnPhyRead( phyAddress, PHYREG_00_BMCR, &ulConfig );

    ulConfig &= ~( PHYBMCR_SPEED_100_Msk | PHYBMCR_FULL_DUPLEX_Msk );

    ulConfig |= PHYBMCR_AN_ENABLE_Msk;

    if( ( config->speed == CY_EPHY_SPEED_100 ) || ( config->speed == CY_EPHY_SPEED_AUTO ) )
    {
        ulConfig |= PHYBMCR_SPEED_100_Msk;
    }
    else if( config->speed == CY_EPHY_SPEED_10 )
    {
        ulConfig &= ~PHYBMCR_SPEED_100_Msk;
    }

    if( ( config->duplex == CY_EPHY_DUPLEX_FULL ) || ( config->duplex == CY_EPHY_DUPLEX_AUTO ) )
    {
        ulConfig |= PHYBMCR_FULL_DUPLEX_Msk;
    }
    else if( config->duplex == CY_EPHY_DUPLEX_HALF )
    {
        ulConfig &= ~PHYBMCR_FULL_DUPLEX_Msk;
    }

    /* Keep these values for later use. */
    phy->bmcr = ulConfig & ~PHYBMCR_ISOLATE_Msk;
    phy->anar = ulAdvertise;
    return CY_EPHY_SUCCESS;
}

cy_en_ephy_status_t Cy_EPHY_StartAutoNegotiation(cy_stc_ephy_t *phy)
{
    uint32_t phyAddress=DEFAULT_PHY_ADDRESS;
    uint32_t ulRegValue;
    uint32 delay = 0, max_try=30;

    CY_ASSERT_L2(phy->fnPhyRead != NULL);
    CY_ASSERT_L2(phy->fnPhyWrite != NULL);

    /* Enable Auto-Negotiation. */
    phy->fnPhyWrite( phyAddress, PHYREG_04_ADVERTISE, phy->anar );
    phy->fnPhyWrite( phyAddress, PHYREG_00_BMCR, ( phy->bmcr | PHYBMCR_AN_RESTART_Msk ) );

    /* Wait until the auto-negotiation will be completed */
    /* max wait = 3s */
    for( delay=0; delay<max_try; delay++ )
    {
        phy->fnPhyRead( phyAddress, PHYREG_01_BMSR, &ulRegValue );
        if( _FLD2VAL( PHYBMSR_AN_COMPLETE,ulRegValue) != 0 )
        {
            break;
        }
        Cy_SysLib_Delay(100);
    }

    if(delay < max_try)
        return CY_EPHY_SUCCESS;
    else
        return CY_EPHY_ERROR;
}

/*
 * we need to read the status register twice, keeping the second value.
 */

uint32_t Cy_EPHY_GetLinkStatus(cy_stc_ephy_t *phy)
{
    uint32_t phyAddress=DEFAULT_PHY_ADDRESS;
    uint32_t status;

    CY_ASSERT_L2(phy->fnPhyRead != NULL);
    CY_ASSERT_L2(phy->fnPhyWrite != NULL);

    /* read bmcr */
    phy->fnPhyRead( phyAddress, PHYREG_00_BMCR, &status );

    /* check for auto-neg-restart. Autoneg is being started, therefore disregard
     * BMSR value and report link as down.
     */
    if (status & PHYBMCR_AN_RESTART_Msk )
        return 0;

    /* Read link and autonegotiation status */
    status = 0;
    phy->fnPhyRead( phyAddress, PHYREG_01_BMSR, &status );

    if (CY_EPHY_INVALID_VALUE == status)
        return 0;
    else
        return _FLD2VAL( PHYBMSR_LINK_STATUS, status );
}



#if defined(__cplusplus)
}
#endif

#endif /* CY_IP_MXETH */

/* [] END OF FILE */
