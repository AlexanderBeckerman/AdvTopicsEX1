Contributors: Ohad Agadi 209639780, Alexander Beckerman 207011180 

The added pdf file contains an explanation of our approach to the solution. Briefly, our implemenation is the following: the user passes the input file path to the main program, which creates a config class that contains all the information about the given map. Then, this info is represented by the robot's sensors, and is used by the robot and algorithm classes to query and learn about the robot's surroundings. Inside the robot class is the loop where the robot queries the algorithm each time for his next step until we reached a stopping point in the program.

The input file will be in the following format:
The first line will hold the max_battery_steps and max_steps respectively separated by a space. After that the map will be represented by a matrix of integers. Each integer will be separated by a space.
-2 will represent a wall.
-1 will represent a charging station.
0-9 will represent the cleanliness of the cell, 0 being the cleanest.

The output file is of the following format:
The output file for input_a.txt will be called output_input_a.txt and be placed inside the output folder. It will show all the locations the robot passed through, represented by the corresponding row and column in the input matrix. At the end we added the needed information such as number of steps, and the exit status.