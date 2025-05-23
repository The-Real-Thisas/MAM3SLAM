#!/bin/bash
set -e

echo "Building ORB_SLAM3"

mkdir -p build && cd build

cmake .. -DCMAKE_BUILD_TYPE=Release \
    -DPangolin_DIR=/MAM3SLAM/Pangolin/build \
    -DCMAKE_CXX_FLAGS="-w"

make -j$(nproc)