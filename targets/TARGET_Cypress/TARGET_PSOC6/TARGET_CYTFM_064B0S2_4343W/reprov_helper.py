#!/usr/bin/python3

"""
Copyright (c) 2020 Cypress Semiconductor Corporation

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
"""

import os
import platform
import sys, argparse
import subprocess
import json
from time import sleep

from cysecuretools import CySecureTools
from cysecuretools.execute.provisioning_lib.cyprov_pem import PemKey
from OpenSSL import crypto, SSL
from cysecuretools.execute.programmer.programmer import ProgrammingTool
from cysecuretools.execute.programmer.base import AP
from cysecuretools.core.target_director import Target
from cysecuretools.execute.programmer.pyocd_wrapper import ResetType

# Examples
# minimal parameters for default values:
# ./reprov_helper.py
#
# non-interactive:
# ./reprov_helper.py -d cys06xxa \
#                    -p policy/policy_multi_CM0_CM4_jitp.json \
#                    -[existing-keys|new-keys] -s <serial number> -y


def myargs(argv):
    parser = argparse.ArgumentParser(add_help=False)
    parser.add_argument('-h', '--help',
                        dest='show_help',
                        action='help',
                        help='Print this help message and exit')

    parser.add_argument('-p', '--policy',
                        dest='policy_file',
                        action='store',
                        type=str,
                        help="Device policy file",
                        required=False)

    parser.add_argument('-d', '--device',
                        dest='device',
                        action='store',
                        type=str,
                        help="Device manufacturing part number",
                        required=False)

    parser.add_argument('-s', '--serial',
                        dest='serial_number',
                        action='store',
                        type=str,
                        help="Device unique serial number",
                        required=False)

    parser.add_argument('-new-keys',
                        dest='new_keys',
                        action='store_true',
                        help="Create a new set keys if defined",
                        required=False)

    parser.add_argument('-existing-keys',
                        dest='existing_keys',
                        action='store_true',
                        help="Force to use existing set keys if defined",
                        required=False)

    parser.add_argument('-y',
                        dest='force_reprov',
                        action='store_true',
                        help="Force reprovisioning",
                        required=False)

    options = parser.parse_args(argv)
    return options


def create_app_keys(overwrite=None):
    cytools.create_keys(overwrite)


def read_device_pub_key():
    # Read Device Key and save
    print('Reading public key from device')
    key=cytools.read_public_key(1, "jwk")
    print("key: {}".format(key))
    if not key:
        print('Error: Cannot read device public key.')
        return 1

    pub_key_json = 'keys/device_pub_key.json'

    with open(pub_key_json, 'w') as json_file:
        json.dump(key, json_file)

    # Change from JWK to PEM
    pub_key_pem = 'keys/device_pub_key.pem'
    if os.path.exists(pub_key_json) and os.stat(pub_key_json).st_size > 0:
        pem = PemKey(pub_key_json)
        pem.save(pub_key_pem, private_key=False)
    else:
        print('Failed to read device public key')
        return 1
    print('Device public key has been read successfully.')
    return 0


def generate_device_cert(dev_serial_num,
    dev_pub_key_path="keys/device_pub_key.pem",
    ca_priv_key_path="certificates/rootCA.key",
    ca_cert_path="certificates/rootCA.pem"):

    if True:
        # read device public key from previously read from the device
        dev_pub_key = crypto.load_publickey(crypto.FILETYPE_PEM,
                                            open(dev_pub_key_path, 'r').read())
    else:
        # for development only, use public key from self generated private key
        dev_priv_key = crypto.load_privatekey(crypto.FILETYPE_ASN1,
                                              open("keys/device_priv_key.der",
                                                   'rb').read())
        dev_pub_key = crypto.load_publickey(crypto.FILETYPE_PEM,
                          crypto.dump_publickey(crypto.FILETYPE_PEM,
                                                dev_priv_key))
    ca_privatekey = crypto.load_privatekey(crypto.FILETYPE_PEM,
                                           open(ca_priv_key_path, 'r').read())
    ca_cert = crypto.load_certificate(crypto.FILETYPE_PEM,
                                      open(ca_cert_path, 'r').read())

    # create cert signed by ca_cert
    cert = crypto.X509()
    cert.set_subject(ca_cert.get_subject())
    cert.get_subject().CN = cytools.target_name.upper() + '-' + str(dev_serial_num)
    cert.set_serial_number(int(dev_serial_num))
    cert.gmtime_adj_notBefore(0)
    cert.gmtime_adj_notAfter(10*365*24*60*60)
    cert.set_issuer(ca_cert.get_subject())
    cert.set_pubkey(dev_pub_key)
    cert.sign(ca_privatekey, 'sha256')

    open("certificates/device_cert.pem", "wb")\
        .write(crypto.dump_certificate(crypto.FILETYPE_PEM, cert))
    open("certificates/device_cert.der", "wb")\
        .write(crypto.dump_certificate(crypto.FILETYPE_ASN1, cert))
    print('Device certificate generated successfully.')
    return 0


def create_provisioning_packet():
    cytools.create_provisioning_packet()


def re_provision_device(device, policy):
     cytools.re_provision_device()


def erase_flash(addr, size):
    tool = ProgrammingTool.create(cytools.PROGRAMMING_TOOL)
    tool.connect(target_name=cytools.target_name, ap='cm0')
    tool.set_ap(AP.CM0)
    print('Erasing address {hex(addr)}, size {hex(size)} ...')
    tool.erase(addr, size)
    print('Erasing complete\n')
    tool.reset(ResetType.HW)
    sleep(3)
    tool.disconnect()
    return 0


def exec_shell_command(cmd):
    if not isinstance(cmd, list) or not cmd:
        raise Exception("Command must be in a non-empty list")

    output = []
    print("Executing command: {}".format(' '.join(cmd)))
    p = subprocess.Popen(cmd, stdout=subprocess.PIPE,
                              stderr=subprocess.STDOUT)
    for line in iter(p.stdout.readline, b''):
        output.append(line.decode('utf-8'))
        print("{}".format(line.decode('utf-8')), end='')
    p.stdout.close()
    ret = p.wait()
    print("Command completed (ret={})".format(ret))
    return ret, ''.join(output)


def main(argv):

    create_signing_keys = False

    options = myargs(argv)
    print("options: {}\r\n".format(options))

    if not options.policy_file:
        options.policy_file = 'policy_multi_CM0_CM4_jitp.json'
        options.policy_file = os.path.join('policy',
                                           options.policy_file)
        answer = \
            input('Policy file name was not provided, use default {}? (Y/n): '
                   .format(options.policy_file))
        if answer == 'N' or answer == 'n':
            print("Please specify policy as a parameter: `-p <policy>`")
            exit(1)
    if not os.path.isfile(options.policy_file):
        print("Policy file {} doesn't exit.".format(options.policy_file))
        exit(1)

    if not options.device:
        options.device = "cys06xxa"
        answer = input("\r\nDevice is not provided, use default {}? (Y/n): "
                        .format(options.device))
        if answer == 'N' or answer == 'n':
            print("Please specify device as a parameter: `-d <device>'")
            exit(1)

    # Create cysecuretools object
    global cytools
    cytools = CySecureTools(options.device, options.policy_file)

    if not options.serial_number:
        dev_serial_num = \
            input("\r\nSelect unique device serial number for {} (digits only):\n"
                                .format(cytools.target_name.upper()))
        if not dev_serial_num.isnumeric():
            print('Error: device serial number not number')
            exit(1)
    else:
        dev_serial_num = options.serial_number

    # signing keys
    if not options.new_keys and not options.existing_keys:
        answer = \
            input('\r\nDo you want to create a new set of keys (y/N): ')
        if answer == 'Y' or answer == 'y':
            print('Will create new keys.')
            create_signing_keys = True
        else:
            # TBD: check if the keys exist (open json, read keys)
            print('Will use existing keys.')
    else:
        if options.new_keys:
            create_signing_keys = True
        else:
            create_signing_keys = False

    print("\r\n")
    print("##################################################################")
    print("Current configuration:")
    print("Policy file:             {}".format(options.policy_file))
    print("Device:                  {}".format(options.device))
    print("Serial Number:           {}".format(dev_serial_num))
    print("Create new signing keys: {}".format(create_signing_keys))
    print("##################################################################")
    print("\r\n")
    print("!!! Make sure the board is in the DAPLink mode (Mode LED blinks at 2Hz) !!!")
    print("\r\n")

    if not options.force_reprov:
        answer = input('Reprovision the device. Are you sure? (y/N): ')
        if answer is not 'y' and answer is not 'Y':
            print('Reprovision skipped.')
            exit(1)

    if create_signing_keys == True:
        print('Creating new signing keys.')
        create_app_keys(overwrite=True)

    # invalidate SPE image in Flash so it won't run.
    ret = erase_flash(0x10000000, 0x1000)
    if ret != 0:
        exit(1)

    ret = read_device_pub_key()
    if ret != 0:
        exit(1)

    ret = generate_device_cert(dev_serial_num)
    if ret != 0:
        exit(1)

    create_provisioning_packet()

    re_provision_device(options.device, options.policy_file)

    exit(0)


if __name__ == "__main__":
    main(sys.argv[1:])
