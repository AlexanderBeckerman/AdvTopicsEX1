import subprocess

# Paths to your Python files
python_file_1 = "visualize_map.py"

# Command line arguments for each file
args_for_file_1 = ["SDFS"]  # Replace with your actual arguments
args_for_file_2 = ["SmartAlgorithm"]  # Replace with your actual arguments

# Run the first Python file with arguments in a new shell
process_1 = subprocess.Popen(['python', python_file_1] + args_for_file_1, shell=True)

# Run the second Python file with arguments in a new shell
process_2 = subprocess.Popen(['python', python_file_1] + args_for_file_2, shell=True)

# Wait for both processes to complete
process_1.wait()
process_2.wait()

print("Both Python scripts have completed.")
