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

import codecs
import click

from aes_cipher import *
from ecc_kdf import ECDH_KDF

def get_header_info(kdf_object, key_to_encrypt_file):

    aes = AESCipherCBC(kdf_object.aes_key, kdf_object.iv)
    #print (kdf_object.aes_key.hex())

    key_to_enc, iv_to_enc = read_key_from_file(key_to_encrypt_file)
    key_encrypted = aes.encrypt(key_to_enc + iv_to_enc)

    if kdf_object.salt is None or kdf_object.info is None:
        raise Exception('salt and info should be presented...')
    if len(kdf_object.salt) != 16 or len(kdf_object.info) != 16:
        raise Exception('salt and info fields length should be 16 bytes...')

    return key_encrypted + kdf_object.salt + kdf_object.info


@click.command()
@click.option('-a', '--algorithm', default='ECC')           #assymetric algorithm for KDF
@click.option('-k', '--private_key')                        #host side key pair file
@click.option('-p', '--public_key')                         #device side public key
@click.option('-l', '--key_length', default=16)             #derived key (AES) length
@click.option('-s', '--salt', default=None)                 #salt for KDF
@click.option('-i', '--info', default=b'_handshake_data_')  #info data for KDF
@click.option('--key_to_encrypt')                           #AES key file name (key and iv are used for image encryption), should be AES encrypted, using derived key
@click.argument('outputfile')                               #AES_header info file name

def main(algorithm, private_key, public_key, key_length, salt, info, key_to_encrypt, outputfile):
    kdf_object = None
    if(algorithm == 'ECC'):
        kdf_object = ECDH_KDF(private_key, public_key, key_length, salt, info)
    else:
        raise Exception('Algorithm not supported...')

    aes_header = get_header_info(kdf_object, key_to_encrypt)

    with open(outputfile, 'wb') as header_out:
        header_out.write(aes_header)

if __name__ == "__main__":
    main()
