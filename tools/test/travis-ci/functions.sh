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

info() { echo -e "I: ${1}"; }
die() { echo -e "E: ${1}" 1>&2; exit ${2:-1}; }

set_status()
{
  local job_name=${NAME}
  local payload=$(<<< "
  {
    'state': '${1}',
    'description': '${2}',
    'context': 'travis-ci/${job_name}',
    'target_url': 'https://travis-ci.org/${TRAVIS_REPO_SLUG}/jobs/${TRAVIS_JOB_ID}'
  }" tr '"' '"')

  curl --silent --output /dev/null --user "${MBED_BOT}" --request POST \
    "https://api.github.com/repos/${TRAVIS_REPO_SLUG}/statuses/${TRAVIS_PULL_REQUEST_SHA:-$TRAVIS_COMMIT}" \
    --data @- <<< "${payload}"
}


_install_gcc()
{
  local url="${deps_url}/gcc6-linux.tar.bz2"
  local gcc_path="${deps_dir}/gcc/gcc-arm-none-eabi-6-2017-q2-update/"
  local archive="gcc.tar.bz2"
  
  info "URL: ${url}"

  if [ ! -d "${deps_dir}/gcc" ]; then
    
    info "Downloading archive"
    curl --location "${url}" --output "${deps_dir}/${archive}"
    ls -al "${deps_dir}"
    
    info "Extracting 'gcc'"
    mkdir -p "${deps_dir}/gcc"
    tar -xf "${deps_dir}/${archive}" -C "${deps_dir}/gcc"
    rm "${deps_dir}/${archive}"

  fi
  
  info "Installing 'gcc'"
  export "PATH=${gcc_path}/bin:${PATH}"
} 


_fetch_deps()
{
  local pkg="${1}"
  local dep_list="${2}"

  info "Fetching '${pkg}' archives"

  while read dep; do
    
    curl --location "${deps_url}/${dep}.deb" \
      --output "${deps_dir}/${dep}.deb" \
      || die "Download failed ('${dep}')" \
      && info "Fetched ${deps_url}/${dep}.deb" &

  done <<< "${dep_list}"

  wait
}

_install_deps()
{
  local pkg="${1}"
  local dep_list="${2}"

  # Assume that if the first package isn't cached, none are.
  local first_dep=$(<<< "${dep_list}" head -n1)
  [ ! -f "${deps_dir}/${first_dep}.deb" ] && _fetch_deps "${pkg}" "${dep_list}"

  # Install dependencies
  info "Installing '${pkg}' packages"
  sudo dpkg -i $(<<< "${dep_list}" sed -e 's_^ *__' -e 's_^\(.*\)$_'"${deps_dir}"'/\1.deb_' | tr $'\n' ' ')
}


source_pkg()
{
  local pkg="${1}"

  local aspell_deps="aspell
    aspell-en
    dictionaries-common
    libaspell15"

  local libfuse_deps="libfuse-dev
    libpcre3-dev
    libpcre32-3
    libpcrecpp0v5
    libselinux1-dev
    libsepol1-dev
    libc-bin"


  case "${pkg}" in

    "fuse" )
      sudo apt-get -o=dir::cache="${deps_dir}/apt-get" install fuse \
        || die "Installation failed"
      ;;
    
    "aspell" )
      _install_deps aspell "${aspell_deps}" \
        || die "Installation failed"
      ;;
    "libfuse-dev" )
      _install_deps libfuse-dev "${libfuse_deps}" \
        || die "Installation failed"
      ;;

    "gcc" )
      _install_gcc \
        || die "Installation failed"
      ;;

    * )
      die "Package not supported: '${pkg}'"
      ;;

  esac
}
