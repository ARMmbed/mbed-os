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

#include <new>

#include "nRF5xPalGattClient.h"

#include "ble/pal/PalGattClient.h"
#include "ble/pal/SimpleAttServerMessage.h"

#include "headers/nrf_ble.h"
#include "headers/ble_gatt.h"
#include "headers/ble_types.h"
#include "headers/ble_err.h"

namespace ble {
namespace pal {
namespace vendor {
namespace nordic {

namespace {

/**
 * Extract an uint16_t value from a memory location.
 */
static uint16_t bytes_to_u16(const void* b)
{
    uint16_t res;
    memcpy(&res, b, sizeof(uint16_t));
    return res;
}

/**
 * Push an uint16_t value into a byte stream.
 *
 * @note it is incremented.
 */
static void u16_to_stream(uint8_t *&it, uint16_t v)
{
    memcpy(it, &v, sizeof(uint16_t));
    it += 2;
}

/**
 * Convert a pal::attribute_handle_range_t into a ble_gattc_handle_range_t.
 */
static ble_gattc_handle_range_t to_nordic_handle_range(const attribute_handle_range_t &range)
{
    ble_gattc_handle_range_t result = {
        range.begin,
        range.end
    };
    return result;
}

/**
 * Convert a ble_gattc_handle_range_t into a pal::attribute_handle_range_t.
 */
static attribute_handle_range_t to_ble_handle_range(const ble_gattc_handle_range_t &range)
{
    attribute_handle_range_t result = {
        range.start_handle,
        range.end_handle
    };
    return result;
}

/**
 * Convert an error from the softdevice into a ble_error_t
 */
static ble_error_t convert_sd_error(uint32_t err)
{
    switch (err) {
        case NRF_SUCCESS:
            return BLE_ERROR_NONE;
        case NRF_ERROR_INVALID_PARAM:
        case BLE_ERROR_INVALID_CONN_HANDLE:
        case NRF_ERROR_INVALID_ADDR:
            return BLE_ERROR_INVALID_PARAM;
        case NRF_ERROR_INVALID_STATE:
            return BLE_ERROR_INVALID_STATE;
        case NRF_ERROR_DATA_SIZE:
            return BLE_ERROR_PARAM_OUT_OF_RANGE;
#if  (NRF_SD_BLE_API_VERSION <= 3) // Removed in Softdevice v4.0
        case BLE_ERROR_NO_TX_PACKETS:
            return BLE_ERROR_NO_MEM;
#endif
        case NRF_ERROR_BUSY:
            return BLE_STACK_BUSY;
        case NRF_ERROR_NO_MEM:
            return BLE_ERROR_NO_MEM;
        default:
            return BLE_ERROR_UNSPECIFIED;
    }
}

/**
 * Convert a UUID into a ble_uuid_t .
 * If the UUID is a 128 bit one then it is registered into the softdevice.
 */
static ble_error_t to_nordic_uuid(const UUID &uuid, ble_uuid_t &nordic_uuid)
{
    if (uuid.getLen() == UUID::LENGTH_OF_LONG_UUID) {
        // first try to get the long UUID in the table of UUIDs
        uint32_t err = sd_ble_uuid_decode(
            uuid.getLen(),
            uuid.getBaseUUID(),
            &nordic_uuid
        );

        // UUID found and filed, return.
        if (err == NRF_SUCCESS) {
            return BLE_ERROR_NONE;
        }

        ble_uuid128_t uuid128;
        memcpy(uuid128.uuid128, uuid.getBaseUUID(), sizeof(uuid128.uuid128));

        // UUID not found, try to register it
        err = sd_ble_uuid_vs_add(&uuid128, &nordic_uuid.type);
        if (err == NRF_SUCCESS) {
            nordic_uuid.uuid = bytes_to_u16(uuid.getBaseUUID() + 12);
        }

        return convert_sd_error(err);
    } else {
        nordic_uuid.type = BLE_UUID_TYPE_BLE;
        nordic_uuid.uuid = uuid.getShortUUID();
        return BLE_ERROR_NONE;
    }
}

/**
 * Convert an attribute error code from the softdevice into a valid ATT error.
 */
static uint8_t convert_sd_att_error_code(uint16_t err)
{
    if (err < 0x101 || err > 0x1FF) {
        return AttErrorResponse::UNLIKELY_ERROR;
    } else {
        return err & 0xFF;
    }
}

static const size_t long_uuid_length = 16;
static const size_t read_by_group_type_long_uuid_index = 4;
static const size_t characteristic_declaration_length = 1 + 2 + 16;

} // end of anonymous namespace

nRF5xGattClient::nRF5xGattClient() :
    ble::pal::GattClient(),
    _procedures()
{
}

nRF5xGattClient::~nRF5xGattClient()
{
    terminate();
}

ble_error_t nRF5xGattClient::initialize()
{
    return BLE_ERROR_NONE;
}

ble_error_t nRF5xGattClient::exchange_mtu(connection_handle_t connection)
{
    // FIXME: implement when SD 140 5.x.x is present
    // (see sd_ble_gatts_exchange_mtu_reply)
    return BLE_ERROR_NOT_IMPLEMENTED;
}

ble_error_t nRF5xGattClient::get_mtu_size(
    connection_handle_t connection_handle, uint16_t& mtu_size
) {
#if (NRF_SD_BLE_API_VERSION >= 3)
    // FIXME: implement when MTU size can be configured; the mtu size must be
    // stored locally when BLE_GATTC_EVT_EXCHANGE_MTU_RSP has been received
    mtu_size = BLE_GATT_MTU_SIZE_DEFAULT;
#else
    mtu_size = GATT_RX_MTU;
#endif
    return BLE_ERROR_NONE;
}

ble_error_t nRF5xGattClient::discover_primary_service(
    connection_handle_t connection,
    attribute_handle_t discovery_range_begining
) {
    return launch_procedure<DiscoverPrimaryServiceProcedure>(
        connection, discovery_range_begining
    );
}

ble_error_t nRF5xGattClient::discover_primary_service_by_service_uuid(
    connection_handle_t connection_handle,
    attribute_handle_t discovery_range_beginning,
    const UUID& uuid
) {
    return launch_procedure<DiscoverPrimaryServiceByUUIDProcedure>(
        connection_handle, discovery_range_beginning, uuid
    );
}

ble_error_t nRF5xGattClient::find_included_service(
    connection_handle_t connection_handle,
    attribute_handle_range_t service_range
) {
    return launch_procedure<FindIncludedServicesProcedure>(
        connection_handle, service_range
    );
}

ble_error_t nRF5xGattClient::discover_characteristics_of_a_service(
    connection_handle_t connection_handle,
    attribute_handle_range_t discovery_range
) {
    return launch_procedure<DiscoverCharacteristicsProcedure>(
        connection_handle, discovery_range
    );
}

ble_error_t nRF5xGattClient::discover_characteristics_descriptors(
    connection_handle_t connection_handle,
    attribute_handle_range_t descriptors_discovery_range
) {
    return launch_procedure<DiscoverDescriptorsProcedure>(
        connection_handle, descriptors_discovery_range
    );
}

ble_error_t nRF5xGattClient::read_attribute_value(
    connection_handle_t connection_handle,
    attribute_handle_t attribute_handle
) {
    return launch_procedure<ReadAttributeProcedure>(
        connection_handle, attribute_handle
    );
}

ble_error_t nRF5xGattClient::read_using_characteristic_uuid(
    connection_handle_t connection_handle,
    attribute_handle_range_t read_range,
    const UUID& uuid
) {
    return launch_procedure<ReadUsingCharacteristicUUIDProcedure>(
        connection_handle, read_range, uuid
    );
}

ble_error_t nRF5xGattClient::read_attribute_blob(
    connection_handle_t connection,
    attribute_handle_t attribute_handle,
    uint16_t offset
) {
    return launch_procedure<ReadAttributeBlobProcedure>(
        connection, attribute_handle, offset
    );
}

ble_error_t nRF5xGattClient::read_multiple_characteristic_values(
    connection_handle_t connection,
    const ArrayView<const attribute_handle_t>& characteristic_handles
) {
    return launch_procedure<ReadMultipleCharacteristicsProcedure>(
        connection, characteristic_handles
    );
}

ble_error_t nRF5xGattClient::write_without_response(
    connection_handle_t connection_handle,
    attribute_handle_t characteristic_value_handle,
    const ArrayView<const uint8_t>& value
) {
    ble_gattc_write_params_t write_params = {
        BLE_GATT_OP_WRITE_CMD,
        /* exec flags */ 0,
        characteristic_value_handle,
        /* offset */ 0,
        static_cast<uint16_t>(value.size()),
        const_cast<uint8_t*>(value.data())
    };

    uint32_t err = sd_ble_gattc_write(connection_handle, &write_params);
    return convert_sd_error(err);
}

ble_error_t nRF5xGattClient::signed_write_without_response(
    connection_handle_t connection_handle,
    attribute_handle_t characteristic_value_handle,
    const ArrayView<const uint8_t>& value
) {
    ble_gattc_write_params_t write_params = {
        BLE_GATT_OP_SIGN_WRITE_CMD,
        /* exec flags */ 0,
        characteristic_value_handle,
        /* offset */ 0,
        static_cast<uint16_t>(value.size()),
        const_cast<uint8_t*>(value.data())
    };

    uint32_t err = sd_ble_gattc_write(connection_handle, &write_params);
    return convert_sd_error(err);
}

ble_error_t nRF5xGattClient::write_attribute(
    connection_handle_t connection_handle,
    attribute_handle_t attribute_handle,
    const ArrayView<const uint8_t>& value
) {
    return launch_procedure<WriteAttributeProcedure>(
        connection_handle, attribute_handle, value
    );
}

ble_error_t nRF5xGattClient::queue_prepare_write(
    connection_handle_t connection_handle,
    attribute_handle_t characteristic_value_handle,
    const ArrayView<const uint8_t>& value,
    uint16_t offset
) {
    return launch_procedure<QueuePrepareWriteProcedure>(
        connection_handle, characteristic_value_handle, value, offset
    );
}

ble_error_t nRF5xGattClient::execute_write_queue(
    connection_handle_t connection_handle,
    bool execute
) {
    return launch_procedure<ExecuteWriteQueueProcedure>(
        connection_handle, execute
    );
}


/**
 * Base definition of a GATT procedure.
 *
 * Nordic implementation of discovery procedures requires more than a single
 * request to the BLE peer when it involves 128bit UUID. As a consequence it is
 * necessary to reserve the connection slot, maintain state between requests and
 * interpret responses depending on the context.
 *
 * The concept of procedures for Gatt operations formalize the process. A
 * procedure lifecycle is defined by three function:
 *   - start: Launch of the procedure. It initiate the first request to send to
 *     the peer. It must be implemented by Procedure derived classes.
 *   - handle: Event handler that process ble events comming from the softdevice.
 *     This function drive the procedure flow and must be implemented by
 *     Procedure derived classes.
 *   - terminate: end the procedure and forward the result to the GattClient
 *     event handle.
 *
 * @note Commands such as write without response or signed write without response
 * are not procedures.
 */
struct nRF5xGattClient::GattProcedure {
    /**
     * Initialize the procedure.
     *
     * @param connection Handle representing the connection used by the procedure.
     * @param op Opcode of the procedure.
     */
    GattProcedure(connection_handle_t connection, AttributeOpcode op) :
        connection_handle(connection), procedure_opcode(op) { }

    /**
     * To overide in child if necessary.
     */
    virtual ~GattProcedure() { }

    /**
     * Handle events targeting the connection.
     */
    virtual void handle(const ble_evt_t &evt) = 0;

    /**
     * Terminate the execution of the procedure and forward msg to the handler
     * registered in the client.
     *
     * @note delete this.
     */
    void terminate(const AttServerMessage &msg)
    {
        get_client().remove_procedure(this);
        get_client().on_server_event(connection_handle, msg);
        delete this;
    }

    /**
     * Terminate the procedure with an unlikely error.
     */
    void abort()
    {
        terminate(AttErrorResponse(
            procedure_opcode, AttErrorResponse::UNLIKELY_ERROR
        ));
    }

    const connection_handle_t connection_handle;
    const AttributeOpcode procedure_opcode;
};


/**
 * A regular procedure is a procedure that follows Gatt specification.
 *
 * It initiate a single request to the peer and excepts a single response. This
 * kind of procedure doesn't requires extra processing step.
 *
 * Given that such procedure expects a single event type from the soft device,
 * error handling can be generalized.
 */
struct nRF5xGattClient::RegularGattProcedure : GattProcedure {

    /**
     * Construct a RegularGattProcedure.
     *
     * @param connection Handle of the connection used by the procedure.
     * @param op Attribute operation code
     * @param event_type Type of event expected by the stack.
     */
    RegularGattProcedure(
        connection_handle_t connection, AttributeOpcode op, BLE_GATTC_EVTS event_type
    ) : GattProcedure(connection, op), _event_type(event_type) { }

    /**
     * Handle events from the BLE stack; do not overide in child.
     *
     * @note This function offload error handling from user error handler. If
     * the event in input match the expected event type and does not carry error
     * then it is forwarded to the do_handle function. Otherwise the procedure
     * is terminated and an error is forwarded to the client event handler.
     */
    virtual void handle(const ble_evt_t &evt)
    {
        if (evt.header.evt_id == _event_type) {
            const ble_gattc_evt_t &gattc_evt = evt.evt.gattc_evt;
            if (gattc_evt.gatt_status != BLE_GATT_STATUS_SUCCESS) {
                terminate(AttErrorResponse(
                    procedure_opcode,
                    gattc_evt.error_handle,
                    convert_sd_att_error_code(gattc_evt.gatt_status)
                ));
                return;
            } else {
                do_handle(gattc_evt);
            }
        } else {
            abort();
        }
    }

    /**
     * Handle gatt event received from the stack.
     *
     * @note The event passed in parameter is valid.
     */
    virtual void do_handle(const ble_gattc_evt_t &evt) = 0;

protected:
    BLE_GATTC_EVTS _event_type;
};


/**
 * Procedure that handle discovery of primary services.
 *
 * The softdevice doesn't forward to user UUID of services that are 128 bit long.
 * In such case a read request is issued for each service attribute handle
 * to extract that information.
 */
struct nRF5xGattClient::DiscoverPrimaryServiceProcedure : GattProcedure {

    typedef ArrayView<const ble_gattc_service_t> services_array_t;

    DiscoverPrimaryServiceProcedure(connection_handle_t connection) :
        GattProcedure(connection, AttributeOpcode::READ_BY_GROUP_TYPE_REQUEST),
        response(NULL), count(0), idx(0) { }

    virtual ~DiscoverPrimaryServiceProcedure()
    {
        if (response) {
            delete[] response;
        }
    }

    ble_error_t start(attribute_handle_t begining)
    {
        uint32_t err = sd_ble_gattc_primary_services_discover(
            connection_handle, begining, /* p_srvc_uuid */ NULL
        );
        return convert_sd_error(err);
    }

    /**
     * Dispatch responses either to service discovery handler or attribute read
     * handler.
     */
    virtual void handle(const ble_evt_t &evt)
    {
        switch (evt.header.evt_id) {
            case BLE_GATTC_EVT_PRIM_SRVC_DISC_RSP:
                handle_service_discovered(evt.evt.gattc_evt);
                return;
            case BLE_GATTC_EVT_READ_RSP:
                handle_uuid_read(evt.evt.gattc_evt);
                return;
            default:
                abort();
                return;
        }
    }

    /**
     * Dispatch service discovery response either to the short UUID handler or
     * the long UUID handler.
     */
    void handle_service_discovered(const ble_gattc_evt_t &evt)
    {
        if (evt.gatt_status != BLE_GATT_STATUS_SUCCESS) {
            terminate(AttErrorResponse(
                AttributeOpcode::READ_BY_GROUP_TYPE_REQUEST,
                convert_sd_att_error_code(evt.gatt_status)
            ));
            return;
        }

        services_array_t services(
            evt.params.prim_srvc_disc_rsp.services,
            evt.params.prim_srvc_disc_rsp.count
        );

        // note 128 bit and 16 bits UUID cannot be mixed up
        if (services[0].uuid.type == BLE_UUID_TYPE_BLE) {
            handle_16bit_services_discovered(services);
        } else {
            handle_128bit_services_discovered(services);
        }
    }

    /**
     * Handle discovery of services with a 16 bit UUID.
     *
     * The procedure ends here.
     */
    void handle_16bit_services_discovered(const services_array_t &services)
    {
        /**
         * Custom implementation of AttReadByGroupTypeResponse that can be used
         * with data returned by the nordic stack.
         */
        struct CustomAttReadByGroupTypeResponse : AttReadByGroupTypeResponse {
            CustomAttReadByGroupTypeResponse(const services_array_t &s) :
                AttReadByGroupTypeResponse(), services(s) {
            }

            virtual size_t size() const
            {
                return services.size();
            }

            virtual attribute_data_t operator[](size_t i) const
            {
                attribute_data_t result = {
                    to_ble_handle_range(services[i].handle_range),
                    make_const_ArrayView(
                        reinterpret_cast<const uint8_t*>(&services[i].uuid.uuid),
                        sizeof(uint16_t)
                    )
                };
                return result;
            }

            const services_array_t &services;
        };

        terminate(CustomAttReadByGroupTypeResponse(services));
    }

    /**
     * Handle discovery of services with a 128 UUID.
     *
     * Handle of the services discovered will be stored locally then the
     * procedure sequentially initiate a read request of each of these attribute
     * handle to acquire the value of the UUID of the service.
     *
     * The procedure ends once all the informations initially sent by the peer
     * has been reconstructed and forwarded to the registered client handler.
     */
    void handle_128bit_services_discovered(const services_array_t &services)
    {
        response = new(std::nothrow) packed_discovery_response_t[services.size()];
        if (!response) {
            abort();
            return;
        }

        count = services.size();
        idx = 0;
        for (size_t i = 0; i < count; ++i) {
            uint8_t *it = &response[i][0];
            u16_to_stream(it, services[i].handle_range.start_handle);
            u16_to_stream(it, services[i].handle_range.end_handle);
        }

        read_service_uuid();
    }

    /**
     * Initiate the read request of the next service attribute handle.
     */
    void read_service_uuid(void)
    {
        // note: use read multiple once different mtu size are supported ?
        uint16_t attribute_handle = bytes_to_u16(&response[idx][0]);

        uint32_t err = sd_ble_gattc_read(connection_handle, attribute_handle, 0);
        if (err) {
            abort();
        }
    }

    /**
     * Handle reception of a service (long) UUID.
     */
    void handle_uuid_read(const ble_gattc_evt_t &evt)
    {
        if (evt.gatt_status != BLE_GATT_STATUS_SUCCESS) {
            terminate(AttErrorResponse(
                AttributeOpcode::READ_REQUEST,
                convert_sd_att_error_code(evt.gatt_status)
            ));
            return;
        }

        const ble_gattc_evt_read_rsp_t &rsp = evt.params.read_rsp;

        uint16_t expected_handle = bytes_to_u16(&response[idx][0]);

        if (rsp.handle != expected_handle || rsp.offset != 0 ||
            rsp.len != long_uuid_length) {
            abort();
            return;
        }

        memcpy(&response[idx][read_by_group_type_long_uuid_index], rsp.data, rsp.len);

        ++idx;

        if (idx == count) {
            terminate(SimpleAttReadByGroupTypeResponse(
                sizeof(packed_discovery_response_t),
                make_const_ArrayView(
                    reinterpret_cast<const uint8_t*>(response),
                    count * sizeof(packed_discovery_response_t)
                ))
            );
        } else {
            read_service_uuid();
        }
    }

    // Hold read by group type response of services with 128 bit UUID.
    // The response is composed of the service attribute handle (2 bytes), the
    // end group handle (2 bytes) and the service UUID (16 bytes).
    typedef uint8_t packed_discovery_response_t[20];

    packed_discovery_response_t* response;
    uint16_t count;
    uint16_t idx;
};


/**
 * Procedure that manage Discover Primary Service by Service UUID transactions.
 *
 * @note Even if the softdevice doesn't forward the complete content of the peer
 * response it is possible to reconstruct it by keeping a copy of the UUID to
 * find.
 */
struct nRF5xGattClient::DiscoverPrimaryServiceByUUIDProcedure : RegularGattProcedure {

    typedef ArrayView<const ble_gattc_service_t> services_array_t;

    DiscoverPrimaryServiceByUUIDProcedure(connection_handle_t connection) :
        RegularGattProcedure(
            connection,
            AttributeOpcode::FIND_BY_TYPE_VALUE_REQUEST,
            BLE_GATTC_EVT_PRIM_SRVC_DISC_RSP
        ),
        _service_uuid() { }

    ble_error_t start(attribute_handle_t begining, const UUID &uuid)
    {
        ble_uuid_t nordic_uuid;
        ble_error_t ble_err = to_nordic_uuid(uuid, nordic_uuid);
        if (ble_err) {
            return ble_err;
        }

        uint32_t err = sd_ble_gattc_primary_services_discover(
            connection_handle, begining, &nordic_uuid
        );
        if (!err) {
            _service_uuid = uuid;
        }

        return convert_sd_error(err);
    }

    virtual void do_handle(const ble_gattc_evt_t &evt)
    {
        services_array_t services(
            evt.params.prim_srvc_disc_rsp.services,
            evt.params.prim_srvc_disc_rsp.count
        );

        /**
         * Implementation of AttReadByGroupTypeResponse that addapt Nordic data
         * structure.
         */
        struct CustomAttReadByGroupTypeResponse : AttReadByGroupTypeResponse {
            CustomAttReadByGroupTypeResponse(const services_array_t &s, const UUID &u) :
                AttReadByGroupTypeResponse(), services(s), uuid(u) {
            }

            virtual size_t size() const
            {
                return services.size();
            }

            virtual attribute_data_t operator[](size_t i) const
            {
                attribute_data_t result = {
                    to_ble_handle_range(services[i].handle_range),
                    make_ArrayView(uuid.getBaseUUID(), uuid.getLen())
                };
                return result;
            }

            const services_array_t &services;
            const UUID &uuid;
        };

        terminate(CustomAttReadByGroupTypeResponse(services, _service_uuid));
    }

    UUID _service_uuid;
};


/**
 * Procedure that manage Find Included Services transactions.
 */
struct nRF5xGattClient::FindIncludedServicesProcedure : RegularGattProcedure {

    typedef ArrayView<const ble_gattc_service_t> services_array_t;

    FindIncludedServicesProcedure(connection_handle_t connection) :
        RegularGattProcedure(
            connection,
            AttributeOpcode::READ_BY_TYPE_REQUEST,
            BLE_GATTC_EVT_REL_DISC_RSP
        ) { }

    ble_error_t start(attribute_handle_range_t service_range)
    {
        ble_gattc_handle_range_t range = to_nordic_handle_range(service_range);
        uint32_t err = sd_ble_gattc_relationships_discover(
            connection_handle, &range
        );

        return convert_sd_error(err);
    }

    virtual void do_handle(const ble_gattc_evt_t &evt)
    {
        // recompose the message into a raw att read by type response.
        const ble_gattc_evt_rel_disc_rsp_t &resp = evt.params.rel_disc_rsp;

        bool contain_short_uuid =
            (resp.includes[0].included_srvc.uuid.type == BLE_UUID_TYPE_BLE);

        uint8_t element_size = 6;
        if (contain_short_uuid) {
            element_size += 2;
        }

        // It would be more efficient to use an API like alloca but it is
        // unavailable and unsafe. Another alternative would be to have a fixed
        // size stack buffer since the size of the MTU is fixed however once new
        // softdevices lands could would have to be rewritten because they support
        // variable MTU size.
        size_t buffer_size = element_size * resp.count;
        uint8_t* buffer = new(std::nothrow) uint8_t[buffer_size];
        if (!buffer) {
            abort();
            return;
        }

        uint8_t *it = buffer;
        for(size_t i = 0; i < resp.count; ++i) {
            u16_to_stream(it, resp.includes[i].handle);
            u16_to_stream(it, resp.includes[i].included_srvc.handle_range.start_handle);
            u16_to_stream(it, resp.includes[i].included_srvc.handle_range.end_handle);
            if (contain_short_uuid) {
                u16_to_stream(it, resp.includes[i].included_srvc.uuid.uuid);
            }
        }

        terminate(SimpleAttReadByTypeResponse(
            element_size,
            make_const_ArrayView(buffer, buffer_size)
        ));

        delete[] buffer;
    }

    UUID _service_uuid;
};


/**
 * Procedure that handle Discover All Characteristics of a Service transactions.
 *
 * The softdevice doesn't forward to user UUID of services that are 128 bit long.
 * In such case a read request is issued for each attribute handle of
 * characteristics that exposes a long UUID.
 */
struct nRF5xGattClient::DiscoverCharacteristicsProcedure : GattProcedure {
    /**
     * Data structure returned by the function flatten_response.
     */
    struct read_by_type_response_t {
        uint16_t count;
        uint16_t element_size;
        uint8_t* buffer;
    };

    DiscoverCharacteristicsProcedure(connection_handle_t connection) :
        GattProcedure(connection, AttributeOpcode::READ_BY_TYPE_REQUEST),
        _response(), _idx(0) { }

    virtual ~DiscoverCharacteristicsProcedure()
    {
        if (_response.buffer) {
            delete[] _response.buffer;
        }
    }

    ble_error_t start(attribute_handle_range_t discovery_range)
    {
        ble_gattc_handle_range_t range = to_nordic_handle_range(discovery_range);
        uint32_t err = sd_ble_gattc_characteristics_discover(
            connection_handle,
            &range
        );

        return convert_sd_error(err);
    }

    /**
     * Dispatch ble events to the appropriate handler:
     *   - handle_characteristic_discovered: For a discovery response.
     *   - handle_uuid_read: For a read response.
     */
    virtual void handle(const ble_evt_t &evt)
    {
        switch (evt.header.evt_id) {
            case BLE_GATTC_EVT_CHAR_DISC_RSP:
                handle_characteristic_discovered(evt.evt.gattc_evt);
                return;
            case BLE_GATTC_EVT_READ_RSP:
                handle_uuid_read(evt.evt.gattc_evt);
                return;
            default:
                abort();
                return;
        }
    }

    void handle_characteristic_discovered(const ble_gattc_evt_t &evt)
    {
        if (evt.gatt_status != BLE_GATT_STATUS_SUCCESS) {
            terminate(AttErrorResponse(
                AttributeOpcode::READ_BY_TYPE_REQUEST,
                convert_sd_att_error_code(evt.gatt_status)
            ));
            return;
        }

        // layout the data structure into a flat byte array.
        _response = flatten_response(evt.params.char_disc_rsp);
        if (!_response.buffer) {
            abort();
            return;
        }

        // If element size is equal to 7 then the characteristic discovered
        // have 16 bit UUID. It is not necessary to read their characteristic
        // declaration attribute.
        if (_response.element_size == 7) {
            forward_response_and_terminate();
        } else {
            // 128 bit UUID.
            // read sequentially each characteristic declaration attribute
            // discovered.
            _idx = 0;
            read_characteristic_uuid();
        }
    }

    /**
     * Initiate read of the next characteristic declaration attribute.
     */
    void read_characteristic_uuid(void)
    {
        // note: use read multiple once different mtu size are supported ?
        uint16_t attribute_handle =
            bytes_to_u16(&_response.buffer[_idx * _response.element_size]);

        uint32_t err = sd_ble_gattc_read(connection_handle, attribute_handle, 0);
        if (err) {
            abort();
        }
    }

    /**
     * Handle read response of a characteristic declaration attribute.
     * It add the data in the response then initiate the read of the next
     * attribute or terminate the procedure if all characteristic declaration
     * attributes have been read.
     */
    void handle_uuid_read(const ble_gattc_evt_t &evt)
    {
        // should never happen
        if (!_response.buffer) {
            abort();
            return;
        }

        if (evt.gatt_status != BLE_GATT_STATUS_SUCCESS) {
            terminate(AttErrorResponse(
                AttributeOpcode::READ_REQUEST,
                convert_sd_att_error_code(evt.gatt_status)
            ));
            return;
        }

        const ble_gattc_evt_read_rsp_t &rsp = evt.params.read_rsp;
        uint8_t* current_element = &_response.buffer[_idx * _response.element_size];
        uint16_t expected_handle = bytes_to_u16(current_element);

        if (rsp.handle != expected_handle || rsp.offset != 0 ||
            rsp.len != characteristic_declaration_length) {
            abort();
            return;
        }

        // note: elements are the pair characteristic declaration handle followed
        // by the attribute value.
        memcpy(current_element + 2, rsp.data, rsp.len);

        ++_idx;

        if (_idx == _response.count) {
            forward_response_and_terminate();
        } else {
            read_characteristic_uuid();
        }
    }

    /**
     * Terminate the procedure by forwarding the AttReadByTypeResponse built.
     */
    void forward_response_and_terminate() {
        terminate(SimpleAttReadByTypeResponse(
            _response.element_size,
            make_const_ArrayView(
                _response.buffer,
                _response.element_size * _response.count
            )
        ));
    }

    /**
     * Convert a ble_gattc_evt_char_disc_rsp_t into a raw response.
     *
     * If UUIDs present are 16 bits long then the output contain the whole
     * response. Otherwise only the handle declaration of each characteristic
     * discovered is present and properties, handle value and UUID are populated
     * by reading the attribute handle.
     */
    static read_by_type_response_t flatten_response(const ble_gattc_evt_char_disc_rsp_t& resp)
    {
        read_by_type_response_t result = { resp.count, 0 };

        bool short_uuid = (resp.chars[0].uuid.type == BLE_UUID_TYPE_BLE);

        // att handle + prop + value handle + uuid size
        result.element_size = 5 + (short_uuid ? 2 : 16) ;

        size_t buffer_size = resp.count * result.element_size;
        result.buffer = new(std::nothrow) uint8_t[buffer_size];
        if(!result.buffer) {
            return result;
        }

        uint8_t *it = result.buffer;
        for(size_t i = 0; i < resp.count; ++i) {
            u16_to_stream(it, resp.chars[i].handle_decl);
            if (short_uuid) {
                *it++ = get_properties(resp.chars[i]);
                u16_to_stream(it, resp.chars[i].handle_value);
                u16_to_stream(it, resp.chars[i].uuid.uuid);
            } else {
                // leave the characteristic value declaration empty; it will be
                // fullfiled by a read of the attribute.
                it += (1 + 2 + 16);
            }
        }

        return result;
    }

    /**
     * Compute characteristic properties from ble_gattc_char_t.
     */
    static uint8_t get_properties(const ble_gattc_char_t& char_desc)
    {
        return
            (char_desc.char_props.broadcast << 0) |
            (char_desc.char_props.read << 1) |
            (char_desc.char_props.write_wo_resp << 2) |
            (char_desc.char_props.write << 3) |
            (char_desc.char_props.notify << 4) |
            (char_desc.char_props.indicate << 5) |
            (char_desc.char_props.auth_signed_wr << 6) |
            (char_desc.char_ext_props << 7);
    }

    read_by_type_response_t _response;
    uint16_t _idx;
};

/**
 * Procedure that handle discovery of characteristic descriptors.
 */
struct nRF5xGattClient::DiscoverDescriptorsProcedure : RegularGattProcedure {
    DiscoverDescriptorsProcedure(connection_handle_t connection) :
        RegularGattProcedure(
            connection,
            AttributeOpcode::FIND_INFORMATION_REQUEST,
            BLE_GATTC_EVT_ATTR_INFO_DISC_RSP
        ) {
        }

    ble_error_t start(attribute_handle_range_t discovery_range)
    {
        ble_gattc_handle_range_t range = to_nordic_handle_range(discovery_range);
        uint32_t err = sd_ble_gattc_attr_info_discover(
            connection_handle,
            &range
        );

        return convert_sd_error(err);
    }

    virtual void do_handle(const ble_gattc_evt_t &evt)
    {
        /**
         * Adapt ble_gattc_evt_attr_info_disc_rsp_t into
         * pal::AttFindInformationResponse
         */
        struct CustomFindInformationResponse : AttFindInformationResponse {
            CustomFindInformationResponse(const ble_gattc_evt_attr_info_disc_rsp_t &resp) :
                AttFindInformationResponse(), response(resp) {}

            virtual size_t size() const
            {
                return response.count;
            }

#if (NRF_SD_BLE_API_VERSION < 3)
            virtual information_data_t operator[](size_t i) const
            {
                information_data_t result = {
                    response.attr_info[i].handle
                };

                if (response.format == BLE_GATTC_ATTR_INFO_FORMAT_16BIT) {
                    result.uuid = UUID(response.attr_info[i].info.uuid16.uuid);
                } else {
                    result.uuid = UUID(
                        response.attr_info[i].info.uuid128.uuid128,
                        UUID::LSB
                    );
                }

                return result;
            }
#else
            virtual information_data_t operator[](size_t i) const
            {
                if (response.format == BLE_GATTC_ATTR_INFO_FORMAT_16BIT) {
                    information_data_t result = {
                        response.info.attr_info16[i].handle,
                        UUID(response.info.attr_info16[i].uuid.uuid)
                    };

                    return result;
                } else {
                    information_data_t result = {
                        response.info.attr_info128[i].handle,
                        UUID(
                            response.info.attr_info128[i].uuid.uuid128,
                            UUID::LSB
                        )
                    };

                    return result;
                }
            }


#endif


            const ble_gattc_evt_attr_info_disc_rsp_t &response;
        };

        terminate(CustomFindInformationResponse(evt.params.attr_info_disc_rsp));
    }
};


/**
 * Procedure that handle read of attribute handles.
 */
struct nRF5xGattClient::ReadAttributeProcedure : RegularGattProcedure {
    ReadAttributeProcedure(connection_handle_t connection) :
        RegularGattProcedure(
            connection, AttributeOpcode::READ_REQUEST, BLE_GATTC_EVT_READ_RSP
        ) { }

    ble_error_t start(attribute_handle_t attribute_handle)
    {
        uint32_t err = sd_ble_gattc_read(connection_handle, attribute_handle, 0);
        return convert_sd_error(err);
    }

    virtual void do_handle(const ble_gattc_evt_t &evt)
    {
        const ble_gattc_evt_read_rsp_t& rsp = evt.params.read_rsp;
        if (rsp.offset != 0 ) {
            abort();
            return;
        }

        terminate(AttReadResponse(make_const_ArrayView(rsp.data, rsp.len)));
    }
};

/**
 * Procedure that handle read of characteristic using characteristic UUID.
 */
struct nRF5xGattClient::ReadUsingCharacteristicUUIDProcedure : RegularGattProcedure {
    ReadUsingCharacteristicUUIDProcedure(connection_handle_t connection) :
        RegularGattProcedure(
            connection,
            AttributeOpcode::READ_BY_TYPE_REQUEST,
            BLE_GATTC_EVT_CHAR_VAL_BY_UUID_READ_RSP
        ) { }

    ble_error_t start(attribute_handle_range_t read_range, const UUID& uuid)
    {
        ble_uuid_t nordic_uuid = { 0 };
        ble_error_t ble_err = to_nordic_uuid(uuid, nordic_uuid);
        if (ble_err) {
            return ble_err;
        }

        ble_gattc_handle_range_t range = {
            read_range.begin,
            read_range.end
        };

        uint32_t err =  sd_ble_gattc_char_value_by_uuid_read(
            connection_handle,
            &nordic_uuid,
            &range
        );
        return convert_sd_error(err);
    }

#if (NRF_SD_BLE_API_VERSION >= 3)
    /**
     * Adapt ble_gattc_evt_char_val_by_uuid_read_rsp_t into AttReadByTypeResponse.
     */
    virtual void do_handle(const ble_gattc_evt_t &evt)
    {
        const ble_gattc_evt_char_val_by_uuid_read_rsp_t &rsp =
            evt.params.char_val_by_uuid_read_rsp;

        uint8_t element_size = sizeof(uint16_t) + rsp.value_len;

        terminate(SimpleAttReadByTypeResponse(
            element_size,
            make_const_ArrayView(
                rsp.handle_value,
                rsp.count * element_size
            )
        ));
    }

#else
    /**
     * Adapt ble_gattc_evt_char_val_by_uuid_read_rsp_t into AttReadByTypeResponse.
     */
    virtual void do_handle(const ble_gattc_evt_t &evt)
    {
        struct CustomReadByTypeResponse : AttReadByTypeResponse {
            CustomReadByTypeResponse(const ble_gattc_evt_char_val_by_uuid_read_rsp_t& rsp) :
                AttReadByTypeResponse(), response(rsp) { }

            virtual size_t size() const
            {
                return response.count;
            }

            virtual attribute_data_t operator[](size_t i) const
            {
                attribute_data_t result = {
                    response.handle_value[i].handle,
                    make_const_ArrayView(
                        response.handle_value[i].p_value,
                        response.value_len
                    )
                };
                return result;
            }

            const ble_gattc_evt_char_val_by_uuid_read_rsp_t& response;
        }

        terminate(CustomReadByTypeResponse(evt.params.char_val_by_uuid_read_rsp));
    }
#endif
};

/**
 * Procedure that handles read blob transactions.
 */
struct nRF5xGattClient::ReadAttributeBlobProcedure : RegularGattProcedure {
    ReadAttributeBlobProcedure(connection_handle_t connection) :
        RegularGattProcedure(
            connection, AttributeOpcode::READ_BLOB_REQUEST, BLE_GATTC_EVT_READ_RSP
        ) { }

    ble_error_t start(attribute_handle_t attribute_handle, uint16_t offset)
    {
        uint32_t err = sd_ble_gattc_read(
            connection_handle, attribute_handle, offset
        );
        return convert_sd_error(err);
    }

    virtual void do_handle(const ble_gattc_evt_t &evt)
    {
        terminate(AttReadBlobResponse(make_const_ArrayView(
            evt.params.read_rsp.data,
            evt.params.read_rsp.len
        )));
    }
};

/**
 * Procedure that handles Read Multiple Characteristic Values transactions.
 */
struct nRF5xGattClient::ReadMultipleCharacteristicsProcedure : RegularGattProcedure {
    ReadMultipleCharacteristicsProcedure(connection_handle_t connection) :
        RegularGattProcedure(
            connection,
            AttributeOpcode::READ_MULTIPLE_REQUEST,
            BLE_GATTC_EVT_CHAR_VALS_READ_RSP
        ) { }

    ble_error_t start(const ArrayView<const attribute_handle_t>& characteristic_handles)
    {
        uint32_t err = sd_ble_gattc_char_values_read(
            connection_handle,
            characteristic_handles.data(),
            characteristic_handles.size()
        );
        return convert_sd_error(err);
    }

    virtual void do_handle(const ble_gattc_evt_t &evt)
    {
        terminate(AttReadMultipleResponse(make_const_ArrayView(
            evt.params.char_vals_read_rsp.values,
            evt.params.char_vals_read_rsp.len
        )));
    }
};

/**
 * Procedure that handles Write transactions.
 */
struct nRF5xGattClient::WriteAttributeProcedure : RegularGattProcedure {
    WriteAttributeProcedure(connection_handle_t connection) :
        RegularGattProcedure(
            connection, AttributeOpcode::WRITE_REQUEST, BLE_GATTC_EVT_WRITE_RSP
        ) { }

    ble_error_t start(
        attribute_handle_t attribute_handle, const ArrayView<const uint8_t>& value
    ) {
        ble_gattc_write_params_t write_params = {
            BLE_GATT_OP_WRITE_REQ,
            /* exec flags */ 0,
            attribute_handle,
            /* offset */ 0,
            static_cast<uint16_t>(value.size()),
            const_cast<uint8_t*>(value.data())
        };

        uint32_t err = sd_ble_gattc_write(connection_handle, &write_params);
        return convert_sd_error(err);
    }

    virtual void do_handle(const ble_gattc_evt_t &evt)
    {
        terminate(AttWriteResponse());
    }
};

/**
 * Procedure that handles Prepare Write transactions.
 */
struct nRF5xGattClient::QueuePrepareWriteProcedure : RegularGattProcedure {
    QueuePrepareWriteProcedure(connection_handle_t connection) :
        RegularGattProcedure(
            connection,
            AttributeOpcode::PREPARE_WRITE_REQUEST,
            BLE_GATTC_EVT_WRITE_RSP
        ) { }

    ble_error_t start(
        attribute_handle_t characteristic_value_handle,
        const ArrayView<const uint8_t>& value,
        uint16_t offset
    ) {
        ble_gattc_write_params_t write_params = {
            BLE_GATT_OP_PREP_WRITE_REQ,
            /* exec flags */ 0,
            characteristic_value_handle,
            offset,
            static_cast<uint16_t>(value.size()),
            const_cast<uint8_t*>(value.data())
        };

        uint32_t err = sd_ble_gattc_write(connection_handle, &write_params);
        return convert_sd_error(err);
    }

    virtual void do_handle(const ble_gattc_evt_t &evt)
    {
        const ble_gattc_evt_write_rsp_t &response = evt.params.write_rsp;

        if (response.write_op != BLE_GATT_OP_PREP_WRITE_REQ) {
            abort();
            return;
        }

        terminate(AttPrepareWriteResponse(
            response.handle,
            response.offset,
            make_const_ArrayView(response.data, response.len)
        ));
    }
};

/**
 * Procedure that handles Execute Write transactions.
 */
struct nRF5xGattClient::ExecuteWriteQueueProcedure : RegularGattProcedure {
    ExecuteWriteQueueProcedure(connection_handle_t connection) :
        RegularGattProcedure(
            connection,
            AttributeOpcode::EXECUTE_WRITE_REQUEST,
            BLE_GATTC_EVT_WRITE_RSP
        ) { }

    ble_error_t start(bool execute) {
        ble_gattc_write_params_t write_params = {
            BLE_GATT_OP_EXEC_WRITE_REQ,
            static_cast<uint8_t>(
                execute ?
                    BLE_GATT_EXEC_WRITE_FLAG_PREPARED_CANCEL :
                    BLE_GATT_EXEC_WRITE_FLAG_PREPARED_WRITE
            ),
            /* attribute handle */ 0,
            /* value offset */ 0,
            /* buffer size*/ 0,
            /* buffer data */ NULL
        };

        uint32_t err = sd_ble_gattc_write(connection_handle, &write_params);
        return convert_sd_error(err);
    }

    virtual void do_handle(const ble_gattc_evt_t &evt)
    {
        const ble_gattc_evt_write_rsp_t &response = evt.params.write_rsp;
        if (response.write_op != BLE_GATT_OP_EXEC_WRITE_REQ) {
            abort();
            return;
        }

        terminate(AttExecuteWriteResponse());
    }
};

// NOTE: position after declaration of GattProcedure on purpose.
ble_error_t nRF5xGattClient::terminate()
{
    for (size_t i = 0; i < max_procedures_count; ++i) {
        if (_procedures[i]) {
            _procedures[i]->abort();
            _procedures[i] = NULL;
        }
    }

    return BLE_ERROR_NONE;
}

template<typename ProcType, typename A0>
ble_error_t nRF5xGattClient::launch_procedure(
    connection_handle_t connection, const A0& a0
) {
    ProcType* p = new(std::nothrow) ProcType(connection);
    if (!p) {
        return BLE_ERROR_NO_MEM;
    }

    if (!register_procedure(p)) {
        delete p;
        return BLE_ERROR_INVALID_STATE;
    }

    ble_error_t err = p->start(a0);
    if (err) {
        remove_procedure(p);
        delete p;
    }

    return err;
}

template<typename ProcType, typename A0, typename A1>
ble_error_t nRF5xGattClient::launch_procedure(
    connection_handle_t connection, const A0& a0, const A1& a1
) {
    ProcType* p = new(std::nothrow) ProcType(connection);
    if (!p) {
        return BLE_ERROR_NO_MEM;
    }

    if (!register_procedure(p)) {
        delete p;
        return BLE_ERROR_INVALID_STATE;
    }

    ble_error_t err = p->start(a0, a1);
    if (err) {
        remove_procedure(p);
        delete p;
    }

    return err;
}

template<typename ProcType, typename A0, typename A1, typename A2>
ble_error_t nRF5xGattClient::launch_procedure(
    connection_handle_t connection,
    const A0& a0, const A1& a1, const A2& a2
) {
    ProcType* p = new(std::nothrow) ProcType(connection);
    if (!p) {
        return BLE_ERROR_NO_MEM;
    }

    if (!register_procedure(p)) {
        delete p;
        return BLE_ERROR_INVALID_STATE;
    }

    ble_error_t err = p->start(a0, a1, a2);
    if (err) {
        remove_procedure(p);
        delete p;
    }

    return err;
}

template<typename ProcType, typename A0, typename A1, typename A2, typename A3>
ble_error_t nRF5xGattClient::launch_procedure(
    connection_handle_t connection,
    const A0& a0, const A1& a1, const A2& a2, const A3& a3
) {
    ProcType* p = new(std::nothrow) ProcType(connection);
    if (!p) {
        return BLE_ERROR_NO_MEM;
    }

    if (!register_procedure(p)) {
        delete p;
        return BLE_ERROR_INVALID_STATE;
    }

    ble_error_t err = p->start(a0, a1, a2, a3);
    if (err) {
        remove_procedure(p);
        delete p;
    }

    return err;
}

nRF5xGattClient::GattProcedure* nRF5xGattClient::get_procedure(
    connection_handle_t connection
) const {
    for (size_t i = 0; i < max_procedures_count; ++i) {
        if (_procedures[i] && _procedures[i]->connection_handle == connection) {
            return _procedures[i];
        }
    }
    return NULL;
}

bool nRF5xGattClient::register_procedure(GattProcedure *p)
{
    if (get_procedure(p->connection_handle)) {
        return false;
    }

    for (size_t i = 0; i < max_procedures_count; ++i) {
        if (!_procedures[i]) {
            _procedures[i] = p;
            return true;
        }
    }

    return false;
}

bool nRF5xGattClient::remove_procedure(nRF5xGattClient::GattProcedure* p)
{
    for (size_t i = 0; i < max_procedures_count; ++i) {
        if (_procedures[i] == p) {
            _procedures[i] = NULL;
            return true;
        }
    }

    return false;
}

// singleton of the ARM Cordio client
nRF5xGattClient& nRF5xGattClient::get_client()
{
    static nRF5xGattClient _client;
    return _client;
}

void nRF5xGattClient::handle_events(const ble_evt_t *evt) {
    switch (evt->header.evt_id) {
        case BLE_GATTC_EVT_PRIM_SRVC_DISC_RSP:
        case BLE_GATTC_EVT_REL_DISC_RSP:
        case BLE_GATTC_EVT_CHAR_DISC_RSP:
        case BLE_GATTC_EVT_DESC_DISC_RSP:
        case BLE_GATTC_EVT_ATTR_INFO_DISC_RSP:
        case BLE_GATTC_EVT_CHAR_VAL_BY_UUID_READ_RSP:
        case BLE_GATTC_EVT_READ_RSP:
        case BLE_GATTC_EVT_CHAR_VALS_READ_RSP:
        case BLE_GATTC_EVT_WRITE_RSP:
            get_client().handle_procedure_event(*evt);
            break;
        case BLE_GATTC_EVT_HVX:
            get_client().handle_hvx_event(*evt);
            break;
        case BLE_GATTC_EVT_TIMEOUT:
            get_client().handle_timeout_event(*evt);
            break;
    }
}

void nRF5xGattClient::handle_procedure_event(const ble_evt_t &evt)
{
    GattProcedure* p = get_procedure(evt.evt.gattc_evt.conn_handle);
    if (p) {
        p->handle(evt);
    }
}

void nRF5xGattClient::handle_hvx_event(const ble_evt_t &evt)
{
    connection_handle_t connection = evt.evt.gattc_evt.conn_handle;
    const ble_gattc_evt_hvx_t &hvx_evt = evt.evt.gattc_evt.params.hvx;

    switch (hvx_evt.type) {
        case BLE_GATT_HVX_NOTIFICATION:
            on_server_event(
                connection,
                AttHandleValueNotification(
                    hvx_evt.handle,
                    make_const_ArrayView(hvx_evt.data, hvx_evt.len)
                )
            );
            return;
        case BLE_GATT_HVX_INDICATION:
            // send confirmation first then process the event
            sd_ble_gattc_hv_confirm(connection, hvx_evt.handle);
            on_server_event(
                connection,
                AttHandleValueIndication(
                    hvx_evt.handle,
                    make_const_ArrayView(hvx_evt.data, hvx_evt.len)
                )
            );
            return;
        default:
            return;
    }
}

void nRF5xGattClient::handle_timeout_event(const ble_evt_t &evt)
{
    connection_handle_t connection = evt.evt.gattc_evt.conn_handle;
    GattProcedure* p = get_procedure(connection);
    if (p) {
        p->abort();
    }

    on_transaction_timeout(connection);
}

void nRF5xGattClient::handle_connection_termination(connection_handle_t connection)
{
    GattProcedure* p = get_client().get_procedure(connection);
    if (p) {
        p->abort();
    }
}

} // nordic
} // vendor
} // pal
} // ble




