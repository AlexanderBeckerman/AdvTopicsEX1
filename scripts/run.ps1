$originalDir = Get-Location
$scriptDir = Split-Path -Parent -Path $MyInvocation.MyCommand.Definition

# Step 1: Compile the C++ program using CMake
Write-Host "Compiling the C++ program..."
Set-Location -Path "build"
cmake ..
make

# Step 2: Run the compiled C++ program to generate the moves file
Write-Host "Running the C++ program..."
Set-Location -Path "./src/main"
./main.exe "input_a.txt"

# Step 3: Run the Python script to visualize the robot's path
Write-Host "Running the Python script..."
Set-Location -Path $scriptDir
python visualize_map.py

Set-Location -Path $originalDir