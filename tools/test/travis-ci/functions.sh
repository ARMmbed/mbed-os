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


#
# Helper functions for printing status information.
#  Uses 'echo' instead of 'printf' due to Travis CI stdout sync issues.
#
info() { echo -e "I: ${1}"; }
die() { echo -e "E: ${1}" 1>&2; exit "${2:-1}"; }


#
# Sets the GitHub job status for a given commit.
#
set_status()
{
  local job_name="${NAME}"
  local payload=""

  payload=$(<<< "
  {
    'state': '${1}',
    'description': '${2}',
    'context': 'travis-ci/${job_name}',
    'target_url': 'https://travis-ci.org/${TRAVIS_REPO_SLUG}/jobs/${TRAVIS_JOB_ID}'
  }" tr "'" '"')

  curl --silent --output /dev/null --user "${MBED_BOT}" --request POST \
    "https://api.github.com/repos/${TRAVIS_REPO_SLUG}/statuses/${TRAVIS_PULL_REQUEST_SHA:-${TRAVIS_COMMIT}}" \
    --data @- <<< "${payload}"
}


#
# Sources a pre-compiled GCC installation from AWS, installing the archive by
#  extracting and prepending the executable directory to PATH.
# 
# Note: Expects 'deps_url' and 'deps_dir' to already be defined.
# 
_install_gcc()
{
  # Ignore shellcheck warnings: Variables defined in .travis.yml
  # shellcheck disable=SC2154
  local url="${deps_url}/gcc6-linux.tar.bz2"

  # shellcheck disable=SC2154
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


#
# Downloads a list of packages from AWS, really fast.
#
_fetch_deps()
{
  local pkg="${1}"
  local dep_list="${2}"
  local pid_list=""

  local PID;

  info "Fetching '${pkg}' archives"

  while read -r dep; do
    
    curl --location "${deps_url}/${dep}.deb" \
      --output "${deps_dir}/${dep}.deb" \
      || die "Download failed ('${dep}')" \
      && info "Fetched ${deps_url}/${dep}.deb" &

    PID=$!
    pid_list="${pid_list} ${PID}"

  done <<< "${dep_list}"

  # Ignoring shellcheck warning, since we need to allow parameter expansion to
  #  turn the list string into parametesr.
  # shellcheck disable=SC2086
  wait ${pid_list}

  info "Fetch completed."
}


#
# Installs a list of Debian packages, fetching them if not locally found.
#
_install_deps()
{
  local pkg="${1}"
  local dep_list="${2}"
  local first_dep=""

  # Assume that if the first package isn't cached, none are.
  first_dep=$(<<< "${dep_list}" head -n1)
  [ ! -f "${deps_dir}/${first_dep}.deb" ] && _fetch_deps "${pkg}" "${dep_list}"

  # Install dependencies
  info "Installing '${pkg}' packages"
  
  # Ignore shellcheck warnings: Word splitting is specifically used to build
  #                             command in one go, and expression non-expansion
  #                             is intentional.
  # shellcheck disable=SC2046 disable=SC2016
  sudo dpkg -i $(<<< "${dep_list}" sed -e 's_^ *__' -e 's_^\(.*\)$_'"${deps_dir}"'/\1.deb_' | tr $'\n' ' ')
}


#
# Wrapper for installing a given package.
#
source_pkg()
{
  # Debian dependencies needed for a single package.
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

  local pkg="${1}"

  case "${pkg}" in

    "fuse" )
      # 'fuse' does not require an 'apt-get update' to install in Travis CI, so
      #  there's no reason to upload it or its dependencies into AWS.
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
