#! /usr/bin/env python3
#
# Copyright 2017 Linaro Limited
# Copyright (c) 2018-2019, Arm Limited.
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
from imgtool_lib import keys
from imgtool_lib import image
from imgtool_lib import version
import sys
import macro_parser

sign_bin_size_re = re.compile(r"^\s*RE_SIGN_BIN_SIZE\s*=\s*(.*)")
image_load_address_re = re.compile(r"^\s*RE_IMAGE_LOAD_ADDRESS\s*=\s*(.*)")

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
    versionProvided = False
    if (version.compare(args.version, defaultVersion) == 0): # Default version
        lastVersion = get_last_version(path)
        if (lastVersion is not None):
            newVersion = version.increment_build_num(lastVersion)
        else:
            newVersion = version.increment_build_num(defaultVersion)
    else: # Version number has been explicitly provided (not using the default)
        versionProvided = True
        newVersion = args.version
    versionString = "{a}.{b}.{c}+{d}".format(
                    a=str(newVersion.major),
                    b=str(newVersion.minor),
                    c=str(newVersion.revision),
                    d=str(newVersion.build)
    )
    if not versionProvided:
        with open(path, "w") as newFile:
            newFile.write(versionString)
    print("**[INFO]** Image version number set to " + versionString)
    return newVersion

def gen_rsa2048(args):
    keys.RSAutil.generate().export_private(args.key)

def gen_rsa3072(args):
    keys.RSAutil.generate(key_size=3072).export_private(args.key)

keygens = {
        'rsa-2048': gen_rsa2048,
        'rsa-3072': gen_rsa3072, }

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

    version_num = next_version_number(args,
                                      version.decode_version("0"),
                                      "lastVerNum.txt")

    if args.security_counter is None:
        # Security counter has not been explicitly provided,
        # generate it from the version number
        args.security_counter = ((version_num.major << 24)
                                 + (version_num.minor << 16)
                                 + version_num.revision)

    if "_s.c" in args.layout:
        sw_type = "SPE"
    elif "_ns.c" in args.layout:
        sw_type = "NSPE"
    else:
        sw_type = "NSPE_SPE"

    pad_size = macro_parser.evaluate_macro(args.layout, sign_bin_size_re, 0, 1)
    img = image.Image.load(args.infile,
                           version=version_num,
                           header_size=args.header_size,
                           security_cnt=args.security_counter,
                           included_header=args.included_header,
                           pad=pad_size)
    key = keys.load(args.key, args.public_key_format) if args.key else None
    ram_load_address = macro_parser.evaluate_macro(args.layout, image_load_address_re, 0, 1)
    img.sign(sw_type, key, ram_load_address, args.dependencies)

    if pad_size:
        img.pad_to(pad_size, args.align)

    img.save(args.outfile)

subcmds = {
        'keygen': do_keygen,
        'getpub': do_getpub,
        'sign': do_sign, }


def get_dependencies(text):
    if text is not None:
        versions = []
        images = re.findall(r"\((\d+)", text)
        if len(images) == 0:
            msg = "Image dependency format is invalid: {}".format(text)
            raise argparse.ArgumentTypeError(msg)
        raw_versions = re.findall(r",\s*([0-9.+]+)\)", text)
        if len(images) != len(raw_versions):
            msg = '''There's a mismatch between the number of dependency images
            and versions in: {}'''.format(text)
            raise argparse.ArgumentTypeError(msg)
        for raw_version in raw_versions:
            try:
                versions.append(version.decode_version(raw_version))
            except ValueError as e:
                print(e)
        dependencies = dict()
        dependencies[image.DEP_IMAGES_KEY] = images
        dependencies[image.DEP_VERSIONS_KEY] = versions
        return dependencies


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
    sign.add_argument('-l', '--layout', required=True,
                      help='Location of the file that contains preprocessed macros')
    sign.add_argument('-k', '--key', metavar='filename')
    sign.add_argument("-K", "--public-key-format",
                      help='In what format to add the public key to the image manifest: full or hash',
                      metavar='pub_key_format', choices=['full', 'hash'], default='hash')
    sign.add_argument("--align", type=alignment_value, required=True)
    sign.add_argument("-v", "--version", type=version.decode_version,
                      default="0.0.0+0")
    sign.add_argument("-d", "--dependencies", type=get_dependencies,
                      required=False, help='''Add dependence on another image,
                      format: "(<image_ID>,<image_version>), ... "''')
    sign.add_argument("-s", "--security-counter", type=intparse,
                      help='Specify explicitly the security counter value')
    sign.add_argument("-H", "--header-size", type=intparse, required=True)
    sign.add_argument("--included-header", default=False, action='store_true',
                      help='Image has gap for header')
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
