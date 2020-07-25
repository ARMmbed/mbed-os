from __future__ import absolute_import

import copy
import io
import itertools
import json
import os.path
import re
import subprocess
from contextlib import contextmanager

from mock import patch

import tools
from tools.export import export_project
from tools.export.exporters import Exporter
from tools.notifier.mock import MockNotifier

_TOOLS_DIR = os.path.join(os.path.dirname(tools.__file__))
_MBED_OS_DIR = os.path.join(os.path.dirname(_TOOLS_DIR))


def load_default_profile(profile_name):
    profile_file = os.path.join(_TOOLS_DIR, 'profiles', profile_name + '.json')
    with io.open(profile_file, encoding='utf-8') as f:
        return json.load(f)


def prepare_test_project(project_dir, target_name, toolchain_name):
    project_dir = str(project_dir)
    # create mbed_app.json
    with io.open(os.path.join(project_dir, 'mbed_app.json'), 'w', encoding='utf-8') as f:
        json.dump({"target_overrides": {"*": {"platform.stdio-baud-rate": 115200}}}, f, indent=4)
    # create main.cpp
    with io.open(os.path.join(project_dir, 'main.cpp'), 'w', encoding='utf-8') as f:
        f.write(r'''
#include "mbed.h"
 
DigitalOut user_led(LED1);
 
int main() {
    while(1) {
        user_led = 1;
        ThisThread::sleep_for(200ms);
        user_led = 0;
        ThisThread::sleep_for(200ms);
    }
}
'''.lstrip())
    # create config
    with io.open(os.path.join(project_dir, '.mbed'), 'w', encoding='utf-8') as f:
        f.write('TARGET={}\n'.format(target_name))
        f.write('TOOLCHAIN={}\n'.format(toolchain_name))
        f.write('ROOT=.\n')

    return project_dir


@contextmanager
def wrap_exporter_gen_file():
    gen_file_origin = Exporter.gen_file
    gen_file_nonoverwrite_origin = Exporter.gen_file_nonoverwrite
    template_data = {}

    def get_file_wrap(self, template_file, data, target_file, *args, **kwargs):
        src_data = copy.deepcopy(data)
        res = gen_file_origin(self, template_file, data, target_file, *args, **kwargs)
        template_data[target_file] = src_data
        return res

    def gen_file_nonoverwrite_origin_wrap(self, template_file, data, target_file, *args, **kwargs):
        src_data = copy.deepcopy(data)
        res = gen_file_nonoverwrite_origin(self, template_file, data, target_file, *args, **kwargs)
        template_data[target_file] = src_data
        return res

    with patch.object(Exporter, 'gen_file', get_file_wrap), \
         patch.object(Exporter, 'gen_file_nonoverwrite', gen_file_nonoverwrite_origin_wrap):
        yield template_data


def test_qtcreator_files_match_makefile_macros(tmpdir):
    target_name = 'K64F'
    toolchain_name = 'GCC_ARM'
    project_name = 'demo-project'
    project_dir = prepare_test_project(project_dir=tmpdir, target_name=target_name, toolchain_name=toolchain_name)

    with wrap_exporter_gen_file() as context_data:
        exporter = export_project(
            src_paths={
                '.': [project_dir],
                os.path.join('.', 'mbed-os'): [_MBED_OS_DIR]
            },
            export_path=project_dir,
            target=target_name,
            ide='qtcreator',
            name=project_name,
            macros=None,
            libraries_paths=None,
            zip_proj=None,
            build_profile=[load_default_profile('debug')],
            notify=MockNotifier(),
            app_config=None,
            ignore=None,
            resource_filter=None
        )
    # get GCC version
    version_output = subprocess.check_output([exporter.toolchain.cc[0], '--version']).decode('utf-8')
    m = re.search(r"\b\d+\.\d+\.\d+\b", version_output)
    if not m:
        raise ValueError("Cannot resolve GCC toolchain version")
    gcc_version = m.group()

    # check that all necessary files are generated
    assert 'Makefile' in context_data
    for qt_ext in ('.creator', '.files', '.includes', '.config'):
        assert project_name + qt_ext in context_data

    # extract qtcreator defines
    qtcreator_defines = set()
    for flag_name, value in context_data[project_name + '.config']['defines']:
        if len(value) == 1 or len(value) == 2:
            qtcreator_defines.add((flag_name,) + tuple(value))
        else:
            raise ValueError("Invalid macro definition: {}".format(value))
    # extracts Makefile defines
    makefile_defines = set()
    for flag in itertools.chain.from_iterable(
            v for k, v in context_data['Makefile'].items() if k.lower().endswith('_flags')
    ):
        m = re.match(r'^-(?P<flag_name>[UD])(?P<name>[^=]+)(:?=(?P<value>.*))?$', flag)
        if m is not None:
            if m.group('value') is not None:
                makefile_defines.add((m.group('flag_name'), m.group('name'), m.group('value')))
            else:
                makefile_defines.add((m.group('flag_name'), m.group('name')))

    # check that makefile and qtcreator uses the same macros
    assert qtcreator_defines == makefile_defines, "Makefile and qtcreator *.config files have different macros"
