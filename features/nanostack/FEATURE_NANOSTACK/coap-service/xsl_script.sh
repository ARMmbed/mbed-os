#!/bin/bash

# Copyright (c) 2015-2017 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0
# Licensed under the Apache License, Version 2.0 (the License); you may
# not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
# * http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an AS IS BASIS, WITHOUT
# WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

echo 
echo "Creating report"
echo 

echo '<?xml version="1.0" encoding="UTF-8" ?>
<?xml-stylesheet type="text/xsl" href="junit_xsl.xslt"?>
<list>' >> lcov/index.xml

for f in lcov/results/*.xml
do
name=${f##*/}
echo '<entry name="results/'"$name"'" />'>> lcov/index.xml
done

echo '</list>' >> lcov/index.xml

echo
echo "Report created to lcov/index.xml (outputs html)"
echo