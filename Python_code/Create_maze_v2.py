
import pandas as pd
import random

width = 8;
height = 8;
Initial_cell = [0,0];  
Total_score = 0
columns1 = ["x","y","N","S","E","W"]
cell1 = []
for i0 in range(width):
    for i1 in range(height):
        cell1.append([i0,i1,1,1,1,1])
Maze_cells_init = pd.DataFrame(cell1, columns=columns1)
Initial_cell_index = Maze_cells_init.index[(Maze_cells_init['x'] == Initial_cell[0]) & (Maze_cells_init['y'] == Initial_cell[1])].values
Initial_cell_index = Initial_cell_index[0]

# Unicode character for a wall with other walls in the given directions.
UNICODE_BY_CONNECTIONS = {'ensw': '┼',
                          'ens': '├',
                          'enw': '┴',
                          'esw': '┬',
                          'es': '┌',
                          'en': '└',
                          'ew': '─',
                          'e': '╶',
                          'nsw': '┤',
                          'ns': '│',
                          'nw': '┘',
                          'sw': '┐',
                          's': '╷',
                          'n': '╵',
                          'w': '╴'} 
    
# Right wall follower (direction_to_cell:1st,2nd,3rd)
dir_order = pd.DataFrame(data={'S': ['W','S','E'], 'E': ['S','E','N'], 'N': ['E','N','W'], 'W': ['N','W','S']})
dir_change = pd.DataFrame(data={'S': [0,1], 'E': [1,0], 'N': [0,-1], 'W': [-1,0]})

    
def g(x, y):
        """
        Returns True if there is a wall at (x, y). Values outside the valid
        range always return false. This is a temporary helper function.
        """
        if 0 <= x < len(matrix[0]) and 0 <= y < len(matrix):
            return matrix[y][x] != ' '
        else:
            return False

while Total_score<76:
    visited_num = 1;
    Maze_cells = Maze_cells_init.copy()
    neighbor_select = Initial_cell_index
    cell_stack = [neighbor_select]
    next_cell = Initial_cell
        
    while (visited_num < width*height):
        # Set current cell to previous iteration's next cell
        current_cell = next_cell
        current_select = neighbor_select
        
        # Check neighbors and make sure they are on within bounds and have all their walls
        neighbor = []
        x1 = current_cell[0]
        y1 = current_cell[1]
        for new_x, new_y in [(x1, y1 - 1), (x1, y1 + 1), (x1 - 1, y1), (x1 + 1, y1)]:
                if (0<=new_x<width and 0<=new_y<height):
                    if [4]==Maze_cells[['N','S','E','W']].loc[(Maze_cells['x'] == new_x) & (Maze_cells['y'] == new_y)].sum(axis=1).tolist():  
                        neighbor.extend(Maze_cells.index[(Maze_cells['x'] == new_x) & (Maze_cells['y'] == new_y)].tolist())
        
        # If there are no eligible neighbors move back down the stack
        if not neighbor:
            neighbor_select = cell_stack.pop()
            next_cell = [int(Maze_cells['x'].loc[[neighbor_select]].values), int(Maze_cells['y'].loc[[neighbor_select]].values)]
            continue
        
        # Randomly select an eligible neighbor, find direction to neighbor and delete adjoining walls               
        neighbor_select = random.choice(neighbor)
        next_cell = [int(Maze_cells['x'].loc[[neighbor_select]].values), int(Maze_cells['y'].loc[[neighbor_select]].values)]
        if current_cell[0] < next_cell[0]:      # Next cell to the East
            Maze_cells['E'].loc[[current_select]] = 0
            Maze_cells['W'].loc[[neighbor_select]] = 0
        elif current_cell[0] > next_cell[0]:    # Next cell to the West
            Maze_cells['W'].loc[[current_select]] = 0
            Maze_cells['E'].loc[[neighbor_select]] = 0
        elif current_cell[1] < next_cell[1]:    # Next cell to the South
            Maze_cells['S'].loc[[current_select]] = 0
            Maze_cells['N'].loc[[neighbor_select]] = 0
        elif current_cell[1] > next_cell[1]:    # Next cell to the North
            Maze_cells['N'].loc[[current_select]] = 0
            Maze_cells['S'].loc[[neighbor_select]] = 0
                       
        # Move to next cell
        cell_stack.append(neighbor_select)
        visited_num += 1
          
        str_matrix = [['O'] * (width * 2 + 1)
                      for i in range(height * 2 + 1)]
        for cell_val in range(len(Maze_cells)):
            cell = Maze_cells.loc[[cell_val]]
            x = int(cell.x) * 2 + 1
            y = int(cell.y) * 2 + 1
            str_matrix[y][x] = ' '
            if int(cell.N)==0 and y > 0:
                str_matrix[y - 1][x + 0] = ' '
            if int(cell.S)==0 and y + 1 < width:
                str_matrix[y + 1][x + 0] = ' '
            if int(cell.W)==0 and x > 0:
                str_matrix[y][x - 1] = ' '
            if int(cell.E)==0 and x + 1 < width:
                str_matrix[y][x + 1] = ' '
    
        # Starts with regular representation. Looks stretched because chars are
        # twice as high as they are wide (look at docs example in
        # `Maze._to_str_matrix`).
        skinny_matrix = str_matrix
    
        # Simply duplicate each character in each line.
        double_wide_matrix = []
        for line in skinny_matrix:
            double_wide_matrix.append([])
            for char in line:
                double_wide_matrix[-1].append(char)
                double_wide_matrix[-1].append(char)
    
        # The last two chars of each line are walls, and we will need only one.
        # So we remove the last char of each line.
        matrix = [line[:-1] for line in double_wide_matrix]
    
        # Fix double wide walls, finally giving the impression of a symmetric
        # maze.
        for y, line in enumerate(matrix):
            for x, char in enumerate(line):
                if not g(x, y) and g(x - 1, y):
                    matrix[y][x - 1] = ' '
    
        # Right now the maze has the correct aspect ratio, but is still using
        # 'O' to represent walls.
       
        # Finally we replace the walls with Unicode characters depending on
        # their context.
        for y, line in enumerate(matrix):
            for x, char in enumerate(line):
                if not g(x, y):
                    continue
    
                connections = set(['n', 's', 'e', 'w'])
                if not g(x, y + 1): connections.remove('s')
                if not g(x, y - 1): connections.remove('n')
                if not g(x + 1, y): connections.remove('e')
                if not g(x - 1, y): connections.remove('w')
    
                str_connections = ''.join(sorted(connections))
                # Note we are changing the matrix we are reading. We need to be
                # careful as to not break the `g` function implementation.
                matrix[y][x] = UNICODE_BY_CONNECTIONS[str_connections]
    
        # Simple double join to transform list of lists into string.
        ##print('\n'.join(''.join(line) for line in matrix) + '\n')
        ##time.sleep(0.05)
    
    
    # Define maze properties
    #   http://datagenetics.com/blog/november22015/index.html
    # 1. Count the number of dead ends
    # 2. Find the longest path
    # 3. Calcualte distribution of valency
    # 4. Determine convolution of defined path
    # 5. Calculate complexity
    
    
    # 1. Count the number of dead ends
    dead_ends = len(Maze_cells.index[(Maze_cells.N+Maze_cells.S+Maze_cells.E+Maze_cells.W) >= 3])
    
    
    # 2. Use the wall follower to determine maze properties
    visited_num = 1
    current_cell = Initial_cell
    current_select = Initial_cell_index
    movement_dir_initial = 'S'
    movement_dir = movement_dir_initial
    cell_stack = [current_select]
    cell_stack_max = []
    cell_stack_max_wall = []
         
    while (((visited_num < width*height) or (current_select != Initial_cell_index))):
        # Find next cell
        for x1 in range(len(dir_order)):
            if (Maze_cells.loc[current_select,dir_order.loc[x1,movement_dir]]==0):
                next_movement_dir = dir_order.loc[x1,movement_dir]  # Select next direction
                next_cell = [sum(i) for i in zip(current_cell, dir_change.loc[:,next_movement_dir].tolist())]
                next_select = Maze_cells.index[(Maze_cells['x'] == next_cell[0]) & (Maze_cells['y'] == next_cell[1])].values
                next_select = next_select[0]  
                                
                # Either add or remove from the stack    
                if not any(cell_stack==next_select):
                    visited_num += 1 
                    cell_stack.append(next_select)
                else:
                    cell_stack.pop()  
                    
                # Track longest route
                if (len(cell_stack)>len(cell_stack_max)):
                    cell_stack_max = cell_stack[:]
                    
                # Track longest route that ends on opposite walls
                if (len(cell_stack)>len(cell_stack_max_wall) and (next_cell[0]==width-1 or next_cell[1]==height-1)):
                    cell_stack_max_wall = cell_stack[:]
    
                break
            else:
                if (x1==len(dir_order)-1):                          # Dead-end if no viable directions are found     
                    cell_stack.pop()                
                    next_select = cell_stack[-1]
                    next_cell = [int(Maze_cells['x'].loc[[next_select]].values), int(Maze_cells['y'].loc[[next_select]].values)]
                    if   movement_dir == 'N': next_movement_dir = 'S'
                    elif movement_dir == 'S': next_movement_dir = 'N'
                    elif movement_dir == 'E': next_movement_dir = 'W'
                    elif movement_dir == 'W': next_movement_dir = 'E'
                
        current_cell = next_cell
        current_select = next_select
        movement_dir = next_movement_dir
        #print(current_cell)
    
    
    # 3. Calculate the distribution of valency
    Maze_cells_flipped = Maze_cells.replace({0:1,1:0})
    Valency_series = Maze_cells_flipped.N+Maze_cells_flipped.S+Maze_cells_flipped.E+Maze_cells_flipped.W
    Valency = Valency_series.value_counts()   
    
    
    # 4 & 5. Determine convolution of the longest paths and Complexity
    Convolution = pd.DataFrame(data={'Straight': [0], 'Turn': [0]})
    Complexity_series = Maze_cells_flipped.N+Maze_cells_flipped.S+Maze_cells_flipped.E+Maze_cells_flipped.W
    Initial_cell2 = [Initial_cell[0]-dir_change.loc[0,movement_dir_initial],Initial_cell[1]-dir_change.loc[1,movement_dir_initial]]
    for i0 in range(len(cell_stack_max_wall)):
        if (i0==len(cell_stack_max_wall)-1):
            continue
    
        # Determine direction (straight or not)   
        if (i0==0):
            previous_cell = Initial_cell2
        else:
            previous_cell = [int(Maze_cells.loc[cell_stack_max_wall[i0-1],['x']].values), int(Maze_cells.loc[cell_stack_max_wall[i0-1],['y']].values)]    
    
        current_cell  = [int(Maze_cells.loc[cell_stack_max_wall[i0  ],['x']].values), int(Maze_cells.loc[cell_stack_max_wall[i0  ],['y']].values)]
        next_cell     = [int(Maze_cells.loc[cell_stack_max_wall[i0+1],['x']].values), int(Maze_cells.loc[cell_stack_max_wall[i0+1],['y']].values)]
            
        if (abs(next_cell[0]-previous_cell[0])==2 or abs(next_cell[1]-previous_cell[1])==2):
            Convolution.Straight += 1
        else:
            Convolution.Turn += 1
            
        # Calculate the minium number of decisions required to arrive at the end    
        if   i0==0:                          Complexity = 0
        elif i0==len(cell_stack_max_wall)-1: Complexity += 0 
        else: Complexity = Complexity+Complexity_series[cell_stack_max_wall[i0]]-2
        
        
    print('\n'.join(''.join(line) for line in matrix) + '\n')
    
    print('1. Dead ends = '+str(dead_ends))    
    print('2. Solution length = '+str(len(cell_stack_max_wall))+', '+str(round(len(cell_stack_max_wall)/(width*height)*100,1))+'% of cells, Exit = ['+str(int(Maze_cells.loc[cell_stack_max_wall[-1],['x']].values))+','+str(int(Maze_cells.loc[cell_stack_max_wall[-1],['y']].values))+']')
    print('3. Valency = 1-way:'+str(Valency[1])+' 2-way:'+str(Valency[2])+' 3-way:'+str(Valency[3]))
    print('4. Convolution = '+str(round(int(Convolution.Turn.values)/int(Convolution.Straight.values),1))+' Turn/Straight ratio, '+str(round((int(Convolution.Turn.values)/(int(Convolution.Turn.values)+int(Convolution.Straight.values)))*100,0))+'% turns')
    print('5. Complexity = '+str(Complexity))

    Property_matrix = [dead_ends,
                       len(cell_stack_max_wall)/(width*height)*10,
                       0,
                       (int(Convolution.Turn.values)/(int(Convolution.Turn.values)+int(Convolution.Straight.values)))*10,
                       Complexity]
    Weighting = [1,3,0,2,4]    
    Total_score = round(sum([a*b for a,b in zip(Property_matrix,Weighting)]),1)
    print('Total Score = '+str(Total_score))