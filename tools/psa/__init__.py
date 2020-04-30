#!/usr/bin/env python
# Copyright (c) 2019 ARM Limited
#
# SPDX-License-Identifier: Apache-2.0
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

import os
from tools.resources import FileType

def find_secure_image(notify, resources, ns_image_path,
                      configured_s_image_filename, image_type):
    """ Find secure image. """
    if configured_s_image_filename is None:
        return None

    assert ns_image_path and configured_s_image_filename, \
        'ns_image_path and configured_s_image_path are mandatory'
    assert image_type in [FileType.BIN, FileType.HEX], \
        'image_type must be of type BIN or HEX'

    image_files = resources.get_file_paths(image_type)
    assert image_files, 'No image files found for this target'

    secure_image = next(
        (f for f in image_files if
         os.path.basename(f) == configured_s_image_filename), None)
    secure_image = next(
        (f for f in image_files if
         os.path.splitext(os.path.basename(f))[0] ==
         os.path.splitext(os.path.basename(ns_image_path))[0]),
        secure_image
    )

    if secure_image:
        notify.debug("Secure image file found: %s." % secure_image)
    else:
        notify.debug("Secure image file %s not found. Aborting."
                     % configured_s_image_filename)
        raise Exception("Required secure image not found.")

    return secure_image
