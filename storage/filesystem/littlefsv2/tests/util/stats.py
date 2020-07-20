#!/usr/bin/env python

import re
import sys
import subprocess
import os

def main(*args):
    with open('main.cpp') as file:
        tests = file.read()

    cases = []
    with open('template_all_names.txt') as file:
        while True:
            name = file.readline().strip('\n')
            desc = file.readline().strip('\n')
            if name == 'test_results':
                break

            cases.append((name, desc))

    with open('template_wrapper.fmt') as file:
        template = file.read()

    with open('main.cpp', 'w') as file:
        file.write(template.format(
                tests=tests,
                test_cases='\n'.join(
                    4*' '+'Case("{desc}", {name}),'.format(
                    name=name, desc=desc) for name, desc in cases)))

if __name__ == "__main__":
    main(*sys.argv[1:])
