/* mbed Microcontroller Library
 * Copyright (c) 2006-2015 ARM Limited
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

#ifndef __NRF_CHARACTERISTIC_DESCRIPTOR_DISCOVERY_H__
#define __NRF_CHARACTERISTIC_DESCRIPTOR_DISCOVERY_H__

#include "ble/Gap.h"
#include "ble/DiscoveredCharacteristic.h"
#include "ble/CharacteristicDescriptorDiscovery.h"
#include "ble/GattClient.h"
#include "headers/nrf_ble_gattc.h"

/**
 * @brief Manage the discovery of Characteristic descriptors
 * @details is a bridge between BLE API and Nordic stack regarding Characteristic
 * Descriptor discovery. The BLE API can launch, monitor and ask for termination
 * of a discovery. The Nordic stack will provide new descriptors and indicate when
 * the discovery is done.
 */
class nRF5xCharacteristicDescriptorDiscoverer
{
    typedef CharacteristicDescriptorDiscovery::DiscoveryCallback_t DiscoveryCallback_t;
    typedef CharacteristicDescriptorDiscovery::TerminationCallback_t TerminationCallback_t;

public:
    /**
     * @brief Construct a new characteristic descriptor discoverer.
     */
    nRF5xCharacteristicDescriptorDiscoverer();

    /**
     * @brief Destroy a characteristic descriptor discoverer.
     */
    ~nRF5xCharacteristicDescriptorDiscoverer();

    /**
     * Launch a new characteristic descriptor discovery for a given DiscoveredCharacteristic.
     * @param characteristic The characteristic owning the descriptors to discover.
     * @param discoveryCallback The callback called when a descriptor is discovered.
     * @param terminationCallback The callback called when the discovery process end.
     * @return BLE_ERROR_NONE if characteristic descriptor discovery is launched successfully;
     *         else an appropriate error.
     * @note: this will be called by BLE API side.
     */
    ble_error_t launch(
        const DiscoveredCharacteristic& characteristic,
        const DiscoveryCallback_t& discoveryCallback,
        const TerminationCallback_t& terminationCallback
    );

    /**
     * @brief indicate if a characteristic descriptor discovery is active for a
     * given DiscoveredCharacteristic.
     * @param characteristic The characteristic for whom the descriptor might be
     * currently discovered.
     * @return true if descriptors of characteristic are discovered, false otherwise.
     * @note: this will be called by BLE API side.
     */
    bool isActive(const DiscoveredCharacteristic& characteristic) const;

    /**
     * @brief request the termination of characteristic descriptor discovery
     * for a give DiscoveredCharacteristic
     * @param characteristic The characteristic for whom the descriptor discovery
     * should be stopped.
     * @note: this will be called by BLE API side.
     */
    void requestTerminate(const DiscoveredCharacteristic& characteristic);

    /**
     * @brief process descriptors discovered from the Nordic stack.
     * @param connectionHandle The connection handle upon which descriptors has been
     * discovered.
     * @param descriptors Discovered descriptors.
     * @note This will be called by the Nordic stack.
     */
    void process(uint16_t connectionHandle, const ble_gattc_evt_desc_disc_rsp_t& descriptors);

    /**
     * @brief Called by the Nordic stack when the discovery is over.
     * @param The connection handle upon which the discovery process is done.
     * @param err An error if the termination is due to an error.
     */
    void terminate(uint16_t connectionHandle, ble_error_t err);

    /**
    * @brief process attribute informations from the Nordic stack.
    * @param connectionHandle The connection handle upon which
    * attribute informations has been fetch.
    * @param infos Informations around attribute, in that case the
    * 128bit UUID of a descriptor.
    * @note This will be called by the Nordic stack.
     */
    void processAttributeInformation(uint16_t handle, const ble_gattc_evt_attr_info_disc_rsp_t& infos);

private:
    // protection against copy construction and assignment
    nRF5xCharacteristicDescriptorDiscoverer(const nRF5xCharacteristicDescriptorDiscoverer&);
    nRF5xCharacteristicDescriptorDiscoverer& operator=(const nRF5xCharacteristicDescriptorDiscoverer&);

    /**
     * @brief Discovery process, it store the DiscoveredCharacteristic, the
     * discovery callback and the termination callback.
     */
    class Discovery {
    public:
        /**
         * @brief Construct an empty discovery, such can be considerate as a not running discovery.
         * @note #isEmpty function will return true
         */
        Discovery();

        /**
         * @brief Construct a valid discovery process.
         *
         * @param c the characteristic from whom descriptors will be discovered.
         * @param dCb The discovery callback called each time a descriptor is discovered.
         * @param tCb The termination callback called when the discovery terminate.
         *
         * @note #isEmpty function will return false
         */
        Discovery(const DiscoveredCharacteristic& c, const DiscoveryCallback_t& dCb, const TerminationCallback_t& tCb);

        /**
         * @brief Process the discovery of a descriptor.
         *
         * @param handle The attribute handle of the descriptor found
         * @param uuid The UUID of the descriptor found.
         */
        void process(GattAttribute::Handle_t handle, const UUID& uuid);

        /**
         * @brief Terminate the discovery process.
         *
         * @param err Error associate with the termination
         * @note after this call #isEmpty function will return true.
         */
        void terminate(ble_error_t err);

        /**
         * @brief check if the discovery process is empty or not. Empty discovery are
         * not running.
         *
         * @detail Discovery are empty after:
         *     - a default construction
         *     - a copy construction form a default constructed
         *     - an assignment from a default constructed Discovery
         * @return true if the Discovery is empty and false otherwise.
         */
        bool isEmpty() const;

        /**
         * @brief return the characteristic from whom descriptors are discovered.
         * @return the characteristic from whom descriptors are discovered.
         */
        const DiscoveredCharacteristic& getCharacteristic() const;

        /**
         * @brief equal to operator, test if two discovery process are equal
         *
         * @param lhs left hand side of the expression
         * @param rhs right hand side of the expression
         * @return true if lhs == rhs
         */
        friend bool operator==(const Discovery& lhs, const Discovery& rhs) {
            return lhs.characteristic == rhs.characteristic &&
                   lhs.onDiscovery == rhs.onDiscovery &&
                   lhs.onTerminate == rhs.onTerminate;
        }

        /**
         * @brief not equal to operator, test if two discovery process are not equal
         *
         * @param lhs left hand side of the expression
         * @param rhs right hand side of the expression
         * @return true if lhs != rhs
         */
        friend bool operator!=(const Discovery& lhs, const Discovery& rhs) {
            return !(lhs == rhs);
        }

    private:
        DiscoveredCharacteristic characteristic;
        DiscoveryCallback_t onDiscovery;
        TerminationCallback_t onTerminate;
    };

    // find a running discovery process
    Discovery* findRunningDiscovery(const DiscoveredCharacteristic& characteristic);
    Discovery* findRunningDiscovery(uint16_t handle);

    // Called to terminate a discovery is over.
    void terminate(Discovery* discovery, ble_error_t err);

    // get one slot for a discovery process
    Discovery* getAvailableDiscoverySlot();

    // indicate if a connection is already running a discovery
    bool isConnectionInUse(uint16_t connHandle);

    // low level start of a discovery
    static ble_error_t gattc_descriptors_discover(uint16_t connection_handle, uint16_t start_handle, uint16_t end_handle);

    // discovery of 128bits UUIDS
    static ble_error_t gattc_attr_info_discover(uint16_t connection_handle, uint16_t start_handle, uint16_t end_handle);

    // count of concurrent connections which can run a descriptor discovery process
    static const size_t MAXIMUM_CONCURRENT_CONNECTIONS_COUNT = 3;

    // array of running discoveries
    Discovery discoveryRunning[MAXIMUM_CONCURRENT_CONNECTIONS_COUNT];
};

#endif /*__NRF_CHARACTERISTIC_DESCRIPTOR_DISCOVERY_H__*/
