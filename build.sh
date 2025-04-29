#!/bin/bash
set -e

echo "Building ORB_SLAM3 main application using pre-built deps in /deps..."

mkdir -p build && cd build

cmake .. -DCMAKE_BUILD_TYPE=Release \
    -DPangolin_DIR=/deps/Pangolin/build \
    -DCMAKE_CXX_FLAGS="-w"

make -j$(nproc)