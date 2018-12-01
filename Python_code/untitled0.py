#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Created on Fri Nov 30 19:31:28 2018

@author: Nuria
"""

        cell = m1.cells[0]     # Set starting point    #cell = random.choice(m1.cells)
        movement_dir = 's'       # Set starting direction
        n_visited_cells = 1
        cell_stack = []
        cell_stack.append(cell)        
        cell_stack_max = []
        cell_stack_max_wall = []
        while (n_visited_cells < len(m1.cells)) or (cell == m1.cells[0]):
            # Find viable wall using right wall follower
            [x,y,movement_dir] = m1.right_wall(cell,movement_dir)
            
            # Move to next cell, adjust stack and check for longest route
            cell = m1[x,y]       
            for i in reversed(range(len(cell_stack))):
                if cell_stack[i]==cell:
                    cell_stack.pop() 
                    break
                if i==0:
                    cell_stack.append(cell)
                    n_visited_cells += 1 
                    # Track longest route
                    if (len(cell_stack)>len(cell_stack_max)):
                        cell_stack_max = cell_stack[:]                    
                    # Track longest route that ends on opposite walls
                    if (len(cell_stack)>len(cell_stack_max_wall) and (cell.x==m1.width-1 or cell.y==m1.height-1)):
                        cell_stack_max_wall = cell_stack[:]



        Longest_segment = 0
        for i0 in range(m1.height):
            Segment_length = 0
            for i1 in range(m1.width): 
                Segment_length += 1
                if E in m1[i1,i0].walls:
                    Segment_length = 0
                if Segment_length>Longest_segment:
                    Longest_segment=Segment_length
        for i0 in range(m1.width):
            Segment_length = 0
            for i1 in range(m1.height): 
                Segment_length += 1
                if S in m1[i0,i1].walls:
                    Segment_length = 0
                if Segment_length>Longest_segment:
                    Longest_segment=Segment_length