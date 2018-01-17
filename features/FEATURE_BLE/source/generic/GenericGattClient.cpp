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

#include <stdio.h>
#include <stdlib.h>

#include <ble/DiscoveredService.h>
#include <ble/DiscoveredCharacteristic.h>
#include "ble/generic/GenericGattClient.h"
#include "ble/blecommon.h"
#include <algorithm>

using ble::pal::AttServerMessage;
using ble::pal::AttReadResponse;
using ble::pal::AttReadBlobResponse;
using ble::pal::AttReadByTypeResponse;
using ble::pal::AttReadByGroupTypeResponse;
using ble::pal::AttFindByTypeValueResponse;
using ble::pal::AttErrorResponse;
using ble::pal::AttributeOpcode;
using ble::pal::AttWriteResponse;
using ble::pal::AttPrepareWriteResponse;
using ble::pal::AttExecuteWriteResponse;
using ble::pal::AttHandleValueIndication;
using ble::pal::AttHandleValueNotification;
using ble::pal::AttFindInformationResponse;

namespace ble {
namespace generic {

/*
 * Type of procedures which can be launched by the client.
 */
enum procedure_type_t {
	COMPLETE_DISCOVERY_PROCEDURE,
	READ_PROCEDURE,
	WRITE_PROCEDURE,
	DESCRIPTOR_DISCOVERY_PROCEDURE
};


/*
 * Base class for a procedure control block
 */
struct GenericGattClient::ProcedureControlBlock {
	/*
	 * Base constructor for procedure control block.
	 */
	ProcedureControlBlock(procedure_type_t type, Gap::Handle_t handle) :
		type(type), connection_handle(handle), next(NULL) { }

	virtual ~ProcedureControlBlock() { }

	/*
	 * Entry point of the control block stack machine.
	 */
	virtual void handle(GenericGattClient* client, const AttServerMessage& message) = 0;

	/*
	 * Function call in case of timeout
	 */
	virtual void handle_timeout_error(GenericGattClient* client) = 0;

	/**
	 * Function called when the procedure is aborted
	 */
	virtual void abort(GenericGattClient *client) = 0;

	procedure_type_t type;
	Gap::Handle_t connection_handle;
	ProcedureControlBlock* next;
};


/*
 * Procedure control block for the discovery process.
 */
struct GenericGattClient::DiscoveryControlBlock : public ProcedureControlBlock {
	DiscoveryControlBlock(
		Gap::Handle_t handle,
		ServiceDiscovery::ServiceCallback_t service_callback,
		ServiceDiscovery::CharacteristicCallback_t characteristic_callback,
		UUID matching_service_uuid,
		UUID matching_characteristic_uuid
	) : ProcedureControlBlock(COMPLETE_DISCOVERY_PROCEDURE, handle),
		service_callback(service_callback),
		characteristic_callback(characteristic_callback),
		matching_service_uuid(matching_service_uuid),
		matching_characteristic_uuid(matching_characteristic_uuid),
		services_discovered(NULL),
		done(false) {
	}

	virtual ~DiscoveryControlBlock() {
		while(services_discovered) {
			service_t* tmp = services_discovered->next;
			delete services_discovered;
			services_discovered = tmp;
		}
	}

	virtual void handle_timeout_error(GenericGattClient* client) {
		terminate(client);
	}

	virtual void abort(GenericGattClient *client) {
		terminate(client);
	}

	virtual void handle(GenericGattClient* client, const AttServerMessage& message) {
		// if end of discovery has been requested, ends it immediately
		if (done) {
			terminate(client);
			return;
		}

		switch(message.opcode) {
			case AttributeOpcode::READ_BY_GROUP_TYPE_RESPONSE:
				handle_service_discovered(
					client, static_cast<const AttReadByGroupTypeResponse&>(message)
				);
			   break;
			case AttributeOpcode::FIND_BY_VALUE_TYPE_RESPONSE:
				handle_service_discovered(
					client, static_cast<const AttFindByTypeValueResponse&>(message)
				);
			    break;
			case AttributeOpcode::READ_BY_TYPE_RESPONSE:
				handle_characteristic_discovered(
					client, static_cast<const AttReadByTypeResponse&>(message)
				);
			    break;
			case AttributeOpcode::ERROR_RESPONSE: {
				const AttErrorResponse& error = static_cast<const AttErrorResponse&>(message);
				if (error.error_code != AttErrorResponse::ATTRIBUTE_NOT_FOUND) {
					terminate(client);
				}

				switch (error.request_opcode) {
					case AttributeOpcode::READ_BY_GROUP_TYPE_REQUEST:
					case AttributeOpcode::FIND_BY_TYPE_VALUE_REQUEST:
						start_characteristic_discovery(client);
						break;
					case AttributeOpcode::READ_BY_TYPE_REQUEST:
						handle_all_characteristics_discovered(client);
						break;
					default:
						// error
						break;
				}
			}   break;
			default:
				// error
				break;
		}
	}

	template<typename Response>
	void handle_service_discovered(GenericGattClient* client, const Response& response) {
		if (!response.size()) {
			terminate(client);
			return;
		}

		uint16_t end_handle = 0x0000;
		for (size_t i = 0; i < response.size(); ++i) {
			uint16_t start_handle = get_start_handle(response[i]);
			end_handle = get_end_handle(response[i]);
			UUID uuid = get_uuid(response[i]);

			if (!characteristic_callback) {
				DiscoveredService discovered_service;
				discovered_service.setup(uuid, start_handle, end_handle);
				service_callback(&discovered_service);
			} else {
				service_t* discovered_service = new (std::nothrow) service_t(
					start_handle, end_handle, uuid
				);

				if (discovered_service == NULL) {
					terminate(client);
					return;
				}

				insert_service(discovered_service);
			}
		}

		if (end_handle == 0xFFFF) {
			start_characteristic_discovery(client);
		} else {
			ble_error_t err = client->_pal_client->discover_primary_service(
				connection_handle, end_handle + 1
			);

			if (err) {
				terminate(client);
			}
		}
	}

	void start_characteristic_discovery(GenericGattClient* client) {
		if (!services_discovered) {
			terminate(client);
			return;
		}

		if (!characteristic_callback) {
			terminate(client);
			return;
		}

		if (service_callback) {
			DiscoveredService discovered_service;
			discovered_service.setup(
				services_discovered->uuid,
				services_discovered->begin,
				services_discovered->end
			);
			service_callback(&discovered_service);
		}

		last_characteristic = characteristic_t();
		client->_pal_client->discover_characteristics_of_a_service(
			connection_handle,
			attribute_handle_range(
				services_discovered->begin,
				services_discovered->end
			)
		);
	}

	void handle_characteristic_discovered(GenericGattClient* client, const AttReadByTypeResponse& response) {
		for (size_t i = 0; i < response.size(); ++i) {
			if (last_characteristic.is_valid() == false) {
				last_characteristic.set_last_handle(response[i].handle - 1);
				if (matching_characteristic_uuid == UUID()
				|| last_characteristic.getUUID() == matching_characteristic_uuid) {
					characteristic_callback(&last_characteristic);
				}
			}

			last_characteristic = characteristic_t(
				client, connection_handle, response[i].handle, response[i].value
			);
		}

		// check if all the characteristics of the service has been discovered
		if (last_characteristic.getValueHandle() == services_discovered->end) {
			handle_all_characteristics_discovered(client);
		} else {
			ble_error_t err = client->_pal_client->discover_characteristics_of_a_service(
				connection_handle,
				attribute_handle_range(
					last_characteristic.getValueHandle() + 1,
					services_discovered->end
				)
			);

			if (err) {
				terminate(client);
			}
		}
	}

	void handle_all_characteristics_discovered(GenericGattClient* client) {
		if (last_characteristic.is_valid() == false) {
			if (matching_characteristic_uuid == UUID()
				|| matching_characteristic_uuid == last_characteristic.getUUID()) {
				last_characteristic.set_last_handle(services_discovered->end);
				characteristic_callback(&last_characteristic);
			}
		}

		service_t* old = services_discovered;
		services_discovered = services_discovered->next;
		delete old;

		if (!services_discovered) {
			terminate(client);
		} else {
			start_characteristic_discovery(client);
		}
	}

	void terminate(GenericGattClient* client) {
		// unknown error, terminate the procedure immediately
		client->remove_control_block(this);
		Gap::Handle_t handle = connection_handle;
		delete this;
		client->on_termination(handle);
	}

	uint16_t get_start_handle(const AttReadByGroupTypeResponse::attribute_data_t& data) {
		return data.group_range.begin;
	}

	uint16_t get_start_handle(const attribute_handle_range_t& range) {
		return range.begin;
	}

	uint16_t get_end_handle(const AttReadByGroupTypeResponse::attribute_data_t& data) {
		return data.group_range.end;
	}

	uint16_t get_end_handle(const attribute_handle_range_t& range) {
		return range.end;
	}

	UUID get_uuid(const AttReadByGroupTypeResponse::attribute_data_t& data) {
		if (data.value.size() == 2) {
			return UUID(data.value[0] | data.value[1] << 8);
		} else {
			return UUID(data.value.data(), UUID::LSB);
		}
	}

	UUID get_uuid(const attribute_handle_range_t& range) {
		return matching_service_uuid;
	}

	struct service_t {
		service_t(uint16_t begin, uint16_t end, const UUID& uuid) :
			begin(begin), end(end), uuid(uuid), next(NULL) { }
		uint16_t begin;
		uint16_t end;
		UUID uuid;
		service_t* next;
	};

	struct characteristic_t : DiscoveredCharacteristic {
		characteristic_t() : DiscoveredCharacteristic() {
			lastHandle = 0x0001;
		}

		characteristic_t(
			GattClient* client,
			Gap::Handle_t connection_handle,
			uint16_t decl_handle,
			const ArrayView<const uint8_t> value
		) : DiscoveredCharacteristic() {
			gattc = client;
			uuid = get_uuid(value);
			props = get_properties(value);
			declHandle = decl_handle;
			valueHandle = get_value_handle(value);
			lastHandle = 0x0000;
			connHandle = connection_handle;
		}

		static UUID get_uuid(const ArrayView<const uint8_t>& value) {
			if (value.size() == 5) {
				return UUID(value[3] | (value[4] << 8));
			} else {
				return UUID(value.data() + 3, UUID::LSB);
			}
		}

		static DiscoveredCharacteristic::Properties_t get_properties(const ArrayView<const uint8_t>& value) {
			uint8_t raw_properties = value[0];
			DiscoveredCharacteristic::Properties_t result;
			result._broadcast = (raw_properties & (1 << 0)) ? true : false;
			result._read = (raw_properties & (1 << 1)) ? true : false;
			result._writeWoResp = (raw_properties & (1 << 2))  ? true : false;
			result._write = (raw_properties & (1 << 3))  ? true : false;
			result._notify = (raw_properties & (1 << 4))  ? true : false;
			result._indicate = (raw_properties & (1 << 5))  ? true : false;
			result._authSignedWrite = (raw_properties & (1 << 6))  ? true : false;
			return result;
		}

		static uint16_t get_value_handle(const ArrayView<const uint8_t>& value) {
			return value[1] | (value[2] << 8);
		}

		void set_last_handle(uint16_t last_handle) {
			lastHandle = last_handle;
		}

		bool is_valid() const {
			return lastHandle != 0x0000;
		}
	};

	void insert_service(service_t* service) {
		if (services_discovered == NULL) {
			services_discovered = service;
			return;
		}

		service_t* current = services_discovered;
		while (current->next) {
			current = current->next;
		}
		current->next = service;
	}

	ServiceDiscovery::ServiceCallback_t service_callback;
	ServiceDiscovery::CharacteristicCallback_t characteristic_callback;
	UUID matching_service_uuid;
	UUID matching_characteristic_uuid;
	service_t* services_discovered;
	characteristic_t last_characteristic;
	bool done;
};


struct GenericGattClient::ReadControlBlock : public ProcedureControlBlock {
	ReadControlBlock(
		Gap::Handle_t connection_handle, uint16_t attribute_handle, uint16_t offset
	) : ProcedureControlBlock(READ_PROCEDURE, connection_handle),
		attribute_handle(attribute_handle),
		offset(offset), current_offset(offset), data(NULL) {
	}

	virtual ~ReadControlBlock() {
		if (data != NULL) {
			free(data);
		}
	}

	virtual void handle_timeout_error(GenericGattClient* client) {
		GattReadCallbackParams response = {
			connection_handle,
			attribute_handle,
			offset,
			0, // size of 0
			NULL, // no data
			BLE_ERROR_UNSPECIFIED,

		};
		terminate(client, response);
	}

	virtual void abort(GenericGattClient *client) {
		GattReadCallbackParams response = {
			connection_handle,
			attribute_handle,
			offset,
			0, // size of 0
			NULL, // no data
			BLE_ERROR_INVALID_STATE,

		};
		terminate(client, response);
	}

	void terminate(GenericGattClient* client, const GattReadCallbackParams& response) {
		client->remove_control_block(this);
		client->processReadResponse(&response);
		delete this;
	}

	virtual void handle(GenericGattClient* client, const AttServerMessage& message) {
		switch(message.opcode) {
			case AttributeOpcode::ERROR_RESPONSE:
				handle_error(client, static_cast<const AttErrorResponse&>(message));
				break;

			case AttributeOpcode::READ_RESPONSE:
				handle_read_response(client, static_cast<const AttReadResponse&>(message));
				break;

			case AttributeOpcode::READ_BLOB_RESPONSE:
				handle_read_response(client, static_cast<const AttReadBlobResponse&>(message));
				break;

			default: {
				// should not happen, terminate the procedure and notify client with an error
				// in such case
				GattReadCallbackParams response = {
					connection_handle,
					attribute_handle,
					AttErrorResponse::UNLIKELY_ERROR,
					0, // size of 0
					NULL, // no data
					BLE_ERROR_UNSPECIFIED,

				};
				terminate(client, response);
			}   break;
		}
	}

	template<typename ResponseType>
	void handle_read_response(GenericGattClient* client, const ResponseType& read_response) {
		uint16_t mtu_size = client->get_mtu(connection_handle);

		// end of responses ?
		if ((uint16_t) read_response.size() < (mtu_size - 1)) {
			GattReadCallbackParams response = {
				connection_handle,
				attribute_handle,
				offset,
				0, // size of 0
				NULL, // no data
				BLE_ERROR_NONE,
			};

			// is it the first response, or is there any other response already
			// in the object ?
			if (data == NULL) {
				response.len = (uint16_t) read_response.size();
				response.data = read_response.data();
			} else {
				// copy the data in the existing buffer
				memcpy(data + (current_offset - offset), read_response.data(), read_response.size());
				response.len = (current_offset + read_response.size()) - offset;
				response.data = data;
			}
			terminate(client, response);
		} else {
			// allocation which will contain the response data plus the next one.
			data = (uint8_t*) realloc(data, (current_offset - offset) + ((mtu_size - 1) * 2));
			if (data == NULL) {
				GattReadCallbackParams response = {
					connection_handle,
					attribute_handle,
					offset,
					AttErrorResponse::INSUFFICIENT_RESOURCES,
					NULL,
					BLE_ERROR_NO_MEM,
				};
				terminate(client, response);
				return;
			}

			memcpy(data + (current_offset - offset), read_response.data(), read_response.size());
			current_offset = current_offset + read_response.size();
			ble_error_t err = client->_pal_client->read_attribute_blob(
				connection_handle,
				attribute_handle,
				current_offset
			);

			if (err) {
				GattReadCallbackParams response = {
					connection_handle,
					attribute_handle,
					AttErrorResponse::UNLIKELY_ERROR,
					0, // size of 0
					NULL, // no data
					BLE_ERROR_UNSPECIFIED,

				};
				terminate(client, response);
			}
		}
	}

	void handle_error(GenericGattClient* client, const AttErrorResponse& error) {
		ble_error_t status = BLE_ERROR_UNSPECIFIED;

		switch (error.error_code) {
			case AttErrorResponse::INVALID_HANDLE:
				status = BLE_ERROR_INVALID_PARAM;
				break;
			case AttErrorResponse::INSUFFICIENT_AUTHORIZATION:
				status = BLE_ERROR_INVALID_STATE;
				break;
			case AttErrorResponse::INSUFFICIENT_AUTHENTICATION:
				status = BLE_ERROR_INVALID_STATE;
				break;
			case AttErrorResponse::INSUFFICIENT_ENCRYPTION_KEY_SIZE:
				status = BLE_ERROR_INVALID_STATE;
				break;
			case AttErrorResponse::INSUFFICIENT_ENCRYPTION:
				status = BLE_ERROR_INVALID_STATE;
				break;
			case AttErrorResponse::READ_NOT_PERMITTED:
				status = BLE_ERROR_OPERATION_NOT_PERMITTED;
				break;
			case AttErrorResponse::INVALID_OFFSET:
				status = BLE_ERROR_PARAM_OUT_OF_RANGE;
				break;
			case AttErrorResponse::ATTRIBUTE_NOT_LONG:
				status = BLE_ERROR_PARAM_OUT_OF_RANGE;
				break;
			default:
				status = BLE_ERROR_UNSPECIFIED;
				break;
		}

		GattReadCallbackParams response = {
			connection_handle,
			attribute_handle,
			offset,
			error.error_code,
			/* data */ NULL,
			status
		};

		terminate(client, response);
	}

	uint16_t attribute_handle;
	uint16_t offset;
	uint16_t current_offset;
	uint8_t* data;
};

/*
 * Control block for the write process
 */
struct GenericGattClient::WriteControlBlock : public ProcedureControlBlock {
	WriteControlBlock(
		Gap::Handle_t connection_handle, uint16_t attribute_handle,
		uint8_t* data, uint16_t len
	) : ProcedureControlBlock(WRITE_PROCEDURE, connection_handle),
		attribute_handle(attribute_handle), len(len), offset(0), data(data),
		prepare_success(false), status(BLE_ERROR_UNSPECIFIED), error_code(0xFF) {
	}

	virtual ~WriteControlBlock() {
		free(data);
	}

	virtual void handle_timeout_error(GenericGattClient* client) {
		GattWriteCallbackParams response = {
			connection_handle,
			attribute_handle,
			GattWriteCallbackParams::OP_WRITE_REQ,
			BLE_ERROR_UNSPECIFIED,
			0x00
		};
		terminate(client, response);
	}

	virtual void abort(GenericGattClient *client) {
		GattWriteCallbackParams response = {
			connection_handle,
			attribute_handle,
			GattWriteCallbackParams::OP_WRITE_REQ,
			BLE_ERROR_INVALID_STATE,
			0x00
		};
		terminate(client, response);
	}

	void terminate(GenericGattClient* client, const GattWriteCallbackParams& response) {
		client->remove_control_block(this);
		client->processWriteResponse(&response);
		delete this;
	}

	virtual void handle(GenericGattClient* client, const AttServerMessage& message) {
		switch(message.opcode) {
			case AttributeOpcode::ERROR_RESPONSE:
				handle_error(client, static_cast<const AttErrorResponse&>(message));
				break;

			case AttributeOpcode::WRITE_RESPONSE:
				handle_write_response(client, static_cast<const AttWriteResponse&>(message));
				break;

			case AttributeOpcode::PREPARE_WRITE_RESPONSE:
				handle_prepare_write_response(client, static_cast<const AttPrepareWriteResponse&>(message));
				break;

			case AttributeOpcode::EXECUTE_WRITE_RESPONSE:
				handle_execute_write_response(client, static_cast<const AttExecuteWriteResponse&>(message));
				break;

			default: {
				GattWriteCallbackParams response = {
					connection_handle,
					attribute_handle,
					GattWriteCallbackParams::OP_WRITE_REQ,
					BLE_ERROR_UNSPECIFIED,
					AttErrorResponse::UNLIKELY_ERROR
				};

				terminate(client, response);
			}   break;
		}
	}

	void handle_write_response(GenericGattClient* client, const AttWriteResponse& write_response) {
		GattWriteCallbackParams response = {
			connection_handle, attribute_handle,
			GattWriteCallbackParams::OP_WRITE_REQ,
			BLE_ERROR_NONE, 0x00
		};

		terminate(client, response);
	}

	void handle_prepare_write_response(GenericGattClient* client, const AttPrepareWriteResponse& write_response) {
		ble_error_t err = BLE_ERROR_UNSPECIFIED;

		uint16_t mtu_size = client->get_mtu(connection_handle);
		offset = write_response.offset + write_response.partial_value.size();
		if (offset < len) {
			err = client->_pal_client->queue_prepare_write(
				connection_handle, attribute_handle,
				make_const_ArrayView(
					data + offset,
					std::min((len - offset), (mtu_size - 5))
				),
				offset
			);
		} else {
			err = client->_pal_client->execute_write_queue(
				connection_handle, true
			);
		}

		if (err) {
			clear_prepare_queue(client, err, AttErrorResponse::UNLIKELY_ERROR);
		}
	}

	void handle_execute_write_response(GenericGattClient* client, const AttExecuteWriteResponse& execute_response) {
		if (prepare_success) {
			status = BLE_ERROR_NONE;
			error_code = 0x00;
		}

		GattWriteCallbackParams response = {
			connection_handle,
			attribute_handle,
			GattWriteCallbackParams::OP_WRITE_REQ,
			status,
			error_code
		};

		terminate(client, response);
	}

	void clear_prepare_queue(GenericGattClient* client, ble_error_t s, uint8_t e) {
		prepare_success = false;
		status = s;
		error_code = e;
		ble_error_t err = client->_pal_client->execute_write_queue(
			connection_handle, false
		);

		if (err) {
			GattWriteCallbackParams response = {
				connection_handle,
				attribute_handle,
				GattWriteCallbackParams::OP_WRITE_REQ,
				err,
				AttErrorResponse::UNLIKELY_ERROR
			};

			terminate(client, response);
		}
	}

	void handle_error(GenericGattClient* client, const AttErrorResponse& error) {
		ble_error_t status = BLE_ERROR_UNSPECIFIED;

		switch (error.error_code) {
			case AttErrorResponse::INVALID_HANDLE:
				status = BLE_ERROR_INVALID_PARAM;
				break;
			case AttErrorResponse::INVALID_ATTRIBUTE_VALUE_LENGTH:
				status = BLE_ERROR_INVALID_PARAM;
				break;
			case AttErrorResponse::INSUFFICIENT_AUTHORIZATION:
				status = BLE_ERROR_INVALID_STATE;
				break;
			case AttErrorResponse::INSUFFICIENT_AUTHENTICATION:
				status = BLE_ERROR_INVALID_STATE;
				break;
			case AttErrorResponse::INSUFFICIENT_ENCRYPTION_KEY_SIZE:
				status = BLE_ERROR_INVALID_STATE;
				break;
			case AttErrorResponse::INSUFFICIENT_ENCRYPTION:
				status = BLE_ERROR_INVALID_STATE;
				break;
			case AttErrorResponse::WRITE_NOT_PERMITTED:
				status = BLE_ERROR_OPERATION_NOT_PERMITTED;
				break;
			default:
				status = BLE_ERROR_UNSPECIFIED;
				break;
		}

		if (error.request_opcode == AttributeOpcode(AttributeOpcode::PREPARE_WRITE_REQUEST)) {
			clear_prepare_queue(client, status, error.error_code);
		} else {
			GattWriteCallbackParams response = {
				connection_handle,
				attribute_handle,
				GattWriteCallbackParams::OP_WRITE_REQ,
				status,
				error.error_code
			};

			terminate(client, response);
		}
	}

	uint16_t attribute_handle;
	uint16_t len;
	uint16_t offset;
	uint8_t* data;
	bool prepare_success;
	ble_error_t status;
	uint8_t error_code;
};

/*
 * Control block for the descriptor discovery process
 */
struct GenericGattClient::DescriptorDiscoveryControlBlock : public ProcedureControlBlock {
	DescriptorDiscoveryControlBlock(
		const DiscoveredCharacteristic& characteristic,
		const CharacteristicDescriptorDiscovery::DiscoveryCallback_t& discoveryCallback,
		const CharacteristicDescriptorDiscovery::TerminationCallback_t& terminationCallback
	) : ProcedureControlBlock(DESCRIPTOR_DISCOVERY_PROCEDURE, characteristic.getConnectionHandle()),
		characteristic(characteristic),
		discovery_cb(discoveryCallback),
		termination_cb(terminationCallback),
		next_handle(characteristic.getValueHandle() + 1),
		done(false) {
	}

	virtual ~DescriptorDiscoveryControlBlock() { }

	ble_error_t start(GenericGattClient* client) {
		return client->_pal_client->discover_characteristics_descriptors(
			connection_handle,
			attribute_handle_range(
				next_handle,
				characteristic.getLastHandle()
			)
		);
	}

	virtual void handle_timeout_error(GenericGattClient* client) {
		terminate(client, BLE_ERROR_UNSPECIFIED);
	}

	virtual void abort(GenericGattClient *client) {
		terminate(client, BLE_ERROR_INVALID_STATE);
	}

	virtual void handle(GenericGattClient* client, const AttServerMessage& message) {
		if (done) {
			terminate(client, BLE_ERROR_NONE);
			return;
		}

		switch(message.opcode) {
			case AttributeOpcode::ERROR_RESPONSE:
				handle_error(client, static_cast<const AttErrorResponse&>(message));
				return;

			case AttributeOpcode::FIND_INFORMATION_RESPONSE:
				handle_response(client, static_cast<const AttFindInformationResponse&>(message));
				return;

			default:
				break;
		}
	}

	void handle_error(GenericGattClient* client, const AttErrorResponse& error) {
		if (error.error_code == AttErrorResponse::ATTRIBUTE_NOT_FOUND) {
			terminate(client, BLE_ERROR_NONE);
		} else {
			terminate(client, BLE_ERROR_UNSPECIFIED, error.error_code);
		}
	}

	void handle_response(GenericGattClient* client, const AttFindInformationResponse& response) {
		for (size_t i = 0; i < response.size(); ++i) {
			DiscoveredCharacteristicDescriptor descriptor(
				client, connection_handle, response[i].handle, response[i].uuid
			);
			CharacteristicDescriptorDiscovery::DiscoveryCallbackParams_t params = {
				characteristic,
				descriptor
			};
			discovery_cb(&params);
			if (done) {
				terminate(client, BLE_ERROR_NONE);
				return;
			}

			if (response[i].handle == characteristic.getLastHandle()) {
				terminate(client, BLE_ERROR_NONE);
				return;
			}
			next_handle = response[i].handle + 1;
		}

		ble_error_t err = start(client);
		if (err) {
			terminate(client, err, AttErrorResponse::UNLIKELY_ERROR);
		}
	}

	void terminate(GenericGattClient* client, ble_error_t status, uint8_t error_code = 0x00) {
		client->remove_control_block(this);
		CharacteristicDescriptorDiscovery::TerminationCallbackParams_t params = {
			characteristic,
			status,
			error_code
		};
		termination_cb(&params);
		delete this;
	}

	DiscoveredCharacteristic characteristic;
	CharacteristicDescriptorDiscovery::DiscoveryCallback_t discovery_cb;
	CharacteristicDescriptorDiscovery::TerminationCallback_t termination_cb;
	uint16_t next_handle;
	bool done;
};


GenericGattClient::GenericGattClient(pal::GattClient* pal_client) :
	_pal_client(pal_client),
	_termination_callback(),
	 control_blocks(NULL),
	_is_reseting(false) {
	_pal_client->when_server_message_received(
		mbed::callback(this, &GenericGattClient::on_server_message_received)
	);
	_pal_client->when_transaction_timeout(
		mbed::callback(this, &GenericGattClient::on_transaction_timeout)
	);
}

ble_error_t GenericGattClient::launchServiceDiscovery(
	Gap::Handle_t connection_handle,
	ServiceDiscovery::ServiceCallback_t service_callback,
	ServiceDiscovery::CharacteristicCallback_t characteristic_callback,
	const UUID& matching_service_uuid,
	const UUID& matching_characteristic_uuid
) {
	// verify that there is no other procedures going on this connection
	if (_is_reseting || get_control_block(connection_handle)) {
		return BLE_ERROR_INVALID_STATE;
	}

	// terminate and return if there is no callback to call
	if (!service_callback && !characteristic_callback) {
		on_termination(connection_handle);
		return BLE_ERROR_NONE;
	}

	DiscoveryControlBlock* discovery_pcb = new(std::nothrow) DiscoveryControlBlock(
		connection_handle,
		service_callback,
		characteristic_callback,
		matching_service_uuid,
		matching_characteristic_uuid
	);

	if (discovery_pcb == NULL) {
		return BLE_ERROR_NO_MEM;
	}

	// note: control block inserted prior the request because they are part of
	// of the transaction and the callback can be call synchronously
	insert_control_block(discovery_pcb);

	// launch the request
	ble_error_t err = BLE_ERROR_UNSPECIFIED;
	if (matching_service_uuid == UUID()) {
		err = _pal_client->discover_primary_service(
			connection_handle,
			0x0001
		);
	} else {
		err = _pal_client->discover_primary_service_by_service_uuid(
			connection_handle,
			0x0001,
			matching_service_uuid
		);
	}

	if (err) {
		remove_control_block(discovery_pcb);
		delete discovery_pcb;
	}

	return err;
}

bool GenericGattClient::isServiceDiscoveryActive() const {
	ProcedureControlBlock* pcb = control_blocks;

	while (pcb) {
		if (pcb->type == COMPLETE_DISCOVERY_PROCEDURE) {
			return true;
		}
		pcb = pcb->next;
	}

	return false;
}

void GenericGattClient::terminateServiceDiscovery()
{
	ProcedureControlBlock* pcb = control_blocks;
	while (pcb) {
		if (pcb->type == COMPLETE_DISCOVERY_PROCEDURE) {
			static_cast<DiscoveryControlBlock*>(pcb)->done = true;
		}
		pcb = pcb->next;
	}
}

ble_error_t GenericGattClient::read(
	Gap::Handle_t connection_handle,
	GattAttribute::Handle_t attribute_handle,
	uint16_t offset) const
{
	// verify that there is no other procedures going on this connection
	if (_is_reseting || get_control_block(connection_handle)) {
		return BLE_ERROR_INVALID_STATE;
	}

	ReadControlBlock* read_pcb = new(std::nothrow) ReadControlBlock(
		connection_handle,
		attribute_handle,
		offset
	);

	if (read_pcb == NULL) {
		return BLE_ERROR_NO_MEM;
	}

	insert_control_block(read_pcb);

	ble_error_t err = BLE_ERROR_NONE;

	if (offset == 0) {
		err = _pal_client->read_attribute_value(
			connection_handle, attribute_handle
		);
	} else {
		err = _pal_client->read_attribute_blob(
			connection_handle, attribute_handle, offset
		);
	}

	if (err) {
		remove_control_block(read_pcb);
		delete read_pcb;
	}

	return err;
}

ble_error_t GenericGattClient::write(
	GattClient::WriteOp_t cmd,
	Gap::Handle_t connection_handle,
	GattAttribute::Handle_t attribute_handle,
	size_t length,
	const uint8_t* value
) const {
	// verify that there is no other procedures going on this connection
	if (_is_reseting || get_control_block(connection_handle)) {
		return BLE_ERROR_INVALID_STATE;
	}

	uint16_t mtu = get_mtu(connection_handle);

	if (cmd == GattClient::GATT_OP_WRITE_CMD) {
		if (length > (uint16_t)(mtu - 3)) {
			return BLE_ERROR_PARAM_OUT_OF_RANGE;
		}
		return _pal_client->write_without_response(
			connection_handle,
			attribute_handle,
			make_const_ArrayView(value, length)
		);
	} else {
		uint8_t* data = NULL;

		if (length > (uint16_t)(mtu - 3)) {
			data = (uint8_t*) malloc(length);
			if (data == NULL) {
				return BLE_ERROR_NO_MEM;
			}
			memcpy(data, value, length);
		}

		WriteControlBlock* write_pcb = new(std::nothrow) WriteControlBlock(
			connection_handle,
			attribute_handle,
			data,
			length
		);

		if (write_pcb == NULL) {
			free(data);
			return BLE_ERROR_NO_MEM;
		}

		insert_control_block(write_pcb);

		ble_error_t err = BLE_ERROR_UNSPECIFIED;
		if (data) {
			err = _pal_client->queue_prepare_write(
				connection_handle,
				attribute_handle,
				make_const_ArrayView(value, mtu - 5),
				/* offset */ 0
			);
		} else {
			err = _pal_client->write_attribute(
				connection_handle,
				attribute_handle,
				make_const_ArrayView(value, length)
			);
		}

		if (err) {
			remove_control_block(write_pcb);
			delete write_pcb;
		}

		return err;
	}

	return BLE_ERROR_NOT_IMPLEMENTED;
}

void GenericGattClient::onServiceDiscoveryTermination(
	ServiceDiscovery::TerminationCallback_t callback
) {
	_termination_callback = callback;
}

ble_error_t GenericGattClient::discoverCharacteristicDescriptors(
	const DiscoveredCharacteristic& characteristic,
	const CharacteristicDescriptorDiscovery::DiscoveryCallback_t& discoveryCallback,
	const CharacteristicDescriptorDiscovery::TerminationCallback_t& terminationCallback
) {
	// verify that there is no other procedures going on this connection
	if (_is_reseting || get_control_block(characteristic.getConnectionHandle())) {
		return BLE_ERROR_INVALID_STATE;
	}

	if (characteristic.getValueHandle() == characteristic.getLastHandle()) {
		CharacteristicDescriptorDiscovery::TerminationCallbackParams_t params = {
			characteristic,
			BLE_ERROR_NONE,
			/* error code */ 0x00
		};

		terminationCallback(&params);
		return BLE_ERROR_NONE;
	}

	DescriptorDiscoveryControlBlock* discovery_pcb =
		new(std::nothrow) DescriptorDiscoveryControlBlock(
			characteristic,
			discoveryCallback,
			terminationCallback
		);

	if (discovery_pcb == NULL) {
		return BLE_ERROR_NO_MEM;
	}

	insert_control_block(discovery_pcb);

	ble_error_t err = discovery_pcb->start(this);

	if (err) {
		remove_control_block(discovery_pcb);
		delete discovery_pcb;
	}

	return err;
}

bool GenericGattClient::isCharacteristicDescriptorDiscoveryActive(
	const DiscoveredCharacteristic& characteristic
) const {
	ProcedureControlBlock* pcb = control_blocks;

	while (pcb) {
		if (pcb->type == DESCRIPTOR_DISCOVERY_PROCEDURE &&
			static_cast<DescriptorDiscoveryControlBlock*>(pcb)->characteristic == characteristic) {
			return true;
		}
		pcb = pcb->next;
	}

	return false;
}

void GenericGattClient::terminateCharacteristicDescriptorDiscovery(
	const DiscoveredCharacteristic& characteristic
) {
	ProcedureControlBlock* pcb = control_blocks;

	while (pcb) {
		if (pcb->type == DESCRIPTOR_DISCOVERY_PROCEDURE) {
			DescriptorDiscoveryControlBlock* dpcb =
				static_cast<DescriptorDiscoveryControlBlock*>(pcb);
			if (dpcb->characteristic == characteristic) {
				dpcb->done = true;
				return;
			}
		}

		pcb = pcb->next;
	}

}

ble_error_t GenericGattClient::reset(void) {

	// _is_reseting prevent executions of new procedure while the instance resets.
	// otherwise new procedures can be launched from callbacks generated by the
	// reset.
	_is_reseting = true;
	while (control_blocks) {
		control_blocks->abort(this);
	}
	_is_reseting = false;

	return BLE_ERROR_NONE;
}

void GenericGattClient::on_termination(Gap::Handle_t connection_handle) {
	if (_termination_callback) {
		_termination_callback(connection_handle);
	}
}

void GenericGattClient::on_server_message_received(
	connection_handle_t connection_handle,
	const AttServerMessage& message
) {
	switch(message.opcode) {
		case AttributeOpcode::ERROR_RESPONSE:
		case AttributeOpcode::EXCHANGE_MTU_RESPONSE:
		case AttributeOpcode::FIND_INFORMATION_RESPONSE:
		case AttributeOpcode::FIND_BY_VALUE_TYPE_RESPONSE:
		case AttributeOpcode::READ_BY_TYPE_RESPONSE:
		case AttributeOpcode::READ_RESPONSE:
		case AttributeOpcode::READ_BLOB_RESPONSE:
		case AttributeOpcode::READ_MULTIPLE_RESPONSE:
		case AttributeOpcode::READ_BY_GROUP_TYPE_RESPONSE:
		case AttributeOpcode::WRITE_RESPONSE:
		case AttributeOpcode::PREPARE_WRITE_RESPONSE:
		case AttributeOpcode::EXECUTE_WRITE_RESPONSE: {
			on_server_response(connection_handle, message);
		} break;

		case AttributeOpcode::HANDLE_VALUE_INDICATION:
		case AttributeOpcode::HANDLE_VALUE_NOTIFICATION: {
			on_server_event(connection_handle, message);
		} break;

		default:
			// invalid message receive
			return;
	}
}

void GenericGattClient::on_server_response(
	connection_handle_t connection,
	const AttServerMessage& message
) {
	ProcedureControlBlock* pcb = get_control_block(connection);
	if (pcb == NULL) {
		return;
	}

	pcb->handle(this, message);
}

void GenericGattClient::on_server_event(connection_handle_t connection, const AttServerMessage& message) {
	GattHVXCallbackParams callbacks_params = {
		(Gap::Handle_t) connection, 0
	};

	switch (message.opcode) {
		case AttributeOpcode::HANDLE_VALUE_NOTIFICATION: {
			const AttHandleValueNotification& notification =
				static_cast<const AttHandleValueNotification&>(message);
			callbacks_params.handle = notification.attribute_handle;
			callbacks_params.type = BLE_HVX_NOTIFICATION;
			callbacks_params.len = notification.attribute_value.size();
			callbacks_params.data = notification.attribute_value.data();
		} break;

		case AttributeOpcode::HANDLE_VALUE_INDICATION: {
			const AttHandleValueIndication& indication =
				static_cast<const AttHandleValueIndication&>(message);
			callbacks_params.handle = indication.attribute_handle;
			callbacks_params.type = BLE_HVX_INDICATION;
			callbacks_params.len = indication.attribute_value.size();
			callbacks_params.data = indication.attribute_value.data();
		} break;

		default:
			return;
	}

	processHVXEvent(&callbacks_params);
}

void GenericGattClient::on_transaction_timeout(connection_handle_t connection) {
	ProcedureControlBlock* pcb = get_control_block(connection);
	if (pcb == NULL) {
		return;
	}

	pcb->handle_timeout_error(this);
}

GenericGattClient::ProcedureControlBlock* GenericGattClient::get_control_block(Gap::Handle_t connection) {
	ProcedureControlBlock* it = control_blocks;
	while (it && it->connection_handle != connection) {
		it = it->next;
	}
	return it;
}

const GenericGattClient::ProcedureControlBlock* GenericGattClient::get_control_block(Gap::Handle_t connection) const {
	ProcedureControlBlock* it = control_blocks;
	while (it && it->connection_handle != connection) {
		it = it->next;
	}
	return it;
}

void GenericGattClient::insert_control_block(ProcedureControlBlock* cb) const {
	if (control_blocks == NULL) {
		control_blocks = cb;
		return;
	}

	ProcedureControlBlock* current = control_blocks;
	while (current->next) {
		current = current->next;
	}
	current->next = cb;
}

void GenericGattClient::remove_control_block(ProcedureControlBlock* cb) const {
	if (control_blocks == NULL) {
		return;
	}

	if (cb == control_blocks) {
		control_blocks = control_blocks->next;
		return;
	}

	ProcedureControlBlock* current = control_blocks;
	while (current->next && current->next != cb) {
		current = current->next;
	}

	if (current->next == NULL) {
		return;
	}

	current->next = cb->next;
	cb->next = NULL;
}

uint16_t GenericGattClient::get_mtu(Gap::Handle_t connection) const {
	uint16_t result = 23;
	if(_pal_client->get_mtu_size((connection_handle_t) connection, result) != BLE_ERROR_NONE) {
		result = 23;
	}
	return result;
}

} // namespace pal
} // namespace ble
