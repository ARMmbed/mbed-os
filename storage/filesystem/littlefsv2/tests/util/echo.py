#!/usr/bin/env python

import re
import sys
import subprocess
import os

def main(*args):
    desc = ' '.join(args).strip('-= ')
    name = 'test_' + desc.lower().replace(' ', '_').replace('-', '_')

    exists = os.path.isfile('template_all_names.txt')

    with open('template_all_names.txt', 'a') as file:
        file.write(name + '\n')
        file.write(desc + '\n')

    with open('template_unit.fmt') as file:
        template = file.read()

    template_header, template_footer = template.split('{test}')

    if exists:
        with open('main.cpp', 'a') as file:
            file.write(template_footer.format(
                    test_name=name))

    if name != 'test_results':
        with open('main.cpp', 'a') as file:
            file.write(template_header.format(
                    test_name=name))

if __name__ == "__main__":
    main(*sys.argv[1:])
