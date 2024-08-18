import matplotlib.pyplot as plt
import numpy as np
import matplotlib.animation as animation
import sys
import os
import re

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

charging_station = []
scores = []
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
    moves = charging_station.copy()
    counter = 0
    with open(moves_file_path, 'r') as file:
        for line in file:
            if counter == 0:
                scores.append(int(line.strip()))
                counter += 1
                continue
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

def animate_robot_multiple(moves_lists, matrix, house_name, algorithm_names, scores):
    num_grids = len(moves_lists)
    fig, axes = plt.subplots(1, num_grids, figsize=(5 * num_grids, 5))
    
    if num_grids == 1:
        axes = [axes]  # Ensure axes is iterable even for a single subplot
    
    # Make a separate copy of the matrix for each grid
    matrix_copies = [np.copy(matrix) for _ in range(num_grids)]
    animations = []
    
    circles = []
    battery_texts = []
    steps_texts = []
    score_texts = []
    patch_lists = []
    text_lists = []

    # Initialize all subplots and prepare for animation
    for i, (moves, ax, algorithm_name, matrix_copy) in enumerate(zip(moves_lists, axes, algorithm_names, matrix_copies)):
        patches, texts = draw_map(ax, matrix_copy)
        patch_lists.append(patches)
        text_lists.append(texts)

        circle = plt.Circle((0, 0), 0.3, color='red')
        ax.add_patch(circle)
        circles.append(circle)
        
        battery_text = ax.text(1, len(matrix_copy) + 2.5, f'Battery:{moves[0][2]}', ha='right', va='bottom')
        steps_text = ax.text(1, len(matrix_copy) + 2.5, f'Steps Left:{moves[0][3]}', ha='right', va='top')
        battery_texts.append(battery_text)
        steps_texts.append(steps_text)

        # Initialize score text but leave it empty initially
        score_text = ax.text(1, len(matrix_copy) + 1.8, '', ha='right', va='top')
        score_texts.append(score_text)

        ax.set_xlim(0, len(matrix_copy[0]))
        ax.set_ylim(0, len(matrix_copy))
        ax.set_aspect('equal')
        ax.set_xticks([])
        ax.set_yticks([])
        ax.set_title(f'{house_name} - {algorithm_name}')

    # Determine the subplot with the highest score
    max_score_index = np.argmax(scores)

    # Function to update all animations simultaneously
    def update_all(frame):
        for i, (moves, circle, battery_text, steps_text, score_text, matrix_copy, patches, texts) in enumerate(
                zip(moves_lists, circles, battery_texts, steps_texts, score_texts, matrix_copies, patch_lists, text_lists)):
            if frame < len(moves):
                row, col, battery_lvl, steps_left = moves[frame]
                stayed = False
                if frame > 0 and row == moves[frame-1][0] and col == moves[frame-1][1]:
                    stayed = True
                robot_y = len(matrix_copy) - row - 1 + 0.4
                robot_x = col + 0.4
                circle.set_center((robot_x, robot_y))
                circle.set_visible(True)
                
                if matrix_copy[row][col] >= 0 and stayed:  # Update cells that are not walls or charging stations
                    matrix_copy[row][col] = max(matrix_copy[row][col] - 1, 0)  # Increase cleanliness level
                    cmap = plt.cm.Blues
                    norm = plt.Normalize(vmin=0, vmax=9)
                    patches[row][col].set_color(cmap(norm(matrix_copy[row][col])))
                    texts[row][col].set_text(str(matrix_copy[row][col]))

                battery_text.set_text(f'Battery: {battery_lvl}')
                steps_text.set_text(f'Steps Left: {steps_left}')
                
                # Show the score at the end of the animation
                if frame == len(moves) - 1:
                    score_text.set_text(f'Score: {scores[i]}')

            # After the last frame, draw a green rectangle around the best-performing algorithm
            if frame == max(len(moves) for moves in moves_lists) - 1:
                if i == max_score_index:
                    rect = plt.Rectangle((0, 0), 1, 1, linewidth=3, edgecolor='green', facecolor='none',
                                         transform=ax.transAxes, clip_on=False)
                    ax.add_patch(rect)
        
        return circles

    ani = animation.FuncAnimation(
        fig, update_all, frames=max(len(moves) for moves in moves_lists), blit=False, repeat=False, interval=150
    )

    plt.show()

def animate_algorithm(algorithm_names, house_name):
    map_file_path = f'../output/{house_name}_cleaned_input.txt' # Adjust this path to your parsed map file location
    all_moves = []
    
    if not os.path.exists(map_file_path) or os.path.getsize(map_file_path) == 0:
        print('Error reading input files: Map File not found or empty')
        return
    matrix = read_input_file(map_file_path)
    for algorithm_name in algorithm_names:
        moves_file_path = f'../output/{house_name}_{algorithm_name}_moves.txt'
        if not os.path.exists(moves_file_path):
            print('Error reading input files: moves File not found')
            return
        if (os.path.getsize(moves_file_path) == 0):
            print('Error reading input files: Empty file')
            return
        all_moves.append(read_moves_file(moves_file_path))
        
    
    animate_robot_multiple(all_moves, matrix, house_name, algorithm_names, scores)

if __name__ == "__main__":
    if len(sys.argv) != 2:
        print('Usage: python visualize_map.py <house_name>')
        sys.exit(1)
    
    house_name = sys.argv[1]
    algorithm_names = find_algorithm_files(house_name, '../output')
    animate_algorithm(algorithm_names, house_name)
