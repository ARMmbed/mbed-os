/* mbed Microcontroller Library
 * Copyright (c) 2006-2013 ARM Limited
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

#include "custom_helper.h"

/*
 * The current version of the soft-device doesn't handle duplicate 128-bit UUIDs
 * very  well. It is therefore necessary to filter away duplicates before
 * passing long UUIDs to sd_ble_uuid_vs_add(). The following types and data
 * structures involved in maintaining a local cache of 128-bit UUIDs.
 */
typedef struct {
    UUID::LongUUIDBytes_t uuid;
    uint8_t         type;
} converted_uuid_table_entry_t;
static const unsigned UUID_TABLE_MAX_ENTRIES = 4; /* This is the maximum number of 128-bit UUIDs with distinct bases that
                                                   * we expect to be in use; increase this limit if needed. */
static unsigned uuidTableEntries = 0; /* current usage of the table */
converted_uuid_table_entry_t convertedUUIDTable[UUID_TABLE_MAX_ENTRIES];

/**
 * lookup the cache of previously converted 128-bit UUIDs to find a type value.
 * @param  uuid          base 128-bit UUID
 * @param  recoveredType the type field of the 3-byte nRF's uuid.
 * @return               true if a match is found.
 */
static bool
lookupConvertedUUIDTable(const UUID::LongUUIDBytes_t uuid, uint8_t *recoveredType)
{
    unsigned i;
    for (i = 0; i < uuidTableEntries; i++) {
        unsigned byteIndex;
        for (byteIndex = 0; byteIndex < UUID::LENGTH_OF_LONG_UUID; byteIndex++) {
            /* Skip bytes 2 and 3, because they contain the shortUUID (16-bit) version of the
             * long UUID; and we're comparing against the remainder. */
            if ((byteIndex == 2) || (byteIndex == 3)) {
                continue;
            }

            if (convertedUUIDTable[i].uuid[byteIndex] != uuid[byteIndex]) {
                break;
            }
        }

        if (byteIndex == UUID::LENGTH_OF_LONG_UUID) {
            *recoveredType = convertedUUIDTable[i].type;
            return true;
        }
    }

    return false;
}

static void
addToConvertedUUIDTable(const UUID::LongUUIDBytes_t uuid, uint8_t type)
{
    if (uuidTableEntries == UUID_TABLE_MAX_ENTRIES) {
        return; /* recovery needed; or at least the user should be warned about this fact.*/
    }

    memcpy(convertedUUIDTable[uuidTableEntries].uuid, uuid, UUID::LENGTH_OF_LONG_UUID);
    convertedUUIDTable[uuidTableEntries].uuid[2] = 0;
    convertedUUIDTable[uuidTableEntries].uuid[3] = 0;
    convertedUUIDTable[uuidTableEntries].type    = type;
    uuidTableEntries++;
}

/**
 * The nRF transport has its own 3-byte representation of a UUID. If the user-
 * specified UUID is 128-bits wide, then the UUID base needs to be added to the
 * soft-device and converted to a 3-byte handle before being used further. This
 * function is responsible for this translation of user-specified UUIDs into
 * nRF's representation.
 *
 * @param[in]  uuid
 *                 user-specified UUID
 * @return nRF
 *              3-byte UUID (containing a type and 16-bit UUID) representation
 *              to be used with SVC calls.
 */
ble_uuid_t custom_convert_to_nordic_uuid(const UUID &uuid)
{
    ble_uuid_t nordicUUID;
    nordicUUID.uuid = uuid.getShortUUID();
    nordicUUID.type = BLE_UUID_TYPE_UNKNOWN; /* to be set below */

    if (uuid.shortOrLong() == UUID::UUID_TYPE_SHORT) {
        nordicUUID.type = BLE_UUID_TYPE_BLE;
    } else {
        if (!lookupConvertedUUIDTable(uuid.getBaseUUID(), &nordicUUID.type)) {
            nordicUUID.type = custom_add_uuid_base(uuid.getBaseUUID());
            addToConvertedUUIDTable(uuid.getBaseUUID(), nordicUUID.type);
        }
    }

    return nordicUUID;
}

/**************************************************************************/
/*!
    @brief      Adds the base UUID to the custom service. All UUIDs used
                by this service are based on this 128-bit UUID.

    @note       This UUID needs to be added to the SoftDevice stack before
                adding the service's primary service via
                'sd_ble_gatts_service_add'

    @param[in]  p_uuid_base   A pointer to the 128-bit UUID array (8*16)

    @returns    The UUID type.
                A return value of 0 should be considered an error.

    @retval     0x00    BLE_UUID_TYPE_UNKNOWN
    @retval     0x01    BLE_UUID_TYPE_BLE
    @retval     0x02    BLE_UUID_TYPE_VENDOR_BEGIN

    @section EXAMPLE
    @code

    // Take note that bytes 2/3 are blank since these are used to identify
    // the primary service and individual characteristics
    #define CFG_CUSTOM_UUID_BASE  "\x6E\x40\x00\x00\xB5\xA3\xF3\x93\xE0\xA9\xE5\x0E\x24\xDC\xCA\x9E"

    uint8_t uuid_type = custom_add_uuid_base(CFG_CUSTOM_UUID_BASE);
    ASSERT_TRUE(uuid_type > 0, ERROR_NOT_FOUND);

    // We can now safely add the primary service and any characteristics
    // for our custom service ...

    @endcode
*/
/**************************************************************************/
uint8_t custom_add_uuid_base(uint8_t const *const p_uuid_base)
{
    ble_uuid128_t base_uuid;
    uint8_t       uuid_type = 0;

    for (unsigned i = 0; i < UUID::LENGTH_OF_LONG_UUID; i++) {
        base_uuid.uuid128[i] = p_uuid_base[i];
    }

    ASSERT_INT( ERROR_NONE, sd_ble_uuid_vs_add( &base_uuid, &uuid_type ), 0);

    return uuid_type;
}

/**************************************************************************/
/*!

*/
/**************************************************************************/
error_t custom_decode_uuid_base(uint8_t const *const p_uuid_base,
                                ble_uuid_t          *p_uuid)
{
    UUID::LongUUIDBytes_t uuid_base_le;

    for (uint8_t i = 0; i < UUID::LENGTH_OF_LONG_UUID; i++) {
        uuid_base_le[i] = p_uuid_base[i];
    }

    ASSERT_STATUS( sd_ble_uuid_decode(UUID::LENGTH_OF_LONG_UUID, uuid_base_le, p_uuid));

    return ERROR_NONE;
}

/**************************************************************************/
/*!
    @brief      Adds a new characteristic to the custom service, assigning
                properties, a UUID add-on value, etc.

    @param[in]  service_handle
    @param[in]  p_uuid            The 16-bit value to add to the base UUID
                                  for this characteristic (normally >1
                                  since 1 is typically used by the primary
                                  service).
    @param[in]  char_props        The characteristic properties, as
                                  defined by ble_gatt_char_props_t
    @param[in]  max_length        The maximum length of this characeristic
    @param[in]  has_variable_len  Whether the characteristic data has
                                  variable length.
    @param[out] p_char_handle

    @returns
    @retval     ERROR_NONE        Everything executed normally
*/
/**************************************************************************/
error_t custom_add_in_characteristic(uint16_t                  service_handle,
                                     ble_uuid_t               *p_uuid,
                                     uint8_t                   properties,
                                     SecurityManager::SecurityMode_t       requiredSecurity,
                                     uint8_t                  *p_data,
                                     uint16_t                  length,
                                     uint16_t                  max_length,
                                     bool                      has_variable_len,
                                     const uint8_t            *userDescriptionDescriptorValuePtr,
                                     uint16_t                  userDescriptionDescriptorValueLen,
                                     bool                      readAuthorization,
                                     bool                      writeAuthorization,
                                     ble_gatts_char_handles_t *p_char_handle)
{
    /* Characteristic metadata */
    ble_gatts_attr_md_t   cccd_md;
    ble_gatt_char_props_t char_props;

    memcpy(&char_props, &properties, 1);

    if (char_props.notify || char_props.indicate) {
        /* Notification requires cccd */
        memclr_( &cccd_md, sizeof(ble_gatts_attr_md_t));
        cccd_md.vloc = BLE_GATTS_VLOC_STACK;
        BLE_GAP_CONN_SEC_MODE_SET_OPEN(&cccd_md.read_perm);
        BLE_GAP_CONN_SEC_MODE_SET_OPEN(&cccd_md.write_perm);
    }

    ble_gatts_char_md_t char_md = {0};

    char_md.char_props = char_props;
    char_md.p_cccd_md  =
        (char_props.notify || char_props.indicate) ? &cccd_md : NULL;
    if ((userDescriptionDescriptorValueLen > 0) && (userDescriptionDescriptorValuePtr != NULL)) {
        char_md.p_char_user_desc        = const_cast<uint8_t *>(userDescriptionDescriptorValuePtr);
        char_md.char_user_desc_max_size = userDescriptionDescriptorValueLen;
        char_md.char_user_desc_size     = userDescriptionDescriptorValueLen;
    }

    /* Attribute declaration */
    ble_gatts_attr_md_t attr_md = {0};

    attr_md.rd_auth = readAuthorization;
    attr_md.wr_auth = writeAuthorization;

    attr_md.vloc = BLE_GATTS_VLOC_STACK;
    /* Always set variable size */
    attr_md.vlen = has_variable_len;

    if (char_props.read || char_props.notify || char_props.indicate) {
        switch (requiredSecurity) {
            case SecurityManager::SECURITY_MODE_ENCRYPTION_OPEN_LINK :
                BLE_GAP_CONN_SEC_MODE_SET_OPEN(&attr_md.read_perm);
                break;
            case SecurityManager::SECURITY_MODE_ENCRYPTION_NO_MITM :
                BLE_GAP_CONN_SEC_MODE_SET_ENC_NO_MITM(&attr_md.read_perm);
                break;
            case SecurityManager::SECURITY_MODE_ENCRYPTION_WITH_MITM :
                BLE_GAP_CONN_SEC_MODE_SET_ENC_WITH_MITM(&attr_md.read_perm);
                break;
            case SecurityManager::SECURITY_MODE_SIGNED_NO_MITM :
                BLE_GAP_CONN_SEC_MODE_SET_SIGNED_NO_MITM(&attr_md.read_perm);
                break;
            case SecurityManager::SECURITY_MODE_SIGNED_WITH_MITM :
                BLE_GAP_CONN_SEC_MODE_SET_SIGNED_WITH_MITM(&attr_md.read_perm);
                break;
            default:
                break;
        };
    }

    if (char_props.write || char_props.write_wo_resp) {
        switch (requiredSecurity) {
            case SecurityManager::SECURITY_MODE_ENCRYPTION_OPEN_LINK :
                BLE_GAP_CONN_SEC_MODE_SET_OPEN(&attr_md.write_perm);
                break;
            case SecurityManager::SECURITY_MODE_ENCRYPTION_NO_MITM :
                BLE_GAP_CONN_SEC_MODE_SET_ENC_NO_MITM(&attr_md.write_perm);
                break;
            case SecurityManager::SECURITY_MODE_ENCRYPTION_WITH_MITM :
                BLE_GAP_CONN_SEC_MODE_SET_ENC_WITH_MITM(&attr_md.write_perm);
                break;
            case SecurityManager::SECURITY_MODE_SIGNED_NO_MITM :
                BLE_GAP_CONN_SEC_MODE_SET_SIGNED_NO_MITM(&attr_md.write_perm);
                break;
            case SecurityManager::SECURITY_MODE_SIGNED_WITH_MITM :
                BLE_GAP_CONN_SEC_MODE_SET_SIGNED_WITH_MITM(&attr_md.write_perm);
                break;
            default:
                break;
        };
    }

    ble_gatts_attr_t attr_char_value = {0};

    attr_char_value.p_uuid    = p_uuid;
    attr_char_value.p_attr_md = &attr_md;
    attr_char_value.init_len  = length;
    attr_char_value.max_len   = max_length;
    attr_char_value.p_value   = p_data;

    ASSERT_STATUS ( sd_ble_gatts_characteristic_add(service_handle,
                                                    &char_md,
                                                    &attr_char_value,
                                                    p_char_handle));

    return ERROR_NONE;
}



/**************************************************************************/
/*!
    @brief      Adds a new descriptor to the custom service, assigning
                value, a UUID add-on value, etc.

    @param[in]  char_handle
    @param[in]  p_uuid            The 16-bit value to add to the base UUID
                                  for this descriptor (normally >1
                                  since 1 is typically used by the primary
                                  service).
    @param[in]  max_length        The maximum length of this descriptor
    @param[in]  has_variable_len  Whether the characteristic data has
                                  variable length.

    @returns
    @retval     ERROR_NONE        Everything executed normally
*/
/**************************************************************************/
error_t custom_add_in_descriptor(uint16_t    char_handle,
                                 ble_uuid_t *p_uuid,
                                 uint8_t    *p_data,
                                 uint16_t    length,
                                 uint16_t    max_length,
                                 bool        has_variable_len,
                                 uint16_t   *p_desc_handle)
{
    /* Descriptor metadata */
    ble_gatts_attr_md_t   desc_md = {0};

    desc_md.vloc = BLE_GATTS_VLOC_STACK;
    /* Always set variable size */
    desc_md.vlen = has_variable_len;

    /* Make it readable and writable */
    BLE_GAP_CONN_SEC_MODE_SET_OPEN(&desc_md.read_perm);
    BLE_GAP_CONN_SEC_MODE_SET_OPEN(&desc_md.write_perm);

    ble_gatts_attr_t attr_desc = {0};

    attr_desc.p_uuid    = p_uuid;
    attr_desc.p_attr_md = &desc_md;
    attr_desc.init_len  = length;
    attr_desc.max_len   = max_length;
    attr_desc.p_value   = p_data;

    ASSERT_STATUS ( sd_ble_gatts_descriptor_add(char_handle,
                                                &attr_desc,
                                                p_desc_handle));

    return ERROR_NONE;
}
