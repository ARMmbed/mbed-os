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
import subprocess
import binascii
from pathlib import Path, PurePath

from intelhex import IntelHex, hex2bin, bin2hex
from intelhex.compat import asbytes

HEADER_SIZE = 0x400
AES_HEADER="aes_header.txt"   # near the script file

def check_file_exist(file):
    if not Path(file).exists():
        print("ERROR: File %s not found. Check script arguments."% file)
        return False
    else:
        return True

def get_final_hex_name(file):
    """
    Determine if script is called from mbed-os build system
    for Secure Boot target processing or directly
    """
    for part in PurePath(file).parts:
        if "_unsigned.hex" in part:
            # suppose file came from mbed-os build execution
            return file[:-13] + "_enc_upgrade.hex"
    # suppose stand alone script execution
    return file[:-4] + "_enc_upgrade.hex"

def manage_output(process, input_f, output_f):
    """
    Function takes care of subprocess 
    """
    stderr = process.communicate()[1]
    rc = process.wait()

    if rc != 0:
        print("ERROR: Encryption script ended with error!")
        print("ERROR: " + stderr.decode("utf-8"))
        raise Exception("imgtool finished execution with errors!")
        
    if check_file_exist(output_f):
        os.remove(input_f)

@click.command()
@click.option('--sdk-path', 'sdk_path',
              default=Path("."),
              type=click.STRING,
              help='Path to Secure Boot tools in case running script from outside')
@click.option('--hex-file', 'hex_file',
              default=None,
              type=click.STRING,
              help='Hex file to process')
@click.option('--key-priv', 'key_priv',
              default=None,
              type=click.STRING,
              help='Private key file to use for signing BOOT or UPGRADE image')
@click.option('--key-pub', 'key_pub',
              default=None,
              type=click.STRING,
              help='Path to device public key - obtained from device on provisioning stage')
@click.option('--key-aes', 'key_aes',
              default=None,
              type=click.STRING,
              help='Path to encryption key')
@click.option('--ver', 'version',
              default=None,
              type=click.STRING,
              help='Version')
@click.option('--img-id', 'img_id',
              default=None,
              type=click.STRING,
              help='Image ID - should correspond to values, used in policy file')
@click.option('--rlb-count', 'rlb_count',
              default=None,
              type=click.STRING,
              help='Rollback counter value')
@click.option('--slot-size', 'slot_size',
              default=None,
              type=click.STRING,
              help='Size of slot available for BOOT or UPGRADE image')
@click.option('--pad', 'pad',
              default=False,
              is_flag=True,
              help='Add padding to image - required for UPGRADE image')
@click.option('--img-offset', 'img_offset',
              default=None,
              type=click.STRING,
              help='Offset of hex file for UPGRADE image')

def main(sdk_path,
            hex_file,
            key_priv,
            key_pub,
            key_aes,
            version,
            img_id,
            rlb_count,
            slot_size,
            pad,
            img_offset):
    """
    Function consequentially performs operations with provided hex file
    and produces an encrypted and signed hex file for UPGRADE
    """

    check_file_exist(key_priv)
    check_file_exist(key_pub)
    check_file_exist(key_aes)
    check_file_exist(hex_file)

    in_f = hex_file[:-4] + "_i.bin"
    out_f = hex_file[:-4] + "_o.bin"

    hex_file_final = get_final_hex_name(hex_file)
    print("Image UPGRADE:" + hex_file_final)

    # ih = IntelHex(hex_file)
    # img_start_addr = ih.start_addr['EIP']
    
    hex2bin(hex_file, in_f) #bin_file)

    # $PYTHON $IMGTOOL sign --key $KEY --header-size $HEADER_SIZE --pad-header --align 8 --version $VERSION --image-id $ID --rollback_counter $ROLLBACK_COUNTER --slot-size $SLOT_SIZE --overwrite-only $binFileName $signedFileName is_file_created $signedFileName

    # call imgtool for signature
    process = subprocess.Popen([sys.executable, sdk_path + "/imgtool/imgtool.py", "sign",
                                "--key", key_priv,
                                "--header-size", str(hex(HEADER_SIZE)),
                                "--pad-header",
                                "--align", "8",
                                "--version", version,
                                "--image-id", img_id,
                                "--rollback_counter", rlb_count,
                                "--slot-size", slot_size,
                                "--overwrite-only",
                                in_f,
                                out_f],
                                stdout=subprocess.PIPE, stderr=subprocess.PIPE)
    manage_output(process, in_f, out_f)
    
    # AES
    # $PYTHON $(dirname "${IMGTOOL}")"/create_aesHeader.py" -k $KEY -p $KEY_DEV --key_to_encrypt "$KEY_AES" $AES_HEADER
    # call aesHeader for crypto header generation
    process = subprocess.Popen([sys.executable, sdk_path + "/imgtool/create_aesHeader.py",
                                "-k", key_priv,
                                "-p", key_pub,
                                "--key_to_encrypt", key_aes,
                                AES_HEADER],
                                stdout=subprocess.PIPE, stderr=subprocess.PIPE)

    # catch stderr outputs
    stderr = process.communicate()
    rc = process.wait()
    check_file_exist(AES_HEADER)

    # aes_cipher.py script file should be in the same folder as imgtool.py    
    # $PYTHON $(dirname "${IMGTOOL}")"/aes_cipher.py" -k $KEY_AES $signedFileName $aes_encryptedFileName
    # is_file_created $aes_encryptedFileName
    # encrypt signed image
    process = subprocess.Popen([sys.executable, sdk_path + "/imgtool/aes_cipher.py",
                                "-k", key_aes,
                                out_f,
                                in_f],
                                stdout=subprocess.PIPE, stderr=subprocess.PIPE)
    manage_output(process, out_f, in_f)

    # second part - obtain signed image from encrypted file - with padding - for staging area
    # $PYTHON $IMGTOOL sign --key $KEY --header-size $HEADER_SIZE --pad-header --align 8 --version $VERSION --image-id $ID --rollback_counter $ROLLBACK_COUNTER --slot-size $SLOT_SIZE --overwrite-only $PAD -a $AES_HEADER $aes_encryptedFileName $signedEncFileName
    # is_file_created $signedEncFileName
    
    # call imgtool for signature
    process = subprocess.Popen([sys.executable, sdk_path + "/imgtool/imgtool.py", "sign",
                                "--key", key_priv,
                                "--header-size", str(hex(HEADER_SIZE)),
                                "--pad-header",
                                "--align", "8",
                                "--version", version,
                                "--image-id", img_id,
                                "--rollback_counter", rlb_count,
                                "--slot-size", slot_size,
                                "--overwrite-only",
                                "--pad",
                                "-a", AES_HEADER,
                                in_f,
                                out_f],
                                #bin_sig_enc,
                                #bin_sig_enc_sig],
                                stdout=subprocess.PIPE, stderr=subprocess.PIPE)
    manage_output(process, in_f, out_f)

    bin2hex(out_f, hex_file_final, int(img_offset))
    os.remove(out_f)

    os.remove(AES_HEADER)
    
if __name__ == "__main__":
    main()
