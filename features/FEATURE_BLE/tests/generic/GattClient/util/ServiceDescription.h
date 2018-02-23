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

#ifndef TESTS_GENERIC_GATTCLIENT_UTIL_SERVICEDESCRIPTION_H_
#define TESTS_GENERIC_GATTCLIENT_UTIL_SERVICEDESCRIPTION_H_

#include <vector>
#include <initializer_list>
#include <stdint.h>
#include "ble/UUID.h"
#include "CharacteristicDescription.h"

struct service_description_t {
	service_description_t(
		uint16_t start,
		uint16_t end,
		UUID uuid,
		std::initializer_list<characteristic_description_t> characteristics
	) : start(start),
		end(end),
		uuid(uuid),
		characteristics(characteristics) {
	}

	uint16_t start;
	uint16_t end;
	UUID uuid;
	std::vector<characteristic_description_t> characteristics;
};

#endif /* TESTS_GENERIC_GATTCLIENT_UTIL_SERVICEDESCRIPTION_H_ */
