#!/bin/bash

# Step 1: Compile the C++ program using CMake
echo "Compiling the C++ program..."
mkdir build
cd build
cmake ..
make

# Step 2: Run the compiled C++ program 
echo "Running the program tests..."
cd src/tests/
./unit_tests 

# Return to starting folder
cd ../../../