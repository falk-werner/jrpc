#!/bin/bash
#
# JRPC - Yet another JSON-RPC server based on libwebsockets
#  <https://github.com/falk-werner/jrpc>
#
# Copyright (c) 2019 Falk Werner
#
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:
#
# The above copyright notice and this permission notice shall be included in all
# copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
# SOFTWARE.
#
# Checks for dependencies and install them, if needed.
#
# A valid package description is defined as follows:
#  local <package>_name=<name>
#  local <package>_type=<type>
#  local <package>_archive=<archive>
#  local <package>_dir=<dir>
#  local <package>_url=<url>
#
# <package> - symbolid id of package
# <name>    - name of the package (used for pkg-config)
# <type>    - type of project ("cmake" or "autotools")
# <archive> - archive file name
# <dir>     - dir of package sources
# <url>     - url of the project archive
#
# For examples, see below.
#
# Since <package>_ variables are not used directly, shellcheck will complain:
# shellcheck disable=SC2034


#######################################
# Installs a cmake package.
#
# Arguments:
#   package - name of package
# Returns:
#   0 on success, otherwise failure
#######################################
function install_cmake_package() {
    local package="$1"
    local archive="${package}_archive"
    local dir="${package}_dir"
    local url="${package}_url"

    wget -O "${!archive}" "${!url}" && \
    tar -xf "${!archive}" && \
    cd "${!dir}" && \
    mkdir .build && \
    cd .build && \
    cmake .. && \
    make && \
    sudo make install && \
    cd .. && \
    cd ..

    return "$?"
}

#######################################
# Installs a autotools package.
#
# Arguments:
#   package - name of package
# Returns:
#   0 on success, otherwise failure
#######################################
function install_autotools_package() {
    local package="$1"
    local archive="${package}_archive"
    local dir="${package}_dir"
    local url="${package}_url"

    wget -O "${!archive}" "${!url}" && \
    tar -xf "${!archive}" && \
    cd "${!dir}" && \
    ./makeconf.sh && \
    ./configure && \
    make && \
    sudo make install && \
    cd ..

    return "$?"
}

#######################################
# Checks, whether a package exists.
#
# Arguments:
#   package_name - name of package
# Returns:
#   0 if package exists, otherwise failure
#######################################
function package_exists() {
    local package_name="$1"

    pkg-config --exists "${package_name}"
    return "$?"
}

#######################################
# Installs a package, if it is not available.
#
# Arguments:
#   package - name of package
# Returns:
#   0 if package exists, otherwise failure
#######################################
function install_if_needed() {
    local package="$1"
    local package_name="${package}_name"
    local package_type="${package}_type"
    local result=0

    package_exists "${!package_name}"
    if [[ "$?" -ne 0 ]]; then
        "install_${!package_type}_package" "$package"
        result="$?"
    fi

    return "${result}"
}

#######################################
# Main
#
# Arguments:
#   None
# Returns:
#   0 on success, otherwise failure
#######################################
function main() {
    rm -rf .deps
    mkdir .deps
    cd .deps

    local packages=(lws jansson)

    local lws_name=libwebsockets
    local lws_type=cmake
    local lws_archive=libwebsockets-3.1.0.tar.gz
    local lws_dir=libwebsockets-3.1.0
    local lws_url=https://github.com/warmcat/libwebsockets/archive/v3.1.0.tar.gz

    local jansson_name=jansson
    local jansson_type=cmake
    local jansson_archive=libjansson-2.12.tar.gz
    local jansson_dir=jansson-2.12
    local jansson_url=https://github.com/akheron/jansson/archive/v2.12.tar.gz
 
    local result=0
    local package=""
    for package in "${packages[@]}" ; do
        install_if_needed $package
        result="$?"

        if [[ "${result}" -ne 0 ]] ; then
            cd ..
            return "${result}"
        fi
    done

    cd ..
    return 0
}

main "$@"