
import pandas as pd
import random

width = 8;
height = 8;
Initial_cell_x = 1
Initial_cell_y = 0
visited_num = 1

x=[]; y=[]; N=[]; S=[]; E=[]; W=[]
for i0 in range(width):
    for i1 in range(height):
        x.append(i0)    # x location
        y.append(i1)    # y location
        N.append(1)     # North wall
        S.append(1)     # South wall
        E.append(1)     # East wall
        W.append(1)     # West wall
        if (i0==Initial_cell_x and i1==Initial_cell_y):
            current_cell = len(x)-1
            cell_stack = len(x)-1


while (visited_num < width*height):
    # Check neighbors
    neighbor = []
    x1 = x[current_cell]
    y1 = y[current_cell]
    for new_x, new_y in [(x1, y1 - 1), (x1, y1 + 1), (x1 - 1, y1), (x1 + 1, y1)]:
        if (0<=new_x<width and 0<=new_y<height):
            if neighbor==[]:
                matches = [i for i,x1 in enumerate(x) if x1==new_x]    
                matches = [x1 for x1 in x if x1==new_x]
                x.index(1)    
                neighbor = 
                
            else:
                neighbor.append(Maze_cells.loc[(Maze_cells['x'] == new_x) & (Maze_cells['y'] == new_y)])
    


import pandas as pd
import random

width = 4;
height = 4;
Initial_cell = [1,0];  
next_cell = Initial_cell

visited_num = 1;
columns1 = ["x","y","N","S","E","W"]
cell1 = []
cell2 = [Initial_cell + [1,1,1,1]]
for i0 in range(width):
    for i1 in range(height):
        cell1.append([i0,i1,1,1,1,1])
Maze_cells = pd.DataFrame(cell1, columns=columns1)
neighbor_select = Maze_cells.index[(Maze_cells['x'] == Initial_cell[0]) & (Maze_cells['y'] == Initial_cell[1])].values
neighbor_select = neighbor_select[0]
cell_stack = [neighbor_select]

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

   def g(x, y):
        """
        Returns True if there is a wall at (x, y). Values outside the valid
        range always return false.

        This is a temporary helper function.
        """
        if 0 <= x < len(matrix[0]) and 0 <= y < len(matrix):
            return matrix[y][x] != ' '
        else:
            return False


FIX FROM HERE


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

            connections = set((N, S, E, W))
            if not g(x, y + 1): connections.remove(S)
            if not g(x, y - 1): connections.remove(N)
            if not g(x + 1, y): connections.remove(E)
            if not g(x - 1, y): connections.remove(W)

            str_connections = ''.join(sorted(connections))
            # Note we are changing the matrix we are reading. We need to be
            # careful as to not break the `g` function implementation.
            matrix[y][x] = Maze.UNICODE_BY_CONNECTIONS[str_connections]

    # Simple double join to transform list of lists into string.
    return '\n'.join(''.join(line) for line in matrix) + '\n'








    def __repr__(self):
        """
        Returns an Unicode representation of the maze. Size is doubled
        horizontally to avoid a stretched look. Example 5x5:

        ┌───┬───────┬───────┐
        │   │       │       │
        │   │   ╷   ╵   ╷   │
        │   │   │       │   │
        │   │   └───┬───┘   │
        │   │       │       │
        │   └───────┤   ┌───┤
        │           │   │   │
        │   ╷   ╶───┘   ╵   │
        │   │               │
        └───┴───────────────┘
        """
        # Starts with regular representation. Looks stretched because chars are
        # twice as high as they are wide (look at docs example in
        # `Maze._to_str_matrix`).
        skinny_matrix = self._to_str_matrix()

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

        def g(x, y):
            """
            Returns True if there is a wall at (x, y). Values outside the valid
            range always return false.

            This is a temporary helper function.
            """
            if 0 <= x < len(matrix[0]) and 0 <= y < len(matrix):
                return matrix[y][x] != ' '
            else:
                return False

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

                connections = set((N, S, E, W))
                if not g(x, y + 1): connections.remove(S)
                if not g(x, y - 1): connections.remove(N)
                if not g(x + 1, y): connections.remove(E)
                if not g(x - 1, y): connections.remove(W)

                str_connections = ''.join(sorted(connections))
                # Note we are changing the matrix we are reading. We need to be
                # careful as to not break the `g` function implementation.
                matrix[y][x] = Maze.UNICODE_BY_CONNECTIONS[str_connections]

        # Simple double join to transform list of lists into string.
        return '\n'.join(''.join(line) for line in matrix) + '\n'

    def randomize(self):
        """
        Knocks down random walls to build a random perfect maze.

        Algorithm from http://mazeworks.com/mazegen/mazetut/index.htm
        """
        cell_stack = []
        cell = random.choice(self.cells)
        n_visited_cells = 1

        while n_visited_cells < len(self.cells):
            neighbors = [c for c in self.neighbors(cell) if c.is_full()]
            if len(neighbors):
                neighbor = random.choice(neighbors)
                cell.connect(neighbor)
                cell_stack.append(cell)
                cell = neighbor
                n_visited_cells += 1
            else:
                cell = cell_stack.pop()

    @staticmethod
    def generate(width=20, height=10):
        """
        Returns a new random perfect maze with the given sizes.
        """
        m = Maze(width, height)
        m.randomize()
        return m
                    

M1 = Maze(4,4)
M1.randomize()
M1.cells[1]

M1_print = ._to_str_matrix()

M1.__repr__
M1_print.__repr__
"""



