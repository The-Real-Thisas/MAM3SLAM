# # Use the official ROS Humble base image
# FROM ros:humble

# # Set the environment variable
# ENV DEBIAN_FRONTEND=noninteractive

# # Set the working directory to /MAM3SLAM
# WORKDIR /MAM3SLAM

# # Install necessary dependencies
# RUN apt-get update && apt-get install -y \
#     build-essential \
#     cmake \
#     git \
#     wget \
#     && rm -rf /var/lib/apt/lists/*

FROM ros:humble

ENV DEBIAN_FRONTEND=noninteractive

# RUN apt-get update && apt-get install -y \
#     build-essential cmake ninja-build git wget \
#     libboost-dev libopencv-dev libboost-serialization-dev \
#     python3 python3-pip && \
#     rm -rf /var/lib/apt/lists/*

RUN apt-get update && apt-get install -y \
    build-essential cmake ninja-build git wget \
    libboost-dev libopencv-dev libboost-serialization-dev \
    python3 python3-pip libgl1-mesa-dev libwayland-dev \
    libxkbcommon-dev wayland-protocols libegl1-mesa-dev \
    libc++-dev libepoxy-dev libglew-dev libeigen3-dev cmake g++ \
    libjpeg-dev libpng-dev catch2 libavcodec-dev \
    libavutil-dev libavformat-dev libswscale-dev libavdevice-dev && \
    rm -rf /var/lib/apt/lists/*

WORKDIR /MAM3SLAM
