import matplotlib.pyplot as plt
import numpy as np
import matplotlib.animation as animation
import sys
import os

charging_station = []
def read_input_file(file_path):
    matrix = []
    max_cols = 0
    counter = 0
    with open(file_path, 'r') as file:
        for line in file:
            if counter == 0:
                charging_station.append(list(map(int, line.strip().split())))
                charging_station[0][0] += 1
                charging_station[0][1] += 1
                counter += 1
                continue
            row = list(map(int, line.strip().split()))
            matrix.append(row)
    
     # Determine the maximum number of columns
    max_columns = max(len(row) for row in matrix)
    
    # Create a uniform matrix with each row having the same number of columns
    for row in matrix:
        while len(row) < max_columns:
            row.append(-2)
    return wrap_matrix_with_layer(matrix)

def wrap_matrix_with_layer(matrix):
    rows = len(matrix)
    cols = len(matrix[0]) if rows > 0 else 0
    new_rows = rows + 2
    new_cols = cols + 2
    
    # Create a new matrix filled with -2's
    wrapped_matrix = [[-2 for _ in range(new_cols)] for _ in range(new_rows)]
    
    # Copy the original matrix to the center of the new matrix
    for i in range(rows):
        for j in range(cols):
            wrapped_matrix[i + 1][j + 1] = matrix[i][j]
    
    return wrapped_matrix

def read_moves_file(moves_file_path):
    moves = charging_station
    with open(moves_file_path, 'r') as file:
        for line in file:
            row, col, battery_level, steps_left = map(int, line.strip().split())
            moves.append((row+1, col+1, battery_level, steps_left))
    return moves


def draw_map(ax, matrix):
    
    patches = []
    texts = []
    # Create a color map for the different values
    cmap = plt.cm.Blues
    norm = plt.Normalize(vmin=0, vmax=9)

    # Define specific colors for walls and charging stations
    color_map = {-2: 'black', -1: 'green'}

    # Create the plot
    for row in range(len(matrix)):
        row_patches = []
        row_texts = []
        for col in range(len(matrix[row])):
            value = matrix[row][col]
            if value in color_map:
                color = color_map[value]
            else:   
                color = cmap(norm(value))
            rect = plt.Rectangle([col, len(matrix) - row - 1], 0.8, 0.8, color=color)
            ax.add_patch(rect)
            row_patches.append(rect)
            
            if value >= 0:
                text = ax.text(col + 0.4, len(matrix) - row - 1 + 0.4, str(value), ha='center', va='center', fontsize=10, color='black')
                row_texts.append(text)
            else:
                row_texts.append(None)
            

        patches.append(row_patches)
        texts.append(row_texts)

    return patches, texts

def animate_robot(moves, matrix):
    fig, ax = plt.subplots()
    patches, texts = draw_map(ax, matrix)
    # Create the robot as a red circle
    circle = plt.Circle((0, 0), 0.3, color='red')
    ax.add_patch(circle)

    def init():
        circle.set_visible(False)
        return circle,

    def update(frame):
        row, col, battery_lvl, steps_left = moves[frame]
        stayed = False
        if row == moves[frame-1][0] and col == moves[frame-1][1]:
            stayed = True
        robot_y = len(matrix) - row - 1 + 0.4
        robot_x = col + 0.4
        circle.set_center((robot_x, robot_y))
        circle.set_visible(True)

         # Update the color of the grid cell the robot is on
        if matrix[row][col] >= 0 and stayed:  # Only update cells that are not walls or charging stations
            matrix[row][col] = max(matrix[row][col] - 1, 0)  # Increase cleanliness level
            cmap = plt.cm.Blues
            norm = plt.Normalize(vmin=0, vmax=9)
            patches[row][col].set_color(cmap(norm(matrix[row][col])))
            texts[row][col].set_text(str(matrix[row][col]))

        battery_text.set_text(f'Battery: {battery_lvl}')
        steps_text.set_text(f'Steps Left: {steps_left}')
        return circle,
    
    battery_text = ax.text(1, len(matrix) + 0.5, f'Battery:{moves[0][2]}', ha='right', va='bottom')
    steps_text = ax.text(1, len(matrix) + 0.5, f'Steps Left:{moves[0][3]}', ha='right', va='top')
    ax.set_ylim(0, len(matrix) + 1)  # Adjust as necessary to ensure visibility
    ani = animation.FuncAnimation(fig, update, frames=range(len(moves)), init_func=init, blit=False, repeat=False, interval=100)

    # Set the limits and aspect ratio
    ax.set_xlim(0, len(matrix[0]))
    ax.set_ylim(0, len(matrix))
    ax.set_aspect('equal')

    # Remove the ticks
    ax.set_xticks([])
    ax.set_yticks([])

    plt.title('Robot Movement Animation')
    plt.show()

def animate_algorithm(algorithm_name, house_name):
    map_file_path = f'../output/{house_name}_cleaned_input.txt' # Adjust this path to your parsed map file location
    moves_file_path = f'../output/{house_name}_{algorithm_name}.moves.txt'
    if not os.path.exists(map_file_path) or not os.path.exists(moves_file_path):
        print('Error reading input files: moves File not found')
        return
    if (os.path.getsize(map_file_path) == 0) or (os.path.getsize(moves_file_path) == 0):
        print('Error reading input files: Empty file')
        return
    matrix = read_input_file(map_file_path)
    moves = read_moves_file(moves_file_path)
    animate_robot(moves, matrix)

if __name__ == "__main__":
    if len(sys.argv) != 3:
        print('Usage: python visualize_map.py <algorithm_name> <house_name>')
        sys.exit(1)
    algorithm_name = sys.argv[1]
    animate_algorithm(algorithm_name)
