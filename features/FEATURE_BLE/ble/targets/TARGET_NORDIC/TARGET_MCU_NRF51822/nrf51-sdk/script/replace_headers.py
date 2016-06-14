# Copyright (c) 2015-2016 ARM Limited
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

with open("copyright_header.txt", "r") as fd:
	header = fd.read()

path = "../source/nordic_sdk"
for root, dirs, files in os.walk(path):
	for fn in [os.path.join(root, x) for x in files]:
		with open(fn, "r+") as fd:
			print "+"*35
			print fn
			s = fd.read()
			start = s.find("/*")
			end = s.find("*/")
			copyright_str = s[start:end+2]
			if "copyright (c)" not in copyright_str.lower():
				s = header + "\n\n" + s
			elif copyright_str is not header:
				s = s.replace(copyright_str, header)

			fd.seek(0)
			fd.write(s)
			fd.truncate()
