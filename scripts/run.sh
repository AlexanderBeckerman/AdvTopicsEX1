#!/bin/bash

# Step 1: Compile the C++ program using CMake
echo "Compiling the C++ program..."
mkdir build
cd build
cmake ..
make

# Step 2: Run the compiled C++ program to generate the moves file
echo "Running the C++ program..."
cd src/main
./main $1

# Return to starting folder
cd ../../../