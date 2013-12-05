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
    multimap<const int, int> neighbours_of_node; // node, neighbours
    map<const pair<int, int>, int> traffic_of_edge; // maximum n*(n-1)/2 edges
};


//int min_node_in(const vector<bool> &partition);
//bool is_well_formed(const vector<vector<bool>> &partitions);
bool is_coherent(const vector<vector<bool>> &partitions);
bool is_over_capacity(const Instance &instance);
//void nodes_in_partition(const int p, const Instance &instance, 
//                        vector<int> *nodes);
//int in_out_node_traffic(const vector<int> &nodes, const Instance &instance);
int intra_traffic(const int p, const Instance &instance);
int in_out_traffic(const int p, const Instance &instance);
int total_traffic(const Instance &instance);
int total_intra_traffic(const Instance &instance);
int total_inter_traffic(const Instance &instance);
//void move_all_nodes(const int from, const int to,
//                vector<vector<bool>> *partitions);
//void set_nodes(const vector<int> nodes, const int partition,
//               vector<vector<bool>> *partitions);

void create_random_neighbour(const Instance &instance,
                             Instance *neighbour);

bool is_well_formed(const vector<vector<bool>> &partitions);

void coherence_step(vector<vector<bool>> *partitions);

void read_instance(ifstream &instance_file,
                   Instance *instance);

void show_instance(const Instance &instance); 

void mock_initialize_partitions(Instance *test_instance);
void naive_initialize_partitions(Instance *instance);
void greedy_initialize_partitions(Instance *instance);

#endif
