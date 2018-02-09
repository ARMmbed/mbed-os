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

#ifndef MBED_OS_LORAWAN_MAC_MLME_H_
#define MBED_OS_LORAWAN_MAC_MLME_H_

#include "lorawan/system/lorawan_data_structures.h"
#include "lorastack/phy/LoRaPHY.h"
#include "lorastack/mac/LoRaMacCommand.h"

// forward declaration
class LoRaMac;

class LoRaMacMlme {

public:

    /** Constructor
     *
     * Sets local handles to NULL. These handles will be set when the subsystem
     * is activated by the MAC layer.
     */
    LoRaMacMlme();

    /** Destructor
     *
     * Does nothing
     */
    ~LoRaMacMlme();

    /** Activating MLME subsystem
     *
     * Stores pointers to MAC and PHY layer handles
     *
     * @param mac    pointer to MAC layer
     * @param phy    pointer to PHY layer
     * @param cmd    pointer to MAC commands
     */
    void activate_mlme_subsystem(LoRaMac *mac, LoRaPHY *phy, LoRaMacCommand *cmd);

    /** Sets up an MLME Request
     *
     * Sets up an MLME request, e.g., a Join Request and sends it through
     * to the central MAC control. It also modifies or uses protocol information
     * provided in the MAC protocol data structure.
     *
     * @param request        pointer to MLME request structure
     * @param params         pointer to MAC protocol parameters
     *
     * @return               LORAWAN_STATUS_OK if everything goes well otherwise
     *                       a negative error code is returned.
     */
    lorawan_status_t set_request(loramac_mlme_req_t *request, loramac_protocol_params *params);

    /** Grants access to MLME confirmation data
     *
     * @return               a reference to MLME confirm data structure
     */
    inline loramac_mlme_confirm_t& get_confirmation()
    {
        return confirmation;
    }

    /** Grants access to MLME indication data
     *
     * @return               a reference to MLME indication data structure
     */
    inline loramac_mlme_indication_t& get_indication()
    {
        return indication;
    }

private:

    /**
     * Pointers to MAC and PHY handles
     */
    LoRaMac *_lora_mac;
    LoRaPHY *_lora_phy;
    LoRaMacCommand *_mac_cmd;

    /**
     * Structure to hold MLME indication data.
     */
    loramac_mlme_indication_t indication;

    /**
     * Structure to hold MLME confirm data.
     */
    loramac_mlme_confirm_t confirmation;
};

#endif /* MBED_OS_LORAWAN_MAC_MLME_H_ */
