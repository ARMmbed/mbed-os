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
import json
import click

# Default input values and pathes
PROD_NAME = 'my_thing'
CYPROV_LIB_PATH = './provisioning_lib'

# Default output values and pathes
OEM_AUDIT_NAME = 'oem_log.json'
HSM_AUDIT_NAME = 'hsm_log.json'
PROV_REQ_JWT_FILE = 'prov_req.jwt'
ROT_AUTH_JWT_FILE = 'rot_auth.jwt'
PROV_JWT_FILE = 'prov_cmd.jwt'

CUSTOMER_KEY_N = 5

sys.path.insert(0, CYPROV_LIB_PATH)
from cyprov_hsm import HsmEntity
from cyprov_oem import OemEntity
from cyprov_customer import CustomerEntity
from cyprov_crypto import crypto


def process_customer_keys(paths):
    customer_key_n = len(paths)
    customer = []
    if customer_key_n > 0:
        customer_key_id = 6
        for i in range(customer_key_n):
            customer_log = os.path.basename(paths[i])
            customer_log = os.path.splitext(customer_log)[0] + '_log.json'
            customer.append(CustomerEntity(paths[i], customer_log))
            if not customer[i].state_loaded:
                customer[i].create_entity(customer_key_id + i)
            customer[i].save_state()
    return customer


@click.command()
@click.option('--oem', 'oem_state_path',
              default='../prebuild/oem_state.json',
              help='OEM key file.')
@click.option('--hsm', 'hsm_state_path',
              default='../prebuild/hsm_state.json',
              help='HSM key file.')
@click.option('--cyboot', 'image_cert',
              default='../prebuild/CypressBootloader_CM0p.jwt',
              help='Cypress Bootloader image certificate.')
@click.option('--cyauth', 'cy_auth_path',
              default='../prebuild/cy_auth.jwt',
              help='Provisioning authorization certificate.')
@click.option('--policy', 'policy_path',
              default='policy_single_stage_CM4.json',
              help='Policy file.')
@click.option('--out', 'output_path',
              default='../packet',
              help='Output directory.')
@click.option('--ckey', 'cust_key_path',
              default=None,
              multiple=True,
              help='Customer key that will be used for image signing. Use the option multiple times '
                   'to specify multiple keys.')
@click.option('--devcert', 'dev_cert',
              default=[],
              multiple=True,
              help='Chain of trust certificate. Use the option multiple times to specify multiple certificates.')
def main(oem_state_path, hsm_state_path, image_cert, cy_auth_path,
         policy_path, output_path, cust_key_path, dev_cert):

    if len(cust_key_path) > CUSTOMER_KEY_N:
        raise Exception('Maximum number of customer keys must be {}!'.format(CUSTOMER_KEY_N))

    prod_id = PROD_NAME

    oem_audit_path = os.path.join(output_path, OEM_AUDIT_NAME)
    hsm_audit_path = os.path.join(output_path, HSM_AUDIT_NAME)
    prov_req_jwt_path = os.path.join(output_path, PROV_REQ_JWT_FILE)
    rot_auth_jwt_path = os.path.join(output_path, ROT_AUTH_JWT_FILE)
    prov_jwt_path = os.path.join(output_path, PROV_JWT_FILE)

    if not os.path.exists(output_path):
        os.makedirs(output_path)

    oem = OemEntity(oem_state_path, oem_audit_path)
    hsm = HsmEntity(hsm_state_path, hsm_audit_path)

    with open(policy_path) as f:
        json_str = f.read()
        blob = json.loads(json_str)
        blob['prod_id'] = prod_id

    customer = process_customer_keys(cust_key_path)
    if len(customer) > 0:
        blob['custom_pub_key'] = [key.get_pub_key() for key in customer]

    prov_req = oem.create_provision_request(blob)
    crypto.dump_jwt(prov_req, prov_req_jwt_path)

    rot_auth_pkg = oem.pack_rot_auth(prod_id, hsm.state["hsm_pub_key"])
    crypto.dump_jwt(rot_auth_pkg, rot_auth_jwt_path)

    prov_cmd = hsm.pack_provision_cmd(
        cy_auth=cy_auth_path,
        image_cert=image_cert,
        prov_req=prov_req,
        rot_auth=rot_auth_pkg,
        chain_of_trust=dev_cert)
    crypto.dump_jwt(prov_cmd, prov_jwt_path)

    print('#' * 80)
    print('Provisioning packet is created')
    print('#' * 80)


if __name__ == "__main__":
    main()
