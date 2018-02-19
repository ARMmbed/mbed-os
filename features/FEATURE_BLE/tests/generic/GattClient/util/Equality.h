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

#ifndef TESTS_GENERIC_GATTCLIENT_UTIL_EQUALITY_H_
#define TESTS_GENERIC_GATTCLIENT_UTIL_EQUALITY_H_

#include "ble/DiscoveredCharacteristicDescriptor.h"
#include "ble/CharacteristicDescriptorDiscovery.h"
#include "ble/GattCallbackParamTypes.h"

bool operator==(
	const DiscoveredCharacteristicDescriptor& lhs,
	const DiscoveredCharacteristicDescriptor& rhs
);

bool operator!=(
	const DiscoveredCharacteristicDescriptor& lhs,
	const DiscoveredCharacteristicDescriptor& rhs
);

bool operator==(
	const CharacteristicDescriptorDiscovery::TerminationCallbackParams_t& lhs,
	const CharacteristicDescriptorDiscovery::TerminationCallbackParams_t& rhs
);

bool operator!=(
	const CharacteristicDescriptorDiscovery::TerminationCallbackParams_t& lhs,
	const CharacteristicDescriptorDiscovery::TerminationCallbackParams_t& rhs
);

bool operator==(
	const CharacteristicDescriptorDiscovery::DiscoveryCallbackParams_t& lhs,
	const CharacteristicDescriptorDiscovery::DiscoveryCallbackParams_t& rhs
);

bool operator!=(
	const CharacteristicDescriptorDiscovery::DiscoveryCallbackParams_t& lhs,
	const CharacteristicDescriptorDiscovery::DiscoveryCallbackParams_t& rhs
);

bool operator==(const GattReadCallbackParams& lhs, const GattReadCallbackParams& rhs);

bool operator==(const GattHVXCallbackParams& lhs, const GattHVXCallbackParams& rhs);

bool operator==(const GattWriteCallbackParams& lhs, const GattWriteCallbackParams& rhs);

#endif /* TESTS_GENERIC_GATTCLIENT_UTIL_EQUALITY_H_ */
