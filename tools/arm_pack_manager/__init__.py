"""
Copyright (c) 2016-2019 ARM Limited. All rights reserved.

SPDX-License-Identifier: Apache-2.0

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

try:
    from urllib2 import urlopen, URLError
except ImportError:
    from urllib.request import urlopen, URLError
from bs4 import BeautifulSoup
from os.path import join, dirname, basename
from os import makedirs
from errno import EEXIST
from threading import Thread
try:
    from Queue import Queue
except ImportError:
    from queue import Queue
from re import compile, sub
from sys import stderr, stdout
from itertools import takewhile
import argparse
from json import dump, load
from zipfile import ZipFile
from tempfile import gettempdir
import warnings
from cmsis_pack_manager import Cache as _Cache
from distutils.version import LooseVersion

from tools.flash_algo import PackFlashAlgo

warnings.filterwarnings("ignore")

RootPackURL = "http://www.keil.com/pack/index.idx"

LocalPackDir = dirname(__file__)
LocalPackIndex = join(LocalPackDir, "index.json")
LocalPackAliases = join(LocalPackDir, "aliases.json")



class Cache (_Cache):
    """ The Cache object is the only relevant API object at the moment

    Constructing the Cache object does not imply any caching.
    A user of the API must explicitly call caching functions.

    :param silent: A boolean that, when True, significantly reduces the printing of this Object
    :type silent: bool
    :param no_timeouts: A boolean that, when True, disables the default connection timeout and low speed timeout for downloading things.
    :type no_timeouts: bool
    """
    def __init__ (self, silent, no_timeouts):
        super(Cache, self).__init__(
            silent, no_timeouts,
            json_path=LocalPackDir, data_path=LocalPackDir
        )

    def _get_sectors(self, device):
        """Extract sector sizes from device FLM algorithm

        Will return None if there is no algorithm, pdsc URL formatted in correctly

        :return: A list tuples of sector start and size
        :rtype: [list]
        """
        try:
            pack = self.pack_from_cache(device)
            ret = []
            for algo in device['algorithms']:
                try:
                    flm = pack.open(algo["file_name"].replace("\\\\", "/").replace("\\", "/"))
                    flash_alg = PackFlashAlgo(flm.read())
                    sectors = [(flash_alg.flash_start + offset, size)
                               for offset, size in flash_alg.sector_sizes]
                    ret.extend(sectors)
                except Exception as e:
                    print(e)
                    print(device)
                    pass
            ret.sort(key=lambda sector: sector[0])
            return ret
        except Exception as e:
            print(e)
            return None

    def cache_everything(self):
        super(Cache, self).cache_everything()
        for name, device in self.index.items():
            if name != "version":
                device["sectors"] = self._get_sectors(device)
        self.generate_index()

    def get_svd_file(self, device_name) :
        """Retrieve the flash algorithm file for a particular part.

        Assumes that both the PDSC and the PACK file associated with that part are in the cache.

        :param device_name: The exact name of a device
        :type device_name: str
        :return: A file-like object that, when read, is the ELF file that describes the flashing algorithm
        :rtype: ZipExtFile
        """
        device = self.index[device_name]
        pack = self.pack_from_cache(device)
        return pack.open(device['debug'])

    def generate_index(self):
        with open(LocalPackIndex, "wb+") as out:
            self._index["version"] = "0.2.0"
            dump(self._index, out, indent=4, sort_keys=True)
