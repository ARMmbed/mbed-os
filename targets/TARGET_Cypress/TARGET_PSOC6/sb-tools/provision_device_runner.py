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

import os
import sys
import click
from execute.helper import get_target_name
from execute.enums import DebugCore
from execute.provision_device import provision_execution
from execute.programmer.programmer import ProgrammingTool
from prepare.provisioning_lib.cyprov_pem import PemKey


TOOL_NAME = 'pyocd'  # Programming/debugging tool used for communication with device
ACCESS_PORT = DebugCore.debug_sys_ap  # Access port used for device provisioning


@click.command()
@click.option('--prov-jwt', 'prov_cmd_jwt',
              default='packet/prov_cmd.jwt',
              type=click.STRING,
              help='Path to provisioning JWT file (packet which contains all data necessary '
                   'for provisioning, including policy, authorization packets and keys)')
@click.option('--hex', 'cy_bootloader_hex',
              default='prebuild/CyBootloader_Release/CypressBootloader_CM0p.hex',
              type=click.STRING,
              help='Path to Cypress Bootloader HEX binary file')
@click.option('--pubkey-json', 'pub_key_json',
              default='keys/dev_pub_key.json',
              type=click.STRING,
              help='File where to save public key in JSON format')
@click.option('--pubkey-pem', 'pub_key_pem',
              default='keys/dev_pub_key.pem',
              type=click.STRING,
              help='File where to save public key in PEM format')
def main(prov_cmd_jwt, cy_bootloader_hex, pub_key_json, pub_key_pem):
    """
    Parses command line arguments and provides high level support for
    provisioning device with the specified programming tool.
    :param prov_cmd_jwt: Path to provisioning JWT file (packet which contains
           all data necessary for provisioning, including policy, authorization
           packets and keys).
    :param cy_bootloader_hex: Path to Cypress Bootloader program file.
    :param pub_key_json: File where to save public key in JSON format.
    :param pub_key_pem: File where to save public key in PEM format.
    """
    # Verify arguments
    target = get_target_name(TOOL_NAME, ACCESS_PORT)
    if not target:
        print('Invalid access port.')
        sys.exit(1)

    test_status = False
    tool = ProgrammingTool.create(TOOL_NAME)
    if tool.connect(target):
        test_status = provision_execution(tool, pub_key_json, prov_cmd_jwt, cy_bootloader_hex)

    if test_status:
        # Read device response file and take device key from it
        if os.path.exists(pub_key_json):
            pem = PemKey(pub_key_json)
            pem.save(pub_key_pem, private_key=False)
    else:
        sys.exit(1)


if __name__ == "__main__":
    main()
