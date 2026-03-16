#!/bin/bash

set -e

LIB_PREFIX="$(dirname "$(readlink -f "$0")")/../lib"

# Get absolute path of LIB_PREFIX
LIB_PREFIX="$(readlink -f "${LIB_PREFIX}")"


cd "${LIB_PREFIX}"


wget https://github.com/icl-utk-edu/papi/releases/download/papi-7-2-0-t/papi-7.2.0.tar.gz
tar xvf papi-7.2.0.tar.gz && rm papi-7.2.0.tar.gz
mv papi-7.2.0 papi
rm -rf papi/papi-7.2.0
# Try to configure and make - ignore some compilation errors that are not critical
(cd papi/src && ./configure --prefix="${LIB_PREFIX}"/papi && make)
