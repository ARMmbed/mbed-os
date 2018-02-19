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

#include "ble/DiscoveredCharacteristic.h"
#include "ble/DiscoveredCharacteristicDescriptor.h"
#include "ble/CharacteristicDescriptorDiscovery.h"
#include "ble/GattCallbackParamTypes.h"
#include "ble/ArrayView.h"

bool operator==(
	const DiscoveredCharacteristicDescriptor& lhs,
	const DiscoveredCharacteristicDescriptor& rhs
) {
	if (lhs.getGattClient() != rhs.getGattClient() ||
		lhs.getConnectionHandle() != rhs.getConnectionHandle() ||
		lhs.getUUID() != rhs.getUUID() ||
		lhs.getAttributeHandle() != rhs.getAttributeHandle()
	) {
		return false;
	}

	return true;
}

bool operator!=(
	const DiscoveredCharacteristicDescriptor& lhs,
	const DiscoveredCharacteristicDescriptor& rhs
) {
	return !(lhs == rhs);
}

bool operator==(
	const CharacteristicDescriptorDiscovery::TerminationCallbackParams_t& lhs,
	const CharacteristicDescriptorDiscovery::TerminationCallbackParams_t& rhs
) {
	if (lhs.characteristic != rhs.characteristic ||
		lhs.status != rhs.status ||
		lhs.error_code != rhs.error_code
	) {
		return false;
	}

	return true;
}

bool operator!=(
	const CharacteristicDescriptorDiscovery::TerminationCallbackParams_t& lhs,
	const CharacteristicDescriptorDiscovery::TerminationCallbackParams_t& rhs
) {
	return !(lhs == rhs);
}

bool operator==(
	const CharacteristicDescriptorDiscovery::DiscoveryCallbackParams_t& lhs,
	const CharacteristicDescriptorDiscovery::DiscoveryCallbackParams_t& rhs
) {
	if (lhs.characteristic != rhs.characteristic ||
		lhs.descriptor != rhs.descriptor
	) {
		return false;
	}

	return true;
}

bool operator!=(
	const CharacteristicDescriptorDiscovery::DiscoveryCallbackParams_t& lhs,
	const CharacteristicDescriptorDiscovery::DiscoveryCallbackParams_t& rhs
) {
	return !(lhs == rhs);
}

bool operator==(const GattReadCallbackParams& lhs, const GattReadCallbackParams& rhs) {
	if ((lhs.connHandle != rhs.connHandle) || (lhs.handle != rhs.handle) ||
		(lhs.offset != rhs.offset) || (lhs.status != rhs.status)) {
		return false;
	}

	if (lhs.status == BLE_ERROR_NONE) {
		if (lhs.len != rhs.len) {
			return false;
		}

		if (memcmp(lhs.data, rhs.data, lhs.len) != 0) {
			return false;
		}
	} else if (lhs.error_code != rhs.error_code) {
		return false;
	}

	return true;
}

bool operator==(const GattHVXCallbackParams& lhs, const GattHVXCallbackParams& rhs) {
	if (lhs.connHandle != rhs.connHandle &&
		lhs.handle != rhs.handle &&
		lhs.type != rhs.type &&
		ble::make_const_ArrayView(lhs.data, lhs.len) != ble::make_const_ArrayView(rhs.data, rhs.len)) {
		return false;
	}

	return true;
}

bool operator==(const GattWriteCallbackParams& lhs, const GattWriteCallbackParams& rhs) {
	if (lhs.connHandle != rhs.connHandle &&
		lhs.handle != rhs.handle &&
		lhs.writeOp != rhs.writeOp &&
		lhs.error_code != rhs.error_code &&
		lhs.status != rhs.status) {
		return false;
	}
	return true;
}
