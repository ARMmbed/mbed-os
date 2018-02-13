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

#ifndef TESTS_GENERIC_GATTCLIENT_UTIL_CHARACTERISTICDESCRIPTION_H_
#define TESTS_GENERIC_GATTCLIENT_UTIL_CHARACTERISTICDESCRIPTION_H_

#include <stdint.h>
#include "ble/UUID.h"
#include "ble/DiscoveredCharacteristic.h"

struct characteristic_description_t {
	characteristic_description_t() :
		handle(0), properties(), value_handle(0), uuid() { }

	characteristic_description_t(
		uint16_t handle,
		DiscoveredCharacteristic::Properties_t properties,
		UUID uuid
	) : handle(handle),
		properties(properties),
		value_handle(handle + 1),
		uuid(uuid) {
	}

	friend bool operator==(
		const characteristic_description_t& lhs, const characteristic_description_t& rhs
	) {
		return (lhs.handle == rhs.handle)
			&& (lhs.properties == rhs.properties)
			&& (lhs.value_handle == rhs.value_handle)
			&& (lhs.uuid == rhs.uuid);
	}

	friend bool operator !=(
		const characteristic_description_t& lhs, const characteristic_description_t& rhs
	) {
		return !(lhs == rhs);
	}

	uint16_t handle;
	DiscoveredCharacteristic::Properties_t properties;
	uint16_t value_handle;
	UUID uuid;
};

#endif /* TESTS_GENERIC_GATTCLIENT_UTIL_CHARACTERISTICDESCRIPTION_H_ */
