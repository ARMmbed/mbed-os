#!/usr/bin/env python

import os
import re

def main(path='.', pattern=r'#include\s+"([^"]*\.(?:c|cpp))"'):
    pattern = re.compile(pattern)

    for root, dirs, files in os.walk(path, followlinks=True):
        for file in files:
            with open(os.path.join(root, file)) as f:
                for line in f.read().splitlines():
                    m = re.search(pattern, line)
                    if m:
                        print os.path.relpath(os.path.join(root, m.group(1)))


if __name__ == "__main__":
    import sys
    main(*sys.argv[1:])

