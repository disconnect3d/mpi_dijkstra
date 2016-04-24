#!/usr/bin/env python
from __future__ import print_function

# Nodes connection graph:
# A -> B --\
# \--> C --+---> G
#  \--> D -/
#
# Connection costs (weights):
# A->B 2
# A->C 3
# A->D 1

# B->G 4
# C->G 2
# D->G 6

# A->B->G 6
# A->C->G 5
# A->D->G 7

# weights (row/col index means node index: A=0 index, B=1 index and so on)
# -1 means no connection
w = [
    #connection to node ...
    # A,  B,  C,  D,  G
    [-1,  2,  3,  1, -1],
    [-1, -1, -1, -1,  4],
    [-1, -1, -1, -1,  2],
    [-1, -1, -1, -1,  6],
    [-1, -1, -1, -1, -1]
]

inf = 99999

def is_neighbour(current, target):
    return w[current][target] != -1

def dijkstra_search(nodes, initial_node, goal_node):
    nodes_count = len(nodes)

    distances = [inf] * nodes_count
    distances[0] = 0
    prev_nodes = [inf] * nodes_count
        
    unvisited = set(nodes)
    current_node = initial_node

    while True:
        print("Checking neighbours of %d node" % current_node)
        for node in nodes:
            if node not in unvisited:    # if we have checked that node, don't check it again
                print("Node %d already visited, checking another neighbour..." % node)
                continue

            if is_neighbour(current_node, node):
                print("Node %d is neighbour of %d" % (node, current_node))
                node_distance = w[current_node][node]
                
                total_cost_to_node = distances[current_node] + w[current_node][node]
                
                if total_cost_to_node < distances[node]:
                    distances[node] = total_cost_to_node
                    prev_nodes[node] = current_node
                    print("New total cost is less then the old, replacing")
        
        if current_node == goal_node:
            print("Found goal node!")
            print(distances)
            print(prev_nodes)
            break
        
        unvisited.remove(current_node)
        
        min_distance = inf
        next_node = -1
        for node, total_cost in enumerate(distances):
            if node in unvisited and total_cost < min_distance:
                min_distance = total_cost
                next_node = node
        
        current_node = next_node
        
        if next_node == -1:
            raise Exception("Path not found")
    
initial_node = 0
goal_node = 4
nodes = range(len(w))

dijkstra_search(nodes, initial_node, goal_node)

