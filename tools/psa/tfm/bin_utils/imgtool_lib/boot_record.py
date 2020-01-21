
# Copyright (c) 2019, Arm Limited.
#
# SPDX-License-Identifier: Apache-2.0
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

from __future__ import print_function
import os
import sys
import cbor


# SW component IDs
SW_COMPONENT_RANGE = 0
SW_COMPONENT_TYPE = SW_COMPONENT_RANGE + 1
MEASUREMENT_VALUE = SW_COMPONENT_RANGE + 2
SW_COMPONENT_VERSION = SW_COMPONENT_RANGE + 4
SIGNER_ID = SW_COMPONENT_RANGE + 5
MEASUREMENT_DESCRIPTION = SW_COMPONENT_RANGE + 6


def create_sw_component_data(sw_type, sw_version, sw_measurement_type,
                             sw_measurement_value, sw_signer_id):

    # List of SW component claims (key ID + value)
    key_value_list = [
        SW_COMPONENT_TYPE, sw_type,
        SW_COMPONENT_VERSION, sw_version,
        SIGNER_ID, sw_signer_id,
        MEASUREMENT_DESCRIPTION, sw_measurement_type,
        MEASUREMENT_VALUE, sw_measurement_value
    ]
    # The measurement value should be the last item (key + value) in the list
    # to make it easier to modify its value later in the bootloader.
    # A dictionary would be the best suited data structure to store these
    # key-value pairs (claims), however dictionaries are not sorted, but for
    # example the lists do keep to order of items which we care about now.
    # An ordered dictionary could be used instead, but it would be converted
    # to a dict before the encoding and this conversion may not keep the order
    # of the items.

    if (len(key_value_list) % 2) != 0:
        print('Error: The length of the sw component claim list must '
              'be even (key + value).', file=sys.stderr)
        sys.exit(1)
    else:
        claim_number = (int)(len(key_value_list) / 2)

    # The output of this function must be a CBOR encoded map (dictionary) of
    # the SW component claims. The CBOR representation of an array and a map
    # (dictionary) is quite similar. To convert the encoded list to a map, it
    # is enough to modify the first byte (CBOR data item header) of the
    # data. This applies up to 23 items (11 claims in this case) - until the 5
    # lower bits of the item header are used as an item count specifier.

    if claim_number > 11:
        print('Error: There are more than 11 claims in the '
              'list of sw component claims.', file=sys.stderr)
        sys.exit(1)

    record_array = bytearray(cbor.dumps(key_value_list))
    # Modify the CBOR data item header (from array to map)
    # 7..5 bits : Major type
    #             Array - 0x80
    #             Map   - 0xA0
    # 4..0 bits : Number of items
    record_array[0] = 0xA0 + claim_number

    return bytes(record_array)
