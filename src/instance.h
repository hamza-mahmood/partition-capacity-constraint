#ifndef INSTANCE_H_INCLUDED
#define INSTANCE_H_INCLUDED

#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <numeric>
#include "assert.h"

using namespace std;

struct Instance {
    int max_partition_traffic;
    int node_count;
    int edge_count;
    // vector<bool> ~ dynamic bitset
    vector<vector<bool>> partitions; // n partitions * n nodes 
    map<const int, pair<int, int>> edges; // edge index, (node i , node j)
    map<const int, pair<int, int>> reverse_edges; // edge index, (node j, node i)
    // TODO: above is friggin' ugly -> maybe go back to
    // multimap<const int, pair<int, int>> edges_of_node; // node, (node, extremity)
    // or
    // multimap<const int, int> neighbours_of_node; // node, neighbour
    //
    // TODO: explore what could be done with an adjacency matrix to store edges
    // traverse adjacency matrix
    //      if cell (i, j) is true
    //          make_pair(i, j) is an edge
    multimap<const int, pair<int, int>> edges_of_partition; // partition, edges
    map<const pair<int, int>, int> traffic_of_edge; // maximum n*(n-1)/2 edges
};

void create_random_neighbour(const Instance &instance,
                             Instance *neighbour);

bool is_coherent(const vector<vector<bool>> &partitions);

void coherence_step(vector<vector<bool>> *partitions);

void read_instance(ifstream &instance_file,
                   Instance *instance);

void show_instance(const Instance &instance); 

void mock_initialize_partitions(Instance *test_instance);

#endif
