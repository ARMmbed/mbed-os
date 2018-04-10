/* mbed Microcontroller Library
 * Copyright (c) 2017-2017 ARM Limited
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

#ifndef CORDIO_GATT_SERVER_H_
#define CORDIO_GATT_SERVER_H_

#include <stddef.h>
#include "ble/blecommon.h"
#include "ble/GattServer.h"
#include "ble/pal/SigningEventMonitor.h"
#include "ble/Gap.h"
#include "wsf_types.h"
#include "att_api.h"

/*! client characteristic configuration descriptors settings */
#define MAX_CCC_CNT 20

namespace ble {
namespace vendor {
namespace cordio {

/**
 * Cordio implementation of ::GattServer
 */
class GattServer : public ::GattServer,
                   public pal::SigningEventMonitor
{
public:
    /**
     * Return the singleton of the Cordio implementation of ::GattServer.
     */
    static GattServer &getInstance();

    /**
     * Initialize the GattServer and add mandatory services (generic access and
     * generic attribute service).
     */
    void initialize();

    /**
     * @see ::GattServer::addService
     */
    virtual ble_error_t addService(GattService &);

    /**
     * @see ::GattServer::read
     */
    virtual ble_error_t read(
        GattAttribute::Handle_t attributeHandle,
        uint8_t buffer[],
        uint16_t *lengthP
    );

    /**
     * @see ::GattServer::read
     */
    virtual ble_error_t read(
        Gap::Handle_t connectionHandle,
        GattAttribute::Handle_t attributeHandle,
        uint8_t buffer[], uint16_t *lengthP
    );

    /**
     * @see ::GattServer::write
     */
    virtual ble_error_t write(
        GattAttribute::Handle_t,
        const uint8_t[], uint16_t,
        bool localOnly = false
    );

    /**
     * @see ::GattServer::write
     */
    virtual ble_error_t write(
        Gap::Handle_t connectionHandle,
        GattAttribute::Handle_t,
        const uint8_t[],
        uint16_t,
        bool localOnly = false
    );

    /**
     * @see ::GattServer::areUpdatesEnabled
     */
    virtual ble_error_t areUpdatesEnabled(
        const GattCharacteristic &characteristic, bool *enabledP
    );

    /**
     * @see ::GattServer::areUpdatesEnabled
     */
    virtual ble_error_t areUpdatesEnabled(
        Gap::Handle_t connectionHandle,
        const GattCharacteristic &characteristic,
        bool *enabledP
    );

    /**
     * @see ::GattServer::isOnDataReadAvailable
     */
    virtual bool isOnDataReadAvailable() const;

    /**
     * @see ::GattServer::getPreferredConnectionParams
     */
    virtual ::Gap::ConnectionParams_t getPreferredConnectionParams();

    /**
     * @see ::GattServer::setPreferredConnectionParams
     */
    virtual void setPreferredConnectionParams(const ::Gap::ConnectionParams_t& params);

    /**
     * @see ::GattServer::setDeviceName
     */
    virtual ble_error_t setDeviceName(const uint8_t *deviceName);

    /**
     * @see ::GattServer::getDeviceName
     */
    virtual void getDeviceName(const uint8_t*& name, uint16_t& length);

    /**
     * @see ::GattServer::setAppearance
     */
    virtual void setAppearance(GapAdvertisingData::Appearance appearance);

    /**
     * @see ::GattServer::getAppearance
     */
    virtual GapAdvertisingData::Appearance getAppearance();

    /**
     * @see ::GattServer::reset
     */
    virtual ble_error_t reset(void);

    /**
     * @see pal::SigningEventMonitor::set_signing_event_handler
     */
    virtual void set_signing_event_handler(
        pal::SigningEventMonitor::EventHandler *signing_event_handler
    ) {
        _signing_event_handler = signing_event_handler;
    }

private:
    static void cccCback(attsCccEvt_t *pEvt);
    static void attCback(attEvt_t *pEvt);
    static uint8_t attsReadCback(dmConnId_t connId, uint16_t handle, uint8_t operation, uint16_t offset, attsAttr_t *pAttr);
    static uint8_t attsWriteCback(dmConnId_t connId, uint16_t handle, uint8_t operation, uint16_t offset, uint16_t len, uint8_t *pValue, attsAttr_t *pAttr);
    void add_generic_access_service();
    void add_generic_attribute_service();
    void* alloc_block(size_t block_size);

    struct alloc_block_t {
        alloc_block_t* next;
        uint8_t data[1];
    };

    struct internal_char_t {
        uint16_t descLen;
    };

    struct internal_service_t {
        uint16_t uuidLen;
        internal_char_t *chars;
        attsGroup_t *attGroup;
        internal_service_t *next;
    };

    pal::SigningEventMonitor::EventHandler *_signing_event_handler;

    attsCccSet_t cccSet[MAX_CCC_CNT];
    uint16_t cccValues[MAX_CCC_CNT];
    uint16_t cccHandles[MAX_CCC_CNT];
    uint8_t cccCnt;

    struct {
        attsGroup_t service;
        attsAttr_t attributes[7];
        uint8_t device_name_declaration_value[5];
        uint16_t device_name_length;
        uint8_t appearance_declaration_value[5];
        uint16_t appearance;
        uint8_t ppcp_declaration_value[5];
        uint8_t ppcp[8];

        uint8_t*& device_name_value() {
            return attributes[2].pValue;
        }
    } generic_access_service;

    struct {
        attsGroup_t service;
        attsAttr_t attributes[4];
        uint8_t service_changed_declaration[5];
    } generic_attribute_service;

    internal_service_t* registered_service;
    alloc_block_t* allocated_blocks;

    uint16_t currentHandle;

private:
    GattServer();

    GattServer(const GattServer &);
    const GattServer& operator=(const GattServer &);
};

} // namespace cordio
} // namespace vendor
} // namespace ble

#endif /* CORDIO_GATT_SERVER_H_ */
