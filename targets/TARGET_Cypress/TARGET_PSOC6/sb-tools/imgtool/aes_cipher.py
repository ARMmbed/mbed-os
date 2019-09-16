
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

import click

from cryptography.hazmat.primitives.ciphers import Cipher, algorithms, modes
from cryptography.hazmat.primitives.ciphers.aead import AESGCM
from cryptography.hazmat.backends import default_backend
from cryptography.hazmat.primitives import padding


class AESCipher(object):

    def __init__(self, key, IV): 
        
        self.backend = default_backend()
        self.key = AESCipher.get_bytes(key)
        self.iv = AESCipher.get_bytes(IV)
        self.block_size = 128
    
    @staticmethod
    def get_bytes(inputdata):
        if type(inputdata) is str:
            return str.encode(inputdata,'utf-8')
        elif type(inputdata) is bytes:
            return inputdata
        else:
            raise Exception("Unknown input data type...")


class AESCipherCBC(AESCipher):
	
    def __init__(self, key, IV): 
        super().__init__(key, IV)
        self.cipher = Cipher(algorithms.AES(self.key), modes.CBC(self.iv), backend=self.backend)

    def encrypt(self, raw):
        encryptor = self.cipher.encryptor()
        padder = padding.PKCS7(self.block_size).padder()		
        return encryptor.update(padder.update(raw) + padder.finalize()) + encryptor.finalize()

    def decrypt(self, enc):
        decryptor = self.cipher.decryptor()
        unpadder = padding.PKCS7(self.block_size).unpadder()
        return unpadder.update(decryptor.update(enc) + decryptor.finalize()) + unpadder.finalize()


class AESCipherGCM(AESCipher):
	
    def __init__(self, key, IV, auth_data): 
        super().__init__(key, IV)
        self.cipher = AESGCM(self.key)
        self.auth_data = str.encode(auth_data,'utf-8')

    def encrypt(self, raw):
        return self.cipher.encrypt(self.iv, raw, self.auth_data)

    def decrypt(self, enc):
        return self.cipher.decrypt(self.iv, enc, self.auth_data)

def read_key_from_file(keyfile):
    
    with open(keyfile) as f:
        content = f.read().splitlines()
    if len(content) < 2:
        raise Exception("Not anough AES input data: in the file should be two lines: key, iv ...")
    key = bytes.fromhex(content[0])
    iv = bytes.fromhex(content[1])

    if 8*len(key) not in set([128, 192, 256]):
        raise Exception("Invalid AES Key length: should be 128, 192 or 256 bits")
    check_iv_length(iv)

    return key, iv

def check_iv_length(iv):
    if 8*len(iv) != 128:
        raise Exception("Invalid AES IV length: should be 128 bits")
    return True	

@click.command()
@click.option('-k', '--keyfile')
@click.option('-a', '--auth_data', default='default data')  
@click.option('-m', '--mode', default='CBC')
@click.argument('inputfile')
@click.argument('outputfile')
def main(keyfile, auth_data, mode, inputfile, outputfile):

    key, iv = read_key_from_file(keyfile)

    if mode == 'CBC':
        check_iv_length(iv)	
        aes = AESCipherCBC(key, iv)
    elif mode == 'GCM':
        aes = AESCipherGCM(key, iv, auth_data)
    else:
        raise Exception("Selected mode is not supported...")

    inFile = open(inputfile,"rb")
    outFile = open(outputfile,"wb")

    outFile.write(aes.encrypt(inFile.read()))

    inFile.close()
    outFile.close()

if __name__ == "__main__":
    main()
