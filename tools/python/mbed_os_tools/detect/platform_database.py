# Copyright (c) 2018, Arm Limited and affiliates.
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

"""Functions that manage a platform database"""

import datetime
import json
import re
from collections import OrderedDict, defaultdict
from copy import copy
from io import open
from os import makedirs
from os.path import join, dirname, getmtime
from appdirs import user_data_dir
from fasteners import InterProcessLock

import logging

logger = logging.getLogger("mbedls.platform_database")
logger.addHandler(logging.NullHandler())
del logging

try:
    unicode
except NameError:
    unicode = str

LOCAL_PLATFORM_DATABASE = join(user_data_dir("mbedls"), "platforms.json")
LOCAL_MOCKS_DATABASE = join(user_data_dir("mbedls"), "mock.json")

DEFAULT_PLATFORM_DB = {
    u"daplink": {
        u"0200": u"KL25Z",
        u"0201": u"KW41Z",
        u"0210": u"KL05Z",
        u"0214": u"HEXIWEAR",
        u"0217": u"K82F",
        u"0218": u"KL82Z",
        u"0220": u"KL46Z",
        u"0227": u"MIMXRT1050_EVK",
        u"0228": u"RAPIDIOT_K64F",
        u"0230": u"K20D50M",
        u"0231": u"K22F",
        u"0234": u"RAPIDIOT_KW41Z",
        u"0236": u"LPC55S69",
        u"0240": u"K64F",
        u"0245": u"K64F",
        u"0250": u"KW24D",
        u"0261": u"KL27Z",
        u"0262": u"KL43Z",
        u"0300": u"MTS_GAMBIT",
        u"0305": u"MTS_MDOT_F405RG",
        u"0310": u"MTS_DRAGONFLY_F411RE",
        u"0311": u"K66F",
        u"0312": u"MTS_DRAGONFLY_L471QG",
        u"0313": u"MTS_DRAGONFLY_L496VG",
        u"0315": u"MTS_MDOT_F411RE",
        u"0316": u"MTS_DRAGONFLY_F413RH",
        u"0350": u"XDOT_L151CC",
        u"0360": u"HANI_IOT",
        u"0400": u"MAXWSNENV",
        u"0405": u"MAX32600MBED",
        u"0407": u"MAX32620HSP",
        u"0408": u"MAX32625NEXPAQ",
        u"0409": u"MAX32630FTHR",
        u"0410": u"ETTEPLAN_LORA",
        u"0415": u"MAX32625MBED",
        u"0416": u"MAX32625PICO",
        u"0418": u"MAX32620FTHR",
        u"0419": u"MAX35103EVKIT2",
        u"0421": u"MAX32660EVSYS",
        u"0424": u"MAX32670EVKIT",
        u"0450": u"MTB_UBLOX_ODIN_W2",
        u"0451": u"MTB_MXCHIP_EMW3166",
        u"0452": u"MTB_LAIRD_BL600",
        u"0453": u"MTB_MTS_XDOT",
        u"0454": u"MTB_MTS_DRAGONFLY",
        u"0455": u"MTB_UBLOX_NINA_B1",
        u"0456": u"MTB_MURATA_ABZ",
        u"0457": u"MTB_RAK811",
        u"0458": u"ADV_WISE_1510",
        u"0459": u"ADV_WISE_1530",
        u"0460": u"ADV_WISE_1570",
        u"0461": u"MTB_LAIRD_BL652",
        u"0462": u"MTB_USI_WM_BN_BM_22",
        u"0465": u"MTB_LAIRD_BL654",
        u"0466": u"MTB_MURATA_WSM_BL241",
        u"0467": u"MTB_STM_S2LP",
        u"0468": u"MTB_STM_L475",
        u"0469": u"MTB_STM32_F439",
        u"0472": u"MTB_ACONNO_ACN52832",
        u"0602": u"EV_COG_AD3029LZ",
        u"0603": u"EV_COG_AD4050LZ",
        u"0604": u"SDP_K1",
        u"0700": u"NUCLEO_F103RB",
        u"0705": u"NUCLEO_F302R8",
        u"0710": u"NUCLEO_L152RE",
        u"0715": u"NUCLEO_L053R8",
        u"0720": u"NUCLEO_F401RE",
        u"0725": u"NUCLEO_F030R8",
        u"0729": u"NUCLEO_G071RB",
        u"0730": u"NUCLEO_F072RB",
        u"0735": u"NUCLEO_F334R8",
        u"0740": u"NUCLEO_F411RE",
        u"0742": u"NUCLEO_F413ZH",
        u"0743": u"DISCO_F413ZH",
        u"0744": u"NUCLEO_F410RB",
        u"0745": u"NUCLEO_F303RE",
        u"0746": u"DISCO_F303VC",
        u"0747": u"NUCLEO_F303ZE",
        u"0750": u"NUCLEO_F091RC",
        u"0755": u"NUCLEO_F070RB",
        u"0760": u"NUCLEO_L073RZ",
        u"0764": u"DISCO_L475VG_IOT01A",
        u"0765": u"NUCLEO_L476RG",
        u"0766": u"SILICA_SENSOR_NODE",
        u"0770": u"NUCLEO_L432KC",
        u"0774": u"DISCO_L4R9I",
        u"0775": u"NUCLEO_F303K8",
        u"0776": u"NUCLEO_L4R5ZI",
        u"0777": u"NUCLEO_F446RE",
        u"0778": u"NUCLEO_F446ZE",
        u"0779": u"NUCLEO_L433RC_P",
        u"0780": u"NUCLEO_L011K4",
        u"0781": u"NUCLEO_L4R5ZI_P",
        u"0783": u"NUCLEO_L010RB",
        u"0785": u"NUCLEO_F042K6",
        u"0788": u"DISCO_F469NI",
        u"0790": u"NUCLEO_L031K6",
        u"0791": u"NUCLEO_F031K6",
        u"0795": u"DISCO_F429ZI",
        u"0796": u"NUCLEO_F429ZI",
        u"0797": u"NUCLEO_F439ZI",
        u"0805": u"DISCO_L053C8",
        u"0810": u"DISCO_F334C8",
        u"0812": u"NUCLEO_F722ZE",
        u"0813": u"NUCLEO_H743ZI",
        u"0814": u"DISCO_H747I",
        u"0815": u"DISCO_F746NG",
        u"0816": u"NUCLEO_F746ZG",
        u"0817": u"DISCO_F769NI",
        u"0818": u"NUCLEO_F767ZI",
        u"0820": u"DISCO_L476VG",
        u"0821": u"NUCLEO_L452RE",
        u"0822": u"DISCO_L496AG",
        u"0823": u"NUCLEO_L496ZG",
        u"0824": u"LPC824",
        u"0825": u"DISCO_F412ZG",
        u"0826": u"NUCLEO_F412ZG",
        u"0827": u"NUCLEO_L486RG",
        u"0828": u"NUCLEO_L496ZG_P",
        u"0829": u"NUCLEO_L452RE_P",
        u"0830": u"DISCO_F407VG",
        u"0833": u"DISCO_L072CZ_LRWAN1",
        u"0835": u"NUCLEO_F207ZG",
        u"0836": u"NUCLEO_H743ZI2",
        u"0839": u"NUCLEO_WB55RG",
        u"0840": u"B96B_F446VE",
        u"0841": u"NUCLEO_G474RE",
        u"0842": u"NUCLEO_H753ZI",
        u"0843": u"NUCLEO_H745ZI_Q",
        u"0844": u"NUCLEO_H755ZI_Q",
        u"0847": u"DISCO_H745I",
        u"0849": u"NUCLEO_G070RB",
        u"0850": u"NUCLEO_G431RB",
        u"0851": u"NUCLEO_G431KB",
        u"0852": u"NUCLEO_G031K8",
        u"0853": u"NUCLEO_F301K8",
        u"0854": u"NUCLEO_L552ZE_Q",
        u"0855": u"DISCO_L562QE",
        u"0858": u"DISCO_H750B",
        u"0859": u"DISCO_H7B3I",
        u"0860": u"NUCLEO_H7A3ZI_Q",
        u"0863": u"DISCO_L4P5G",
        u"0865": u"NUCLEO_L4P5ZG",
        u"0866": u"NUCLEO_WL55JC",
        u"0871": u"NUCLEO_H723ZG",
        u"0872": u"NUCLEO_G0B1RE",
        u"0875": u"DISCO_H735G",
        u"0879": u"NUCLEO_F756ZG",
        u"0882": u"NUCLEO_G491RE",
        u"0883": u"NUCLEO_WB15CC",
        u"0884": u"DISCO_WB5MMG",
        u"0885": u"B_L4S5I_IOT01A",
        u"0886": u"NUCLEO_U575ZI_Q",
        u"0887": u"B_U585I_IOT02A",
        u"0900": u"SAMR21G18A",
        u"0905": u"SAMD21G18A",
        u"0910": u"SAML21J18A",
        u"0915": u"SAMD21J18A",
        u"1000": u"LPC2368",
        u"1010": u"LPC1768",
        u"1017": u"HRM1017",
        u"1018": u"SSCI824",
        u"1019": u"TY51822R3",
        u"1022": u"RO359B",
        u"1034": u"LPC11U34",
        u"1040": u"LPC11U24",
        u"1045": u"LPC11U24",
        u"1050": u"LPC812",
        u"1054": u"LPC54114",
        u"1056": u"LPC546XX",
        u"1060": u"LPC4088",
        u"1061": u"LPC11U35_401",
        u"1062": u"LPC4088_DM",
        u"1070": u"NRF51822",
        u"1075": u"NRF51822_OTA",
        u"1080": u"OC_MBUINO",
        u"1090": u"RBLAB_NRF51822",
        u"1093": u"RBLAB_BLENANO2",
        u"1095": u"RBLAB_BLENANO",
        u"1100": u"NRF51_DK",
        u"1101": u"NRF52_DK",
        u"1102": u"NRF52840_DK",
        u"1105": u"NRF51_DK_OTA",
        u"1114": u"LPC1114",
        u"1120": u"NRF51_DONGLE",
        u"1130": u"NRF51822_SBK",
        u"1140": u"WALLBOT_BLE",
        u"1168": u"LPC11U68",
        u"1200": u"NCS36510",
        u"1234": u"UBLOX_C027",
        u"1236": u"UBLOX_EVK_ODIN_W2",
        u"1237": u"UBLOX_EVK_NINA_B1",
        u"1280": u"OKDO_ODIN_W2",
        u"1300": u"NUC472-NUTINY",
        u"1301": u"NUMBED",
        u"1302": u"NUMAKER_PFM_NUC472",
        u"1303": u"NUMAKER_PFM_M453",
        u"1304": u"NUMAKER_PFM_M487",
        u"1305": u"NU_PFM_M2351",
        u"1306": u"NUMAKER_PFM_NANO130",
        u"1307": u"NUMAKER_PFM_NUC240",
        u"1308": u"NUMAKER_IOT_M487",
        u"1309": u"NUMAKER_IOT_M252",
        u"1310": u"NUMAKER_IOT_M263A",
        u"1312": u"NU_M2354",
        u"1313": u"NUMAKER_IOT_M467",
        u"1500": u"RHOMBIO_L476DMW1K",
        u"1549": u"LPC1549",
        u"1600": u"LPC4330_M4",
        u"1605": u"LPC4330_M4",
        u"1701": u"GD32_F307VG",
        u"1702": u"GD32_F450ZI",
        u"1703": u"GD32_E103VB",
        u'1900': u'CY8CKIT_062_WIFI_BT',
        u'1901': u'CY8CPROTO_062_4343W',
        u'1902': u'CY8CKIT_062_BLE',
        u'1903': u'CYW9P62S1_43012EVB_01',
        u'1904': u'CY8CPROTO_063_BLE',
        u'1905': u'CY8CKIT_062S2_4343W',
        u'1906': u'CYW943012P6EVB_01',
        u'1907': u'CY8CPROTO_064_SB',
        u'1908': u'CYW9P62S1_43438EVB_01',
        u'1909': u'CY8CPROTO_062S2_43012',
        u'190A': u'CY8CKIT_064S2_4343W',
        u'190B': u'CY8CKIT_062S2_43012',
        u'190C': u'CY8CPROTO_064B0S3',
        u'190E': u'CY8CPROTO_062S3_4343W',
        u'190F': u'CY8CPROTO_064B0S1_BLE',
        u'1910': u'CY8CKIT064B0S2_4343W',
        u'1911': u'CY8CKIT064S0S2_4343W',
        u'1912': u'CYSBSYSKIT_01',
        u"2000": u"EFM32_G8XX_STK",
        u"2005": u"EFM32HG_STK3400",
        u"2010": u"EFM32WG_STK3800",
        u"2015": u"EFM32GG_STK3700",
        u"2020": u"EFM32LG_STK3600",
        u"2025": u"EFM32TG_STK3300",
        u"2030": u"EFM32ZG_STK3200",
        u"2035": u"EFM32PG_STK3401",
        u"2040": u"EFM32PG12_STK3402",
        u"2041": u"TB_SENSE_12",
        u"2042": u"EFM32GG11_STK3701",
        u"2043": u"EFM32TG11_STK3301",
        u"2045": u"TB_SENSE_1",
        u"2100": u"XBED_LPC1768",
        u"2201": u"WIZWIKI_W7500",
        u"2202": u"WIZWIKI_W7500ECO",
        u"2203": u"WIZWIKI_W7500P",
        u"2600": u"EP_AGORA",
        u"3001": u"LPC11U24",
        u"3101": u"SDT32620B",
        u"3102": u"SDT32625B",
        u"3103": u"SDT51822B",
        u"3104": u"SDT52832B",
        u"3105": u"SDT64B",
        u"3701": u"S5JS100",
        u"3702": u"S3JT100",
        u"3703": u"S1SBP6A",
        u"4000": u"LPC11U35_Y5_MBUG",
        u"4005": u"NRF51822_Y5_MBUG",
        u"4100": u"MOTE_L152RC",
        u"4337": u"LPC4337",
        u"4500": u"DELTA_DFCM_NNN40",
        u"4501": u"DELTA_DFBM_NQ620",
        u"4502": u"DELTA_DFCM_NNN50",
        u"4600": u"REALTEK_RTL8195AM",
        u"5000": u"ARM_MPS2",
        u"5001": u"ARM_IOTSS_BEID",
        u"5002": u"ARM_BEETLE_SOC",
        u"5003": u"ARM_MPS2_M0P",
        u"5004": u"ARM_CM3DS_MPS2",
        u"5005": u"ARM_MPS2_M0DS",
        u"5006": u"ARM_MUSCA_A1",
        u"5007": u"ARM_MUSCA_B1",
        u"5009": u"ARM_MUSCA_S1",
        u"5020": u"HOME_GATEWAY_6LOWPAN",
        u"5500": u"RZ_A1H",
        u"5501": u"GR_LYCHEE",
        u"5502": u"GR_MANGO",
        u"6000": u"FUTURE_SEQUANA",
        u"6660": u"NZ32_SC151",
        u"7011": u"TMPM066",
        u"7012": u"TMPM3H6",
        u"7013": u"TMPM46B",
        u"7014": u"TMPM3HQ",
        u"7015": u"TMPM4G9",
        u"7020": u"TMPM4KN",
        u"7402": u"MBED_BR_HAT",
        u"7778": u"TEENSY3_1",
        u"8001": u"UNO_91H",
        u"8012": u"TT_M3HQ",
        u"8013": u"TT_M4G9",
        u"8080": u"FF1705_L151CC",
        u"8081": u"FF_LPC546XX",
        u"9001": u"LPC1347",
        u"9002": u"LPC11U24",
        u"9003": u"LPC1347",
        u"9004": u"ARCH_PRO",
        u"9006": u"LPC11U24",
        u"9007": u"LPC11U35_501",
        u"9008": u"XADOW_M0",
        u"9009": u"ARCH_BLE",
        u"9010": u"ARCH_GPRS",
        u"9011": u"ARCH_MAX",
        u"9012": u"SEEED_TINY_BLE",
        u"9014": u"WIO_3G",
        u"9015": u"WIO_BG96",
        u"9017": u"WIO_EMW3166",
        u"9020": u"UHURU_RAVEN",
        u"9900": u"NRF51_MICROBIT",
        u"C002": u"VK_RZ_A1H",
        u"C005": u"MTM_MTCONNECT04S",
        u"C006": u"VBLUNO51",
        u"C008": u"SAKURAIO_EVB_01",
        u"C030": u"UBLOX_C030_U201",
        u"C031": u"UBLOX_C030_N211",
        u"C032": u"UBLOX_C030_R404M",
        u"C033": u"UBLOX_C030_R410M",
        u"C034": u"UBLOX_C030_S200",
        u"C035": u"UBLOX_C030_R3121",
        u"C036": u"UBLOX_C030_R412M",
        u"RIOT": u"RIOT",
    },
    u"jlink": {
        u"X729475D28G": {
            u"platform_name": u"NRF51_DK",
            u"jlink_device_name": u"nRF51422_xxAC",
        },
        u"X349858SLYN": {
            u"platform_name": u"NRF52_DK",
            u"jlink_device_name": u"nRF52832_xxaa",
        },
        u"FRDM-KL25Z": {
            u"platform_name": u"KL25Z",
            u"jlink_device_name": u"MKL25Z128xxx4",
        },
        u"FRDM-KL27Z": {
            u"platform_name": u"KL27Z",
            u"jlink_device_name": u"MKL27Z64xxx4",
        },
        u"FRDM-KL43Z": {
            u"platform_name": u"KL43Z",
            u"jlink_device_name": u"MKL43Z256xxx4",
        },
    },
    u"atmel": {u"2241": "SAML21J18A"},
}


def _get_modified_time(path):
    try:
        mtime = getmtime(path)
    except OSError:
        mtime = 0
    return datetime.datetime.fromtimestamp(mtime)


def _older_than_me(path):
    return _get_modified_time(path) < _get_modified_time(__file__)


def _modify_data_format(data, verbose_data, simple_data_key="platform_name"):
    if isinstance(data, dict):
        if verbose_data:
            return data

        return data[simple_data_key]
    else:
        if verbose_data:
            return {simple_data_key: data}

        return data


def _overwrite_or_open(db):
    try:
        if db is LOCAL_PLATFORM_DATABASE and _older_than_me(db):
            raise ValueError("Platform Database is out of date")
        with open(db, encoding="utf-8") as db_in:
            return json.load(db_in)
    except (IOError, ValueError) as exc:
        if db is LOCAL_PLATFORM_DATABASE:
            logger.warning("Error loading database %s: %s; Recreating", db, str(exc))
            try:
                makedirs(dirname(db))
            except OSError:
                pass
            try:
                with open(db, "w", encoding="utf-8") as out:
                    out.write(unicode(json.dumps(DEFAULT_PLATFORM_DB)))
            except IOError:
                pass
            return copy(DEFAULT_PLATFORM_DB)
        else:
            return {}


class PlatformDatabase(object):
    """Represents a union of multiple platform database files.
    Handles inter-process synchronization of database files.
    """

    target_id_pattern = re.compile(r"^[a-fA-F0-9]{4}$")

    def __init__(self, database_files, primary_database=None):
        """Construct a PlatformDatabase object from a series of platform database
        files
        """
        self._prim_db = primary_database
        if not self._prim_db and len(database_files) == 1:
            self._prim_db = database_files[0]
        self._dbs = OrderedDict()
        self._keys = defaultdict(set)
        for db in database_files:
            new_db = _overwrite_or_open(db)
            first_value = None
            if new_db.values():
                first_value = next(iter(new_db.values()))
                if not isinstance(first_value, dict):
                    new_db = {"daplink": new_db}

            if new_db:
                for device_type in new_db:
                    duplicates = self._keys[device_type].intersection(
                        set(new_db[device_type].keys())
                    )
                    duplicates = set(["%s.%s" % (device_type, k) for k in duplicates])
                    if duplicates:
                        logger.warning(
                            "Duplicate platform ids found: %s,"
                            " ignoring the definitions from %s",
                            " ".join(duplicates),
                            db,
                        )
                    self._dbs[db] = new_db
                    self._keys[device_type] = self._keys[device_type].union(
                        new_db[device_type].keys()
                    )
            else:
                self._dbs[db] = new_db

    def items(self, device_type="daplink"):
        for db in self._dbs.values():
            for entry in db.get(device_type, {}).items():
                yield entry

    def all_ids(self, device_type="daplink"):
        return iter(self._keys[device_type])

    def get(self, index, default=None, device_type="daplink", verbose_data=False):
        """Standard lookup function. Works exactly like a dict. If 'verbose_data'
        is True, all data for the platform is returned as a dict."""
        for db in self._dbs.values():
            if device_type in db:
                maybe_answer = db[device_type].get(index, None)
                if maybe_answer:
                    return _modify_data_format(maybe_answer, verbose_data)

        return default

    def _update_db(self):
        if self._prim_db:
            lock = InterProcessLock("%s.lock" % self._prim_db)
            acquired = lock.acquire(blocking=False)
            if not acquired:
                logger.debug("Waiting 60 seconds for file lock")
                acquired = lock.acquire(blocking=True, timeout=60)
            if acquired:
                try:
                    with open(self._prim_db, "w", encoding="utf-8") as out:
                        out.write(unicode(json.dumps(self._dbs[self._prim_db])))
                    return True
                finally:
                    lock.release()
            else:
                logger.error(
                    "Could not update platform database: "
                    "Lock acquire failed after 60 seconds"
                )
                return False
        else:
            logger.error(
                "Can't update platform database: destination database is ambiguous"
            )
            return False

    def add(self, id, platform_name, permanent=False, device_type="daplink"):
        """Add a platform to this database, optionally updating an origin
        database
        """
        if self.target_id_pattern.match(id):
            if self._prim_db:
                if device_type not in self._dbs[self._prim_db]:
                    self._dbs[self._prim_db][device_type] = {}
                self._dbs[self._prim_db][device_type][id] = platform_name
            else:
                cur_db = next(iter(self._dbs.values()))
                if device_type not in cur_db:
                    cur_db[device_type] = {}
                cur_db[device_type][id] = platform_name
            self._keys[device_type].add(id)
            if permanent:
                self._update_db()
        else:
            raise ValueError("Invald target id: %s" % id)

    def remove(self, id, permanent=False, device_type="daplink", verbose_data=False):
        """Remove a platform from this database, optionally updating an origin
        database. If 'verbose_data' is True, all data for the platform is returned
        as a dict.
        """
        logger.debug("Trying remove of %s", id)
        if id == "*" and device_type in self._dbs[self._prim_db]:
            self._dbs[self._prim_db][device_type] = {}
            if permanent:
                self._update_db()
        else:
            for db in self._dbs.values():
                if device_type in db and id in db[device_type]:
                    logger.debug("Removing id...")
                    removed = db[device_type][id]
                    del db[device_type][id]
                    self._keys[device_type].remove(id)
                    if permanent:
                        self._update_db()

                    return _modify_data_format(removed, verbose_data)
