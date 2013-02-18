import os, tempfile
from os.path import join

from workspace_tools.export import uvision4, codesourcery, codered, gccarm, ds5_5, iar
from workspace_tools.export.exporters import zip_working_directory_and_clean_up, OldLibrariesException


EXPORTERS = {
    'uvision': uvision4.Uvision4,
    'codered': codered.CodeRed,
    'codesourcery': codesourcery.CodeSourcery,
    'gcc_arm': gccarm.GccArm,
    'ds5_5': ds5_5.DS5_5,
    'iar': iar.IAREmbeddedWorkbench
}

ERROR_MESSAGE_UNSUPPORTED_TOOLCHAIN = """
Sorry, the target %s is not currently supported on the %s toolchain.
Please refer to <a href='/handbook/Exporting-to-offline-toolchains' target='_blank'>Exporting to offline toolchains</a> for more information.
"""

ERROR_MESSAGE_NOT_EXPORT_LIBS = """
To export this project please <a href='http://mbed.org/compiler/?import=http://mbed.org/users/mbed_official/code/mbed-export/k&mode=lib' target='_blank'>import the export version of the mbed library</a>.
"""

def online_build_url_resolver(url):
    # TODO: Retrieve the path and name of an online library build URL
    return {'path':'', 'name':''}


def export(project_path, project_name, ide, target, destination='/tmp/', tempdir=None, clean=True, build_url_resolver=online_build_url_resolver):
    # Convention: we are using capitals for toolchain and target names
    if target is not None:
        target = target.upper()
    
    if tempdir is None:
        tempdir = tempfile.mkdtemp()
    
    if ide is None:
        # Simply copy everything, no project files to be generated
        for d in ['src', 'lib']:
            os.system("cp -r %s/* %s" % (join(project_path, d), tempdir))
        report = {'success': True}
    
    else:
        report = {'success': False}
        if ide not in EXPORTERS:
            report['errormsg'] = "Unsupported toolchain"
        else:
            Exporter = EXPORTERS[ide]
            if target not in Exporter.TARGETS:
                report['errormsg'] = ERROR_MESSAGE_UNSUPPORTED_TOOLCHAIN % (target, ide)
            else:
                try:
                    exporter = Exporter(target, tempdir, project_name, build_url_resolver)
                    exporter.scan_and_copy_resources(project_path, tempdir)
                    exporter.generate()
                    report['success'] = True
                except OldLibrariesException, e:
                    report['errormsg'] = ERROR_MESSAGE_NOT_EXPORT_LIBS
    
    zip_path = None
    if report['success']:
        zip_path = zip_working_directory_and_clean_up(tempdir, destination, project_name, clean)
    
    return zip_path, report
