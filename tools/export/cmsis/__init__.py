import os
from os.path import sep, join, exists
from itertools import groupby
from xml.etree.ElementTree import Element, tostring
import ntpath
import re
import json

from tools.arm_pack_manager import Cache
from tools.targets import TARGET_MAP
from tools.export.exporters import Exporter, TargetNotSupportedException

class fileCMSIS():
    """CMSIS file class.

    Encapsulates information necessary for files in cpdsc project file"""
    file_types = {'.cpp': 'sourceCpp', '.c': 'sourceC', '.s': 'sourceAsm',
                  '.obj': 'object', '.o': 'object', '.lib': 'library',
                  '.ar': 'linkerScript', '.h': 'header', '.sct': 'linkerScript'}

    def __init__(self, loc, name):
        #print loc
        _, ext = os.path.splitext(loc)
        self.type = self.file_types[ext.lower()]
        self.loc = loc
        self.name = name


class DeviceCMSIS():
    """CMSIS Device class

    Encapsulates target information retrieved by arm-pack-manager"""
    def __init__(self, target):
        cache = Cache(True, False)

        t = TARGET_MAP[target]
        self.core = t.core
        try:
            cpu_name = t.device_name
            target_info = cache.index[cpu_name]
        # Target does not have device name or pdsc file
        except:
            try:
                # Try to find the core as a generic CMSIS target
                cpu_name = self.cpu_cmsis()
                target_info = cache.index[cpu_name]
            except:
                raise TargetNotSupportedException("Target not in CMSIS packs")

        self.target_info = target_info

        self.url = target_info['pdsc_file']
        self.pack_url, self.pack_id = ntpath.split(self.url)
        self.dname = cpu_name
        self.dfpu = target_info['processor']['fpu']
        self.debug, self.dvendor = self.vendor_debug(target_info['vendor'])
        self.dendian = target_info['processor'].get('endianness','Little-endian')
        self.debug_svd = target_info.get('debug', '')
        self.compile_header = target_info['compile']['header']

    def check_version(self, filename):
        with open(filename) as data_file:
            data = json.load(data_file)
            return data.get("version", "0") == "0.1.0"

    def vendor_debug(self, vendor):
        reg = "([\w\s]+):?\d*?"
        m = re.search(reg, vendor)
        vendor_match = m.group(1) if m else None
        debug_map ={
            'STMicroelectronics':'ST-Link',
            'Silicon Labs':'J-LINK',
            'Nuvoton':'NULink'
        }
        return debug_map.get(vendor_match, "CMSIS-DAP"), vendor_match

    def cpu_cmsis(self):
        #Cortex-M4F => ARMCM4_FP, Cortex-M0+ => ARMCM0P
        cpu = self.core
        cpu = cpu.replace("Cortex-","ARMC")
        cpu = cpu.replace("+","P")
        cpu = cpu.replace("F","_FP")
        return cpu


class CMSIS(Exporter):
    NAME = 'cmsis'
    TOOLCHAIN = 'ARM'
    TARGETS = [target for target, obj in TARGET_MAP.iteritems()
               if "ARM" in obj.supported_toolchains]

    def make_key(self, src):
        """turn a source file into its group name"""
        key = src.name.split(sep)[0]
        if key == ".":
            key = os.path.basename(os.path.realpath(self.export_dir))
        return key

    def group_project_files(self, sources, root_element):
        """Recursively group the source files by their encompassing directory"""

        data = sorted(sources, key=self.make_key)
        for group, files in groupby(data, self.make_key):
            new_srcs = []
            for f in list(files):
                spl = f.name.split(sep)
                if len(spl)==2:
                    file_element = Element('file',
                                           attrib={
                                               'category':f.type,
                                               'name': f.loc})
                    root_element.append(file_element)
                else:
                    f.name = os.path.join(*spl[1:])
                    new_srcs.append(f)
            if new_srcs:
                group_element = Element('group',attrib={'name':group})
                root_element.append(self.group_project_files(new_srcs,
                                                        group_element))
        return root_element

    def generate(self):

        srcs = self.resources.headers + self.resources.s_sources + \
               self.resources.c_sources + self.resources.cpp_sources + \
               self.resources.objects + self.resources.libraries + \
               [self.resources.linker_script]
        srcs = [fileCMSIS(src, src) for src in srcs if src]
        ctx = {
            'name': self.project_name,
            'project_files': tostring(self.group_project_files(srcs, Element('files'))),
            'device': DeviceCMSIS(self.target),
            'date': ''
        }
        # TODO: find how to keep prettyxml from adding xml version to this blob
        #dom = parseString(ctx['project_files'])
        #ctx['project_files'] = dom.toprettyxml(indent="\t")

        self.gen_file('cmsis/cpdsc.tmpl', ctx, 'project.cpdsc')
