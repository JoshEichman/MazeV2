
# -*- coding: utf-8 -*-
import random

# Easy to read representation for each cardinal direction.
N, S, W, E = ('n', 's', 'w', 'e')

class Cell(object):
    """
    Class for each individual cell. Knows only its position and which walls are
    still standing.
    """
    def __init__(self, x, y, walls):
        self.x = x
        self.y = y
        self.walls = set(walls)

    def __repr__(self):
        # <15, 25 (es  )>
        return '<{}, {} ({:4})>'.format(self.x, self.y, ''.join(sorted(self.walls)))

    def __contains__(self, item):
        # N in cell
        return item in self.walls

    def is_full(self):
        """
        Returns True if all walls are still standing.
        """
        return len(self.walls) == 4

    def _wall_to(self, other):
        """
        Returns the direction to the given cell from the current one.
        Must be one cell away only.
        """
        assert abs(self.x - other.x) + abs(self.y - other.y) == 1, '{}, {}'.format(self, other)
        if other.y < self.y:
            return N
        elif other.y > self.y:
            return S
        elif other.x < self.x:
            return W
        elif other.x > self.x:
            return E
        else:
            assert False

    def connect(self, other):
        """
        Removes the wall between two adjacent cells.
        """
        other.walls.remove(other._wall_to(self))
        self.walls.remove(self._wall_to(other))

       
class Maze(object):
    """
    Maze class containing full board and maze generation algorithms.
    """

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

    def __init__(self, width=20, height=10):
        """
        Creates a new maze with the given sizes, with all walls standing.
        """
        self.width = width
        self.height = height
        self.cells = []
        for y in range(self.height):
            for x in range(self.width):
                self.cells.append(Cell(x, y, [N, S, E, W]))

    def __getitem__(self, index):
        """
        Returns the cell at index = (x, y).
        """
        x, y = index
        if 0 <= x < self.width and 0 <= y < self.height:
            return self.cells[x + y * self.width]
        else:
            return None

    def neighbors(self, cell):
        """
        Returns the list of neighboring cells, not counting diagonals. Cells on
        borders or corners may have less than 4 neighbors.
        """
        x = cell.x
        y = cell.y
        for new_x, new_y in [(x, y - 1), (x, y + 1), (x - 1, y), (x + 1, y)]:
            neighbor = self[new_x, new_y]
            if neighbor is not None:
                yield neighbor
    
    def right_wall(self, cell, move_dir):
        """
        Find next open cell along right wall and return x,y adjustment to get there
        """
        # Find next open cell (backwards for dead ends)
        i=[]
        if (move_dir == N):             
            for i in [E,N,W,S]:
                if i not in cell: break
        elif (move_dir == S):
            for i in [W,S,E,N]:
                if i not in cell: break
        elif (move_dir == E):
            for i in [S,E,N,W]:
                if i not in cell: break
        elif (move_dir == W):
            for i in [N,W,S,E]:
                if i not in cell: break 
        # Return next x and y coordinates
        x = cell.x
        y = cell.y
        if   i==N: return [x,y-1,i] 
        elif i==S: return [x,y+1,i] 
        elif i==E: return [x+1,y,i] 
        elif i==W: return [x-1,y,i] 
    
    def go_to_path_end(self,cell1,movement_dir):
        # Runs through dead end to find length    
        if   movement_dir==N: cell = self[cell1.x,cell1.y-1] 
        elif movement_dir==S: cell = self[cell1.x,cell1.y+1] 
        elif movement_dir==E: cell = self[cell1.x+1,cell1.y] 
        elif movement_dir==W: cell = self[cell1.x-1,cell1.y] 
        dead_end_stack = []
        dead_end_stack.append(cell1)
        dead_end_stack.append(cell)                    
        n_visited_cells = 1
        while (dead_end_stack != [cell1]):
            # Find viable wall using right wall follower
            [x,y,movement_dir] = self.right_wall(cell,movement_dir)                        
            # Move to next cell, adjust stack and check for longest route
            cell = self[x,y]       
            for i in reversed(range(len(dead_end_stack))):
                if dead_end_stack[i]==cell:
                    dead_end_stack.pop() 
                    break
                if i==0:
                    dead_end_stack.append(cell)
                    n_visited_cells += 1 
        return n_visited_cells
                
                                
    def _to_str_matrix(self):
        """
        Returns a matrix with a pretty printed visual representation of this
        maze. Example 5x5:

        OOOOOOOOOOO
        O       O O
        OOO OOO O O
        O O   O   O
        O OOO OOO O
        O   O O   O
        OOO O O OOO
        O   O O O O
        O OOO O O O
        O     O   O
        OOOOOOOOOOO
        """
        str_matrix = [['O'] * (self.width * 2 + 1)
                      for i in range(self.height * 2 + 1)]

        for cell in self.cells:
            x = cell.x * 2 + 1
            y = cell.y * 2 + 1
            str_matrix[y][x] = ' '
            if N not in cell and y > 0:
                str_matrix[y - 1][x + 0] = ' '
            if S not in cell and y + 1 < self.width:
                str_matrix[y + 1][x + 0] = ' '
            if W not in cell and x > 0:
                str_matrix[y][x - 1] = ' '
            if E not in cell and x + 1 < self.width:
                str_matrix[y][x + 1] = ' '

        return str_matrix

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
        cell = self.cells[0]     # Set starting point    #cell = random.choice(self.cells)
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

    
    def adjust_maze(self): 
        """
        Steps for reshaping maze
        1. randomly pick several segments (even number) (Use maze solution skeleton to quickly help find potential doors to adjust)
        2. Change door position
        3. Check properties
        4. If relaxed constraints are met then continue
        
        """
        doors = random.choice(range(1,2)) #4))     # Randomly selects the number of doors to change
        # Select cells to close
        for i in range(doors):
            change_item = random.choice(range(m1.width*m1.height)) 

            # Close 1 door (create islanded map section)
            change_item_neighbor = m1.cells[change_item].walls  # Randomly select cell outside of island to close door           
            door_close_options = [N, S, E, W]
            for i1 in change_item_neighbor:                     # find open doors
                door_close_options.remove(i1)
            door_close = random.choice(door_close_options)      # randomly select from open doors
            
            new_cell = [door_close]                             # Combine existing doors and new door
            for i1 in change_item_neighbor:
                new_cell.append(str(i1))            
            m1.cells[change_item] = Cell(m1.cells[change_item].x, m1.cells[change_item].y, sorted(new_cell))

           
            
            
            FIX FROM HERE:
                Need to capture the cells in the island created and make sure they are less than the outside
                Challenges dealing with intersections 
                A Tee could close door in a location that connects the soln path to a dead end or cuts off a dead-end)
                Need to determine which and then select matches2 appropriately
            
            
            # Find shortest path (1: to end of dead end, 2: beginning or end of maze)
            # On solution path
            if m1.map_section[change_item] in m1.Sections_on_soln_path:     
                items_down = len([x for x in m1.map_section if x < m1.map_section[change_item]]) + m1.map_length[change_item]                                                                               # cells behind location     
                items_up   = len([x for x in m1.map_section if x > (m1.map_section[change_item])]) + len([x for x in m1.map_section if x==m1.map_section[change_item]]) - (m1.map_length[change_item]+1)    # cells ahead of location
                if items_down<items_up:
                    matches2 = list(x for x in range(len(m1.map_section)) if ((m1.map_section[x] < m1.map_section[change_item]) or (m1.map_section[x]==m1.map_section[change_item] and m1.map_length[change_item]>m1.map_length[x])))  # find index of all items behind current cell        
                else:
                    matches2 = list(x for x in range(len(m1.map_section)) if ((m1.map_section[x] > m1.map_section[change_item]) or (m1.map_section[x]==m1.map_section[change_item] and m1.map_length[change_item]<m1.map_length[x])))  # find index of all items behind current cell                     
            # On dead-end
            else:                                                           
                matches2 = list(x for x in range(len(m1.map_section)) if ((m1.map_length[x]>m1.map_length[change_item]) and (m1.map_section[x]==m1.map_section[change_item])))  # find the items that follow change_item           

            """
            matches1 = list(x for x in range(len(m1.map_section)) if m1.map_section[x]==m1.map_section[change_item])  # find index of all items in a given section
            matches2 = list(x for x in matches1 if m1.map_length[x]>m1.map_length[change_item])  # find the items that follow change_item           

            [x for x in range(len(m1.map_section)) if m1.map_section[x] == m1.map_section[change_item]]
            """            
            
             
            
            # Open random door to connect islanded section
            neighbors_stack = []                                # Create list of cell indexes to connect islanded section
            for i1 in matches2:
                neighbors = [c for c in m1.neighbors(m1.cells[i1])]
                for i2 in neighbors:
                    if not i2 in list(m1.cells[x] for x in matches2+[change_item]):                       
                        neighbors_stack.append(i2.x + i2.y * m1.width)  # Add only items that are around the islanded section and not items in that section
                     
            neighbor = m1.cells[random.choice(neighbors_stack)] # Randomly select cell to connect to outside of island            
            neighbors = []                                      # Match random main branch cell (neighbor) to nearest islanded cell
            for i1 in [c for c in m1.neighbors(neighbor)]:
                if i1 in [m1.cells[c] for c in matches2]:
                    neighbors.append(i1)
            neighbors = random.choice(neighbors)     
            neighbor.connect(neighbors)
    
        
    def prop_calc(self, cell_entrance, cell_exit):
        """
        Define maze properties
          http://datagenetics.com/blog/november22015/index.html
        1. Find the longest path
        2. Count the number of dead ends
        3. Calcualte distribution of valency
        4. Determine convolution of defined path
        5. Calculate complexity
        6. Length of dead ends
        7. Maximum straight section
        """        
        # Property     [1   2   3   4   5   6   7  ]
        Weighting =    [10 ,1  ,0  ,10 ,3  ,3  ,1  ]      # Set Total_score weighting values
        Constraints = [[0.4,0  ,0  ,.65,5  ,0  ,0  ],     # Min constraint
                       [0.6,10 ,1  ,1  ,10 ,9  ,4  ]]     # Max constraint

        text1 = []    
        # 1. Use the wall follower to determine maze properties
        ###if cell_entrance is None: cell_entrance = [0,0]
        ###if cell_exit is None: cell_exit = [0,0]
        cell = self[cell_entrance] # Set starting point    #cell = random.choice(self.cells)
        movement_dir = 's'       # Set starting direction
        n_visited_cells = 1
        cell_stack = []
        cell_stack.append(cell)        
        self.cell_stack_max_wall = [] # Holds maximum length solution that touches east or south wall        
        previous_cell = []
        self.map_section = [0] * self.height * self.width     # Contains a number for the section of maze      
        self.map_length = [0] * self.height * self.width      # Contains the consecutive length of each path
        while (n_visited_cells < len(self.cells)) or (cell == self.cells[0]):
            # Find viable wall using right wall follower
            [x,y,movement_dir] = self.right_wall(cell,movement_dir)
            
            # Move to next cell, adjust stack and check for longest route
            cell = self[x,y]       
            for i in reversed(range(len(cell_stack))):
                if cell_stack[i]==cell:
                    cell_stack.pop() 
                    break
                if i==0:
                    previous_cell = cell_stack[-1].x + cell_stack[-1].y * self.width
                    if (4-len(cell_stack[-1].walls)>2):
                        self.map_section[x + y * self.width] = max(self.map_section)+1
                    else:
                        self.map_section[x + y * self.width] = self.map_section[previous_cell]
                        self.map_length[x + y * self.width] = self.map_length[previous_cell]+1   
                    cell_stack.append(cell)
                    n_visited_cells += 1 
                    
                    # Track longest route that ends on opposite walls
                    if (len(cell_stack)>len(self.cell_stack_max_wall) and (cell.x==self.width-1 or cell.y==self.height-1)):
                            if not cell_exit==cell_entrance: 
                                if (cell.x==cell_exit[0] and cell.y==cell_exit[1]):
                                    self.cell_stack_max_wall = cell_stack[:]
                            else:
                                self.cell_stack_max_wall = cell_stack[:]

        # Adjust the mapping along the solution path
        self.Sections_on_soln_path = set()      # Keep track of the different maze sections that are on the solution path
        for i in self.cell_stack_max_wall:  
            x = i.x
            y = i.y
            if i in self.cell_stack_max_wall:
                self.Sections_on_soln_path.add(self.map_section[x + y * self.width])               
            
            # Adjust section along the solution path and renumber for path length 
            """
            self.map_section[x + y * self.width] = 0
            if i==0:
                len1 = 0
            else: 
                len1 += 1
            self.map_length[x + y * self.width] = len1
            """
        text1.append('1. Solution length = '+str(len(self.cell_stack_max_wall))+', '+str(round(len(self.cell_stack_max_wall)/(self.width*self.height)*100,1))+'% of cells, Exit = ['+str(self.cell_stack_max_wall[-1].x)+','+str(self.cell_stack_max_wall[-1].y)+']')

        # 1. Count the number of dead ends
        dead_ends = 0
        for cell in self.cells:
            direction_vec = 0
            if N in cell: direction_vec += 1
            if S in cell: direction_vec += 1
            if W in cell: direction_vec += 1
            if E in cell: direction_vec += 1
            if direction_vec >= 3: dead_ends += 1        
        text1.append('2. Dead ends = '+str(dead_ends)) 
        
        # 3. Calculate the distribution of valency
        Valency = [0,0,0,0]
        for cell in self.cells:
            Valency[4-len(cell.walls)-1] += 1
        text1.append('3. Valency = 1-way:'+str(Valency[0])+' 2-way:'+str(Valency[1])+' 3-way:'+str(Valency[2])+' 4-way:'+str(Valency[3]))

        # 4 & 5. Determine convolution of the longest paths and Complexity
        Convolution = [0,0]     # [Straight, Turn]
        Complexity = 0          # Minimum number of decisions to solve maze
        for i0 in range(len(self.cell_stack_max_wall)-2):
            cell0 = self.cell_stack_max_wall[i0]
            cell1 = self.cell_stack_max_wall[i0+1]
            cell2 = self.cell_stack_max_wall[i0+2]
            if (abs(cell0.x-cell2.x)==2 or abs(cell0.y-cell2.y)==2):
                Convolution[0] += 1     # Straight
            else:
                Convolution[1] += 1     # Turn
            
            # Calculate the minium number of decisions required to arrive at the end    
            Complexity += 2-len(cell1.walls)            
        text1.append('4. Convolution = '+str(round(int(Convolution[1])/int(Convolution[0]),1))+' Turn/Straight ratio, '+str(round((int(Convolution[1])/(int(Convolution[1])+int(Convolution[0])))*100,0))+'% turns')
        text1.append('5. Complexity = '+str(Complexity))
         
        #6 Length of dead ends
        List_of_dead_ends = []
        for i0 in range(len(self.cell_stack_max_wall)-2):
            cell0 = self.cell_stack_max_wall[i0]
            cell1 = self.cell_stack_max_wall[i0+1]
            cell2 = self.cell_stack_max_wall[i0+2]
            if len(cell1.walls)<2:
                open_dir = [N,S,E,W]
                if len(cell1.walls)!=0:
                    for i1 in cell1.walls: open_dir.remove(i1)
                open_dir.remove(cell1._wall_to(cell2))
                open_dir.remove(cell1._wall_to(cell0))
                for movement_dir in open_dir:      # Runs through each available dead end
                    List_of_dead_ends.append(self.go_to_path_end(cell1,movement_dir))
            if i0==len(self.cell_stack_max_wall)-3:
                open_dir = [N,S,E,W]
                if len(cell2.walls)!=0:
                    for i1 in cell2.walls: open_dir.remove(i1)
                open_dir.remove(cell2._wall_to(cell1))
                for movement_dir in open_dir:      # Runs through each available dead end
                    #List_of_dead_ends.append(self.go_to_path_end(cell2,movement_dir))   # Consider adding back in when adjusting doors
                    List_of_dead_ends.append(100)     # Add a 100 to prevent having dead ends that require you to pass the door
        if List_of_dead_ends==[]: List_of_dead_ends=0
        text1.append('6. Max length of dead end = '+str(max(List_of_dead_ends)))
        
        # 7. Maximum straight length
        Longest_segment = 0
        for i0 in range(self.height):           # Check rows
            Segment_length = 0
            for i1 in range(self.width): 
                Segment_length += 1
                if E in self[i1,i0].walls:
                    Segment_length = 0
                if Segment_length>Longest_segment:
                    Longest_segment=Segment_length
        for i0 in range(self.width):            # Check columns
            Segment_length = 0
            for i1 in range(self.height): 
                Segment_length += 1
                if S in self[i0,i1].walls:
                    Segment_length = 0
                if Segment_length>Longest_segment:
                    Longest_segment=Segment_length
        text1.append('7. Maximum straight length = '+str(Longest_segment))
        
        Property_matrix = [len(self.cell_stack_max_wall)/(self.width*self.height),
                           dead_ends,
                           0,
                           (int(Convolution[1])/(int(Convolution[1])+int(Convolution[0]))),
                           Complexity,
                           max(List_of_dead_ends),
                           Longest_segment]

        Total_score = round(sum([a*b for a,b in zip(Property_matrix,Weighting)]),1)
        if any(Property_matrix[i] < Constraints[0][i] for i in range(len(Property_matrix))):
            Total_score = 0
        elif any(Property_matrix[i] > Constraints[1][i] for i in range(len(Property_matrix))):
            Total_score = 0   
        #print('Total Score = '+str(Total_score))
        return [Total_score,text1]
    
    
    @staticmethod
    def generate(width=20, height=10):
        """
        Returns a new random perfect maze with the given sizes.
        """
        m = Maze(width, height)
        m.randomize()
        #m.prop_calc()
        return m
    



Total_score = 0
Desired_score = 20 
count1 = 0   
while Total_score<Desired_score:   
    m1 = Maze(8,8)
    m1.randomize()
    cell_entrance = [0,0]
    cell_exit = [0,0]
    [Total_score,Output_text] = m1.prop_calc(cell_entrance,cell_exit)
    count1 += 1
    if count1 % 1000==0: 
        print(count1)

print(m1)
for i in Output_text:
    print(i)
print('Iterations required to achieve a score of '+str(Total_score)+' was '+str(count1))   
 


"""
Steps for reshaping maze
1. randomly pick several segments (even number)
2. Change door position
3. Check properties
4. If relaxed constraints are met then continue
Use maze solution skeleton to quickly help find potential doors to 

   1111    1111   
   1     1   1   1 
11111111111111111111

"""

"""
BEGIN FIXING FROM HERE

Total_score = 0
Desired_score = 20 
count1 = 0   
while Total_score<Desired_score:   
    cell_entrance = [0,0]
    cell_exit = [0,0]
    [Total_score,Output_text] = m1.prop_calc(cell_entrance,cell_exit)
    count1 += 1
    if count1 % 1000==0: 
        print(count1)

print(m1)
for i in Output_text:
    print(i)
print('Iterations required to achieve a score of '+str(Total_score)+' was '+str(count1))   
"""


