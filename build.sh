echo "Installing Pangolin ..."
cd Pangolin/
./scripts/install_prerequisites.sh recommended -u -y
apt install python3-pip -y
cmake -B build -GNinja
cmake --build build
cd ..

echo "Configuring and building Thirdparty/DBoW2 ..."

cd Thirdparty/DBoW2
mkdir build
cd build
apt update
apt install -y libboost-dev libopencv-dev
cmake .. -DCMAKE_BUILD_TYPE=Release -DCMAKE_CXX_FLAGS="-w"
make -j1
cd ../../

echo "Configuring and building Thirdparty/g2o ..."

cd g2o
mkdir build
cd build
cmake .. -DCMAKE_BUILD_TYPE=Release -DCMAKE_CXX_FLAGS="-w"
make -j1
cd ../../

echo "Configuring and building Thirdparty/Sophus ..."

cd Sophus
mkdir build
cd build
cmake .. -DCMAKE_BUILD_TYPE=Release -DCMAKE_CXX_FLAGS="-w"
make -j1

cd ../../../

echo "Uncompress vocabulary ..."

cd Vocabulary
tar -xf ORBvoc.txt.tar.gz
cd ..

echo "Configuring and building ORB_SLAM3 ..."

mkdir build
cd build
apt update
apt install libboost-serialization-dev -y
cmake .. -DCMAKE_BUILD_TYPE=Release  -DPangolin_DIR=/ORB_SLAM3/Pangolin/build -DCMAKE_CXX_FLAGS="-w"
make -j1