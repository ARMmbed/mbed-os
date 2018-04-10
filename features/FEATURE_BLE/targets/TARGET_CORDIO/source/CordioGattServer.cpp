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

#include <algorithm>
#include "CordioGattServer.h"
#include "mbed.h"
#include "wsf_types.h"
#include "att_api.h"

static UUID cccUUID(BLE_UUID_DESCRIPTOR_CLIENT_CHAR_CONFIG);
static const uint16_t cccSize = sizeof(uint16_t);

namespace ble {
namespace vendor {
namespace cordio {

GattServer &GattServer::getInstance()
{
    static GattServer m_instance;
    return m_instance;
}

void GattServer::initialize()
{
    add_generic_access_service();
    add_generic_attribute_service();
}

ble_error_t GattServer::addService(GattService &service)
{
    ++currentHandle;
    uint16_t startHandle = currentHandle;

    internal_service_t *internal_service = new internal_service_t;

    if (registered_service) {
        internal_service->next = registered_service;
    } else {
        internal_service->next = NULL;
    }

    registered_service = internal_service;

    // Create cordio attribute group
    internal_service->attGroup = new attsGroup_t;

    // Determine the attribute list length
    unsigned int attListLen = 1;
    for (int i = 0; i < service.getCharacteristicCount(); i++) {
        attListLen += 2;
        GattCharacteristic *p_char = service.getCharacteristic(i);

        attListLen += p_char->getDescriptorCount();
        if (p_char->getProperties() & (GattCharacteristic::BLE_GATT_CHAR_PROPERTIES_NOTIFY | GattCharacteristic::BLE_GATT_CHAR_PROPERTIES_INDICATE)) {
            // add a CCCD
            attListLen++;
        }
    }

    // Create cordio attribute list
    internal_service->attGroup->pAttr = (attsAttr_t*)alloc_block(attListLen * sizeof(attsAttr_t));
    if (internal_service->attGroup->pAttr == NULL) {
        return BLE_ERROR_BUFFER_OVERFLOW;
    }

    // Create characteristics
    internal_service->chars = new internal_char_t [service.getCharacteristicCount()];

    attsAttr_t *currAtt = internal_service->attGroup->pAttr;

    /* Service */
    currAtt->pUuid = attPrimSvcUuid;
    if (service.getUUID().shortOrLong() == UUID::UUID_TYPE_LONG) {
        internal_service->uuidLen = UUID::LENGTH_OF_LONG_UUID;
    } else {
        internal_service->uuidLen = sizeof(UUID::ShortUUIDBytes_t);
    }
    currAtt->pValue = (uint8_t*)alloc_block(internal_service->uuidLen);
    memcpy(currAtt->pValue, service.getUUID().getBaseUUID(), internal_service->uuidLen);
    currAtt->maxLen = internal_service->uuidLen;
    currAtt->pLen = &internal_service->uuidLen;
    currAtt->settings = 0;
    currAtt->permissions = ATTS_PERMIT_READ;

    currAtt++;

    /* Add characteristics to the service */
    for (int i = 0; i < service.getCharacteristicCount(); i++) {
        GattCharacteristic *p_char = service.getCharacteristic(i);

        /* Skip any incompletely defined, read-only characteristics. */
        if ((p_char->getValueAttribute().getValuePtr() == NULL) &&
            (p_char->getValueAttribute().getLength() == 0) &&
            (p_char->getProperties() == GattCharacteristic::BLE_GATT_CHAR_PROPERTIES_READ)) {
            continue;
        }

        // Create Characteristic Attribute
        currentHandle += 2;
        currAtt->pUuid = attChUuid;

        p_char->getValueAttribute().setHandle(currentHandle);
        internal_service->chars[i].descLen = 1 + sizeof(currentHandle) + p_char->getValueAttribute().getUUID().getLen();
        currAtt->pValue = (uint8_t*) alloc_block(internal_service->chars[i].descLen);
        uint8_t *pValue = currAtt->pValue;
        *pValue++ = p_char->getProperties();
        memcpy(pValue, &currentHandle, sizeof(currentHandle));
        pValue += sizeof(currentHandle);
        memcpy(pValue, p_char->getValueAttribute().getUUID().getBaseUUID(), p_char->getValueAttribute().getUUID().getLen());

        currAtt->pLen = &internal_service->chars[i].descLen;
        currAtt->maxLen = internal_service->chars[i].descLen;
        currAtt->settings = 0;
        currAtt->permissions = ATTS_PERMIT_READ;
        currAtt++;

        // Create Value Attribute
        currAtt->pUuid = p_char->getValueAttribute().getUUID().getBaseUUID();
        currAtt->maxLen = p_char->getValueAttribute().getMaxLength();
        currAtt->pLen = (uint16_t*) alloc_block(currAtt->maxLen + sizeof(uint16_t));
        *currAtt->pLen = p_char->getValueAttribute().getLength();
        currAtt->pValue = (uint8_t*) ((uint16_t*)currAtt->pLen + 1);
        memcpy(currAtt->pValue, p_char->getValueAttribute().getValuePtr(), *currAtt->pLen);
        memset(currAtt->pValue + *currAtt->pLen, 0, currAtt->maxLen - *currAtt->pLen);

        currAtt->settings = ATTS_SET_WRITE_CBACK | ATTS_SET_READ_CBACK;
        if (p_char->getValueAttribute().getUUID().shortOrLong() == UUID::UUID_TYPE_LONG) {
            currAtt->settings |= ATTS_SET_UUID_128;
        }
        if (p_char->getValueAttribute().hasVariableLength()) {
            currAtt->settings |= ATTS_SET_VARIABLE_LEN;
        }

        currAtt->permissions = 0;
        if (p_char->getProperties() & GattCharacteristic::BLE_GATT_CHAR_PROPERTIES_READ)  { currAtt->permissions |= ATTS_PERMIT_READ; }
        if (p_char->getProperties() & GattCharacteristic::BLE_GATT_CHAR_PROPERTIES_WRITE) { currAtt->permissions |= ATTS_PERMIT_WRITE; }
        currAtt++;

        bool cccCreated = false;

        for (int i = 0; i < p_char->getDescriptorCount(); i++) {
            GattAttribute *p_att = p_char->getDescriptor(i);

            currentHandle++;

            p_att->setHandle(currentHandle);

            currAtt->pUuid = p_att->getUUID().getBaseUUID();
            currAtt->maxLen = p_att->getMaxLength();
            currAtt->pLen = (uint16_t*) alloc_block(currAtt->maxLen + sizeof(uint16_t));
            *currAtt->pLen = p_att->getLength();
            currAtt->pValue = (uint8_t*) ((uint16_t*)currAtt->pLen + 1);
            memcpy(currAtt->pValue, p_att->getValuePtr(), *currAtt->pLen);
            memset(currAtt->pValue + *currAtt->pLen, 0, currAtt->maxLen - *currAtt->pLen);

            currAtt->settings = 0;
            currAtt->permissions = ATTS_PERMIT_READ | ATTS_PERMIT_WRITE;
            if (p_att->getUUID().shortOrLong() == UUID::UUID_TYPE_LONG) {
                currAtt->settings |= ATTS_SET_UUID_128;
            }
            if (p_att->getUUID() == UUID(BLE_UUID_DESCRIPTOR_CLIENT_CHAR_CONFIG)) {
                cccCreated = true;
                currAtt->settings |= ATTS_SET_CCC;
                currAtt->permissions |= ATTS_PERMIT_READ;
                currAtt->permissions |= ATTS_PERMIT_WRITE;

                if (cccCnt < MAX_CCC_CNT) {
                    cccSet[cccCnt].handle = currentHandle;
                    cccSet[cccCnt].valueRange = 0;
                    if (p_char->getProperties() & GattCharacteristic::BLE_GATT_CHAR_PROPERTIES_NOTIFY) {
                        cccSet[cccCnt].valueRange |= ATT_CLIENT_CFG_NOTIFY;
                    }
                    if (p_char->getProperties() & GattCharacteristic::BLE_GATT_CHAR_PROPERTIES_INDICATE) {
                        cccSet[cccCnt].valueRange |= ATT_CLIENT_CFG_INDICATE;
                    }
                    cccSet[cccCnt].secLevel = DM_SEC_LEVEL_NONE;
                    cccHandles[cccCnt] = p_char->getValueAttribute().getHandle();
                    cccCnt++;
                } else {
                    return BLE_ERROR_PARAM_OUT_OF_RANGE;
                }
            }
            if (p_att->hasVariableLength()) {
                currAtt->settings |= ATTS_SET_VARIABLE_LEN;
            }
            currAtt++;
        }

        if (!cccCreated && (p_char->getProperties() & (GattCharacteristic::BLE_GATT_CHAR_PROPERTIES_NOTIFY | GattCharacteristic::BLE_GATT_CHAR_PROPERTIES_INDICATE))) {
            /* There was not a CCCD included in the descriptors, but this
             * characteristic is notifiable and/or indicatable. A CCCD is
             * required so create one now.
             */
            if (cccCnt >= MAX_CCC_CNT) {
                return BLE_ERROR_PARAM_OUT_OF_RANGE;
            }

            currentHandle++;

            currAtt->pUuid = cccUUID.getBaseUUID();
            currAtt->pValue = (uint8_t*)&cccValues[cccCnt];
            currAtt->pLen = (uint16_t*)&cccSize;
            currAtt->maxLen = sizeof(uint16_t);
            currAtt->settings = ATTS_SET_CCC;
            currAtt->permissions = (ATTS_PERMIT_READ | ATTS_PERMIT_WRITE);

            cccSet[cccCnt].handle = currentHandle;
            cccSet[cccCnt].valueRange = 0;
            if (p_char->getProperties() & GattCharacteristic::BLE_GATT_CHAR_PROPERTIES_NOTIFY) {
                cccSet[cccCnt].valueRange |= ATT_CLIENT_CFG_NOTIFY;
            }
            if (p_char->getProperties() & GattCharacteristic::BLE_GATT_CHAR_PROPERTIES_INDICATE) {
                cccSet[cccCnt].valueRange |= ATT_CLIENT_CFG_INDICATE;
            }
            cccSet[cccCnt].secLevel = DM_SEC_LEVEL_NONE;
            cccHandles[cccCnt] = p_char->getValueAttribute().getHandle();

            cccCnt++;
            currAtt++;
        }
    }

    internal_service->attGroup->pNext = NULL;
    internal_service->attGroup->readCback = attsReadCback;
    internal_service->attGroup->writeCback = attsWriteCback;
    internal_service->attGroup->startHandle = startHandle;
    internal_service->attGroup->endHandle = currentHandle;
    AttsAddGroup(internal_service->attGroup);
    AttsCccRegister(cccCnt, (attsCccSet_t*)cccSet, cccCback);

    service.setHandle(startHandle);

    return BLE_ERROR_NONE;
}

ble_error_t GattServer::read(GattAttribute::Handle_t attributeHandle, uint8_t buffer[], uint16_t * lengthP)
{
    uint16_t attribute_length = 0;
    uint8_t* attribute_value = NULL;

    if (AttsGetAttr(attributeHandle, &attribute_length, &attribute_value) != ATT_SUCCESS) {
        return BLE_ERROR_PARAM_OUT_OF_RANGE;
    }

    if (buffer) {
        memcpy(buffer, attribute_value, std::min(*lengthP, attribute_length));
    }

    *lengthP = attribute_length;

    return BLE_ERROR_NONE;
}

ble_error_t GattServer::read(Gap::Handle_t connectionHandle, GattAttribute::Handle_t attributeHandle, uint8_t buffer[], uint16_t *lengthP)
{
    // Check to see if this is a CCCD
    uint8_t idx;
    for (idx = 0; idx < cccCnt; idx++) {
        if (attributeHandle == cccSet[idx].handle) {
            if (connectionHandle == DM_CONN_ID_NONE) { // CCCDs are always 16 bits
                return BLE_ERROR_PARAM_OUT_OF_RANGE;
            }
            *((uint16_t*)buffer) = AttsCccGet(connectionHandle, idx);
            *lengthP = 2;   // CCCDs are always 16 bits
            return BLE_ERROR_NONE;
        }
    }

    // This is not a CCCD. Use the non-connection specific update method.
    return read(attributeHandle, buffer, lengthP);
}

ble_error_t GattServer::write(GattAttribute::Handle_t attributeHandle, const uint8_t buffer[], uint16_t len, bool localOnly)
{
    // Check to see if this is a CCCD, if it is the case update the value for all
    // connections
    uint8_t idx;
    for (idx = 0; idx < cccCnt; idx++) {
        if (attributeHandle == cccSet[idx].handle) {
            for (uint16_t conn_id = 0, conn_found = 0; (conn_found < DM_CONN_MAX) && (conn_id < 0x100); ++conn_id) {
                if (DmConnInUse(conn_id) == true) {
                    ++conn_found;
                } else {
                    continue;
                }

                AttsCccSet(conn_id, idx, *((uint16_t*)buffer));
            }
            return BLE_ERROR_NONE;
        }
    }

    // write the value to the attribute handle
    if (AttsSetAttr(attributeHandle, len, (uint8_t*)buffer) != ATT_SUCCESS) {
        return BLE_ERROR_PARAM_OUT_OF_RANGE;
    }

    // return if the update does not have to be propagated to peers
    if (localOnly) {
        return BLE_ERROR_NONE;
    }

    // Check to see if this characteristic has a CCCD attribute
    for (idx = 0; idx < cccCnt; idx++) {
        if (attributeHandle == cccHandles[idx]) {
            break;
        }
    }

    // exit if the characteristic has no CCCD attribute
    if (idx >= cccCnt) {
        return BLE_ERROR_NONE;
    }

    // This characteristic has a CCCD attribute. Handle notifications and indications.
    // for all connections

    for (uint16_t conn_id = 0, conn_found = 0; (conn_found < DM_CONN_MAX) && (conn_id < 0x100); ++conn_id) {
        if (DmConnInUse(conn_id) == true) {
            ++conn_found;
        } else {
            uint16_t cccEnabled = AttsCccEnabled(conn_id, idx);
            if (cccEnabled & ATT_CLIENT_CFG_NOTIFY) {
                AttsHandleValueNtf(conn_id, attributeHandle, len, (uint8_t*)buffer);
            }
            if (cccEnabled & ATT_CLIENT_CFG_INDICATE) {
                AttsHandleValueInd(conn_id, attributeHandle, len, (uint8_t*)buffer);
            }
        }

        AttsCccSet(conn_id, idx, *((uint16_t*)buffer));
    }

    return BLE_ERROR_NONE;
}

ble_error_t GattServer::write(Gap::Handle_t connectionHandle, GattAttribute::Handle_t attributeHandle, const uint8_t buffer[], uint16_t len, bool localOnly)
{
    // Check to see if this is a CCCD
    uint8_t idx;
    for (idx = 0; idx < cccCnt; idx++) {
        if (attributeHandle == cccSet[idx].handle) {
            if ((connectionHandle == DM_CONN_ID_NONE) || (len != 2)) { // CCCDs are always 16 bits
                return BLE_ERROR_PARAM_OUT_OF_RANGE;
            }
            AttsCccSet(connectionHandle, idx, *((uint16_t*)buffer));
            return BLE_ERROR_NONE;
        }
    }

    // write the value to the attribute handle
    if (AttsSetAttr(attributeHandle, len, (uint8_t*)buffer) != ATT_SUCCESS) {
        return BLE_ERROR_PARAM_OUT_OF_RANGE;
    }

    // return if the update does not have to be propagated to peers
    if (localOnly) {
        return BLE_ERROR_NONE;
    }

    // Check to see if this characteristic has a CCCD attribute
    for (idx = 0; idx < cccCnt; idx++) {
        if (attributeHandle == cccHandles[idx]) {
            break;
        }
    }

    // exit if the characteristic has no CCCD attribute
    if (idx >= cccCnt) {
        return BLE_ERROR_NONE;
    }

    // This characteristic has a CCCD attribute. Handle notifications and indications.
    uint16_t cccEnabled = AttsCccEnabled(connectionHandle, idx);
    if (cccEnabled & ATT_CLIENT_CFG_NOTIFY) {
        AttsHandleValueNtf(connectionHandle, attributeHandle, len, (uint8_t*)buffer);
    }
    if (cccEnabled & ATT_CLIENT_CFG_INDICATE) {
        AttsHandleValueInd(connectionHandle, attributeHandle, len, (uint8_t*)buffer);
    }

    return BLE_ERROR_NONE;
}

ble_error_t GattServer::areUpdatesEnabled(const GattCharacteristic &characteristic, bool *enabledP)
{
    for (size_t idx = 0; idx < cccCnt; idx++) {
        if (characteristic.getValueHandle() == cccHandles[idx]) {
            for (uint16_t conn_id = 0, conn_found = 0; (conn_found < DM_CONN_MAX) && (conn_id < 0x100); ++conn_id) {
                if (DmConnInUse(conn_id) == true) {
                    ++conn_found;
                } else {
                    continue;
                }

                uint16_t cccValue = AttsCccGet(conn_id, idx);
                if ((cccValue & ATT_CLIENT_CFG_NOTIFY) || (cccValue & ATT_CLIENT_CFG_INDICATE)) {
                    *enabledP = true;
                    return BLE_ERROR_NONE;
                }
            }
            *enabledP = false;
            return BLE_ERROR_NONE;
        }
    }

    return BLE_ERROR_PARAM_OUT_OF_RANGE;
}

ble_error_t GattServer::areUpdatesEnabled(Gap::Handle_t connectionHandle, const GattCharacteristic &characteristic, bool *enabledP)
{
    if (connectionHandle != DM_CONN_ID_NONE) {
        uint8_t idx;
        for (idx = 0; idx < cccCnt; idx++) {
            if (characteristic.getValueHandle() == cccHandles[idx]) {
                uint16_t cccValue = AttsCccGet(connectionHandle, idx);
                if (cccValue & ATT_CLIENT_CFG_NOTIFY || (cccValue & ATT_CLIENT_CFG_INDICATE)) {
                    *enabledP = true;
                } else {
                    *enabledP = false;
                }
                return BLE_ERROR_NONE;
            }
        }
    }

    return BLE_ERROR_PARAM_OUT_OF_RANGE;
}

bool GattServer::isOnDataReadAvailable() const
{
    return true;
}

::Gap::ConnectionParams_t GattServer::getPreferredConnectionParams()
{
    ::Gap::ConnectionParams_t params = { 0 };
    memcpy(&params.minConnectionInterval, generic_access_service.ppcp, 2);
    memcpy(&params.maxConnectionInterval, generic_access_service.ppcp + 2, 2);
    memcpy(&params.slaveLatency, generic_access_service.ppcp + 4, 2);
    memcpy(&params.connectionSupervisionTimeout, generic_access_service.ppcp + 6, 2);
    return params;
}

void GattServer::setPreferredConnectionParams(const ::Gap::ConnectionParams_t& params)
{
    memcpy(generic_access_service.ppcp, &params.minConnectionInterval, 2);
    memcpy(generic_access_service.ppcp + 2, &params.maxConnectionInterval, 2);
    memcpy(generic_access_service.ppcp + 4, &params.slaveLatency, 2);
    memcpy(generic_access_service.ppcp + 6, &params.connectionSupervisionTimeout, 2);
}

ble_error_t GattServer::setDeviceName(const uint8_t *deviceName)
{
    size_t length = 0;

    if (deviceName != NULL) {
        length = strlen((const char*)deviceName);
    }

    if (length == 0) {
        free(generic_access_service.device_name_value());
    } else {
        uint8_t* res = (uint8_t*) realloc(generic_access_service.device_name_value(), length);
        if (res == NULL) {
            return BLE_ERROR_NO_MEM;
        }

        generic_access_service.device_name_value() = res;
        memcpy(res, deviceName, length);
    }

    generic_access_service.device_name_length = length;

    return BLE_ERROR_NONE;
}

void GattServer::getDeviceName(const uint8_t*& name, uint16_t& length)
{
    length = generic_access_service.device_name_length;
    name = generic_access_service.device_name_value();
}

void GattServer::setAppearance(GapAdvertisingData::Appearance appearance)
{
    generic_access_service.appearance = appearance;
}

GapAdvertisingData::Appearance GattServer::getAppearance()
{
    return (GapAdvertisingData::Appearance) generic_access_service.appearance;
}

ble_error_t GattServer::reset(void)
{
    this->::GattServer::reset();

    while (registered_service) {
        internal_service_t* s = registered_service;
        registered_service = s->next;
        AttsRemoveGroup(s->attGroup->startHandle);
        delete s->attGroup;
        delete[] s->chars;
        delete s;
    }

    while (allocated_blocks) {
        alloc_block_t* b = allocated_blocks;
        allocated_blocks = b->next;
        free(b);
    }

    AttsRemoveGroup(generic_access_service.service.startHandle);
    AttsRemoveGroup(generic_attribute_service.service.startHandle);
    free(generic_access_service.device_name_value());

    currentHandle = 0;
    cccCnt = 0;

    AttsCccRegister(cccCnt, (attsCccSet_t*)cccSet, cccCback);

    return BLE_ERROR_NONE;
}

void GattServer::cccCback(attsCccEvt_t *pEvt)
{
    if (pEvt->value & (ATT_CLIENT_CFG_NOTIFY | ATT_CLIENT_CFG_INDICATE)) {
        getInstance().handleEvent(GattServerEvents::GATT_EVENT_UPDATES_ENABLED, pEvt->handle);
    } else {
        getInstance().handleEvent(GattServerEvents::GATT_EVENT_UPDATES_DISABLED, pEvt->handle);
    }
}

void GattServer::attCback(attEvt_t *pEvt)
{
    // TODO enable again
    if (pEvt->hdr.status == ATT_SUCCESS) {
        getInstance().handleEvent(GattServerEvents::GATT_EVENT_DATA_SENT, pEvt->handle);
    }
}

uint8_t GattServer::attsReadCback(dmConnId_t connId, uint16_t handle, uint8_t operation, uint16_t offset, attsAttr_t *pAttr)
{
    GattReadCallbackParams cbParams = {
        connId,
        handle,
        offset,
        *pAttr->pLen,
        pAttr->pValue,
        /* status */ BLE_ERROR_NONE,
    };
    getInstance().handleDataReadEvent(&cbParams);

    return ATT_SUCCESS;
}

uint8_t GattServer::attsWriteCback(dmConnId_t connId, uint16_t handle, uint8_t operation, uint16_t offset, uint16_t len, uint8_t *pValue, attsAttr_t *pAttr)
{
    uint8_t err;

    /* TODO: offset is not handled properly */
    if ((err = AttsSetAttr(handle, len, pValue)) != ATT_SUCCESS) {
        return err;
    }

    GattWriteCallbackParams::WriteOp_t writeOp;
    switch (operation) {
        case ATT_PDU_WRITE_REQ:
            writeOp = GattWriteCallbackParams::OP_WRITE_REQ;
            break;
        case ATT_PDU_WRITE_CMD:
            writeOp = GattWriteCallbackParams::OP_WRITE_CMD;
            break;
        case ATT_PDU_SIGNED_WRITE_CMD:
            if (getInstance()._signing_event_handler) {
                getInstance()._signing_event_handler->on_signed_write_received(
                    connId,
                    AttsGetSignCounter(connId)
                );
            }
            writeOp = GattWriteCallbackParams::OP_SIGN_WRITE_CMD;
            break;
        case ATT_PDU_PREP_WRITE_REQ:
            writeOp = GattWriteCallbackParams::OP_PREP_WRITE_REQ;
            break;
        case ATT_PDU_EXEC_WRITE_REQ:
            writeOp = GattWriteCallbackParams::OP_EXEC_WRITE_REQ_NOW;
            break;
        default:
            writeOp = GattWriteCallbackParams::OP_INVALID;
            break;
    }

    GattWriteCallbackParams cbParams = {
        connId,
        handle,
        writeOp,
        offset,
        len,
        pValue
    };
    getInstance().handleDataWrittenEvent(&cbParams);

    return ATT_SUCCESS;
}

void GattServer::add_generic_access_service()
{
    ++currentHandle;
    generic_access_service.service.pNext = NULL;
    generic_access_service.service.startHandle = currentHandle;
    generic_access_service.service.readCback = attsReadCback;
    generic_access_service.service.writeCback = attsWriteCback;

    // bind attributes to the service
    generic_access_service.service.pAttr = generic_access_service.attributes;

    attsAttr_t* current_attribute = generic_access_service.attributes;

    // service attribute
    current_attribute->pUuid = attPrimSvcUuid;
    current_attribute->pValue = (uint8_t*) attGapSvcUuid;
    current_attribute->maxLen = sizeof(attGapSvcUuid);
    current_attribute->pLen = &current_attribute->maxLen;
    current_attribute->settings = 0;
    current_attribute->permissions = ATTS_PERMIT_READ;

    // device name declaration
    currentHandle += 2; // note: incremented by two to get a pointer to the value handle
    ++current_attribute;

    // set properties
    generic_access_service.device_name_declaration_value[0] = ATT_PROP_READ;
    // set value handle
    memcpy(generic_access_service.device_name_declaration_value + 1, &currentHandle, sizeof(currentHandle));
    // set the characteristic UUID
    memcpy(generic_access_service.device_name_declaration_value + 3, attDnChUuid, sizeof(attDnChUuid));

    current_attribute->pUuid = attChUuid;
    current_attribute->pValue = generic_access_service.device_name_declaration_value;
    current_attribute->maxLen = sizeof(generic_access_service.device_name_declaration_value);
    current_attribute->pLen = &current_attribute->maxLen;
    current_attribute->settings = 0;
    current_attribute->permissions = ATTS_PERMIT_READ;

    // device name value
    ++current_attribute;
    generic_access_service.device_name_length = 0;
    current_attribute->pUuid = attDnChUuid;
    current_attribute->maxLen = 248;
    current_attribute->pLen = &generic_access_service.device_name_length;
    current_attribute->pValue = NULL;
    current_attribute->settings = ATTS_SET_VARIABLE_LEN;
    current_attribute->permissions = ATTS_PERMIT_READ;

    // appearance declaration
    currentHandle += 2; // note: incremented by two to get a pointer to the value handle
    ++current_attribute;

    // set properties
    generic_access_service.appearance_declaration_value[0] = ATT_PROP_READ;
    // set value handle
    memcpy(generic_access_service.appearance_declaration_value + 1, &currentHandle, sizeof(currentHandle));
    // set the characteristic UUID
    memcpy(generic_access_service.appearance_declaration_value + 3, attApChUuid, sizeof(attApChUuid));

    current_attribute->pUuid = attChUuid;
    current_attribute->pValue = generic_access_service.appearance_declaration_value;
    current_attribute->maxLen = sizeof(generic_access_service.appearance_declaration_value);
    current_attribute->pLen = &current_attribute->maxLen;
    current_attribute->settings = 0;
    current_attribute->permissions = ATTS_PERMIT_READ;

    // appearance value
    ++current_attribute;
    generic_access_service.appearance = 0; // unknown appearance
    current_attribute->pUuid = attApChUuid;
    current_attribute->maxLen = sizeof(generic_access_service.appearance);
    current_attribute->pLen = &current_attribute->maxLen;
    current_attribute->pValue = (uint8_t*) &generic_access_service.appearance;
    current_attribute->settings = 0;
    current_attribute->permissions = ATTS_PERMIT_READ;


    // peripheral prefered connection parameters declaration
    currentHandle += 2; // note: incremented by two to get a pointer to the value handle
    ++current_attribute;

    // set properties
    generic_access_service.ppcp_declaration_value[0] = ATT_PROP_READ;
    // set value handle
    memcpy(generic_access_service.ppcp_declaration_value + 1, &currentHandle, sizeof(currentHandle));
    // set the characteristic UUID
    memcpy(generic_access_service.ppcp_declaration_value + 3, attPpcpChUuid, sizeof(attPpcpChUuid));

    current_attribute->pUuid = attChUuid;
    current_attribute->pValue = generic_access_service.ppcp_declaration_value;
    current_attribute->maxLen = sizeof(generic_access_service.ppcp_declaration_value);
    current_attribute->pLen = &current_attribute->maxLen;
    current_attribute->settings = 0;
    current_attribute->permissions = ATTS_PERMIT_READ;

    // peripheral prefered connection parameters value
    ++current_attribute;
    const uint8_t default_ppcp_value[] = {
        0xFF, 0xFF, // no specific min connection interval
        0xFF, 0xFF, // no specific max connection interval
        0x00, 0x00, // no slave latency
        0xFF, 0xFF // no specific connection supervision timeout
    };
    memcpy(&generic_access_service.ppcp, default_ppcp_value, sizeof(default_ppcp_value));
    current_attribute->pUuid = attPpcpChUuid;
    current_attribute->maxLen = sizeof(generic_access_service.ppcp);
    current_attribute->pLen = &current_attribute->maxLen;
    current_attribute->pValue = generic_access_service.ppcp;
    current_attribute->settings = 0;
    current_attribute->permissions = ATTS_PERMIT_READ;

    generic_access_service.service.endHandle = currentHandle;
    AttsAddGroup(&generic_access_service.service);
}

void GattServer::add_generic_attribute_service()
{
    ++currentHandle;
    generic_attribute_service.service.pNext = NULL;
    generic_attribute_service.service.startHandle = currentHandle;
    generic_attribute_service.service.readCback = attsReadCback;
    generic_attribute_service.service.writeCback = attsWriteCback;

    // bind attributes to the service
    generic_attribute_service.service.pAttr = generic_attribute_service.attributes;

    attsAttr_t* current_attribute = generic_attribute_service.attributes;

    // service attribute
    current_attribute->pUuid = attPrimSvcUuid;
    current_attribute->pValue = (uint8_t*) attGattSvcUuid;
    current_attribute->maxLen = sizeof(attGattSvcUuid);
    current_attribute->pLen = &current_attribute->maxLen;
    current_attribute->settings = 0;
    current_attribute->permissions = ATTS_PERMIT_READ;

    // service changed declaration
    currentHandle += 2; // note: incremented by two to get a pointer to the value handle
    ++current_attribute;

    // set properties
    generic_attribute_service.service_changed_declaration[0] = ATT_PROP_INDICATE;
    // set value handle
    memcpy(generic_attribute_service.service_changed_declaration + 1, &currentHandle, sizeof(currentHandle));
    // set the characteristic UUID
    memcpy(generic_attribute_service.service_changed_declaration + 3, attScChUuid, sizeof(attScChUuid));

    current_attribute->pUuid = attChUuid;
    current_attribute->pValue = generic_attribute_service.service_changed_declaration;
    current_attribute->maxLen = sizeof(generic_attribute_service.service_changed_declaration);
    current_attribute->pLen = &current_attribute->maxLen;
    current_attribute->settings = 0;
    current_attribute->permissions = ATTS_PERMIT_READ;

    // service changed value
    ++current_attribute;
    current_attribute->pUuid = attScChUuid;
    current_attribute->maxLen = 0;
    current_attribute->pLen = &current_attribute->maxLen;
    current_attribute->pValue = NULL;
    current_attribute->settings = 0;
    current_attribute->permissions = 0;

    // CCCD
    ++current_attribute;
    current_attribute->pUuid = attCliChCfgUuid;
    current_attribute->pValue = (uint8_t*)&cccValues[cccCnt];
    current_attribute->maxLen = 2;
    current_attribute->pLen = &current_attribute->maxLen;
    current_attribute->settings = ATTS_SET_CCC;
    current_attribute->permissions = (ATTS_PERMIT_READ | ATTS_PERMIT_WRITE);

    cccSet[cccCnt].handle = currentHandle;
    cccSet[cccCnt].valueRange = ATT_CLIENT_CFG_INDICATE;
    cccSet[cccCnt].secLevel = DM_SEC_LEVEL_NONE;
    cccHandles[cccCnt] = currentHandle - 1;
    cccCnt++;

    generic_attribute_service.service.endHandle = currentHandle;
    AttsAddGroup(&generic_attribute_service.service);
    AttsCccRegister(cccCnt, (attsCccSet_t*)cccSet, cccCback);
}

void* GattServer::alloc_block(size_t block_size) {
    alloc_block_t* block = (alloc_block_t*) malloc(sizeof(alloc_block_t) + block_size);
    if (block == NULL) {
        return NULL;
    }

    if (allocated_blocks) {
        block->next = allocated_blocks;
    } else {
        block->next = NULL;
    }

    allocated_blocks = block;

    return block->data;
}

GattServer::GattServer() :
    ::GattServer(), _signing_event_handler(NULL), cccSet(), cccValues(), cccHandles(), cccCnt(0),
    generic_access_service(), generic_attribute_service(),
    registered_service(NULL), allocated_blocks(NULL),
    currentHandle(0)
{
}

} // namespace cordio
} // namespace vendor
} // namespace ble
