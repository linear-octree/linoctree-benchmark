#!/bin/bash

set -e

LIB_PREFIX="$(dirname "$(readlink -f "$0")")/../lib"

# Get absolute path of LIB_PREFIX
LIB_PREFIX="$(readlink -f "${LIB_PREFIX}")"

cd "${LIB_PREFIX}"

# TODO: FIX papi not compiling with gcc 15.x.x

wget https://github.com/icl-utk-edu/papi/releases/download/papi-7-2-0-t/papi-7.2.0.tar.gz
tar xvf papi-7.2.0.tar.gz && rm papi-7.2.0.tar.gz && mv papi-7.2.0 papi_source/
(cd papi_source/src && ./configure --prefix="${LIB_PREFIX}"/papi)
(cd papi_source/src && make -j; make install)

rm -r papi_source