#!/bin/bash
set -e

# Parse command line arguments
REINSTALL=false
for arg in "$@"; do
  if [[ "$arg" == "--reinstall" ]]; then
    REINSTALL=true
  fi
done

# Function to check if a directory exists and is not empty
directory_exists_not_empty() {
  [ -d "$1" ] && [ "$(ls -A "$1" 2>/dev/null)" ]
}

# Install Pangolin dependencies
if [ "$REINSTALL" = true ] || ! directory_exists_not_empty "Pangolin/build"; then
  echo "Installing Pangolin dependencies ..."
  cd Pangolin/
  ./scripts/install_prerequisites.sh recommended -u -y
  apt install python3-pip -y
  cmake -B build -GNinja -DCMAKE_CXX_FLAGS="-w"
  cmake --build build
  cd /MAM3SLAM
else
  echo "Pangolin already built. Skipping. Use --reinstall to force rebuild."
fi

# Build DBoW2
if [ "$REINSTALL" = true ] || ! directory_exists_not_empty "Thirdparty/DBoW2/build"; then
  echo "Building Thirdparty/DBoW2 ..."
  cd Thirdparty/DBoW2
  mkdir -p build && cd build
  apt-get update
  apt-get install -y libboost-dev libopencv-dev
  cmake .. -DCMAKE_BUILD_TYPE=Release -DCMAKE_CXX_FLAGS="-w"
  make -j$(nproc)
  cd /MAM3SLAM
else
  echo "DBoW2 already built. Skipping. Use --reinstall to force rebuild."
fi

# Build g2o
if [ "$REINSTALL" = true ] || ! directory_exists_not_empty "Thirdparty/g2o/build"; then
  echo "Building Thirdparty/g2o ..."
  cd Thirdparty/g2o
  mkdir -p build && cd build
  cmake .. -DCMAKE_BUILD_TYPE=Release -DCMAKE_CXX_FLAGS="-w"
  make -j$(nproc)
  cd /MAM3SLAM
else
  echo "g2o already built. Skipping. Use --reinstall to force rebuild."
fi

# Build Sophus - FIXED PATH
if [ "$REINSTALL" = true ] || ! directory_exists_not_empty "Thirdparty/Sophus/build"; then
  echo "Building Thirdparty/Sophus ..."
  cd Thirdparty/Sophus
  mkdir -p build && cd build
  cmake .. -DCMAKE_BUILD_TYPE=Release -DCMAKE_CXX_FLAGS="-w"
  make -j$(nproc)
  cd /MAM3SLAM
else
  echo "Sophus already built. Skipping. Use --reinstall to force rebuild."
fi

# Extract vocabulary if needed
if [ "$REINSTALL" = true ] || ! [ -f "Vocabulary/ORBvoc.txt" ]; then
  echo "Uncompressing vocabulary ..."
  cd Vocabulary
  tar -xf ORBvoc.txt.tar.gz
  cd ..
else
  echo "Vocabulary already extracted. Skipping."
fi

# Install additional dependencies
apt-get update 
apt-get install -y libboost-serialization-dev

echo "Dependency installation complete."