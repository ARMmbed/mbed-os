# Copyright 2019 Cypress Semiconductor Corporation
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

TEST_BIN = "test_data/test.bin"
BLINKY_SMALL_HEX = "test_data/blinky_small.hex"
BLINKY_LARGE_HEX = "test_data/blinky_large.hex"


def create_test_bin():
    with open(TEST_BIN, 'wb') as file:
        file.write(b'A1A2A3B1B2B3111213FFA1A2A3B1B2B3111213FFA1A2A3B1B2B3111213FFA1A2A3B1B2B3111213FF'
                   b'A1A2A3B1B2B3111213FFA1A2A3B1B2B3111213FFA1A2A3B1B2B3111213FFA1A2A3B1B2B3111213FF'
                   b'A1A2A3B1B2B3111213FFA1A2A3B1B2B3111213FFA1A2A3B1B2B3111213FFA1A2A3B1B2B3111213FF'
                   b'A1A2A3B1B2B3111213FFA1A2A3B1B2B3111213FFA1A2A3B1B2B3111213FFA1A2A3B1B2B3111213FF'
                   b'A1A2A3B1B2B3111213FFA1A2A3B1B2B3111213FFA1A2A3B1B2B3111213FFA1A2A3B1B2B3111213FF'
                   b'A1A2A3B1B2B3111213FFA1A2A3B1B2B3111213FFA1A2A3B1B2B3111213FFA1A2A3B1B2B3111213FF'
                   b'A1A2A3B1B2B3111213FFA1A2A3B1B2B')


def decomposite32(value):
    """
    Decomposites 32-bit value into byte array.
    :param value: 32-bit value.
    :return: Array of bytes.
    """
    byte = value.to_bytes(4, 'little')
    return byte
