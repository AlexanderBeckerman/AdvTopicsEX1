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

# Step 3: Run the Python script to visualize the robot's path
echo "Running the animation using Python..."
cd ../../../scripts/
python3 visualize_map.py ../input/$1

cd ../