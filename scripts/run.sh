#!/bin/bash

# Step 1: Compile the C++ program using CMake
echo "Compiling the C++ program..."
mkdir build
cd build
cmake ..
make

# Step 2: Run the compiled C++ program to generate the moves file
echo "Running the C++ program..."
cd src/simulator
ARG1=${1-}
ARG2=${2-}
ARG3=${3-}
ARG4=${4-}
./myrobot  $ARG1 $ARG2 $ARG3 $ARG4

# Return to starting folder
cd ../../../