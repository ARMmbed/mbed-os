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

    toolchain = GCC_ARM(TARGET_MAP["K64F"])

    resources = sum([toolchain.scan_resources(d) for d in args.dirs], None)

    scanned_files = {}

    exit(resources.detect_duplicates(toolchain))

