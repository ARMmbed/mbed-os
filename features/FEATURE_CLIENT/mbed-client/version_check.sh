#!/bin/bash
# Copyright (c) 2015 ARM Limited. All rights reserved.
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
echo "Check version in module json"
echo
echo $1
echo $2

function version_gt() { 
    test "$(echo "$@" | tr " " "\n" | sort -V | tail -n 1)" == "$1"; 
}

git clone $1 $PWD/master
git clone $1 $PWD/pull_req
pushd $PWD/pull_req
git checkout $2
popd
if [ ! -e "$PWD/master/module.json" ]
then
	echo "$PWD/master/module.json not found."
        exit 1
fi
if [ ! -e "$PWD/pull_req/module.json" ]
then
	echo "$PWD/pull_req/module.json not found."
        exit 1
fi

OLD_VERSION=`sed -n 's#version##p' master/module.json | sed 's|[^0-9]*\([0-9\.]*\)|\1 |g'`
NEW_VERSION=`sed -n 's#version##p' pull_req/module.json | sed 's|[^0-9]*\([0-9\.]*\)|\1 |g'`

echo "Version in master branch: $OLD_VERSION"
echo "New version: $NEW_VERSION"

if [[ "$OLD_VERSION" != "$NEW_VERSION" ]]; then
    if version_gt $NEW_VERSION $OLD_VERSION; then
        exit 0
    else
        echo "Update version in module.json!"
        exit 1
fi
else
    echo "Update version in module.json!"
    exit 1
fi
