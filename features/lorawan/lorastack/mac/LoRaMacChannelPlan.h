/**
 \code
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
\endcode

Description: LoRaWAN stack layer that controls both MAC and PHY underneath

License: Revised BSD License, see LICENSE.TXT file include in the project

Maintainer: Miguel Luis ( Semtech ), Gregory Cristian ( Semtech ) and Daniel Jaeckle ( STACKFORCE )


Copyright (c) 2017, Arm Limited and affiliates.

SPDX-License-Identifier: BSD-3-Clause
*/

#ifndef MBED_LORAWAN_LORAMACCHANNELPLAN_H_
#define MBED_LORAWAN_LORAMACCHANNELPLAN_H_

#include "system/lorawan_data_structures.h"
#include "lorastack/phy/LoRaPHY.h"

class LoRaMacChannelPlan {

public:

    /** Constructor
     *
     * Sets local handles to NULL. These handles will be set when the subsystem
     * is activated by the MAC layer.
     */
    LoRaMacChannelPlan();

    /** Destructor
     *
     * Does nothing
     */
    ~LoRaMacChannelPlan();

    /** Activates Channel Planning subsystem
     *
     * Stores pointers to PHY layer MIB subsystem
     *
     * @param phy    pointer to PHY layer
     */
    void activate_channelplan_subsystem(LoRaPHY *phy);

    /** Set a given channel plan
     *
     * Used to set application provided channel plan. This API can be used to
     * set a single channel as well to the existing channel plan.
     *
     * @param plan    a reference to application channel plan. PHY layer takes a
     *                copy of the channel parameters provided within.
     *
     * @return        LORAWAN_STATUS_OK if everything goes well otherwise
     *                a negative error code is returned.
     */
    lorawan_status_t set_plan(const lorawan_channelplan_t& plan);

    /** Access the active channel plan
     *
     * Used to get active channel plan.
     *
     * @param plan          a reference to application provided channel plan structure
     *                      which gets filled in with active channel plan data.
     *
     * @param channel_list  pointer to structure containing channel information
     *
     * @return              LORAWAN_STATUS_OK if everything goes well otherwise
     *                      a negative error code is returned.
     */
    lorawan_status_t get_plan(lorawan_channelplan_t& plan, const channel_params_t* channel_list);

    /** Remove the active channel plan
     *
     * Drops the whole channel list except the 'Default Channels' ofcourse.
     *
     * @return        LORAWAN_STATUS_OK if everything goes well otherwise
     *                a negative error code is returned.
     */
    lorawan_status_t remove_plan();

    /** Remove a single channel from the plan
     *
     * @param id    the channel id which needs to be removed
     *
     * @return      LORAWAN_STATUS_OK if everything goes well otherwise
     *              a negative error code is returned.
     */
    lorawan_status_t remove_single_channel(uint8_t id);

private:

    /**
     * Local handles
     */
    LoRaPHY *_lora_phy;
};



#endif /* MBED_LORAWAN_LORAMACCHANNELPLAN_H_ */
