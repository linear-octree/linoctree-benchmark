#!/bin/bash

# Exit on error
set -e

# Get the path of this script
SCRIPT_DIR="$(dirname "$(readlink -f "$0")")"

# Libs prefix
LIB_PREFIX="${SCRIPT_DIR}/../lib"

# Get absolute path of LIB_PREFIX
LIB_PREFIX="$(readlink -f "${LIB_PREFIX}")"

cd "${LIB_PREFIX}"

BOOST_PREFIX="${LIB_PREFIX}/boost"
EIGEN3_PREFIX="${LIB_PREFIX}/eigen3"
FLANN_PREFIX="${LIB_PREFIX}/flann"
PCL_PREFIX="${LIB_PREFIX}/pcl"

# Build Boost from source
BOOST_VERSION=1_82_0
BOOST_ARCHIVE="boost_${BOOST_VERSION}.tar.gz"
BOOST_DIR="boost_${BOOST_VERSION}"

#if [ ! -f "${BOOST_ARCHIVE}" ]; then
#    wget -c "https://archives.boost.io/release/1.82.0/source/${BOOST_ARCHIVE}"
#fi
#tar -xvzf "${BOOST_ARCHIVE}"
#cd "${BOOST_DIR}"
#./bootstrap.sh --prefix="${BOOST_PREFIX}"
#./b2 install -j 8
#cd ..
#
## Install Eigen3 (required by PCL)
#bash "${SCRIPT_DIR}/install_eigen3.sh"

# Build FLANN from source
if [ ! -f "1.9.2.tar.gz" ]; then
  wget -c https://github.com/flann-lib/flann/archive/refs/tags/1.9.2.tar.gz
fi

tar xvf 1.9.2.tar.gz
mv flann-1.9.2 flann_source

# Install FLANN
mkdir flann_source/build
cd flann_source/build
cmake -DCMAKE_BUILD_TYPE=Release -DCMAKE_POLICY_VERSION_MINIMUM=3.5 -DBUILD_MATLAB_BINDINGS=OFF -DCMAKE_INSTALL_PREFIX=${FLANN_PREFIX} -DBUILD_EXAMPLES=OFF -DBUILD_TESTS=OFF -DBUILD_DOC=OFF ..
make install -j
cd ../..
rm -rf flann_source

# Build PCL 1.15 from source
if [ ! -f "source.tar.gz" ]; then
  wget https://github.com/PointCloudLibrary/pcl/releases/download/pcl-1.15.0/source.tar.gz
fi

tar xvf source.tar.gz

mv pcl pcl_source

mkdir -p pcl_source/build
cd pcl_source/build
cmake -DCMAKE_BUILD_TYPE=Release \
      -DCMAKE_POLICY_VERSION_MINIMUM=3.5 \
      -DBoost_ROOT="${BOOST_PREFIX}" \
      -DBoost_NO_SYSTEM_PATHS=ON \
      -DEigen3_DIR=${EIGEN3_PREFIX} \
      -DFLANN_ROOT=${FLANN_PREFIX} \
      -DWITH_VTK=OFF \
      -DWITH_QT=OFF \
      -DWITH_PCAP=OFF \
      -DBUILD_2d=OFF \
      -DBUILD_apps=OFF \
      -DBUILD_benchmarks=OFF \
      -DBUILD_cuda=OFF \
      -DBUILD_examples=OFF \
      -DBUILD_features=OFF \
      -DBUILD_filters=OFF \
      -DBUILD_geometry=OFF \
      -DBUILD_global_tests=OFF \
      -DBUILD_gpu=OFF \
      -DBUILD_io=OFF \
      -DBUILD_ml=OFF \
      -DBUILD_outofcore=OFF \
      -DBUILD_people=OFF \
      -DBUILD_recognition=OFF \
      -DBUILD_registration=OFF \
      -DBUILD_sample_consensus=OFF \
      -DBUILD_search=OFF \
      -DBUILD_segmentation=OFF \
      -DBUILD_simulation=OFF \
      -DBUILD_stereo=OFF \
      -DBUILD_surface=OFF \
      -DBUILD_tools=OFF \
      -DBUILD_tracking=OFF \
      -DBUILD_visualization=OFF \
      -DCMAKE_INSTALL_PREFIX=${PCL_PREFIX} ..
make -j 4
make install
cd ../..
rm -rf pcl_source

# Cleanup