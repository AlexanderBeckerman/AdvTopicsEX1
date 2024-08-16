import os
import subprocess
import re
import sys

def find_algorithm_files(house_filename, output_dir):
    # Create a regex pattern to match the desired file format
    pattern = re.compile(rf'^output_{house_filename}_(.+)\.txt$')
    
    # List all files in the output directory
    files = os.listdir(output_dir)
    
    # Filter files that match the pattern and extract the algorithm name
    algorithms = []
    for file in files:
        match = pattern.match(file)
        if match:
            algorithms.append(match.group(1))  # Extract algorithm name
    
    return algorithms

def run_visualize_map(algorithm_name, house_filename):
    # Construct the command to run visualize_map.py
    command = ['python3', 'visualize_map.py', algorithm_name, house_filename]
    
    # Start the process using Popen and return the process object
    process = subprocess.Popen(command)
    return process

def main(house_filename, output_dir='output'):
    # Find all algorithm names for the given house filename
    algorithms = find_algorithm_files(house_filename, output_dir)
    
    # List to store all process objects
    processes = []
    
    # Start visualize_map.py for each algorithm found
    for algorithm in algorithms:
        print(f"Running visualize_map.py for algorithm: {algorithm}")
        process = run_visualize_map(algorithm, house_filename)
        processes.append(process)
    
    # Wait for all processes to complete
    for process in processes:
        process.wait()

if __name__ == '__main__':
    if len(sys.argv) != 2:
        print("Usage: python script_name.py <house_filename>")
        sys.exit(1)
    
    house_filename = sys.argv[1]
    output_dir = '../output'  # You can change this to the desired output directory
    main(house_filename, output_dir)
