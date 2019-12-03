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
from __future__ import print_function, absolute_import
import logging
import sys
import argparse
from os.path import join, abspath, dirname, basename

from manifesttool import create, parse, verify, cert, init, update
from manifesttool.argparser import MainArgumentParser
from mbed_cloud import AccountManagementAPI, CertificatesAPI
import colorama
colorama.init()

LOG = logging.getLogger(__name__)
LOG_FORMAT = '[%(levelname)s] %(asctime)s - %(name)s - %(message)s'

# Be sure that the tools directory is in the search path
ROOT = abspath(join(dirname(__file__), ".."))
sys.path.insert(0, ROOT)

from tools.config import Config
from tools.options import extract_mcus
from tools.utils import generate_update_filename


class MbedExtendedArgs(MainArgumentParser):

    def __init__(self, *args, **kwargs):
        MainArgumentParser.__init__(self, *args, **kwargs)
        self.parser.prog = "mbed device-management"
        self.parser.description = (
            "Create or transform a manifest. "
            "Use {} [command] -h for help on each command."
        ).format(self.parser.prog)
        for initIndex in range(len(self.parser._subparsers._actions)):
            try:
                if 'init' in (self.parser._subparsers._actions[initIndex]).choices:
                    initParser = self.parser._subparsers._actions[initIndex].choices['init']
            except TypeError:
                pass
        initParser.add_argument('--no-developer-cert', help='Do not download the developer certificate')

    def _addCreateArgs(self, parser, exclusions=[]):
        if 'payload' not in exclusions:
            parser.add_argument(
                '-p', '--payload',
                help='Supply a local copy of the payload file.'
                     'This option overrides any payload file supplied in a '
                     '`-i` argument.',
                metavar='FILE',
                type=argparse.FileType('rb')
            )
            parser.add_argument('-m', '--mcu')
            parser.add_argument('-t', '--toolchain')
            parser.add_argument('--source', nargs='+', dest='source_dir')
            parser.add_argument('--custom-targets', dest='custom_targets_directory')
            parser.add_argument('--build')
            exclusions.append('payload')
        super(MbedExtendedArgs, self)._addCreateArgs(parser, exclusions)


def wrap_payload(func):
    def inner(options):
        if not options.payload and options.mcu and options.build:
            mcus = extract_mcus(MbedExtendedArgs(), options)
            sources = options.source_dir or ['.']
            config = Config(mcus[0], sources)
            app_name = config.name or basename(abspath(sources[0]))
            payload_name = join(options.build, generate_update_filename(app_name, config.target))
            options.payload = open(payload_name, "rb")
        if not options.payload:
            LOG.error("No Payload specified. Please use \"-t\" and \"-m\" or \"-p\" to specify a payload ")
            exit(1)
        return func(options)
    return inner


def wrap_init(func):
    def inner(options):
        config = {}
        if getattr(options, 'api_key'):
            config["api_key"] = options.api_key
        if getattr(options, 'server_address'):
            config["host"] = options.server_address

        try:
            accounts = AccountManagementAPI(config)
            certs = CertificatesAPI(config)
        except Exception as e:
            LOG.error(
                'Missing api key. Set it with '
                '"mbed config -G CLOUD_SDK_API_KEY <api key>"'
            )
            exit(1)

        # Get the currently in-use API key (may come from environment or
        # configuration files, which is handled by the cloud SDK)
        api_key_value = accounts.config.get("api_key")
        api_key = next(accounts.list_api_keys(
            filter={
                "key": api_key_value
            }
        ))
        certificates_owned = list(certs.list_certificates())
        dev_cert_info = None
        for certif in certificates_owned:
            if certif.type == "developer" and (certif.owner_id == api_key.owner_id or
                                               certif.owner_id == api_key.id):
                dev_cert_info = certs.get_certificate(certif.id)
                LOG.info("Found developer certificate named %s",
                         dev_cert_info.name)
                break
        else:
            LOG.warning(
                "Could not find developer certificate for this account."
                " Generting a new developer certificate."
            )
            dev_cert_info = CertificatesAPI().add_developer_certificate(
                "mbed-cli-auto {}".format(api_key.name),
                description="cetificate auto-generated by Mbed CLI"
            )
        if getattr(options, 'no_developer_cert'):
            LOG.info("Skipping download of developer certificate")
        else:
            LOG.info("Writing developer certificate %s into c file "
                 "mbed_cloud_dev_credentials.c", dev_cert_info.name)
            with open("mbed_cloud_dev_credentials.c", "w") as fout:
                fout.write(dev_cert_info.header_file)
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
        "init": wrap_init(init.main),
        "update": wrap_payload(update.main),
    }[options.action](options) or 0

    sys.exit(rc)

if __name__ == "__main__":
    main()
