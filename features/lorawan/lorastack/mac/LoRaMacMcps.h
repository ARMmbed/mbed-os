/**
 / _____)             _              | |
( (____  _____ ____ _| |_ _____  ____| |__
 \____ \| ___ |    (_   _) ___ |/ ___)  _ \
 _____) ) ____| | | || |_| ____( (___| | | |
(______/|_____)_|_|_| \__)_____)\____)_| |_|
    (C)2013 Semtech
 ___ _____ _   ___ _  _____ ___  ___  ___ ___
/ __|_   _/_\ / __| |/ / __/ _ \| _ \/ __| __|
\__ \ | |/ _ \ (__| ' <| _| (_) |   / (__| _|
|___/ |_/_/ \_\___|_|\_\_| \___/|_|_\\___|___|
embedded.connectivity.solutions===============

Description: LoRaWAN stack layer that controls both MAC and PHY underneath

License: Revised BSD License, see LICENSE.TXT file include in the project

Maintainer: Miguel Luis ( Semtech ), Gregory Cristian ( Semtech ) and Daniel Jaeckle ( STACKFORCE )


Copyright (c) 2017, Arm Limited and affiliates.

SPDX-License-Identifier: BSD-3-Clause
*/

#ifndef MBED_OS_LORAWAN_MAC_MCPS_H_
#define MBED_OS_LORAWAN_MAC_MCPS_H_

#include "lorawan/system/lorawan_data_structures.h"
#include "lorastack/phy/LoRaPHY.h"

// forward declaration
class LoRaMac;

class LoRaMacMcps {

public:

    /** Constructor
     *
     * Sets local handles to NULL. These handles will be set when the subsystem
     * is activated by the MAC layer.
     */
    LoRaMacMcps();

    /** Destructor
     *
     * Does nothing
     */
    ~LoRaMacMcps();

    /**
     * @brief reset_confirmation Resets the confirmation struct
     */
    void reset_confirmation();

    /** Activating MCPS subsystem
     *
     * Stores pointers to MAC and PHY layer handles
     */
    void activate_mcps_subsystem();

    /** Grants access to MCPS confirmation data
     *
     * @return               a reference to MCPS confirm data structure
     */
    loramac_mcps_confirm_t& get_confirmation();

    /** Grants access to MCPS indication data
     *
     * @return               a reference to MCPS indication data structure
     */
    loramac_mcps_indication_t& get_indication();

private:

    /**
     * Structure to hold MCPS indication data.
     */
    loramac_mcps_indication_t indication;

    /**
     * Structure to hold MCPS confirm data.
     */
    loramac_mcps_confirm_t confirmation;
};

#endif /* MBED_OS_LORAWAN_MAC_MCPS_H_ */
