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

from os.path import join, dirname
from json import load, dump
import warnings
from cmsis_pack_manager import Cache as _Cache

from tools.flash_algo import PackFlashAlgo

warnings.filterwarnings("ignore")

RootPackURL = "http://www.keil.com/pack/index.idx"

LocalPackDir = dirname(__file__)
LocalPackIndex = join(LocalPackDir, "index.json")
LocalPackAliases = join(LocalPackDir, "aliases.json")
LocalPackLegacyNames = join(LocalPackDir, "legacy-names.json")


class _CacheLookup(object):
    def __init__(self, index, legacy_names):
        self.index = index
        self.legacy_names = legacy_names

    def __getitem__(self, name):
        try:
            return self.index[name]
        except KeyError:
            return self.index[self.legacy_names[name]]

    def __contains__(self, name):
        return name in self.index or name in self.legacy_names


class Cache(object):
    """ The Cache object is the only relevant API object at the moment

    Constructing the Cache object does not imply any caching.
    A user of the API must explicitly call caching functions.

    :param silent: Not used
    :type silent: bool
    :param no_timeouts: Not used
    :type no_timeouts: bool
    """
    def __init__(self, silent, no_timeouts):
        self._cache = _Cache(
            silent, no_timeouts,
            json_path=LocalPackDir, data_path=LocalPackDir
        )
        try:
            self._legacy_names = load(open(LocalPackLegacyNames))
        except IOError:
            self._legacy_names = {}

    def _get_sectors(self, device):
        """Extract sector sizes from device FLM algorithm

        Will return None if there is no algorithm, pdsc URL formatted in
        correctly

        :return: A list tuples of sector start and size
        :rtype: [list]
        """
        try:
            pack = self._cache.pack_from_cache(device)
            ret = []
            for algo in device['algorithms']:
                try:
                    flm = pack.open(
                        algo["file_name"]
                        .replace("\\\\", "/")
                        .replace("\\", "/")
                    )
                    flash_alg = PackFlashAlgo(flm.read())
                    sectors = [(flash_alg.flash_start + offset, size)
                               for offset, size in flash_alg.sector_sizes]
                    ret.extend(sectors)
                except Exception:
                    pass
            ret.sort(key=lambda sector: sector[0])
            return ret
        except Exception:
            return None

    @property
    def index(self):
        return _CacheLookup(self._cache.index, self._legacy_names)

    def cache_descriptors(self):
        self._cache.cache_descriptors

    def cache_everything(self):
        self._cache.cache_everything()
        for name, device in self._cache.index.items():
            if name != "version":
                device["sectors"] = self._get_sectors(device)
        self.generate_index()

    def get_svd_file(self, device_name):
        """Retrieve the flash algorithm file for a particular part.

        Assumes that both the PDSC and the PACK file associated with that part
        are in the cache.

        :param device_name: The exact name of a device
        :type device_name: str
        :return: A file-like object that, when read, is the ELF file that
                 describes the flashing algorithm
        :rtype: ZipExtFile
        """
        device = self.index[device_name]
        pack = self.pack_from_cache(device)
        return pack.open(device['debug'])

    def generate_index(self):
        with open(LocalPackIndex, "w+") as out:
            self._cache.index["version"] = "0.2.0"
            dump(self._cache.index, out, indent=4, sort_keys=True)
