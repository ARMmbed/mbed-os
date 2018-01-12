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

#ifndef MBED_OS_LORAWAN_MAC_MIB_H_
#define MBED_OS_LORAWAN_MAC_MIB_H_

#include "lorawan/system/lorawan_data_structures.h"
#include "lorastack/phy/LoRaPHY.h"

// forward declaration
class LoRaMac;

class LoRaMacMib {

public:

    /** Constructor
     *
     * Sets local handles to NULL. These handles will be set when the subsystem
     * is activated by the MAC layer.
     */
    LoRaMacMib();

    /** Destructor
     *
     * Does nothing
     */
    ~LoRaMacMib();

    /** Activating MLME subsystem
     *
     * Stores pointers to MAC and PHY layer handles
     *
     * @param mac    pointer to MAC layer
     * @param phy    pointer to PHY layer
     */
    void activate_mib_subsystem(LoRaMac *mac, LoRaPHY *phy);

    /** Sets up a MIB Request
     *
     * Used to configure MAC protocol parameters using appropriate
     * key/value pair in the MIB request structure. Use this API to set
     * any system wide configurable parameter exposed by MIB service.
     *
     * @param mibSet [in]    pointer to MIB request structure
     * @param params         pointer to MAC protocol parameters which will be modified
     *
     * @return               LORAWAN_STATUS_OK if everything goes well otherwise
     *                       a negative error code is returned.
     */
    lorawan_status_t set_request(loramac_mib_req_confirm_t *mibSet,
                                 loramac_protocol_params *params);

    /** Provides access to the given MIB parameter
     *
     * Used to extract information about system wide MAC protocol parameters
     * which are exposed by MIB service.
     *
     * @param mibGet [out]   pointer to MIB request structure which will be filled in
     * @param params         pointer to MAC protocol parameters
     *
     * @return               LORAWAN_STATUS_OK if everything goes well otherwise
     *                       a negative error code is returned.
     */
    lorawan_status_t get_request(loramac_mib_req_confirm_t *mibGet,
                                 loramac_protocol_params *params);

private:

    /**
     * Pointers to MAC and PHY handles
     */
    LoRaMac *_lora_mac;
    LoRaPHY *_lora_phy;
};




#endif /* MBED_OS_LORAWAN_MAC_MIB_H_ */
