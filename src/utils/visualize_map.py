import matplotlib.pyplot as plt
import numpy as np


def read_input_file(file_path):
    matrix = []
    counter = 0
    with open(file_path, 'r') as file:
        for line in file:
            # skip first line:
            if counter == 0:
                counter += 1
                continue
            row = list(map(int, line.strip().split()))
            matrix.append(row)
    
     # Add a border of walls (-2) around the original matrix
    rows = len(matrix)
    cols = len(matrix[0])
    
    # Create a new matrix with an extra border of -2s (walls)
    bordered_matrix = [[-2] * (cols + 2)]
    for row in matrix:
        bordered_matrix.append([-2] + row + [-2])
    bordered_matrix.append([-2] * (cols + 2))
    
    return bordered_matrix

def draw_map(matrix, robot_position=None):
    # Create a color map for the different values
    cmap = plt.cm.Blues
    norm = plt.Normalize(vmin=-1, vmax=9)

    # Define specific colors for walls and charging stations
    color_map = {-2: 'black', -1: 'green'}

    # Create the plot
    fig, ax = plt.subplots()
    for row in range(len(matrix)):
        for col in range(len(matrix[row])):
            value = matrix[row][col]
            if value in color_map:
                color = color_map[value]
            else:   
                color = cmap(norm(value))
            rect = plt.Rectangle([col, len(matrix) - row - 1], 0.8, 0.8, color=color)
            ax.add_patch(rect)
    if robot_position:
        robot_row, robot_col = robot_position[0], robot_position[1]
        robot_y = len(matrix) - robot_row - 1 + 0.4
        robot_x = robot_col + 0.4
        circle = plt.Circle((robot_x, robot_y), 0.3, color='red')
        ax.add_patch(circle)

    # Set the limits and aspect ratio
    ax.set_xlim(0, len(matrix[0]))
    ax.set_ylim(0, len(matrix))
    ax.set_aspect('equal')

    # Remove the ticks
    ax.set_xticks([])
    ax.set_yticks([])

    plt.title('Map Representation')
    plt.show()

def main():
    file_path = '../tests/input.txt' # Adjust this path to your input file location
    matrix = read_input_file(file_path)
    draw_map(matrix, [1,2])

if __name__ == "__main__":
    main()
