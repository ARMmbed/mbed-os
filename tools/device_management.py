#! /usr/bin/env python2
"""
mbed SDK
Copyright (c) 2011-2013 ARM Limited

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.


device-management, dev-mgmt, and dm sub command
"""
from __future__ import print_function
from builtins import str
import logging
import sys
import argparse
from os.path import join, abspath, dirname, basename

from manifesttool import create, parse, verify, cert, init, update
from manifesttool.argparser import MainArgumentParser
import colorama
colorama.init()


LOG = logging.getLogger(__name__)
LOG_FORMAT='[%(levelname)s] %(asctime)s - %(name)s - %(message)s'

# Be sure that the tools directory is in the search path
ROOT = abspath(join(dirname(__file__), ".."))
sys.path.insert(0, ROOT)

from tools.config import Config
from tools.options import extract_mcus

class MbedExtendedArgs(MainArgumentParser):
    def _addCreateArgs(self, parser, exclusions=[]):
        if 'payload' not in exclusions:
            parser.add_argument('-p', '--payload',
                help='Supply a local copy of the payload file.'
                     'This option overrides any payload file supplied in a `-i` argument.',
                metavar='FILE',
                type=argparse.FileType('rb')
            )
            parser.add_argument('-m', '--mcu')
            parser.add_argument('-t', '--toolchain')
            parser.add_argument('--source', nargs='+', dest='source_dir')
            parser.add_argument('--build')
            exclusions.append('payload')
        super(MbedExtendedArgs, self)._addCreateArgs(parser, exclusions)


def wrap_payload(func):
    def inner(options):
        if (not options.payload and
            options.mcu and
            options.build
        ):
            mcus = extract_mcus(MbedExtendedArgs(), options)
            sources = options.source_dir or ['.']
            config = Config(mcus[0], sources)
            app_name = config.name or basename(abspath(sources[0]))
            output_ext = getattr(config.target, "OUTPUT_EXT", "bin")
            payload_name = join(options.build, "{}_application.{}".format(
                app_name, output_ext
            ))
            options.payload = open(payload_name, "rb")
        return func(options)
    return inner


def main():
    options = MbedExtendedArgs().parse_args().options

    log_level = {
        'debug': logging.DEBUG,
        'info': logging.INFO,
        'warning': logging.WARNING,
        'exception': logging.CRITICAL,
    }[options.log_level]
    logging.basicConfig(
        level=log_level,
        format=LOG_FORMAT,
        datefmt='%Y-%m-%d %H:%M:%S',
    )
    logging.addLevelName(
        logging.INFO,
        "\033[1;32m%s\033[1;0m" % logging.getLevelName(logging.INFO)
    )
    logging.addLevelName(
        logging.WARNING,
        "\033[1;93m%s\033[1;0m" % logging.getLevelName(logging.WARNING)
    )
    logging.addLevelName(
        logging.CRITICAL,
        "\033[1;31m%s\033[1;0m" % logging.getLevelName(logging.CRITICAL)
    )
    LOG.debug('CLIDriver created. Arguments parsed and logging setup.')

    rc = {
        "create": wrap_payload(create.main),
        "parse": parse.main,
        "verify": verify.main,
        "cert": cert.main,
        "init": init.main,
        "update" : wrap_payload(update.main),
    }[options.action](options) or 0

    sys.exit(rc)

if __name__ == "__main__":
    main()
