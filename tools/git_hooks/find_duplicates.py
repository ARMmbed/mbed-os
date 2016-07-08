from os import walk
from os.path import join, abspath, dirname, basename, splitext
import sys

ROOT = abspath(join(dirname(__file__), "..", ".."))
sys.path.insert(0, ROOT)

from tools.toolchains.gcc import GCC_ARM
from tools.targets import TARGET_MAP
from argparse import ArgumentParser

if __name__ == "__main__":
    parser = ArgumentParser("Find duplicate file names within a directory structure")
    parser.add_argument("dirs", help="Directories to search for duplicate file names"
                        , nargs="*")
    parser.add_argument("--silent", help="Supress printing of filenames, just return number of duplicates", action="store_true")
    args = parser.parse_args()

    toolchain = GCC_ARM(TARGET_MAP[".Super-Target"])

    resources = sum([toolchain.scan_resources(d) for d in args.dirs], None)

    scanned_files = {}

    for r in [resources] + resources.features.values():
        for file in r.c_sources + r.s_sources + r.cpp_sources + r.objects + r.libraries + r.hex_files + r.bin_files:
            scanned_files.setdefault(basename(file), [])
            scanned_files[basename(file)].append(file)
            filenameparts = splitext(file)
            if filenameparts[-1] in ["c", "cpp", "s", "S"]:
                filenameparts[-1] = "o"
                file = ".".join(filenamparts)
                scanned_files.setdefault(basename(file), [])
                scanned_files[basename(file)].append(file)

    count_dupe = 0
    for key, value in scanned_files.iteritems():
        if len(value) > 1:
            count_dupe += 1
            if not args.silent:
                print("Multiple files found with name {}".format(key))
                for file in value:
                    print("  {}".format(file))

    exit(count_dupe)

