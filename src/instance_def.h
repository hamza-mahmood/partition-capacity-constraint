#ifndef INSTANCE_DEF_H_INCLUDED
#define INSTANCE_DEF_H_INCLUDED

#include <vector>
#include <map>

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

#endif
