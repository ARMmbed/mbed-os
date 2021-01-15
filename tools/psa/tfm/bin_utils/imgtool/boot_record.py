# Copyright (c) 2019, Arm Limited.
# Copyright (c) 2020, Linaro Limited
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

from enum import Enum
import cbor


class SwComponent(int, Enum):
    """
    Software component property IDs specified by
    Arm's PSA Attestation API 1.0 document.
    """
    TYPE = 1
    MEASUREMENT_VALUE = 2
    VERSION = 4
    SIGNER_ID = 5
    MEASUREMENT_DESCRIPTION = 6


def create_sw_component_data(sw_type, sw_version, sw_measurement_description,
                             sw_measurement_value, sw_signer_id):

    # List of software component properties (Key ID + value)
    properties = {
        SwComponent.TYPE: sw_type,
        SwComponent.VERSION: sw_version,
        SwComponent.SIGNER_ID: sw_signer_id,
        SwComponent.MEASUREMENT_DESCRIPTION: sw_measurement_description,
    }

    # Note: The measurement value must be the last item of the property
    #       list because later it will be modified by the bootloader.
    properties[SwComponent.MEASUREMENT_VALUE] = sw_measurement_value

    return cbor.dumps(properties)
