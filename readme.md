Contributors: Ohad Agadi 209639780, Alexander Beckerman 207011180 

~~~~~~~~~~~~~

In this assignment, we followed the assignment guidelines and API. We create a MySimulator class to represent the simulator tasked with running the program.
The simulator creates and initializes instances of class ConfigInfo, which parses the input file, and class Robot which is responsible for running the algorithm.

To make our algorithm smarter and deterministic, we implemented an algorithm that uses DFS to explore and clean the house. The algorithm remembers a DFS state so that it can calcualate the next move between each step function call. The algorithm also uses BFS, to efficiently return to the charging station as well as to return to the last non-explored area of the map to continue the DFS run.

~~~~~~~~~~~~~

HOW TO RUN THE PROGRAM:

The input file should be located inside the input/ directory. To use the shell script to run the program, use 
    $ ./scripts/run.sh <input file name> 
    
This will build the project and run the main program. If you want to run the program yourself, run the following commands from the exercise directory:
$ mkdir build
$ cd build
$ cmake ..
$ make
$ d src/main
$ ./main <input file name>

To use the shell script to run the program with visualization of the robots path, use
    $ ./scripts/run_visualization.sh <input file name>

This script will build the project, run the main program and then run a python program to visualize the robot's movements and actions.

Note: To see the animation, make sure you have the matplotlib python package installed. (if not, install using 'pip install matplotlib').
If you get an error "UserWarning: Matplotlib is currently using agg...", install tk-inter package using 'sudo apt-get install python3-tk'.


The relavent output file will be generated inside the output/ folder, named output_<input file name>.