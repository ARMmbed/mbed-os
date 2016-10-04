"""An api for generating documentation from the codebase
"""

from os.path import dirname, join
from os import sep
from re import compile

import subprocess

def generate_documentation(dirs, output_dir):
    """Use doxygen to generate the documentation

    Positional arguments:
    dirs - the directories that doxygen should scan for documentation
    output_dir - location of the documentation after the return of this function
    """
    print dirs
    with open(join(dirname(__file__), "Doxyfile")) as doxyfile:
        proc = subprocess.Popen(["doxygen", "-"], stdin=subprocess.PIPE)
        proc.stdin.write(doxyfile.read())
        proc.stdin.write("OUTPUT_DIRECTORY={}\n".format(output_dir))
        proc.stdin.write("INPUT={}".format(" ".join(dirs)))
        proc.stdin.close()
        proc.wait()

MATCH_NAMESPACE = compile(r'^namespace.*')

def find_namespace(contents):
    for num, line in enumerate(contents.split("\n")):
        if MATCH_NAMESPACE.match(line):
            return num + 1
    return 0

if __name__ == "__main__":
    import sys
    from os.path import abspath, dirname, join
    # Be sure that the tools directory is in the search path
    ROOT = abspath(join(dirname(__file__), ".."))
    sys.path.insert(0, ROOT)

    from tools.toolchains.gcc import GCC_ARM
    from tools.targets import TARGET_MAP
    toolchain = GCC_ARM(TARGET_MAP["Super_Target"])
    resources = toolchain.scan_resources(".")
    for src in resources.headers:
        if len(src.split(sep)) > 2:
            name = src.split(sep)[1]
            if name == "features":
                name = src.split(sep)[2]
            with open(src) as fd:
                contents = fd.read()
            with open(src, "w+") as fd:
                insert_at = find_namespace(contents)
                before = "\n".join(contents.split("\n")[:insert_at])
                after = "\n".join(contents.split("\n")[insert_at:])
                fd.write("{:s}\n/** \\addtogroup {:s} */\n/** @{{*/\n{:s}\n/** @}}*/\n".format(before,name,after))
    for name, res in resources.features.iteritems():
        for src in res.headers:
            with open(src) as fd:
                contents = fd.read()
            with open(src, "w+") as fd:
                insert_at = find_namespace(contents)
                before = "\n".join(contents.split("\n")[:insert_at])
                after = "\n".join(contents.split("\n")[insert_at:])
                fd.write("{:s}\n/** \\addtogroup FEATURE_{:s} */\n/** @{{*/\n{:s}\n/** @}}*/\n".format(before,name,after))

    generate_documentation(filter(lambda x: "targets" not in x, sum(map(lambda x:x.headers, resources.features.values()), resources.headers)),
                           join(dirname(dirname(__file__)), "mbed-docs"))
