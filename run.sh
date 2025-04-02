#!/bin/bash

# Create and enter build directory
mkdir -p build
cd build

# Configure and build with CMake
cmake -DCMAKE_BUILD_TYPE=Debug ..
make

# Check if build was successful
if [ $? -eq 0 ]; then
    echo -e "\nBuild successful! Running program:\n"
    ./spin_sat_controller
else
    echo -e "\nBuild failed!"
    exit 1
fi 