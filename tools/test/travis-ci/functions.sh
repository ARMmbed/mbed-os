#!/bin/bash -euf
#
# Copyright (c) 2013-2018 Arm Limited. All rights reserved.
#
# SPDX-License-Identifier: Apache-2.0
#
# Licensed under the Apache License, Version 2.0 (the License); you may
# not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
# www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an AS IS BASIS, WITHOUT
# WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

set -o pipefail

info() { echo "I: ${1}"; }
die() { echo "E: ${1}" 1>&2; exit ${2:-1}; }

set_status()
{
  local _name=${NAME}
  local _payload=$(<<< "
  {
    'state': '${1}',
    'description': '${2}',
    'context': 'travis-ci/${_name}',
    'target_url': 'https://travis-ci.org/${TRAVIS_REPO_SLUG}/jobs/${TRAVIS_JOB_ID}'
  }" tr '"' '"')

  curl --silent --output /dev/null --user "${MBED_BOT}" --request POST \
    "https://api.github.com/repos/${TRAVIS_REPO_SLUG}/statuses/${TRAVIS_PULL_REQUEST_SHA:-$TRAVIS_COMMIT}" \
    --data @- <<< "${payload}"
}
