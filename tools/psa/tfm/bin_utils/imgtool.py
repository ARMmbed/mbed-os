#! /usr/bin/env python3
#
# Copyright 2017 Linaro Limited
# Copyright (c) 2018, Arm Limited.
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
import re
import argparse
from .imgtool_lib import keys
from .imgtool_lib import image
from .imgtool_lib import version
import sys

def find_load_address(args):
    load_address_re = re.compile(r"^#define\sIMAGE_LOAD_ADDRESS\s+(0x[0-9a-fA-F]+)")

    if os.path.isabs(args.layout):
            configFile = args.layout
    else:
        scriptsDir = os.path.dirname(os.path.abspath(__file__))
        configFile = os.path.join(scriptsDir, args.layout)

    ramLoadAddress = None
    with open(configFile, 'r') as flash_layout_file:
        for line in flash_layout_file:
            m = load_address_re.match(line)
            if m is not None:
                ramLoadAddress = int(m.group(1), 0)
                print("**[INFO]** Writing load address from the macro in "
                      "flash_layout.h to the image header.. "
                       + hex(ramLoadAddress)
                       + " (dec. " + str(ramLoadAddress) + ")")
                break
    return ramLoadAddress

# Returns the last version number if present, or None if not
def get_last_version(path):
    if (os.path.isfile(path) == False): # Version file not present
        return None
    else: # Version file is present, check it has a valid number inside it
        with open(path, "r") as oldFile:
            fileContents = oldFile.read()
            if version.version_re.match(fileContents): # number is valid
                return version.decode_version(fileContents)
            else:
                return None

def next_version_number(args, defaultVersion, path):
    newVersion = None
    if (version.compare(args.version, defaultVersion) == 0): # Default version
        lastVersion = get_last_version(path)
        if (lastVersion is not None):
            newVersion = version.increment_build_num(lastVersion)
        else:
            newVersion = version.increment_build_num(defaultVersion)
    else: # Version number has been explicitly provided (not using the default)
        newVersion = args.version
    versionString = "{a}.{b}.{c}+{d}".format(
                    a=str(newVersion.major),
                    b=str(newVersion.minor),
                    c=str(newVersion.revision),
                    d=str(newVersion.build)
    )
    with open(path, "w") as newFile:
        newFile.write(versionString)
    print("**[INFO]** Image version number set to " + versionString)
    return newVersion

def gen_rsa2048(args):
    keys.RSA2048.generate().export_private(args.key)

keygens = {
        'rsa-2048': gen_rsa2048, }

def do_keygen(args):
    if args.type not in keygens:
        msg = "Unexpected key type: {}".format(args.type)
        raise argparse.ArgumentTypeError(msg)
    keygens[args.type](args)

def do_getpub(args):
    key = keys.load(args.key)
    if args.lang == 'c':
        key.emit_c()
    else:
        msg = "Unsupported language, valid are: c"
        raise argparse.ArgumentTypeError(msg)

def do_sign(args):
    if args.rsa_pkcs1_15:
        keys.sign_rsa_pss = False
    img = image.Image.load(args.infile,
            version=next_version_number(args,
                                        version.decode_version("0"),
                                        "lastVerNum.txt"),
            header_size=args.header_size,
            included_header=args.included_header,
            pad=args.pad)
    key = keys.load(args.key) if args.key else None
    img.sign(key, find_load_address(args))

    if args.pad:
        img.pad_to(args.pad, args.align)

    img.save(args.outfile)

subcmds = {
        'keygen': do_keygen,
        'getpub': do_getpub,
        'sign': do_sign, }

def alignment_value(text):
    value = int(text)
    if value not in [1, 2, 4, 8]:
        msg = "{} must be one of 1, 2, 4 or 8".format(value)
        raise argparse.ArgumentTypeError(msg)
    return value

def intparse(text):
    """Parse a command line argument as an integer.

    Accepts 0x and other prefixes to allow other bases to be used."""
    return int(text, 0)

def args():
    parser = argparse.ArgumentParser()
    subs = parser.add_subparsers(help='subcommand help', dest='subcmd')

    keygenp = subs.add_parser('keygen', help='Generate pub/private keypair')
    keygenp.add_argument('-k', '--key', metavar='filename', required=True)
    keygenp.add_argument('-t', '--type', metavar='type',
                         choices=keygens.keys(), required=True)

    getpub = subs.add_parser('getpub', help='Get public key from keypair')
    getpub.add_argument('-k', '--key', metavar='filename', required=True)
    getpub.add_argument('-l', '--lang', metavar='lang', default='c')

    sign = subs.add_parser('sign', help='Sign an image with a private key')
    sign.add_argument('--layout', required=True,
                      help='Location of the memory layout file')
    sign.add_argument('-k', '--key', metavar='filename')
    sign.add_argument("--align", type=alignment_value, required=True)
    sign.add_argument("-v", "--version", type=version.decode_version,
                      default="0.0.0+0")
    sign.add_argument("-H", "--header-size", type=intparse, required=True)
    sign.add_argument("--included-header", default=False, action='store_true',
                      help='Image has gap for header')
    sign.add_argument("--pad", type=intparse,
                      help='Pad image to this many bytes, adding trailer magic')
    sign.add_argument("--rsa-pkcs1-15",
                      help='Use old PKCS#1 v1.5 signature algorithm',
                      default=False, action='store_true')
    sign.add_argument("infile")
    sign.add_argument("outfile")

    args = parser.parse_args()
    if args.subcmd is None:
        print('Must specify a subcommand', file=sys.stderr)
        sys.exit(1)

    subcmds[args.subcmd](args)

if __name__ == '__main__':
    args()