#!/bin/bash
set -e

echo "Installing Pangolin dependencies ..."
cd Pangolin/
./scripts/install_prerequisites.sh recommended -u -y
apt install python3-pip -y
cmake -B build -GNinja -DCMAKE_CXX_FLAGS="-w"
cmake --build build
cd ..

echo "Building Thirdparty/DBoW2 ..."
cd Thirdparty/DBoW2
mkdir -p build && cd build
apt-get update
apt-get install -y libboost-dev libopencv-dev
cmake .. -DCMAKE_BUILD_TYPE=Release -DCMAKE_CXX_FLAGS="-w"
make -j$(nproc)
cd ../../

echo "Building Thirdparty/g2o ..."
cd g2o
mkdir -p build && cd build
cmake .. -DCMAKE_BUILD_TYPE=Release -DCMAKE_CXX_FLAGS="-w"
make -j$(nproc)
cd ../../

echo "Building Thirdparty/Sophus ..."
cd Sophus
mkdir -p build && cd build
cmake .. -DCMAKE_BUILD_TYPE=Release -DCMAKE_CXX_FLAGS="-w"
make -j$(nproc)
cd ../../../

echo "Uncompressing vocabulary ..."
cd Vocabulary
tar -xf ORBvoc.txt.tar.gz
cd ..

apt-get update 
apt-get install -y libboost-serialization-dev

echo "Dependency installation complete."