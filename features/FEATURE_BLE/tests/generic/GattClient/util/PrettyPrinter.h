/* mbed Microcontroller Library
 * Copyright (c) 2018 ARM Limited
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

#ifndef TESTS_GENERIC_GATTCLIENT_UTIL_PRETTYPRINTER_H_
#define TESTS_GENERIC_GATTCLIENT_UTIL_PRETTYPRINTER_H_

#include <ostream>
#include <ble/UUID.h>
#include <ble/GattCallbackParamTypes.h>
#include <ble/GattCallbackParamTypes.h>

/*
 * UUID pretty printer
 */
inline ::std::ostream& operator<<(::std::ostream& os, const UUID& uuid) {
	char str[60];
	if (uuid.shortOrLong() == UUID::UUID_TYPE_SHORT) {
		sprintf(str, "0x%04X", uuid.getShortUUID());
	} else {
		const uint8_t* data = uuid.getBaseUUID();
		sprintf(
			str,
			"\"%02X%02X%02X%02X-%02X%02X-%02X%02X-%02X%02X-%02X%02X%02X%02X%02X%02X\"",
		    data[15], data[14], data[13], data[12], data[11], data[10], data[9], data[8],
		    data[7], data[6], data[5], data[4], data[3], data[2], data[1], data[0]
		);
	}
	return os << str;
}

/*
 * GattReadCallbackParams pretty printer
 */
inline ::std::ostream& operator<<(::std::ostream& os, const GattReadCallbackParams& p) {
	os << "connection_handle: " << p.connHandle <<
	", handle: " << p.handle <<
	", offset: " << p.offset <<
	", status: " << p.status;

	if (p.status) {
		os << ", error_code: " << p.error_code;
	} else {
		os << ", len: " << p.len;
		os << ", data: ";
		for (size_t i = 0; i < p.len; ++i) {
			os << (int) p.data[i] << " ";
		}
	}

	return os;
}

/*
 * attribute_handle_range_t pretty printer
 */
inline ::std::ostream& operator<<(::std::ostream& os, const ble::attribute_handle_range_t& handle_range) {
	return os << "(" << handle_range.begin << "," << handle_range.end << ")";
}

/*
 * Pretty printer of an view to an array of uint8_t.
 */
inline ::std::ostream& operator<<(::std::ostream& os, const ble::ArrayView<const uint8_t>& array) {
	os << "size: " << array.size() << " value: { ";
	for (size_t i = 0; i <array.size(); ++i) {
		os << (int) array[i] << " ";
	}
	os << " }";
	return os;
}

/*
 * DiscoveredCharacteristic pretty printer
 */
inline ::std::ostream& operator<<(::std::ostream& os, const DiscoveredCharacteristic& d) {
	return os << "client: " << d.getGattClient() << " " <<
			     "connection_handle: " << d.getConnectionHandle() << " " <<
				 "decl_handle: " << d.getDeclHandle() << " " <<
				 "value_handle: " << d.getValueHandle() << " " <<
				 "last_handle: " << d.getLastHandle() << " " <<
				 "uuid: { " << d.getUUID() << " } ";
}

/*
 * DiscoveredCharacteristicDescriptor pretty printer
 */
inline ::std::ostream& operator<<(::std::ostream& os, const DiscoveredCharacteristicDescriptor& d) {
	return os << "client: " << d.getGattClient() << " " <<
		  "connection_handle: " << d.getConnectionHandle() << " " <<
		  "attribute_handle: " << d.getAttributeHandle() << " " <<
		  "uuid: { " << d.getUUID() << " }";
}

/*
 * CharacteristicDescriptorDiscovery::TerminationCallbackParams_t pretty printer
 */
inline ::std::ostream& operator<<(::std::ostream& os, CharacteristicDescriptorDiscovery::TerminationCallbackParams_t& t) {
	return os << "characteristic: { " << t.characteristic << " } " <<
			     "status: " << t.status << " " <<
				 "error_code: " << t.error_code;
}

/*
 * CharacteristicDescriptorDiscovery::DiscoveryCallbackParams_t pretty printer
 */
inline ::std::ostream& operator<<(::std::ostream& os, const CharacteristicDescriptorDiscovery::DiscoveryCallbackParams_t& p) {
	return os << "characteristic: { " << p.characteristic << " } " <<
			     "descriptor: { " << p.descriptor << " } ";
}

#endif /* TESTS_GENERIC_GATTCLIENT_UTIL_PRETTYPRINTER_H_ */
