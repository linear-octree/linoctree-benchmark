#!/bin/bash

set -e

LIB_PREFIX="$(dirname "$(readlink -f "$0")")/../lib"

# Get absolute path of LIB_PREFIX
LIB_PREFIX="$(readlink -f "${LIB_PREFIX}")"

cd "${LIB_PREFIX}"

git clone --depth=1 https://github.com/myermo/papi.git papi_source
(cd papi_source/src && ./configure --prefix="${LIB_PREFIX}"/papi)
(cd papi_source/src && make -j && make install)

rm -fr papi_source