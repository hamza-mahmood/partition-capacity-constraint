#ifndef TRAFFIC_H_INCLUDED
#define TRAFFIC_H_INCLUDED

#include<iostream>

#include "instance_def.h"

int in_out_node_traffic(const vector<int> &nodes, const Instance &instance);
int intra_traffic(const int p, const Instance &instance);
int in_out_traffic(const int p, const Instance &instance);
int total_traffic(const Instance &instance);
int total_intra_traffic(const Instance &instance);
int total_inter_traffic(const Instance &instance);

// UTILS
void nodes_in_partition(const int p, const Instance &instance,
                        vector<int> *nodes);
pair<int, int> proper_edge(const int i, const int j);
bool is_partition_empty(const Instance &instance, const int p);

#endif
