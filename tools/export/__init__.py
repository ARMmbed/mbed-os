"""The generic interface for all exporters.
"""
# mbed SDK
# Copyright (c) 2011-2016 ARM Limited
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

from tools.export import codered, ds5_5, iar, makefile
from tools.export import embitz, coide, kds, simplicityv3, atmelstudio
from tools.export import sw4stm32, e2studio, zip, cmsis, uvision, cdt
from tools.targets import TARGET_NAMES

EXPORTERS = {
    'uvision5': uvision.Uvision,
    'uvision': uvision.Uvision,
    'lpcxpresso': codered.CodeRed,
    'gcc_arm': makefile.GccArm,
    'make_gcc_arm': makefile.GccArm,
    'make_armc5': makefile.Armc5,
    'make_iar': makefile.IAR,
    'ds5_5': ds5_5.DS5_5,
    'iar': iar.IAR,
    'embitz' : embitz.EmBitz,
    'coide' : coide.CoIDE,
    'kds' : kds.KDS,
    'simplicityv3' : simplicityv3.SimplicityV3,
    'atmelstudio' : atmelstudio.AtmelStudio,
    'sw4stm32'    : sw4stm32.Sw4STM32,
    'e2studio' : e2studio.E2Studio,
    'eclipse_gcc_arm'  : cdt.EclipseGcc,
    'eclipse_iar'      : cdt.EclipseIAR,
    'eclipse_armc5'    : cdt.EclipseArmc5,
    'zip' : zip.ZIP,
    'cmsis'    : cmsis.CMSIS
}

ERROR_MESSAGE_UNSUPPORTED_TOOLCHAIN = """
Sorry, the target %s is not currently supported on the %s toolchain.
Please refer to <a href='/handbook/Exporting-to-offline-toolchains' target='_blank'>Exporting to offline toolchains</a> for more information.
"""

ERROR_MESSAGE_NOT_EXPORT_LIBS = """
To export this project please <a href='http://mbed.org/compiler/?import=http://mbed.org/users/mbed_official/code/mbed-export/k&mode=lib' target='_blank'>import the export version of the mbed library</a>.
"""

def mcu_ide_matrix(verbose_html=False):
    """Shows target map using prettytable

    Keyword argumets:
    verbose_html - print the matrix in html format
    """
    supported_ides = sorted(EXPORTERS.keys())
    # Only use it in this function so building works without extra modules
    from prettytable import PrettyTable, ALL

    # All tests status table print
    table_printer = PrettyTable(["Platform"] + supported_ides)
    # Align table
    for col in supported_ides:
        table_printer.align[col] = "c"
    table_printer.align["Platform"] = "l"

    perm_counter = 0
    for target in sorted(TARGET_NAMES):
        row = [target]  # First column is platform name
        for ide in supported_ides:
            text = "-"
            if target in EXPORTERS[ide].TARGETS:
                if verbose_html:
                    text = "&#10003;"
                else:
                    text = "x"
                perm_counter += 1
            row.append(text)
        table_printer.add_row(row)

    table_printer.border = True
    table_printer.vrules = ALL
    table_printer.hrules = ALL
    # creates a html page in a shorter format suitable for readme.md
    if verbose_html:
        result = table_printer.get_html_string()
    else:
        result = table_printer.get_string()
    result += "\n"
    result += "Total IDEs: %d\n"% (len(supported_ides))
    if verbose_html:
        result += "<br>"
    result += "Total platforms: %d\n"% (len(TARGET_NAMES))
    if verbose_html:
        result += "<br>"
    result += "Total permutations: %d"% (perm_counter)
    if verbose_html:
        result = result.replace("&amp;", "&")
    return result
