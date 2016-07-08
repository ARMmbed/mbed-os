from os import walk
from os.path import join
from argparse import ArgumentParser

if __name__ == "__main__":
    parser = ArgumentParser("Find duplicate file names within a directory structure")
    parser.add_argument("dirs", help="Directories to search for duplicate file names"
                        , nargs="*")
    parser.add_argument("--silent", help="Supress printing of filenames, just return number of duplicates", action="store_true")
    args = parser.parse_args()

    scanned_files = {}

    for dir in args.dirs:
        for root, dirs, files in walk(dir):
            for file in files:
                scanned_files.setdefault(file, [])
                scanned_files[file].append(join(root, file))

    count_dupe = 0
    for key, value in scanned_files.iteritems():
        if len(value) > 1:
            count_dupe += 1
            if not args.silent:
                print("Multiple files found with name {}".format(key))
                for file in value:
                    print("  {}".format(file))

    exit(count_dupe)

